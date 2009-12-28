/*
 *
 * Copyright (c) 2003 Dr John Maddock
 * Use, modification and distribution is subject to the 
 * Boost Software License, Version 1.0. (See accompanying file 
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * This file implements the following:
 *    void bcp_implementation::copy_path(const fs::path& p)
 *    void bcp_implementation::create_path(const fs::path& p)
 */

#include "bcp_imp.hpp"
#include "fileview.hpp"
#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <iostream>

struct get_new_library_name
{ 
   get_new_library_name(const std::string& n) : m_new_name(n) {}
   template <class I>
   std::string operator()(const boost::match_results<I>& what)
   {
      std::string s = what[0];
      std::string::size_type n = s.find("boost");
      if(n == std::string::npos)
      {
         s.insert(0, m_new_name);
      }
      else
      {
         s.replace(n, 5, m_new_name);
      }
      return s;
   }
private:
   std::string m_new_name;
};

void bcp_implementation::copy_path(const fs::path& p)
{
   assert(!fs::is_directory(m_boost_path / p));
   if(fs::exists(m_dest_path / p))
   {
      std::cout << "Copying (and overwriting) file: " << p.string() << "\n";
     fs::remove(m_dest_path / p);
   }
   else
      std::cout << "Copying file: " << p.string() << "\n";
   //
   // create the path to the new file if it doesn't already exist:
   //
   create_path(p.branch_path());
   //
   // do text based copy if requested:
   //
   if(m_namespace_name.size() && m_lib_names.size() && is_jam_file(p))
   {
      static std::vector<char> v1, v2;
      v1.clear();
      v2.clear();
      std::ifstream is((m_boost_path / p).native_file_string().c_str());
      std::copy(std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>(), std::back_inserter(v1));

      static boost::regex libname_matcher;
      if(libname_matcher.empty())
      {
         std::string re = "\\<";
         re += *m_lib_names.begin();
         for(std::set<std::string>::const_iterator i = ++m_lib_names.begin(); i != m_lib_names.end(); ++i)
         {
            re += "|" + *i;
         }
         re += "\\>";
         libname_matcher.assign(re);
      }

      regex_replace(std::back_inserter(v2), v1.begin(), v1.end(), libname_matcher, get_new_library_name(m_namespace_name));
      std::swap(v1, v2);
      v2.clear();

      std::ofstream os;
      if(m_unix_lines)
         os.open((m_dest_path / p).native_file_string().c_str(), std::ios_base::binary | std::ios_base::out);
      else
         os.open((m_dest_path / p).native_file_string().c_str(), std::ios_base::out);
      os.write(&*v1.begin(), v1.size());
      os.close();
   }
   else if(m_namespace_name.size() && is_source_file(p))
   {
      //
      // v1 hold the current content, v2 is temp buffer.
      // Each time we do a search and replace the new content 
      // ends up in v2: we then swap v1 and v2, and clear v2.
      //
      static std::vector<char> v1, v2;
      v1.clear();
      v2.clear();
      std::ifstream is((m_boost_path / p).native_file_string().c_str());
      std::copy(std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>(), std::back_inserter(v1));

      static const boost::regex namespace_matcher(
         "(?|"
            "(namespace\\s+)boost\\>()"
         "|"
            "()boost(\\s*(?:::|,|\\)))"
         "|"
            "(namespace\\s+\\w+\\s*=\\s*(?:::\\s*)?)boost\\>()"
         "|"
            "(^\\s*#\\s*define[^\\n]+)boost(\\s*)$"
         ")"
         );

      regex_replace(std::back_inserter(v2), v1.begin(), v1.end(), namespace_matcher, "$1" + m_namespace_name + "$2");
      std::swap(v1, v2);
      v2.clear();

      if(m_namespace_alias)
      {
         static const boost::regex namespace_alias(
            /*
            "namespace\\s+" + m_namespace_name + 
            "\\s*"
            "("
               "\\{"
               "(?:"
                  "(?>[^\\{\\}/]+)"
                  "(?>"
                     "(?:"
                        "(?1)"
                        "|//[^\\n]+$"
                        "|/[^/]"
                        "|(?:^\\s*#[^\\n]*"
                           "(?:(?<=\\\\)\\n[^\\n]*)*)"
                     ")"
                     "[^\\{\\}]+"
                  ")*"
               ")*"
               "\\}"
            ")"
            */
            /*
            "(namespace\\s+" + m_namespace_name + 
            "\\s*\\{.*"
            "\\})([^\\{\\};]*)\\z"
            */
            "namespace\\s+" + m_namespace_name + 
            "\\s*\\{"
            );
         regex_replace(std::back_inserter(v2), v1.begin(), v1.end(), namespace_alias, 
            "namespace " + m_namespace_name + "{} namespace boost = " + m_namespace_name + "; namespace " + m_namespace_name + "{");
         std::swap(v1, v2);
         v2.clear();
      }

      std::ofstream os;
      if(m_unix_lines)
         os.open((m_dest_path / p).native_file_string().c_str(), std::ios_base::binary | std::ios_base::out);
      else
         os.open((m_dest_path / p).native_file_string().c_str(), std::ios_base::out);
      os.write(&*v1.begin(), v1.size());
      os.close();
   }
   else if(m_unix_lines && !is_binary_file(p))
   {
      std::ifstream is((m_boost_path / p).native_file_string().c_str());
      std::istreambuf_iterator<char> isi(is);
      std::istreambuf_iterator<char> end;

      std::ofstream os((m_dest_path / p).native_file_string().c_str(), std::ios_base::binary | std::ios_base::out);
      std::ostreambuf_iterator<char> osi(os);

      std::copy(isi, end, osi);
   }
   else
   {
      // binary copy:
      fs::copy_file(m_boost_path / p, m_dest_path / p);
   }
}

void bcp_implementation::create_path(const fs::path& p)
{
   if(!fs::exists(m_dest_path / p))
   {
      // recurse then create the path:
      create_path(p.branch_path());
      fs::create_directory(m_dest_path / p);
   }
}


