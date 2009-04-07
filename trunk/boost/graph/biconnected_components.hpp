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
#include <boost/property_map/property_map.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/graph_utility.hpp>

namespace boost
{
  namespace detail
  {
    template<typename ComponentMap, typename DiscoverTimeMap,
             typename LowPointMap, typename PredecessorMap,
             typename OutputIterator, typename Stack, 
             typename DFSVisitor>
    struct biconnected_components_visitor : public dfs_visitor<>
    {
      biconnected_components_visitor
        (ComponentMap comp, std::size_t& c, DiscoverTimeMap dtm,
         std::size_t& dfs_time, LowPointMap lowpt, PredecessorMap pred,
         OutputIterator out, Stack& S, DFSVisitor vis)
          : comp(comp), c(c), dtm(dtm), dfs_time(dfs_time), lowpt(lowpt),
            pred(pred), out(out), S(S), vis(vis) { }

      template <typename Vertex, typename Graph>
      void initialize_vertex(const Vertex& u, Graph& g)
      {
        vis.initialize_vertex(u, g);
      }

      template <typename Vertex, typename Graph>
      void start_vertex(const Vertex& u, Graph& g)
      {
        put(pred, u, u);
        vis.start_vertex(u, g);
      }

      template <typename Vertex, typename Graph>
      void discover_vertex(const Vertex& u, Graph& g)
      {
        put(dtm, u, ++dfs_time);
        put(lowpt, u, get(dtm, u));
        vis.discover_vertex(u, g);
      }

      template <typename Edge, typename Graph>
      void examine_edge(const Edge& e, Graph& g)
      {
        vis.examine_edge(e, g);
      }

      template <typename Edge, typename Graph>
      void tree_edge(const Edge& e, Graph& g)
      {
        S.push(e);
        put(pred, target(e, g), source(e, g));
        vis.tree_edge(e, g);
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
        vis.back_edge(e, g);
      }
      }

      template <typename Edge, typename Graph>
      void forward_or_cross_edge(const Edge& e, Graph& g)
      {
        vis.forward_or_cross_edge(e, g);
      }

      template <typename Vertex, typename Graph>
      void finish_vertex(const Vertex& u, Graph& g)
      {
        BOOST_USING_STD_MIN();
        Vertex parent = get(pred, u);
        const std::size_t dtm_of_dubious_parent = get(dtm, parent);
        bool is_art_point = false;
        if ( dtm_of_dubious_parent > get(dtm, u) ) {
          parent = get(pred, parent);
          is_art_point = true;
          put(pred, get(pred, u), u);
          put(pred, u, parent);
        }

        if ( parent == u ) { // at top
          if ( get(dtm, u) + 1 == dtm_of_dubious_parent )
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
        vis.finish_vertex(u, g);
      }

      ComponentMap comp;
      std::size_t& c;
      DiscoverTimeMap dtm;
      std::size_t& dfs_time;
      LowPointMap lowpt;
      PredecessorMap pred;
      OutputIterator out;
      Stack& S;
      DFSVisitor vis;
    };

  template<typename Graph, typename ComponentMap, typename OutputIterator,
        typename VertexIndexMap, typename DiscoverTimeMap, typename LowPointMap,
        typename PredecessorMap, typename DFSVisitor>
  std::pair<std::size_t, OutputIterator>
    biconnected_components_impl(const Graph & g, ComponentMap comp,
        OutputIterator out, VertexIndexMap index_map, DiscoverTimeMap dtm,
        LowPointMap lowpt, PredecessorMap pred, DFSVisitor dfs_vis)
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
      std::stack<edge_t> S;

      biconnected_components_visitor<ComponentMap, DiscoverTimeMap,
          LowPointMap, PredecessorMap, OutputIterator, std::stack<edge_t>, 
          DFSVisitor>
      vis(comp, num_components, dtm, dfs_time, lowpt, pred, out, 
          S, dfs_vis);

    depth_first_search(g, visitor(vis).vertex_index_map(index_map));

    return std::pair<std::size_t, OutputIterator>(num_components, vis.out);
  }

