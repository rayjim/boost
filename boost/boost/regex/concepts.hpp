/*
 *
 * Copyright (c) 2004
 * John Maddock
 *
 * Use, modification and distribution are subject to the 
 * Boost Software License, Version 1.0. (See accompanying file 
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */
 
 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE         concepts.hpp
  *   VERSION      see <boost/version.hpp>
  *   DESCRIPTION: Declares regular expression concepts.
  */

#ifndef BOOST_REGEX_CONCEPTS_HPP_INCLUDED
#define BOOST_REGEX_CONCEPTS_HPP_INCLUDED

#include <boost/concept_archetype.hpp>
#include <boost/concept_check.hpp>
#include <boost/type_traits/is_enum.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/static_assert.hpp>
#include <boost/regex.hpp>
#include <bitset>
#include <vector>

namespace boost{

//
// bitmask_archetype:
// this can be either an integer type, an enum, or a std::bitset,
// we use the latter as the architype as it offers the "strictest"
// of the possible interfaces:
//
typedef std::bitset<512> bitmask_archetype;
//
// char_architype:
// A strict model for the character type interface.
//
struct char_architype
{
   // default constructable:
   char_architype();
   // copy constructable / assignable:
   char_architype(const char_architype&);
   char_architype& operator=(const char_architype&);
   // constructable from an integral value:
   char_architype(unsigned long val);
   // comparable:
   bool operator==(const char_architype&)const;
   bool operator!=(const char_architype&)const;
   bool operator<(const char_architype&)const;
   bool operator<=(const char_architype&)const;
   bool operator>=(const char_architype&)const;
   bool operator>(const char_architype&)const;
   // conversion to integral type:
   operator long()const;
};
//
// char_architype can not be used with basic_string:
//
} // namespace boost
namespace std{
   template<> struct char_traits<boost::char_architype>{};
}
namespace boost{
//
// regex_traits_architype:
// A strict interpretation of the regular expression traits class requirements.
//
template <class charT>
struct regex_traits_architype
{
public:
   regex_traits_architype();
   typedef charT char_type;
   typedef std::size_t size_type;
   typedef std::vector<char_type> string_type;
   typedef copy_constructible_archetype<assignable_archetype<> > locale_type;
   typedef bitmask_archetype char_class_type;

   static size_type length(const char_type* ) { return 0; }

   charT translate(charT ) const { return charT(); }
   charT translate_nocase(charT ) const { return static_object<charT>::get(); }

   template <class ForwardIterator>
   string_type transform(ForwardIterator , ForwardIterator ) const
   { return static_object<string_type>::get(); }
   template <class ForwardIterator>
   string_type transform_primary(ForwardIterator , ForwardIterator ) const
   { return static_object<string_type>::get(); }

   template <class ForwardIterator>
   char_class_type lookup_classname(ForwardIterator , ForwardIterator ) const
   { return static_object<char_class_type>::get(); }
   template <class ForwardIterator>
   string_type lookup_collatename(ForwardIterator , ForwardIterator ) const
   { return static_object<string_type>::get(); }

   bool isctype(charT, char_class_type) const
   { return false; }
   int value(charT, int) const
   { return 0; }

   locale_type imbue(locale_type l)
   { return l; }
   locale_type getloc()const
   { return static_object<locale_type>::get(); }

private:
   // this type is not copyable:
   regex_traits_architype(const regex_traits_architype&);
   regex_traits_architype& operator=(const regex_traits_architype&);
};

//
// alter this to std::tr1, to test a std implementation:
//
namespace global_regex_namespace = ::boost;

template <class Bitmask>
struct BitmaskConcept
{
   void constraints() 
   {
      function_requires<CopyConstructibleConcept<Bitmask> >();
      function_requires<AssignableConcept<Bitmask> >();

      m_mask1 = m_mask2 | m_mask3;
      m_mask1 = m_mask2 & m_mask3;
      m_mask1 = m_mask2 ^ m_mask3;

      m_mask1 = ~m_mask2;

      m_mask1 |= m_mask2;
      m_mask1 &= m_mask2;
      m_mask1 ^= m_mask2;
   }
   Bitmask m_mask1, m_mask2, m_mask3;
};

template <class traits>
struct RegexTraitsConcept
{
   RegexTraitsConcept();
   // required typedefs:
   typedef typename traits::char_type char_type;
   typedef typename traits::size_type size_type;
   typedef typename traits::string_type string_type;
   typedef typename traits::locale_type locale_type;
   typedef typename traits::char_class_type char_class_type;

