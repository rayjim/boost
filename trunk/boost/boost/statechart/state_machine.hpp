#ifndef BOOST_FSM_STATE_MACHINE_HPP_INCLUDED
#define BOOST_FSM_STATE_MACHINE_HPP_INCLUDED
//////////////////////////////////////////////////////////////////////////////
// (c) Copyright Andreas Huber Doenni 2002-2004
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////



#include <boost/fsm/event.hpp>
#include <boost/fsm/null_exception_translator.hpp>
#include <boost/fsm/result.hpp>

#include <boost/fsm/detail/rtti_policy.hpp>
#include <boost/fsm/detail/state_base.hpp>
#include <boost/fsm/detail/leaf_state.hpp>
#include <boost/fsm/detail/node_state.hpp>
#include <boost/fsm/detail/constructor.hpp>
#include <boost/fsm/detail/avoid_unused_warning.hpp>

#include <boost/mpl/list.hpp>
#include <boost/mpl/clear.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/equal_to.hpp>

#include <boost/intrusive_ptr.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/noncopyable.hpp>
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/cast.hpp> // boost::polymorphic_downcast
// BOOST_NO_STD_ALLOCATOR, BOOST_HAS_PARTIAL_STD_ALLOCATOR,
// BOOST_NO_EXCEPTIONS, BOOST_MSVC, BOOST_MSVC_STD_ITERATOR
#include <boost/config.hpp>

#ifdef BOOST_MSVC
#  pragma warning( push )
#  pragma warning( disable: 4702 ) // unreachable code (in release mode only)
#endif

#include <map>

#ifdef BOOST_MSVC
#  pragma warning( pop )
#endif

#include <memory>   // std::allocator
#include <typeinfo> // std::bad_cast
#include <functional> // std::less
#include <iterator>



#ifdef BOOST_MSVC
// We permanently turn off the following level 4 warnings because users will
// have to do so themselves anyway if we turn them back on
#  pragma warning( disable: 4511 ) // copy constructor could not be generated
#  pragma warning( disable: 4512 ) // assignment op could not be generated
#endif



namespace boost
{
namespace fsm
{
namespace detail
{



//////////////////////////////////////////////////////////////////////////////
template< class StateBaseType, class EventBaseType, class IdType >
class send_function
{
  public:
    //////////////////////////////////////////////////////////////////////////
    send_function(
      StateBaseType & toState,
      const EventBaseType & evt,
      IdType eventType
    ) :
      toState_( toState ), evt_( evt ), eventType_( eventType )
    {
    }

    result operator()()
    {
      return toState_.react_impl( evt_, eventType_ );
    }

  private:
    //////////////////////////////////////////////////////////////////////////
    StateBaseType & toState_;
    const EventBaseType & evt_;
    IdType eventType_;
};


//////////////////////////////////////////////////////////////////////////////
struct state_cast_impl_pointer_target
{
  public:
    //////////////////////////////////////////////////////////////////////////
    template< class StateBaseType >
    static const StateBaseType * deref_if_necessary(
      const StateBaseType * pState )
    {
      return pState;
    }

    template< class Target, class IdType >
    static IdType type_id()
    {
      Target p = 0;
      return type_id_impl< IdType >( p );
    }

    static bool found( const void * pFound )
    {
      return pFound != 0;
    }

    template< class Target >
    static Target not_found()
    {
      return 0;
    }

  private:
    //////////////////////////////////////////////////////////////////////////
    template< class IdType, class Type >
    static IdType type_id_impl( const Type * )
    {
      return Type::static_type();
    }
};

struct state_cast_impl_reference_target
{
  template< class StateBaseType >
  static const StateBaseType & deref_if_necessary(
    const StateBaseType * pState )
  {
    return *pState;
  }

  template< class Target, class IdType >
  static IdType type_id()
  {
    return remove_reference< Target >::type::static_type();
  }

  template< class Dummy >
  static bool found( const Dummy & )
  {
    return true;
  }

