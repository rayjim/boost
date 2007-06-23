/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Olaf Krzikalla 2004-2006.
// (C) Copyright Ion Gaztanaga  2006-2007.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#include <boost/intrusive/detail/config_begin.hpp>
#include <boost/intrusive/unordered_set.hpp>
#include <boost/intrusive/detail/pointer_to_other.hpp>
#include "itestvalue.hpp"
#include "smart_ptr.hpp"
#include "common_functors.hpp"
#include <vector>
#include <set>
#include <boost/detail/lightweight_test.hpp>
#include "test_macros.hpp"

using namespace boost::intrusive;

static const std::size_t BucketSize = 11;

template<class ValueTraits>
struct test_unordered_multiset
{
   typedef typename ValueTraits::value_type value_type;
   static void test_all (std::vector<value_type>& values);
   static void test_sort(std::vector<value_type>& values);
   static void test_insert(std::vector<value_type>& values);
   static void test_swap(std::vector<value_type>& values);
   static void test_rehash(std::vector<value_type>& values);
   static void test_find(std::vector<value_type>& values);
   static void test_impl();
   static void test_clone(std::vector<value_type>& values);
};

template<class ValueTraits>
void test_unordered_multiset<ValueTraits>::test_all (std::vector<typename ValueTraits::value_type>& values)
{
   test_sort(values);
   test_insert(values);
   test_swap(values);
   test_rehash(values);
   test_find(values);
   test_impl();
   test_clone(values);
}

//test case due to an error in tree implementation:
template<class ValueTraits>
void test_unordered_multiset<ValueTraits>::test_impl()
{
   typedef typename ValueTraits::value_type testvalue_t;
   std::vector<testvalue_t> values (5);
   for (int i = 0; i < 5; ++i)
      values[i].value_ = i; 
   typedef boost::intrusive::unordered_multiset
      <ValueTraits
      ,boost::hash<typename ValueTraits::value_type>
      ,std::equal_to<typename ValueTraits::value_type>
      ,ValueTraits::value_type::constant_time_size, std::size_t 
      > unordered_multiset_type;
   typename unordered_multiset_type::bucket_type buckets [BucketSize];
   unordered_multiset_type testset(buckets, BucketSize);

   for (int i = 0; i < 5; ++i)
      testset.insert (values[i]);

   testset.erase (testset.iterator_to (values[0]));
   testset.erase (testset.iterator_to (values[1]));
   testset.insert (values[1]);
     
   testset.erase (testset.iterator_to (values[2]));
   testset.erase (testset.iterator_to (values[3]));
}

//test: constructor, iterator, clear, reverse_iterator, front, back, size:
template<class ValueTraits>
void test_unordered_multiset<ValueTraits>::test_sort(std::vector<typename ValueTraits::value_type>& values)
{
   typedef typename ValueTraits::value_type testvalue_t;
   typedef boost::intrusive::unordered_multiset
      <ValueTraits
      ,boost::hash<typename ValueTraits::value_type>
      ,std::equal_to<typename ValueTraits::value_type>
      ,ValueTraits::value_type::constant_time_size, std::size_t 
      > unordered_multiset_type;
   typename unordered_multiset_type::bucket_type buckets [BucketSize];
   unordered_multiset_type testset1(buckets, BucketSize, values.begin(), values.end());

   {  int init_values [] = { 1, 2, 2, 3, 4, 5 };
      TEST_INTRUSIVE_SEQUENCE( init_values, testset1.begin() );  }
   testset1.clear();
   BOOST_TEST (testset1.empty());
}  
  
