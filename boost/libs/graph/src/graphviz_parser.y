%{
//=======================================================================
// Copyright 2001 University of Notre Dame.
// Author: Lie-Quan Lee
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

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include <boost/config.hpp>
#include <boost/graph/graphviz.hpp>

#if defined BOOST_NO_STRINGSTREAM 
#include <strstream>
#else
#include <sstream>
#endif

#define YYPARSE_PARAM g

  extern void yyerror(char* str);
  extern int yylex();
  extern void* yyin;

  enum AttrState {GRAPH_GRAPH_A, GRAPH_NODE_A, GRAPH_EDGE_A, NODE_A, EDGE_A};

  namespace graphviz {

    typedef boost::graph_traits<GraphvizGraph>::vertex_descriptor Vertex;
    typedef boost::graph_traits<GraphvizGraph>::edge_descriptor   Edge;
    typedef GraphvizGraph Subgraph;

    Vertex current_vertex;
    Edge   current_edge;
    Subgraph* current_graph = NULL;

    std::vector< std::pair<void*, bool>* > vlist;//store a list of rhs 

    std::map<std::string,std::string> attributes;//store attributes temporarily
    AttrState attribute_state;

    std::map<std::string, Subgraph*> subgraphs;  //store the names of subgraphs
    std::map<std::string, Vertex> nodes;         //store the names of nodes

    typedef std::map<std::string, Subgraph*>::iterator It; 
    typedef std::map<std::string, Vertex>::iterator Iter; 

    const std::string& print_name(const Subgraph& g) {
      const boost::graph_property<Subgraph, boost::graph_name_t>::type&
	name = boost::get_property(*current_graph, boost::graph_name);
      return name; 
    }

    std::pair<Iter, bool> lookup(const std::string& name) {
      //lookup in the top level
      Iter it = nodes.find(name);
      bool found = (it != nodes.end() );
      return std::make_pair(it, found);
    }
    
    Vertex add_name(const std::string& name, GraphvizGraph& g) {
      Vertex v = boost::add_vertex(*current_graph);
      v = current_graph->local_to_global(v);

      //set the label of vertex, it could be overwritten later.
      boost::property_map<GraphvizGraph, boost::vertex_attribute_t>::type
	va = boost::get(boost::vertex_attribute, g); 
      va[v]["label"] = name; 
      
      //add v into the map so next time we will find it.
      nodes[name] = v; 
      return v;
    }

    std::pair<It, bool> lookup_subgraph(const std::string& name) {
      It it = subgraphs.find(name);
      bool found = (it != subgraphs.end() );
      return std::make_pair(it, found);
    }
    
    Subgraph* create_subgraph(const std::string& name) { 

      Subgraph* new_subgraph = &(current_graph->create_subgraph()); 

      subgraphs[name]        = new_subgraph;
      return new_subgraph;
    }

    
    void set_attribute(AttrType& p, const AttrType& attr) {
      AttrType::const_iterator i, end;
      for ( i=attr.begin(), end=attr.end(); i!=end; ++i)
	p[i->first]=i->second;
    }
  
    void set_attribute(Subgraph& g,
		       AttrState s, bool clear_attribute = true) {
      typedef Subgraph Graph;
      switch ( s ) {
      case GRAPH_GRAPH_A: 
	{
	  boost::graph_property<Graph, boost::graph_graph_attribute_t>::type&
	    gga = boost::get_property(g, boost::graph_graph_attribute);
	  set_attribute(gga, attributes); 
	}
	break;
      case GRAPH_NODE_A: 
	{
	  boost::graph_property<Graph, boost::graph_node_attribute_t>::type&
	    gna = boost::get_property(g, boost::graph_node_attribute);
	  set_attribute(gna, attributes); 
	}
	break;
      case GRAPH_EDGE_A: 
	{
	  boost::graph_property<Graph, boost::graph_edge_attribute_t>::type&
	    gea = boost::get_property(g, boost::graph_edge_attribute);
	  set_attribute(gea, attributes); 
	}
	break;
      case NODE_A:
	{
	  boost::property_map<Graph, boost::vertex_attribute_t>::type
	    va = boost::get(boost::vertex_attribute, g);    //va[v]
	  set_attribute(va[current_vertex], attributes);
	}
	break;
      case EDGE_A: 
	{
	  boost::property_map<Graph, boost::edge_attribute_t>::type
	    ea = boost::get(boost::edge_attribute, g);      //ea[e]
	  set_attribute(ea[current_edge], attributes); 
	}
	break;
      }
      if ( clear_attribute )
	attributes.clear();
    }


    void add_edges(const Vertex& u, const Vertex& v, GraphvizGraph& g) {
      graphviz::current_edge = boost::add_edge(u, v, g).first; 
      graphviz::set_attribute(g, EDGE_A, false);
    }
    
    void add_edges(Subgraph* G1, Subgraph* G2, GraphvizGraph& g) {
      boost::graph_traits<Subgraph>::vertex_iterator i, j, m, n;
      for ( boost::tie(i, j) = boost::vertices(*G1); i != j; ++i) {
	for ( boost::tie(m, n) = boost::vertices(*G2); m != n; ++m) {
	  graphviz::add_edges(G1->local_to_global(*i),
			      G2->local_to_global(*m), g);
	}
      }
    }

    void add_edges(Subgraph* G, const Vertex& v, GraphvizGraph& g) {
      boost::graph_traits<Subgraph>::vertex_iterator i, j;
      for ( boost::tie(i, j) = boost::vertices(*G); i != j; ++i) {
	graphviz::add_edges(G->local_to_global(*i), v, g);
      }
    }

    void add_edges(const Vertex& u, Subgraph* G, GraphvizGraph& g) {
      boost::graph_traits<Subgraph>::vertex_iterator i, j;
      for ( boost::tie(i, j) = boost::vertices(*G); i != j; ++i) {
	graphviz::add_edges(u, G->local_to_global(*i), g);
      }
    }

    std::string random_string() {
      static int i=0;
#if defined BOOST_NO_STRINGSTREAM
      std::strstream out;
#else
      std::stringstream out;
#endif
      out << "default" << i;
      ++i;
      return out.str();
    }


    void set_graph_name(const std::string& name) {
      boost::graph_property<Subgraph, boost::graph_name_t>::type&
	gea = boost::get_property(*current_graph, boost::graph_name);
      gea = name;
    }

  } //namespace detail {

%}