  template< class Target >
  static Target not_found()
  {
    throw std::bad_cast();
  }
};

template< class Target >
struct state_cast_impl : public mpl::if_<
  is_pointer< Target >,
  state_cast_impl_pointer_target,
  state_cast_impl_reference_target
>::type {};


//////////////////////////////////////////////////////////////////////////////
template< class RttiPolicy >
class history_key
{
  public:
    //////////////////////////////////////////////////////////////////////////
    template< class HistorizedState >
    static history_key make_history_key()
    {
      return history_key(
        HistorizedState::context_type::static_type(),
        HistorizedState::orthogonal_position::value );
    }

    typename RttiPolicy::id_type history_context_type() const
    {
      return historyContextType_;
    }

    friend bool operator<(
      const history_key & left, const history_key & right )
    {
      return
        std::less< typename RttiPolicy::id_type >()( 
          left.historyContextType_, right.historyContextType_ ) ||
        ( ( left.historyContextType_ == right.historyContextType_ ) &&
          ( left.historizedOrthogonalRegion_ <
            right.historizedOrthogonalRegion_ ) );
    }

  private:
    //////////////////////////////////////////////////////////////////////////
    history_key(
      typename RttiPolicy::id_type historyContextType, 
      orthogonal_position_type historizedOrthogonalRegion
    ) :
      historyContextType_( historyContextType ),
      historizedOrthogonalRegion_( historizedOrthogonalRegion )
    {
    }

    const typename RttiPolicy::id_type historyContextType_;
    const orthogonal_position_type historizedOrthogonalRegion_;
};



} // namespace detail



//////////////////////////////////////////////////////////////////////////////
template< class MostDerived,
          class InitialState, 
          class Allocator = std::allocator< void >,
          class ExceptionTranslator = null_exception_translator >
class state_machine : noncopyable
{
  public:
    //////////////////////////////////////////////////////////////////////////
    typedef Allocator allocator_type;
    typedef detail::rtti_policy rtti_policy_type;
    typedef event_base event_base_type;
    typedef intrusive_ptr< const event_base_type > event_base_ptr_type;

    void initiate()
    {
      terminate();

      {
        terminator guard( *this );
        translator_(
          initial_construct_function( *this ),
          exception_event_handler( *this ),
          do_discard_event );
        guard.dismiss();
      }

      process_queued_events();
    }

    void terminate()
    {
      terminator guard( *this );
      translator_(
        terminate_function( *this ),
        exception_event_handler( *this ),
        do_discard_event );
      guard.dismiss();
    }

    bool terminated() const
    {
      return pOutermostState_ == 0;
    }

    void process_event( const event_base_type & evt )
    {
      send_event( evt );
      process_queued_events();
    }

    template< class Target >
    Target state_cast() const
    {
      typedef detail::state_cast_impl< Target > impl;

      for ( typename state_list_type::const_iterator pCurrentLeafState =
              currentStates_.begin();
            pCurrentLeafState != currentStatesEnd_;
            ++pCurrentLeafState )
      {
        const state_base_type * pCurrentState(
          get_pointer( *pCurrentLeafState ) );

        while ( pCurrentState != 0 )
        {
          // The unnecessary try/catch overhead for pointer targets is
          // typically small compared to the cycles dynamic_cast needs
          #ifndef BOOST_NO_EXCEPTIONS
          try
          #endif
          {
            Target result = dynamic_cast< Target >(
              impl::deref_if_necessary( pCurrentState ) );

            if ( impl::found( result ) )
            {
              return result;
            }
          }
          #ifndef BOOST_NO_EXCEPTIONS
          catch ( const std::bad_cast & ) {}
          #endif

          pCurrentState = pCurrentState->outer_state_ptr();
        }
      }

      return impl::template not_found< Target >();
    }

    template< class Target >
    Target state_downcast() const
    {
      typedef detail::state_cast_impl< Target > impl;

      typename rtti_policy_type::id_type targetType =
        impl::template type_id< Target, rtti_policy_type::id_type >();

      for ( typename state_list_type::const_iterator pCurrentLeafState =
              currentStates_.begin();
            pCurrentLeafState != currentStatesEnd_;
            ++pCurrentLeafState )
      {
        const state_base_type * pCurrentState(
          get_pointer( *pCurrentLeafState ) );

        while ( pCurrentState != 0 )
        {
          if ( pCurrentState->dynamic_type() == targetType )
          {
            return static_cast< Target >(
              impl::deref_if_necessary( pCurrentState ) );
          }

          pCurrentState = pCurrentState->outer_state_ptr();
        }
      }

      return impl::template not_found< Target >();
    }

