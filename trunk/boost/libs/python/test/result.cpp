// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#include <boost/python/detail/result.hpp>
#include <boost/type.hpp>
#include <functional>

using boost::python::detail::result;
using boost::type;

void expect_int(type<int>*) {}
void expect_string(type<char*>*) {}

struct X {};

int main()
{
    // Test the usage which works for functions, member functions, and data members
    expect_int(
        result((int(*)())0)
        );

    expect_int(
        result((int(*)(char))0)
        );

    expect_int(
        result((int(X::*)())0)
        );

    expect_int(
        result((int(X::*)(char))0)
        );

    expect_int(
        result((int(X::*))0)
        );

    expect_string(
        result((char*(*)())0)
        );
    
    expect_string(
        result((char*(*)(char))0)
        );
    
    expect_string(
        result((char*(X::*)())0)
        );
    
    expect_string(
        result((char*(X::*)(char))0)
        );
    
    expect_string(
        result((char*(X::*))0)
        );
    
    // Show that we can use the general version that works for
    // AdaptableFunctions
    expect_int(
        result((int(*)())0,0L)
        );

    expect_int(
        result((int(*)(char))0,0L)
        );

    expect_int(
        result((int(X::*)())0,0L)
        );

    expect_int(
        result((int(X::*)(char))0,0L)
        );

    expect_int(
        result((int(X::*))0,0L)
        );
    
    expect_int(
        result(std::plus<int>(), 0L)
        );

    expect_string(
        result((char*(*)())0,0L)
        );
    
    expect_string(
        result((char*(*)(char))0,0L)
        );
    
    expect_string(
        result((char*(X::*)())0,0L)
        );
    
    expect_string(
        result((char*(X::*)(char))0,0L)
        );
    
    expect_string(
        result((char*(X::*))0,0L)
        );
    
    expect_string(
        result(std::plus<char*>(), 0L)
        );

    return 0;
}
