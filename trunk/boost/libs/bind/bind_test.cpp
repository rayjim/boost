#if defined(_MSC_VER) && !defined(__ICL)
#pragma warning(disable: 4786)  // identifier truncated in debug info
#pragma warning(disable: 4710)  // function not inlined
#pragma warning(disable: 4711)  // function selected for automatic inline expansion
#pragma warning(disable: 4514)  // unreferenced inline removed
#endif

//
//  bind_test.cpp - monolithic test for bind.hpp
//
//  Version 1.00.0002 (2001-09-02)
//
//  Copyright (c) 2001 Peter Dimov and Multi Media Ltd.
//  Copyright (c) 2001 David Abrahams
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <iostream>

#define BOOST_INCLUDE_MAIN
#include <boost/test/test_tools.hpp>

//

long f_0()
{
    return 17041L;
}

long f_1(long a)
{
    return a;
}

long f_2(long a, long b)
{
    return a + 10 * b;
}

long f_3(long a, long b, long c)
{
    return a + 10 * b + 100 * c;
}

long f_4(long a, long b, long c, long d)
{
    return a + 10 * b + 100 * c + 1000 * d;
}

long f_5(long a, long b, long c, long d, long e)
{
    return a + 10 * b + 100 * c + 1000 * d + 10000 * e;
}

long f_6(long a, long b, long c, long d, long e, long f)
{
    return a + 10 * b + 100 * c + 1000 * d + 10000 * e + 100000 * f;
}

long f_7(long a, long b, long c, long d, long e, long f, long g)
{
    return a + 10 * b + 100 * c + 1000 * d + 10000 * e + 100000 * f + 1000000 * g;
}

long f_8(long a, long b, long c, long d, long e, long f, long g, long h)
{
    return a + 10 * b + 100 * c + 1000 * d + 10000 * e + 100000 * f + 1000000 * g + 10000000 * h;
}

long f_9(long a, long b, long c, long d, long e, long f, long g, long h, long i)
{
    return a + 10 * b + 100 * c + 1000 * d + 10000 * e + 100000 * f + 1000000 * g + 10000000 * h + 100000000 * i;
}

long global_result;

void fv_0()
{
    global_result = 17041L;
}

void fv_1(long a)
{
    global_result = a;
}

void fv_2(long a, long b)
{
    global_result = a + 10 * b;
}

void fv_3(long a, long b, long c)
{
    global_result = a + 10 * b + 100 * c;
}

void fv_4(long a, long b, long c, long d)
{
    global_result = a + 10 * b + 100 * c + 1000 * d;
}

void fv_5(long a, long b, long c, long d, long e)
{
    global_result = a + 10 * b + 100 * c + 1000 * d + 10000 * e;
}

void fv_6(long a, long b, long c, long d, long e, long f)
{
    global_result = a + 10 * b + 100 * c + 1000 * d + 10000 * e + 100000 * f;
}

void fv_7(long a, long b, long c, long d, long e, long f, long g)
{
    global_result = a + 10 * b + 100 * c + 1000 * d + 10000 * e + 100000 * f + 1000000 * g;
}

void fv_8(long a, long b, long c, long d, long e, long f, long g, long h)
{
    global_result = a + 10 * b + 100 * c + 1000 * d + 10000 * e + 100000 * f + 1000000 * g + 10000000 * h;
}

void fv_9(long a, long b, long c, long d, long e, long f, long g, long h, long i)
{
    global_result = a + 10 * b + 100 * c + 1000 * d + 10000 * e + 100000 * f + 1000000 * g + 10000000 * h + 100000000 * i;
}

