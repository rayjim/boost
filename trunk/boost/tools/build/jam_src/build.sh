#!/bin/sh
# Copyrigt (C) 2002 Rene Rivera.
# Permission to copy, use, modify, sell and distribute this software
# is granted provided this copyright notice appears in all copies.
# This software is provided "as is" without express or implied
# warranty, and with no claim as to its suitability for any purpose.

# Reset the toolset.
BOOST_JAM_TOOLSET=

function echo_run ()
{
    echo "$@"
    "$@"
    r=$?
    if test $r -ne 0 ; then
        exit $r
    fi
}
function error_exit()
{
    echo "###"
    echo "###" "$@"
    echo "###"
    echo "### You can specify the toolset as the argument, i.e.:"
    echo "###     ./build.sh gcc"
    echo "###"
    echo "### Toolsets supported by this script are: como, darwin, gcc, intel-linux, kcc, vacpp"
    echo "###"
    exit 1
}
function test_path ()
{
    hash $1 2>/dev/null
}
function test_uname ()
{
    if test_path uname; then
        test `uname` = $*
    fi
}
# Try and guess the toolset to bootstrap the build with...
function Guess_Toolset ()
{
    if test_uname Darwin ; then BOOST_JAM_TOOLSET=darwin
    elif test_path gcc ; then BOOST_JAM_TOOLSET=gcc
    elif test_path icc ; then BOOST_JAM_TOOLSET=intel-linux
    elif test -e /opt/intel/compiler50/ia32/bin/iccvars.sh ; then BOOST_JAM_TOOLSET=intel-linux
    elif test_path xlc ; then BOOST_JAM_TOOLSET=vacpp
    elif test_path como ; then BOOST_JAM_TOOLSET=como
    elif test_path KCC ; then BOOST_JAM_TOOLSET=kcc
    fi
    if test "$BOOST_JAM_TOOLSET" = "" ; then
        error_exit "Could not find a suitable toolset."
    fi
}

# The one option we support in the invocation
# is the name of the toolset to force building
# with.
case "$1" in
    -*) Guess_Toolset ;;
    ?*) BOOST_JAM_TOOLSET=$1 ; shift ;;
    *) Guess_Toolset ;;
esac
case $BOOST_JAM_TOOLSET in
    gcc)
    BOOST_JAM_CC=gcc
    BOOST_JAM_OPT_JAM="-o bootstrap.$BOOST_JAM_TOOLSET/jam0"
    BOOST_JAM_OPT_MKJAMBASE="-o bootstrap.$BOOST_JAM_TOOLSET/mkjambase0"
    ;;
    
    darwin)
    BOOST_JAM_CC=cc
    BOOST_JAM_OPT_JAM="-o bootstrap.$BOOST_JAM_TOOLSET/jam0"
    BOOST_JAM_OPT_MKJAMBASE="-o bootstrap.$BOOST_JAM_TOOLSET/mkjambase0"
    ;;
    
    intel-linux)
    if test -e /opt/intel/compiler50/ia32/bin/iccvars.sh ; then
        . /opt/intel/compiler50/ia32/bin/iccvars.sh
    fi
    BOOST_JAM_CC=icc
    BOOST_JAM_OPT_JAM="-o bootstrap.$BOOST_JAM_TOOLSET/jam0"
    BOOST_JAM_OPT_MKJAMBASE="-o bootstrap.$BOOST_JAM_TOOLSET/mkjambase0"
    ;;
    
    vacpp)
    BOOST_JAM_CC=xlc
    BOOST_JAM_OPT_JAM="-qstrict -o bootstrap.$BOOST_JAM_TOOLSET/jam0"
    BOOST_JAM_OPT_MKJAMBASE="-qstrict -o bootstrap.$BOOST_JAM_TOOLSET/mkjambase0"
    ;;
    
    como)
    BOOST_JAM_CC=como
    BOOST_JAM_OPT_JAM="-o bootstrap.$BOOST_JAM_TOOLSET/jam0"
    BOOST_JAM_OPT_MKJAMBASE="-o bootstrap.$BOOST_JAM_TOOLSET/mkjambase0"
    ;;
    
    kcc)
    BOOST_JAM_CC=KCC
    BOOST_JAM_OPT_JAM="-o bootstrap.$BOOST_JAM_TOOLSET/jam0"
    BOOST_JAM_OPT_MKJAMBASE="-o bootstrap.$BOOST_JAM_TOOLSET/mkjambase0"
    ;;
    
    *)
    error_exit "Unknown toolset: $BOOST_JAM_TOOLSET"
    ;;
esac

echo "###"
echo "### Using '$BOOST_JAM_TOOLSET' toolset."
echo "###"

MKJAMBASE_SOURCES="mkjambase.c"
BJAM_SOURCES="\
 command.c compile.c execnt.c execunix.c execvms.c expand.c\
 filent.c fileos2.c fileunix.c filevms.c glob.c hash.c\
 hdrmacro.c headers.c jam.c jambase.c jamgram.c lists.c make.c make1.c\
 newstr.c option.c parse.c pathunix.c pathvms.c regexp.c\
 rules.c scan.c search.c subst.c timestamp.c variable.c modules.c\
 strings.c filesys.c builtins.c pwd.c"

echo_run rm -rf bootstrap.$BOOST_JAM_TOOLSET
echo_run mkdir bootstrap.$BOOST_JAM_TOOLSET
if test ! -e jamgram.y -o ! -e jamgramtab.h ; then
    echo_run /bin/sh ./yyacc jamgram.y jamgramtab.h jamgram.yy
fi
if test ! -e jamgram.c -o ! -e jamgram.h ; then
    if test_path yacc ; then YACC="yacc -d"
    elif test_path bison ; then YACC="bison -y -d --yacc"
    fi
    echo_run $YACC jamgram.y
    mv -f y.tab.c jamgram.c
    mv -f y.tab.h jamgram.h
fi
if test ! -e jambase.c ; then
    echo_run ${BOOST_JAM_CC} ${BOOST_JAM_OPT_MKJAMBASE} ${MKJAMBASE_SOURCES}
    if test -x "./bootstrap.$BOOST_JAM_TOOLSET/mkjambase0" ; then
        echo_run ./bootstrap.$BOOST_JAM_TOOLSET/mkjambase0 jambase.c Jambase
    fi
fi
echo_run ${BOOST_JAM_CC} ${BOOST_JAM_OPT_JAM} ${BJAM_SOURCES}
if test -x "./bootstrap.$BOOST_JAM_TOOLSET/jam0" ; then
    echo_run ./bootstrap.$BOOST_JAM_TOOLSET/jam0 -f build.jam --toolset=$BOOST_JAM_TOOLSET "$@"
fi
