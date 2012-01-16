// Copyright (C) 2010 Vicente Botet
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>

#include <iostream>

#include <boost/thread.hpp>

	using namespace std;

	boost::mutex mutex_;

	void perform()
	{
	   try
	   {
	      boost::this_thread::sleep(boost::posix_time::seconds(100));
	   }
	   catch(boost::thread_interrupted& interrupt)
	   {
	      boost::mutex::scoped_lock lock(mutex_);
	      cerr << "Thread " << boost::this_thread::get_id() << " got interrupted" << endl;
	      throw(interrupt);
	   }
	   catch(std::exception& e)
	   {
	      boost::mutex::scoped_lock lock(mutex_);
	      cerr << "Thread " << boost::this_thread::get_id() << " caught std::exception" << e.what() << endl;
	   }
	   catch(...)
	   {
	      boost::mutex::scoped_lock lock(mutex_);
	      cerr << "Thread " << boost::this_thread::get_id() << " caught something else" << endl;
	   }
	}

	void test()
	{
	   boost::thread_group threads;

	   for (int i = 0; i < 2; ++i)
	   {
	      threads.create_thread(perform);
	   }

	   //boost::this_thread::sleep(1);
	   threads.interrupt_all();
	   threads.join_all();
	}

boost::unit_test_framework::test_suite* init_unit_test_suite(int, char*[])
{
    boost::unit_test_framework::test_suite* tests =
        BOOST_TEST_SUITE("Boost.Threads: 2309");

    tests->add(BOOST_TEST_CASE(&test));

    return tests;
}