//test: insert, const_iterator, const_reverse_iterator, erase, iterator_to:
template<class ValueTraits>
void test_unordered_multiset<ValueTraits>::test_insert(std::vector<typename ValueTraits::value_type>& values)
{
   typedef typename ValueTraits::value_type testvalue_t;
   typedef boost::intrusive::unordered_multiset
      <ValueTraits
      ,boost::hash<typename ValueTraits::value_type>
      ,std::equal_to<typename ValueTraits::value_type>
      ,ValueTraits::value_type::constant_time_size, std::size_t 
      > unordered_multiset_type;
   typename unordered_multiset_type::bucket_type buckets [BucketSize];
   unordered_multiset_type testset(buckets, BucketSize);
   typedef typename unordered_multiset_type::value_type value_type;

   testset.insert(&values[0] + 2, &values[0] + 5);

   const unordered_multiset_type& const_testset = testset;
   {  int init_values [] = { 1, 4, 5 };
      TEST_INTRUSIVE_SEQUENCE( init_values, const_testset.begin() );  }

   typename unordered_multiset_type::iterator i = testset.begin();
   BOOST_TEST (i->value_ == 1);

   i = testset.insert (values[0]);
   BOOST_TEST (&*i == &values[0]);
     
   i = testset.iterator_to (values[2]);
   BOOST_TEST (&*i == &values[2]);
   testset.erase(i);

   {  int init_values [] = { 1, 3, 5 };
      TEST_INTRUSIVE_SEQUENCE( init_values, const_testset.begin() );  }
   testset.clear();
   testset.insert(&values[0], &values[0] + values.size());

   {  int init_values [] = { 1, 2, 2, 3, 4, 5 };
      TEST_INTRUSIVE_SEQUENCE( init_values, const_testset.begin() );  }

   BOOST_TEST (testset.erase(1) == 1);
   BOOST_TEST (testset.erase(2) == 2);
   BOOST_TEST (testset.erase(3) == 1);
   BOOST_TEST (testset.erase(4) == 1);
   BOOST_TEST (testset.erase(5) == 1);
   BOOST_TEST (testset.empty() == true);

   //Now with a single bucket
   typename unordered_multiset_type::bucket_type single_bucket[1];
   unordered_multiset_type testset2(single_bucket, 1);
   testset2.insert(&values[0], &values[0] + values.size());
   BOOST_TEST (testset2.erase(5) == 1);
   BOOST_TEST (testset2.erase(2) == 2);
   BOOST_TEST (testset2.erase(1) == 1);
   BOOST_TEST (testset2.erase(4) == 1);
   BOOST_TEST (testset2.erase(3) == 1);
   BOOST_TEST (testset2.empty() == true);
}  

//test: insert (seq-version), swap, erase (seq-version), size:
template<class ValueTraits>
void test_unordered_multiset<ValueTraits>::test_swap(std::vector<typename ValueTraits::value_type>& values)
{
   typedef typename ValueTraits::value_type testvalue_t;
   typedef boost::intrusive::unordered_multiset
      <ValueTraits
      ,boost::hash<typename ValueTraits::value_type>
      ,std::equal_to<typename ValueTraits::value_type>
      ,ValueTraits::value_type::constant_time_size, std::size_t 
      > unordered_multiset_type;
   typename unordered_multiset_type::bucket_type buckets [BucketSize];
   unordered_multiset_type testset1(buckets, BucketSize, &values[0], &values[0] + 2);

   typename unordered_multiset_type::bucket_type buckets2 [BucketSize];
   unordered_multiset_type testset2(buckets2, BucketSize);

   testset2.insert (&values[0] + 2, &values[0] + 6);
   testset1.swap (testset2);

   {  int init_values [] = { 1, 2, 4, 5 };
      TEST_INTRUSIVE_SEQUENCE( init_values, testset1.begin() );  }

   {  int init_values [] = { 2, 3 };
      TEST_INTRUSIVE_SEQUENCE( init_values, testset2.begin() );  }

   testset1.erase (testset1.iterator_to(values[5]), testset1.end());
   BOOST_TEST (testset1.size() == 1);
   //  BOOST_TEST (&testset1.front() == &values[3]);
   BOOST_TEST (&*testset1.begin() == &values[3]);
}  