    typedef detail::state_base< allocator_type, rtti_policy_type >
      state_base_type;

    class state_iterator : public std::iterator<
      std::forward_iterator_tag,
      state_base_type, std::ptrdiff_t
      #ifndef BOOST_MSVC_STD_ITERATOR
      , const state_base_type *, const state_base_type &
      #endif
    >
    {
      public:
        //////////////////////////////////////////////////////////////////////
        explicit state_iterator(
          typename state_base_type::state_list_type::const_iterator 
            baseIterator
        ) : baseIterator_( baseIterator ) {}

        const state_base_type & operator*() const { return **baseIterator_; }
        const state_base_type * operator->() const
        {
          return &**baseIterator_;
        }

        state_iterator & operator++() { ++baseIterator_; return *this; }
        state_iterator operator++( int )
        {
          return state_iterator( baseIterator_++ );
        }

        bool operator==( const state_iterator & right ) const
        {
          return baseIterator_ == right.baseIterator_;
        }
        bool operator!=( const state_iterator & right ) const
        {
          return !( *this == right );
        }

      private:
        typename state_base_type::state_list_type::const_iterator
          baseIterator_;
    };

    state_iterator state_begin() const
    {
      return state_iterator( currentStates_.begin() );
    }

    state_iterator state_end() const
    {
      return state_iterator( currentStatesEnd_ );
    }

  protected:
    //////////////////////////////////////////////////////////////////////////
    state_machine() :
      currentStatesEnd_( currentStates_.end() ),
      pOutermostState_( 0 ),
      isInnermostCommonOuter_( false ),
      isExceptionPending_( false )
    {
    }

    // This destructor was only made virtual so that that
    // polymorphic_downcast can be used to cast to MostDerived.
    virtual ~state_machine()
    {
      terminate_impl( false );
    }

  public:
    //////////////////////////////////////////////////////////////////////////
    // The following declarations should be private.
    // They are only public because many compilers lack template friends.
    //////////////////////////////////////////////////////////////////////////
    typedef MostDerived inner_context_type;
    typedef mpl::integral_c< detail::orthogonal_position_type, 0 >
      inner_orthogonal_position;
    typedef mpl::integral_c< detail::orthogonal_position_type, 1 >
      no_of_orthogonal_regions;

    typedef MostDerived outermost_context_type;
    typedef state_machine outermost_context_base_type;
    typedef state_machine * inner_context_ptr_type;
    typedef typename state_base_type::node_state_base_ptr_type
      node_state_base_ptr_type;
    typedef typename state_base_type::leaf_state_ptr_type leaf_state_ptr_type;
    typedef typename state_base_type::state_list_type state_list_type;

    typedef mpl::clear< mpl::list<> >::type context_type_list;

    typedef mpl::bool_< false > shallow_history;
    typedef mpl::bool_< false > deep_history;
    typedef mpl::bool_< false > inherited_deep_history;

    result react_impl(
      const event_base_type &,
      typename rtti_policy_type::id_type )
    {
      return do_forward_event;
    }

    void exit_impl(
      inner_context_ptr_type &,
      typename state_base_type::node_state_base_ptr_type &,
      bool ) {}

    void set_outermost_unstable_state(
      typename state_base_type::node_state_base_ptr_type &
        pOutermostUnstableState )
    {
      pOutermostUnstableState = 0;
    }

    // Returns a reference to the context identified by the template
    // parameter. This can either be _this_ object or one of its direct or
    // indirect contexts.
    template< class Context >
    Context & context()
    {
      // As we are in the outermost context here, only this object can be
      // returned.
      return *polymorphic_downcast< MostDerived * >( this );
    }

    template< class Context >
    const Context & context() const
    {
      // As we are in the outermost context here, only this object can be
      // returned.
      return *polymorphic_downcast< const MostDerived * >( this );
    }

    outermost_context_type & outermost_context()
    {
      return *polymorphic_downcast< MostDerived * >( this );
    }

    const outermost_context_type & outermost_context() const
    {
      return *polymorphic_downcast< const MostDerived * >( this );
    }

