/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*  This file is ALSO:
 *  Copyright 2001-2004 David Abrahams.
 *  Copyright 2007 Rene Rivera.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

#include "jam.h"
#include "execcmd.h"

#include "lists.h"
#include "output.h"
#include "pathsys.h"
#include "string.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <time.h>

#ifdef USE_EXECNT

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <process.h>
#include <tlhelp32.h>

/*
 * execnt.c - execute a shell command on Windows NT
 *
 * If $(JAMSHELL) is defined, uses that to formulate the actual command. The
 * default is: cmd.exe /Q/C
 *
 * In $(JAMSHELL), % expands to the command string and ! expands to the slot
 * number (starting at 1) for multiprocess (-j) invocations. If $(JAMSHELL) does
 * not include a %, it is tacked on as the last argument.
 *
 * Each $(JAMSHELL) placeholder must be specified as a separate individual
 * element in a jam variable value.
 *
 * Do not just set JAMSHELL to cmd.exe - it will not work!
 *
 * External routines:
 *  exec_cmd() - launch an async command execution.
 *  exec_wait() - wait for any of the async command processes to terminate.
 */

/* get the maximum shell command line length according to the OS */
int maxline();

/* bump intr to note command interruption */
static void onintr( int );
/* trim leading and trailing whitespace */
void string_new_trimmed( string * pResult, string const * source );
/* is the command suitable for direct execution via CreateProcessA() */
static long can_spawn( string * pCommand );
/* add two 64-bit unsigned numbers, h1l1 and h2l2 */
static FILETIME add_64(
    unsigned long h1, unsigned long l1,
    unsigned long h2, unsigned long l2 );
/* */
static FILETIME add_FILETIME( FILETIME t1, FILETIME t2 );
/* */
static FILETIME negate_FILETIME( FILETIME t );
/* convert a FILETIME to a number of seconds */
static double filetime_seconds( FILETIME t );
/* record the timing info for the process */
static void record_times( HANDLE, timing_info * );
/* calc the current running time of an *active* process */
static double running_time( HANDLE );
/* */
DWORD get_process_id( HANDLE );
/* terminate the given process, after terminating all its children first */
static void kill_process_tree( DWORD, HANDLE );
/* waits for a command to complete or time out */
static int try_wait( int timeoutMillis );
/* reads any pending output for running commands */
static void read_output();
/* checks if a command ran out of time, and kills it */
static int try_kill_one();
/* */
static double creation_time( HANDLE );
/* is the first process a parent (direct or indirect) to the second one */
static int is_parent_child( DWORD, DWORD );
/* */
static void close_alert( HANDLE );
/* close any alerts hanging around */
static void close_alerts();
/* returns a string's value buffer if not empty or 0 if empty */
static char const * null_if_empty( string const * str );
/* Reports the last failed Windows API related error message. */
static void reportWindowsError( char const * const apiName );

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int intr = 0;
static int cmdsrunning = 0;
static void (* istat)( int );


/* The list of commands we run. */
static struct
{
    string action[ 1 ];   /* buffer to hold action */
    string target[ 1 ];   /* buffer to hold target */
    string command[ 1 ];  /* buffer to hold command being invoked */

    /* Temporary command file used to execute the action when needed. */
    char * tempfile_bat;

    /* Pipes for communicating with the child process. Parent reads from (0),
     * child writes to (1).
     */
    HANDLE pipe_out[ 2 ];
    HANDLE pipe_err[ 2 ];

    string buffer_out[ 1 ];  /* buffer to hold stdout, if any */
    string buffer_err[ 1 ];  /* buffer to hold stderr, if any */

    PROCESS_INFORMATION pi;  /* running process information */
    DWORD exit_code;         /* executed command's exit code */
    int exit_reason;         /* reason why a command completed */

    /* Function called when the command completes. */
    ExecCmdCallback func;

    /* Opaque data passed back to the 'func' callback. */
    void * closure;
}
cmdtab[ MAXJOBS ] = { { 0 } };


/*
 * Execution unit tests.
 */

void execnt_unit_test()
{
#if !defined( NDEBUG )
    /* vc6 preprocessor is broken, so assert with these strings gets confused.
     * Use a table instead.
     */
    typedef struct test { char * command; int result; } test;
    test tests[] = {
        { "x", 1 },
        { "x\ny", 0 },
        { "x\n\n y", 0 },
        { "echo x > foo.bar", 0 },
        { "echo x < foo.bar", 0 },
        { "echo x \">\" foo.bar", 1 },
        { "echo x \"<\" foo.bar", 1 },
        { "echo x \\\">\\\" foo.bar", 0 },
        { "echo x \\\"<\\\" foo.bar", 0 } };
    int i;
    for ( i = 0; i < sizeof( tests ) / sizeof( *tests ); ++i )
    {
        string temp;
        string_copy( &temp, tests[ i ].command );
        assert( !!can_spawn( &temp ) == tests[ i ].result );
    }

    {
        string long_command[ 1 ];
        string_new( long_command );
        string_reserve( long_command, MAXLINE + 10 );
        long_command->size = long_command->capacity - 1;
        memset( long_command->value, 'x', long_command->size );
        long_command->value[ long_command->size ] = 0;
        assert( can_spawn( long_command ) == MAXLINE + 9 );
        string_free( long_command );
    }
#endif
}


