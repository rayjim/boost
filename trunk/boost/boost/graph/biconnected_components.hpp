// Copyright (c) Jeremy Siek 2001
// Copyright (c) Douglas Gregor 2004
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// NOTE: this final is generated by libs/graph/doc/biconnected_components.w

#ifndef BOOST_GRAPH_BICONNECTED_COMPONENTS_HPP
#define BOOST_GRAPH_BICONNECTED_COMPONENTS_HPP

#include <stack>
#include <vector>
#include <algorithm> // for std::min and std::max
#include <boost/config.hpp>
#include <boost/limits.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/property_map.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/graph_utility.hpp>

namespace boost
{
  namespace detail
  {
    template <typename ComponentMap, typename DiscoverTimeMap,
        typename LowPointMap, typename PredecessorMap,
        typename OutputIterator, typename Stack>
    struct biconnected_components_visitor : public dfs_visitor<>
    {
      biconnected_components_visitor
        (ComponentMap comp, std::size_t& c, DiscoverTimeMap dtm,
         std::size_t& dfs_time, LowPointMap lowpt, PredecessorMap pred,
         OutputIterator out, Stack& S)
          : comp(comp), c(c), dtm(dtm), dfs_time(dfs_time), lowpt(lowpt),
            pred(pred), out(out), S(S) { }

      template <typename Vertex, typename Graph>
      void start_vertex(const Vertex& u, Graph&)
      {
        put(pred, u, u);
      }

      template <typename Vertex, typename Graph>
      void discover_vertex(const Vertex& u, Graph&)
      {
        put(dtm, u, ++dfs_time);
        put(lowpt, u, get(dtm, u));
      }

      template <typename Edge, typename Graph>
      void tree_edge(const Edge& e, Graph& g)
      {
        S.push(e);
        put(pred, target(e, g), source(e, g));
      }

      template <typename Edge, typename Graph>
      void back_edge(const Edge& e, Graph& g)
      {
        BOOST_USING_STD_MIN();

        if ( target(e, g) != get(pred, source(e, g)) ) {
          S.push(e);
          put(lowpt, source(e, g),
              min BOOST_PREVENT_MACRO_SUBSTITUTION(get(lowpt, source(e, g)),
                                                   get(dtm, target(e, g))));
        }
      }

      template <typename Vertex, typename Graph>
      void finish_vertex(const Vertex& u, Graph& g)
      {
        BOOST_USING_STD_MIN();
        Vertex parent = get(pred, u);
        bool is_art_point = false;
        if ( get(dtm, parent) > get(dtm, u) ) {
          parent = get(pred, parent);
          is_art_point = true;
        }

        if ( parent == u ) { // at top
          if ( get(dtm, u) + 1 == get(dtm, get(pred, u)) )
            is_art_point = false;
        } else {
          put(lowpt, parent,
              min BOOST_PREVENT_MACRO_SUBSTITUTION(get(lowpt, parent),
                                                   get(lowpt, u)));

          if (get(lowpt, u) >= get(dtm, parent)) {
            if ( get(dtm, parent) > get(dtm, get(pred, parent)) ) {
              put(pred, u, get(pred, parent));
              put(pred, parent, u);
            }

            while ( get(dtm, source(S.top(), g)) >= get(dtm, u) ) {
              put(comp, S.top(), c);
              S.pop();
            }
            put(comp, S.top(), c);
              S.pop();
            ++c;
            if ( S.empty() ) {
              put(pred, u, parent);
              put(pred, parent, u);
            }
          }
        }
        if ( is_art_point )
          *out++ = u;
      }

      ComponentMap comp;
      std::size_t& c;
      DiscoverTimeMap dtm;
      std::size_t& dfs_time;
      LowPointMap lowpt;
      PredecessorMap pred;
      OutputIterator out;
      Stack& S;
    };
  } // namespace detail