    outermost_context_base_type & outermost_context_base()
    {
      return *this;
    }

    const outermost_context_base_type & outermost_context_base() const
    {
      return *this;
    }

    void terminate_as_reaction( state_base_type & theState )
    {
      terminate_impl( theState, !isExceptionPending_ );
      pOutermostUnstableState_ = 0;
    }

    void terminate_as_part_of_transit( state_base_type & theState )
    {
      terminate_impl( theState, !isExceptionPending_ );
      isInnermostCommonOuter_ = true;
    }

    void terminate_as_part_of_transit( state_machine & )
    {
      terminate_impl( *pOutermostState_, !isExceptionPending_ );
    }

    void post_event( const event_base_ptr_type & pEvent )
    {
      BOOST_ASSERT( get_pointer( pEvent ) != 0 );
      eventQueue_.push_back( pEvent );
    }


    template< class State >
    void add(
      const intrusive_ptr< State > & pState,
      bool isInnermostCommonOuter )
    {
      // The second dummy argument is necessary because the call to the
      // overloaded function add_impl would otherwise be ambiguous.
      node_state_base_ptr_type pNewOutermostUnstableStateCandidate =
        add_impl( pState, *pState );

      const state_base_type * const pCurrentOutermostUnstableState =
        get_pointer( pOutermostUnstableState_ );

      // TODO: outer_state_ptr() is a virtual function, which could be called
      // non-virtually here (State is the most-derived type). Investigate why
      // MSVC7.1 has problems with non-virtual call.
      if ( ( pCurrentOutermostUnstableState == 0 ) ||
        ( is_in_highest_orthogonal_region< State >() ||
          isInnermostCommonOuter_ ) &&
        ( pCurrentOutermostUnstableState == pState->outer_state_ptr() ) )
      {
        isInnermostCommonOuter_ = isInnermostCommonOuter;
        pOutermostUnstableState_ = pNewOutermostUnstableStateCandidate;
      }
    }


    void add_inner_state(
      detail::orthogonal_position_type position,
      state_base_type * pOutermostState )
    {
      BOOST_ASSERT( position == 0 );
      detail::avoid_unused_warning( position );
      pOutermostState_ = pOutermostState;
    }

    void remove_inner_state( detail::orthogonal_position_type position )
    {
      BOOST_ASSERT( position == 0 );
      detail::avoid_unused_warning( position );
      pOutermostState_ = 0;
    }


    void defer_event(
      const event_base_type & evt,
      const state_base_type * pForState )
    {
      deferredMap_[ pForState ].push_back( evt.intrusive_from_this() );
    }

    void release_events( const state_base_type * pForState )
    {
      const typename deferred_map_type::iterator pFound =
        deferredMap_.find( pForState );

      // We are not guaranteed to find an entry because a state is marked for
      // having deferred events _before_ the event is actually deferred. An
      // exception might be thrown during deferral.
      if ( pFound != deferredMap_.end() )
      {
        eventQueue_.splice( eventQueue_.end(), pFound->second );
        deferredMap_.erase( pFound );
      }
    }


    template< class HistorizedState >
    void store_shallow_history()
    {
      // 5.2.10.6 declares that reinterpret_casting a function pointer to a
      // different function pointer and back must yield the same value. The
      // following reinterpret_cast is the first half of such a sequence.
      store_history_impl(
        shallowHistoryMap_,
        history_key_type::make_history_key< HistorizedState >(),
        reinterpret_cast< void (*)() >( &HistorizedState::deep_construct ) );
    }

    template<
      class HistoryContext,
      detail::orthogonal_position_type orthogonalPosition >
    void clear_shallow_history()
    {
      // If you receive a
      // "use of undefined type 'boost::STATIC_ASSERTION_FAILURE<x>'" or
      // similar compiler error here then you tried to clear shallow history
      // for a state that does not have shallow history. That is, the state
      // does not pass either fsm::has_shallow_history or
      // fsm::has_full_history to its base class template.
      BOOST_STATIC_ASSERT( HistoryContext::shallow_history::value );

      typedef typename mpl::at_c<
        typename HistoryContext::inner_initial_list,
        orthogonalPosition >::type historized_state;

      store_history_impl(
        shallowHistoryMap_,
        history_key_type::make_history_key< historized_state >(),
        0 );
    }

