// Copyright 2002 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software 
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Boost.MultiArray Library
//  Authors: Ronald Garcia
//           Jeremy Siek
//           Andrew Lumsdaine
//  See http://www.boost.org/libs/multi_array for documentation.

//
// resize.cpp - Test of resizing multi_arrays
//

#include "boost/test/minimal.hpp"
#include "boost/multi_array.hpp"
#include <iostream>
using namespace std;


int test_main(int,char*[]) {

  typedef boost::multi_array<int,3> marray;


  int A_data[] = {
    0,1,2,3,
    4,5,6,7,
    8,9,10,11,

    12,13,14,15,
    16,17,18,19,
    20,21,22,23
  };


  marray A(boost::extents[2][3][4]);

  A.assign(A_data,A_data+(2*3*4));

  A.resize(boost::extents[4][3][2]);
  
  int A_resize[] = {
    0,1,
    4,5,
    8,9,

    12,13,
    16,17,
    20,21,

    0,0,
    0,0,
    0,0,

    0,0,
    0,0,
    0,0
  };

  BOOST_CHECK(std::equal(A_resize,A_resize+(4*3*2),A.data()));


  {
    marray defaultA;
    defaultA.resize(boost::extents[2][3][4]);
    BOOST_CHECK(std::accumulate(defaultA.data(),
                               defaultA.data()+(2*3*4),0) == 0);
  }



  // verify the preservation of storage order
  {
    int tiling_graph_storage_order[] = {2, 0, 1}; 
    bool tiling_graph_index_order[] = {true, true, true};
  
    marray A(boost::extents[3][4][2],
             boost::general_storage_order<3>(tiling_graph_storage_order,
                                             tiling_graph_index_order)); 


    int value = 0;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 4; j++) {
        for (int k = 0; k < 2; k++) {
          *(A.data() + value) = value;
          ++value;
        }
      }
    }

    // "Resize" to the same size
    A.resize(boost::extents[3][4][2]);

    int check = 0;
    for (int i = 0; i < 3; i++) { 
      for (int j = 0; j < 4; j++) {
        for (int k = 0; k < 2; k++) {
          BOOST_CHECK(*(A.data() + check) == check);
          ++check;
        }
      }
    }
  }


  return boost::exit_success;
}
