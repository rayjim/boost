// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef OBJECT_SLICES_DWA2002615_HPP
# define OBJECT_SLICES_DWA2002615_HPP

# include <boost/python/detail/prefix.hpp>

# include <boost/python/proxy.hpp>
# include <boost/python/object_core.hpp>
# include <boost/python/object_protocol.hpp>
# include <boost/python/handle.hpp>
# include <utility>

namespace boost { namespace python { namespace api {

struct const_slice_policies
{
    typedef std::pair<handle<>, handle<> > key_type;
    static object get(object const& target, key_type const& key);
};
  
struct slice_policies : const_slice_policies
{
    static object const& set(object const& target, key_type const& key, object const& value);
    static void del(object const& target, key_type const& key);
};

//
// implementation
//
template <class U>
object_slice
object_operators<U>::slice(object_cref start, object_cref finish)
{
    object_cref2 x = *static_cast<U*>(this);
    return object_slice(x, std::make_pair(borrowed(start.ptr()), borrowed(finish.ptr())));
}

template <class U>
const_object_slice
object_operators<U>::slice(object_cref start, object_cref finish) const
{
    object_cref2 x = *static_cast<U const*>(this);
    return const_object_slice(x, std::make_pair(borrowed(start.ptr()), borrowed(finish.ptr())));
}

template <class U>
object_slice
object_operators<U>::slice(slice_nil, object_cref finish)
{
    object_cref2 x = *static_cast<U*>(this);
    return object_slice(x, std::make_pair(allow_null((PyObject*)0), borrowed(finish.ptr())));
}

template <class U>
const_object_slice
object_operators<U>::slice(slice_nil, object_cref finish) const
{
    object_cref2 x = *static_cast<U const*>(this);
    return const_object_slice(x, std::make_pair(allow_null((PyObject*)0), borrowed(finish.ptr())));
}

template <class U>
object_slice
object_operators<U>::slice(slice_nil, slice_nil)
{
    object_cref2 x = *static_cast<U*>(this);
    return object_slice(x, std::make_pair(allow_null((PyObject*)0), allow_null((PyObject*)0)));
}

template <class U>
const_object_slice
object_operators<U>::slice(slice_nil, slice_nil) const
{
    object_cref2 x = *static_cast<U const*>(this);
    return const_object_slice(x, std::make_pair(allow_null((PyObject*)0), allow_null((PyObject*)0)));
}

template <class U>
object_slice
object_operators<U>::slice(object_cref start, slice_nil)
{
    object_cref2 x = *static_cast<U*>(this);
    return object_slice(x, std::make_pair(borrowed(start.ptr()), allow_null((PyObject*)0)));
}

template <class U>
const_object_slice
object_operators<U>::slice(object_cref start, slice_nil) const
{
    object_cref2 x = *static_cast<U const*>(this);
    return const_object_slice(x, std::make_pair(borrowed(start.ptr()), allow_null((PyObject*)0)));
}
# if !defined(BOOST_MSVC) || BOOST_MSVC > 1300
template <class U>
template <class T, class V>
inline const_object_slice
object_operators<U>::slice(T const& start, V const& end) const
{
    return this->slice(
        typename slice_bound<T>::type(start)
        , typename slice_bound<V>::type(end));
}

template <class U>
template <class T, class V>
inline object_slice
object_operators<U>::slice(T const& start, V const& end)
{
    return this->slice(
        typename slice_bound<T>::type(start)
        , typename slice_bound<V>::type(end));
}
# endif 


inline object const_slice_policies::get(object const& target, key_type const& key)
{
    return getslice(target, key.first, key.second);
}

inline object const& slice_policies::set(
    object const& target
    , key_type const& key
    , object const& value)
{
    setslice(target, key.first, key.second, value);
    return value;
}

inline void slice_policies::del(
    object const& target
    , key_type const& key)
{
    delslice(target, key.first, key.second);
}

}}} // namespace boost::python::api

#endif // OBJECT_SLICES_DWA2002615_HPP
