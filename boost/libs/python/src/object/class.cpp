// Copyright David Abrahams 2001. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#include <boost/python/detail/prefix.hpp>
#include <boost/mpl/lambda.hpp> // #including this first is an intel6 workaround

#include <boost/python/object/class.hpp>
#include <boost/python/object/instance.hpp>
#include <boost/python/object/class_detail.hpp>
#include <boost/python/scope.hpp>
#include <boost/python/converter/registry.hpp>
#include <boost/python/object/find_instance.hpp>
#include <boost/python/object/pickle_support.hpp>
#include <boost/python/detail/map_entry.hpp>
#include <boost/python/object.hpp>
#include <boost/python/object_protocol.hpp>
#include <boost/detail/binary_search.hpp>
#include <boost/python/self.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/str.hpp>
#include <functional>
#include <vector>
#include <cstddef>
#include <new>
#include <structmember.h>

namespace boost { namespace python {

# ifdef BOOST_PYTHON_SELF_IS_CLASS
namespace self_ns
{
  self_t self;
}
# endif 

instance_holder::instance_holder()
    : m_next(0)
{
}

instance_holder::~instance_holder()
{
}

extern "C"
{
  // This is copied from typeobject.c in the Python sources. Even though
  // class_metatype_object doesn't set Py_TPFLAGS_HAVE_GC, that bit gets
  // filled in by the base class initialization process in
  // PyType_Ready(). However, tp_is_gc is *not* copied from the base
  // type, making it assume that classes are GC-able even if (like
  // class_type_object) they're statically allocated.
  static int
  type_is_gc(PyTypeObject *python_type)
  {
      return python_type->tp_flags & Py_TPFLAGS_HEAPTYPE;
  }

  // This is also copied from the Python sources.  We can't implement
  // static_data as a subclass property effectively without it.
  typedef struct {
      PyObject_HEAD
      PyObject *prop_get;
      PyObject *prop_set;
      PyObject *prop_del;
      PyObject *prop_doc;
  } propertyobject;

  static PyObject *
  static_data_descr_get(PyObject *self, PyObject *obj, PyObject * /*type*/)
  {
      propertyobject *gs = (propertyobject *)self;

      return PyObject_CallFunction(gs->prop_get, "()");
  }

  static int
  static_data_descr_set(PyObject *self, PyObject *obj, PyObject *value)
  {
      propertyobject *gs = (propertyobject *)self;
      PyObject *func, *res;

      if (value == NULL)
          func = gs->prop_del;
      else
          func = gs->prop_set;
      if (func == NULL) {
          PyErr_SetString(PyExc_AttributeError,
                          value == NULL ?
                          "can't delete attribute" :
                          "can't set attribute");
          return -1;
      }
      if (value == NULL)
          res = PyObject_CallFunction(func, "()");
      else
          res = PyObject_CallFunction(func, "(O)", value);
      if (res == NULL)
          return -1;
      Py_DECREF(res);
      return 0;
  }
}

static PyTypeObject static_data_object = {
    PyObject_HEAD_INIT(0)//&PyType_Type)
    0,
    "Boost.Python.StaticProperty",
    PyType_Type.tp_basicsize,
    0,
    0,                                      /* tp_dealloc */
    0,                                      /* tp_print */
    0,                                      /* tp_getattr */
    0,                                      /* tp_setattr */
    0,                                      /* tp_compare */
    0,                                      /* tp_repr */
    0,                                      /* tp_as_number */
    0,                                      /* tp_as_sequence */
    0,                                      /* tp_as_mapping */
    0,                                      /* tp_hash */
    0,                                      /* tp_call */
    0,                                      /* tp_str */
    0,                                      /* tp_getattro */
    0,                                      /* tp_setattro */
    0,                                      /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT // | Py_TPFLAGS_HAVE_GC
    | Py_TPFLAGS_BASETYPE,          /* tp_flags */
    0,                                      /* tp_doc */
    0,                                      /* tp_traverse */
    0,                                      /* tp_clear */
    0,                                      /* tp_richcompare */
    0,                                      /* tp_weaklistoffset */
    0,                                      /* tp_iter */
    0,                                      /* tp_iternext */
    0,                                      /* tp_methods */
    0,                                      /* tp_members */
    0,                                      /* tp_getset */
    0, //&PyProperty_Type,                           /* tp_base */
    0,                                      /* tp_dict */
    static_data_descr_get,                                      /* tp_descr_get */
    static_data_descr_set,                                      /* tp_descr_set */
    0,                                      /* tp_dictoffset */
    0,                                      /* tp_init */
    0,                                      /* tp_alloc */
    0, // filled in with type_new           /* tp_new */
    0, // filled in with __PyObject_GC_Del  /* tp_free */
    (inquiry)type_is_gc,                    /* tp_is_gc */
};

namespace objects
{
  extern "C"
  {
      // This declaration needed due to broken Python 2.2 headers
      extern DL_IMPORT(PyTypeObject) PyProperty_Type;
  }