    template <typename PredecessorMap>
    struct bicomp_dispatch3
    {
  template<typename Graph, typename ComponentMap, typename OutputIterator,
                typename VertexIndexMap, typename DiscoverTimeMap, 
                typename LowPointMap, class P, class T, class R>
      static std::pair<std::size_t, OutputIterator> apply (const Graph & g, 
          ComponentMap comp, OutputIterator out, VertexIndexMap index_map, 
          DiscoverTimeMap dtm, LowPointMap lowpt, 
          const bgl_named_params<P, T, R>& params, PredecessorMap pred)
      {
        return biconnected_components_impl
                (g, comp, out, index_map, dtm, lowpt, pred,
                 choose_param(get_param(params, graph_visitor),
                    make_dfs_visitor(null_visitor())));
      }
    };
    
    template <>
    struct bicomp_dispatch3<error_property_not_found>
    {
      template<typename Graph, typename ComponentMap, typename OutputIterator,
                typename VertexIndexMap, typename DiscoverTimeMap, 
                typename LowPointMap, class P, class T, class R>
      static std::pair<std::size_t, OutputIterator> apply (const Graph & g, 
          ComponentMap comp, OutputIterator out, VertexIndexMap index_map, 
          DiscoverTimeMap dtm, LowPointMap lowpt, 
          const bgl_named_params<P, T, R>& params, 
          error_property_not_found)
  {
    typedef typename graph_traits<Graph>::vertex_descriptor vertex_t;
    std::vector<vertex_t> pred(num_vertices(g));
    vertex_t vert = graph_traits<Graph>::null_vertex();

        return biconnected_components_impl
                (g, comp, out, index_map, dtm, lowpt, 
              make_iterator_property_map(pred.begin(), index_map, vert),
                 choose_param(get_param(params, graph_visitor),
                    make_dfs_visitor(null_visitor())));
  }
    };

    template <typename LowPointMap>
    struct bicomp_dispatch2
    {
  template<typename Graph, typename ComponentMap, typename OutputIterator,
                typename VertexIndexMap, typename DiscoverTimeMap, 
                typename P, typename T, typename R>
      static std::pair<std::size_t, OutputIterator> apply (const Graph& g, 
          ComponentMap comp, OutputIterator out, VertexIndexMap index_map, 
          DiscoverTimeMap dtm, const bgl_named_params<P, T, R>& params, 
          LowPointMap lowpt)
      {
        typedef typename property_value< bgl_named_params<P,T,R>,
            vertex_predecessor_t>::type dispatch_type;

        return bicomp_dispatch3<dispatch_type>::apply
            (g, comp, out, index_map, dtm, lowpt, params, 
             get_param(params, vertex_predecessor));
      }
    };


    template <>
    struct bicomp_dispatch2<error_property_not_found>
    {
      template<typename Graph, typename ComponentMap, typename OutputIterator,
                typename VertexIndexMap, typename DiscoverTimeMap, 
                typename P, typename T, typename R>
      static std::pair<std::size_t, OutputIterator> apply (const Graph& g, 
          ComponentMap comp, OutputIterator out, VertexIndexMap index_map, 
          DiscoverTimeMap dtm, const bgl_named_params<P, T, R>& params, 
          error_property_not_found)
  {
    typedef typename graph_traits<Graph>::vertices_size_type
      vertices_size_type;
    std::vector<vertices_size_type> lowpt(num_vertices(g));
        vertices_size_type vst(0);

        typedef typename property_value< bgl_named_params<P,T,R>,
            vertex_predecessor_t>::type dispatch_type;
  
        return bicomp_dispatch3<dispatch_type>::apply
            (g, comp, out, index_map, dtm,
             make_iterator_property_map(lowpt.begin(), index_map, vst),
             params, get_param(params, vertex_predecessor));
      }
    };

    template <typename DiscoverTimeMap>
    struct bicomp_dispatch1
    {
      template<typename Graph, typename ComponentMap, typename OutputIterator,
                typename VertexIndexMap, class P, class T, class R>
      static std::pair<std::size_t, OutputIterator> apply(const Graph& g, 
          ComponentMap comp, OutputIterator out, VertexIndexMap index_map, 
          const bgl_named_params<P, T, R>& params, DiscoverTimeMap dtm)
      {
        typedef typename property_value< bgl_named_params<P,T,R>,
            vertex_lowpoint_t>::type dispatch_type;

        return bicomp_dispatch2<dispatch_type>::apply
            (g, comp, out, index_map, dtm, params, 
             get_param(params, vertex_lowpoint));
      }
    };