/*
 * exec_cmd() - launch an async command execution.
 */

void exec_cmd
(
    string const * pCommand_orig,
    ExecCmdCallback func,
    void * closure,
    LIST * shell,
    char const * action,
    char const * target
)
{
    int slot;
    int raw_cmd = 0 ;
    string cmd_local[ 1 ];

    /* Find a free slot in the running commands table. */
    for ( slot = 0; slot < MAXJOBS; ++slot )
        if ( !cmdtab[ slot ].pi.hProcess )
            break;
    if ( slot == MAXJOBS )
    {
        printf( "no slots for child!\n" );
        exit( EXITBAD );
    }

    /* Trim all leading and trailing leading whitespace. */
    string_new_trimmed( cmd_local, pCommand_orig );

    /* Check to see if we need to hack around the line-length limitation. Look
     * for a JAMSHELL setting of "%", indicating that the command should be
     * invoked directly.
     */
    if ( !list_empty( shell ) &&
        !strcmp( object_str( list_front( shell ) ), "%" ) &&
        list_next( list_begin( shell ) ) == list_end( shell ) )
    {
        /* Check to see if we need to hack around the line-length limitation.
         * JAMSHELL setting of "%", indicates that the command should be invoked
         * directly if it satisfies all the spawnability criteria or using a
         * batch file and the default shell if not.
         */
        raw_cmd = can_spawn( cmd_local ) >= MAXLINE;
        shell = L0;
    }

    /* If we are not running a raw command directly, prepare a .BAT file to be
     * executed using an external shell (default or not - does not matter here).
     */
    if ( !raw_cmd )
    {
        FILE * f = 0;
        int tries = 0;

        /* Compute the name of a temp batch file if we have not used it already
         * for this command slot.
         */
        if ( !cmdtab[ slot ].tempfile_bat )
        {
            char const * tempdir = path_tmpdir();
            DWORD procID = GetCurrentProcessId();

            /* SVA - allocate 64 bytes extra just to be safe. */
            cmdtab[ slot ].tempfile_bat = BJAM_MALLOC_ATOMIC( strlen( tempdir )
                + 64 );

            sprintf( cmdtab[ slot ].tempfile_bat, "%s\\jam%d-%02d.bat", tempdir,
                procID, slot );
        }

        /* Write command to bat file. For some reason this open can fail
         * intermittently. But doing some retries works. Most likely this is due
         * to a previously existing file of the same name that happens to be
         * opened by an active virus scanner. Pointed out and fixed by Bronek
         * Kozicki.
         */
        for ( ; !f && ( tries < 4 ); ++tries )
        {
            f = fopen( cmdtab[ slot ].tempfile_bat, "w" );
            if ( !f && ( tries < 4 ) ) Sleep( 250 );
        }
        if ( !f )
        {
            printf( "failed to write command file!\n" );
            exit( EXITBAD );
        }
        fputs( cmd_local->value, f );
        fclose( f );

        if ( DEBUG_EXECCMD )
            printf( "Executing through .bat file\n" );
    }
    else
    {
        if ( DEBUG_EXECCMD )
            printf( "Executing raw command directly\n" );
    }

    /* If we are running a command directly, we already have it prepared in
     * cmd_local. Now prepare the final command-string to execute in case we are
     * using a shell. If a custom shell was defined, be prepared for % and !
     * subs. Otherwise, use stock cmd.exe.
     */
    if ( !raw_cmd )
    {
        char const * command = cmdtab[ slot ].tempfile_bat;
        char const * argv[ MAXARGC + 1 ];  /* +1 for NULL */

        if ( list_empty( shell ) )
        {
            argv[ 0 ] = "cmd.exe";
            argv[ 1 ] = "/Q/C";  /* anything more is non-portable */
            argv[ 2 ] = command;
            argv[ 3 ] = 0;
        }
        else
        {
            int i;
            char jobno[ 4 ];
            int gotpercent = 0;
            LISTITER shell_iter = list_begin( shell );
            LISTITER shell_end = list_end( shell );

            sprintf( jobno, "%d", slot + 1 );

            if ( DEBUG_EXECCMD )
            {
                printf( "Using user-specified shell: " );
                list_print( shell );
                printf( "\n" );
            }
            for ( i = 0; shell_iter != shell_end && ( i < MAXARGC );
                ++i, shell_iter = list_next( shell_iter ) )
            {
                switch ( object_str( list_item( shell_iter ) )[ 0 ] )
                {
                    case '%': argv[ i ] = command; ++gotpercent; break;
                    case '!': argv[ i ] = jobno; break;
                    default : argv[ i ] = object_str( list_item( shell_iter ) );
                }
            }

            if ( !gotpercent )
                argv[ i++ ] = command;

            argv[ i ] = 0;
        }

        if ( DEBUG_EXECCMD )
        {
            int i;
            for ( i = 0 ; argv[ i ]; ++i )
                printf( "argv[%d] = '%s'\n", i, argv[ i ] );
        }

        /* Put together the final command string we are to run. */
        {
            char const * * argp = argv;
            string_truncate( cmd_local, 0 );
            string_append( cmd_local, *(argp++) );
            while ( *argp )
            {
                string_push_back( cmd_local, ' ' );
                string_append( cmd_local, *(argp++) );
            }
        }
    }

    /* Catch interrupts whenever commands are running. */
    if ( !cmdsrunning++ )
        istat = signal( SIGINT, onintr );

    /* Start the command. */
    {
        SECURITY_ATTRIBUTES sa = { sizeof( SECURITY_ATTRIBUTES ), 0, 0 };
        SECURITY_DESCRIPTOR sd;
        STARTUPINFO si = { sizeof( STARTUPINFO ), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0 };

        /* Init the security data. */
        InitializeSecurityDescriptor( &sd, SECURITY_DESCRIPTOR_REVISION );
        SetSecurityDescriptorDacl( &sd, TRUE, NULL, FALSE );
        sa.lpSecurityDescriptor = &sd;
        sa.bInheritHandle = TRUE;

        /* Create the stdout, which is also the merged out + err, pipe. */
        if ( !CreatePipe( &cmdtab[ slot ].pipe_out[ 0 ],
            &cmdtab[ slot ].pipe_out[ 1 ], &sa, 0 ) )
        {
            reportWindowsError( "CreatePipe" );
            exit( EXITBAD );
        }

        /* Create the stdout, which is also the merged out+err, pipe. */
        if ( globs.pipe_action == 2 )
        {
            if ( !CreatePipe( &cmdtab[ slot ].pipe_err[ 0 ],
                &cmdtab[ slot ].pipe_err[ 1 ], &sa, 0 ) )
            {
                reportWindowsError( "CreatePipe" );
                exit( EXITBAD );
            }
        }

        /* Set handle inheritance off for the pipe ends the parent reads from.
         */
        SetHandleInformation( cmdtab[ slot ].pipe_out[ 0 ], HANDLE_FLAG_INHERIT,
            0 );
        if ( globs.pipe_action == 2 )
            SetHandleInformation( cmdtab[ slot ].pipe_err[ 0 ],
                HANDLE_FLAG_INHERIT, 0 );

        /* Hide the child window, if any. */
        si.dwFlags |= STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE;

        /* Set the child outputs to the pipes. */
        si.dwFlags |= STARTF_USESTDHANDLES;
        si.hStdOutput = cmdtab[ slot ].pipe_out[ 1 ];
        if ( globs.pipe_action == 2 )
        {
            /* Pipe stderr to the action error output. */
            si.hStdError = cmdtab[ slot ].pipe_err[ 1 ];
        }
        else if ( globs.pipe_action == 1 )
        {
            /* Pipe stderr to the console error output. */
            si.hStdError = GetStdHandle( STD_ERROR_HANDLE );
        }
        else
        {
            /* Pipe stderr to the action merged output. */
            si.hStdError = cmdtab[ slot ].pipe_out[ 1 ];
        }

        /* Let the child inherit stdin, as some commands assume it is
         * available.
         */
        si.hStdInput = GetStdHandle( STD_INPUT_HANDLE );

        /* Save the operation for exec_wait() to find. */
        cmdtab[ slot ].func = func;
        cmdtab[ slot ].closure = closure;
        /* No need to free action and target cmdtab[ slot ] members here as they
         * have either never been explicitly constructed before (and are thus
         * zero initialized) or have been freed when the previous command in the
         * same slot has been cleaned up.
         */
        assert( !cmdtab[ slot ].action->size );
        assert( !cmdtab[ slot ].action->value || cmdtab[ slot ].action->value ==
            cmdtab[ slot ].action->opt );
        assert( !cmdtab[ slot ].target->size );
        assert( !cmdtab[ slot ].target->value || cmdtab[ slot ].target->value ==
            cmdtab[ slot ].target->opt );
        if ( action && target )
        {
            string_copy( cmdtab[ slot ].action, action );
            string_copy( cmdtab[ slot ].target, target );
        }
        else
        {
            string_new ( cmdtab[ slot ].action );
            string_new ( cmdtab[ slot ].target );
        }
        string_copy( cmdtab[ slot ].command, pCommand_orig->value );

        /* CreateProcessA() Windows API places a limit of 32768 characters
         * (bytes) on the allowed command-line length, including a trailing
         * Unicode (2-byte) nul-terminator character.
         */
        #define MAX_RAW_COMMAND_LENGTH 32766
        if ( cmd_local->size > MAX_RAW_COMMAND_LENGTH )
        {
            printf( "Command line too long (%d characters). Maximum executable "
                "command-line length is %d.", cmd_local->size,
                MAX_RAW_COMMAND_LENGTH );
            exit( EXITBAD );
        }

        /* Create output buffers. */
        string_new( cmdtab[ slot ].buffer_out );
        string_new( cmdtab[ slot ].buffer_err );

        if ( DEBUG_EXECCMD )
            printf( "Command string to be sent to CreateProcessA(): '%s'\n",
                cmd_local->value );

        /* Run the command by creating a sub-process for it. */
        if (
            !CreateProcessA(
                NULL                    ,  /* application name               */
                cmd_local->value        ,  /* command line                   */
                NULL                    ,  /* process attributes             */
                NULL                    ,  /* thread attributes              */
                TRUE                    ,  /* inherit handles                */
                CREATE_NEW_PROCESS_GROUP,  /* create flags                   */
                NULL                    ,  /* env vars, null inherits env    */
                NULL                    ,  /* current dir, null is our       */
                                           /* current dir                    */
                &si                     ,  /* startup info                   */
                &cmdtab[ slot ].pi         /* child process info, if created */
                )
            )
        {
            reportWindowsError( "CreateProcessA" );
            exit( EXITBAD );
        }
    }

    /* Free our local command string copy. */
    string_free( cmd_local );

    /* Wait until we are under the limit of concurrent commands. Do not trust
     * globs.jobs alone.
     */
    while ( ( cmdsrunning >= MAXJOBS ) || ( cmdsrunning >= globs.jobs ) )
        if ( !exec_wait() )
            break;
}


