// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#include <boost/python/module.hpp>
#include <boost/python/class.hpp>
#include <boost/python/list.hpp>
#include <boost/python/make_function.hpp>
#include <boost/lexical_cast.hpp>
#include "test_class.hpp"

using namespace boost::python;

object new_list()
{
    return list();
}

list listify(object x)
{
    return list(x);
}

object listify_string(char const* s)
{
    return list(s);
}

std::string x_rep(test_class<> const& x)
{
    return "X("  + boost::lexical_cast<std::string>(x.value()) + ")";
}

object apply_object_list(object f, list x)
{
    return f(x);
}

list apply_list_list(object f, list x)
{
    return call<list>(f.ptr(), x);
}

void append_object(list& x, object y)
{
    x.append(y);
}

void append_list(list& x, list const& y)
{
    x.append(y);
}

typedef test_class<> X;

int notcmp(object const& x, object const& y)
{
    return y < x ? -1 : y > x ? 1 : 0;
}

void exercise(list x, object y, object print)
{
    x.append(y);
    x.append(5);
    x.append(X(3));
    
    print("after append:");
    print(x);
    
    print("number of", y, "instances:", x.count(y));
    
    print("number of 5s:", x.count(5));

    x.extend("xyz");
    print("after extend:");
    print(x);
    print("index of", y, "is:", x.index(y));
    print("index of 'l' is:", x.index("l"));
    
    x.insert(4, 666);
    print("after inserting 666:");
    print(x);
    print("inserting with object as index:");
    x.insert(x[x.index(5)], "---");
    print(x);
    
    print("popping...");
    x.pop();
    print(x);
    x.pop(x[x.index(5)]);
    print(x);
    x.pop(x.index(5));
    print(x);

    print("removing", y);
    x.remove(y);
    print(x);
    print("removing", 666);
    x.remove(666);
    print(x);

    print("reversing...");
    x.reverse();
    print(x);

    print("sorted:");
    x.pop(2); // make sorting predictable
    x.sort();
    print(x);

    print("reverse sorted:");
    x.sort(&notcmp);
    print(x);

    list w;
    w.append(5);
    w.append(6);
    w += "hi";
    assert(w[0] == 5);
    assert(w[1] == 6);
    assert(w[2] == 'h');
    assert(w[3] == 'i');
}

BOOST_PYTHON_MODULE_INIT(list_ext)
{
    module("list_ext")
        .def("new_list", new_list)
        .def("listify", listify)
        .def("listify_string", listify_string)
        .def("apply_object_list", apply_object_list)
        .def("apply_list_list", apply_list_list)
        
        .def("append_object", append_object)
        .def("append_list", append_list)

        .def("exercise", exercise)
        
        .add(class_<X>("X", args<int>())
             .def( "__repr__", x_rep))
        ;
}