void function_test()
{
    using namespace boost;

    int const i = 1;

    BOOST_TEST( bind(f_0)(i) == 17041L );
    BOOST_TEST( bind(f_1, _1)(i) == 1L );
    BOOST_TEST( bind(f_2, _1, 2)(i) == 21L );
    BOOST_TEST( bind(f_3, _1, 2, 3)(i) == 321L );
    BOOST_TEST( bind(f_4, _1, 2, 3, 4)(i) == 4321L );
    BOOST_TEST( bind(f_5, _1, 2, 3, 4, 5)(i) == 54321L );
    BOOST_TEST( bind(f_6, _1, 2, 3, 4, 5, 6)(i) == 654321L );
    BOOST_TEST( bind(f_7, _1, 2, 3, 4, 5, 6, 7)(i) == 7654321L );
    BOOST_TEST( bind(f_8, _1, 2, 3, 4, 5, 6, 7, 8)(i) == 87654321L );
    BOOST_TEST( bind(f_9, _1, 2, 3, 4, 5, 6, 7, 8, 9)(i) == 987654321L );

    BOOST_TEST( (bind(fv_0)(i), (global_result == 17041L)) );
    BOOST_TEST( (bind(fv_1, _1)(i), (global_result == 1L)) );
    BOOST_TEST( (bind(fv_2, _1, 2)(i), (global_result == 21L)) );
    BOOST_TEST( (bind(fv_3, _1, 2, 3)(i), (global_result == 321L)) );
    BOOST_TEST( (bind(fv_4, _1, 2, 3, 4)(i), (global_result == 4321L)) );
    BOOST_TEST( (bind(fv_5, _1, 2, 3, 4, 5)(i), (global_result == 54321L)) );
    BOOST_TEST( (bind(fv_6, _1, 2, 3, 4, 5, 6)(i), (global_result == 654321L)) );
    BOOST_TEST( (bind(fv_7, _1, 2, 3, 4, 5, 6, 7)(i), (global_result == 7654321L)) );
    BOOST_TEST( (bind(fv_8, _1, 2, 3, 4, 5, 6, 7, 8)(i), (global_result == 87654321L)) );
    BOOST_TEST( (bind(fv_9, _1, 2, 3, 4, 5, 6, 7, 8, 9)(i), (global_result == 987654321L)) );
}

//

struct Y
{
    short operator()(short & r) const { return ++r; }
    int operator()(int a, int b) const { return a + 10 * b; }
    long operator() (long a, long b, long c) const { return a + 10 * b + 100 * c; }
    void operator() (long a, long b, long c, const char*) const { global_result = a + 10 * b + 100 * c; }
};

void function_object_test()
{
    using namespace boost;

    short i(6);

    int const k = 3;

    BOOST_TEST( bind<short>(Y(), ref(i))() == 7 );
    BOOST_TEST( bind<short>(Y(), ref(i))() == 8 );
    BOOST_TEST( bind<int>(Y(), i, _1)(k) == 38 );
    BOOST_TEST( bind<long>(Y(), i, _1, 9)(k) == 938 );
#if !defined(__MWERKS__) || __MWERKS__ > 0x2406     // Fails for this version of the compiler.
    BOOST_TEST( (bind<void>(Y(), i, _1, 9, "nothing")(k), (global_result == 938)) );
#endif
}

//

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) && !defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING)

struct Z
{
    typedef int result_type;
    int operator()(int a, int b) const { return a + 10 * b; }
};

void adaptable_function_object_test()
{
    BOOST_TEST( boost::bind(Z(), 7, 4)() == 47 );
}

#endif

//

struct X
{
    mutable unsigned int hash;

    X(): hash(0) {}

    int f0() { f1(17); return 0; }
    int g0() const { g1(17); return 0; }

    int f1(int a1) { hash = (hash * 17041 + a1) % 32768; return 0; }
    int g1(int a1) const { hash = (hash * 17041 + a1 * 2) % 32768; return 0; }

    int f2(int a1, int a2) { f1(a1); f1(a2); return 0; }
    int g2(int a1, int a2) const { g1(a1); g1(a2); return 0; }

    int f3(int a1, int a2, int a3) { f2(a1, a2); f1(a3); return 0; }
    int g3(int a1, int a2, int a3) const { g2(a1, a2); g1(a3); return 0; }

    int f4(int a1, int a2, int a3, int a4) { f3(a1, a2, a3); f1(a4); return 0; }
    int g4(int a1, int a2, int a3, int a4) const { g3(a1, a2, a3); g1(a4); return 0; }