    template< class DefaultState >
    void construct_with_shallow_history(
      const typename DefaultState::context_ptr_type & pContext )
    {
      construct_with_history_impl< DefaultState >(
        shallowHistoryMap_, pContext );
    }


    template< class HistorizedState, class LeafState >
    void store_deep_history()
    {
      typedef typename detail::make_context_list<
        typename HistorizedState::context_type,
        LeafState >::type history_context_list;
      typedef detail::constructor< 
        history_context_list, outermost_context_base_type > constructor_type;
      // 5.2.10.6 declares that reinterpret_casting a function pointer to a
      // different function pointer and back must yield the same value. The
      // following reinterpret_cast is the first half of such a sequence.
      store_history_impl(
        deepHistoryMap_, 
        history_key_type::make_history_key< HistorizedState >(),
        reinterpret_cast< void (*)() >( &constructor_type::construct ) );
    }

    template<
      class HistoryContext,
      detail::orthogonal_position_type orthogonalPosition >
    void clear_deep_history()
    {
      // If you receive a
      // "use of undefined type 'boost::STATIC_ASSERTION_FAILURE<x>'" or
      // similar compiler error here then you tried to clear deep history for
      // a state that does not have deep history. That is, the state does not
      // pass either fsm::has_deep_history or fsm::has_full_history to its
      // base class template
      BOOST_STATIC_ASSERT( HistoryContext::deep_history::value );

      typedef typename mpl::at_c<
        typename HistoryContext::inner_initial_list,
        orthogonalPosition >::type historized_state;

      store_history_impl(
        deepHistoryMap_,
        history_key_type::make_history_key< historized_state >(),
        0 );
    }

    template< class DefaultState >
    void construct_with_deep_history(
      const typename DefaultState::context_ptr_type & pContext )
    {
      construct_with_history_impl< DefaultState >(
        deepHistoryMap_, pContext );
    }

  private: // implementation
    //////////////////////////////////////////////////////////////////////////
    void initial_construct()
    {
      InitialState::initial_deep_construct(
        *polymorphic_downcast< MostDerived * >( this ) );
    }

    class initial_construct_function
    {
      public:
        //////////////////////////////////////////////////////////////////////
        initial_construct_function( state_machine & machine ) :
          machine_( machine )
        {
        }

        result operator()()
        {
          machine_.initial_construct();
          return do_discard_event; // there is nothing to be consumed
        }

      private:
        //////////////////////////////////////////////////////////////////////
        state_machine & machine_;
    };
    friend class initial_construct_function;

    class terminate_function
    {
      public:
        //////////////////////////////////////////////////////////////////////
        terminate_function( state_machine & machine ) : machine_( machine ) {}

        result operator()()
        {
          machine_.terminate_impl( true );
          return do_discard_event; // there is nothing to be consumed
        }

      private:
        //////////////////////////////////////////////////////////////////////
        state_machine & machine_;
    };
    friend class terminate_function;

    template< class ExceptionEvent >
    bool handle_exception_event(
      const ExceptionEvent & exceptionEvent,
      state_base_type * pCurrentState )
    {
      if ( terminated() )
      {
        // there is no state that could handle the exception -> bail out
        return false;
      }

      // If we are stable, an event handler has thrown.
      // Otherwise, either a state constructor, a transition action or an exit
      // function has thrown and the state machine is now in an invalid state.
      // This situation can be resolved by the exception event handler
      // function by orderly transiting to another state or terminating.
      // As a result of this, the machine must not be unstable when this
      // function is left.
      state_base_type * const pOutermostUnstableState =
        get_pointer( pOutermostUnstableState_ );
      state_base_type * const pHandlingState = pOutermostUnstableState == 0 ?
        pCurrentState : pOutermostUnstableState;

      BOOST_ASSERT( pHandlingState != 0 );

      isExceptionPending_ = true;
      const result reactionResult = pHandlingState->react_impl(
        exceptionEvent, exceptionEvent.dynamic_type() );

      if ( reactionResult == do_defer_event )
      {
        // For intrusive_from_this() to work, the exception event must have
        // been allocated with new. With the library-supplied
        // exception_translator this is not the case, so users will have to
        // craft their own should they ever need to defer an exception event.
        // Deferring exception events makes sense only in very rare cases and
        // is dangerous unless operator new never throws.
        defer_event( exceptionEvent, pHandlingState );
      }

      return ( reactionResult != do_forward_event ) &&
        ( get_pointer( pOutermostUnstableState_ ) == 0 );
    }

