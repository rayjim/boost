// Copyright David Abrahams 2001. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef CLASS_DWA20011214_HPP
# define CLASS_DWA20011214_HPP

# include <boost/python/detail/wrap_python.hpp>
# include <boost/python/detail/config.hpp>
# include <boost/utility.hpp>
# include <boost/python/instance_holder.hpp>
# include <boost/python/object_core.hpp>
# include <boost/python/type_id.hpp>
# include <cstddef>

namespace boost { namespace python {

namespace objects { 

struct BOOST_PYTHON_DECL class_base : python::api::object
{
    // constructor
    class_base(
        char const* name              // The name of the class
        
        , std::size_t num_types         // A list of class_ids. The first is the type
        , type_info const*const types    // this is wrapping. The rest are the types of
                                        // any bases.
        
        , char const* doc = 0           // Docstring, if any.
        );

    // Retrieve the underlying object
    void add_property(char const* name, object const& fget);
    void add_property(char const* name, object const& fget, object const& fset);
    void setattr(char const* name, object const&);
    void enable_pickling(bool getstate_manages_dict);

    // Set a special attribute in the class which tells Boost.Python
    // to allocate extra bytes for embedded C++ objects in Python
    // instances.
    void set_instance_size(std::size_t bytes);

    // Set an __init__ function which throws an appropriate exception
    // for abstract classes.
    void def_no_init();
};

}}} // namespace boost::python::objects

#endif // CLASS_DWA20011214_HPP