    int f5(int a1, int a2, int a3, int a4, int a5) { f4(a1, a2, a3, a4); f1(a5); return 0; }
    int g5(int a1, int a2, int a3, int a4, int a5) const { g4(a1, a2, a3, a4); g1(a5); return 0; }

    int f6(int a1, int a2, int a3, int a4, int a5, int a6) { f5(a1, a2, a3, a4, a5); f1(a6); return 0; }
    int g6(int a1, int a2, int a3, int a4, int a5, int a6) const { g5(a1, a2, a3, a4, a5); g1(a6); return 0; }

    int f7(int a1, int a2, int a3, int a4, int a5, int a6, int a7) { f6(a1, a2, a3, a4, a5, a6); f1(a7); return 0; }
    int g7(int a1, int a2, int a3, int a4, int a5, int a6, int a7) const { g6(a1, a2, a3, a4, a5, a6); g1(a7); return 0; }

    int f8(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8) { f7(a1, a2, a3, a4, a5, a6, a7); f1(a8); return 0; }
    int g8(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8) const { g7(a1, a2, a3, a4, a5, a6, a7); g1(a8); return 0; }
};

struct V
{
    mutable unsigned int hash;

    V(): hash(0) {}

    void f0() { f1(17); }
    void g0() const { g1(17); }

    void f1(int a1) { hash = (hash * 17041 + a1) % 32768; }
    void g1(int a1) const { hash = (hash * 17041 + a1 * 2) % 32768; }

    void f2(int a1, int a2) { f1(a1); f1(a2); }
    void g2(int a1, int a2) const { g1(a1); g1(a2); }

    void f3(int a1, int a2, int a3) { f2(a1, a2); f1(a3); }
    void g3(int a1, int a2, int a3) const { g2(a1, a2); g1(a3); }

    void f4(int a1, int a2, int a3, int a4) { f3(a1, a2, a3); f1(a4); }
    void g4(int a1, int a2, int a3, int a4) const { g3(a1, a2, a3); g1(a4); }

    void f5(int a1, int a2, int a3, int a4, int a5) { f4(a1, a2, a3, a4); f1(a5); }
    void g5(int a1, int a2, int a3, int a4, int a5) const { g4(a1, a2, a3, a4); g1(a5); }

    void f6(int a1, int a2, int a3, int a4, int a5, int a6) { f5(a1, a2, a3, a4, a5); f1(a6); }
    void g6(int a1, int a2, int a3, int a4, int a5, int a6) const { g5(a1, a2, a3, a4, a5); g1(a6); }

    void f7(int a1, int a2, int a3, int a4, int a5, int a6, int a7) { f6(a1, a2, a3, a4, a5, a6); f1(a7); }
    void g7(int a1, int a2, int a3, int a4, int a5, int a6, int a7) const { g6(a1, a2, a3, a4, a5, a6); g1(a7); }

    void f8(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8) { f7(a1, a2, a3, a4, a5, a6, a7); f1(a8); }
    void g8(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8) const { g7(a1, a2, a3, a4, a5, a6, a7); g1(a8); }
};

