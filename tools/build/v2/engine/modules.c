/*
 *  Copyright 2001-2004 David Abrahams.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */
#include "jam.h"

#include "modules.h"
#include "string.h"
#include "hash.h"
#include "object.h"
#include "lists.h"
#include "parse.h"
#include "rules.h"
#include "variable.h"
#include "strings.h"
#include "native.h"
#include <assert.h>

static struct hash * module_hash = 0;
static module_t root;

module_t * bindmodule( OBJECT * name )
{

    if ( !name )
    {
        return &root;
    }
    else
    {
        PROFILE_ENTER( BINDMODULE );

        module_t * m;
        int found;

        if ( !module_hash )
            module_hash = hashinit( sizeof( module_t ), "modules" );

        m = (module_t *)hash_insert( module_hash, name, &found );
        if ( !found )
        {
            m->name = object_copy( name );
            m->variables = 0;
            m->variable_indices = 0;
            m->num_fixed_variables = 0;
            m->fixed_variables = 0;
            m->rules = 0;
            m->imported_modules = 0;
            m->class_module = 0;
            m->native_rules = 0;
            m->user_module = 0;
        }

        PROFILE_EXIT( BINDMODULE );

        return m;
    }
}

/*
 * demand_rules() - Get the module's "rules" hash on demand.
 */
struct hash * demand_rules( module_t * m )
{
    if ( !m->rules )
        m->rules = hashinit( sizeof( RULE ), "rules" );
    return m->rules;
}


/*
 * delete_module() - wipe out the module's rules and variables.
 */

static void delete_rule_( void * xrule, void * data )
{
    rule_free( (RULE *)xrule );
}


static void delete_native_rule( void * xrule, void * data )
{
    native_rule_t * rule = (native_rule_t *)xrule;
    object_free( rule->name );
    if ( rule->procedure )
        function_free( rule->procedure );
}


static void delete_imported_modules( void * xmodule_name, void * data )
{
    object_free( *(OBJECT * *)xmodule_name );
}


void delete_module( module_t * m )
{
    /* Clear out all the rules. */
    if ( m->rules )
    {
        hashenumerate( m->rules, delete_rule_, (void *)0 );
        hashdone( m->rules );
        m->rules = 0;
    }

    if ( m->native_rules )
    {
        hashenumerate( m->native_rules, delete_native_rule, (void *)0 );
        hashdone( m->native_rules );
        m->native_rules = 0;
    }

    if ( m->variables )
    {
        var_done( m );
        m->variables = 0;
    }

    if ( m->fixed_variables )
    {
        int i;
        for ( i = 0; i < m->num_fixed_variables; ++i )
        {
            list_free( m->fixed_variables[ i ] );
        }
        BJAM_FREE( m->fixed_variables );
        m->fixed_variables = 0;
        hashdone( m->variable_indices );
        m->variable_indices = 0;
    }

    if ( m->imported_modules )
    {
        hashenumerate( m->imported_modules, delete_imported_modules, (void *)0 );
        hashdone( m->imported_modules );
        m->imported_modules = 0;
    }
}


static void delete_module_( void * xmodule, void * data )
{
    module_t *m = (module_t *)xmodule;

    delete_module( m );
    object_free( m->name );
}

void modules_done()
{
    hashenumerate( module_hash, delete_module_, (void *)0 ); 
    hashdone( module_hash );
    module_hash = 0;
    delete_module( &root );
}

module_t * root_module()
{
    return &root;
}


void import_module( LIST * module_names, module_t * target_module )
{
    PROFILE_ENTER( IMPORT_MODULE );

    struct hash * h;
    LISTITER iter, end;

    if ( !target_module->imported_modules )
        target_module->imported_modules = hashinit( sizeof( char * ), "imported" );
    h = target_module->imported_modules;

    iter = list_begin( module_names ), end = list_end( module_names );
    for ( ; iter != end; iter = list_next( iter ) )
    {
        int found;
        OBJECT * s = list_item( iter );
        OBJECT * * ss = (OBJECT * *)hash_insert( h, s, &found );
        if( !found )
        {
            *ss = object_copy( s );
        }
    }

    PROFILE_EXIT( IMPORT_MODULE );
}


