/*=============================================================================
    A Standard compliant C++ preprocessor

    Copyright (c) 2001-2003 Hartmut Kaiser
    Copyright (c) 2003 Paul Mensonides
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.

    See Copyright.txt for full copyright notices and acknowledgements.
=============================================================================*/

// Tests macro expansion order together with the need to skip expansion of
// the same macro as it is currently expanded.

#define CONCAT(a, b) a ## b
#define CONCAT_INDIRECT() CONCAT

CONCAT(CON, CAT)(1, 2)              // should expand to CONCAT(1, 2)
CONCAT(CON, CAT(1, 2))              // should expand to CONCAT(1, 2)
CONCAT(1, CONCAT(2, 3))             // should expand to 1CONCAT(2, 3)
CONCAT(1, CONCAT_INDIRECT()(2, 3))  // should expand to 1CONCAT_INDIRECT()(2, 3)
CONCAT(CONCAT_, INDIRECT)()(1, 2)   // should expand to 12
