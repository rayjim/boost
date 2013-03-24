#ifndef BOOST_THREAD_SYNC_QUEUE_HPP
#define BOOST_THREAD_SYNC_QUEUE_HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/thread for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/thread/detail/config.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/detail/move.hpp>
#include <boost/throw_exception.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared.hpp>

#include <boost/thread/sync_bounded_queue.hpp>
#include <boost/container/deque.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost
{


  template <typename ValueType>
  class sync_queue
  {
  public:
    typedef ValueType value_type;
    typedef std::size_t size_type;


    BOOST_THREAD_NO_COPYABLE(sync_queue)

    explicit sync_queue();
    template <typename Range>
    sync_queue(Range range);
    ~sync_queue();

    // Observers
    bool empty() const;
    bool full() const;
    size_type size() const;
    bool closed() const;

    // Modifiers
    void wait_and_push(const value_type& x);
    void wait_and_push(BOOST_THREAD_RV_REF(value_type) x);
    bool try_push(const value_type& x);
    bool try_push(BOOST_THREAD_RV_REF(value_type) x);
    bool try_push(no_block_tag, const value_type& x);
    bool try_push(no_block_tag, BOOST_THREAD_RV_REF(value_type) x);

    void wait_and_pop(value_type&);
    void wait_and_pop(ValueType& elem, bool & closed);

    // enable_if is_nothrow_movable<value_type>
    value_type value_pop();
    shared_ptr<ValueType> wait_and_pop();

    bool try_pop(value_type&);
    bool try_pop(no_block_tag,value_type&);
    shared_ptr<ValueType> try_pop();

    void close();

  private:
    mutable mutex mtx_;
    condition_variable not_empty_;
    size_type waiting_empty_;
    boost::container::deque<ValueType> data_;
    bool closed_;


    bool empty(unique_lock<mutex>& ) const BOOST_NOEXCEPT
    {
      //std::cout << __LINE__ << std::endl;
      return data_.empty();
    }
    bool empty(lock_guard<mutex>& ) const BOOST_NOEXCEPT
    {
      //std::cout << __LINE__ << std::endl;
      return data_.empty();
    }

    size_type size(lock_guard<mutex>& ) const BOOST_NOEXCEPT
    {
      return data_.size();
    }

    void throw_if_closed(unique_lock<mutex>&);

    bool try_pop(value_type& x, unique_lock<mutex>& lk);
    bool try_push(const value_type& x, unique_lock<mutex>& lk);
    bool try_push(BOOST_THREAD_RV_REF(value_type) x, unique_lock<mutex>& lk);
    shared_ptr<value_type> try_pop(unique_lock<mutex>& lk);

    void wait_until_not_empty(unique_lock<mutex>& lk);
    void wait_until_not_empty(unique_lock<mutex>& lk, bool&);


    void notify_not_empty_if_needed(unique_lock<mutex>& lk)
    {
      if (waiting_empty_ > 0)
      {
        --waiting_empty_;
        lk.unlock();
        not_empty_.notify_one();
      }
    }

    void pop(value_type& elem, unique_lock<mutex>& )
    {
      elem = boost::move(data_.front());
      data_.pop_front();
    }
    boost::shared_ptr<value_type> pop(unique_lock<mutex>& )
    {
      shared_ptr<value_type> res = make_shared<value_type>(boost::move(data_.front()));
      data_.pop_front();
      return res;
    }

    void push_at(const value_type& elem, unique_lock<mutex>& lk)
    {
      data_.push_back(elem);
      notify_not_empty_if_needed(lk);
    }

    void push_at(BOOST_THREAD_RV_REF(value_type) elem, unique_lock<mutex>& lk)
    {
      data_.push(boost::move(elem));
      notify_not_empty_if_needed(lk);
    }


  };

  template <typename ValueType>
  sync_queue<ValueType>::sync_queue() :
    waiting_empty_(0), data_(), closed_(false)
  {
    BOOST_ASSERT(data_.empty());
  }

//  template <typename ValueType>
//  template <typename Range>
//  sync_queue<ValueType>::sync_queue(size_type max_elems, Range range) :
//    waiting_empty_(0), data_(), closed_(false)
//  {
//    try
//    {
//      typedef typename Range::iterator iterator_t;
//      iterator_t first = boost::begin(range);
//      iterator_t end = boost::end(range);
//      for (iterator_t cur = first; cur != end; ++cur)
//      {
//        data_.push(boost::move(*cur));;
//      }
//      notify_not_empty_if_needed(lk);
//    }
//    catch (...)
//    {
//      delete[] data_;
//    }
//  }

  template <typename ValueType>
  sync_queue<ValueType>::~sync_queue()
  {
  }

  template <typename ValueType>
  void sync_queue<ValueType>::close()
  {
    {
      lock_guard<mutex> lk(mtx_);
      closed_ = true;
    }
    not_empty_.notify_all();
  }

  template <typename ValueType>
  bool sync_queue<ValueType>::closed() const
  {
    lock_guard<mutex> lk(mtx_);
    return closed_;
  }

  template <typename ValueType>
  bool sync_queue<ValueType>::empty() const
  {
    lock_guard<mutex> lk(mtx_);
    return empty(lk);
  }
  template <typename ValueType>
  bool sync_queue<ValueType>::full() const
  {
    return false;
  }

  template <typename ValueType>
  typename sync_queue<ValueType>::size_type sync_queue<ValueType>::size() const
  {
    lock_guard<mutex> lk(mtx_);
    return size(lk);
  }


  template <typename ValueType>
  bool sync_queue<ValueType>::try_pop(ValueType& elem, unique_lock<mutex>& lk)
  {
    if (empty(lk))
    {
      throw_if_closed(lk);
      return false;
    }
    pop(elem, lk);
    return true;
  }
  template <typename ValueType>
  shared_ptr<ValueType> sync_queue<ValueType>::try_pop(unique_lock<mutex>& lk)
  {
    if (empty(lk))
    {
      throw_if_closed(lk);
      return shared_ptr<ValueType>();
    }
    return pop(lk);
  }

  template <typename ValueType>
  bool sync_queue<ValueType>::try_pop(ValueType& elem)
  {
    try
    {
      unique_lock<mutex> lk(mtx_);
      return try_pop(elem, lk);
    }
    catch (...)
    {
      close();
      throw;
    }
  }

  template <typename ValueType>
  bool sync_queue<ValueType>::try_pop(no_block_tag,ValueType& elem)
  {
    try
    {
      unique_lock<mutex> lk(mtx_, try_to_lock);
      if (!lk.owns_lock())
      {
        return false;
      }
      return try_pop(elem, lk);
    }
    catch (...)
    {
      close();
      throw;
    }
  }
  template <typename ValueType>
  boost::shared_ptr<ValueType> sync_queue<ValueType>::try_pop()
  {
    try
    {
      unique_lock<mutex> lk(mtx_);
      return try_pop(lk);
    }
    catch (...)
    {
      close();
      throw;
    }
  }

  template <typename ValueType>
  void sync_queue<ValueType>::throw_if_closed(unique_lock<mutex>&)
  {
    //std::cout << __LINE__ << std::endl;
    if (closed_)
    {
      std::cout << __LINE__ << std::endl;
      BOOST_THROW_EXCEPTION( sync_queue_is_closed() );
    }
  }

  template <typename ValueType>
  void sync_queue<ValueType>::wait_until_not_empty(unique_lock<mutex>& lk)
  {
    for (;;)
    {
      //std::cout << __LINE__ << std::endl;
      if (! empty(lk)) break;
      //std::cout << __LINE__ << std::endl;

      throw_if_closed(lk);
      ++waiting_empty_;
      not_empty_.wait(lk);
    }
  }
  template <typename ValueType>
  void sync_queue<ValueType>::wait_until_not_empty(unique_lock<mutex>& lk, bool & closed)
  {
    for (;;)
    {
      if (! empty(lk)) break;
      //std::cout << __LINE__ << std::endl;
      if (closed_) {closed=true; return;}
      ++waiting_empty_;
      not_empty_.wait(lk);
    }
  }

  template <typename ValueType>
  void sync_queue<ValueType>::wait_and_pop(ValueType& elem)
  {
    try
    {
      unique_lock<mutex> lk(mtx_);
      //std::cout << __LINE__ << std::endl;
      wait_until_not_empty(lk);
      pop(elem, lk);
    }
    catch (...)
    {
      close();
      throw;
    }
  }
  template <typename ValueType>
  void sync_queue<ValueType>::wait_and_pop(ValueType& elem, bool & closed)
  {
    try
    {
      unique_lock<mutex> lk(mtx_);
      //std::cout << __LINE__ << std::endl;
      wait_until_not_empty(lk, closed);
      if (closed) {return;}
      pop(elem, lk);
    }
    catch (...)
    {
      close();
      throw;
    }
  }

  // enable if ValueType is nothrow movable
  template <typename ValueType>
  ValueType sync_queue<ValueType>::value_pop()
  {
    try
    {
      value_type elem;
      wait_and_pop(elem);
      return boost::move(elem);
    }
    catch (...)
    {
      close();
      throw;
    }
  }
  template <typename ValueType>
  boost::shared_ptr<ValueType> sync_queue<ValueType>::wait_and_pop()
  {
    try
    {
      unique_lock<mutex> lk(mtx_);
      wait_until_not_empty(lk);
      return pop(lk);
    }
    catch (...)
    {
      close();
      throw;
    }
  }

  template <typename ValueType>
  bool sync_queue<ValueType>::try_push(const ValueType& elem, unique_lock<mutex>& lk)
  {
    throw_if_closed(lk);
    push_at(elem, lk);
    return true;
  }

  template <typename ValueType>
  bool sync_queue<ValueType>::try_push(const ValueType& elem)
  {
    try
    {
      unique_lock<mutex> lk(mtx_);
      return try_push(elem, lk);
    }
    catch (...)
    {
      close();
      throw;
    }
  }

  template <typename ValueType>
  bool sync_queue<ValueType>::try_push(no_block_tag, const ValueType& elem)
  {
    try
    {
      unique_lock<mutex> lk(mtx_, try_to_lock);
      if (!lk.owns_lock()) return false;
      return try_push(elem, lk);
    }
    catch (...)
    {
      close();
      throw;
    }
  }

  template <typename ValueType>
  void sync_queue<ValueType>::wait_and_push(const ValueType& elem)
  {
    try
    {
      unique_lock<mutex> lk(mtx_);
      throw_if_closed(lk);
      push_at(elem, lk);
    }
    catch (...)
    {
      close();
      throw;
    }
  }

  template <typename ValueType>
  bool sync_queue<ValueType>::try_push(BOOST_THREAD_RV_REF(ValueType) elem, unique_lock<mutex>& lk)
  {
    throw_if_closed(lk);
    push_at(boost::forward<ValueType>(elem), lk);
    return true;
  }

  template <typename ValueType>
  bool sync_queue<ValueType>::try_push(BOOST_THREAD_RV_REF(ValueType) elem)
  {
    try
    {
      unique_lock<mutex> lk(mtx_);
      return try_push(elem, lk);
    }
    catch (...)
    {
      close();
      throw;
    }
  }

  template <typename ValueType>
  bool sync_queue<ValueType>::try_push(no_block_tag, BOOST_THREAD_RV_REF(ValueType) elem)
  {
    try
    {
      unique_lock<mutex> lk(mtx_, try_to_lock);
      if (!lk.owns_lock())
      {
        return false;
      }
      return try_push(elem, lk);
    }
    catch (...)
    {
      close();
      throw;
    }
  }

  template <typename ValueType>
  void sync_queue<ValueType>::wait_and_push(BOOST_THREAD_RV_REF(ValueType) elem)
  {
    try
    {
      unique_lock<mutex> lk(mtx_);
      throw_if_closed(lk);
      push_at(elem, lk);
    }
    catch (...)
    {
      close();
      throw;
    }
  }

  template <typename ValueType>
  sync_queue<ValueType>& operator<<(sync_queue<ValueType>& sbq, BOOST_THREAD_RV_REF(ValueType) elem)
  {
    sbq.wait_and_push(boost::forward<ValueType>(elem));
    return sbq;
  }

  template <typename ValueType>
  sync_queue<ValueType>& operator<<(sync_queue<ValueType>& sbq, ValueType const&elem)
  {
    sbq.wait_and_push(elem);
    return sbq;
  }

  template <typename ValueType>
  sync_queue<ValueType>& operator>>(sync_queue<ValueType>& sbq, ValueType &elem)
  {
    sbq.wait_and_pop(elem);
    return sbq;
  }

}

#include <boost/config/abi_suffix.hpp>

#endif
