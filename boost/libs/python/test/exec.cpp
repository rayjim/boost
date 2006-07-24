// Copyright Stefan Seefeld 2005.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/python.hpp>

#include <boost/detail/lightweight_test.hpp>
#include <iostream>


namespace python = boost::python;

// An abstract base class
class Base : public boost::noncopyable
{
public:
  virtual ~Base() {};
  virtual std::string hello() = 0;
};

// C++ derived class
class CppDerived : public Base
{
public:
  virtual ~CppDerived() {}
  virtual std::string hello() { return "Hello from C++!";}
};

// Familiar Boost.Python wrapper class for Base
struct BaseWrap : Base, python::wrapper<Base>
{
  virtual std::string hello() 
  {
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
    // workaround for VC++ 6.x or 7.0, see
    // http://boost.org/libs/python/doc/tutorial/doc/html/python/exposing.html#python.class_virtual_functions
    return python::call<std::string>(this->get_override("hello").ptr());
#else
    return this->get_override("hello")();
#endif
  }
};

// Pack the Base class wrapper into a module
BOOST_PYTHON_MODULE(embedded_hello)
{
  python::class_<BaseWrap, boost::noncopyable> base("Base");
}


void exec_test()
{
  // Register the module with the interpreter
  if (PyImport_AppendInittab("embedded_hello", initembedded_hello) == -1)
    throw std::runtime_error("Failed to add embedded_hello to the interpreter's "
                 "builtin modules");
  // Retrieve the main module
  python::object main = python::import("__main__");
  
  // Retrieve the main module's namespace
  python::object global(main.attr("__dict__"));

  // Define the derived class in Python.
  python::object result = python::exec(
    "from embedded_hello import *        \n"
    "class PythonDerived(Base):          \n"
    "    def hello(self):                \n"
    "        return 'Hello from Python!' \n",
    global, global);

  python::object PythonDerived = global["PythonDerived"];

  // Creating and using instances of the C++ class is as easy as always.
  CppDerived cpp;
  BOOST_TEST(cpp.hello() == "Hello from C++!");

  // But now creating and using instances of the Python class is almost
  // as easy!
  python::object py_base = PythonDerived();
  Base& py = python::extract<Base&>(py_base) BOOST_EXTRACT_WORKAROUND;

  // Make sure the right 'hello' method is called.
  BOOST_TEST(py.hello() == "Hello from Python!");
}

void exec_file_test(std::string const &script)
{
  // Run a python script in an empty environment.
  python::dict global;
  python::object result = python::exec_file(script.c_str(), global, global);

  // Extract an object the script stored in the global dictionary.
  BOOST_TEST(python::extract<int>(global["number"]) ==  42);
}

void exec_test_error()
{
  // Execute a statement that raises a python exception.
  python::dict global;
  python::object result = python::exec("print unknown \n", global, global);
}

int main(int argc, char **argv)
{
  BOOST_TEST(argc == 2);
  std::string script = argv[1];
  // Initialize the interpreter
  Py_Initialize();

  if (python::handle_exception(exec_test) ||
      python::handle_exception(boost::bind(exec_file_test, script)))
  {
    if (PyErr_Occurred())
    {
      BOOST_ERROR("Python Error detected");
      PyErr_Print();
    }
    else
    {
        BOOST_ERROR("A C++ exception was thrown  for which "
                    "there was no exception handler registered.");
    }
  }
  
  if (python::handle_exception(exec_test_error))
  {
    if (PyErr_Occurred())
    {
      PyErr_Print();
    }
    else
    {
        BOOST_ERROR("A C++ exception was thrown  for which "
                    "there was no exception handler registered.");
    }
  }
  else
  {
      BOOST_ERROR("Python exception expected, but not seen.");
  }
  
  // Boost.Python doesn't support Py_Finalize yet.
  // Py_Finalize();
  return boost::report_errors();
}

// Including this file makes sure
// that on Windows, any crashes (e.g. null pointer dereferences) invoke
// the debugger immediately, rather than being translated into structured
// exceptions that can interfere with debugging.
#include "module_tail.cpp"
