/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library
    http://www.boost.org/

    Copyright (c) 2001-2006 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define BOOST_WAVE_SOURCE 1
#include <boost/wave/wave_config.hpp>

#if BOOST_WAVE_SEPARATE_GRAMMAR_INSTANTIATION != 0

#include <string>
#include <list>

#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>

#include <boost/wave/grammars/cpp_grammar.hpp>

// this must occur after all of the includes and before any code appears
#ifdef BOOST_HAS_ABI_HEADERS
#include BOOST_ABI_PREFIX
#endif

///////////////////////////////////////////////////////////////////////////////
//  
//  Explicit instantiation of the cpp_grammar_gen template with the correct
//  token type. This instantiates the corresponding pt_parse function, which
//  in turn instantiates the cpp_grammar object 
//  (see wave/grammars/cpp_grammar.hpp)
//
///////////////////////////////////////////////////////////////////////////////

typedef boost::wave::cpplexer::lex_token<> token_type;
typedef boost::wave::cpplexer::lex_iterator<token_type> lexer_type;
typedef std::list<token_type, boost::fast_pool_allocator<token_type> > 
    token_sequence_type;
    
template struct boost::wave::grammars::cpp_grammar_gen<lexer_type, token_sequence_type>;

// the suffix header occurs after all of the code
#ifdef BOOST_HAS_ABI_HEADERS
#include BOOST_ABI_SUFFIX
#endif

#endif // #if BOOST_WAVE_SEPARATE_GRAMMAR_INSTANTIATION != 0