   void constraints() 
   {
      function_requires<UnsignedIntegerConcept<size_type> >();
      function_requires<RandomAccessContainerConcept<string_type> >();
      function_requires<DefaultConstructibleConcept<locale_type> >();
      function_requires<CopyConstructibleConcept<locale_type> >();
      function_requires<AssignableConcept<locale_type> >();
      function_requires<BitmaskConcept<char_class_type> >();

      size_type n = traits::length(m_pointer);
      ignore_unused_variable_warning(n);

      char_type c = m_ctraits.translate(m_char);
      ignore_unused_variable_warning(c);
      c = m_ctraits.translate_nocase(m_char);
      
      //string_type::foobar bar;
      string_type s1 = m_ctraits.transform(m_pointer, m_pointer);
      ignore_unused_variable_warning(s1);

      string_type s2 = m_ctraits.transform_primary(m_pointer, m_pointer);
      ignore_unused_variable_warning(s2);

      char_class_type cc = m_ctraits.lookup_classname(m_pointer, m_pointer);
      ignore_unused_variable_warning(cc);

      string_type s3 = m_ctraits.lookup_collatename(m_pointer, m_pointer);
      ignore_unused_variable_warning(s3);

      bool b = m_ctraits.isctype(m_char, cc);
      ignore_unused_variable_warning(b);

      int v = m_ctraits.value(m_char, 16);
      ignore_unused_variable_warning(v);

      locale_type l(m_ctraits.getloc());
      m_traits.imbue(l);
      ignore_unused_variable_warning(l);
   }
   traits m_traits;
   const traits m_ctraits;
   const char_type* m_pointer;
   char_type m_char;
};

//
// helper class to compute what traits class a regular expression type is using:
//
template <class Regex>
struct regex_traits_computer;

template <class charT, class traits>
struct regex_traits_computer< global_regex_namespace::basic_regex<charT, traits> >
{
   typedef traits type;
};

//
// BaseRegexConcept does not test anything dependent on basic_string,
// in case our charT does not have an associated char_traits:
//
template <class Regex>
struct BaseRegexConcept
{
   typedef typename Regex::value_type value_type;
   typedef typename Regex::size_type size_type;
   typedef typename Regex::flag_type flag_type;
   typedef typename Regex::locale_type locale_type;
   typedef input_iterator_archetype<value_type> input_iterator_type;

   // derived test types:
   typedef const value_type* pointer_type;
   typedef bidirectional_iterator_archetype<value_type> BidiIterator;
   typedef global_regex_namespace::sub_match<BidiIterator> sub_match_type;
   typedef global_regex_namespace::match_results<BidiIterator> match_results_type;
   typedef output_iterator_archetype<value_type> OutIterator;
   typedef typename regex_traits_computer<Regex>::type traits_type;
   typedef global_regex_namespace::regex_iterator<BidiIterator, value_type, traits_type> regex_iterator_type;
   typedef global_regex_namespace::regex_token_iterator<BidiIterator, value_type, traits_type> regex_token_iterator_type;