  BOOST_PYTHON_DECL PyObject* static_data()
  {
      if (static_data_object.tp_dict == 0)
      {
          static_data_object.ob_type = &PyType_Type;
          static_data_object.tp_base = &PyProperty_Type;
          if (PyType_Ready(&static_data_object))
              return 0;
      }
      return upcast<PyObject>(&static_data_object);
  }
}  

extern "C"
{
    // Ordinarily, descriptors have a certain assymetry: you can use
    // them to read attributes off the class object they adorn, but
    // writing the same attribute on the class object always replaces
    // the descriptor in the class __dict__.  In order to properly
    // represent C++ static data members, we need to allow them to be
    // written through the class instance.  This function of the
    // metaclass makes it possible.
    static int
    class_setattro(PyObject *obj, PyObject *name, PyObject* value)
    {
        // Must use "private" Python implementation detail
        // _PyType_Lookup instead of PyObject_GetAttr because the
        // latter will always end up calling the descr_get function on
        // any descriptor it finds; we need the unadulterated
        // descriptor here.
        PyObject* a = _PyType_Lookup(downcast<PyTypeObject>(obj), name);

        // a is a borrowed reference or 0
        
        // If we found a static data descriptor, call it directly to
        // force it to set the static data member
        if (a != 0 && PyObject_IsInstance(a, objects::static_data()))
            return a->ob_type->tp_descr_set(a, obj, value);
        else
            return PyType_Type.tp_setattro(obj, name, value);
    }
}

static PyTypeObject class_metatype_object = {
    PyObject_HEAD_INIT(0)//&PyType_Type)
    0,
    "Boost.Python.class",
    PyType_Type.tp_basicsize,
    0,
    0,                                      /* tp_dealloc */
    0,                                      /* tp_print */
    0,                                      /* tp_getattr */
    0,                                      /* tp_setattr */
    0,                                      /* tp_compare */
    0,                                      /* tp_repr */
    0,                                      /* tp_as_number */
    0,                                      /* tp_as_sequence */
    0,                                      /* tp_as_mapping */
    0,                                      /* tp_hash */
    0,                                      /* tp_call */
    0,                                      /* tp_str */
    0,                                      /* tp_getattro */
    class_setattro,                         /* tp_setattro */
    0,                                      /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT // | Py_TPFLAGS_HAVE_GC
    | Py_TPFLAGS_BASETYPE,          /* tp_flags */
    0,                                      /* tp_doc */
    0,                                      /* tp_traverse */
    0,                                      /* tp_clear */
    0,                                      /* tp_richcompare */
    0,                                      /* tp_weaklistoffset */
    0,                                      /* tp_iter */
    0,                                      /* tp_iternext */
    0,                                      /* tp_methods */
    0,                                      /* tp_members */
    0,                                      /* tp_getset */
    0, //&PyType_Type,                           /* tp_base */
    0,                                      /* tp_dict */
    0,                                      /* tp_descr_get */
    0,                                      /* tp_descr_set */
    0,                                      /* tp_dictoffset */
    0,                                      /* tp_init */
    0,                                      /* tp_alloc */
    0, // filled in with type_new           /* tp_new */
    0, // filled in with __PyObject_GC_Del  /* tp_free */
    (inquiry)type_is_gc,                    /* tp_is_gc */
};

// Install the instance data for a C++ object into a Python instance
// object.
void instance_holder::install(PyObject* self) throw()
{
    assert(self->ob_type->ob_type == &class_metatype_object);
    m_next = ((objects::instance<>*)self)->objects;
    ((objects::instance<>*)self)->objects = this;
}


namespace objects
{
// Get the metatype object for all extension classes.
  BOOST_PYTHON_DECL type_handle class_metatype()
  {
      if (class_metatype_object.tp_dict == 0)
      {
          class_metatype_object.ob_type = &PyType_Type;
          class_metatype_object.tp_base = &PyType_Type;
          if (PyType_Ready(&class_metatype_object))
              return type_handle();
      }
      return type_handle(borrowed(&class_metatype_object));
  }
  extern "C"
  {
      static void instance_dealloc(PyObject* inst)
      {
          instance<>* kill_me = (instance<>*)inst;

          for (instance_holder* p = kill_me->objects, *next; p != 0; p = next)
          {
              next = p->next();
              p->~instance_holder();
              instance_holder::deallocate(inst, dynamic_cast<void*>(p));
          }
        
          // Python 2.2.1 won't add weak references automatically when
          // tp_itemsize > 0, so we need to manage that
          // ourselves. Accordingly, we also have to clean up the
          // weakrefs ourselves.
          if (kill_me->weakrefs != NULL)
            PyObject_ClearWeakRefs(inst);

          Py_XDECREF(kill_me->dict);
          
          inst->ob_type->tp_free(inst);
      }

      static PyObject *
      instance_new(PyTypeObject* type_, PyObject* args, PyObject *kw)
      {
          // Attempt to find the __instance_size__ attribute. If not present, no problem.
          PyObject* d = type_->tp_dict;
          PyObject* instance_size_obj = PyObject_GetAttrString(d, "__instance_size__");

          long instance_size = 0;
          if (instance_size != 0)
              instance_size = PyInt_AsLong(instance_size_obj);
          
          if (instance_size < 0)
              instance_size = 0;
          PyErr_Clear(); // Clear any errors that may have occurred.

          instance<>* result = (instance<>*)type_->tp_alloc(type_, instance_size);
          if (result)
          {
              // Guido says we can use ob_size for any purpose we
              // like, so we'll store the total size of the object
              // there. A negative number indicates that the extra
              // instance memory is not yet allocated to any holders.
              result->ob_size = -(static_cast<int>(offsetof(instance<>,storage) + instance_size));
          }
          return (PyObject*)result;
      }

      static PyObject* instance_get_dict(PyObject* op, void*)
      {
          instance<>* inst = downcast<instance<> >(op);
          if (inst->dict == 0)
              inst->dict = PyDict_New();
          return python::xincref(inst->dict);
      }
    
      static int instance_set_dict(PyObject* op, PyObject* dict, void*)
      {
          instance<>* inst = downcast<instance<> >(op);
          python::xdecref(inst->dict);
          inst->dict = python::incref(dict);
          return 0;
      }

  }


