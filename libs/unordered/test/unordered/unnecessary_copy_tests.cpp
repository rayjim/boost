
// Copyright 2006-2009 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "../helpers/prefix.hpp"

#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include "../helpers/test.hpp"

namespace unnecessary_copy_tests
{
    struct count_copies
    {
    private:
        BOOST_COPYABLE_AND_MOVABLE(count_copies)
    public:
        static int copies;
        static int moves;
        count_copies() : tag_(0) { ++copies; }
        explicit count_copies(int tag) : tag_(tag) { ++copies; }

        // This bizarre constructor is an attempt to confuse emplace.
        //
        // unordered_map<count_copies, count_copies> x:
        // x.emplace(count_copies(1), count_copies(2));
        // x.emplace(count_copies(1), count_copies(2), count_copies(3));
        //
        // The first emplace should use the single argument constructor twice.
        // The second emplace should use the single argument contructor for
        // the key, and this constructor for the value.
        count_copies(count_copies const&, count_copies const& x)
            : tag_(x.tag_) { ++copies; }

        count_copies(count_copies const& x) : tag_(x.tag_) { ++copies; }
        count_copies(BOOST_RV_REF(count_copies) x) : tag_(x.tag_) {
            x.tag_ = -1; ++moves;
        }

        count_copies& operator=(BOOST_COPY_ASSIGN_REF(count_copies) p) // Copy assignment
        {
            tag_ = p.tag_;
            ++copies;
            return *this;
        }

        count_copies& operator=(BOOST_RV_REF(count_copies) p) //Move assignment
        {
            tag_ = p.tag_;
            ++moves;
            return *this;
        }

        int tag_;
    };

    bool operator==(count_copies const& x, count_copies const& y) {
        return x.tag_ == y.tag_;
    }

    template <class T>
    T source() {
        return T();
    }

    void reset() {
        count_copies::copies = 0;
        count_copies::moves = 0;
    }
}

#if defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)
namespace boost
#else
namespace unnecessary_copy_tests
#endif
{
    std::size_t hash_value(unnecessary_copy_tests::count_copies const& x) {
        return x.tag_;
    }
}

#define COPY_COUNT(n)                                                       \
    if(::unnecessary_copy_tests::count_copies::copies != n) {               \
        BOOST_ERROR("Wrong number of copies.");                             \
        std::cerr                                                           \
            << "Number of copies: "                                         \
            << ::unnecessary_copy_tests::count_copies::copies               \
            << " expecting: " << n << std::endl;                            \
    }
#define MOVE_COUNT(n)                                                       \
    if(::unnecessary_copy_tests::count_copies::moves != n) {                \
        BOOST_ERROR("Wrong number of moves.");                              \
        std::cerr                                                           \
            << "Number of moves: "                                          \
            << ::unnecessary_copy_tests::count_copies::moves                \
            << " expecting: " <<n << std::endl;                             \
    }
#define COPY_COUNT_RANGE(a, b)                                              \
    if(::unnecessary_copy_tests::count_copies::copies < a ||                \
            ::unnecessary_copy_tests::count_copies::copies > b) {           \
        BOOST_ERROR("Wrong number of copies.");                             \
        std::cerr                                                           \
            << "Number of copies: "                                         \
            << ::unnecessary_copy_tests::count_copies::copies               \
            << " expecting: [" << a << ", " << b << "]" << std::endl;       \
    }
#define MOVE_COUNT_RANGE(a, b)                                              \
    if(::unnecessary_copy_tests::count_copies::moves < a ||                 \
            ::unnecessary_copy_tests::count_copies::moves > b) {            \
        BOOST_ERROR("Wrong number of moves.");                              \
        std::cerr                                                           \
            << "Number of moves: "                                          \
            << ::unnecessary_copy_tests::count_copies::copies               \
            << " expecting: [" << a << ", " << b << "]" << std::endl;       \
    }

