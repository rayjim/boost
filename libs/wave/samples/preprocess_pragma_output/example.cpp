/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library
    Example demonstrating how to preprocess the token stream generated by a
    #pragma directive
    
    http://www.boost.org/

    Copyright (c) 2001-2008 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

///////////////////////////////////////////////////////////////////////////////
//  This special pragma is implemented by the interpret_pragma hook function
//  provided in the preprocess_pragma_output_hooks policy class. This 
//  #pragma preprocesses the provided arguments in the current context.
#pragma wave pp (                                                             \
        "#define A() \"some text\" and more\n"                                \
        "#define B() 1.0\n"                                                   \
    )                                                                         \
    /**/
    
A()   // this should produce: "some text" and more
B()   // and this expands to 1.0
