//  Copyright (c) 2001-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_SPIRIT_LEXER_PARSE_NOV_17_2007_0246PM)
#define BOOST_SPIRIT_LEXER_PARSE_NOV_17_2007_0246PM

#include <boost/spirit/home/qi/skip_over.hpp>
#include <boost/spirit/home/qi/parse.hpp>
#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/support/unused.hpp>
#include <boost/spirit/home/lex/lexer.hpp>
#include <boost/mpl/assert.hpp>

namespace boost { namespace spirit { namespace lex
{
    ///////////////////////////////////////////////////////////////////////////
    //  Import skip_flag enumerator type from Qi namespace
    using qi::skip_flag;

    ///////////////////////////////////////////////////////////////////////////
    //
    //  The tokenize_and_parse() function is one of the main Spirit API 
    //  functions. It simplifies using a lexer as the underlying token source
    //  while parsing a given input sequence.
    //
    //  The function takes a pair of iterators spanning the underlying input 
    //  stream to parse, the lexer object (built from the token definitions) 
    //  and a parser object (built from the parser grammar definition).
    //
    //  The second version of this function additionally takes an attribute to 
    //  be used as the top level data structure instance the parser should use 
    //  to store the recognized input to.
    //
    //  The function returns true if the parsing succeeded (the given input
    //  sequence has been successfully matched by the given grammar).
    //
    //  first, last:    The pair of iterators spanning the underlying input 
    //                  sequence to parse. These iterators must at least 
    //                  conform to the requirements of the std::intput_iterator 
    //                  category.
    //                  On exit the iterator 'first' will be updated to the 
    //                  position right after the last successfully matched 
    //                  token. 
    //  lex:            The lexer object (encoding the token definitions) to be
    //                  used to convert the input sequence into a sequence of 
    //                  tokens. This token sequence is passed to the parsing 
    //                  process. The LexerExpr type must conform to the 
    //                  lexer interface described in the corresponding section
    //                  of the documentation.
    //  xpr:            The grammar object (encoding the parser grammar) to be
    //                  used to match the token sequence generated by the lex 
    //                  object instance. The ParserExpr type must conform to 
    //                  the grammar interface described in the corresponding 
    //                  section of the documentation.
    //  attr:           The top level attribute passed to the parser. It will 
    //                  be populated during the parsing of the input sequence.
    //                  On exit it will hold the 'parser result' corresponding 
    //                  to the matched input sequence.
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename Iterator, typename Lexer, typename ParserExpr>
    inline bool
    tokenize_and_parse(Iterator& first, Iterator last, Lexer const& lex,
        ParserExpr const& xpr)
    {
        // Report invalid expression error as early as possible.
        // If you got an error_invalid_expression error message here,
        // then the expression (expr) is not a valid spirit qi expression.
        BOOST_SPIRIT_ASSERT_MATCH(qi::domain, ParserExpr);

        typename Lexer::iterator_type iter = lex.begin(first, last);
        return compile<qi::domain>(xpr).parse(iter, lex.end(), unused, unused, unused);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Iterator, typename Lexer, typename ParserExpr
      , typename Attribute>
    inline bool
    tokenize_and_parse(Iterator& first, Iterator last, Lexer const& lex
      , ParserExpr const& xpr, Attribute& attr)
    {
        // Report invalid expression error as early as possible.
        // If you got an error_invalid_expression error message here,
        // then the expression (expr) is not a valid spirit qi expression.
        BOOST_SPIRIT_ASSERT_MATCH(qi::domain, ParserExpr);

        typename Lexer::iterator_type iter = lex.begin(first, last);
        return compile<qi::domain>(xpr).parse(iter, lex.end(), unused, unused, attr);
    }

    ///////////////////////////////////////////////////////////////////////////
    //
    //  The tokenize_and_phrase_parse() function is one of the main Spirit API 
    //  functions. It simplifies using a lexer as the underlying token source
    //  while phrase parsing a given input sequence.
    //
    //  The function takes a pair of iterators spanning the underlying input 
    //  stream to parse, the lexer object (built from the token definitions) 
    //  and a parser object (built from the parser grammar definition). The 
    //  additional skipper parameter will be used as the skip parser during
    //  the parsing process.
    //
    //  The second version of this function additionally takes an attribute to 
    //  be used as the top level data structure instance the parser should use 
    //  to store the recognized input to.
    //
    //  The function returns true if the parsing succeeded (the given input
    //  sequence has been successfully matched by the given grammar).
    //
    //  first, last:    The pair of iterators spanning the underlying input 
    //                  sequence to parse. These iterators must at least 
    //                  conform to the requirements of the std::intput_iterator 
    //                  category.
    //                  On exit the iterator 'first' will be updated to the 
    //                  position right after the last successfully matched 
    //                  token. 
    //  lex:            The lexer object (encoding the token definitions) to be
    //                  used to convert the input sequence into a sequence of 
    //                  tokens. This token sequence is passed to the parsing 
    //                  process. The LexerExpr type must conform to the 
    //                  lexer interface described in the corresponding section
    //                  of the documentation.
    //  xpr:            The grammar object (encoding the parser grammar) to be
    //                  used to match the token sequence generated by the lex 
    //                  object instance. The ParserExpr type must conform to 
    //                  the grammar interface described in the corresponding 
    //                  section of the documentation.
    //  skipper:        The skip parser to be used while parsing the given 
    //                  input sequence. Note, the skip parser will have to 
    //                  act on the same token sequence as the main parser 
    //                  'xpr'.
    //  post_skip:      The post_skip flag controls whether the funciton will
    //                  invoke an additional post skip after the main parser
    //                  returned.
    //  attr:           The top level attribute passed to the parser. It will 
    //                  be populated during the parsing of the input sequence.
    //                  On exit it will hold the 'parser result' corresponding 
    //                  to the matched input sequence.
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename Iterator, typename Lexer, typename ParserExpr
      , typename Skipper>
    inline bool
    tokenize_and_phrase_parse(Iterator& first, Iterator last
      , Lexer const& lex, ParserExpr const& xpr, Skipper const& skipper
      , BOOST_SCOPED_ENUM(skip_flag) post_skip = skip_flag::postskip)
    {
        // Report invalid expression error as early as possible.
        // If you got an error_invalid_expression error message here,
        // then the expression (expr) is not a valid spirit qi expression.
        BOOST_SPIRIT_ASSERT_MATCH(qi::domain, ParserExpr);
        BOOST_SPIRIT_ASSERT_MATCH(qi::domain, Skipper);

        typedef
            typename result_of::compile<qi::domain, Skipper>::type
        skipper_type;
        skipper_type const skipper_ = compile<qi::domain>(skipper);

        typename Lexer::iterator_type iter = lex.begin(first, last);
        if (!compile<qi::domain>(xpr).parse(
                iter, lex.end(), unused, skipper_, unused))
            return false;

        // do a final post-skip
        if (post_skip == skip_flag::postskip)
            qi::skip_over(iter, lex.end(), skipper_);
        return true;
    }

    template <typename Iterator, typename Lexer, typename ParserExpr
      , typename Skipper, typename Attribute>
    inline bool
    tokenize_and_phrase_parse(Iterator& first, Iterator last
      , Lexer const& lex, ParserExpr const& xpr, Skipper const& skipper
      , BOOST_SCOPED_ENUM(skip_flag) post_skip, Attribute& attr)
    {
        // Report invalid expression error as early as possible.
        // If you got an error_invalid_expression error message here,
        // then the expression (expr) is not a valid spirit qi expression.
        BOOST_SPIRIT_ASSERT_MATCH(qi::domain, ParserExpr);
        BOOST_SPIRIT_ASSERT_MATCH(qi::domain, Skipper);

        typedef
            typename result_of::compile<qi::domain, Skipper>::type
        skipper_type;
        skipper_type const skipper_ = compile<qi::domain>(skipper);

        typename Lexer::iterator_type iter = lex.begin(first, last);
        if (!compile<qi::domain>(xpr).parse(
                iter, lex.end(), unused, skipper_, attr))
            return false;

        // do a final post-skip
        if (post_skip == skip_flag::postskip)
            qi::skip_over(iter, lex.end(), skipper_);
        return true;
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Iterator, typename Lexer, typename ParserExpr
      , typename Skipper, typename Attribute>
    inline bool
    tokenize_and_phrase_parse(Iterator& first, Iterator last
      , Lexer const& lex, ParserExpr const& xpr, Skipper const& skipper
      , Attribute& attr)
    {
        return tokenize_and_phrase_parse(first, last, lex, xpr, skipper
          , skip_flag::postskip, attr);
    }

    ///////////////////////////////////////////////////////////////////////////
    //
    //  The tokenize() function is one of the main Spirit API functions. It 
    //  simplifies using a lexer to tokenize a given input sequence. It's main
    //  purpose is to use the lexer to tokenize all the input. 
    //
    //  The second version below discards all generated tokens afterwards. 
    //  This is useful whenever all the needed functionality has been 
    //  implemented directly inside the lexer semantic actions, which are being 
    //  executed while the tokens are matched. 
    //
    //  The function takes a pair of iterators spanning the underlying input 
    //  stream to scan, the lexer object (built from the token definitions),
    //  and a (optional) functor being call for each of the generated tokens. 
    //
    //  The function returns true if the scanning of the input succeeded (the 
    //  given input sequence has been successfully matched by the given token
    //  definitions).
    //
    //  first, last:    The pair of iterators spanning the underlying input 
    //                  sequence to parse. These iterators must at least 
    //                  conform to the requirements of the std::intput_iterator 
    //                  category.
    //                  On exit the iterator 'first' will be updated to the 
    //                  position right after the last successfully matched 
    //                  token. 
    //  lex:            The lexer object (encoding the token definitions) to be
    //                  used to convert the input sequence into a sequence of 
    //                  tokens. The LexerExpr type must conform to the 
    //                  lexer interface described in the corresponding section
    //                  of the documentation.
    //  f:              A functor (callable object) taking a single argument of
    //                  the token type and returning a bool, indicating whether
    //                  the tokenization should be canceled.
    //  initial_state:  The name of the state the lexer should start matching.
    //                  The default value is zero, causing the lexer to start 
    //                  in its 'INITIAL' state.
    //
    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        template <typename Token>
        bool tokenize_callback(Token const& t, void (*f)(Token const&))
        {
            f(t);
            return true;
        }

        template <typename Token, typename Eval>
        bool tokenize_callback(Token const& t, phoenix::actor<Eval> const& f)
        {
            f(t);
            return true;
        }

        template <typename Token>
        bool tokenize_callback(Token const& t, bool (*f)(Token const&))
        {
            return f(t);
        }
    }

    template <typename Iterator, typename Lexer, typename F>
    inline bool
    tokenize(Iterator& first, Iterator last, Lexer const& lex, F f
      , typename Lexer::char_type const* initial_state = 0)
    {
        typedef typename Lexer::iterator_type iterator_type;

        iterator_type iter = lex.begin(first, last, initial_state);
        iterator_type end = lex.end();
        for (/**/; iter != end; ++iter) 
        {
            if (!detail::tokenize_callback(*iter, f))
                return false;
        }
        return (iter == end) ? true : false;
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Iterator, typename Lexer>
    inline bool
    tokenize(Iterator& first, Iterator last, Lexer const& lex
      , typename Lexer::char_type const* initial_state = 0)
    {
        typedef typename Lexer::iterator_type iterator_type;

        iterator_type iter = lex.begin(first, last, initial_state);
        iterator_type end = lex.end();

        while (iter != end && token_is_valid(*iter))
            ++iter;

        return (iter == end) ? true : false;
    }

}}}

#endif