%union {
  int         i;
  void*       ptr;
};

%token<i>   GRAPH_T NODE_T EDGE_T DIGRAPH_T EDGEOP_T SUBGRAPH_T
%token<ptr> ID_T

%type<i>    graph graph_header graph_type stmt_list stmt attr_stmt attr_header
%type<i>    attr_list attr compound_stmt node_stmt edge_stmt opt_attr
%type<i>    graph_body edge_rhs_one edge_rhs graph_attr opt_graph_body
%type<ptr>  graph_name
%type<ptr>  edge_endpoint node_id  node_port subgraph subgraph_header


%%
graph:        graph_header graph_body
              ;

graph_body:  '{' stmt_list '}' {$$=0;}
              ;

graph_header: graph_type graph_name
  {
    std::string* name = static_cast<std::string*>($2);
    graphviz::current_graph = static_cast<graphviz::Subgraph*>(g);
    graphviz::set_graph_name(*name);
    delete name;
  }
              ;

graph_type:   GRAPH_T | DIGRAPH_T
              ;

graph_name:   ID_T {$$ = $1; } | {$$=(void*)(new std::string("G")); }
              ;

stmt_list:    stmt_list stmt | stmt
              ;

semicolon:    ';' | 
              ;

stmt:         attr_stmt semicolon | 
              compound_stmt  semicolon
              ;

attr_stmt:    attr_header '[' attr_list ']' 
  { 
    graphviz::set_attribute(*graphviz::current_graph,
			  graphviz::attribute_state); 
  }
              ;

attr_header:  GRAPH_T  { graphviz::attribute_state = GRAPH_GRAPH_A; } 
              | NODE_T { graphviz::attribute_state = GRAPH_NODE_A; }
              | EDGE_T { graphviz::attribute_state = GRAPH_EDGE_A; }
              ;

attr_list:    attr | attr_list attr_separator attr
              ;

attr:         ID_T '=' ID_T 
  { 
    std::string* name  = static_cast<std::string*>($1);
    std::string* value = static_cast<std::string*>($3);
    graphviz::attributes[*name] = *value; 
    delete name;
    delete value;
  }
              ;

attr_separator:    ';' | ',' |
              ;

compound_stmt: node_stmt | edge_stmt  | graph_attr | subgraph { $$ = 0; }
              ;

