#ifndef BOOST_DETAIL_SHARED_COUNT_HPP_INCLUDED
#define BOOST_DETAIL_SHARED_COUNT_HPP_INCLUDED

#if _MSC_VER >= 1020
#pragma once
#endif

//
//  detail/shared_count.hpp
//
//  Copyright (c) 2001, 2002 Peter Dimov and Multi Media Ltd.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#include <boost/config.hpp>

#ifndef BOOST_NO_AUTO_PTR
# include <memory>
#endif

#include <boost/checked_delete.hpp>
#include <boost/throw_exception.hpp>
#include <boost/detail/lightweight_mutex.hpp>

#include <functional>       // for std::less
#include <exception>        // for std::exception
#include <new>              // for std::bad_alloc
#include <typeinfo>         // for std::type_info in get_deleter

#ifdef __BORLANDC__
# pragma warn -8026     // Functions with excep. spec. are not expanded inline
# pragma warn -8027     // Functions containing try are not expanded inline
#endif

namespace boost
{

// Debug hooks

#if defined(BOOST_ENABLE_SP_DEBUG_HOOKS)

void sp_scalar_constructor_hook(void * p);
void sp_array_constructor_hook(void * p);
void sp_scalar_destructor_hook(void * p);
void sp_array_destructor_hook(void * p);

#endif


// The standard library that comes with Borland C++ 5.5.1
// defines std::exception and its members as having C calling
// convention (-pc). When the definition of bad_weak_ptr
// is compiled with -ps, the compiler issues an error.
// Hence, the temporary #pragma option -pc below. The version
// check is deliberately conservative.

#if defined(__BORLANDC__) && __BORLANDC__ == 0x551
# pragma option push -pc
#endif

class bad_weak_ptr: public std::exception
{
public:

    virtual char const * what() const throw()
    {
        return "boost::bad_weak_ptr";
    }
};

#if defined(__BORLANDC__) && __BORLANDC__ == 0x551
# pragma option pop
#endif

namespace detail
{

class counted_base
{
private:

    typedef detail::lightweight_mutex mutex_type;

public:

    counted_base(): use_count_(1), weak_count_(1)
    {
    }

    virtual ~counted_base() // nothrow
    {
    }

    // dispose() is called when use_count_ drops to zero, to release
    // the resources managed by *this.

    virtual void dispose() = 0; // nothrow

    // destruct() is called when weak_count_ drops to zero.

    virtual void destruct() // nothrow
    {
        delete this;
    }

    virtual void * get_deleter(std::type_info const & ti) = 0;

    void add_ref()
    {
#if defined(BOOST_HAS_THREADS)
        mutex_type::scoped_lock lock(mtx_);
#endif
        if(use_count_ == 0 && weak_count_ != 0) boost::throw_exception(boost::bad_weak_ptr());
        ++use_count_;
        ++weak_count_;
    }

    void release() // nothrow
    {
        {
#if defined(BOOST_HAS_THREADS)
            mutex_type::scoped_lock lock(mtx_);
#endif
            long new_use_count = --use_count_;

            if(new_use_count != 0)
            {
                --weak_count_;
                return;
            }
        }

        dispose();
        weak_release();
    }

    void weak_add_ref() // nothrow
    {
#if defined(BOOST_HAS_THREADS)
        mutex_type::scoped_lock lock(mtx_);
#endif
        ++weak_count_;
    }

    void weak_release() // nothrow
    {
        long new_weak_count;

        {
#if defined(BOOST_HAS_THREADS)
            mutex_type::scoped_lock lock(mtx_);
#endif
            new_weak_count = --weak_count_;
        }

        if(new_weak_count == 0)
        {
            destruct();
        }
    }

    long use_count() const // nothrow
    {
#if defined(BOOST_HAS_THREADS)
        mutex_type::scoped_lock lock(mtx_);
#endif
        return use_count_;
    }

private:

    counted_base(counted_base const &);
    counted_base & operator= (counted_base const &);

    // inv: use_count_ <= weak_count_

    long use_count_;
    long weak_count_;

#if defined(BOOST_HAS_THREADS)
    mutable mutex_type mtx_;
#endif
};

#if defined(BOOST_ENABLE_SP_DEBUG_HOOKS)

template<class T> void cbi_call_constructor_hook(T * p, checked_deleter<T> const &, int)
{
    boost::sp_scalar_constructor_hook(p);
}

template<class T> void cbi_call_constructor_hook(T * p, checked_array_deleter<T> const &, int)
{
    boost::sp_array_constructor_hook(p);
}

template<class P, class D> void cbi_call_constructor_hook(P const &, D const &, long)
{
}

template<class T> void cbi_call_destructor_hook(T * p, checked_deleter<T> const &, int)
{
    boost::sp_scalar_destructor_hook(p);
}

template<class T> void cbi_call_destructor_hook(T * p, checked_array_deleter<T> const &, int)
{
    boost::sp_array_destructor_hook(p);
}

template<class P, class D> void cbi_call_destructor_hook(P const &, D const &, long)
{
}

#endif

//
// Borland's Codeguard trips up over the -Vx- option here:
//
#ifdef __CODEGUARD__
# pragma option push -Vx-
#endif

template<class P, class D> class counted_base_impl: public counted_base
{
private:

    P ptr; // copy constructor must not throw
    D del; // copy constructor must not throw

    counted_base_impl(counted_base_impl const &);
    counted_base_impl & operator= (counted_base_impl const &);

    typedef counted_base_impl<P, D> this_type;

public:

    // pre: initial_use_count <= initial_weak_count, d(p) must not throw

    counted_base_impl(P p, D d): ptr(p), del(d)
    {
#if defined(BOOST_ENABLE_SP_DEBUG_HOOKS)
        detail::cbi_call_constructor_hook(p, d, 0);
#endif
    }

    virtual void dispose() // nothrow
    {
#if defined(BOOST_ENABLE_SP_DEBUG_HOOKS)
        detail::cbi_call_destructor_hook(ptr, del, 0);
#endif
        del(ptr);
    }

    virtual void * get_deleter(std::type_info const & ti)
    {
        return ti == typeid(D)? &del: 0;
    }

#if defined(BOOST_SP_USE_STD_ALLOCATOR)

    void * operator new(std::size_t)
    {
        return std::allocator<this_type>().allocate(1, static_cast<this_type *>(0));
    }

    void operator delete(void * p)
    {
        std::allocator<this_type>().deallocate(static_cast<this_type *>(p), 1);
    }

#endif
};

class weak_count;

class shared_count
{
private:

    counted_base * pi_;

    friend class weak_count;

public:

    shared_count(): pi_(0) // nothrow
    {
    }

    template<class P, class D> shared_count(P p, D d): pi_(0)
    {
#ifndef BOOST_NO_EXCEPTIONS

        try
        {
            pi_ = new counted_base_impl<P, D>(p, d);
        }
        catch(...)
        {
            d(p); // delete p
            throw;
        }

#else

        pi_ = new counted_base_impl<P, D>(p, d);

        if(pi_ == 0)
        {
            d(p); // delete p
            boost::throw_exception(std::bad_alloc());
        }

#endif
    }

#ifndef BOOST_NO_AUTO_PTR

    // auto_ptr<Y> is special cased to provide the strong guarantee

    template<class Y>
    explicit shared_count(std::auto_ptr<Y> & r): pi_(new counted_base_impl< Y *, checked_deleter<Y> >(r.get(), checked_deleter<Y>()))
    {
        r.release();
    }

#endif 

    ~shared_count() // nothrow
    {
        if(pi_ != 0) pi_->release();
    }

    shared_count(shared_count const & r): pi_(r.pi_) // nothrow
    {
        if(pi_ != 0) pi_->add_ref();
    }

    explicit shared_count(weak_count const & r); // throws bad_weak_ptr when r.use_count() == 0

    shared_count & operator= (shared_count const & r) // nothrow
    {
        counted_base * tmp = r.pi_;
        if(tmp != 0) tmp->add_ref();
        if(pi_ != 0) pi_->release();
        pi_ = tmp;

        return *this;
    }

    void swap(shared_count & r) // nothrow
    {
        counted_base * tmp = r.pi_;
        r.pi_ = pi_;
        pi_ = tmp;
    }

    long use_count() const // nothrow
    {
        return pi_ != 0? pi_->use_count(): 42; // '42' is an example of 'unspecified'
    }

    bool unique() const // nothrow
    {
        return use_count() == 1;
    }

    friend inline bool operator==(shared_count const & a, shared_count const & b)
    {
        return a.pi_ == b.pi_;
    }

    friend inline bool operator<(shared_count const & a, shared_count const & b)
    {
        return std::less<counted_base *>()(a.pi_, b.pi_);
    }

    void * get_deleter(std::type_info const & ti) const
    {
        return pi_? pi_->get_deleter(ti): 0;
    }
};

#ifdef __CODEGUARD__
# pragma option pop
#endif


class weak_count
{
private:

    counted_base * pi_;

    friend class shared_count;

public:

    weak_count(): pi_(0) // nothrow
    {
    }

    weak_count(shared_count const & r): pi_(r.pi_) // nothrow
    {
        if(pi_ != 0) pi_->weak_add_ref();
    }

    weak_count(weak_count const & r): pi_(r.pi_) // nothrow
    {
        if(pi_ != 0) pi_->weak_add_ref();
    }

    ~weak_count() // nothrow
    {
        if(pi_ != 0) pi_->weak_release();
    }

    weak_count & operator= (shared_count const & r) // nothrow
    {
        counted_base * tmp = r.pi_;
        if(tmp != 0) tmp->weak_add_ref();
        if(pi_ != 0) pi_->weak_release();
        pi_ = tmp;

        return *this;
    }

    weak_count & operator= (weak_count const & r) // nothrow
    {
        counted_base * tmp = r.pi_;
        if(tmp != 0) tmp->weak_add_ref();
        if(pi_ != 0) pi_->weak_release();
        pi_ = tmp;

        return *this;
    }

    void swap(weak_count & r) // nothrow
    {
        counted_base * tmp = r.pi_;
        r.pi_ = pi_;
        pi_ = tmp;
    }

    long use_count() const // nothrow
    {
        return pi_ != 0? pi_->use_count(): 0;
    }

    friend inline bool operator==(weak_count const & a, weak_count const & b)
    {
        return a.pi_ == b.pi_;
    }

    friend inline bool operator<(weak_count const & a, weak_count const & b)
    {
        return std::less<counted_base *>()(a.pi_, b.pi_);
    }
};

inline shared_count::shared_count(weak_count const & r): pi_(r.pi_)
{
    if(pi_ != 0)
    {
        pi_->add_ref();
    }
    else
    {
        boost::throw_exception(boost::bad_weak_ptr());
    }
}

} // namespace detail

} // namespace boost

#ifdef __BORLANDC__
# pragma warn .8027     // Functions containing try are not expanded inline
# pragma warn .8026     // Functions with excep. spec. are not expanded inline
#endif

#endif  // #ifndef BOOST_DETAIL_SHARED_COUNT_HPP_INCLUDED
