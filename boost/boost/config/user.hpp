//  (C) Copyright Boost.org 2001.
//  Do not check in modified versions of this file,
//  This file may be customised by the end user, but not by boost.

//
//  Use this file to define a site and compiler specific
//  configuration policy:
//

// define this to locate a compiler config file:
// #define BOOST_COMPILER_CONFIG <myheader>

// define this to locate a stdlib config file:
// #define BOOST_STDLIB_CONFIG   <myheader>

// define this to locate a platform config file:
// #define BOOST_PLATFORM_CONFIG <myheader>

// define this to disable compiler config,
// use if your compiler config has nothing to set:
// #define BOOST_NO_COMPILER_CONFIG

// define this to disable stdlib config,
// use if your stdlib config has nothing to set:
// #define BOOST_NO_STDLIB_CONFIG

// define this to disable platform config,
// use if your platform config has nothing to set:
// #define BOOST_NO_PLATFORM_CONFIG

// define this to disable all config options,
// excluding the user config.  Use if your
// setup is fully ISO compliant, and has no
// useful extentions, or for autoconf generated
// setups:
// #define BOOST_NO_CONFIG

// define this to make the config "optimistic"
// about unknown compiler versions.  Normally
// unknown compiler versions are assumed to have
// all the defects of the last known version, however
// setting this flag, causes the config to assume
// that unknown compiler versions are fully conformant
// with the standard:
// #define BOOST_STRICT_CONFIC

// define this to cause the config to halt compilation
// with a #error if it encounters anything unknown -
// either an unknown compiler version or an unknown
// compiler/platform/library:
// #define BOOST_ASSERT_CONFIG


// define if you want to disable threading support, even
// when available:
// #define BOOST_DISABLE_THREADS