/*
 * exec_wait()
 *  * wait and drive at most one execution completion.
 *  * waits for one command to complete, while processing the i/o for all
 *    ongoing commands.
 *
 *   Returns 0 if called when there were no more commands being executed or 1
 * otherwise.
 */

int exec_wait()
{
    int i = -1;

    /* Handle naive make1() which does not know if cmds are running. */
    if ( !cmdsrunning )
        return 0;

    /* Wait for a command to complete, while snarfing up any output. */
    do
    {
        /* Check for a complete command, briefly. */
        i = try_wait( 500 );
        /* Read in the output of all running commands. */
        read_output();
        /* Close out pending debug style dialogs. */
        close_alerts();
        /* Check if a command ran out of time. */
        if ( i < 0 ) i = try_kill_one();
    }
    while ( i < 0 );

    /* We have a command... process it. */
    --cmdsrunning;
    {
        timing_info time;
        int rstat;

        /* The time data for the command. */
        record_times( cmdtab[ i ].pi.hProcess, &time );

        /* Clear the temp file. */
        if ( cmdtab[ i ].tempfile_bat )
        {
            unlink( cmdtab[ i ].tempfile_bat );
            BJAM_FREE( cmdtab[ i ].tempfile_bat );
            cmdtab[ i ].tempfile_bat = NULL;
        }

        /* Find out the process exit code. */
        GetExitCodeProcess( cmdtab[ i ].pi.hProcess, &cmdtab[ i ].exit_code );

        /* The dispossition of the command. */
        if ( intr )
            rstat = EXEC_CMD_INTR;
        else if ( cmdtab[ i ].exit_code != 0 )
            rstat = EXEC_CMD_FAIL;
        else
            rstat = EXEC_CMD_OK;

        /* Output the action block. */
        out_action(
            null_if_empty( cmdtab[ i ].action ),
            null_if_empty( cmdtab[ i ].target ),
            null_if_empty( cmdtab[ i ].command ),
            null_if_empty( cmdtab[ i ].buffer_out ),
            null_if_empty( cmdtab[ i ].buffer_err ),
            cmdtab[ i ].exit_reason );

        /* Call the callback, may call back to jam rule land. Assume -p0 is in
         * effect so only pass buffer containing merged output.
         */
        (*cmdtab[ i ].func)( cmdtab[ i ].closure, rstat, &time,
            cmdtab[ i ].command->value, cmdtab[ i ].buffer_out->value );

        /* Clean up the command data, process, etc. */
        string_free( cmdtab[ i ].action  ); string_new( cmdtab[ i ].action  );
        string_free( cmdtab[ i ].target  ); string_new( cmdtab[ i ].target  );
        string_free( cmdtab[ i ].command ); string_new( cmdtab[ i ].command );
        if ( cmdtab[ i ].pi.hProcess   ) { CloseHandle( cmdtab[ i ].pi.hProcess   ); cmdtab[ i ].pi.hProcess   = 0; }
        if ( cmdtab[ i ].pi.hThread    ) { CloseHandle( cmdtab[ i ].pi.hThread    ); cmdtab[ i ].pi.hThread    = 0; }
        if ( cmdtab[ i ].pipe_out[ 0 ] ) { CloseHandle( cmdtab[ i ].pipe_out[ 0 ] ); cmdtab[ i ].pipe_out[ 0 ] = 0; }
        if ( cmdtab[ i ].pipe_out[ 1 ] ) { CloseHandle( cmdtab[ i ].pipe_out[ 1 ] ); cmdtab[ i ].pipe_out[ 1 ] = 0; }
        if ( cmdtab[ i ].pipe_err[ 0 ] ) { CloseHandle( cmdtab[ i ].pipe_err[ 0 ] ); cmdtab[ i ].pipe_err[ 0 ] = 0; }
        if ( cmdtab[ i ].pipe_err[ 1 ] ) { CloseHandle( cmdtab[ i ].pipe_err[ 1 ] ); cmdtab[ i ].pipe_err[ 1 ] = 0; }
        string_free( cmdtab[ i ].buffer_out ); string_new( cmdtab[ i ].buffer_out );
        string_free( cmdtab[ i ].buffer_err ); string_new( cmdtab[ i ].buffer_err );
        cmdtab[ i ].exit_code = 0;
        cmdtab[ i ].exit_reason = EXIT_OK;
    }

    return 1;
}


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*
 * For more details on Windows cmd.exe shell command-line length limitations see
 * the following MSDN article:
 *     http://support.microsoft.com/default.aspx?scid=kb;en-us;830473
 */

