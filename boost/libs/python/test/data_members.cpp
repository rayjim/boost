// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#include <boost/python/class.hpp>
#include <boost/python/module.hpp>
#include <boost/python/return_value_policy.hpp>
#include <boost/python/return_by_value.hpp>
#include "test_class.hpp"

#if defined(_AIX) && defined(__EDG_VERSION__) && __EDG_VERSION__ < 245
# include <iostream> // works around a KCC intermediate code generation bug
#endif


using namespace boost::python;

typedef test_class<> X;

struct Y : test_class<1>
{
    Y(int v) : test_class<1>(v) {}
    Y& operator=(Y const& rhs) { x = rhs.x; return *this; }
};

double get_fair_value(X const& x) { return x.value(); }


struct VarBase
{
    VarBase(std::string name_) : name(name_) {}
    
    std::string const name;
    std::string get_name1() const { return name; }
    
};

struct Var : VarBase
{
    Var(std::string name_) : VarBase(name_), value(), name2(name.c_str()), y(6) {}
    std::string const& get_name2() const { return name; }
    float value;
    char const* name2;
    Y y;

    static int static1;
    static Y static2;
};

int Var::static1 = 0;
Y Var::static2(0);

BOOST_PYTHON_MODULE(data_members_ext)
{
    class_<X>("X", init<int>())
        .def("value", &X::value)
        .def("set", &X::set)
        .def_readonly("x", &X::x)
        .add_property("fair_value", get_fair_value)
        ;

    class_<Y>("Y", init<int>())
        .def("value", &Y::value)
        .def("set", &Y::set)
        .def_readwrite("x", &Y::x)
        ;

    class_<Var>("Var", init<std::string>())
        .def_readonly("name", &Var::name)
        .def_readonly("name2",
#if __MWERKS__ <= 0x2407 // Old MWerks mis-deduces the type here as `char* Var::*'
                      (char const* Var::*)
#endif 
                      &Var::name2
            )
        .def_readwrite("value", &Var::value)
        .def_readonly("y", &Var::y)
        
        // Test return_by_value for plain values and for
        // pointers... return_by_value was implemented as a
        // side-effect of implementing data member support, so it made
        // sense to add the test here.
        .def("get_name1", &Var::get_name1, return_value_policy<return_by_value>())
        .def("get_name2", &Var::get_name2, return_value_policy<return_by_value>())
        
        .add_property("name3", &Var::get_name1)

        // Test static data members
        .def_readonly("ro1a", &Var::static1)
        .def_readonly("ro1b", Var::static1)
        .def_readwrite("rw1a", &Var::static1)
        .def_readwrite("rw1b", Var::static1)

        .def_readonly("ro2a", &Var::static2)
        .def_readonly("ro2b", Var::static2)
        .def_readwrite("rw2a", &Var::static2)
        .def_readwrite("rw2b", Var::static2)
        ;
}

#include "module_tail.cpp"
