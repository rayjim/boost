//
//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// This file is part of the Boost Graph Library
//
// You should have received a copy of the License Agreement for the
// Boost Graph Library along with the software; see the file LICENSE.
// If not, contact Office of Research, University of Notre Dame, Notre
// Dame, IN 46556.
//
// Permission to modify the code and to distribute modified code is
// granted, provided the text of this NOTICE is retained, a notice that
// the code was modified is included with the above COPYRIGHT NOTICE and
// with the COPYRIGHT NOTICE in the LICENSE file, and that the LICENSE
// file is distributed with the modified code.
//
// LICENSOR MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.
// By way of example, but not limitation, Licensor MAKES NO
// REPRESENTATIONS OR WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
// PARTICULAR PURPOSE OR THAT THE USE OF THE LICENSED SOFTWARE COMPONENTS
// OR DOCUMENTATION WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS
// OR OTHER RIGHTS.
//=======================================================================
//
#ifndef BOOST_GRAPH_BREADTH_FIRST_SEARCH_HPP
#define BOOST_GRAPH_BREADTH_FIRST_SEARCH_HPP

/*
  Breadth First Search Algorithm (Cormen, Leiserson, and Rivest p. 470)
*/
#include <boost/config.hpp>
#include <vector>
#include <boost/pending/queue.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/named_function_params.hpp>

namespace boost {

  template <class Visitor, class Graph>
  struct BFSVisitorConcept {
    void constraints() {
      function_requires< CopyConstructibleConcept<Visitor> >();
      vis.initialize_vertex(u, g);
      vis.discover_vertex(u, g);
      vis.examine_vertex(u, g);
      vis.examine_edge(e, g);
      vis.tree_edge(e, g);
      vis.non_tree_edge(e, g);
      vis.gray_target(e, g);
      vis.black_target(e, g);
      vis.finish_vertex(u, g);
    }
    Visitor vis;
    Graph g;
    typename graph_traits<Graph>::vertex_descriptor u;
    typename graph_traits<Graph>::edge_descriptor e;
  };

  template <class Visitors = null_visitor>
  class bfs_visitor {
  public:
    bfs_visitor(Visitors vis = Visitors()) : m_vis(vis) { }

    template <class Vertex, class Graph>
    void initialize_vertex(Vertex u, Graph& g) {
      invoke_visitors(m_vis, u, g, on_initialize_vertex());      
    }
    template <class Vertex, class Graph>
    void discover_vertex(Vertex u, Graph& g) {
      invoke_visitors(m_vis, u, g, on_discover_vertex());      
    }
    template <class Vertex, class Graph>
    void examine_vertex(Vertex u, Graph& g) {
      invoke_visitors(m_vis, u, g, on_examine_vertex());
    }
    template <class Edge, class Graph>
    void examine_edge(Edge e, Graph& g) {
      invoke_visitors(m_vis, e, g, on_examine_edge());
    }
    template <class Edge, class Graph>
    void tree_edge(Edge e, Graph& g) {
      invoke_visitors(m_vis, e, g, on_tree_edge());      
    }
    template <class Edge, class Graph>
    void non_tree_edge(Edge e, Graph& g) {
      invoke_visitors(m_vis, e, g, on_non_tree_edge());
    }
    template <class Edge, class Graph>
    void gray_target(Edge e, Graph& g) {
      invoke_visitors(m_vis, e, g, on_gray_target());
    }
    template <class Edge, class Graph>
    void black_target(Edge e, Graph& g) {
      invoke_visitors(m_vis, e, g, on_black_target());
    }
    template <class Vertex, class Graph>
    void finish_vertex(Vertex u, Graph& g) {
      invoke_visitors(m_vis, u, g, on_finish_vertex());      
    }
  protected:
    Visitors m_vis;
  };
  template <class Visitors>
  bfs_visitor<Visitors>
  make_bfs_visitor(Visitors vis) {
    return bfs_visitor<Visitors>(vis);
  }

  namespace detail {

    template <class IncidenceGraph, class Buffer, class BFSVisitor, 
              class ColorMap>
    void bfs_impl(const IncidenceGraph& g, 
		  typename graph_traits<IncidenceGraph>::vertex_descriptor s, 
		  Buffer& Q, BFSVisitor vis, ColorMap color)
    {
      function_requires< IncidenceGraphConcept<IncidenceGraph> >();
      typedef graph_traits<IncidenceGraph> GTraits;
      typedef typename GTraits::vertex_descriptor Vertex;
      typedef typename GTraits::edge_descriptor Edge;
      function_requires< BFSVisitorConcept<BFSVisitor, IncidenceGraph> >();
      function_requires< ReadWritePropertyMapConcept<ColorMap, Vertex> >();
      typedef typename property_traits<ColorMap>::value_type ColorValue;
      typedef color_traits<ColorValue> Color;
      
      put(color, s, Color::gray());
      vis.discover_vertex(s, g);
      Q.push(s);
      while (! Q.empty()) {
	Vertex u = Q.top();
	Q.pop(); // pop before push to avoid problem if Q is priority_queue.
	vis.examine_vertex(u, g);
	typename GTraits::out_edge_iterator ei, ei_end;
	for (tie(ei, ei_end) = out_edges(u, g); ei != ei_end; ++ei) {
	  Edge e = *ei;
	  vis.examine_edge(e, g);
	  Vertex v = target(e, g);
	  if (get(color, v) == Color::white()) {
	    vis.tree_edge(e, g);
	    put(color, v, Color::gray());
	    vis.discover_vertex(v, g);
	    Q.push(v);
	  } else {
	    vis.non_tree_edge(e, g);
	    
	    if (get(color, v) == Color::gray())
	      vis.gray_target(e, g);
	    else
	      vis.black_target(e, g);
	  }
	} // for
	put(color, u, Color::black());
	vis.finish_vertex(u, g);
      } // while
    }
    