    class exception_event_handler
    {
      public:
        //////////////////////////////////////////////////////////////////////
        exception_event_handler(
          state_machine & machine,
          state_base_type * pCurrentState = 0
        ) :
          machine_( machine ),
          pCurrentState_( pCurrentState )
        {
        }

        template< class ExceptionEvent >
        bool operator()( const ExceptionEvent & exceptionEvent )
        {
          return machine_.handle_exception_event(
            exceptionEvent, pCurrentState_ );
        }

      private:
        //////////////////////////////////////////////////////////////////////
        state_machine & machine_;
        state_base_type * pCurrentState_;
    };
    friend class exception_event_handler;

    class terminator
    {
      public:
        terminator( state_machine & machine ) :
          machine_( machine ), dismissed_( false ) {}
        ~terminator()
        {
          if ( !dismissed_ ) { machine_.terminate_impl( false ); }
        }
        void dismiss() { dismissed_ = true; }

      private:
        state_machine & machine_;
        bool dismissed_;
    };
    friend class terminator;


    void send_event( const event_base_type & evt )
    {
      terminator guard( *this );
      BOOST_ASSERT( get_pointer( pOutermostUnstableState_ ) == 0 );
      isExceptionPending_ = false;
      const typename rtti_policy_type::id_type eventType = evt.dynamic_type();
      result reactionResult = do_forward_event;
      
      for (
        typename state_list_type::iterator pState = currentStates_.begin();
        ( reactionResult == do_forward_event ) &&
          ( pState != currentStatesEnd_ );
        ++pState )
      {
        // CAUTION: The following statement could modify our state list!
        // We must not continue iterating if the event was consumed
        reactionResult = translator_( detail::send_function<
          state_base_type, event_base_type, rtti_policy_type::id_type >(
            **pState, evt, eventType ),
          exception_event_handler( *this, get_pointer( *pState ) ),
          do_discard_event );
      }

      guard.dismiss();
    }


    void process_queued_events()
    {
      while ( !eventQueue_.empty() )
      {
        const event_base_ptr_type pCurrentEvent( eventQueue_.front() );
        eventQueue_.pop_front();
        send_event( *pCurrentEvent );
      }
    }


    void terminate_impl( bool callExitActions )
    {
      if ( !terminated() )
      {
        // this also empties deferredMap_
        terminate_impl( *pOutermostState_, callExitActions );
      }

      eventQueue_.clear();
      shallowHistoryMap_.clear();
      deepHistoryMap_.clear();
    }

    void terminate_impl( state_base_type & theState, bool callExitActions )
    {
      isInnermostCommonOuter_ = false;

      // If pOutermostUnstableState_ == 0, we know for sure that
      // currentStates_.size() > 0, otherwise theState couldn't be alive any
      // more
      if ( get_pointer( pOutermostUnstableState_ ) != 0 )
      {
        theState.remove_from_state_list(
          currentStatesEnd_, pOutermostUnstableState_, callExitActions );
      }
      // Optimization: We want to find out whether currentStates_ has size 1
      // and if yes use the optimized implementation below. Since
      // list<>::size() is implemented quite inefficiently in some std libs
      // it is best to just decrement the currentStatesEnd_ here and
      // increment it again, if the test failed.
      else if ( currentStates_.begin() == --currentStatesEnd_ )
      {
        // The machine is stable and there is exactly one innermost state.
        // The following optimization is only correct for a stable machine
        // without orthogonal regions.
        leaf_state_ptr_type & pState = *currentStatesEnd_;
        pState->exit_impl(
          pState, pOutermostUnstableState_, callExitActions );
      }
      else
      {
        BOOST_ASSERT( currentStates_.size() > 1 );
        // The machine is stable and there are multiple innermost states
        theState.remove_from_state_list(
          ++currentStatesEnd_, pOutermostUnstableState_, callExitActions );
      }
    }