  template<typename Graph, typename ComponentMap, typename OutputIterator,
           typename DiscoverTimeMap, typename LowPointMap,
           typename PredecessorMap>
  std::pair<std::size_t, OutputIterator>
  biconnected_components(const Graph & g, ComponentMap comp,
                         OutputIterator out, DiscoverTimeMap discover_time,
                         LowPointMap lowpt, PredecessorMap pred)
  {
    typedef typename graph_traits<Graph>::vertex_descriptor vertex_t;
    typedef typename graph_traits<Graph>::edge_descriptor edge_t;
    function_requires<VertexListGraphConcept<Graph> >();
    function_requires<IncidenceGraphConcept<Graph> >();
    function_requires<WritablePropertyMapConcept<ComponentMap, edge_t> >();
    function_requires<ReadWritePropertyMapConcept<DiscoverTimeMap,
                                                  vertex_t> >();
    function_requires<ReadWritePropertyMapConcept<LowPointMap, vertex_t > >();
    function_requires<ReadWritePropertyMapConcept<PredecessorMap,
                                                  vertex_t> >();

    std::size_t num_components = 0;
    std::size_t dfs_time = 0;
    std::stack < edge_t > S;

    detail::biconnected_components_visitor<ComponentMap, DiscoverTimeMap,
          iterator_property_map<typename std::vector<vertex_t>::iterator,
                                typename property_map<Graph, vertex_index_t>
                                  ::type,
                                vertex_t, vertex_t&>,
          LowPointMap, OutputIterator, std::stack<edge_t> >
      vis(comp, num_components, discover_time, dfs_time, lowpt, pred, out, S);

    depth_first_search(g, visitor(vis));

    return std::pair<std::size_t, OutputIterator>(num_components, vis.out);
  }

  template<typename Graph, typename ComponentMap, typename OutputIterator,
           typename DiscoverTimeMap, typename LowPointMap >
  std::pair<std::size_t, OutputIterator>
  biconnected_components(const Graph & g, ComponentMap comp,
                         OutputIterator out, DiscoverTimeMap discover_time,
                         LowPointMap lowpt)
  {
    typedef typename graph_traits<Graph>::vertex_descriptor vertex_t;
    std::vector<vertex_t> pred(num_vertices(g));

    return biconnected_components(g, comp, out, discover_time, lowpt,
        make_iterator_property_map(pred.begin(), get(vertex_index, g)));
  }

  template<typename Graph, typename ComponentMap, typename OutputIterator,
           typename VertexIndexMap>
  std::pair<std::size_t, OutputIterator>
  biconnected_components(const Graph& g, ComponentMap comp, OutputIterator out,
                         VertexIndexMap index_map)
  {
    typedef typename graph_traits<Graph>::vertex_descriptor vertex_t;
    typedef typename graph_traits<Graph>::vertices_size_type
      vertices_size_type;

    std::vector<vertices_size_type> discover_time(num_vertices(g));
    std::vector<vertices_size_type> lowpt(num_vertices(g));

    return biconnected_components
             (g, comp, out,
              make_iterator_property_map(discover_time.begin(), index_map),
              make_iterator_property_map(lowpt.begin(), index_map));
  }

  template < typename Graph, typename ComponentMap, typename OutputIterator>
  std::pair<std::size_t, OutputIterator>
  biconnected_components(const Graph& g, ComponentMap comp, OutputIterator out)
  {
    return biconnected_components(g, comp, out, get(vertex_index, g));
  }

  namespace graph_detail {
    struct dummy_output_iterator
    {
      typedef std::output_iterator_tag iterator_category;
      typedef void value_type;
      typedef void pointer;
      typedef void difference_type;

      struct reference {
        template<typename T>
        reference& operator=(const T&) { return *this; }
      };

      reference operator*() const { return reference(); }
      dummy_output_iterator& operator++() { return *this; }
      dummy_output_iterator operator++(int) { return *this; }
    };
  } // end namespace graph_detail

  template <typename Graph, typename ComponentMap>
  std::size_t
  biconnected_components(const Graph& g, ComponentMap comp)
  {
    return biconnected_components(g, comp,
                                  graph_detail::dummy_output_iterator()).first;
  }

  template<typename Graph, typename OutputIterator, typename VertexIndexMap>
  OutputIterator
  articulation_points(const Graph& g, OutputIterator out, 
                      VertexIndexMap index_map)
  {
    return biconnected_components(g, dummy_property_map(), out, 
                                  index_map).second;
  }

  template<typename Graph, typename OutputIterator>
  OutputIterator
  articulation_points(const Graph& g, OutputIterator out)
  {
    return biconnected_components(g, dummy_property_map(), out, 
                                  get(vertex_index, g)).second;
  }

}                               // namespace boost

#endif  /* BOOST_GRAPH_BICONNECTED_COMPONENTS_HPP */
