/*
 *
 * Copyright (c) 2003
 * Dr John Maddock
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Dr John Maddock makes no representations
 * about the suitability of this software for any purpose.  
 * It is provided "as is" without express or implied warranty.
 *
 * This file implements the bcp_implementation virtuals.
 */

#include "bcp_imp.hpp"
#include <boost/filesystem/operations.hpp>
#include <iostream>
#include <stdexcept>

bcp_implementation::bcp_implementation()
  : m_list_mode(false), m_cvs_mode(false), m_unix_lines(false), m_scan_mode(false)
{
}

bcp_implementation::~bcp_implementation()
{
}

bcp_application::~bcp_application()
{
}

void bcp_implementation::enable_list_mode()
{
   m_list_mode = true;
}

void bcp_implementation::enable_cvs_mode()
{
   m_cvs_mode = true;
}

void bcp_implementation::enable_scan_mode()
{
   m_scan_mode = true;
}

void bcp_implementation::enable_unix_lines()
{
   m_unix_lines = true;
}

void bcp_implementation::set_boost_path(const char* p)
{
   m_boost_path = fs::path(p, fs::native);
}

void bcp_implementation::set_destination(const char* p)
{
   m_dest_path = fs::path(p, fs::native);
}

void bcp_implementation::add_module(const char* p)
{
   m_module_list.push_back(p);
}

int bcp_implementation::run()
{
   //
   // check output path is OK:
   //
   if(!m_list_mode && !fs::exists(m_dest_path))
   {
      std::string msg("Destination path does not exist: ");
      msg.append(m_dest_path.native_file_string());
      std::runtime_error e(msg);
      boost::throw_exception(e);
   }
   // start by building a list of permitted files
   // if m_cvs_mode is true:
   if(m_cvs_mode)
   {
      scan_cvs_path(fs::path());
   }
   //
   // scan through modules looking for the equivalent
   // file to add to our list:
   //
   std::list<std::string>::const_iterator i = m_module_list.begin();
   std::list<std::string>::const_iterator j = m_module_list.end();
   while(i != j)
   {
      if(m_scan_mode)
      {
         // in scan mode each module must be a real file:
         fs::path p(*i, fs::native);
         add_file_dependencies(p, true);
      }
      else
      {
         if(fs::exists(m_boost_path / "tools" / *i))
            add_path(fs::path("tools") / *i);
         if(fs::exists(m_boost_path / "libs" / *i))
            add_path(fs::path("libs") / *i);
         if(fs::exists(m_boost_path / "boost" / *i))
            add_path(fs::path("boost") / *i);
         if(fs::exists(m_boost_path / "boost" / (*i + ".hpp")))
            add_path(fs::path("boost") / (*i + ".hpp"));
         if(fs::exists(m_boost_path / *i))
            add_path(fs::path(*i));
      }
      ++i;
   }
   //
   // now perform output:
   //
   std::set<fs::path, path_less>::iterator m, n;
   m = m_copy_paths.begin();
   n = m_copy_paths.end();
   while(m != n)
   {
      if(m_list_mode)
         std::cout << m->string() << "\n";
      else
         copy_path(*m);
      ++m;
   }
   return 0;
}

pbcp_application bcp_application::create()
{
   pbcp_application result(static_cast<bcp_application*>(new bcp_implementation()));
   return result;
}
