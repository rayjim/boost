// Copyright Vladimir Prus 2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// This file defines template functions that are declared in
// ../value_semantic.hpp.

namespace boost { namespace program_options { 

    extern std::string arg;
    
    template<class T, class charT>
    std::string
    typed_value<T, charT>::name() const
    {
        if (!m_default_value.empty() && !m_default_value_as_text.empty()) {
            return arg + " (=" + m_default_value_as_text + ")";
        } else {
            return arg;
        }
    }

    template<class T, class charT>
    void 
    typed_value<T, charT>::notify(const boost::any& value_store) const
    {
        const T* value = boost::any_cast<const T>(&value_store);
        if (m_store_to) {
            *m_store_to = *value;
        }
        if (m_notifier) {
            m_notifier(*value);
        }
    }

    namespace validators {
        /* If v.size() > 1, throw validation_error. 
           If v.size() == 1, return v.front()
           Otherwise, returns a reference to a statically allocated
           empty string if 'allow_empty' and throws validation_error
           otherwise. */
        template<class charT>
        const std::basic_string<charT>& get_single_string(
            const std::vector<std::basic_string<charT> >& v, 
            bool allow_empty = false)
        {
            static std::basic_string<charT> empty;
            if (v.size() > 1)
                throw validation_error("multiple values not allowed");
            if (v.size() == 1)
                return v.front();
            else if (allow_empty)
                return empty;
            else
                throw validation_error("at least one value required");
        }

        /* Throws multiple_occurences if 'value' is not empty. */
        void check_first_occurence(const boost::any& value);
    }

    using namespace validators;

    /** Validates 's' and updates 'v'.
        @pre 'v' is either empty or in the state assigned by the previous
        invocation of 'validate'.
        Specializations are provided for bool, float, int, and string.
        The target type is specified via a parameter of which has type of 
        pointer to the desired type. This is workaround for compilers without
        partial template ordering, just like the last 'long/int' parameter.
    */
    template<class T, class charT>
    void validate(boost::any& v, 
                  const std::vector< std::basic_string<charT> >& xs, 
                  T* target_type, long)
    {
        validators::check_first_occurence(v);
        std::basic_string<charT> s(validators::get_single_string(xs));
        try {
            v = any(lexical_cast<T>(s));
        }
        catch(const bad_lexical_cast&) {
            throw validation_error("invalid option value");
        }
    }

    void validate(boost::any& v, 
                  const std::vector<std::string>& xs, 
                  bool*,
                  int);

    void validate(boost::any& v, 
                  const std::vector<std::wstring>& xs, 
                  bool*,
                  int);

    // For some reason, this declaration, which is require by the standard,
    // cause gcc 3.2 to not generate code to specialization defined in
    // value_semantic.cpp
#if ! ( ( BOOST_WORKAROUND(__GNUC__, <= 3) &&\
          BOOST_WORKAROUND(__GNUC_MINOR__, < 3) ) || \
        ( BOOST_WORKAROUND(BOOST_MSVC, == 1310) ) \
      ) 
    void validate(boost::any& v, 
                  const std::vector<std::string>& xs,
                  std::string*,
                  int);

    void validate(boost::any& v, 
                  const std::vector<std::wstring>& xs,
                  std::string*,
                  int);
#endif

    /** Validates sequences. Allows multiple values per option occurence
       and multiple occurences. */
    template<class T, class charT>
    void validate(boost::any& v, 
                  const std::vector<std::basic_string<charT> >& s, 
                  std::vector<T>*,
                  int)
    {
        if (v.empty()) {
            v = boost::any(std::vector<T>());
        }
        std::vector<T>* tv = boost::any_cast< std::vector<T> >(&v);
        assert(tv);
        for (unsigned i = 0; i < s.size(); ++i)
        {
            try {
                tv->push_back(boost::lexical_cast<T>(s[i]));
            }
            catch(const bad_lexical_cast& /*e*/) {
                throw validation_error(std::string("value ").append(s[i]).
                                       append(" is invalid"));
            }
        }
    }

    template<class T, class charT>
    void 
    typed_value<T, charT>::
    xparse(boost::any& value_store, 
           const std::vector<std::basic_string<charT> >& new_tokens) const
    {
        validate(value_store, new_tokens, (T*)0, 0);
    }

    template<class T>
    typed_value<T>*
    value()
    {
        return value<T>(0);
    }

    template<class T>
    typed_value<T>*
    value(T* v)
    {
        typed_value<T>* r = new typed_value<T>(v);

        return r;        
    }

    template<class T>
    typed_value<T, wchar_t>*
    wvalue()
    {
        return wvalue<T>(0);
    }

    template<class T>
    typed_value<T, wchar_t>*
    wvalue(T* v)
    {
        typed_value<T, wchar_t>* r = new typed_value<T, wchar_t>(v);

        return r;        
    }



}}