    template <>
    struct bicomp_dispatch1<error_property_not_found>
    {
      template<typename Graph, typename ComponentMap, typename OutputIterator,
                typename VertexIndexMap, class P, class T, class R>
      static std::pair<std::size_t, OutputIterator> apply(const Graph& g, 
          ComponentMap comp, OutputIterator out, VertexIndexMap index_map, 
          const bgl_named_params<P, T, R>& params, error_property_not_found)
      {
        typedef typename graph_traits<Graph>::vertices_size_type
            vertices_size_type;
        std::vector<vertices_size_type> discover_time(num_vertices(g));
    vertices_size_type vst(0);

        typedef typename property_value< bgl_named_params<P,T,R>,
            vertex_lowpoint_t>::type dispatch_type;

        return bicomp_dispatch2<dispatch_type>::apply
            (g, comp, out, index_map, 
              make_iterator_property_map(discover_time.begin(), index_map, vst),
             params, get_param(params, vertex_lowpoint));
      }
    };

  }

  template<typename Graph, typename ComponentMap, typename OutputIterator,
      typename DiscoverTimeMap, typename LowPointMap>
  std::pair<std::size_t, OutputIterator>
  biconnected_components(const Graph& g, ComponentMap comp, 
      OutputIterator out, DiscoverTimeMap dtm, LowPointMap lowpt)
  {
    typedef detail::error_property_not_found dispatch_type;

    return detail::bicomp_dispatch3<dispatch_type>::apply
            (g, comp, out, 
             get(vertex_index, g), 
             dtm, lowpt, 
             bgl_named_params<int, buffer_param_t>(0), 
             detail::error_property_not_found());
  }

  template <typename Graph, typename ComponentMap, typename OutputIterator,
      typename P, typename T, typename R>
  std::pair<std::size_t, OutputIterator>
  biconnected_components(const Graph& g, ComponentMap comp, OutputIterator out, 
      const bgl_named_params<P, T, R>& params)
  {
    typedef typename property_value< bgl_named_params<P,T,R>,
        vertex_discover_time_t>::type dispatch_type;

    return detail::bicomp_dispatch1<dispatch_type>::apply(g, comp, out, 
        choose_const_pmap(get_param(params, vertex_index), g, vertex_index), 
        params, get_param(params, vertex_discover_time));
  }

  template < typename Graph, typename ComponentMap, typename OutputIterator>
  std::pair<std::size_t, OutputIterator>
  biconnected_components(const Graph& g, ComponentMap comp, OutputIterator out)
  {
    return biconnected_components(g, comp, out,  
        bgl_named_params<int, buffer_param_t>(0));
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

  template <typename Graph, typename ComponentMap,
      typename P, typename T, typename R>
  std::size_t
  biconnected_components(const Graph& g, ComponentMap comp, 
      const bgl_named_params<P, T, R>& params)
  {
    return biconnected_components(g, comp,
        graph_detail::dummy_output_iterator(), params).first;
  }

  template <typename Graph, typename ComponentMap>
  std::size_t
  biconnected_components(const Graph& g, ComponentMap comp)
  {
    return biconnected_components(g, comp,
                                  graph_detail::dummy_output_iterator()).first;
  }

  template<typename Graph, typename OutputIterator, 
      typename P, typename T, typename R>
  OutputIterator
  articulation_points(const Graph& g, OutputIterator out, 
      const bgl_named_params<P, T, R>& params)
  {
    return biconnected_components(g, dummy_property_map(), out, 
        params).second;
  }

  template<typename Graph, typename OutputIterator>
  OutputIterator
  articulation_points(const Graph& g, OutputIterator out)
  {
    return biconnected_components(g, dummy_property_map(), out, 
        bgl_named_params<int, buffer_param_t>(0)).second;
  }

}                               // namespace boost

#endif  /* BOOST_GRAPH_BICONNECTED_COMPONENTS_HPP */
