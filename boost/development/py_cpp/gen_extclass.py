from gen_function import *
import string

def gen_extclass(args):
    held_instance = """%{
    template <%(class A%n%:, %)>%}
    HeldInstance(PyObject* p%(, const A%n%& a%n%)) : T(%(a%n%:, %)), m_self(p) {}"""

    instance_value_holder = """%{
    template <%(class A%n%:, %)>%}
    InstanceValueHolder(ExtensionInstance* p%(, const A%n& a%n%)) :
        m_held(p%(, a%n%)) {}"""
    
    return (
"""//  (C) Copyright David Abrahams 2000. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  The author gratefully acknowleges the support of Dragon Systems, Inc., in
//  producing this work.
//
//  This file automatically generated for %d-argument constructors by
//  gen_extclass.py

#ifndef EXTENSION_CLASS_DWA052000_H_
# define EXTENSION_CLASS_DWA052000_H_

# include "pyconfig.h"
# include "subclass.h"
# include <vector>
# include "none.h"
# include "objects.h"
# include "functions.h"
# include <memory>
# include "init_function.h"
# include <typeinfo>
# include <boost/smart_ptr.hpp>

namespace py {

// forward declarations
class ExtensionInstance;
template <class T> class InstanceHolder;
template <class T, class U> class InstanceValueHolder;
template <class Ptr, class T> class InstancePtrHolder;

MetaClass<ExtensionInstance>* extension_meta_class();
ExtensionInstance* get_extension_instance(PyObject* p);
void report_missing_instance_data(ExtensionInstance*, Class<ExtensionInstance>*, const std::type_info&);
void report_missing_ptr_data(ExtensionInstance*, Class<ExtensionInstance>*, const std::type_info&);
void report_missing_class_object(const std::type_info&);
void report_released_smart_pointer(const std::type_info&);
    
template <class T, class U>
struct ExtensionClassFromPython
{
};

template <class T>
T* check_non_null(T* p)
{
    if (p == 0)
        report_released_smart_pointer(typeid(T));
    return p;
}

template <class T> class HeldInstance;

class ExtensionClassBase : public Class<ExtensionInstance>
{
 public:
    ExtensionClassBase(const char* name);
 protected:
    void add_method(PyPtr<Function> method, const char* name);
    void add_default_method(PyPtr<Function> method, const char* name);
    void add_method(Function* method, const char* name);
    void add_default_method(Function* method, const char* name);
    
    void add_constructor_object(Function*);
    void add_setter_method(Function*, const char* name);
    void add_getter_method(Function*, const char* name);
};

template <class T>
class ClassRegistry
{
 public:
    static Class<ExtensionInstance>* class_object()
        { return static_class_object; }
    static void register_class(py::Class<py::ExtensionInstance>*);
    static void unregister_class(py::Class<py::ExtensionInstance>*);
 private:
    static py::Class<py::ExtensionInstance>* static_class_object;
};

#ifdef PY_NO_INLINE_FRIENDS_IN_NAMESPACE // back to global namespace for this GCC bug
}
#endif

// This class' only job is to define from_python and to_python converters for T
// and U. T is the class the user really intends to wrap. U is a class derived
// from T with some virtual function overriding boilerplate, or if there are no
// virtual functions, U = HeldInstance<T>.
template <class T, class U = py::HeldInstance<T> >
class PyExtensionClassConverters
{
 public:
#ifdef BOOST_MSVC
    // Convert return values of type T to python objects.  What happens if T is
    // not copyable? Apparently there is no problem with g++ or MSVC unless this
    // is actually used. With a conforming compiler we will have a problem.
    friend PyObject* to_python(const T& x)
        {
            py::PyPtr<py::ExtensionInstance> result(create_instance(false));
            result->add_implementation(
                std::auto_ptr<py::InstanceHolderBase>(
                    new py::InstanceValueHolder<T,U>(result.get(), x)));
            return result.release();
        }
#else
    friend py::Type<U> py_holder_type(const T&)
        { return py::Type<U>(); }
#endif
    
    PyExtensionClassConverters() {}
    
    // Convert to T*
    friend T* from_python(PyObject* obj, py::Type<T*>)
    {
        // Downcast to an ExtensionInstance, then find the actual T
        py::ExtensionInstance* self = py::get_extension_instance(obj);
        typedef std::vector<py::InstanceHolderBase*>::const_iterator Iterator;
        for (Iterator p = self->wrapped_objects().begin();
             p != self->wrapped_objects().end(); ++p)
        {
            py::InstanceHolder<T>* held = dynamic_cast<py::InstanceHolder<T>*>(*p);
            if (held != 0)
                return held->target();
        }
        py::report_missing_instance_data(self, py::ClassRegistry<T>::class_object(), typeid(T));
        throw py::ArgumentError();
    }

    // Convert to PtrType, where PtrType can be dereferenced to obtain a T.
    template <class PtrType>
    static PtrType& ptr_from_python(PyObject* obj, py::Type<PtrType>)
    {
        // Downcast to an ExtensionInstance, then find the actual T
        py::ExtensionInstance* self = py::get_extension_instance(obj);
        typedef std::vector<py::InstanceHolderBase*>::const_iterator Iterator;
        for (Iterator p = self->wrapped_objects().begin();
             p != self->wrapped_objects().end(); ++p)
        {
            py::InstancePtrHolder<PtrType, T>* held =
                dynamic_cast<py::InstancePtrHolder<PtrType, T>*>(*p);
            if (held != 0)
                return held->ptr();
        }
        py::report_missing_ptr_data(self, py::ClassRegistry<T>::class_object(), typeid(T));
        throw py::ArgumentError();
    }

    template <class PtrType>
    static PyObject* ptr_to_python(PtrType x)
        {
            py::PyPtr<py::ExtensionInstance> result(create_instance(true));
            result->add_implementation(
                std::auto_ptr<py::InstanceHolderBase>(
                    new py::InstancePtrHolder<PtrType,T>(x)));
            return result.release();
        }

    static py::PyPtr<py::ExtensionInstance> create_instance(bool seek_base)
        {
            if (py::ClassRegistry<T>::class_object() == 0)
                py::report_missing_class_object(typeid(T));
            
            py::Class<py::ExtensionInstance>* class_
                = seek_base && py::ClassRegistry<T>::class_object()->bases().size() > 0
                ? py::Downcast<py::Class<py::ExtensionInstance> >(
                    py::ClassRegistry<T>::class_object()->bases()[0].get()).get()
                : py::ClassRegistry<T>::class_object();
            
            return py::PyPtr<py::ExtensionInstance>(new py::ExtensionInstance(class_));
        }


    // Convert to const T*
    friend const T* from_python(PyObject* p, py::Type<const T*>)
        { return from_python(p, py::Type<T*>()); }

    // Convert to T&
    friend T& from_python(PyObject* p, py::Type<T&>)
        { return *py::check_non_null(from_python(p, py::Type<T*>())); }

    // Convert to const T&
    friend const T& from_python(PyObject* p, py::Type<const T&>)
        { return from_python(p, py::Type<T&>()); }

    // Convert to T
    friend const T& from_python(PyObject* p, py::Type<T>)
        { return from_python(p, py::Type<T&>()); }

    friend std::auto_ptr<T>& from_python(PyObject* p, py::Type<std::auto_ptr<T>&>)
        { return ptr_from_python(p, py::Type<std::auto_ptr<T> >()); }
    
    friend std::auto_ptr<T>& from_python(PyObject* p, py::Type<std::auto_ptr<T> >)
        { return ptr_from_python(p, py::Type<std::auto_ptr<T> >()); }
    
    friend const std::auto_ptr<T>& from_python(PyObject* p, py::Type<const std::auto_ptr<T>&>)
        { return ptr_from_python(p, py::Type<std::auto_ptr<T> >()); }

    friend PyObject* to_python(std::auto_ptr<T> x)
        { return ptr_to_python(x); }

    friend boost::shared_ptr<T>& from_python(PyObject* p, py::Type<boost::shared_ptr<T>&>)
        { return ptr_from_python(p, py::Type<boost::shared_ptr<T> >()); }
    
    friend boost::shared_ptr<T>& from_python(PyObject* p, py::Type<boost::shared_ptr<T> >)
        { return ptr_from_python(p, py::Type<boost::shared_ptr<T> >()); }
    
    friend const boost::shared_ptr<T>& from_python(PyObject* p, py::Type<const boost::shared_ptr<T>&>)
        { return ptr_from_python(p, py::Type<boost::shared_ptr<T> >()); }

    friend PyObject* to_python(boost::shared_ptr<T> x)
        { return ptr_to_python(x); }
};

#ifndef BOOST_MSVC
template <class T, class U>
py::InstanceHolderBase*
py_copy_to_new_value_holder(py::ExtensionInstance* p, const T& x, py::Type<U>)
{
    return new py::InstanceValueHolder<T,U>(p, x);
}

template <class T>
PyObject* to_python(const T& x)
{
    py::PyPtr<py::ExtensionInstance> result(
        PyExtensionClassConverters<T>::create_instance(false));
    result->add_implementation(
        std::auto_ptr<py::InstanceHolderBase>(
            py_copy_to_new_value_holder(result.get(), x, py_holder_type(x))));
    return result.release();
}
#endif

#ifdef PY_NO_INLINE_FRIENDS_IN_NAMESPACE // back from global namespace for this GCC bug
namespace py {
using ::PyExtensionClassConverters;
#endif

template <class T> class InstanceHolder;

class ReadOnlySetattrFunction : public Function
{
 public:
    ReadOnlySetattrFunction(const char* name);
    PyObject* do_call(PyObject* args, PyObject* keywords) const;
    const char* description() const;
 private:
    String m_name;
};

// An easy way to make an extension base class which wraps T. Note that Python
// subclasses of this class will simply be Class<ExtensionInstance> objects.
//
// U should be a class derived from T which overrides virtual functions with
// boilerplate code to call back into Python. See extclass_demo.h for examples.
// 
// U is optional, but you won't be able to override any member functions in
// Python which are called from C++ if you don't supply it. If you just want to
// be able to use T in python without overriding member functions, you can omit
// U.
template <class T, class U = HeldInstance<T> >
class ExtensionClass
    : public PyExtensionClassConverters<T, U>, // This generates the to_python/from_python functions
      public ExtensionClassBase
{
 public:
    typedef T WrappedType;
    typedef U CallbackType;
    
    // Construct with a name that comes from typeid(T).name(). The name only
    // affects the objects of this class are represented through repr()
    ExtensionClass();
    
    // Construct with the given name. The name only affects the objects of this
    // class are represented through repr()
    ExtensionClass(const char* name);
    
    ~ExtensionClass();

    // define constructors
""" % args
        + gen_function(
"""    template <%(class A%n%:, %)>
    void def(Constructor<%(A%n%:, %)>)
    // The following incantation builds a Signature1, Signature2,... object. It
    // should _all_ get optimized away.
    { add_constructor(
        %(prepend(Type<A%n>::Id(),
        %)        Signature0()%()%));
    }
""", args)
        +
"""
    // define member functions. In fact this works for free functions, too -
    // they act like static member functions, or if they start with the
    // appropriate self argument (as a pointer), they can be used just like
    // ordinary member functions -- just like Python!
    template <class Fn>
    void def(Fn fn, const char* name)
    {
        this->add_method(new_wrapped_function(fn), name);
    }

    // Define a virtual member function with a default implementation.
    // default_fn should be a function which provides the default implementation.
    // Be careful that default_fn does not in fact call fn virtually!
    template <class Fn, class DefaultFn>
    void def(Fn fn, const char* name, DefaultFn default_fn)
    {
        this->add_default_method(new_wrapped_function(default_fn), name);
        this->add_method(new_wrapped_function(fn), name);
    }

    // Provide a function which implements x.<name>, reading from the given
    // member (pm) of the T instance
    template <class MemberType>
    void def_getter(MemberType T::*pm, const char* name)
    {
        this->add_getter_method(new GetterFunction<T, MemberType>(pm), name);
    }
    
    // Provide a function which implements assignment to x.<name>, writing to
    // the given member (pm) of the T instance
    template <class MemberType>
    void def_setter(MemberType T::*pm, const char* name)
    {
        this->add_setter_method(new SetterFunction<T, MemberType>(pm), name);
    }
    
    // Expose the given member (pm) of the T instance as a read-only attribute
    template <class MemberType>
    void def_readonly(MemberType T::*pm, const char* name)
    {
        this->add_setter_method(new ReadOnlySetattrFunction(name), name);
        this->def_getter(pm, name);
    }
    
    // Expose the given member (pm) of the T instance as a read/write attribute
    template <class MemberType>
    void def_read_write(MemberType T::*pm, const char* name)
    {
        this->def_getter(pm, name);
        this->def_setter(pm, name);
    }
    
 private:
    typedef InstanceValueHolder<T,U> Holder;
    
    template <class Signature>
    void add_constructor(Signature sig)
    {
        this->add_constructor_object(InitFunction<Holder>::create(sig));
    }
};

// A simple wrapper over a T which allows us to use ExtensionClass<T> with a
// single template parameter only. See ExtensionClass<T>, above.
template <class T>
class HeldInstance : public T
{
    // There are no member functions: we want to avoid inadvertently overriding
    // any virtual functions in T.
public:"""
        + gen_functions(held_instance, args)
        + """
protected:
    PyObject* m_self; // Not really needed; doesn't really hurt.
};

class InstanceHolderBase
{
public:
    virtual ~InstanceHolderBase() {}
};

template <class Held>
class InstanceHolder : public InstanceHolderBase
{
public:
    virtual Held *target() = 0;
};
    
template <class Held, class Wrapper>
class InstanceValueHolder : public InstanceHolder<Held>
{
public:
    Held* target() { return &m_held; }
    Wrapper* value_target() { return &m_held; }
"""
        + gen_functions(instance_value_holder, args)
        + """
private:
    Wrapper m_held;
};
""" +
"""
template <class PtrType, class HeldType>
class InstancePtrHolder : public InstanceHolder<HeldType>
{
 public:
    HeldType* target() { return &*m_ptr; }
    PtrType& ptr() { return m_ptr; }

    InstancePtrHolder(PtrType ptr) : m_ptr(ptr) {}
 private:
    PtrType m_ptr;
};

class ExtensionInstance : public Instance
{
 public:
    ExtensionInstance(PyTypeObject* class_);
    ~ExtensionInstance();
    
    void add_implementation(std::auto_ptr<InstanceHolderBase> holder);

    typedef std::vector<InstanceHolderBase*> WrappedObjects;
    const WrappedObjects& wrapped_objects() const
        { return m_wrapped_objects; }
 private:
    WrappedObjects m_wrapped_objects;
};

//
// Template function implementations
//

template <class T, class U>
ExtensionClass<T, U>::ExtensionClass()
    : ExtensionClassBase(typeid(T).name())
{
    ClassRegistry<T>::register_class(this);
}

template <class T, class U>
ExtensionClass<T, U>::ExtensionClass(const char* name)
    : ExtensionClassBase(name)
{
    ClassRegistry<T>::register_class(this);
}

template <class T, class U>
ExtensionClass<T, U>::~ExtensionClass()
{
    ClassRegistry<T>::unregister_class(this);
}

template <class T>
inline void ClassRegistry<T>::register_class(Class<ExtensionInstance>* p)
{
    // You're not expected to create more than one of these!
    assert(static_class_object == 0);
    static_class_object = p;
}

template <class T>
inline void ClassRegistry<T>::unregister_class(Class<ExtensionInstance>* p)
{
    // The user should be destroying the same object they created.
    assert(static_class_object == p);
    (void)p; // unused in shipping version
    static_class_object = 0;
}

//
// Static data member declaration.
//
template <class T>
Class<py::ExtensionInstance>* ClassRegistry<T>::static_class_object;

} // namespace py

#endif // EXTENSION_CLASS_DWA052000_H_
""")

if __name__ == '__main__':
    import sys

    if len(sys.argv) == 1:
        args = 5
    else:
        args = int(sys.argv[1])

    print gen_extclass(args)
