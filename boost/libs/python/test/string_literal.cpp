//#include <stdio.h>
#include <cassert>
#include <boost/static_assert.hpp>
#include <boost/python/detail/string_literal.hpp>

int main()
{
    using namespace boost::python::detail;
    
    BOOST_STATIC_ASSERT(!is_string_literal<int*&>::value);
    BOOST_STATIC_ASSERT(!is_string_literal<int* const&>::value);
    BOOST_STATIC_ASSERT(!is_string_literal<int*volatile&>::value);
    BOOST_STATIC_ASSERT(!is_string_literal<int*const volatile&>::value);
    
    BOOST_STATIC_ASSERT(!is_string_literal<char const*>::value);
    BOOST_STATIC_ASSERT(!is_string_literal<char*>::value);
    BOOST_STATIC_ASSERT(!is_string_literal<char*&>::value);
    BOOST_STATIC_ASSERT(!is_string_literal<char* const&>::value);
    BOOST_STATIC_ASSERT(!is_string_literal<char*volatile&>::value);
    BOOST_STATIC_ASSERT(!is_string_literal<char*const volatile&>::value);
    
    BOOST_STATIC_ASSERT(!is_string_literal<char[20]>::value);
    BOOST_STATIC_ASSERT(is_string_literal<char const[20]>::value);
    BOOST_STATIC_ASSERT(is_string_literal<char const[3]>::value);

    BOOST_STATIC_ASSERT(!is_string_literal<int[20]>::value);
    BOOST_STATIC_ASSERT(!is_string_literal<int const[20]>::value);
    BOOST_STATIC_ASSERT(!is_string_literal<int const[3]>::value);
    return 0;
}