namespace unnecessary_copy_tests
{
    int count_copies::copies;
    int count_copies::moves;

    template <class T>
    void unnecessary_copy_insert_test(T*)
    {
        T x;
        BOOST_DEDUCED_TYPENAME T::value_type a;
        reset();
        x.insert(a);
        COPY_COUNT(1);
    }

    boost::unordered_set<count_copies>* set;
    boost::unordered_multiset<count_copies>* multiset;
    boost::unordered_map<int, count_copies>* map;
    boost::unordered_multimap<int, count_copies>* multimap;

    UNORDERED_TEST(unnecessary_copy_insert_test,
            ((set)(multiset)(map)(multimap)))

    template <class T>
    void unnecessary_copy_emplace_test(T*)
    {
        reset();
        T x;
        BOOST_DEDUCED_TYPENAME T::value_type a;
        COPY_COUNT(1);
        x.emplace(a);
        COPY_COUNT(2);
    }

    template <class T>
    void unnecessary_copy_emplace_rvalue_test(T*)
    {
        reset();
        T x;
        x.emplace(source<BOOST_DEDUCED_TYPENAME T::value_type>());
#if !defined(BOOST_NO_RVALUE_REFERENCES)
        COPY_COUNT(1);
#else
        COPY_COUNT(2);
#endif
    }

    UNORDERED_TEST(unnecessary_copy_emplace_test,
            ((set)(multiset)(map)(multimap)))
    UNORDERED_TEST(unnecessary_copy_emplace_rvalue_test,
            ((set)(multiset)(map)(multimap)))

#if defined(BOOST_UNORDERED_STD_FORWARD_MOVE)
    template <class T>
    void unnecessary_copy_emplace_move_test(T*)
    {
        reset();
        T x;
        BOOST_DEDUCED_TYPENAME T::value_type a;
        COPY_COUNT(1); MOVE_COUNT(0);
        x.emplace(std::move(a));
        COPY_COUNT(1); MOVE_COUNT(1);
    }

    UNORDERED_TEST(unnecessary_copy_emplace_move_test,
            ((set)(multiset)(map)(multimap)))
#endif

    template <class T>
    void unnecessary_copy_emplace_boost_move_set_test(T*)
    {
        reset();
        T x;
        BOOST_DEDUCED_TYPENAME T::value_type a;
        COPY_COUNT(1); MOVE_COUNT(0);
        x.emplace(boost::move(a));
        COPY_COUNT(1); MOVE_COUNT(1);
    }

    UNORDERED_TEST(unnecessary_copy_emplace_boost_move_set_test,
            ((set)(multiset)))

    template <class T>
    void unnecessary_copy_emplace_boost_move_map_test(T*)
    {
        reset();
        T x;
        BOOST_DEDUCED_TYPENAME T::value_type a;
        COPY_COUNT(1); MOVE_COUNT(0);
        x.emplace(boost::move(a));
#if defined(BOOST_NO_RVALUE_REFERENCES)
        COPY_COUNT(2); MOVE_COUNT(0);
#else
        COPY_COUNT(1); MOVE_COUNT(1);
#endif
    }

    UNORDERED_TEST(unnecessary_copy_emplace_boost_move_map_test,
            ((map)(multimap)))

