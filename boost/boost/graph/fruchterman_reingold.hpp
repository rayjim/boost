// Copyright 2004 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#ifndef BOOST_GRAPH_FRUCHTERMAN_REINGOLD_FORCE_DIRECTED_LAYOUT_HPP
#define BOOST_GRAPH_FRUCHTERMAN_REINGOLD_FORCE_DIRECTED_LAYOUT_HPP

#include <cmath>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/simple_point.hpp>
#include <vector>
#include <list>

namespace boost {

struct square_distance_attractive_force {
  template<typename Graph, typename T>
  T
  operator()(typename graph_traits<Graph>::edge_descriptor,
             T k,
             T d,
             const Graph&) const
  {
    return d * d / k;
  }
};

struct square_distance_repulsive_force {
  template<typename Graph, typename T>
  T
  operator()(typename graph_traits<Graph>::vertex_descriptor,
             typename graph_traits<Graph>::vertex_descriptor,
             T k,
             T d,
             const Graph&) const
  {
    return k * k / d;
  }
};

template<typename T>
struct linear_cooling {
  typedef T result_type;

  linear_cooling(std::size_t iterations)
    : temp(T(iterations) / T(10)), step(0.1) { }

  linear_cooling(std::size_t iterations, T temp)
    : temp(temp), step(temp / T(iterations)) { }
                                           
  T operator()()
  {
    T old_temp = temp;
    temp -= step;
    if (temp < T(0)) temp = T(0);
    return old_temp;
  }

 private:
  T temp;
  T step;
};

struct all_force_pairs
{
  template<typename Graph, typename ApplyForce >
  void operator()(const Graph& g, ApplyForce apply_force)
  {
    typedef typename graph_traits<Graph>::vertex_iterator vertex_iterator;
    vertex_iterator v, end;
    for (tie(v, end) = vertices(g); v != end; ++v) {
      vertex_iterator u = v;
      for (++u; u != end; ++u) {
        apply_force(*u, *v);
	apply_force(*v, *u);
      }
    }
  }
};

template<typename Dim, typename PositionMap>
struct grid_force_pairs
{
  explicit grid_force_pairs(Dim width, Dim height, PositionMap position) 
    : width(width), height(height), position(position)
  { 
    using std::sqrt;
    two_k = Dim(2) * sqrt(width*height);
  }

  template<typename Graph, typename ApplyForce >
  void operator()(const Graph& g, ApplyForce apply_force)
  {
    typedef typename graph_traits<Graph>::vertex_iterator vertex_iterator;
    typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
    typedef std::list<vertex_descriptor> bucket_t;
    typedef std::vector<bucket_t> buckets_t;

    std::size_t columns = std::size_t(width / two_k);
    buckets_t buckets(num_vertices(g) / 4);
    vertex_iterator v, v_end;
    for (tie(v, v_end) = vertices(g); v != v_end; ++v) {
      std::size_t column = std::size_t((position[*v].x - width  / 2) / two_k);
      std::size_t row    = std::size_t((position[*v].y - height / 2) / two_k);
      buckets.at(row * columns + column).push_back(*v);
    }

    typedef typename buckets_t::iterator buckets_iterator;
    for (buckets_iterator i = buckets.begin(); i != buckets.end(); ++i) {
      typedef typename bucket_t::iterator bucket_iterator;
      for (bucket_iterator u = i->begin(); u != i->end(); ++u) {
	for (bucket_iterator v = i->begin(); v != i->end(); ++v) {
	  if (*u != *v) apply_force(*u, *v);
	}
      }
    }
  }