static int raw_maxline()
{
    OSVERSIONINFO os_info;
    os_info.dwOSVersionInfoSize = sizeof( os_info );
    GetVersionEx( &os_info );

    if ( os_info.dwMajorVersion >= 5 ) return 8191;  /* XP       */
    if ( os_info.dwMajorVersion == 4 ) return 2047;  /* NT 4.x   */
    return 996;                                      /* NT 3.5.1 */
}

int maxline()
{
    static result;
    if ( !result ) result = raw_maxline();
    return result;
}


/*
 * Creates and returns a new trimmed copy of the given source string. Returned
 * value needs to be released using string_free().
 */

void string_new_trimmed( string * pResult, string const * pSource )
{
    char const * source = pSource->value;
    int source_len;
    while ( isspace( *source ) )
        ++source;
    source_len = pSource->size - ( source - pSource->value );
    while ( ( source_len > 0 ) && isspace( source[ source_len - 1 ] ) )
        --source_len;
    string_new( pResult );
    string_append_range( pResult, source, source + source_len );
}


static void onintr( int disp )
{
    ++intr;
    printf( "...interrupted\n" );
}


/*
 * can_spawn() - If the command is suitable for execution via CreateProcessA(),
 * return a number >= the number of characters it would occupy on the
 * command-line. Otherwise, return zero. Expects the command string to have
 * already been trimmed of all leading and trailing whitespace.
 */

