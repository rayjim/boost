//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// This file is part of the Boost Graph Library
//
// You should have received a copy of the License Agreement for the
// Boost Graph Library along with the software; see the file LICENSE.
// If not, contact Office of Research, Indiana University,
// Bloomington, IN 47405.
//
// Permission to modify the code and to distribute the code is
// granted, provided the text of this NOTICE is retained, a notice if
// the code was modified is included with the above COPYRIGHT NOTICE
// and with the COPYRIGHT NOTICE in the LICENSE file, and that the
// LICENSE file is distributed with the modified code.
//
// LICENSOR MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.
// By way of example, but not limitation, Licensor MAKES NO
// REPRESENTATIONS OR WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
// PARTICULAR PURPOSE OR THAT THE USE OF THE LICENSED SOFTWARE COMPONENTS
// OR DOCUMENTATION WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS
// OR OTHER RIGHTS.
//=======================================================================
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;

template < typename Graph > void
generic_foo(Graph & g)
{
  // Access descriptor types
  typedef typename graph_traits < Graph >::vertex_descriptor Vertex;
  typedef typename graph_traits < Graph >::edge_descriptor Edge;
  // Access category types
  typedef typename graph_traits < Graph >::directed_category Dir;
  typedef typename graph_traits < Graph >::edge_parallel_category Par;
  // Access iterator types...
  // Access size types...
  // Now do something useful...
}

template < typename Graph > void
generic_bar(Graph & g)
{
  // Declare some vertex and edge descriptor variables
  typename graph_traits < Graph >::vertex_descriptor u, v;
  typename graph_traits < Graph >::edge_descriptor e1, e2;
  // Set u and e1 to valid descriptors...
  v = u;                        // Make v a handle to the same vertex as u.
  e2 = e1;                      // Make e2 a handle to the same edge as e1.
  assert(u == v);               // Do u and v identify the same vertex? Yes
  assert(!(u != v));            // Do u and v identify different vertices? No
  assert(e1 == e2);             // Do e1 and e2 identify the same edge? Yes
  assert(!(e1 != e2));          // Do e1 and e2 identify different edges? No
}

// This version of foo gets called when g is directed
template < typename Graph > void
foo_dispatch(Graph & g, boost::directed_tag)
{
  //... 
}

// This version of foo gets called when g is undirected
template < typename Graph > void
foo_dispatch(Graph & g, boost::undirected_tag)
{
  //...
}

template < typename Graph > void
foo(Graph & g)
{
  typedef typename boost::graph_traits < Graph >::directed_category Cat;
  foo_dispatch(g, Cat());
}

template < typename Digraph > void
foo(Digraph & digraph,
    typename graph_traits < Digraph >::vertex_descriptor u,
    typename graph_traits < Digraph >::vertex_descriptor v)
{
  std::pair < typename graph_traits < Digraph >::edge_descriptor, bool > e1,
    e2;
  e1 = edge(u, v, digraph);
  e2 = edge(v, u, digraph);
  assert(e1.first != e2.first);
}
template < typename Undigraph > void
bar(Undigraph & undigraph,
    typename graph_traits < Undigraph >::vertex_descriptor u,
    typename graph_traits < Undigraph >::vertex_descriptor v)
{
  std::pair < typename graph_traits < Undigraph >::edge_descriptor, bool > e1,
    e2;
  e1 = edge(u, v, undigraph);
  e2 = edge(v, u, undigraph);
  assert(e1.first == e2.first);
}


int
main()
{

  boost::adjacency_list < vecS, vecS, directedS > g(2);
  add_edge(0, 1, g);
  add_edge(1, 0, g);
  generic_foo(g);
  generic_bar(g);
  foo(g);
  foo(g, vertex(0, g), vertex(1, g));

  boost::adjacency_list < vecS, vecS, undirectedS > ug(2);
  add_edge(0, 1, g);
  bar(ug, vertex(0, g), vertex(1, g));

  return 0;
}
