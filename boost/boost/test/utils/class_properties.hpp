//  (C) Copyright Gennadiy Rozental 2001-2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : simple facility that mimmic notion of read-only read-write 
//  properties in C++ classes. Original idea by Henrik Ravn.
// ***************************************************************************

#ifndef BOOST_TEST_CLASS_PROPERTIES_HPP_071894GER
#define BOOST_TEST_CLASS_PROPERTIES_HPP_071894GER

// Boost.Test
#include <boost/test/detail/config.hpp>

// Boost
#if !BOOST_WORKAROUND(__IBMCPP__, BOOST_TESTED_AT(600))
#include <boost/preprocessor/seq/for_each.hpp>
#endif
#include <boost/call_traits.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/utility/addressof.hpp>

// STL
#include <iosfwd>

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {

namespace unit_test {

// ************************************************************************** //
// **************                 class_property               ************** //
// ************************************************************************** //

template<class PropertyType>
class class_property {
protected:
    typedef typename call_traits<PropertyType>::const_reference read_access_t;
    typedef typename call_traits<PropertyType>::param_type      write_param_t;
    typedef typename add_pointer<PropertyType const>::type      address_res_t;

public:
    // Constructor
                    class_property() : value( PropertyType() ) {}
    explicit        class_property( write_param_t init_value )
    : value( init_value ) {}

    // Access methods
    operator        read_access_t() const   { return value; }
    read_access_t   get() const             { return value; }
    bool            operator!() const       { return !value; }
    address_res_t   operator&() const       { return &value; }

    // Data members
#ifndef BOOST_TEST_NO_PROTECTED_USING
protected:
#endif
    PropertyType        value;
};

//____________________________________________________________________________//

#ifdef BOOST_CLASSIC_IOSTREAMS

template<class PropertyType>
inline std::ostream&
operator<<( std::ostream& os, class_property<PropertyType> const& p )

#else

template<typename CharT1, typename Tr,class PropertyType>
inline std::basic_ostream<CharT1,Tr>&
operator<<( std::basic_ostream<CharT1,Tr>& os, class_property<PropertyType> const& p )

#endif
{
    return os << p.get();
}

//____________________________________________________________________________//

#define DEFINE_PROPERTY_FREE_BINARY_OPERATOR( op )                              \
template<class PropertyType>                                                    \
inline bool                                                                     \
operator op( PropertyType const& lhs, class_property<PropertyType> const& rhs ) \
{                                                                               \
    return lhs op rhs.get();                                                    \
}                                                                               \
template<class PropertyType>                                                    \
inline bool                                                                     \
operator op( class_property<PropertyType> const& lhs, PropertyType const& rhs ) \
{                                                                               \
    return lhs.get() op rhs;                                                    \
}                                                                               \
template<class PropertyType>                                                    \
inline bool                                                                     \
operator op( class_property<PropertyType> const& lhs,                           \
             class_property<PropertyType> const& rhs )                          \
{                                                                               \
    return lhs.get() op rhs.get();                                              \
}                                                                               \
/**/

DEFINE_PROPERTY_FREE_BINARY_OPERATOR( == )
DEFINE_PROPERTY_FREE_BINARY_OPERATOR( != )

#undef DEFINE_PROPERTY_FREE_BINARY_OPERATOR

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1200)

#define DEFINE_PROPERTY_LOGICAL_OPERATOR( op )                                  \
template<class PropertyType>                                                    \
inline bool                                                                     \
operator op( bool b, class_property<PropertyType> const& p )                    \
{                                                                               \
    return b op p.get();                                                        \
}                                                                               \
template<class PropertyType>                                                    \
inline bool                                                                     \
operator op( class_property<PropertyType> const& p, bool b )                    \
{                                                                               \
    return b op p.get();                                                        \
}                                                                               \
/**/

DEFINE_PROPERTY_LOGICAL_OPERATOR( && )
DEFINE_PROPERTY_LOGICAL_OPERATOR( || )

#endif

// ************************************************************************** //
// **************               readonly_property              ************** //
// ************************************************************************** //

template<class PropertyType>
class readonly_property : public class_property<PropertyType> {
    typedef class_property<PropertyType>    base;
    typedef typename base::address_res_t    arrow_res_t;
protected:
    typedef typename base::write_param_t    write_param_t;
public:
    // Constructor
                    readonly_property() {}
    explicit        readonly_property( write_param_t init_value ) : base( init_value ) {}

    // access methods
    arrow_res_t     operator->() const      { return boost::addressof( base::value ); }
};

//____________________________________________________________________________//

#if BOOST_WORKAROUND(__IBMCPP__, BOOST_TESTED_AT(600))

#define BOOST_READONLY_PROPERTY( property_type, friends ) boost::unit_test::readwrite_property<property_type >

#else

#define BOOST_READONLY_PROPERTY_DECLARE_FRIEND(r, data, elem) friend class elem;

#define BOOST_READONLY_PROPERTY( property_type, friends )                           \
class BOOST_JOIN( readonly_property, __LINE__ )                                     \
: public boost::unit_test::readonly_property<property_type > {                      \
    typedef boost::unit_test::readonly_property<property_type > base;               \
    BOOST_PP_SEQ_FOR_EACH( BOOST_READONLY_PROPERTY_DECLARE_FRIEND, ' ', friends )   \
    typedef base::write_param_t  write_param_t;                                     \
public:                                                                             \
                BOOST_JOIN( readonly_property, __LINE__ )() {}                      \
    explicit    BOOST_JOIN( readonly_property, __LINE__ )( write_param_t init_v  )  \
    : base( init_v ) {}                                                             \
}                                                                                   \
/**/

#endif

// ************************************************************************** //
// **************              readwrite_property              ************** //
// ************************************************************************** //

template<class PropertyType>
class readwrite_property : public class_property<PropertyType> {
    typedef class_property<PropertyType>                base;
    typedef typename add_pointer<PropertyType>::type    arrow_res_t;
    typedef typename base::address_res_t                const_arrow_res_t;
    typedef typename base::write_param_t                write_param_t;
public:
                    readwrite_property() : base() {}
    explicit        readwrite_property( write_param_t init_value ) : base( init_value ) {}

    // access methods
    void            set( write_param_t v )  { base::value = v; }
    arrow_res_t     operator->()            { return boost::addressof( base::value ); }
    const_arrow_res_t operator->() const    { return boost::addressof( base::value ); }

#ifndef BOOST_TEST_NO_PROTECTED_USING
    using           base::value;
#endif
};

//____________________________________________________________________________//

} // unit_test

} // namespace boost

//____________________________________________________________________________//

#include <boost/test/detail/enable_warnings.hpp>

#undef BOOST_TEST_NO_PROTECTED_USING

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.4  2005/02/20 08:27:08  rogeeff
//  This a major update for Boost.Test framework. See release docs for complete list of fixes/updates
//
//  Revision 1.3  2005/02/01 06:40:07  rogeeff
//  copyright update
//  old log entries removed
//  minor stilistic changes
//  depricated tools removed
//
//  Revision 1.2  2005/01/22 19:22:13  rogeeff
//  implementation moved into headers section to eliminate dependency of included/minimal component on src directory
//
//  Revision 1.1  2005/01/22 18:21:39  rogeeff
//  moved sharable staff into utils
//
// ***************************************************************************

#endif // BOOST_TEST_CLASS_PROPERTIES_HPP_071894GER
