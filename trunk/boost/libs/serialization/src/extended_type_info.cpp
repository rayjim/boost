/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// extended_type_info.cpp: implementation for portable version of type_info

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.


#include <boost/config.hpp> // msvc needs this to suppress warning

#include <cstring>
#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std{ using ::strcmp; }
#endif

#include <algorithm>
#include <set>
#include <cassert>

#define BOOST_SERIALIZATION_SOURCE
#include <boost/serialization/extended_type_info.hpp>

namespace boost { 
namespace serialization {

// remove all registrations corresponding to a given type
void unregister_void_casts(extended_type_info *eti);

namespace { // anonymous

// it turns out that at least one compiler (msvc 6.0) doesn't guarentee
// to destroy static objects in exactly the reverse sequence that they
// are constructed.  To guarentee this, use a singleton pattern
class tkmap {
    struct type_info_compare
    {
        bool
        operator()(const extended_type_info * lhs, const extended_type_info * rhs) const
        {
        	return *lhs < *rhs;
        }
    };
    typedef std::multiset<const extended_type_info *, type_info_compare> type;
    tkmap(){}
    type m_map;
    static tkmap * m_self;
public:
    ~tkmap(){
        m_self = NULL;
    }
    static void
    insert(const extended_type_info * eti){
        if(NULL == m_self){
            static tkmap instance;
        	m_self = & instance;
        }
        m_self->m_map.insert(eti);
    }
    static const extended_type_info * 
    find(const extended_type_info * t){
        if(NULL == m_self)
        	return NULL;
        tkmap::type::const_iterator it;
        it = m_self->m_map.find(t);
        if(it == m_self->m_map.end())
        	return NULL;
        return *it;
    }
    static void 
    purge(const extended_type_info * eti){
        if(NULL == m_self)
        	return;
        tkmap::type::iterator i = m_self->m_map.begin();
        tkmap::type::iterator k = m_self->m_map.end();
        while(i != k){
        	// note that the erase might invalidate i so save it here
        	tkmap::type::iterator j = i++;
        	if(*j == eti)
        		m_self->m_map.erase(j);
        }
    }
};

tkmap * tkmap::m_self = NULL;

class ktmap {
    struct key_compare
    {
        bool
        operator()(const extended_type_info * lhs, const extended_type_info * rhs) const
        {
            // shortcut to exploit string pooling
            if(lhs->key == rhs->key)
                return false;
            if(NULL == lhs->key)
                return true;
            if(NULL == rhs->key)
                return false;
            return std::strcmp(lhs->key, rhs->key) < 0; 
        }
    };
    typedef std::multiset<const extended_type_info *, key_compare> type;
    type m_map;
    static ktmap * m_self;
    ktmap(){}
public:
    ~ktmap(){
        m_self = NULL;
    }
    static void
    insert(const extended_type_info * eti){
        if(NULL == m_self){
            static ktmap instance;
        	m_self = & instance;
        }
        m_self->m_map.insert(eti);
    }
    static void 
    purge(const extended_type_info * eti){
        if(NULL == m_self)
        	return;
        ktmap::type::iterator i = m_self->m_map.begin();
        ktmap::type::iterator k = m_self->m_map.end();
        while(i == k){
            // note that the erase might invalidate i so save it here
            ktmap::type::iterator j = i++;
            if(*j == eti)
                m_self->m_map.erase(j);
        }
    }
    class extended_type_info_arg : public extended_type_info
    {
    public:
        extended_type_info_arg(const char * search_key_)
            : extended_type_info(NULL)
        {
            key = search_key_;
        }
        virtual bool
        less_than(const extended_type_info &rhs) const
        {
            assert(false);
            return false;   // to prevent a syntax error
        }
        virtual bool
        equal_to(const extended_type_info &rhs) const{
            assert(false);
            return false;   // to prevent a syntax error
        }
        virtual bool
        not_equal_to(const extended_type_info &rhs) const{
            assert(false);
            return false;   // to prevent a syntax error
        }
    };
    static const extended_type_info * 
    find(const char *key)
    {
        if(NULL == m_self)
        	return NULL;
        extended_type_info_arg arg(key);
        ktmap::type::const_iterator it;
        it = m_self->m_map.find(&arg);
        if(it == m_self->m_map.end())
            return NULL;
        return *it;
    }

};

ktmap * ktmap::m_self = NULL;

} // anonymous

BOOST_SERIALIZATION_DECL const extended_type_info * 
extended_type_info::find(const char *key)
{
    return ktmap::find(key);
}

BOOST_SERIALIZATION_DECL void 
extended_type_info::self_register()
{
    tkmap::insert(this);
}

BOOST_SERIALIZATION_DECL void  
extended_type_info::key_register(const char *key_) {
    if(NULL == key_)
        return;
    key = key_;
    ktmap::insert(this);
}

BOOST_SERIALIZATION_DECL 
extended_type_info::extended_type_info(const char * type_info_key_) :
    type_info_key(type_info_key_),
    key(NULL)
{}

BOOST_SERIALIZATION_DECL 
extended_type_info::~extended_type_info(){
    // remove entries in maps which correspond to this type
    tkmap::purge(this);
    ktmap::purge(this);
    unregister_void_casts(this);
}


int BOOST_SERIALIZATION_DECL 
extended_type_info::type_info_key_cmp(const extended_type_info & rhs) const {
    if(type_info_key == rhs.type_info_key)
        return 0;
    //return strcmp(lhs.type_info_key, rhs.type_info_key);
    // all we require is that the type_info_key be unique
    // so just compare the addresses
    return type_info_key < rhs.type_info_key ? -1 : 1;
}

BOOST_SERIALIZATION_DECL const extended_type_info * 
extended_type_info::find(const extended_type_info * t)
{
    return tkmap::find(t);
}

bool BOOST_SERIALIZATION_DECL 
extended_type_info::operator<(const extended_type_info &rhs) const {
    int i = type_info_key_cmp(rhs);
    if(i < 0)
        return true;
    if(i > 0)
        return false;
    return less_than(rhs);
}

bool BOOST_SERIALIZATION_DECL 
extended_type_info::operator==(const extended_type_info &rhs) const {
    int i = type_info_key_cmp(rhs);
    if(i != 0)
        return false;
    return equal_to(rhs);
}

bool BOOST_SERIALIZATION_DECL 
extended_type_info::operator!=(const extended_type_info &rhs) const {
    int i = type_info_key_cmp(rhs);
    if(i != 0)
        return true;
    return not_equal_to(rhs);
}

} // namespace serialization
} // namespace boost
