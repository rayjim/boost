//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gazta�aga 2004-2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/detail/workaround.hpp>
#include "mutex_test_template.hpp"
#include "sharable_mutex_test_template.hpp"
#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>
#include <boost/interprocess/sync/upgradable_lock.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include "util.hpp"

int main ()
{
   using namespace boost::interprocess;

   test::test_all_lock<interprocess_upgradable_mutex>();
   test::test_all_mutex<true, interprocess_upgradable_mutex>();
   test::test_all_sharable_mutex<true, interprocess_upgradable_mutex>();

   //Test lock transition
   {
      typedef interprocess_upgradable_mutex Mutex;
      Mutex mut;
      Mutex mut2;

      //Conversions to scoped_lock
      {
         scoped_lock<Mutex>      lock(mut);
         scoped_lock<Mutex>      e_lock(move(lock));
         lock.swap(move(e_lock));
      }
      {
         scoped_lock<Mutex>      lock(mut);
         scoped_lock<Mutex>      e_lock(mut2);
         e_lock = move(lock);
      }
      {
         upgradable_lock<Mutex>  u_lock(mut);
         //This calls unlock_upgradable_and_lock()
         scoped_lock<Mutex>      e_lock(move(u_lock));
      }
      {
         upgradable_lock<Mutex>  u_lock(mut);
         //This calls unlock_upgradable_and_lock()
         scoped_lock<Mutex>      e_lock(mut2);
         scoped_lock<Mutex>      moved(move(u_lock));
         e_lock = move(moved);
      }
      {
         upgradable_lock<Mutex>  u_lock(mut);
         //This calls try_unlock_upgradable_and_lock()
         scoped_lock<Mutex>      e_lock(move(u_lock), try_to_lock);
      }
      {
         upgradable_lock<Mutex>  u_lock(mut);
         //This calls try_unlock_upgradable_and_lock()
         scoped_lock<Mutex>      e_lock(mut2);
         scoped_lock<Mutex>      moved(move(u_lock), try_to_lock);
         e_lock = move(moved);
      }
      {
         boost::posix_time::ptime t = test::delay(100);
         upgradable_lock<Mutex>  u_lock(mut);
         //This calls timed_unlock_upgradable_and_lock()
         scoped_lock<Mutex>      e_lock(move(u_lock), t);
      }
      {
         boost::posix_time::ptime t = test::delay(100);
         upgradable_lock<Mutex>  u_lock(mut);
         //This calls timed_unlock_upgradable_and_lock()
         scoped_lock<Mutex>      e_lock(mut2);
         scoped_lock<Mutex>      moved(move(u_lock), t);
         e_lock = move(moved);
      }
      {
         sharable_lock<Mutex>    s_lock(mut);
         //This calls try_unlock_sharable_and_lock()
         scoped_lock<Mutex>      e_lock(move(s_lock), try_to_lock);
      }
      {
         sharable_lock<Mutex>    s_lock(mut);
         //This calls try_unlock_sharable_and_lock()
         scoped_lock<Mutex>      e_lock(mut2);
         scoped_lock<Mutex>      moved(move(s_lock), try_to_lock);
         e_lock = move(moved);
      }
      //Conversions to upgradable_lock
      {
         upgradable_lock<Mutex>  lock(mut);
         upgradable_lock<Mutex>  u_lock(move(lock));
         lock.swap(move(u_lock));
      }
      {
         upgradable_lock<Mutex>  lock(mut);
         upgradable_lock<Mutex>  u_lock(mut2);
         upgradable_lock<Mutex>  moved(move(lock));
         u_lock = move(moved);
      }
      {
         sharable_lock<Mutex>    s_lock(mut);
         //This calls unlock_sharable_and_lock_upgradable()
         upgradable_lock<Mutex>  u_lock(move(s_lock), try_to_lock);
      }
      {
         sharable_lock<Mutex>    s_lock(mut);
         //This calls unlock_sharable_and_lock_upgradable()
         upgradable_lock<Mutex>  u_lock(mut2);
         upgradable_lock<Mutex>  moved(move(s_lock), try_to_lock);
         u_lock = move(moved);
      }
      {
         scoped_lock<Mutex>      e_lock(mut);
         //This calls unlock_and_lock_upgradable()
         upgradable_lock<Mutex>  u_lock(move(e_lock));
      }
      {
         scoped_lock<Mutex>      e_lock(mut);
         //This calls unlock_and_lock_upgradable()
         upgradable_lock<Mutex>  u_lock(mut2);
         upgradable_lock<Mutex>  moved(move(e_lock));
         u_lock = move(moved);
      }
      //Conversions to sharable_lock
      {
         sharable_lock<Mutex>    lock(mut);
         sharable_lock<Mutex>    s_lock(move(lock));
         lock.swap(move(s_lock));
      }
      {
         sharable_lock<Mutex>    lock(mut);
         sharable_lock<Mutex>    s_lock(mut2);
         sharable_lock<Mutex>    moved(move(lock));
         s_lock = move(moved);
      }
      {
         upgradable_lock<Mutex>  u_lock(mut);
         //This calls unlock_upgradable_and_lock_sharable()
         sharable_lock<Mutex>    s_lock(move(u_lock));
      }
      {
         upgradable_lock<Mutex>  u_lock(mut);
         //This calls unlock_upgradable_and_lock_sharable()
         sharable_lock<Mutex>    s_lock(mut2);
         sharable_lock<Mutex>    moved(move(u_lock));
         s_lock = move(moved);
      }
      {
         scoped_lock<Mutex>      e_lock(mut);
         //This calls unlock_and_lock_sharable()
         sharable_lock<Mutex>    s_lock(move(e_lock));
      }
      {
         scoped_lock<Mutex>      e_lock(mut);
         //This calls unlock_and_lock_sharable()
         sharable_lock<Mutex>    s_lock(mut2);
         sharable_lock<Mutex>    moved(move(e_lock));
         s_lock = move(moved);
      }
   }

   return 0;
}

#include <boost/interprocess/detail/config_end.hpp>
