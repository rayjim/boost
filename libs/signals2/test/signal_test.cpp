// Boost.Signals library

// Copyright Douglas Gregor 2001-2003. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#include <boost/optional.hpp>
#include <boost/test/minimal.hpp>
#include <boost/signals2.hpp>
#include <functional>
#include <iostream>

template<typename T>
struct max_or_default {
  typedef T result_type;
  template<typename InputIterator>
  typename InputIterator::value_type
  operator()(InputIterator first, InputIterator last) const
  {
    boost::optional<T> max;
    for (; first != last; ++first)
    {
      try
      {
        if(max == false) max = *first;
        else max = (*first > max.get())? *first : max;
      }
      catch(const boost::bad_weak_ptr &)
      {}
    }
    if(max) return max.get();
    return T();
  }
};

struct make_int {
  make_int(int n, int cn) : N(n), CN(cn) {}

  int operator()() { return N; }
  int operator()() const { return CN; }

  int N;
  int CN;
};

template<int N>
struct make_increasing_int {
  make_increasing_int() : n(N) {}

  int operator()() const { return n++; }

  mutable int n;
};

static void
test_zero_args()
{
  make_int i42(42, 41);
  make_int i2(2, 1);
  make_int i72(72, 71);
  make_int i63(63, 63);
  make_int i62(62, 61);

  {
    boost::signals2::signal<int (), max_or_default<int> > s0;

    std::cout << "sizeof(signal) = " << sizeof(s0) << std::endl;
    boost::signals2::connection c2 = s0.connect(i2);
    boost::signals2::connection c72 = s0.connect(72, i72);
    boost::signals2::connection c62 = s0.connect(60, i62);
    boost::signals2::connection c42 = s0.connect(i42);

    BOOST_CHECK(s0() == 72);

    s0.disconnect(72);
    BOOST_CHECK(s0() == 62);

    c72.disconnect(); // Double-disconnect should be safe
    BOOST_CHECK(s0() == 62);

    s0.disconnect(72); // Triple-disconect should be safe
    BOOST_CHECK(s0() == 62);

    // Also connect 63 in the same group as 62
    s0.connect(60, i63);
    BOOST_CHECK(s0() == 63);

    // Disconnect all of the 60's
    s0.disconnect(60);
    BOOST_CHECK(s0() == 42);

    c42.disconnect();
    BOOST_CHECK(s0() == 2);

    c2.disconnect();
    BOOST_CHECK(s0() == 0);
  }

  {
    boost::signals2::signal<int (), max_or_default<int> > s0;
    boost::signals2::connection c2 = s0.connect(i2);
    boost::signals2::connection c72 = s0.connect(i72);
    boost::signals2::connection c62 = s0.connect(i62);
    boost::signals2::connection c42 = s0.connect(i42);

    const boost::signals2::signal<int (), max_or_default<int> >& cs0 = s0;
    BOOST_CHECK(cs0() == 72);
  }

  {
    make_increasing_int<7> i7;
    make_increasing_int<10> i10;

    boost::signals2::signal<int (), max_or_default<int> > s0;
    boost::signals2::connection c7 = s0.connect(i7);
    boost::signals2::connection c10 = s0.connect(i10);

    BOOST_CHECK(s0() == 10);
    BOOST_CHECK(s0() == 11);
  }
}

static void
test_one_arg()
{
  boost::signals2::signal<int (int value), max_or_default<int> > s1;

  s1.connect(std::negate<int>());
  s1.connect(std::bind1st(std::multiplies<int>(), 2));

  BOOST_CHECK(s1(1) == 2);
  BOOST_CHECK(s1(-1) == 1);
}

static void
test_signal_signal_connect()
{
  typedef boost::signals2::signal<int (int value), max_or_default<int> > signal_type;
  signal_type s1;

  s1.connect(std::negate<int>());

  BOOST_CHECK(s1(3) == -3);

  {
    signal_type s2;
    s1.connect(s2);
    s2.connect(std::bind1st(std::multiplies<int>(), 2));
    s2.connect(std::bind1st(std::multiplies<int>(), -3));

    BOOST_CHECK(s2(-3) == 9);
    BOOST_CHECK(s1(3) == 6);
  } // s2 goes out of scope and disconnects
  BOOST_CHECK(s1(3) == -3);
}

template<typename ResultType>
  ResultType disconnecting_slot(const boost::signals2::connection &conn, int)
{
  conn.disconnect();
  return ResultType();
}

#ifdef BOOST_NO_VOID_RETURNS
template<>
  void disconnecting_slot<void>(const boost::signals2::connection &conn, int)
{
  conn.disconnect();
  return;
}
#endif

template<typename ResultType>
  void test_extended_slot()
{
  {
    typedef boost::signals2::signal<ResultType (int)> signal_type;
    typedef typename signal_type::extended_slot_type slot_type;
    signal_type sig;
    slot_type myslot(&disconnecting_slot<ResultType>);
    sig.connect_extended(myslot);
    BOOST_CHECK(sig.num_slots() == 1);
    sig(0);
    BOOST_CHECK(sig.num_slots() == 0);
  }
  { // test 0 arg signal
    typedef boost::signals2::signal<ResultType ()> signal_type;
    typedef typename signal_type::extended_slot_type slot_type;
    signal_type sig;
    slot_type myslot(&disconnecting_slot<ResultType>, _1, 0);
    sig.connect_extended(myslot);
    BOOST_CHECK(sig.num_slots() == 1);
    sig();
    BOOST_CHECK(sig.num_slots() == 0);
  }
  // test disconnection by slot
  {
    typedef boost::signals2::signal<ResultType (int)> signal_type;
    typedef typename signal_type::extended_slot_type slot_type;
    signal_type sig;
    slot_type myslot(&disconnecting_slot<ResultType>);
    sig.connect_extended(myslot);
    BOOST_CHECK(sig.num_slots() == 1);
    sig.disconnect(&disconnecting_slot<ResultType>);
    BOOST_CHECK(sig.num_slots() == 0);
  }
}

int
test_main(int, char* [])
{
  test_zero_args();
  test_one_arg();
  test_signal_signal_connect();
  test_extended_slot<void>();
  test_extended_slot<int>();

  return 0;
}
