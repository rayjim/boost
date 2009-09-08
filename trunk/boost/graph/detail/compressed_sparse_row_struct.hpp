// Copyright 2005-2009 The Trustees of Indiana University.

// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Jeremiah Willcock
//           Douglas Gregor
//           Andrew Lumsdaine

// Compressed sparse row graph type internal structure

#ifndef BOOST_GRAPH_COMPRESSED_SPARSE_ROW_STRUCT_HPP
#define BOOST_GRAPH_COMPRESSED_SPARSE_ROW_STRUCT_HPP

#ifndef BOOST_GRAPH_COMPRESSED_SPARSE_ROW_GRAPH_HPP
#error This file should only be included from boost/graph/compressed_sparse_row_graph.hpp
#endif

#include <vector>
#include <utility>
#include <algorithm>
#include <climits>
#include <cassert>
#include <iterator>
#if 0
#include <iostream> // For some debugging code below
#endif
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/filtered_graph.hpp> // For keep_all
#include <boost/graph/detail/indexed_properties.hpp>
#include <boost/graph/detail/histogram_sort.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/reverse_iterator.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/integer.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/mpl/if.hpp>
#include <boost/graph/graph_selectors.hpp>
#include <boost/static_assert.hpp>
#include <boost/functional/hash.hpp>
#include <boost/utility.hpp>

