// (C) Copyright 2006 Douglas Gregor <doug.gregor -at- gmail.com>

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor

/** @file module.cpp
 *
 *  This file provides the top-level module for the Boost.MPI Python
 *  bindings.
 */
#include <boost/python.hpp>
#include <boost/parallel/mpi.hpp>

using namespace boost::python;
using namespace boost::parallel::mpi;

namespace boost { namespace parallel { namespace mpi { namespace python {

extern void export_environment();
extern void export_exception();
extern void export_collectives();
extern void export_communicator();
extern void export_datatypes();
extern void export_request();
extern void export_status();
extern void export_timer();

extern const char* module_docstring;

BOOST_PYTHON_MODULE(mpi)
{
  // Setup module documentation
  scope().attr("__doc__") = module_docstring;
  scope().attr("__author__") = "Douglas Gregor <doug.gregor@gmail.com>";
  scope().attr("__date__") = "$LastChangedDate: 2006-07-16 15:25:47 -0400 (Sun, 16 Jul 2006) $";
  scope().attr("__version__") = "$Revision$";
  scope().attr("__copyright__") = "Copyright (C) 2006 Douglas Gregor";
  scope().attr("__license__") = "http://www.boost.org/LICENSE_1_0.txt";

  export_environment();
  export_exception();
  export_communicator();
  export_collectives();
  export_datatypes();
  export_request();
  export_status();
  export_timer();
}

} } } } // end namespace boost::parallel::mpi::python
