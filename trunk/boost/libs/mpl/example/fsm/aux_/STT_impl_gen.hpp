//-----------------------------------------------------------------------------
// boost fsm/handler.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_FSM_HANDLER_INCLUDED
#define BOOST_FSM_HANDLER_INCLUDED

#include "boost/mpl/if.hpp"
#include "boost/mpl/fold.hpp"
#include "boost/mpl/front.hpp"
#include "boost/type_traits/is_same.hpp"

#include <typeinfo>
#include <cassert>

namespace fsm {
namespace aux {

namespace mpl = boost::mpl;
using namespace mpl::placeholders;

template< typename Transition > 
struct STT_void_row_impl
{
    typedef typename Transition::from_state_t   state_t;
    typedef typename Transition::fsm_t          fsm_t;
    typedef typename Transition::base_event_t   base_event_t;

    static long do_process_event(fsm_t&, long state, base_event_t const&)
    {
        assert(false);
        return state;
    }
    
    static long do_transition(fsm_t&, long state, base_event_t const&)
    {
        assert(false);
        return state;
    }
};


template<
      typename PrevRowImpl
    , typename Transition
    >
struct STT_event_row_impl
    : PrevRowImpl
{
    typedef typename Transition::from_state_t   state_t;
    typedef typename Transition::fsm_t          fsm_t;
    typedef typename Transition::base_event_t   base_event_t;

    static long do_process_event(fsm_t& fsm, long state, base_event_t const& evt)
    {
        if (typeid(typename Transition::event_t) == typeid(evt))
        {
            // typedefs are here to make GCC happy
            typedef typename Transition::to_state_t to_state_;
            typedef typename Transition::from_state_t from_state_;

            return Transition::do_transition(fsm, evt)
                ? to_state_::do_check_invariant(fsm)
                : from_state_::do_check_invariant(fsm)
                ;
        }

        return PrevRowImpl::do_process_event(fsm, state, evt);
    }
};

template<
      typename PrevRowImpl
    , typename StateType
    >
struct STT_state_row_impl
    : PrevRowImpl
{
    typedef typename PrevRowImpl::fsm_t         fsm_t;
    typedef typename PrevRowImpl::base_event_t  base_event_t;

    static long do_transition(fsm_t& fsm, long state, base_event_t const& evt)
    {
        return state == StateType::value
            ? PrevRowImpl::do_process_event(fsm, state, evt)
            : PrevRowImpl::do_transition(fsm, state, evt)
            ;
    }

    static long do_process_event(fsm_t&, long state, base_event_t const&)
    {
        assert(false);
        return state;
    }
};

template<
      typename PrevRowImpl
    , typename Transition
    >
struct STT_row_impl
{
    typedef typename mpl::if_<
          boost::is_same<
              typename PrevRowImpl::state_t
            , typename Transition::from_state_t
            >
        , STT_event_row_impl< PrevRowImpl,Transition >
        , STT_event_row_impl<
              STT_state_row_impl< PrevRowImpl,typename PrevRowImpl::state_t >
            , Transition
            >
        >::type type;
};


template< typename Transitions >
struct STT_impl_gen
{
 private:
    typedef typename mpl::front<Transitions>::type first_;
    typedef typename mpl::fold<
          Transitions
        , STT_void_row_impl<first_>
        , STT_row_impl<_,_>
        >::type STT_impl_;

 public:
    typedef STT_state_row_impl<
          STT_impl_
        , typename STT_impl_::state_t
        > type;
};

} // namespace aux
} // namespace fsm

#endif // BOOST_FSM_HANDLER_INCLUDED