    template <class VertexListGraph, class ColorMap, class BFSVisitor,
      class P, class T, class R>
    void bfs_helper
      (VertexListGraph& g,
       typename graph_traits<VertexListGraph>::vertex_descriptor s,
       ColorMap color, 
       BFSVisitor vis,
       const bgl_named_params<P, T, R>& params)
    {
      typedef graph_traits<VertexListGraph> Traits;
      // Buffer default
      typedef typename Traits::vertex_descriptor Vertex;
      typedef boost::queue<Vertex> queue_t;
      queue_t Q;
      detail::wrap_ref<queue_t> Qref(Q);
      // Initialization
      typedef typename property_traits<ColorMap>::value_type ColorValue;
      typedef color_traits<ColorValue> Color;
      typename boost::graph_traits<VertexListGraph>::vertex_iterator i, i_end;
      for (tie(i, i_end) = vertices(g); i != i_end; ++i) {
        put(color, *i, Color::white());
        vis.initialize_vertex(*i, g);
      }
      bfs_impl
        (g, s, 
         choose_param(get_param(params, buffer_param_t()), Qref).ref,
         vis, color);
    }

    //-------------------------------------------------------------------------
    // Choose between default color and color parameters. Using
    // function dispatching so that we don't require vertex index if
    // the color default is not being used.

    template <class ColorMap>
    struct bfs_dispatch {
      template <class VertexListGraph, class P, class T, class R>
      static void apply
      (VertexListGraph& g,
       typename graph_traits<VertexListGraph>::vertex_descriptor s,
       const bgl_named_params<P, T, R>& params,
       ColorMap color)
      {
	bfs_helper
	  (g, s, color,
	   choose_param(get_param(params, graph_visitor),
			make_bfs_visitor(null_visitor())),
	   params);
      }
    };

    template <>
    struct bfs_dispatch<detail::error_property_not_found> {
      template <class VertexListGraph, class P, class T, class R>
      static void apply
      (VertexListGraph& g,
       typename graph_traits<VertexListGraph>::vertex_descriptor s,
       const bgl_named_params<P, T, R>& params,
       detail::error_property_not_found)
      {
	std::vector<default_color_type> color_vec(num_vertices(g));
	null_visitor null_vis;
	
	bfs_helper
	  (g, s, 
	   make_iterator_property_map
	   (color_vec.begin(), 
	    choose_const_pmap(get_param(params, vertex_index), 
			      g, vertex_index), color_vec[0]),
	   choose_param(get_param(params, graph_visitor),
			make_bfs_visitor(null_vis)),
	   params);
      }
    };

  } // namespace detail


  // Named Parameter Variant
  template <class VertexListGraph, class P, class T, class R>
  void breadth_first_search
    (VertexListGraph& g,
     typename graph_traits<VertexListGraph>::vertex_descriptor s,
     const bgl_named_params<P, T, R>& params)
  {
    typedef typename property_value< bgl_named_params<P,T,R>, vertex_color_t>::type C;
    detail::bfs_dispatch<C>::apply(g, s, params, get_param(params, vertex_color));
  }


  // This version does not initialize colors, user has to.

  template <class IncidenceGraph, class P, class T, class R>
  void breadth_first_visit
    (IncidenceGraph& g,
     typename graph_traits<IncidenceGraph>::vertex_descriptor s,
     const bgl_named_params<P, T, R>& params)
  {
    typedef graph_traits<IncidenceGraph> Traits;
    // Buffer default
    typedef boost::queue<typename Traits::vertex_descriptor> queue_t;
    queue_t Q;
    detail::wrap_ref<queue_t> Qref(Q);

    breadth_first_search
      (g, s,
       choose_param(get_param(params, buffer_param_t()), Qref).ref,
       choose_param(get_param(params, graph_visitor),
                    make_bfs_visitor(null_visitor())),
       choose_pmap(get_param(params, vertex_color), g, vertex_color)
       );
  }

} // namespace boost

#endif // BOOST_GRAPH_BREADTH_FIRST_SEARCH_HPP