   void global_constraints()
   {
      //
      // test non-template components:
      //
      function_requires<BitmaskConcept<global_regex_namespace::regex_constants::syntax_option_type> >();
      global_regex_namespace::regex_constants::syntax_option_type opts
         = global_regex_namespace::regex_constants::icase
         | global_regex_namespace::regex_constants::nosubs
         | global_regex_namespace::regex_constants::optimize
         | global_regex_namespace::regex_constants::collate
         | global_regex_namespace::regex_constants::ECMAScript
         | global_regex_namespace::regex_constants::basic
         | global_regex_namespace::regex_constants::extended
         | global_regex_namespace::regex_constants::awk
         | global_regex_namespace::regex_constants::grep
         | global_regex_namespace::regex_constants::egrep;
      ignore_unused_variable_warning(opts);

      function_requires<BitmaskConcept<global_regex_namespace::regex_constants::match_flag_type> >();
      global_regex_namespace::regex_constants::match_flag_type mopts
         = global_regex_namespace::regex_constants::match_default
         | global_regex_namespace::regex_constants::match_not_bol
         | global_regex_namespace::regex_constants::match_not_eol
         | global_regex_namespace::regex_constants::match_not_bow
         | global_regex_namespace::regex_constants::match_not_eow
         | global_regex_namespace::regex_constants::match_any
         | global_regex_namespace::regex_constants::match_not_null
         | global_regex_namespace::regex_constants::match_continuous
         | global_regex_namespace::regex_constants::match_partial
         | global_regex_namespace::regex_constants::match_prev_avail
         | global_regex_namespace::regex_constants::format_default
         | global_regex_namespace::regex_constants::format_sed
         | global_regex_namespace::regex_constants::format_perl
         | global_regex_namespace::regex_constants::format_no_copy
         | global_regex_namespace::regex_constants::format_first_only;
      ignore_unused_variable_warning(mopts);

      BOOST_STATIC_ASSERT((::boost::is_enum<global_regex_namespace::regex_constants::error_type>::value));
      global_regex_namespace::regex_constants::error_type e1 = global_regex_namespace::regex_constants::error_collate;
      ignore_unused_variable_warning(e1);
      e1 = global_regex_namespace::regex_constants::error_ctype;
      ignore_unused_variable_warning(e1);
      e1 = global_regex_namespace::regex_constants::error_escape;
      ignore_unused_variable_warning(e1);
      e1 = global_regex_namespace::regex_constants::error_backref;
      ignore_unused_variable_warning(e1);
      e1 = global_regex_namespace::regex_constants::error_brack;
      ignore_unused_variable_warning(e1);
      e1 = global_regex_namespace::regex_constants::error_paren;
      ignore_unused_variable_warning(e1);
      e1 = global_regex_namespace::regex_constants::error_brace;
      ignore_unused_variable_warning(e1);
      e1 = global_regex_namespace::regex_constants::error_badbrace;
      ignore_unused_variable_warning(e1);
      e1 = global_regex_namespace::regex_constants::error_range;
      ignore_unused_variable_warning(e1);
      e1 = global_regex_namespace::regex_constants::error_space;
      ignore_unused_variable_warning(e1);
      e1 = global_regex_namespace::regex_constants::error_badrepeat;
      ignore_unused_variable_warning(e1);
      e1 = global_regex_namespace::regex_constants::error_complexity;
      ignore_unused_variable_warning(e1);
      e1 = global_regex_namespace::regex_constants::error_stack;
      ignore_unused_variable_warning(e1);

      BOOST_STATIC_ASSERT((::boost::is_base_and_derived<std::runtime_error, global_regex_namespace::regex_error>::value  ));
      const global_regex_namespace::regex_error except(e1);
      e1 = except.code();

      typedef typename Regex::value_type value_type;
      function_requires< RegexTraitsConcept<global_regex_namespace::regex_traits<char> > >();
      function_requires< BaseRegexConcept<global_regex_namespace::basic_regex<char> > >();
   }
   void constraints() 
   {
      global_constraints();

      BOOST_STATIC_ASSERT((::boost::is_same< flag_type, global_regex_namespace::regex_constants::syntax_option_type>::value));
      flag_type opts
         = Regex::icase
         | Regex::nosubs
         | Regex::optimize
         | Regex::collate
         | Regex::ECMAScript
         | Regex::basic
         | Regex::extended
         | Regex::awk
         | Regex::grep
         | Regex::egrep;
      ignore_unused_variable_warning(opts);

      function_requires<DefaultConstructibleConcept<Regex> >();
      function_requires<CopyConstructibleConcept<Regex> >();

      // Regex constructors:
      Regex e1(m_pointer);
      ignore_unused_variable_warning(e1);
      Regex e2(m_pointer, m_flags);
      ignore_unused_variable_warning(e2);
      Regex e3(m_pointer, m_size, m_flags);
      ignore_unused_variable_warning(e3);
      Regex e4(in1, in2);
      ignore_unused_variable_warning(e4);
      Regex e5(in1, in2, m_flags);
      ignore_unused_variable_warning(e5);

      // assign etc:
      Regex e;
      e = m_pointer;
      e = e1;
      e.assign(e1);
      e.assign(m_pointer);
      e.assign(m_pointer, m_flags);
      e.assign(m_pointer, m_size, m_flags);
      e.assign(in1, in2);
      e.assign(in1, in2, m_flags);

      // access:
      const Regex ce;
      bool b = ce.empty();
      ignore_unused_variable_warning(b);
      size_type i = ce.mark_count();
      ignore_unused_variable_warning(i);
      m_flags = ce.flags();
      e.imbue(ce.getloc());
      e.swap(e1);
      
      global_regex_namespace::swap(e, e1);

      // sub_match:
      BOOST_STATIC_ASSERT((::boost::is_base_and_derived<std::pair<BidiIterator, BidiIterator>, sub_match_type>::value));
      typedef typename sub_match_type::value_type sub_value_type;
      typedef typename sub_match_type::difference_type sub_diff_type;
      typedef typename sub_match_type::iterator sub_iter_type;
      BOOST_STATIC_ASSERT((::boost::is_same<sub_value_type, value_type>::value));
      BOOST_STATIC_ASSERT((::boost::is_same<sub_iter_type, BidiIterator>::value));
      b = m_sub.matched;
      ignore_unused_variable_warning(b);
      BidiIterator bi = m_sub.first;
      ignore_unused_variable_warning(bi);
      bi = m_sub.second;
      ignore_unused_variable_warning(bi);
      sub_diff_type diff = m_sub.length();
      ignore_unused_variable_warning(diff);
      // match_results tests:
      typedef typename match_results_type::value_type mr_value_type;
      typedef typename match_results_type::const_reference mr_const_reference;
      typedef typename match_results_type::reference mr_reference;
      typedef typename match_results_type::const_iterator mr_const_iterator;
      typedef typename match_results_type::iterator mr_iterator;
      typedef typename match_results_type::difference_type mr_difference_type;
      typedef typename match_results_type::size_type mr_size_type;
      typedef typename match_results_type::allocator_type mr_allocator_type;
      typedef typename match_results_type::char_type mr_char_type;
      typedef typename match_results_type::string_type mr_string_type;

      match_results_type m1;
      mr_allocator_type at;
      match_results_type m2(at);
      match_results_type m3(m1);
      m1 = m2;

      mr_size_type mrs = m_cresults.size();
      ignore_unused_variable_warning(mrs);
      mrs = m_cresults.max_size();
      ignore_unused_variable_warning(mrs);
      b = m_cresults.empty();
      ignore_unused_variable_warning(b);
      mr_difference_type mrd = m_cresults.length();
      ignore_unused_variable_warning(mrd);
      mrd = m_cresults.length(mrs);
      ignore_unused_variable_warning(mrd);
      mrd = m_cresults.position();
      ignore_unused_variable_warning(mrd);
      mrd = m_cresults.position(mrs);
      ignore_unused_variable_warning(mrd);

      mr_const_reference mrcr = m_cresults[m_size];
      ignore_unused_variable_warning(mrcr);
      mr_const_reference mrcr2 = m_cresults.prefix();
      ignore_unused_variable_warning(mrcr2);
      mr_const_reference mrcr3 = m_cresults.suffix();
      ignore_unused_variable_warning(mrcr3);
      mr_const_iterator mrci = m_cresults.begin();
      ignore_unused_variable_warning(mrci);
      mrci = m_cresults.end();
      ignore_unused_variable_warning(mrci);

      mr_allocator_type at2 = m_cresults.get_allocator();
      m_results.swap(m_results);
      global_regex_namespace::swap(m_results, m_results);

      // regex_match:
      b = global_regex_namespace::regex_match(m_in, m_in, m_results, e);
      ignore_unused_variable_warning(b);
      b = global_regex_namespace::regex_match(m_in, m_in, m_results, e, m_mft);
      ignore_unused_variable_warning(b);
      b = global_regex_namespace::regex_match(m_in, m_in, e);
      ignore_unused_variable_warning(b);
      b = global_regex_namespace::regex_match(m_in, m_in, e, m_mft);
      ignore_unused_variable_warning(b);
      b = global_regex_namespace::regex_match(m_pointer, m_pmatch, e);
      ignore_unused_variable_warning(b);
      b = global_regex_namespace::regex_match(m_pointer, m_pmatch, e, m_mft);
      ignore_unused_variable_warning(b);
      b = global_regex_namespace::regex_match(m_pointer, e);
      ignore_unused_variable_warning(b);
      b = global_regex_namespace::regex_match(m_pointer, e, m_mft);
      ignore_unused_variable_warning(b);
      // regex_search:
      b = global_regex_namespace::regex_search(m_in, m_in, m_results, e);
      ignore_unused_variable_warning(b);
      b = global_regex_namespace::regex_search(m_in, m_in, m_results, e, m_mft);
      ignore_unused_variable_warning(b);
      b = global_regex_namespace::regex_search(m_in, m_in, e);
      ignore_unused_variable_warning(b);
      b = global_regex_namespace::regex_search(m_in, m_in, e, m_mft);
      ignore_unused_variable_warning(b);
      b = global_regex_namespace::regex_search(m_pointer, m_pmatch, e);
      ignore_unused_variable_warning(b);
      b = global_regex_namespace::regex_search(m_pointer, m_pmatch, e, m_mft);
      ignore_unused_variable_warning(b);
      b = global_regex_namespace::regex_search(m_pointer, e);
      ignore_unused_variable_warning(b);
      b = global_regex_namespace::regex_search(m_pointer, e, m_mft);
      ignore_unused_variable_warning(b);

      // regex_iterator:
      typedef typename regex_iterator_type::regex_type rit_regex_type;
      typedef typename regex_iterator_type::value_type rit_value_type;
      typedef typename regex_iterator_type::difference_type rit_difference_type;
      typedef typename regex_iterator_type::pointer rit_pointer;
      typedef typename regex_iterator_type::reference rit_reference;
      typedef typename regex_iterator_type::iterator_category rit_iterator_category;
      BOOST_STATIC_ASSERT((::boost::is_same<rit_regex_type, Regex>::value));
      BOOST_STATIC_ASSERT((::boost::is_same<rit_value_type, match_results_type>::value));
      BOOST_STATIC_ASSERT((::boost::is_same<rit_difference_type, std::ptrdiff_t>::value));
      BOOST_STATIC_ASSERT((::boost::is_same<rit_pointer, const match_results_type*>::value));
      BOOST_STATIC_ASSERT((::boost::is_same<rit_reference, const match_results_type&>::value));
      BOOST_STATIC_ASSERT((::boost::is_convertible<rit_iterator_category*, std::forward_iterator_tag*>::value));
      // this takes care of most of the checks needed:
      function_requires<ForwardIteratorConcept<regex_iterator_type> >();
      regex_iterator_type iter1(m_in, m_in, e);
      ignore_unused_variable_warning(iter1);
      regex_iterator_type iter2(m_in, m_in, e, m_mft);
      ignore_unused_variable_warning(iter2);

      // regex_token_iterator:
      typedef typename regex_token_iterator_type::regex_type rtit_regex_type;
      typedef typename regex_token_iterator_type::value_type rtit_value_type;
      typedef typename regex_token_iterator_type::difference_type rtit_difference_type;
      typedef typename regex_token_iterator_type::pointer rtit_pointer;
      typedef typename regex_token_iterator_type::reference rtit_reference;
      typedef typename regex_token_iterator_type::iterator_category rtit_iterator_category;
      BOOST_STATIC_ASSERT((::boost::is_same<rtit_regex_type, Regex>::value));
      BOOST_STATIC_ASSERT((::boost::is_same<rtit_value_type, sub_match_type>::value));
      BOOST_STATIC_ASSERT((::boost::is_same<rtit_difference_type, std::ptrdiff_t>::value));
      BOOST_STATIC_ASSERT((::boost::is_same<rtit_pointer, const sub_match_type*>::value));
      BOOST_STATIC_ASSERT((::boost::is_same<rtit_reference, const sub_match_type&>::value));
      BOOST_STATIC_ASSERT((::boost::is_convertible<rtit_iterator_category*, std::forward_iterator_tag*>::value));
      // this takes care of most of the checks needed:
      function_requires<ForwardIteratorConcept<regex_token_iterator_type> >();
      regex_token_iterator_type ti1(m_in, m_in, e);
      ignore_unused_variable_warning(ti1);
      regex_token_iterator_type ti2(m_in, m_in, e, 0);
      ignore_unused_variable_warning(ti2);
      regex_token_iterator_type ti3(m_in, m_in, e, 0, m_mft);
      ignore_unused_variable_warning(ti3);
      std::vector<int> subs;
      regex_token_iterator_type ti4(m_in, m_in, e, subs);
      ignore_unused_variable_warning(ti4);
      regex_token_iterator_type ti5(m_in, m_in, e, subs, m_mft);
      ignore_unused_variable_warning(ti5);
      static const int i_array[3] = { 1, 2, 3, };
      regex_token_iterator_type ti6(m_in, m_in, e, i_array);
      ignore_unused_variable_warning(ti6);
      regex_token_iterator_type ti7(m_in, m_in, e, i_array, m_mft);
      ignore_unused_variable_warning(ti7);
   }