namespace boost {

// Forward declaration of CSR edge descriptor type, needed to pass to
// indexed_edge_properties.
template<typename Vertex, typename EdgeIndex>
class csr_edge_descriptor;

namespace detail {
  /** Compressed sparse row graph internal structure.
   *
   * Vertex and EdgeIndex should be unsigned integral types and should
   * specialize numeric_limits.
   */
  template <typename EdgeProperty,
            typename Vertex = std::size_t, typename EdgeIndex = Vertex>
  class compressed_sparse_row_structure :
    public detail::indexed_edge_properties<
             compressed_sparse_row_structure<EdgeProperty, Vertex, EdgeIndex>,
             EdgeProperty,
             csr_edge_descriptor<Vertex, EdgeIndex> > {
    public:
    typedef detail::indexed_edge_properties<
              compressed_sparse_row_structure<EdgeProperty, Vertex, EdgeIndex>,
              EdgeProperty,
              csr_edge_descriptor<Vertex, EdgeIndex> >
      inherited_edge_properties;

    typedef Vertex vertices_size_type;
    typedef Vertex vertex_descriptor;
    typedef EdgeIndex edges_size_type;

    static vertex_descriptor null_vertex() { return vertex_descriptor(-1); }

    std::vector<EdgeIndex> m_rowstart;
    std::vector<Vertex> m_column;
#ifdef BOOST_GRAPH_USE_OLD_CSR_INTERFACE
    // This member is only needed to support add_edge(), which is not provided by
    // the new interface
    Vertex m_last_source; // Last source of added edge, plus one
#endif // BOOST_GRAPH_USE_OLD_CSR_INTERFACE

    compressed_sparse_row_structure(Vertex numverts = 0)
      : m_rowstart(numverts + 1, EdgeIndex(0)), m_column()
#ifdef BOOST_GRAPH_USE_OLD_CSR_INTERFACE
        , m_last_source(numverts)
#endif
      {}
    
    //  Rebuild graph from number of vertices and multi-pass unsorted list of
    //  edges (filtered using source_pred and mapped using global_to_local)
    template <typename MultiPassInputIterator, typename GlobalToLocal, typename SourcePred>
    void
    assign_unsorted_multi_pass_edges(MultiPassInputIterator edge_begin,
                                     MultiPassInputIterator edge_end,
                                     vertices_size_type numlocalverts,
                                     const GlobalToLocal& global_to_local,
                                     const SourcePred& source_pred) {
      m_rowstart.clear();
      m_rowstart.resize(numlocalverts + 1, 0);
      typedef std::pair<vertices_size_type, vertices_size_type> edge_type;
      typedef boost::transform_iterator<boost::graph::detail::project1st<edge_type>, MultiPassInputIterator> source_iterator;
      typedef boost::transform_iterator<boost::graph::detail::project2nd<edge_type>, MultiPassInputIterator> target_iterator;
      source_iterator sources_begin(edge_begin, boost::graph::detail::project1st<edge_type>());
      source_iterator sources_end(edge_end, boost::graph::detail::project1st<edge_type>());
      target_iterator targets_begin(edge_begin, boost::graph::detail::project2nd<edge_type>());
      target_iterator targets_end(edge_end, boost::graph::detail::project2nd<edge_type>());

      boost::graph::detail::count_starts
        (sources_begin, sources_end, m_rowstart.begin(), numlocalverts,
         source_pred, make_property_map_function(global_to_local));

      m_column.resize(m_rowstart.back());

      boost::graph::detail::histogram_sort
        (sources_begin, sources_end, m_rowstart.begin(), numlocalverts,
         targets_begin, m_column.begin(),
         source_pred, make_property_map_function(global_to_local));
    }

    //  Rebuild graph from number of vertices and multi-pass unsorted list of
    //  edges and their properties (filtered using source_pred and mapped using
    //  global_to_local)
    template <typename MultiPassInputIterator, typename EdgePropertyIterator, typename GlobalToLocal, typename SourcePred>
    void
    assign_unsorted_multi_pass_edges(MultiPassInputIterator edge_begin,
                                     MultiPassInputIterator edge_end,
                                     EdgePropertyIterator ep_iter,
                                     vertices_size_type numlocalverts,
                                     const GlobalToLocal& global_to_local,
                                     const SourcePred& source_pred) {
      m_rowstart.clear();
      m_rowstart.resize(numlocalverts + 1, 0);
      typedef std::pair<vertices_size_type, vertices_size_type> edge_type;
      typedef boost::transform_iterator<boost::graph::detail::project1st<edge_type>, MultiPassInputIterator> source_iterator;
      typedef boost::transform_iterator<boost::graph::detail::project2nd<edge_type>, MultiPassInputIterator> target_iterator;
      source_iterator sources_begin(edge_begin, boost::graph::detail::project1st<edge_type>());
      source_iterator sources_end(edge_end, boost::graph::detail::project1st<edge_type>());
      target_iterator targets_begin(edge_begin, boost::graph::detail::project2nd<edge_type>());
      target_iterator targets_end(edge_end, boost::graph::detail::project2nd<edge_type>());

      boost::graph::detail::count_starts
        (sources_begin, sources_end, m_rowstart.begin(), numlocalverts,
         source_pred, make_property_map_function(global_to_local));

      m_column.resize(m_rowstart.back());
      inherited_edge_properties::resize(m_rowstart.back());

      boost::graph::detail::histogram_sort
        (sources_begin, sources_end, m_rowstart.begin(), numlocalverts,
         targets_begin, m_column.begin(),
         ep_iter, inherited_edge_properties::begin(),
         source_pred, make_property_map_function(global_to_local));
    }

    //  Assign from number of vertices and sorted list of edges
    template<typename InputIterator, typename GlobalToLocal, typename SourcePred>
    void assign_from_sorted_edges(
           InputIterator edge_begin, InputIterator edge_end,
           const GlobalToLocal& global_to_local,
           const SourcePred& source_pred,
           vertices_size_type numlocalverts,
           edges_size_type numedges_or_zero) {
      m_column.clear();
      m_column.reserve(numedges_or_zero);
      m_rowstart.resize(numlocalverts + 1);
      EdgeIndex current_edge = 0;
      Vertex current_vertex_plus_one = 1;
      m_rowstart[0] = 0;
      for (InputIterator ei = edge_begin; ei != edge_end; ++ei) {
        if (!source_pred(ei->first)) continue;
        Vertex src = get(global_to_local, ei->first);
        Vertex tgt = ei->second;
        for (; current_vertex_plus_one != src + 1; ++current_vertex_plus_one)
          m_rowstart[current_vertex_plus_one] = current_edge;
        m_column.push_back(tgt);
        ++current_edge;
      }

      // The remaining vertices have no edges
      for (; current_vertex_plus_one != numlocalverts + 1; ++current_vertex_plus_one)
        m_rowstart[current_vertex_plus_one] = current_edge;

      // Default-construct properties for edges
      inherited_edge_properties::resize(m_column.size());
    }

    //  Assign from number of vertices and sorted list of edges
    template<typename InputIterator, typename EdgePropertyIterator, typename GlobalToLocal, typename SourcePred>
    void assign_from_sorted_edges(
           InputIterator edge_begin, InputIterator edge_end,
           EdgePropertyIterator ep_iter,
           const GlobalToLocal& global_to_local,
           const SourcePred& source_pred,
           vertices_size_type numlocalverts,
           edges_size_type numedges_or_zero) {
      // Reserving storage in advance can save us lots of time and
      // memory, but it can only be done if we have forward iterators or
      // the user has supplied the number of edges.
      edges_size_type numedges = numedges_or_zero;
      if (numedges == 0) {
        typedef typename std::iterator_traits<InputIterator>::iterator_category
          category;
        numedges = boost::graph::detail::reserve_count_for_single_pass(edge_begin, edge_end);
      }
      m_column.clear();
      m_column.reserve(numedges_or_zero);
      inherited_edge_properties::clear();
      inherited_edge_properties::reserve(numedges_or_zero);
      m_rowstart.resize(numlocalverts + 1);
      EdgeIndex current_edge = 0;
      Vertex current_vertex_plus_one = 1;
      m_rowstart[0] = 0;
      for (InputIterator ei = edge_begin; ei != edge_end; ++ei, ++ep_iter) {
        if (!source_pred(ei->first)) continue;
        Vertex src = get(global_to_local, ei->first);
        Vertex tgt = ei->second;
        for (; current_vertex_plus_one != src + 1; ++current_vertex_plus_one)
          m_rowstart[current_vertex_plus_one] = current_edge;
        m_column.push_back(tgt);
        inherited_edge_properties::push_back(*ep_iter);
        ++current_edge;
      }

      // The remaining vertices have no edges
      for (; current_vertex_plus_one != numlocalverts + 1; ++current_vertex_plus_one)
        m_rowstart[current_vertex_plus_one] = current_edge;
    }

    // Replace graph with sources and targets given, sorting them in-place, and
    // using the given global-to-local property map to get local indices from
    // global ones in the two arrays.
    template <typename GlobalToLocal>
    void assign_sources_and_targets_global(std::vector<vertex_descriptor>& sources,
                                           std::vector<vertex_descriptor>& targets,
                                           vertices_size_type numverts,
                                           GlobalToLocal global_to_local) {
      assert (sources.size() == targets.size());
      // Do an in-place histogram sort (at least that's what I think it is) to
      // sort sources and targets
      m_rowstart.clear();
      m_rowstart.resize(numverts + 1);
      boost::graph::detail::count_starts
        (sources.begin(), sources.end(), m_rowstart.begin(), numverts,
         keep_all(), make_property_map_function(global_to_local));
      boost::graph::detail::histogram_sort_inplace
        (sources.begin(), sources.end(), m_rowstart.begin(), numverts,
         targets.begin(), make_property_map_function(global_to_local));
      // Now targets is the correct vector (properly sorted by source) for
      // m_column
      m_column.swap(targets);
    }

    // Replace graph with sources and targets and edge properties given, sorting
    // them in-place, and using the given global-to-local property map to get
    // local indices from global ones in the two arrays.
    template <typename GlobalToLocal>
    void assign_sources_and_targets_global(std::vector<vertex_descriptor>& sources,
                                           std::vector<vertex_descriptor>& targets,
                                           std::vector<typename inherited_edge_properties::edge_bundled>& edge_props,
                                           vertices_size_type numverts,
                                           GlobalToLocal global_to_local) {
      assert (sources.size() == targets.size());
      assert (sources.size() == edge_props.size());
      // Do an in-place histogram sort (at least that's what I think it is) to
      // sort sources and targets
      m_rowstart.clear();
      m_rowstart.resize(numverts + 1);
      boost::graph::detail::count_starts
        (sources.begin(), sources.end(), m_rowstart.begin(), numverts,
         keep_all(), make_property_map_function(global_to_local));
      boost::graph::detail::histogram_sort_inplace
        (sources.begin(), sources.end(), m_rowstart.begin(), numverts,
         targets.begin(), edge_props.begin(),
         make_property_map_function(global_to_local));
      // Now targets is the correct vector (properly sorted by source) for
      // m_column, and edge_props for m_edge_properties
      m_column.swap(targets);
      this->m_edge_properties.swap(edge_props);
    }

    // From any graph (slow and uses a lot of memory)
    //   Requires IncidenceGraph and a vertex index map
    //   Internal helper function
    //   Note that numedges must be doubled for undirected source graphs
    template<typename Graph, typename VertexIndexMap>
    void
    assign(const Graph& g, const VertexIndexMap& vi,
           vertices_size_type numverts, edges_size_type numedges)
    {
      m_rowstart.resize(numverts + 1);
      m_column.resize(numedges);
      EdgeIndex current_edge = 0;
      typedef typename boost::graph_traits<Graph>::vertex_descriptor g_vertex;
      typedef typename boost::graph_traits<Graph>::edge_descriptor g_edge;
      typedef typename boost::graph_traits<Graph>::out_edge_iterator
        g_out_edge_iter;

      std::vector<g_vertex> ordered_verts_of_g(numverts);
      BGL_FORALL_VERTICES_T(v, g, Graph) {
        ordered_verts_of_g[get(vertex_index, g, v)] = v;
      }
      for (Vertex i = 0; i != numverts; ++i) {
        m_rowstart[i] = current_edge;
        g_vertex v = ordered_verts_of_g[i];
#ifdef BOOST_GRAPH_USE_OLD_CSR_INTERFACE
        // Out edges in a single vertex are only sorted for the old interface
        EdgeIndex num_edges_before_this_vertex = current_edge;
#endif // BOOST_GRAPH_USE_OLD_CSR_INTERFACE
        g_out_edge_iter ei, ei_end;
        for (tie(ei, ei_end) = out_edges(v, g); ei != ei_end; ++ei) {
          m_column[current_edge++] = get(vi, target(*ei, g));
        }
#ifdef BOOST_GRAPH_USE_OLD_CSR_INTERFACE
        // Out edges in a single vertex are only sorted for the old interface
        std::sort(m_column.begin() + num_edges_before_this_vertex,
                  m_column.begin() + current_edge);
#endif // BOOST_GRAPH_USE_OLD_CSR_INTERFACE
      }
      m_rowstart[numverts] = current_edge;
#ifdef BOOST_GRAPH_USE_OLD_CSR_INTERFACE
      m_last_source = numverts;
#endif // BOOST_GRAPH_USE_OLD_CSR_INTERFACE
    }

    // Add edges from a sorted (smallest sources first) range of pairs and edge
    // properties
    template <typename BidirectionalIteratorOrig, typename EPIterOrig,
              typename GlobalToLocal>
    void
    add_edges_sorted_internal(
        BidirectionalIteratorOrig first_sorted,
        BidirectionalIteratorOrig last_sorted,
        EPIterOrig ep_iter_sorted,
        const GlobalToLocal& global_to_local) {
      typedef boost::reverse_iterator<BidirectionalIteratorOrig> BidirectionalIterator;
      typedef boost::reverse_iterator<EPIterOrig> EPIter;
      // Flip sequence
      BidirectionalIterator first(last_sorted);
      BidirectionalIterator last(first_sorted);
      typedef Vertex vertex_t;
      typedef Vertex vertex_num;
      typedef EdgeIndex edge_num;
      edge_num new_edge_count = std::distance(first, last);

      EPIter ep_iter(ep_iter_sorted);
      std::advance(ep_iter, -(std::ptrdiff_t)new_edge_count);
      edge_num edges_added_before_i = new_edge_count; // Count increment to add to rowstarts
      m_column.resize(m_column.size() + new_edge_count);
      inherited_edge_properties::resize(inherited_edge_properties::size() + new_edge_count);
      BidirectionalIterator current_new_edge = first, prev_new_edge = first;
      EPIter current_new_edge_prop = ep_iter;
      for (vertex_num i_plus_1 = m_rowstart.size() - 1; i_plus_1 > 0; --i_plus_1) {
        vertex_num i = i_plus_1 - 1;
        prev_new_edge = current_new_edge;
        // edges_added_to_this_vertex = #mbrs of new_edges with first == i
        edge_num edges_added_to_this_vertex = 0;
        while (current_new_edge != last) {
          if (get(global_to_local, current_new_edge->first) != i) break;
          ++current_new_edge;
          ++current_new_edge_prop;
          ++edges_added_to_this_vertex;
        }
        edges_added_before_i -= edges_added_to_this_vertex;
        // Invariant: edges_added_before_i = #mbrs of new_edges with first < i
        edge_num old_rowstart = m_rowstart[i];
        edge_num new_rowstart = m_rowstart[i] + edges_added_before_i;
        edge_num old_degree = m_rowstart[i + 1] - m_rowstart[i];
        edge_num new_degree = old_degree + edges_added_to_this_vertex;
        // Move old edges forward (by #new_edges before this i) to make room
        // new_rowstart > old_rowstart, so use copy_backwards
        if (old_rowstart != new_rowstart) {
          std::copy_backward(m_column.begin() + old_rowstart,
                             m_column.begin() + old_rowstart + old_degree,
                             m_column.begin() + new_rowstart + old_degree);
          inherited_edge_properties::move_range(old_rowstart, old_rowstart + old_degree, new_rowstart);
        }
        // Add new edges (reversed because current_new_edge is a
        // const_reverse_iterator)
        BidirectionalIterator temp = current_new_edge;
        EPIter temp_prop = current_new_edge_prop;
        for (; temp != prev_new_edge; ++old_degree) {
          --temp;
          --temp_prop;
          m_column[new_rowstart + old_degree] = temp->second;
          inherited_edge_properties::write_by_index(new_rowstart + old_degree, *temp_prop);
        }
        m_rowstart[i + 1] = new_rowstart + new_degree;
        if (edges_added_before_i == 0) break; // No more edges inserted before this point
        // m_rowstart[i] will be fixed up on the next iteration (to avoid
        // changing the degree of vertex i - 1); the last iteration never changes
        // it (either because of the condition of the break or because
        // m_rowstart[0] is always 0)
      }
    }

  };

} // namespace detail
} // namespace boost

#endif // BOOST_GRAPH_COMPRESSED_SPARSE_ROW_STRUCT_HPP