 private:
  Dim width;
  Dim height;
  PositionMap position;
  Dim two_k;
};

template<typename Dim, typename PositionMap>
inline grid_force_pairs<Dim, PositionMap>
make_grid_force_pairs(Dim width, Dim height, const PositionMap& position)
{ return grid_force_pairs<Dim, PositionMap>(width, height, position); }

template<typename Graph, typename PositionMap, typename Dim>
void
scale_graph(const Graph& g, PositionMap position, 
	    Dim left, Dim top, Dim right, Dim bottom)
{
  if (num_vertices(g) == 0) return;

  if (bottom > top) {
    using std::swap;
    swap(bottom, top);
  }

  typedef typename graph_traits<Graph>::vertex_iterator vertex_iterator;

  // Find min/max ranges
  Dim minX = position[*vertices(g).first].x, maxX = minX;
  Dim minY = position[*vertices(g).first].y, maxY = minY;
  vertex_iterator vi, vi_end;
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
    using std::min; // TBD: eric's tricks
    using std::max;
    minX = min(minX, position[*vi].x);
    maxX = max(maxX, position[*vi].x);
    minY = min(minY, position[*vi].y);
    maxY = max(maxY, position[*vi].y);
  }

  // Scale to bounding box provided
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
    position[*vi].x = ((position[*vi].x - minX) / (maxX - minX))
                    * (right - left) + left; 
    position[*vi].y = ((position[*vi].y - minY) / (maxY - minY))
                    * (top - bottom) + bottom;
  }
}

namespace detail {
  template<typename PositionMap, typename DisplacementMap, 
	   typename RepulsiveForce, typename Dim, typename Graph>
  struct fr_apply_force
  {
    typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;

    fr_apply_force(const PositionMap& position, 
		   const DisplacementMap& displacement,
		   RepulsiveForce repulsive_force, Dim k, const Graph& g)
      : position(position), displacement(displacement),
	repulsive_force(repulsive_force), k(k), g(g)
    { }

    void operator()(vertex_descriptor u, vertex_descriptor v) 
    {
      if (u != v) {
	using std::sqrt;

	Dim delta_x = position[v].x - position[u].x;
	Dim delta_y = position[v].y - position[u].y;
	Dim dist = sqrt(delta_x * delta_x + delta_y * delta_y);
	Dim fr = repulsive_force(u, v, k, dist, g);
	displacement[v].x += delta_x / dist * fr;
	displacement[v].y += delta_y / dist * fr;
      }
    }

  private:
    PositionMap position;
    DisplacementMap displacement;
    RepulsiveForce repulsive_force;
    Dim k;
    const Graph& g;
  };

} // end namespace detail

template<typename Graph, typename PositionMap, typename Dim,
	 typename AttractiveForce, typename RepulsiveForce,
         typename ForcePairs, typename Cooling, typename DisplacementMap>
void
fruchterman_reingold_force_directed_layout
 (const Graph&    g,
  PositionMap     position,
  Dim             width,
  Dim             height,
  AttractiveForce attractive_force,
  RepulsiveForce  repulsive_force,
  ForcePairs      force_pairs,
  Cooling         cool,
  DisplacementMap displacement)
{
  typedef typename graph_traits<Graph>::vertex_iterator   vertex_iterator;
  typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
  typedef typename graph_traits<Graph>::edge_iterator     edge_iterator;

  using std::sqrt;

  Dim area = width * height;
  // assume positions are initialized randomly
  Dim k = sqrt(area / num_vertices(g));

  detail::fr_apply_force<PositionMap, DisplacementMap, 
                         RepulsiveForce, Dim, Graph>
    apply_force(position, displacement, repulsive_force, k, g);
  
  while (Dim temp = cool()) {
    // Calculate repulsive forces
    vertex_iterator v, v_end;
    for (tie(v, v_end) = vertices(g); v != v_end; ++v) {
      displacement[*v].x = 0;
      displacement[*v].y = 0;
    }
    force_pairs(g, apply_force);

    // Calculate attractive forces
    edge_iterator e, e_end;
    for (tie(e, e_end) = edges(g); e != e_end; ++e) {
      vertex_descriptor v = source(*e, g);
      vertex_descriptor u = target(*e, g);
      Dim delta_x = position[v].x - position[u].x;
      Dim delta_y = position[v].y - position[u].y;
      Dim dist = sqrt(delta_x * delta_x + delta_y * delta_y);
      Dim fa = attractive_force(*e, k, dist, g);

      displacement[v].x -= delta_x / dist * fa;
      displacement[v].y -= delta_y / dist * fa;
      displacement[u].x += delta_x / dist * fa;
      displacement[u].y += delta_y / dist * fa;
    }

    // Update positions
    for (tie(v, v_end) = vertices(g); v != v_end; ++v) {
      using std::min; // TBD: use Eric's crazy hacks here
      using std::max; // TBD: use Eric's crazy hacks here
      Dim disp_size = sqrt(displacement[*v].x * displacement[*v].x 
                           + displacement[*v].y * displacement[*v].y);
      position[*v].x += displacement[*v].x / disp_size * min(disp_size, temp);
      position[*v].y += displacement[*v].y / disp_size * min(disp_size, temp);
      position[*v].x = min(width / 2, max(-width / 2, position[*v].x));
      position[*v].y = min(height / 2, max(-height / 2, position[*v].y));
    }
  }
}