  static PyGetSetDef instance_getsets[] = {
      {"__dict__",  instance_get_dict,  instance_set_dict, NULL},
      {0}
  };

  
  static PyMemberDef instance_members[] = {
      {"__weakref__", T_OBJECT, offsetof(instance<>, weakrefs), 0},
      {0}
  };

  static PyTypeObject class_type_object = {
      PyObject_HEAD_INIT(0) //&class_metatype_object)
      0,
      "Boost.Python.instance",
      offsetof(instance<>,storage),           /* tp_basicsize */
      1,                                      /* tp_itemsize */
      instance_dealloc,                       /* tp_dealloc */
      0,                                      /* tp_print */
      0,                                      /* tp_getattr */
      0,                                      /* tp_setattr */
      0,                                      /* tp_compare */
      0,                                      /* tp_repr */
      0,                                      /* tp_as_number */
      0,                                      /* tp_as_sequence */
      0,                                      /* tp_as_mapping */
      0,                                      /* tp_hash */
      0,                                      /* tp_call */
      0,                                      /* tp_str */
      0,                                      /* tp_getattro */
      0,                                      /* tp_setattro */
      0,                                      /* tp_as_buffer */
      Py_TPFLAGS_DEFAULT // | Py_TPFLAGS_HAVE_GC
      | Py_TPFLAGS_BASETYPE,          /* tp_flags */
      0,                                      /* tp_doc */
      0,                                      /* tp_traverse */
      0,                                      /* tp_clear */
      0,                                      /* tp_richcompare */
      offsetof(instance<>,weakrefs),          /* tp_weaklistoffset */
      0,                                      /* tp_iter */
      0,                                      /* tp_iternext */
      0,                                      /* tp_methods */
      instance_members,                       /* tp_members */
      instance_getsets,                       /* tp_getset */
      0, //&PyBaseObject_Type,                /* tp_base */
      0,                                      /* tp_dict */
      0,                                      /* tp_descr_get */
      0,                                      /* tp_descr_set */
      offsetof(instance<>,dict),              /* tp_dictoffset */
      0,                                      /* tp_init */
      PyType_GenericAlloc,                    /* tp_alloc */
      instance_new                            /* tp_new */
  };