graph_attr    : attr 
  { 
    graphviz::set_attribute(
         *static_cast<graphviz::Subgraph*>(graphviz::current_graph),
			    GRAPH_GRAPH_A);
  }
              ;

node_stmt:    node_id opt_attr
  { 
    graphviz::Vertex* temp   = static_cast<graphviz::Vertex*>($1); 
    graphviz::current_vertex = *temp;
    graphviz::set_attribute(*static_cast<GraphvizGraph*>(YYPARSE_PARAM),
			    NODE_A); 
    delete temp;
    $$ = 0;
  }
              ;

opt_attr:     '[' attr_list ']' { $$=0; } | { $$=0; }
              ;

node_id:      ID_T 
  {
    std::string* name  = static_cast<std::string*>($1);
    std::pair<graphviz::Iter, bool> result = graphviz::lookup(*name); 
    if (result.second) 
      graphviz::current_vertex = result.first->second; 
    else
      graphviz::current_vertex = graphviz::add_name(*name, *static_cast<GraphvizGraph*>(YYPARSE_PARAM)) ; 
    graphviz::Vertex* temp = new graphviz::Vertex(graphviz::current_vertex);
    $$ = (void *)temp;
    graphviz::attribute_state = NODE_A;  
    delete name;
  }
              | node_port  { $$=$1; }
              ;

node_port:    ID_T ':' ID_T
  {
    //consider port as a special properties ?? --need work here
    std::string* name = static_cast<std::string*>($1);
    std::string* port = static_cast<std::string*>($3);

    std::pair<graphviz::Iter, bool> result = graphviz::lookup(*name); 
    if (result.second) 
      graphviz::current_vertex = result.first->second; 
    else
      graphviz::current_vertex = graphviz::add_name(*name, *static_cast<GraphvizGraph*>(YYPARSE_PARAM)) ; 
    graphviz::Vertex* temp = new graphviz::Vertex(graphviz::current_vertex);
    $$ = (void *)temp;
    graphviz::attribute_state = NODE_A;  
    delete name;
    delete port;
  }
              ;

edge_stmt:    edge_endpoint edge_rhs opt_attr 
  {

    typedef std::pair<void*, bool>* Ptr;
    Ptr source = static_cast<Ptr>($1);

    for (std::vector<Ptr>::iterator it=graphviz::vlist.begin();
	 it !=graphviz::vlist.end(); ++it) { 
      if ( source->second ) {
	if ( (*it)->second )
	  graphviz::add_edges(static_cast<graphviz::Subgraph*>(source->first),
			    static_cast<graphviz::Subgraph*>((*it)->first),
			    *static_cast<GraphvizGraph*>(YYPARSE_PARAM));
	else
	  graphviz::add_edges(static_cast<graphviz::Subgraph*>(source->first),
			    *static_cast<graphviz::Vertex*>((*it)->first),
			    *static_cast<GraphvizGraph*>(YYPARSE_PARAM));
      } else {
	graphviz::Vertex* temp = static_cast<graphviz::Vertex*>(source->first);
	if ( (*it)->second )
	  graphviz::add_edges(*temp,
			    static_cast<graphviz::Subgraph*>((*it)->first),
			    *static_cast<GraphvizGraph*>(YYPARSE_PARAM));
	else
	  graphviz::add_edges(*temp,
			    *static_cast<graphviz::Vertex*>((*it)->first),
			    *static_cast<GraphvizGraph*>(YYPARSE_PARAM));
	delete temp;
      }

      delete source; 
      source = *it; 
    } 
    
    if ( ! source->second ) {
      graphviz::Vertex* temp = static_cast<graphviz::Vertex*>(source->first);
      delete temp;
    }
    delete source;

    graphviz::attributes.clear();
    graphviz::vlist.clear(); 
  }
              ;

edge_rhs_one: EDGEOP_T edge_endpoint   
  { graphviz::vlist.push_back(static_cast<std::pair<void*, bool>*>($2)); }
; 

edge_rhs:     edge_rhs_one | edge_rhs edge_rhs_one
              ;

edge_endpoint:   node_id 
  { 
    std::pair<void*, bool>* temp = new std::pair<void*, bool>;
    temp->first = $1;
    temp->second = false;
    $$ = (void*)temp;

    graphviz::attribute_state = EDGE_A; 
  }
                 | subgraph 
  { 
    std::pair<void*, bool>* temp = new std::pair<void*, bool>;
    temp->first = $1;
    temp->second = true;
    $$ = (void*)temp;

    graphviz::attribute_state = EDGE_A; 
  }
              ;