template <class T>
void member_function_test(T x)
{
    using namespace boost;

    // 0

    bind(&T::f0, &x)();
    bind(&T::f0, ref(x))();

    bind(&T::g0, &x)();
    bind(&T::g0, x)();
    bind(&T::g0, ref(x))();

    // 1

    bind(&T::f1, &x, 1)();
    bind(&T::f1, ref(x), 1)();

    bind(&T::g1, &x, 1)();
    bind(&T::g1, x, 1)();
    bind(&T::g1, ref(x), 1)();

    // 2

    bind(&T::f2, &x, 1, 2)();
    bind(&T::f2, ref(x), 1, 2)();

    bind(&T::g2, &x, 1, 2)();
    bind(&T::g2, x, 1, 2)();
    bind(&T::g2, ref(x), 1, 2)();

    // 3

    bind(&T::f3, &x, 1, 2, 3)();
    bind(&T::f3, ref(x), 1, 2, 3)();

    bind(&T::g3, &x, 1, 2, 3)();
    bind(&T::g3, x, 1, 2, 3)();
    bind(&T::g3, ref(x), 1, 2, 3)();

    // 4

    bind(&T::f4, &x, 1, 2, 3, 4)();
    bind(&T::f4, ref(x), 1, 2, 3, 4)();

    bind(&T::g4, &x, 1, 2, 3, 4)();
    bind(&T::g4, x, 1, 2, 3, 4)();
    bind(&T::g4, ref(x), 1, 2, 3, 4)();

    // 5

    bind(&T::f5, &x, 1, 2, 3, 4, 5)();
    bind(&T::f5, ref(x), 1, 2, 3, 4, 5)();

    bind(&T::g5, &x, 1, 2, 3, 4, 5)();
    bind(&T::g5, x, 1, 2, 3, 4, 5)();
    bind(&T::g5, ref(x), 1, 2, 3, 4, 5)();

    // 6

    bind(&T::f6, &x, 1, 2, 3, 4, 5, 6)();
    bind(&T::f6, ref(x), 1, 2, 3, 4, 5, 6)();

    bind(&T::g6, &x, 1, 2, 3, 4, 5, 6)();
    bind(&T::g6, x, 1, 2, 3, 4, 5, 6)();
    bind(&T::g6, ref(x), 1, 2, 3, 4, 5, 6)();

    // 7

    bind(&T::f7, &x, 1, 2, 3, 4, 5, 6, 7)();
    bind(&T::f7, ref(x), 1, 2, 3, 4, 5, 6, 7)();

    bind(&T::g7, &x, 1, 2, 3, 4, 5, 6, 7)();
    bind(&T::g7, x, 1, 2, 3, 4, 5, 6, 7)();
    bind(&T::g7, ref(x), 1, 2, 3, 4, 5, 6, 7)();

    // 8

    bind(&T::f8, &x, 1, 2, 3, 4, 5, 6, 7, 8)();
    bind(&T::f8, ref(x), 1, 2, 3, 4, 5, 6, 7, 8)();

    bind(&T::g8, &x, 1, 2, 3, 4, 5, 6, 7, 8)();
    bind(&T::g8, x, 1, 2, 3, 4, 5, 6, 7, 8)();
    bind(&T::g8, ref(x), 1, 2, 3, 4, 5, 6, 7, 8)();

    BOOST_TEST( x.hash == 23558 );
}

void nested_bind_test()
{
    using namespace boost;

    int const x = 1;
    int const y = 2;

    BOOST_TEST( bind(f_1, bind(f_1, _1))(x) == 1L );
    BOOST_TEST( bind(f_1, bind(f_2, _1, _2))(x, y) == 21L );
    BOOST_TEST( bind(f_2, bind(f_1, _1), bind(f_1, _1))(x) == 11L );
    BOOST_TEST( bind(f_2, bind(f_1, _1), bind(f_1, _2))(x, y) == 21L );
    BOOST_TEST( bind(f_1, bind(f_0))() == 17041L );

    BOOST_TEST( (bind(fv_1, bind(f_1, _1))(x), (global_result == 1L)) );
    BOOST_TEST( (bind(fv_1, bind(f_2, _1, _2))(x, y), (global_result == 21L)) );
    BOOST_TEST( (bind(fv_2, bind(f_1, _1), bind(f_1, _1))(x), (global_result == 11L)) );
    BOOST_TEST( (bind(fv_2, bind(f_1, _1), bind(f_1, _2))(x, y), (global_result == 21L)) );
    BOOST_TEST( (bind(fv_1, bind(f_0))(), (global_result == 17041L)) );
}

int test_main(int, char * [])
{
    function_test();
    function_object_test();
#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) && !defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING)
    adaptable_function_object_test();
#endif
    member_function_test(X());
    member_function_test(V());
    nested_bind_test();

    return 0;
}