    node_state_base_ptr_type add_impl(
      const leaf_state_ptr_type & pState,
      detail::leaf_state< allocator_type, rtti_policy_type > & )
    {
      if ( currentStatesEnd_ == currentStates_.end() )
      {
        pState->set_list_position( 
          currentStates_.insert( currentStatesEnd_, pState ) );
      }
      else
      {
        *currentStatesEnd_ = pState;
        pState->set_list_position( currentStatesEnd_ );
        ++currentStatesEnd_;
      }

      return 0;
    }

    node_state_base_ptr_type add_impl(
      const node_state_base_ptr_type & pState,
      state_base_type & )
    {
      return pState;
    }

    template< class State >
    bool is_in_highest_orthogonal_region()
    {
      return mpl::equal_to<
        typename State::orthogonal_position,
        mpl::minus< 
          typename State::context_type::no_of_orthogonal_regions,
          mpl::integral_c< detail::orthogonal_position_type, 1 > >
      >::value;
    }


    typedef detail::history_key< rtti_policy_type > history_key_type;

    typedef std::map<
      history_key_type, void (*)(),
      std::less< history_key_type >
      #if !defined( BOOST_NO_STD_ALLOCATOR ) && \
        !defined( BOOST_HAS_PARTIAL_STD_ALLOCATOR )
      // TODO: Add allocator support for broken std libs when
      // the workaround is available in boost::detail
      , typename allocator_type::template rebind< 
          std::pair< const history_key_type, void (*)() > >::other
      #endif
    > history_map_type;

    void store_history_impl(
      history_map_type & historyMap,
      const history_key_type & historyId,
      void (*pConstructFunction)() )
    {
      historyMap[ historyId ] = pConstructFunction;
    }

    template< class DefaultState >
    void construct_with_history_impl(
      history_map_type & historyMap,
      const typename DefaultState::context_ptr_type & pContext )
    {
      typename history_map_type::iterator pFoundSlot = historyMap.find(
        history_key_type::make_history_key< DefaultState >() );
      
      if ( pFoundSlot == historyMap.end() || ( pFoundSlot->second == 0 ) )
      {
        // We have never entered this state before or history was cleared
        DefaultState::deep_construct(
          pContext, *polymorphic_downcast< MostDerived * >( this ) );
      }
      else
      {
        typedef void construct_function(
          const typename DefaultState::context_ptr_type &,
          typename DefaultState::outermost_context_base_type & );
        // 5.2.10.6 declares that reinterpret_casting a function pointer to a
        // different function pointer and back must yield the same value. The
        // following reinterpret_cast is the second half of such a sequence.
        construct_function * const pConstructFunction =
          reinterpret_cast< construct_function * >( pFoundSlot->second );
        (*pConstructFunction)(
          pContext, *polymorphic_downcast< MostDerived * >( this ) );
      }
    }

    typedef std::list<
      event_base_ptr_type
      #if !defined( BOOST_NO_STD_ALLOCATOR ) && \
        !defined( BOOST_HAS_PARTIAL_STD_ALLOCATOR )
      // TODO: Add allocator support for broken std libs when
      // the workaround is available in boost::detail
      , typename allocator_type::template rebind< event_base_ptr_type >::other
      #endif
    > event_queue_type;

    typedef std::map<
      const state_base_type *, event_queue_type,
      std::less< const state_base_type * >
      #if !defined( BOOST_NO_STD_ALLOCATOR ) && \
        !defined( BOOST_HAS_PARTIAL_STD_ALLOCATOR )
      // TODO: Add allocator support for broken std libs when
      // the workaround is available in boost::detail
      , typename allocator_type::template rebind< std::pair<
          const state_base_type * const, event_queue_type > >::other
      #endif
    > deferred_map_type;


    event_queue_type eventQueue_;
    deferred_map_type deferredMap_;
    state_list_type currentStates_;
    typename state_list_type::iterator currentStatesEnd_;
    state_base_type * pOutermostState_;
    bool isInnermostCommonOuter_;
    node_state_base_ptr_type pOutermostUnstableState_;
    ExceptionTranslator translator_;
    bool isExceptionPending_;
    history_map_type shallowHistoryMap_;
    history_map_type deepHistoryMap_;
};



} // namespace fsm
} // namespace boost



#endif