static long can_spawn( string * pCommand )
{
    char const * p = pCommand->value;
    char inquote = 0;

    assert( !isspace( *pCommand->value ) );
    assert( !pCommand->size || !isspace( pCommand->value[ pCommand->size - 1 ] ) );

    /* Look for newlines and unquoted I/O redirection. */
    do
    {
        p += strcspn( p, "\n\"'<>|" );
        switch ( *p )
        {
        case '\n':
            /* If our command contains newlines we can not execute it directly.
             * Note that there is no need to check for leading or trailing
             * newlines since we already assume the command string has been
             * trimmed prior to this call.
             */
            return 0;

        case '"':
        case '\'':
            if ( ( p > pCommand->value ) && ( p[-1] != '\\' ) )
            {
                if ( inquote == *p )
                    inquote = 0;
                else if ( !inquote )
                    inquote = *p;
            }
            ++p;
            break;

        case '<':
        case '>':
        case '|':
            if ( !inquote )
                return 0;
            ++p;
            break;
        }
    }
    while ( *p );

    /* Return the number of characters the command will occupy. */
    return p - pCommand->value;
}


/* 64-bit arithmetic helpers. */

/* Compute the carry bit from the addition of two 32-bit unsigned numbers. */
#define add_carry_bit( a, b ) ( (((a) | (b)) >> 31) & (~((a) + (b)) >> 31) & 0x1 )

/* Compute the high 32 bits of the addition of two 64-bit unsigned numbers, h1l1 and h2l2. */
#define add_64_hi( h1, l1, h2, l2 ) ((h1) + (h2) + add_carry_bit(l1, l2))


/*
 * Add two 64-bit unsigned numbers, h1l1 and h2l2.
 */

static FILETIME add_64
(
    unsigned long h1, unsigned long l1,
    unsigned long h2, unsigned long l2
)
{
    FILETIME result;
    result.dwLowDateTime = l1 + l2;
    result.dwHighDateTime = add_64_hi( h1, l1, h2, l2 );
    return result;
}


static FILETIME add_FILETIME( FILETIME t1, FILETIME t2 )
{
    return add_64( t1.dwHighDateTime, t1.dwLowDateTime, t2.dwHighDateTime,
        t2.dwLowDateTime );
}


static FILETIME negate_FILETIME( FILETIME t )
{
    /* 2s complement negation */
    return add_64( ~t.dwHighDateTime, ~t.dwLowDateTime, 0, 1 );
}


/*
 * Convert a FILETIME to a number of seconds.
 */

static double filetime_seconds( FILETIME t )
{
    return t.dwHighDateTime * ( (double)( 1UL << 31 ) * 2.0 * 1.0e-7 ) + t.dwLowDateTime * 1.0e-7;
}


/*
 * What should be a simple conversion, turns out to be horribly complicated by
 * the defficiencies of MSVC and the Win32 API.
 */

static time_t filetime_dt( FILETIME t_utc )
{
    static int calc_time_diff = 1;
    static double time_diff;
    if ( calc_time_diff )
    {
        struct tm t0_;
        FILETIME f0_local;
        FILETIME f0_;
        SYSTEMTIME s0_;
        GetSystemTime( &s0_ );
        t0_.tm_year = s0_.wYear-1900;
        t0_.tm_mon = s0_.wMonth-1;
        t0_.tm_wday = s0_.wDayOfWeek;
        t0_.tm_mday = s0_.wDay;
        t0_.tm_hour = s0_.wHour;
        t0_.tm_min = s0_.wMinute;
        t0_.tm_sec = s0_.wSecond;
        t0_.tm_isdst = 0;
        SystemTimeToFileTime( &s0_, &f0_local );
        LocalFileTimeToFileTime( &f0_local, &f0_ );
        time_diff = filetime_seconds( f0_ ) - (double)mktime( &t0_ );
        calc_time_diff = 0;
    }
    return ceil( filetime_seconds( t_utc ) - time_diff );
}


