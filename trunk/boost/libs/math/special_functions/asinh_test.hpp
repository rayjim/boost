// unit test file asinh.hpp for the special functions test suite

//  (C) Copyright Hubert Holin 2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.


#include <functional>
#include <iomanip>
#include <iostream>


#include <boost/math/special_functions/asinh.hpp>


#include <boost/test/unit_test.hpp>


template<typename T>
T    asinh_error_evaluator(T x)
{
    using    ::std::abs;
    using    ::std::sinh;
    using    ::std::cosh;
        
    using    ::std::numeric_limits;
    
    using    ::boost::math::asinh;
    
    
    static T const    epsilon = numeric_limits<float>::epsilon();
    
    T                y = sinh(x);
    T                z = asinh(y);
    
    T                absolute_error = abs(z-x);
    T                relative_error = absolute_error*cosh(x);
    T                scaled_error = relative_error/epsilon;
    
    return(scaled_error);
}


template<typename T>
void    asinh_test(const char * more_blurb)
{
    BOOST_MESSAGE("Testing asinh in the real domain for "
        << more_blurb << ".");
    
    for    (int i = 0; i <= 100; i++)
    {
        T    x = static_cast<T>(i-50)/static_cast<T>(5);
        
        BOOST_CHECK_PREDICATE(::std::less_equal<T>(), 2,
            (
                asinh_error_evaluator(x),
                static_cast<T>(4)
            ));
    }
}


void    asinh_manual_check()
{
    BOOST_MESSAGE("asinh");
    
    for    (int i = 0; i <= 100; i++)
    {
        float        xf = static_cast<float>(i-50)/static_cast<float>(5);
        double       xd = static_cast<double>(i-50)/static_cast<double>(5);
        long double  xl =
                static_cast<long double>(i-50)/static_cast<long double>(5);
        
        BOOST_MESSAGE(  ::std::setw(15)
                     << asinh_error_evaluator(xf)
                     << ::std::setw(15)
                     << asinh_error_evaluator(xd)
                     << ::std::setw(15)
                     << asinh_error_evaluator(xl));
    }
    
    BOOST_MESSAGE(" ");
}
