// Copyright (c) 2008-2009 Ben Hanson
// Copyright (c) 2008-2009 Hartmut Kaiser
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// Auto-generated by boost::lexer, do not edit

#if !defined(BOOST_SPIRIT_LEXER_NEXT_TOKEN_MAY_14_2009_13_47_08)
#define BOOST_SPIRIT_LEXER_NEXT_TOKEN_MAY_14_2009_13_47_08

#include <boost/detail/iterator.hpp>
#include <boost/spirit/home/support/detail/lexer/char_traits.hpp>

// the generated table of state names and the tokenizer have to be
// defined in the boost::spirit::lex::lexertl::static_ namespace
namespace boost { namespace spirit { namespace lex { namespace lexertl { namespace static_ {

// this table defines the names of the lexer states
char const* const lexer_state_names[1] = 
{
    "INITIAL"
};

template<typename Iterator>
std::size_t next_token (std::size_t &start_state_, Iterator const& start_, 
    Iterator &start_token_, Iterator const& end_)
{
    enum {end_state_index, id_index, state_index, bol_index, eol_index,
        dead_state_index, dfa_offset};
    static const std::size_t npos = static_cast<std::size_t>(~0);
    static const std::size_t lookup_[256] = {7, 7, 7, 7, 7, 7, 7, 7,
        7, 8, 6, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        8, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7};
    static const std::size_t dfa_alphabet_ = 9;
    static const std::size_t dfa_[45] = {0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 3,
        2, 4, 1, 65536, 0, 0, 0, 0,
        0, 2, 0, 1, 10, 0, 0, 0,
        0, 0, 0, 0, 1, 65537, 0, 0,
        0, 0, 0, 0, 0};

    if (start_token_ == end_) return 0;

    const std::size_t *ptr_ = dfa_ + dfa_alphabet_;
    Iterator curr_ = start_token_;
    bool end_state_ = *ptr_ != 0;
    std::size_t id_ = *(ptr_ + id_index);
    Iterator end_token_ = start_token_;

    while (curr_ != end_)
    {
        std::size_t const state_ =
            ptr_[lookup_[static_cast<unsigned char>
                (*curr_++)]];

        if (state_ == 0) break;

        ptr_ = &dfa_[state_ * dfa_alphabet_];

        if (*ptr_)
        {
            end_state_ = true;
            id_ = *(ptr_ + id_index);
            end_token_ = curr_;
        }
    }

    if (end_state_)
    {
        // return longest match
        start_token_ = end_token_;
    }
    else
    {
        id_ = npos;
    }

    return id_;
}

}}}}}  // namespace boost::spirit::lex::lexertl::static_

#endif