//test: rehash:
template<class ValueTraits>
void test_unordered_multiset<ValueTraits>::test_rehash(std::vector<typename ValueTraits::value_type>& values)
{
   typedef typename ValueTraits::value_type testvalue_t;
   typedef boost::intrusive::unordered_multiset
      <ValueTraits
      ,boost::hash<typename ValueTraits::value_type>
      ,std::equal_to<typename ValueTraits::value_type>
      ,ValueTraits::value_type::constant_time_size, std::size_t 
      > unordered_multiset_type;

   typename unordered_multiset_type::bucket_type buckets1 [BucketSize];
   unordered_multiset_type testset1(buckets1, BucketSize, &values[0], &values[0] + 6);
   BOOST_TEST (testset1.size() == 6);
   {  int init_values [] = { 1, 2, 2, 3, 4, 5 };
      TEST_INTRUSIVE_SEQUENCE( init_values, testset1.begin() );  }

   typename unordered_multiset_type::bucket_type buckets2 [2];
   testset1.rehash(buckets2, 2);
   BOOST_TEST (testset1.size() == 6);
   {  int init_values [] = { 4, 2, 2, 5, 3, 1 };
      TEST_INTRUSIVE_SEQUENCE( init_values, testset1.begin() );  }

   typename unordered_multiset_type::bucket_type buckets3 [BucketSize*2];
   testset1.rehash(buckets3, BucketSize*2);
   BOOST_TEST (testset1.size() == 6);
   {  int init_values [] = { 1, 2, 2, 3, 4, 5 };
      TEST_INTRUSIVE_SEQUENCE( init_values, testset1.begin() );  }

   //Now rehash reducing the buckets
   testset1.rehash(buckets3, 2);
   BOOST_TEST (testset1.size() == 6);
   {  int init_values [] = { 4, 2, 2, 5, 3, 1 };
      TEST_INTRUSIVE_SEQUENCE( init_values, testset1.begin() );  }

   //Now rehash increasing the buckets
   testset1.rehash(buckets3, BucketSize*2);
   BOOST_TEST (testset1.size() == 6);
   {  int init_values [] = { 1, 2, 2, 3, 4, 5 };
      TEST_INTRUSIVE_SEQUENCE( init_values, testset1.begin() );  }
}  

//test: find, equal_range (lower_bound, upper_bound):
template<class ValueTraits>
void test_unordered_multiset<ValueTraits>::test_find(std::vector<typename ValueTraits::value_type>& values)
{
   typedef typename ValueTraits::value_type testvalue_t;
   typedef boost::intrusive::unordered_multiset
      <ValueTraits
      ,boost::hash<typename ValueTraits::value_type>
      ,std::equal_to<typename ValueTraits::value_type>
      ,ValueTraits::value_type::constant_time_size, std::size_t 
      > unordered_multiset_type;
   typename unordered_multiset_type::bucket_type buckets[BucketSize];
   unordered_multiset_type testset(buckets, BucketSize, values.begin(), values.end());

   typedef typename unordered_multiset_type::iterator iterator;

   testvalue_t cmp_val;
   cmp_val.value_ = 2;
   iterator i = testset.find (cmp_val);
   BOOST_TEST (i->value_ == 2);
   BOOST_TEST ((++i)->value_ == 2);
   std::pair<iterator,iterator> range = testset.equal_range (cmp_val);
     
   BOOST_TEST (range.first->value_ == 2);
   BOOST_TEST (range.second->value_ == 3);
   BOOST_TEST (std::distance (range.first, range.second) == 2);

   cmp_val.value_ = 7;
   BOOST_TEST (testset.find (cmp_val) == testset.end());
} 


