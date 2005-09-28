#include "test.hpp"
#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

template<class T, T n> struct t{};

BOOST_TYPEOF_REGISTER_TEMPLATE(t,
                               (class)
                               (BOOST_TYPEOF_INTEGRAL(P0))
                               )

BOOST_STATIC_ASSERT((boost::type_of::test<t<int, 5> >::value));