static void record_times( HANDLE process, timing_info * time )
{
    FILETIME creation;
    FILETIME exit;
    FILETIME kernel;
    FILETIME user;
    if ( GetProcessTimes( process, &creation, &exit, &kernel, &user ) )
    {
        time->system = filetime_seconds( kernel   );
        time->user   = filetime_seconds( user     );
        time->start  = filetime_dt     ( creation );
        time->end    = filetime_dt     ( exit     );
    }
}


#define IO_BUFFER_SIZE ( 16 * 1024 )

static char ioBuffer[ IO_BUFFER_SIZE + 1 ];


static void read_pipe
(
    HANDLE   in,  /* the pipe to read from */
    string * out
)
{
    DWORD bytesInBuffer = 0;
    DWORD bytesAvailable = 0;

    do
    {
        /* check if we have any data to read */
        if ( !PeekNamedPipe( in, ioBuffer, IO_BUFFER_SIZE, &bytesInBuffer, &bytesAvailable, NULL ) )
            bytesAvailable = 0;

        /* read in the available data */
        if ( bytesAvailable > 0 )
        {
            /* we only read in the available bytes, to avoid blocking */
            if ( ReadFile( in, ioBuffer,
                bytesAvailable <= IO_BUFFER_SIZE ? bytesAvailable : IO_BUFFER_SIZE,
                &bytesInBuffer, NULL ) )
            {
                if ( bytesInBuffer > 0 )
                {
                    /* Clean up some illegal chars. */
                    int i;
                    for ( i = 0; i < bytesInBuffer; ++i )
                    {
                        if ( ( (unsigned char)ioBuffer[ i ] < 1 ) )
                            ioBuffer[ i ] = '?';
                    }
                    /* Null, terminate. */
                    ioBuffer[ bytesInBuffer ] = '\0';
                    /* Append to the output. */
                    string_append( out, ioBuffer );
                    /* Subtract what we read in. */
                    bytesAvailable -= bytesInBuffer;
                }
                else
                {
                    /* Likely read a error, bail out. */
                    bytesAvailable = 0;
                }
            }
            else
            {
                /* Definitely read a error, bail out. */
                bytesAvailable = 0;
            }
        }
    }
    while ( bytesAvailable > 0 );
}


static void read_output()
{
    int i;
    for ( i = 0; i < globs.jobs && i < MAXJOBS; ++i )
    {
        /* Read stdout data. */
        if ( cmdtab[ i ].pipe_out[ 0 ] )
            read_pipe( cmdtab[ i ].pipe_out[ 0 ], cmdtab[ i ].buffer_out );
        /* Read stderr data. */
        if ( cmdtab[ i ].pipe_err[ 0 ] )
            read_pipe( cmdtab[ i ].pipe_err[ 0 ], cmdtab[ i ].buffer_err );
    }
}


/*
 * Waits for a single child process command to complete, or the timeout,
 * whichever comes first. Returns the index of the completed command in the
 * cmdtab array, or -1.
 */

static int try_wait( int timeoutMillis )
{
    int i;
    int num_active;
    int wait_api_result;
    HANDLE active_handles[ MAXJOBS ];
    int active_procs[ MAXJOBS ];

    /* Prepare a list of all active processes to wait for. */
    for ( num_active = 0, i = 0; i < globs.jobs; ++i )
    {
        if ( cmdtab[ i ].pi.hProcess )
        {
            active_handles[ num_active ] = cmdtab[ i ].pi.hProcess;
            active_procs[ num_active ] = i;
            ++num_active;
        }
    }

    /* Wait for a child to complete, or for our timeout window to expire. */
    wait_api_result = WaitForMultipleObjects( num_active, active_handles,
        FALSE, timeoutMillis );
    if ( ( WAIT_OBJECT_0 <= wait_api_result ) &&
        ( wait_api_result < WAIT_OBJECT_0 + num_active ) )
    {
        /* Terminated process detected - return its index. */
        return active_procs[ wait_api_result - WAIT_OBJECT_0 ];
    }

    /* Timeout. */
    return -1;
}


static int try_kill_one()
{
    /* Only need to check if a timeout was specified with the -l option. */
    if ( globs.timeout > 0 )
    {
        int i;
        for ( i = 0; i < globs.jobs; ++i )
        {
            double t = running_time( cmdtab[ i ].pi.hProcess );
            if ( t > (double)globs.timeout )
            {
                /* The job may have left an alert dialog around, try and get rid
                 * of it before killing
                 */
                close_alert( cmdtab[ i ].pi.hProcess );
                /* We have a "runaway" job, kill it. */
                kill_process_tree( 0, cmdtab[ i ].pi.hProcess );
                /* And return it marked as a timeout. */
                cmdtab[ i ].exit_reason = EXIT_TIMEOUT;
                return i;
            }
        }
    }
    return -1;
}


static void close_alerts()
{
    /* We only attempt this every 5 seconds, or so, because it is not a cheap
     * operation, and we will catch the alerts eventually. This check uses
     * floats as some compilers define CLOCKS_PER_SEC as a float or double.
     */
    if ( ( (float)clock() / (float)( CLOCKS_PER_SEC * 5 ) ) < ( 1.0 / 5.0 ) )
    {
        int i;
        for ( i = 0; i < globs.jobs; ++i )
            close_alert( cmdtab[ i ].pi.hProcess );
    }
}


