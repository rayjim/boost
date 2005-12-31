//////////////////////////////////////////////////////////////////////////////
// regress.cpp
//
//  (C) Copyright Eric Niebler 2004.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/*
 Revision history:
   7 March 2004 : Initial version.
*/

#if defined(BOOST_MSVC) && defined(_DEBUG)
# include <crtdbg.h>
#endif

#include <locale>
#include <vector>
#include <fstream>
#include <boost/test/minimal.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/xpressive/xpressive.hpp>

#define BOOST_XPR_CHECK(pred)                                                   \
    if(pred) {} else { BOOST_ERROR(format_msg(#pred).c_str()); }

using namespace boost::xpressive;

//////////////////////////////////////////////////////////////////////////////
// test_case
template<typename Char>
struct test_case
{
    typedef std::basic_string<Char> string_type;
    std::string section;
    string_type str;
    string_type pat;
    string_type sub;
    string_type res;
    regex_constants::syntax_option_type syntax_flags;
    regex_constants::match_flag_type match_flags;
    std::vector<string_type> br;

    test_case()
    {
        this->reset();
    }

    void reset()
    {
        this->section.clear();
        this->str.clear();
        this->pat.clear();
        this->sub.clear();
        this->res.clear();
        this->br.clear();
        this->syntax_flags = regex_constants::ECMAScript;
        this->match_flags = regex_constants::match_default | regex_constants::format_first_only;
    }
};

//////////////////////////////////////////////////////////////////////////////
// globals
char const file[] = "regress.txt";

std::ifstream in;
unsigned int test_count = 0;

// The global object that contains the current test case
test_case<char> test;

sregex const rx_newline = as_xpr("\\n");
sregex const rx_sec = '[' >> (s1= +_) >> ']';
sregex const rx_str = "str=" >> (s1= *_);
sregex const rx_pat = "pat=" >> (s1= *_);
sregex const rx_flg = "flg=" >> (s1= *_);
sregex const rx_sub = "sub=" >> (s1= *_);
sregex const rx_res = "res=" >> (s1= *_);
sregex const rx_br = "br" >> (s1= +digit) >> '=' >> (s2= *_);

///////////////////////////////////////////////////////////////////////////////
// format_msg
std::string format_msg(char const *msg)
{
    return test.section + " /" + test.pat + "/ : " + msg;
}

#ifndef BOOST_XPRESSIVE_NO_WREGEX
///////////////////////////////////////////////////////////////////////////////
// widen
//  make a std::wstring from a std::string by widening according to the
//  current ctype<char> facet
std::wstring widen(std::string const &str)
{
    std::ctype<char> const &ct = BOOST_USE_FACET(std::ctype<char>, std::locale());
    std::wstring res;
    for(int i=0; i<str.size(); ++i)
    {
        res += ct.widen(str[i]);
    }
    return res;
}

///////////////////////////////////////////////////////////////////////////////
// widen
//  widens an entire test case
test_case<wchar_t> widen(test_case<char> const &test)
{
    test_case<wchar_t> wtest;
    wtest.section = test.section;
    wtest.str = ::widen(test.str);
    wtest.pat = ::widen(test.pat);
    wtest.sub = ::widen(test.sub);
    wtest.res = ::widen(test.res);
    wtest.syntax_flags = test.syntax_flags;
    wtest.match_flags = test.match_flags;
    wtest.br.reserve(test.br.size());
    for(std::size_t i = 0; i < test.br.size(); ++i)
    {
        wtest.br.push_back(::widen(test.br[i]));
    }
    return wtest;
}
#endif // BOOST_XPRESSIVE_NO_WREGEX

///////////////////////////////////////////////////////////////////////////////
// get_test
//   read the next section out of the input file, and fill out
//   the global variables
bool get_test()
{
    test.reset();
    bool first = true;
    std::string line;
    smatch what;

    while(in.good())
    {
        std::getline(in, line);

        if(regex_match(line, what, rx_sec))
        {
            if(!first)
            {
                if(what[1] != "end")
                {
                    BOOST_FAIL(("invalid input : " + line).c_str());
                }
                break;
            }

            first = false;
            test.section = what[1].str();
        }
        else if(regex_match(line, what, rx_str))
        {
            // replace newline escape sequence
            test.str = regex_replace(what[1].str(), rx_newline, std::string("\n"));
        }
        else if(regex_match(line, what, rx_pat))
        {
            test.pat = what[1].str();
        }
        else if(regex_match(line, what, rx_sub))
        {
            test.sub = what[1].str();
        }
        else if(regex_match(line, what, rx_res))
        {
            test.res = what[1].str();
        }
        else if(regex_match(line, what, rx_flg))
        {
            std::string flg = what[1].str();

            if(std::string::npos != flg.find('i'))
            {
                test.syntax_flags = test.syntax_flags | regex_constants::icase;
            }
            if(std::string::npos == flg.find('m'))
            {
                test.syntax_flags = test.syntax_flags | regex_constants::single_line;
            }
            if(std::string::npos == flg.find('s'))
            {
                test.syntax_flags = test.syntax_flags | regex_constants::not_dot_newline;
            }
            if(std::string::npos != flg.find('x'))
            {
                test.syntax_flags = test.syntax_flags | regex_constants::ignore_white_space;
            }
            if(std::string::npos != flg.find('g'))
            {
                test.match_flags = test.match_flags & ~regex_constants::format_first_only;
            }
        }
        else if(regex_match(line, what, rx_br))
        {
            std::size_t nbr = boost::lexical_cast<std::size_t>(what[1].str());

            if(nbr >= test.br.size())
            {
                test.br.resize(nbr + 1);
            }

            test.br[nbr] = what[2].str();
        }
        else if(!line.empty() && ';' != line[0])
        {
            BOOST_FAIL((std::string("invalid input : ") + line).c_str());
        }
    }

    return !first;
}

///////////////////////////////////////////////////////////////////////////////
// run_test_impl
//   run the test
template<typename Char>
void run_test_impl(test_case<Char> const &test)
{
    try
    {
        typedef typename std::basic_string<Char>::const_iterator iterator;
        basic_regex<iterator> rx = basic_regex<iterator>::compile(test.pat, test.syntax_flags);

        if(!test.res.empty())
        {
            // test regex_replace
            std::basic_string<Char> res = regex_replace(test.str, rx, test.sub, test.match_flags);
            BOOST_XPR_CHECK(res == test.res);
        }

        if(0 == (test.match_flags & regex_constants::format_first_only))
        {
            // global search, use regex_iterator
            std::vector<sub_match<iterator> > br;
            regex_iterator<iterator> begin(test.str.begin(), test.str.end(), rx, test.match_flags), end;
            for(; begin != end; ++begin)
            {
                match_results<iterator> const &what = *begin;
                br.insert(br.end(), what.begin(), what.end());
            }

            // match succeeded: was it expected to succeed?
            BOOST_XPR_CHECK(br.size() == test.br.size());

            for(std::size_t i = 0; i < br.size() && i < test.br.size(); ++i)
            {
                BOOST_XPR_CHECK(test.br[i] == br[i].str());
            }
        }
        else
        {
            // test regex_search
            match_results<iterator> what;
            if(regex_search(test.str, what, rx, test.match_flags))
            {
                // match succeeded: was it expected to succeed?
                BOOST_XPR_CHECK(what.size() == test.br.size());

                for(std::size_t i = 0; i < what.size() && i < test.br.size(); ++i)
                {
                    BOOST_XPR_CHECK(test.br[i] == what[i].str());
                }
            }
            else
            {
                // match failed: was it expected to fail?
                BOOST_XPR_CHECK(0 == test.br.size());
            }
        }
    }
    catch(regex_error const &e)
    {
        BOOST_ERROR(format_msg(e.what()).c_str());
    }
}

///////////////////////////////////////////////////////////////////////////////
// run_test_impl
//   run the current test
void run_test_a()
{
    run_test_impl(test);
}

///////////////////////////////////////////////////////////////////////////////
// run_test_u
//   widen the current test and run it
void run_test_u()
{
    #ifndef BOOST_XPRESSIVE_NO_WREGEX
    test_case<wchar_t> wtest = ::widen(test);
    run_test_impl(wtest);
    #endif
}

void run_test()
{
    run_test_a();
    run_test_u();
}

///////////////////////////////////////////////////////////////////////////////
// test_main
//   read the tests from the input file and execute them
int test_main(int, char*[])
{
    in.open(file);

    if(!in.good())
    {
        std::cout << "Error unable to open file: " << file << std::endl << std::endl;
        return -1;
    }

    while(get_test())
    {
        run_test();
        ++test_count;
    }

    std::cout << test_count << " tests completed in file " << file << std::endl;

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// debug_init
static const struct debug_init
{
    debug_init()
    {
    #if defined(BOOST_MSVC) && defined(_DEBUG)
        // Send warnings, errors and asserts to STDERR
        _CrtSetReportMode(_CRT_WARN,   _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
        _CrtSetReportFile(_CRT_WARN,   _CRTDBG_FILE_STDERR);
        _CrtSetReportMode(_CRT_ERROR,  _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
        _CrtSetReportFile(_CRT_ERROR,  _CRTDBG_FILE_STDERR);
        _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
        _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);

        // Check for leaks at program termination
        _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));

        //_CrtSetBreakAlloc(221);
    #endif
    }
} g_debug_init;