    UNORDERED_AUTO_TEST(unnecessary_copy_emplace_set_test)
    {
        // When calling 'source' the object is moved on some compilers, but not
        // others. So count that here to adjust later.

        reset();
        source<count_copies>();
        int source_cost = ::unnecessary_copy_tests::count_copies::moves;

        //

        reset();
        boost::unordered_set<count_copies> x;
        count_copies a;
        x.insert(a);
        COPY_COUNT(2); MOVE_COUNT(0);

        //
        // 0 arguments
        // 

        // The container will have to create a copy in order to compare with
        // the existing element.
        reset();
        x.emplace();
#if defined(BOOST_UNORDERED_STD_FORWARD_MOVE)
        COPY_COUNT(1); MOVE_COUNT(0);
#else
        // source_cost doesn't make much sense here, but it seems to fit.
        COPY_COUNT(1); MOVE_COUNT(source_cost);
#endif

        //
        // 1 argument
        // 

        // Emplace should be able to tell that there already is an element
        // without creating a new one.
        reset();
        x.emplace(a);
        COPY_COUNT(0); MOVE_COUNT(0);

        // A new object is created by source, but it shouldn't be moved or
        // copied.
        reset();
        x.emplace(source<count_copies>());
        COPY_COUNT(1); MOVE_COUNT(source_cost);

#if defined(BOOST_UNORDERED_STD_FORWARD_MOVE)
        // No move should take place.
        reset();
        x.emplace(std::move(a));
        COPY_COUNT(0); MOVE_COUNT(0);
#endif

        // Just in case a did get moved...
        count_copies b;

        // The container will have to create a copy in order to compare with
        // the existing element.
        reset();
        x.emplace(b.tag_);
        COPY_COUNT(1); MOVE_COUNT(0);

        //
        // 2 arguments
        //

        // The container will have to create b copy in order to compare with
        // the existing element.
        //
        // Note to self: If copy_count == 0 it's an error not an optimization.
        // TODO: Devise a better test.

        reset();

        x.emplace(b, b);
        COPY_COUNT(1); MOVE_COUNT(0);
    }

    UNORDERED_AUTO_TEST(unnecessary_copy_emplace_map_test)
    {
        // When calling 'source' the object is moved on some compilers, but not
        // others. So count that here to adjust later.

        reset();
        source<count_copies>();
        int source_cost = ::unnecessary_copy_tests::count_copies::moves;

        //

        reset();
        boost::unordered_map<count_copies, count_copies> x;
        // TODO: Run tests for pairs without const etc.
        std::pair<count_copies const, count_copies> a;
        x.emplace(a);
        COPY_COUNT(4); MOVE_COUNT(0);

        //
        // 0 arguments
        //

        // COPY_COUNT(1) would be okay here.
        reset();
        x.emplace();
        COPY_COUNT(2); MOVE_COUNT(0);

        //
        // 1 argument
        //

        reset();
        x.emplace(a);
        COPY_COUNT(0); MOVE_COUNT(0);

        // A new object is created by source, but it shouldn't be moved or
        // copied.
        //
        // (Note: source_cost is not needed here, because std::pair is not
        // move enabled).
        reset();
        x.emplace(source<std::pair<count_copies, count_copies> >());
        COPY_COUNT(2); MOVE_COUNT(0);

        // TODO: This doesn't work on older versions of gcc.
        //count_copies part;
        std::pair<count_copies const, count_copies> b;
        //reset();
        //std::pair<count_copies const&, count_copies const&> a_ref(part, part);
        //x.emplace(a_ref);
        //COPY_COUNT(0); MOVE_COUNT(0);

#if defined(BOOST_UNORDERED_STD_FORWARD_MOVE)
        // No move should take place.
        // (since a is already in the container)
        reset();
        x.emplace(std::move(a));
        COPY_COUNT(0); MOVE_COUNT(0);
#endif

        //
        // 2 arguments
        //

        reset();
        x.emplace(b.first, b.second);
        COPY_COUNT(0); MOVE_COUNT(0);

        reset();
        x.emplace(source<count_copies>(), source<count_copies>());
        COPY_COUNT(2); MOVE_COUNT(source_cost * 2);

        // source<count_copies> creates a single copy.
        reset();
        x.emplace(b.first, source<count_copies>());
        COPY_COUNT(1); MOVE_COUNT(source_cost);

        reset();
        x.emplace(count_copies(b.first.tag_), count_copies(b.second.tag_));
        COPY_COUNT(2); MOVE_COUNT(0);        
    }
}

RUN_TESTS()