  BOOST_PYTHON_DECL type_handle class_type()
  {
      if (class_type_object.tp_dict == 0)
      {
          class_type_object.ob_type = incref(class_metatype().get());
          class_type_object.tp_base = &PyBaseObject_Type;
          if (PyType_Ready(&class_type_object))
              return type_handle();
//          class_type_object.tp_setattro = class_setattro;
      }
      return type_handle(borrowed(&class_type_object));
  }

  BOOST_PYTHON_DECL void*
  find_instance_impl(PyObject* inst, type_info type)
  {
      if (inst->ob_type->ob_type != &class_metatype_object)
          return 0;
    
      instance<>* self = reinterpret_cast<instance<>*>(inst);

      for (instance_holder* match = self->objects; match != 0; match = match->next())
      {
          void* const found = match->holds(type);
          if (found)
              return found;
      }
      return 0;
  }

  object module_prefix()
  {
      return object(
          PyObject_IsInstance(scope().ptr(), upcast<PyObject>(&PyModule_Type))
          ? object(scope().attr("__name__"))
          : api::getattr(scope(), "__module__", str())
          );
  }

  namespace
  {
    // Find a registered class object corresponding to id. Return a
    // null handle if no such class is registered.
    inline type_handle query_class(type_info id)
    {
        converter::registration const* p = converter::registry::query(id);
        return type_handle(
            python::borrowed(
                python::allow_null(p ? p->m_class_object : 0))
            );
    }

    // Find a registered class corresponding to id. If not found,
    // throw an appropriate exception.
    type_handle get_class(type_info id)
    {
        type_handle result(query_class(id));

        if (result.get() == 0)
        {
            object report("extension class wrapper for base class ");
            report = report + id.name() + " has not been created yet";
            PyErr_SetObject(PyExc_RuntimeError, report.ptr());
            throw_error_already_set();
        }
        return result;
    }

    // class_base constructor
    //
    // name -      the name of the new Python class
    //
    // num_types - one more than the number of declared bases
    //
    // types -     array of python::type_info, the first item
    //             corresponding to the class being created, and the
    //             rest corresponding to its declared bases.
    //
    inline object
    new_class(char const* name, std::size_t num_types, type_info const* const types, char const* doc)
    {
      assert(num_types >= 1);
      
      // Build a tuple of the base Python type objects. If no bases
      // were declared, we'll use our class_type() as the single base
      // class.
      std::size_t const num_bases = (std::max)(num_types - 1, static_cast<std::size_t>(1));
      handle<> bases(PyTuple_New(num_bases));

      for (std::size_t i = 1; i <= num_bases; ++i)
      {
          type_handle c = (i >= num_types) ? class_type() : get_class(types[i]);
          // PyTuple_SET_ITEM steals this reference
          PyTuple_SET_ITEM(bases.get(), i - 1, upcast<PyObject>(c.release()));
      }

      // Call the class metatype to create a new class
      dict d;
   
      object m = module_prefix();
      if (m) d["__module__"] = m;

      if (doc != 0)
          d["__doc__"] = doc;
      
      object result = object(class_metatype())(name, bases, d);
      assert(PyType_IsSubtype(result.ptr()->ob_type, &PyType_Type));
      
      if (scope().ptr() != Py_None)
          scope().attr(name) = result;

      return result;
    }
  }
  
  class_base::class_base(
      char const* name, std::size_t num_types, type_info const* const types, char const* doc)
      : object(new_class(name, num_types, types, doc))
  {
      // Insert the new class object in the registry
      converter::registration& converters = const_cast<converter::registration&>(
          converter::registry::lookup(types[0]));

      // Class object is leaked, for now
      converters.m_class_object = (PyTypeObject*)incref(this->ptr());
  }

  BOOST_PYTHON_DECL void copy_class_object(type_info const& src, type_info const& dst)
  {
      converter::registration& dst_converters
          = const_cast<converter::registration&>(converter::registry::lookup(dst));
      
      converter::registration const& src_converters = converter::registry::lookup(src);

      dst_converters.m_class_object = src_converters.m_class_object;
  }
  