   pointer_type m_pointer;
   flag_type m_flags;
   size_type m_size;
   input_iterator_type in1, in2;
   const sub_match_type m_sub;
   const value_type m_char;
   match_results_type m_results;
   const match_results_type m_cresults;
   OutIterator m_out;
   BidiIterator m_in;
   global_regex_namespace::regex_constants::match_flag_type m_mft;
   global_regex_namespace::match_results<pointer_type> m_pmatch;

   BaseRegexConcept();
   BaseRegexConcept(const BaseRegexConcept&);
   BaseRegexConcept& operator=(const BaseRegexConcept&);
};

//
// RegexConcept:
// Test every interface in the std:
//
template <class Regex>
struct RegexConcept
{
   typedef typename Regex::value_type value_type;
   typedef typename Regex::size_type size_type;
   typedef typename Regex::flag_type flag_type;
   typedef typename Regex::locale_type locale_type;

   // derived test types:
   typedef const value_type* pointer_type;
   typedef std::basic_string<value_type> string_type;
   typedef boost::bidirectional_iterator_archetype<value_type> BidiIterator;
   typedef global_regex_namespace::sub_match<BidiIterator> sub_match_type;
   typedef global_regex_namespace::match_results<BidiIterator> match_results_type;
   typedef output_iterator_archetype<value_type> OutIterator;


