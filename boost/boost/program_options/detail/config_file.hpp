// Copyright Vladimir Prus 2002-2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)


#ifndef CONFIG_FILE_VP_2003_01_02
#define CONFIG_FILE_VP_2003_01_02

#include <iosfwd>
#include <string>
#include <set>

#include <boost/noncopyable.hpp>
#include <boost/program_options/config.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/eof_iterator.hpp>

#if BOOST_WORKAROUND(__MWERKS__, BOOST_TESTED_AT(0x3202))
#include <boost/program_options/detail/convert.hpp>
#endif

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/detail/workaround.hpp>

namespace boost { namespace program_options { namespace detail {

    /** Standalone parser for config files in ini-line format.
        The parser is a model of single-pass lvalue iterator, and
        default constructor creates past-the-end-iterator. The typical usage is:
        config_file_iterator i(is, ... set of options ...), e;
        for(; i !=e; ++i) {
            *i;
        }
        
        Syntax conventions:

        - config file can not contain positional options
        - '#' is comment character: it is ignored together with
          the rest of the line.
        - variable assignments are in the form
          name '=' value.
          spaces around '=' are trimmed.
        - Section names are given in brackets. 

         The actuall option name is constructed by combining current section
         name and specified option name, with dot between. If section_name 
         already contains dot at the end, new dot is not inserted. For example:
         @verbatim
         [gui.accessibility]
         visual_bell=yes
         @endverbatim
         will result in option "gui.accessibility.visual_bell" with value
         "yes" been returned.

         TODO: maybe, we should just accept a pointer to options_description
         class.
     */    
    class common_config_file_iterator 
        : public eof_iterator<common_config_file_iterator, option>
    {
    public:
        common_config_file_iterator() { found_eof(); }
        common_config_file_iterator(
            const std::set<std::string>& allowed_options);

        virtual ~common_config_file_iterator() {}

    public: // Method required by eof_iterator
        
        void get();
        
    protected: // Stubs for derived classes

        // Obtains next line from the config file
        // Note: really, this design is a bit ugly
        // The most clean thing would be to pass 'line_iterator' to
        // constructor of this class, but to avoid templating this class
        // we'd need polymorphic iterator, which does not exist yet.
        virtual bool getline(std::string&) { return false; }
        
    private:
        /** Adds another allowed option. If the 'name' ends with
            '*', then all options with the same prefix are
            allowed. For example, if 'name' is 'foo*', then 'foo1' and
            'foo_bar' are allowed. */
        void add_option(const char* name);

        // Returns true if 's' is a registered option name.
        bool allowed_option(const std::string& s) const; 

        // That's probably too much data for iterator, since
        // it will be copied, but let's not bother for now.
        std::set<std::string> allowed_options;
        // Invariant: no element is prefix of other element.
        std::set<std::string> allowed_prefixes;
        std::string m_prefix;
    };

    template<class charT>
    class basic_config_file_iterator : public common_config_file_iterator {
    public:
        basic_config_file_iterator()
        {
            found_eof();
        }

        /** Creates a config file parser for the specified stream.            
        */
        basic_config_file_iterator(std::basic_istream<charT>& is, 
                                   const std::set<std::string>& allowed_options); 

    private: // base overrides

        bool getline(std::string&);

    private: // internal data
        shared_ptr<std::basic_istream<charT> > is;
    };
    
    typedef basic_config_file_iterator<char> config_file_iterator;
    typedef basic_config_file_iterator<wchar_t> wconfig_file_iterator;


    struct null_deleter
    {
        void operator()(void const *) const {}
    };


    template<class charT>
    basic_config_file_iterator<charT>::
    basic_config_file_iterator(std::basic_istream<charT>& is, 
                               const std::set<std::string>& allowed_options)
    : common_config_file_iterator(allowed_options)
    {
        this->is.reset(&is, null_deleter());                 
        get();
    }

    template<>
    bool
    basic_config_file_iterator<wchar_t>::getline(std::string& s)
#if BOOST_WORKAROUND(__MWERKS__, BOOST_TESTED_AT(0x3202))
    {
        // When the function is out-of-line, Metrowerks tries to use
        // the unspecialized version below, which causes an error.
        std::wstring ws;
        if (std::getline(*is, ws)) {
            s = to_utf8(ws);
            return true;
        } else {
            return false;
        }                    
    }
#else
;
#endif


    template<class charT>
    bool
    basic_config_file_iterator<charT>::getline(std::string& s)
    {
        return std::getline(*is, s);
    }

    

}}}

#endif