template<class ValueTraits>
void test_unordered_multiset<ValueTraits>
   ::test_clone(std::vector<typename ValueTraits::value_type>& values)
{
   typedef boost::intrusive::unordered_multiset
      <ValueTraits
      ,boost::hash<typename ValueTraits::value_type>
      ,std::equal_to<typename ValueTraits::value_type>
      ,ValueTraits::value_type::constant_time_size, std::size_t 
      > unordered_multiset_type;
   {
      //Test with equal bucket arrays
      typename unordered_multiset_type::bucket_type buckets1 [BucketSize];
      typename unordered_multiset_type::bucket_type buckets2 [BucketSize];
      unordered_multiset_type testset1 (buckets1, BucketSize, values.begin(), values.end());
      unordered_multiset_type testset2 (buckets2, BucketSize);

      testset2.clone_from(testset1, test::new_cloner(), test::delete_disposer());
      //Ordering is not guarantee in the cloning so insert data in a set and test
      std::multiset<typename ValueTraits::value_type>
         src(testset1.begin(), testset1.end());
      std::multiset<typename ValueTraits::value_type>
         dst(testset2.begin(), testset2.end());
      BOOST_TEST (src == dst);
      testset2.clear_and_dispose(test::delete_disposer());
      BOOST_TEST (testset2.empty());
   }
   {
      //Test with bigger source bucket arrays
      typename unordered_multiset_type::bucket_type buckets1 [BucketSize*2];
      typename unordered_multiset_type::bucket_type buckets2 [BucketSize];
      unordered_multiset_type testset1 (buckets1, BucketSize*2, values.begin(), values.end());
      unordered_multiset_type testset2 (buckets2, BucketSize);

      testset2.clone_from(testset1, test::new_cloner(), test::delete_disposer());
      //Ordering is not guarantee in the cloning so insert data in a set and test
      std::multiset<typename ValueTraits::value_type>
         src(testset1.begin(), testset1.end());
      std::multiset<typename ValueTraits::value_type>
         dst(testset2.begin(), testset2.end());
      BOOST_TEST (src == dst);
      testset2.clear_and_dispose(test::delete_disposer());
      BOOST_TEST (testset2.empty());
   }
   {
      //Test with smaller source bucket arrays
      typename unordered_multiset_type::bucket_type buckets1 [BucketSize];
      typename unordered_multiset_type::bucket_type buckets2 [BucketSize*2];
      unordered_multiset_type testset1 (buckets1, BucketSize, values.begin(), values.end());
      unordered_multiset_type testset2 (buckets2, BucketSize*2);

      testset2.clone_from(testset1, test::new_cloner(), test::delete_disposer());
      //Ordering is not guarantee in the cloning so insert data in a set and test
      std::multiset<typename ValueTraits::value_type>
         src(testset1.begin(), testset1.end());
      std::multiset<typename ValueTraits::value_type>
         dst(testset2.begin(), testset2.end());
      BOOST_TEST (src == dst);
      testset2.clear_and_dispose(test::delete_disposer());
      BOOST_TEST (testset2.empty());
   }
}

template<class VoidPointer, bool constant_time_size>
class test_main_template
{
   public:
   int operator()()
   {
      typedef testvalue<VoidPointer, constant_time_size> testvalue_t;
      static const int random_init[6] = { 3, 2, 4, 1, 5, 2 };
      std::vector<testvalue<VoidPointer, constant_time_size> > data (6);
      for (int i = 0; i < 6; ++i)
         data[i].value_ = random_init[i]; 

      test_unordered_multiset <typename testvalue_t::unordered_set_base_hook_t::template
         value_traits<testvalue_t> >::test_all(data);

      test_unordered_multiset <typename testvalue_t::unordered_set_member_hook_t::template
            value_traits<testvalue_t, &testvalue_t::unordered_set_node_> >::test_all(data);

      return 0;
   }
};