   void constraints() 
   {
      function_requires<BaseRegexConcept<Regex> >();
      // string based construct:
      Regex e1(m_string);
      ignore_unused_variable_warning(e1);
      Regex e2(m_string, m_flags);
      ignore_unused_variable_warning(e2);

      // assign etc:
      Regex e;
      e = m_string;
      e.assign(m_string);
      e.assign(m_string, m_flags);

      // sub_match:
      string_type s(m_sub);
      ignore_unused_variable_warning(s);
      s = m_sub.str();
      ignore_unused_variable_warning(s);
      int i = m_sub.compare(m_string);
      ignore_unused_variable_warning(i);

      int i2 = m_sub.compare(m_sub);
      ignore_unused_variable_warning(i2);
      i2 = m_sub.compare(m_pointer);
      ignore_unused_variable_warning(i2);

      bool b = m_sub == m_sub;
      ignore_unused_variable_warning(b);
      b = m_sub != m_sub;
      ignore_unused_variable_warning(b);
      b = m_sub <= m_sub;
      ignore_unused_variable_warning(b);
      b = m_sub <= m_sub;
      ignore_unused_variable_warning(b);
      b = m_sub > m_sub;
      ignore_unused_variable_warning(b);
      b = m_sub >= m_sub;
      ignore_unused_variable_warning(b);

      b = m_sub == m_pointer;
      ignore_unused_variable_warning(b);
      b = m_sub != m_pointer;
      ignore_unused_variable_warning(b);
      b = m_sub <= m_pointer;
      ignore_unused_variable_warning(b);
      b = m_sub <= m_pointer;
      ignore_unused_variable_warning(b);
      b = m_sub > m_pointer;
      ignore_unused_variable_warning(b);
      b = m_sub >= m_pointer;
      ignore_unused_variable_warning(b);

      b = m_pointer == m_sub;
      ignore_unused_variable_warning(b);
      b = m_pointer != m_sub;
      ignore_unused_variable_warning(b);
      b = m_pointer <= m_sub;
      ignore_unused_variable_warning(b);
      b = m_pointer <= m_sub;
      ignore_unused_variable_warning(b);
      b = m_pointer > m_sub;
      ignore_unused_variable_warning(b);
      b = m_pointer >= m_sub;
      ignore_unused_variable_warning(b);

      b = m_sub == m_char;
      ignore_unused_variable_warning(b);
      b = m_sub != m_char;
      ignore_unused_variable_warning(b);
      b = m_sub <= m_char;
      ignore_unused_variable_warning(b);
      b = m_sub <= m_char;
      ignore_unused_variable_warning(b);
      b = m_sub > m_char;
      ignore_unused_variable_warning(b);
      b = m_sub >= m_char;
      ignore_unused_variable_warning(b);

      b = m_char == m_sub;
      ignore_unused_variable_warning(b);
      b = m_char != m_sub;
      ignore_unused_variable_warning(b);
      b = m_char <= m_sub;
      ignore_unused_variable_warning(b);
      b = m_char <= m_sub;
      ignore_unused_variable_warning(b);
      b = m_char > m_sub;
      ignore_unused_variable_warning(b);
      b = m_char >= m_sub;
      ignore_unused_variable_warning(b);

      b = m_sub == m_string;
      ignore_unused_variable_warning(b);
      b = m_sub != m_string;
      ignore_unused_variable_warning(b);
      b = m_sub <= m_string;
      ignore_unused_variable_warning(b);
      b = m_sub <= m_string;
      ignore_unused_variable_warning(b);
      b = m_sub > m_string;
      ignore_unused_variable_warning(b);
      b = m_sub >= m_string;
      ignore_unused_variable_warning(b);

      b = m_string == m_sub;
      ignore_unused_variable_warning(b);
      b = m_string != m_sub;
      ignore_unused_variable_warning(b);
      b = m_string <= m_sub;
      ignore_unused_variable_warning(b);
      b = m_string <= m_sub;
      ignore_unused_variable_warning(b);
      b = m_string > m_sub;
      ignore_unused_variable_warning(b);
      b = m_string >= m_sub;
      ignore_unused_variable_warning(b);

      // match results:
      m_string = m_results.str();
      ignore_unused_variable_warning(m_string);
      m_string = m_results.str(0);
      ignore_unused_variable_warning(m_string);
      m_out = m_cresults.format(m_out, m_string);
      m_out = m_cresults.format(m_out, m_string, m_mft);
      m_string = m_cresults.format(m_string);
      ignore_unused_variable_warning(m_string);
      m_string = m_cresults.format(m_string, m_mft);
      ignore_unused_variable_warning(m_string);

      // regex_match:
      b = global_regex_namespace::regex_match(m_string, m_smatch, e);
      ignore_unused_variable_warning(b);
      b = global_regex_namespace::regex_match(m_string, m_smatch, e, m_mft);
      ignore_unused_variable_warning(b);
      b = global_regex_namespace::regex_match(m_string, e);
      ignore_unused_variable_warning(b);
      b = global_regex_namespace::regex_match(m_string, e, m_mft);
      ignore_unused_variable_warning(b);

      // regex_search:
      b = global_regex_namespace::regex_search(m_string, m_smatch, e);
      ignore_unused_variable_warning(b);
      b = global_regex_namespace::regex_search(m_string, m_smatch, e, m_mft);
      ignore_unused_variable_warning(b);
      b = global_regex_namespace::regex_search(m_string, e);
      ignore_unused_variable_warning(b);
      b = global_regex_namespace::regex_search(m_string, e, m_mft);
      ignore_unused_variable_warning(b);

      // regex_replace:
      m_out = global_regex_namespace::regex_replace(m_out, m_in, m_in, e, m_string, m_mft);
      m_out = global_regex_namespace::regex_replace(m_out, m_in, m_in, e, m_string);
      m_string = global_regex_namespace::regex_replace(m_string, e, m_string, m_mft);
      ignore_unused_variable_warning(m_string);
      m_string = global_regex_namespace::regex_replace(m_string, e, m_string);
      ignore_unused_variable_warning(m_string);

   }

