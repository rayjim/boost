/* Copyright Vladimir Prus 2003. Distributed under the Boost */
/* Software License, Version 1.0. (See accompanying */
/* file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt) */

#include "native.h"
#include "hash.h"
#include "object.h"

# define P0 (PARSE *)0
# define C0 (OBJECT *)0


void declare_native_rule( const char * module, const char * rule, const char * * args,
                          LIST * (*f)( PARSE *, FRAME * ), int version )
{
    OBJECT * module_obj = 0;
    module_t * m;
    if ( module )
    {
        module_obj = object_new( module );
    }
    m = bindmodule( module_obj );
    if ( module_obj )
    {
        object_free( module_obj );
    }
    if (m->native_rules == 0) {
        m->native_rules = hashinit( sizeof( native_rule_t ), "native rules");
    }

    {
        native_rule_t n, *np = &n;
        n.name = object_new( rule );
        if (args)
        {
            n.arguments = args_new();
            lol_build( n.arguments->data, args );
        }
        else
        {
            n.arguments = 0;
        }
        n.procedure = parse_make( f, P0, P0, P0, C0, C0, 0 );
        n.version = version;
        hashenter(m->native_rules, (HASHDATA**)&np);
    }
}
