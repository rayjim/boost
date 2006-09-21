/*=============================================================================
    Copyright (c) 2005 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_MAP_07212005_1106)
#define FUSION_MAP_07212005_1106

#include <boost/fusion/support/pair.hpp>
#include <boost/fusion/support/detail/access.hpp>
#include <boost/fusion/support/detail/access.hpp>
#include <boost/fusion/sequence/container/map/map_fwd.hpp>
#include <boost/fusion/sequence/container/map/detail/lookup_key.hpp>
#include <boost/fusion/sequence/container/map/detail/begin_impl.hpp>
#include <boost/fusion/sequence/container/map/detail/end_impl.hpp>
#include <boost/fusion/sequence/container/map/detail/at_key_impl.hpp>
#include <boost/fusion/sequence/container/map/detail/value_at_key_impl.hpp>
#include <boost/fusion/sequence/container/map/detail/is_associative_impl.hpp>
#include <boost/fusion/sequence/container/vector/vector.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion
{
    struct void_;
    struct map_tag;
    struct forward_sequence_tag;
    struct fusion_sequence_tag;

    template <BOOST_PP_ENUM_PARAMS(FUSION_MAX_MAP_SIZE, typename T)>
    struct map : sequence_base<map<BOOST_PP_ENUM_PARAMS(FUSION_MAX_MAP_SIZE, T)> >
    {
        typedef map_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef forward_sequence_tag category;
        typedef mpl::false_ is_view;

        typedef vector<
            BOOST_PP_ENUM_PARAMS(FUSION_MAX_MAP_SIZE, T)> 
        storage_type;

        typedef typename storage_type::size size;

        map()
            : data() {}
        
        template <typename T>
        explicit map(T const& rhs)
            : data(rhs) {}

        #include <boost/fusion/sequence/container/map/detail/map_forward_ctor.hpp>
        #include <boost/fusion/sequence/container/map/detail/map_lookup.hpp>

        template <typename T>
        map&
        operator=(T const& rhs)
        {
            data = rhs;
            return *this;
        }

        storage_type& get_data() { return data; }
        storage_type const& get_data() const { return data; }

    private:
        
        storage_type data;
    };
}}

#endif