template<class VoidPointer>
class test_main_template<VoidPointer, false>
{
   public:
   int operator()()
   {
      typedef testvalue<VoidPointer, false> testvalue_t;
      static const int random_init[6] = { 3, 2, 4, 1, 5, 2 };
      std::vector<testvalue<VoidPointer, false> > data (6);
      for (int i = 0; i < 6; ++i)
         data[i].value_ = random_init[i]; 

      test_unordered_multiset <typename testvalue_t::unordered_set_base_hook_t::template
         value_traits<testvalue_t> >::test_all(data);

      test_unordered_multiset <typename testvalue_t::unordered_set_member_hook_t::template
            value_traits<testvalue_t, &testvalue_t::unordered_set_node_> >::test_all(data);

      test_unordered_multiset <typename testvalue_t::unordered_set_auto_base_hook_t::template
         value_traits<testvalue_t> >::test_all(data);

      test_unordered_multiset <typename testvalue_t::unordered_set_auto_member_hook_t::template
            value_traits<testvalue_t, &testvalue_t::unordered_set_auto_node_> >::test_all(data);
      return 0;
   }
};

/*
//Explicit instantiations of non-counted classes
template class boost::intrusive::unordered_multiset
   < unordered_set_base_raw_t
   , boost::hash<unordered_set_base_raw_t::value_type>
   , std::equal_to<unordered_set_base_raw_t::value_type>, false>;
template class boost::intrusive::unordered_multiset
   < unordered_set_member_raw_t
   , boost::hash<unordered_set_member_raw_t::value_type>
   , std::equal_to<unordered_set_member_raw_t::value_type>, false>;
template class boost::intrusive::unordered_multiset
   < unordered_set_auto_base_raw
   , boost::hash<unordered_set_auto_base_raw::value_type>
   , std::equal_to<unordered_set_auto_base_raw::value_type>, false>;
template class boost::intrusive::unordered_multiset
   < unordered_set_auto_member_raw
   , boost::hash<unordered_set_auto_member_raw::value_type>
   , std::equal_to<unordered_set_auto_member_raw::value_type>, false>;
template class boost::intrusive::unordered_multiset
   < unordered_set_base_smart
   , boost::hash<unordered_set_base_smart::value_type>
   , std::equal_to<unordered_set_base_smart::value_type>, false>;
template class boost::intrusive::unordered_multiset
   < unordered_set_member_smart
   , boost::hash<unordered_set_member_smart::value_type>
   , std::equal_to<unordered_set_member_smart::value_type>, false>;
template class boost::intrusive::unordered_multiset
   < unordered_set_auto_base_smart
   , boost::hash<unordered_set_auto_base_smart::value_type>
   , std::equal_to<unordered_set_auto_base_smart::value_type>, false>;
template class boost::intrusive::unordered_multiset
   < unordered_set_auto_member_smart
   , boost::hash<unordered_set_auto_member_smart::value_type>
   , std::equal_to<unordered_set_auto_member_smart::value_type>, false>;

//Explicit instantiation of counted classes
template class boost::intrusive::unordered_multiset
   < unordered_set_base_raw_t
   , boost::hash<unordered_set_base_raw_t::value_type>
   , std::equal_to<unordered_set_base_raw_t::value_type>, true>;
template class boost::intrusive::unordered_multiset
   < unordered_set_member_raw_t
   , boost::hash<unordered_set_base_raw_t::value_type>
   , std::equal_to<unordered_set_member_raw_t::value_type>, true>;
template class boost::intrusive::unordered_multiset
   < unordered_set_base_smart_t
   , boost::hash<unordered_set_base_smart_t::value_type>
   , std::equal_to<unordered_set_base_smart_t::value_type>, true>;
template class boost::intrusive::unordered_multiset
   < unordered_set_member_smart_t
   , boost::hash<unordered_set_member_smart_t::value_type>
   , std::equal_to<unordered_set_member_smart_t::value_type>, true>;
*/

int main( int, char* [] ) 
{
   test_main_template<void*, false>()();
   test_main_template<boost::intrusive::smart_ptr<void>, false>()();
   test_main_template<void*, true>()();
   test_main_template<boost::intrusive::smart_ptr<void>, true>()();
   return boost::report_errors();
}

#include <boost/intrusive/detail/config_end.hpp>