static void add_module_name( void * r_, void * result_ )
{
    OBJECT * * r = (OBJECT * *)r_;
    LIST * * result = (LIST * *)result_;

    *result = list_push_back( *result, object_copy( *r ) );
}


LIST * imported_modules( module_t * module )
{
    LIST * result = L0;
    if ( module->imported_modules )
        hashenumerate( module->imported_modules, add_module_name, &result );
    return result;
}


FUNCTION * function_bind_variables( FUNCTION * f, module_t * module, int * counter );
FUNCTION * function_unbind_variables( FUNCTION * f );

struct fixed_variable
{
    OBJECT * key;
    int n;
};

struct bind_vars_t
{
    module_t * module;
    int counter;
};

static void bind_variables_for_rule( void * xrule, void * xdata )
{
    RULE * rule = (RULE *)xrule;
    struct bind_vars_t * data = (struct bind_vars_t *)xdata;
    if ( rule->procedure && rule->module == data->module )
        rule->procedure = function_bind_variables( rule->procedure, data->module, &data->counter );
}

void module_bind_variables( struct module_t * m )
{
    if ( m != root_module() && m->rules )
    {
        struct bind_vars_t data;
        data.module = m;
        data.counter = m->num_fixed_variables;
        hashenumerate( m->rules, &bind_variables_for_rule, &data );
        module_set_fixed_variables( m, data.counter );
    }
}

int module_add_fixed_var( struct module_t * m, OBJECT * name, int * counter )
{
    struct fixed_variable * v;
    int found;

    assert( !m->class_module );

    if ( !m->variable_indices )
        m->variable_indices = hashinit( sizeof( struct fixed_variable ), "variable index table" );

    v = (struct fixed_variable *)hash_insert( m->variable_indices, name, &found );
    if ( !found )
    {
        v->key = object_copy( name );
        v->n = (*counter)++;
    }

    return v->n;
}

LIST * var_get_and_clear_raw( module_t * m, OBJECT * name );

static void load_fixed_variable( void * xvar, void * data )
{
    struct fixed_variable * var = (struct fixed_variable *)xvar;
    struct module_t * m = (struct module_t *)data;
    if ( var->n >= m->num_fixed_variables )
    {
        m->fixed_variables[ var->n ] = var_get_and_clear_raw( m, var->key );
    }
}

void module_set_fixed_variables( struct module_t * m, int n_variables )
{
    /* Reallocate */
    struct hash * variable_indices;
    LIST * * fixed_variables = BJAM_MALLOC( n_variables * sizeof( LIST * ) );
    if ( m->fixed_variables )
    {
        memcpy( fixed_variables, m->fixed_variables, n_variables * sizeof( LIST * ) );
        BJAM_FREE( m->fixed_variables );
    }
    m->fixed_variables = fixed_variables;
    if ( m->class_module )
    {
        variable_indices = m->class_module->variable_indices;
    }
    else
    {
        variable_indices = m->variable_indices;
    }
    if ( variable_indices )
        hashenumerate( variable_indices, &load_fixed_variable, m );
    m->num_fixed_variables = n_variables;
}

int module_get_fixed_var( struct module_t * m_, OBJECT * name )
{
    struct fixed_variable * v;
    struct module_t * m = m_;

    if ( m->class_module )
    {
        m = m->class_module;
    }

    if ( !m->variable_indices )
        return -1;

    v = (struct fixed_variable *)hash_find( m->variable_indices, name );
    if ( v && v->n < m_->num_fixed_variables )
    {
        return v->n;
    }
    else
    {
        return -1;
    }
}
