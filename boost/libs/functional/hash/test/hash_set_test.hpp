
//  (C) Copyright Daniel James 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CONTAINER_TYPE)
#error "CONTAINER_TYPE not defined"
#else

#include <boost/preprocessor/cat.hpp>

namespace BOOST_PP_CAT(CONTAINER_TYPE, _tests)
{
    template <class T>
    void integer_tests(T* = 0)
    {
        const int number_of_containers = 11;
        T containers[number_of_containers];

        for(int i = 0; i < 5; ++i) {
            for(int j = 0; j < i; ++j)
                containers[i].insert(0);
        }

        containers[6].insert(1);
        containers[7].insert(1);
        containers[7].insert(1);
        containers[8].insert(-1);
        containers[9].insert(-1);
        containers[9].insert(-1);
        containers[10].insert(-1);
        containers[10].insert(1);

        boost::hash<T> hasher;

        for(int i2 = 0; i2 < number_of_containers; ++i2) {
            BOOST_CHECK_EQUAL(hasher(containers[i2]), hasher(containers[i2]));

            for(int j2 = i2 + 1; j2 < number_of_containers; ++j2) {
                BOOST_CHECK(
                        (containers[i2] == containers[j2]) ==
                        (hasher(containers[i2]) == hasher(containers[j2]))
                        );
            }
        }
    }

    BOOST_AUTO_UNIT_TEST(BOOST_PP_CAT(CONTAINER_TYPE, _hash_integer_tests))
    {
        integer_tests((CONTAINER_TYPE<char>*) 0);
        integer_tests((CONTAINER_TYPE<int>*) 0);
        integer_tests((CONTAINER_TYPE<unsigned long>*) 0);
        integer_tests((CONTAINER_TYPE<double>*) 0);
    }
}

#undef CONTAINER_TYPE
#endif