  void class_base::set_instance_size(std::size_t instance_size)
  {
      this->attr("__instance_size__") = instance_size;
  }
  
  void class_base::add_property(char const* name, object const& fget)
  {
      object property(
          (python::detail::new_reference)
              PyObject_CallFunction((PyObject*)&PyProperty_Type, "O", fget.ptr()));
      
      this->setattr(name, property);
  }

  void class_base::add_property(char const* name, object const& fget, object const& fset)
  {
      object property(
          (python::detail::new_reference)
              PyObject_CallFunction((PyObject*)&PyProperty_Type, "OO", fget.ptr(), fset.ptr()));
      
      this->setattr(name, property);
  }

  void class_base::add_static_property(char const* name, object const& fget)
  {
      object property(
          (python::detail::new_reference)
          PyObject_CallFunction(static_data(), "O", fget.ptr()));
      
      this->setattr(name, property);
  }

  void class_base::add_static_property(char const* name, object const& fget, object const& fset)
  {
      object property(
          (python::detail::new_reference)
          PyObject_CallFunction(static_data(), "OO", fget.ptr(), fset.ptr()));
      
      this->setattr(name, property);
  }

  void class_base::setattr(char const* name, object const& x)
  {
      if (PyObject_SetAttrString(this->ptr(), const_cast<char*>(name), x.ptr()) < 0)
          throw_error_already_set();
  }

  namespace
  {
    extern "C" PyObject* no_init(PyObject*, PyObject*)
    {
        ::PyErr_SetString(::PyExc_RuntimeError, "This class cannot be instantiated from Python");
        return NULL;
    }
    static ::PyMethodDef no_init_def = {
        "__init__", no_init, METH_VARARGS,
        "Raises an exception\n"
        "This class cannot be instantiated from Python\n"
    };
  }
  
  void class_base::def_no_init()
  {
      handle<> f(::PyCFunction_New(&no_init_def, 0));
      this->setattr("__init__", object(f));
  }

  void class_base::enable_pickling_(bool getstate_manages_dict)
  {
      setattr("__reduce__", object(make_instance_reduce_function()));
      setattr("__safe_for_unpickling__", object(true));
      
      if (getstate_manages_dict)
      {
          setattr("__getstate_manages_dict__", object(true));
      }
  }

  namespace
  {
    PyObject* callable_check(PyObject* callable)
    {
        if (PyCallable_Check(expect_non_null(callable)))
            return callable;

        ::PyErr_Format(
            PyExc_TypeError
            , "staticmethod expects callable object; got an object of type %s, which is not callable"
            , callable->ob_type->tp_name
            );
        
        throw_error_already_set();
        return 0;
    }
  }
  
  void class_base::make_method_static(const char * method_name)
  {
      PyTypeObject* self = downcast<PyTypeObject>(this->ptr());
      dict d((handle<>(borrowed(self->tp_dict))));

      object method(d[method_name]);

      this->attr(method_name) = object(
          handle<>(
              PyStaticMethod_New((callable_check)(method.ptr()) )
              ));
  }

  BOOST_PYTHON_DECL type_handle registered_class_object(type_info id)
  {
      return query_class(id);
  }
} // namespace objects


void* instance_holder::allocate(PyObject* self_, std::size_t holder_offset, std::size_t holder_size)
{
    assert(self_->ob_type->ob_type == &class_metatype_object);
    objects::instance<>* self = (objects::instance<>*)self_;
    
    int total_size_needed = holder_offset + holder_size;
    
    if (-self->ob_size >= total_size_needed)
    {
        // holder_offset should at least point into the variable-sized part
        assert(holder_offset >= offsetof(objects::instance<>,storage));

        // Record the fact that the storage is occupied, noting where it starts
        self->ob_size = holder_offset;
        return (char*)self + holder_offset;
    }
    else
    {
        void* const result = PyMem_Malloc(holder_size);
        if (result == 0)
            throw std::bad_alloc();
        return result;
    }
}

void instance_holder::deallocate(PyObject* self_, void* storage) throw()
{
    assert(self_->ob_type->ob_type == &class_metatype_object);
    objects::instance<>* self = (objects::instance<>*)self_;
    if (storage != (char*)self + self->ob_size)
    {
        PyMem_Free(storage);
    }
}

}} // namespace boost::python
