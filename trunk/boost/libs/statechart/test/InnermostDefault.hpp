#ifndef BOOST_FSM_TEST_INNERMOST_DEFAULT_HPP_INCLUDED
#define BOOST_FSM_TEST_INNERMOST_DEFAULT_HPP_INCLUDED
//////////////////////////////////////////////////////////////////////////////
// (c) Copyright Andreas Huber Doenni 2004
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////



#include <boost/fsm/state.hpp>



namespace fsm = boost::fsm;



//////////////////////////////////////////////////////////////////////////////
template< class MostDerived, class Context >
struct InnermostDefault : fsm::state< MostDerived, Context >
{
  typedef fsm::state< MostDerived, Context > base_type;
  typedef typename base_type::my_context my_context;
  typedef InnermostDefault my_base;

  InnermostDefault( my_context ctx ) : base_type( ctx )
  {
    this->outermost_context().Entry( typeid( MostDerived ) );
  }

  ~InnermostDefault()
  {
    this->outermost_context().Exit( typeid( MostDerived ) );
  }
};

//////////////////////////////////////////////////////////////////////////////
template< class Context >
struct Default0 : InnermostDefault<
  Default0< Context >, typename Context::template orthogonal< 0 > >
{
  typedef InnermostDefault<
    Default0, typename Context::template orthogonal< 0 > > base_type;
  typedef typename base_type::my_context my_context;

  Default0( my_context ctx ) : base_type( ctx ) {}
};

//////////////////////////////////////////////////////////////////////////////
template< class Context >
struct Default1 : InnermostDefault<
  Default1< Context >, typename Context::template orthogonal< 1 > >
{
  typedef InnermostDefault<
    Default1, typename Context::template orthogonal< 1 > > base_type;
  typedef typename base_type::my_context my_context;

  Default1( my_context ctx ) : base_type( ctx ) {}
};

//////////////////////////////////////////////////////////////////////////////
template< class Context >
struct Default2 : InnermostDefault<
  Default2< Context >, typename Context::template orthogonal< 2 > >
{
  typedef InnermostDefault<
    Default2, typename Context::template orthogonal< 2 > > base_type;
  typedef typename base_type::my_context my_context;

  Default2( my_context ctx ) : base_type( ctx ) {}
};



#endif
