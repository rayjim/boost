// Copyright Vladimir Prus 2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_PROGRAM_OPTIONS_SOURCE
#include <boost/program_options/config.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/detail/convert.hpp>

namespace boost { namespace program_options {

    using namespace std;

    void 
    value_semantic_codecvt_helper<char>::
    parse(boost::any& value_store, 
          const std::vector<std::string>& new_tokens,
          bool utf8) const
    {
        if (utf8) {
#ifndef BOOST_NO_STD_WSTRING
            // Need to convert to local encoding.
            std::vector<string> local_tokens;
            for (unsigned i = 0; i < new_tokens.size(); ++i) {
                std::wstring w = from_utf8(new_tokens[i]);
                local_tokens.push_back(to_local_8_bit(w));
            }
            xparse(value_store, local_tokens);
#else
            throw std::runtime_error("UTF-8 conversion not supported.");
#endif
        } else {
            // Already in local encoding, pass unmodified
            xparse(value_store, new_tokens);
        }        
    }

#ifndef BOOST_NO_STD_WSTRING
    void 
    value_semantic_codecvt_helper<wchar_t>::
    parse(boost::any& value_store, 
          const std::vector<std::string>& new_tokens,
          bool utf8) const
    {
        std::vector<wstring> tokens;
        if (utf8) {
            // Convert from utf8
            for (unsigned i = 0; i < new_tokens.size(); ++i) {
                tokens.push_back(from_utf8(new_tokens[i]));
            }
               
        } else {
            // Convert from local encoding
            for (unsigned i = 0; i < new_tokens.size(); ++i) {
                tokens.push_back(from_local_8_bit(new_tokens[i]));
            }
        }      

        xparse(value_store, tokens);  
    }
#endif

    BOOST_PROGRAM_OPTIONS_DECL std::string arg("arg");

    std::string
    untyped_value::name() const
    {
        return arg;
    }

    void 
    untyped_value::xparse(boost::any& value_store,
                          const std::vector<std::string>& new_tokens) const
    {
        if (!value_store.empty()) 
            throw multiple_occurrences("multiple_occurrences");
        if (new_tokens.size() > 1)
            throw multiple_values("multiple_values");
        value_store = new_tokens.empty() ? std::string("") : new_tokens.front();
    }

    BOOST_PROGRAM_OPTIONS_DECL typed_value<bool>*
    bool_switch()
    {
        return bool_switch(0);
    }

    BOOST_PROGRAM_OPTIONS_DECL typed_value<bool>*
    bool_switch(bool* v)
    {
        typed_value<bool>* r = new typed_value<bool>(v);
        r->default_value(0);
        r->implicit();

        return r;
    }

    /* Validates bool value.
        Any of "1", "true", "yes", "on" will be converted to "1".<br>
        Any of "0", "false", "no", "off" will be converted to "0".<br>
        Case is ignored. Regardless of name passed, parameter will always
        be optional.
    */
    BOOST_PROGRAM_OPTIONS_DECL void validate(any& v, const vector<string>& xs,
                       bool*, int)
    {
        check_first_occurrence(v);
        string s(get_single_string(xs, true));

        for (size_t i = 0; i < s.size(); ++i)
            s[i] = char(tolower(s[i]));

        if (s.empty() || s == "on" || s == "yes" || s == "1" || s == "true")
            v = any(true);
        else if (s == "off" || s == "no" || s == "0" || s == "false")
            v = any(false);
        else
            throw validation_error("'" + s + "' doesn't look like a bool value.");
    }

    // This is blatant copy-paste. However, templating this will cause a problem,
    // since wstring can't be constructed/compared with char*. We'd need to
    // create auxiliary 'widen' routine to convert from char* into 
    // needed string type, and that's more work.
#if !defined(BOOST_NO_STD_WSTRING)
    BOOST_PROGRAM_OPTIONS_DECL 
    void validate(any& v, const vector<wstring>& xs, bool*, int)
    {
        check_first_occurrence(v);
        wstring s(get_single_string(xs, true));

        for (size_t i = 0; i < s.size(); ++i)
            s[i] = wchar_t(tolower(s[i]));

        if (s.empty() || s == L"on" || s == L"yes" || s == L"1" || s == L"true")
            v = any(true);
        else if (s == L"off" || s == L"no" || s == L"0" || s == L"false")
            v = any(false);
        else
            throw validation_error("invalid bool value");
    }
#endif
    BOOST_PROGRAM_OPTIONS_DECL 
    void validate(any& v, const vector<string>& xs, std::string*, int)
    {
        check_first_occurrence(v);
        string s(get_single_string(xs));
        if (*s.begin() == '\'' && *s.rbegin() == '\'' ||
            *s.begin() == '"' && *s.rbegin() == '"')
            v = any(s.substr(1, s.size()-2));
        else
            v = any(s);
    }

#if !defined(BOOST_NO_STD_WSTRING)
    BOOST_PROGRAM_OPTIONS_DECL 
    void validate(any& v, const vector<wstring>& xs, std::string*, int)
    {
        check_first_occurrence(v);
        wstring s(get_single_string(xs));
        if (*s.begin() == L'\'' && *s.rbegin() == L'\'' ||
            *s.begin() == L'"' && *s.rbegin() == L'"')
            v = any(s.substr(1, s.size()-2));
        else
            v = any(s);
    }
#endif

    namespace validators {

        BOOST_PROGRAM_OPTIONS_DECL 
        void check_first_occurrence(const boost::any& value)
        {
            if (!value.empty())
                throw multiple_occurrences("multiple_occurrences");
        }
    }




}}