   flag_type m_flags;
   string_type m_string;
   const sub_match_type m_sub;
   match_results_type m_results;
   pointer_type m_pointer;
   value_type m_char;
   const match_results_type m_cresults;
   OutIterator m_out;
   BidiIterator m_in;
   global_regex_namespace::regex_constants::match_flag_type m_mft;
   global_regex_namespace::match_results<typename string_type::const_iterator> m_smatch;

   RegexConcept();
   RegexConcept(const RegexConcept&);
   RegexConcept& operator=(const RegexConcept&);
};

//
// BoostRegexConcept:
// Test every interface in the Boost implementation:
//
template <class Regex>
struct BoostRegexConcept
{
   typedef typename Regex::value_type value_type;
   typedef typename Regex::size_type size_type;
   typedef typename Regex::flag_type flag_type;
   typedef typename Regex::locale_type locale_type;

   // derived test types:
   typedef const value_type* pointer_type;
   typedef std::basic_string<value_type> string_type;
   typedef typename Regex::const_iterator const_iterator;
   typedef bidirectional_iterator_archetype<value_type> BidiIterator;
   typedef global_regex_namespace::sub_match<BidiIterator> sub_match_type;
   typedef global_regex_namespace::match_results<BidiIterator> match_results_type;

   void constraints() 
   {
      function_requires<RegexConcept<Regex> >();
      const global_regex_namespace::regex_error except(global_regex_namespace::regex_constants::error_collate);
      std::ptrdiff_t pt = except.position();
      ignore_unused_variable_warning(pt);
      const Regex ce, ce2;
      m_stream << ce;
      unsigned i = ce.error_code();
      ignore_unused_variable_warning(i);
      pointer_type p = ce.expression();
      ignore_unused_variable_warning(p);
      int i2 = ce.compare(ce2);
      ignore_unused_variable_warning(i2);
      bool b = ce == ce2;
      ignore_unused_variable_warning(b);
      b = ce != ce2;
      ignore_unused_variable_warning(b);
      b = ce < ce2;
      ignore_unused_variable_warning(b);
      b = ce > ce2;
      ignore_unused_variable_warning(b);
      b = ce <= ce2;
      ignore_unused_variable_warning(b);
      b = ce >= ce2;
      ignore_unused_variable_warning(b);
      i = ce.status();
      ignore_unused_variable_warning(i);
      size_type s = ce.max_size();
      ignore_unused_variable_warning(s);
      s = ce.size();
      ignore_unused_variable_warning(s);
      const_iterator pi = ce.begin();
      ignore_unused_variable_warning(pi);
      pi = ce.end();
      ignore_unused_variable_warning(pi);
      string_type s2 = ce.str();
      ignore_unused_variable_warning(s2);

      m_string = m_sub + m_sub;
      ignore_unused_variable_warning(m_string);
      m_string = m_sub + m_pointer;
      ignore_unused_variable_warning(m_string);
      m_string = m_pointer + m_sub;
      ignore_unused_variable_warning(m_string);
      m_string = m_sub + m_string;
      ignore_unused_variable_warning(m_string);
      m_string = m_string + m_sub;
      ignore_unused_variable_warning(m_string);
      m_string = m_sub + m_char;
      ignore_unused_variable_warning(m_string);
      m_string = m_char + m_sub;
      ignore_unused_variable_warning(m_string);

      m_stream << m_sub;
      m_stream << m_cresults;
   }

   std::basic_ostream<value_type> m_stream;
   sub_match_type m_sub;
   pointer_type m_pointer;
   string_type m_string;
   const value_type m_char;
   match_results_type m_results;
   const match_results_type m_cresults;

   BoostRegexConcept();
   BoostRegexConcept(const BoostRegexConcept&);
   BoostRegexConcept& operator=(const BoostRegexConcept&);
};

}

#endif