/*
 * Calc the current running time of an *active* process.
 */

static double running_time( HANDLE process )
{
    FILETIME creation;
    FILETIME exit;
    FILETIME kernel;
    FILETIME user;
    FILETIME current;
    if ( GetProcessTimes( process, &creation, &exit, &kernel, &user ) )
    {
        /* Compute the elapsed time. */
        GetSystemTimeAsFileTime( &current );
        return filetime_seconds( add_FILETIME( current,
            negate_FILETIME( creation ) ) );
    }
    return 0.0;
}


/* It is just stupidly silly that one has to do this. */
typedef struct PROCESS_BASIC_INFORMATION__
{
    LONG  ExitStatus;
    PVOID PebBaseAddress;
    ULONG AffinityMask;
    LONG  BasePriority;
    ULONG UniqueProcessId;
    ULONG InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION_;
typedef LONG (__stdcall * NtQueryInformationProcess__)(
    HANDLE ProcessHandle,
    LONG ProcessInformationClass,
    PVOID ProcessInformation,
    ULONG ProcessInformationLength,
    PULONG ReturnLength);
static NtQueryInformationProcess__ NtQueryInformationProcess_;
static HMODULE NTDLL_;
DWORD get_process_id( HANDLE process )
{
    PROCESS_BASIC_INFORMATION_ pinfo;
    if ( !NtQueryInformationProcess_ )
    {
        if ( !NTDLL_ )
            NTDLL_ = GetModuleHandleA( "ntdll" );
        if ( NTDLL_ )
            NtQueryInformationProcess_ =
                (NtQueryInformationProcess__)GetProcAddress( NTDLL_,
                    "NtQueryInformationProcess" );
    }
    if ( NtQueryInformationProcess_ )
    {
        (*NtQueryInformationProcess_)( process,
            /* ProcessBasicInformation == */ 0, &pinfo,
            sizeof( PROCESS_BASIC_INFORMATION_ ), NULL );
        return pinfo.UniqueProcessId;
    }
    return 0;
}


/*
 * Not really optimal, or efficient, but it is easier this way, and it is not
 * like we are going to be killing thousands, or even tens of processes.
 */

static void kill_process_tree( DWORD pid, HANDLE process )
{
    HANDLE process_snapshot_h = INVALID_HANDLE_VALUE;
    if ( !pid )
        pid = get_process_id( process );
    process_snapshot_h = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

    if ( INVALID_HANDLE_VALUE != process_snapshot_h )
    {
        BOOL ok = TRUE;
        PROCESSENTRY32 pinfo;
        pinfo.dwSize = sizeof( PROCESSENTRY32 );
        for (
            ok = Process32First( process_snapshot_h, &pinfo );
            ok == TRUE;
            ok = Process32Next( process_snapshot_h, &pinfo ) )
        {
            if ( pinfo.th32ParentProcessID == pid )
            {
                /* Found a child, recurse to kill it and anything else below it.
                 */
                HANDLE ph = OpenProcess( PROCESS_ALL_ACCESS, FALSE,
                    pinfo.th32ProcessID );
                if ( NULL != ph )
                {
                    kill_process_tree( pinfo.th32ProcessID, ph );
                    CloseHandle( ph );
                }
            }
        }
        CloseHandle( process_snapshot_h );
    }
    /* Now that the children are all dead, kill the root. */
    TerminateProcess( process, -2 );
}


static double creation_time( HANDLE process )
{
    FILETIME creation;
    FILETIME exit;
    FILETIME kernel;
    FILETIME user;
    FILETIME current;
    return GetProcessTimes( process, &creation, &exit, &kernel, &user )
        ? filetime_seconds( creation )
        : 0.0;
}


/*
 * Recursive check if first process is parent (directly or indirectly) of the
 * second one. Both processes are passed as process ids, not handles. Special
 * return value 2 means that the second process is smss.exe and its parent
 * process is System (first argument is ignored).
 */

static int is_parent_child( DWORD parent, DWORD child )
{
    HANDLE process_snapshot_h = INVALID_HANDLE_VALUE;

    if ( !child )
        return 0;
    if ( parent == child )
        return 1;

    process_snapshot_h = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if ( INVALID_HANDLE_VALUE != process_snapshot_h )
    {
        BOOL ok = TRUE;
        PROCESSENTRY32 pinfo;
        pinfo.dwSize = sizeof( PROCESSENTRY32 );
        for (
            ok = Process32First( process_snapshot_h, &pinfo );
            ok == TRUE;
            ok = Process32Next( process_snapshot_h, &pinfo ) )
        {
            if ( pinfo.th32ProcessID == child )
            {
                /* Unfortunately, process ids are not really unique. There might
                 * be spurious "parent and child" relationship match between two
                 * non-related processes if real parent process of a given
                 * process has exited (while child process kept running as an
                 * "orphan") and the process id of such parent process has been
                 * reused by internals of the operating system when creating
                 * another process.
                 *
                 * Thus additional check is needed - process creation time. This
                 * check may fail (i.e. return 0) for system processes due to
                 * insufficient privileges, and that is OK.
                 */
                double tchild = 0.0;
                double tparent = 0.0;
                HANDLE hchild = OpenProcess( PROCESS_QUERY_INFORMATION, FALSE, pinfo.th32ProcessID );
                CloseHandle( process_snapshot_h );

                /* csrss.exe may display message box like following:
                 *   xyz.exe - Unable To Locate Component
                 *   This application has failed to start because
                 *   boost_foo-bar.dll was not found. Re-installing the
                 *   application may fix the problem
                 * This actually happens when starting test process that depends
                 * on a dynamic library which failed to build. We want to
                 * automatically close these message boxes even though csrss.exe
                 * is not our child process. We may depend on the fact that (in
                 * all current versions of Windows) csrss.exe is directly child
                 * of the smss.exe process, which in turn is directly child of
                 * the System process, which always has process id == 4. This
                 * check must be performed before comparison of process creation
                 * times.
                 */
                if ( !stricmp( pinfo.szExeFile, "csrss.exe" ) &&
                    ( is_parent_child( parent, pinfo.th32ParentProcessID ) == 2 ) )
                    return 1;
                if ( !stricmp( pinfo.szExeFile, "smss.exe" ) &&
                    ( pinfo.th32ParentProcessID == 4 ) )
                    return 2;

                if ( hchild )
                {
                    HANDLE hparent = OpenProcess( PROCESS_QUERY_INFORMATION,
                        FALSE, pinfo.th32ParentProcessID );
                    if ( hparent )
                    {
                        tchild = creation_time( hchild );
                        tparent = creation_time( hparent );
                        CloseHandle( hparent );
                    }
                    CloseHandle( hchild );
                }

                /* Return 0 if one of the following is true:
                 *  1. we failed to read process creation time
                 *  2. child was created before alleged parent
                 */
                if ( ( tchild == 0.0 ) || ( tparent == 0.0 ) ||
                    ( tchild < tparent ) )
                    return 0;

                return is_parent_child( parent, pinfo.th32ParentProcessID ) & 1;
            }
        }

        CloseHandle( process_snapshot_h );
    }

    return 0;
}

typedef struct PROCESS_HANDLE_ID { HANDLE h; DWORD pid; } PROCESS_HANDLE_ID;


/*
 * This function is called by the operating system for each topmost window.
 */

BOOL CALLBACK close_alert_window_enum( HWND hwnd, LPARAM lParam )
{
    char buf[ 7 ] = { 0 };
    PROCESS_HANDLE_ID p = *( (PROCESS_HANDLE_ID *)lParam );
    DWORD pid = 0;
    DWORD tid = 0;

    /* We want to find and close any window that:
     *  1. is visible and
     *  2. is a dialog and
     *  3. is displayed by any of our child processes
     */
    if ( !IsWindowVisible( hwnd ) )
        return TRUE;

    if ( !GetClassNameA( hwnd, buf, sizeof( buf ) ) )
        return TRUE;  /* Failed to read class name; presume it is not a dialog. */

    if ( strcmp( buf, "#32770" ) )
        return TRUE;  /* Not a dialog */

    /* GetWindowThreadProcessId() returns 0 on error, otherwise thread id of
     * window message pump thread.
     */
    tid = GetWindowThreadProcessId( hwnd, &pid );

    if ( tid && is_parent_child( p.pid, pid ) )
    {
        /* Ask really nice. */
        PostMessageA( hwnd, WM_CLOSE, 0, 0 );
        /* Now wait and see if it worked. If not, insist. */
        if ( WaitForSingleObject( p.h, 200 ) == WAIT_TIMEOUT )
        {
            PostThreadMessageA( tid, WM_QUIT, 0, 0 );
            WaitForSingleObject( p.h, 300 );
        }

        /* Done, we do not want to check any other window now. */
        return FALSE;
    }

    return TRUE;
}


static void close_alert( HANDLE process )
{
    DWORD pid = get_process_id( process );
    /* If process already exited or we just can not get its process id, do not
     * go any further.
     */
    if ( pid )
    {
        PROCESS_HANDLE_ID p;
        p.h = process;
        p.pid = pid;
        EnumWindows( &close_alert_window_enum, (LPARAM)&p );
    }
}


/*
 * Returns a string's value buffer if not empty or 0 if empty.
 */

static char const * null_if_empty( string const * str )
{
    return str->size ? str->value : 0;
}


/*
 * Reports the last failed Windows API related error message.
 */

static void reportWindowsError( char const * const apiName )
{
    char * errorMessage;
    DWORD const errorCode = GetLastError();
    DWORD apiResult = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |  /* __in      DWORD dwFlags       */
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,                             /* __in_opt  LPCVOID lpSource    */
        errorCode,                        /* __in      DWORD dwMessageId   */
        0,                                /* __in      DWORD dwLanguageId  */
        (LPSTR)&errorMessage,             /* __out     LPTSTR lpBuffer     */
        0,                                /* __in      DWORD nSize         */
        0 );                              /* __in_opt  va_list * Arguments */
    if ( !apiResult )
        printf( "%s() Windows API failed: %d.\n", apiName, errorCode );
    else
    {
        printf( "%s() Windows API failed: %d - %s\n", apiName, errorCode,
            errorMessage );
        LocalFree( errorMessage );
    }
}


void exec_done( void )
{
}


#endif /* USE_EXECNT */