subgraph:    subgraph_header opt_graph_body 
  {
#ifndef DEBUG_SUBGRAPH
    graphviz::current_graph = &graphviz::current_graph->parent(); 
#endif
  }
             |
  {
    std::string name = graphviz::random_string();
    graphviz::Subgraph* temp = graphviz::create_subgraph(name);
    graphviz::current_graph = temp;
    graphviz::set_graph_name(name);

    $$ = (void *) graphviz::current_graph;
  } graph_body
  {
#ifndef DEBUG_SUBGRAPH
    graphviz::current_graph = &graphviz::current_graph->parent(); 
#endif
  }
              ;

subgraph_header: SUBGRAPH_T ID_T
  {
    //lookup ID_T if it is already in the subgraph,
    //if it is not, add a new subgraph
    std::string* name  = static_cast<std::string*>($2);

    std::pair<graphviz::It, bool> temp = graphviz::lookup_subgraph(*name);

    if ( temp.second )  //found 
      graphviz::current_graph = (temp.first)->second;
    else {
      graphviz::current_graph = graphviz::create_subgraph(*name);
      graphviz::set_graph_name(*name);
    }

    $$ = (void *) graphviz::current_graph;
    delete name;
  }
              ;

opt_graph_body: graph_body |  { $$ = 0; }
              ;
%%

namespace boost {
  
  void read_graphviz(GraphvizGraph& g, const std::string& filename) {
    FILE* file = fopen(filename.c_str(), "r");
    yyin = static_cast<void*>(file);

    yyparse(static_cast<void*>(&g));
  }
  

  template <class Indexer, class Key>
  int get_index(Indexer indexer, const Key& u) {
    return indexer[u];
  }
  
  template <class Graph, class RandomAccessIterator>
    void write_graphviz_subgraph (std::ostream& out, const Graph& g, 
				  const GraphvizGraph& root,
				  RandomAccessIterator vertex_marker,
				  RandomAccessIterator edge_marker) {
    typedef typename boost::graph_traits<Graph>::directed_category CAT;
    typedef graphviz_io_traits<CAT> Traits;

    graph_property<Graph, graph_name_t>::type&
      g_name = const_cast<graph_property<Graph, graph_name_t>::type&>( get_property(g, graph_name));

    if ( &g == &root )
      out << Traits::name() ;
    else
      out << "subgraph";

    out << " " << g_name << " {" << std::endl;

    typename Graph::const_children_iterator i_child, j_child;
    
    //print graph/node/edge attributes
    make_graph_attributes_writer(g)(out);

    //print subgraph
    for ( boost::tie(i_child,j_child) = g.children();
	  i_child != j_child; ++i_child )
      write_graphviz_subgraph(out, *i_child, root, vertex_marker, edge_marker);

    //print the lefted 
    typename boost::graph_traits<Graph>::vertex_iterator i, end;
    typename boost::graph_traits<Graph>::out_edge_iterator ei, edge_end;

    for(boost::tie(i,end) = boost::vertices(g); i != end; ++i) {
      boost::tie(ei,edge_end) = out_edges(*i, g);

      graphviz::Vertex source = g.local_to_global(*i);

      while ( ei != edge_end ) {
	int pos = get_index(get(edge_index, root), g.local_to_global(*ei));
	if ( edge_marker[pos] ) {
	  edge_marker[pos] = false;
	  out << source << " " << Traits::delimiter()
	      << " " << g.local_to_global(target(*ei, g));
	  make_edge_attributes_writer(g)(out, *ei); //print edge properties
	  out << ";" << std::endl;
	}
	++ei;
      }
      
      int pos = get_index(get(vertex_index, root), source);
      if ( vertex_marker[pos] ) {
	vertex_marker[pos] = false;
	out << source << ";" << std::endl;
	make_vertex_attributes_writer(root)(out, source); //print vertex properties 
      }
    }
    out << "}" << std::endl;
    
  }

  void write_graphviz(const GraphvizGraph& g, 
		      const std::string& filename) {
    std::ofstream out(filename.c_str());
    typedef GraphvizGraph Graph;

    std::vector<bool> edge_marker(num_edges(g), true);
    std::vector<bool> vertex_marker(num_vertices(g), true);

    write_graphviz_subgraph(out, g, g,
			    vertex_marker.begin(),
			    edge_marker.begin());
  }
  
}

