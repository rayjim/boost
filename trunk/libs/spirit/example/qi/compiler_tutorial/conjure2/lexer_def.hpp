/*=============================================================================
    Copyright (c) 2001-2011 Hartmut Kaiser

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "lexer.hpp"

namespace client { namespace lexer
{
    template <typename BaseIterator>
    conjure_tokens<BaseIterator>::conjure_tokens()
      : identifier("[a-zA-Z_][a-zA-Z_0-9]*", ID_IDENTIFIER)
      , uint_("[0-9]+", ID_UINT)
      , bool_("true|false", ID_BOOL)
    {
        lex::_pass_type _pass;

        this->self = uint_ | bool_;

        add_keyword("void");
        add_keyword("int");
        add_keyword("if");
        add_keyword("else");
        add_keyword("while");
        add_keyword("return");

        this->self.add 
                ("\\|\\|", ID_OP_LOGICAL_OR)
                ("&&", ID_OP_LOGICAL_AND)
                ("==", ID_OP_EQUAL)
                ("!=", ID_OP_NOT_EQUAL)
                ("<", ID_OP_LESS)
                ("<=", ID_OP_LESS_EQUAL)
                (">", ID_OP_GREATER)
                (">=", ID_OP_GREATER_EQUAL)
                ("\\+", ID_OP_PLUS)
                ("\\-", ID_OP_MINUS)
                ("\\*", ID_OP_TIMES)
                ("\\/", ID_OP_DIVIDE)
                ("!", ID_OP_NOT)
            ;

        this->self += lex::char_('(') | ')' | '{' | '}' | ',' | '=' | ';';

        this->self += 
                identifier
            |   lex::string("\\/\\*[^*]*\\*+([^/*][^*]*\\*+)*\\/", ID_COMMENT) 
                [
                    lex::_pass = lex::pass_flags::pass_ignore
                ] 
            |   lex::string("[ \t\n\r]+", ID_WHITESPACE) 
                [
                    lex::_pass = lex::pass_flags::pass_ignore
                ]
            ;
    }

    template <typename BaseIterator>
    bool conjure_tokens<BaseIterator>::add_keyword(std::string const& keyword)
    {
        // add the token to the lexer
        tokenids id = tokenids(this->get_next_id());
        this->self.add(keyword, id);

        // store the mapping for later retrieval 
        std::pair<typename keyword_map_type::iterator, bool> p = 
            keywords_.insert(typename keyword_map_type::value_type(keyword, id));

        return p.second;
    }
}}