namespace detail {
  template<typename DisplacementMap>
  struct fr_force_directed_layout
  {
    template<typename Graph, typename PositionMap, typename Dim,
	     typename AttractiveForce, typename RepulsiveForce,
	     typename ForcePairs, typename Cooling, 
	     typename Param, typename Tag, typename Rest>
    static void 
    run(const Graph&    g,
	PositionMap     position,
	Dim             width,
	Dim             height,
	AttractiveForce attractive_force,
	RepulsiveForce  repulsive_force,
	ForcePairs      force_pairs,
	Cooling         cool,
	DisplacementMap displacement,
	const bgl_named_params<Param, Tag, Rest>&)
    {
      fruchterman_reingold_force_directed_layout
	(g, position, width, height, attractive_force, repulsive_force,
	 force_pairs, cool, displacement);
    }
  };

  template<>
  struct fr_force_directed_layout<error_property_not_found>
  {
    template<typename Graph, typename PositionMap, typename Dim,
	     typename AttractiveForce, typename RepulsiveForce,
	     typename ForcePairs, typename Cooling, 
	     typename Param, typename Tag, typename Rest>
    static void 
    run(const Graph&    g,
	PositionMap     position,
	Dim             width,
	Dim             height,
	AttractiveForce attractive_force,
	RepulsiveForce  repulsive_force,
	ForcePairs      force_pairs,
	Cooling         cool,
	error_property_not_found,
	const bgl_named_params<Param, Tag, Rest>& params)
    {
      std::vector<simple_point<double> > displacements(num_vertices(g));
      fruchterman_reingold_force_directed_layout
	(g, position, width, height, attractive_force, repulsive_force,
	 force_pairs, cool, 
	 make_iterator_property_map
	 (displacements.begin(),
	  choose_const_pmap(get_param(params, vertex_index), g, 
			    vertex_index)));
    }
  };

} // end namespace detail

template<typename Graph, typename PositionMap, typename Dim, typename Param,
	 typename Tag, typename Rest>
void
fruchterman_reingold_force_directed_layout
  (const Graph&    g,
   PositionMap     position,
   Dim             width,
   Dim             height,
   const bgl_named_params<Param, Tag, Rest>& params)
{
  typedef typename property_value<bgl_named_params<Param,Tag,Rest>,
                                  vertex_displacement_t>::type D;

  detail::fr_force_directed_layout<D>::run
    (g, position, width, height,
     choose_param(get_param(params, attractive_force_t()), 
		  square_distance_attractive_force()),
     choose_param(get_param(params, repulsive_force_t()),
		  square_distance_repulsive_force()),
     choose_param(get_param(params, force_pairs_t()),
		  make_grid_force_pairs(width, height, position)),
     choose_param(get_param(params, cooling_t()),
		  linear_cooling<double>(100)),
     get_param(params, vertex_displacement_t()),
     params);
}

template<typename Graph, typename PositionMap, typename Dim, typename Param,
	 typename Tag, typename Rest>
void
fruchterman_reingold_force_directed_layout(const Graph&    g,
					   PositionMap     position,
					   Dim             width,
					   Dim             height)
{
  fruchterman_reingold_force_directed_layout
    (g, position, width, height,
     attractive_force(square_distance_attractive_force()));
}

} // end namespace boost

#endif // BOOST_GRAPH_FRUCHTERMAN_REINGOLD_FORCE_DIRECTED_LAYOUT_HPP
