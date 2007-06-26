///////////////////////////////////////////////////////////////////////////////
/// \file symbols.hpp
///   Contains the Ternary Search Trie implementation.
/// Based on the following papers:
/// J. Bentley and R. Sedgewick. (1998) Ternary search trees. Dr. Dobbs Journal
/// G. Badr and B.J. Oommen. (2005) Self-Adjusting of Ternary Search Tries Using
/// 	Conditional Rotations and Randomized Heuristics
//
//  Copyright 2007 David Jenkins. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_XPRESSIVE_DETAIL_SYMBOLS_HPP_DRJ_06_11_2007
#define BOOST_XPRESSIVE_DETAIL_SYMBOLS_HPP_DRJ_06_11_2007

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/range/iterator.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/xpressive/detail/utility/counted_base.hpp>

namespace boost { namespace xpressive { namespace detail
{

    ///////////////////////////////////////////////////////////////////////////////
    // symbols (using a ternary search trie)
    //
    template<typename T>
    struct symbols
    {
        typedef typename range_value<T>::type::first_type key_type;
        typedef typename range_value<T>::type::second_type value_type;
        typedef typename range_value<key_type>::type char_type;
        typedef typename range_iterator<T const>::type iterator;
        typedef typename range_iterator<key_type const>::type key_iterator;
        typedef value_type const *result_type;

        symbols()
          : root(0)
        {}

        // copies of this symbols table share the TST

        template<typename Trans>
        void load(T const &m, Trans trans)
        {
            if(0 == this->root)
            {
                iterator begin = boost::begin(m);
                iterator end = boost::end(m);
                for(; begin != end; ++begin)
                {
                    key_iterator kbegin = boost::begin(begin->first);
                    key_iterator kend = boost::end(begin->first);
                    this->root = this->insert(this->root, kbegin, kend, &begin->second, trans);
                }
            }
        }

        template<typename BidiIter, typename Trans>
        result_type operator ()(BidiIter &begin, BidiIter end, Trans trans) const
        {
            return this->search(this->root, begin, end, trans);
        }

    private:
        struct node;
        typedef intrusive_ptr<node> node_ptr;

        struct node
          : counted_base<node>
        {
            node()
              : counted_base<node>()
              , ch(0)
              , lo(0)
              , eq(0)
              , hi(0)
              , result(0)
            {}

            char_type ch;
            node_ptr lo;
            node_ptr eq;
            node_ptr hi;
            result_type result;
        };

        template<typename Trans>
        node_ptr insert(node_ptr const &pp, key_iterator &begin, key_iterator end, result_type r, Trans trans) const
        {
            node_ptr p = pp;
            char_type c1 = 0;

            if(begin != end)
            {
                c1 = trans(*begin);
            }

            if(0 == p)
            {
                p = new node;
                p->ch = c1;
            }

            if(c1 < p->ch)
            {
                p->lo = this->insert(p->lo, begin, end, r, trans);
            }
            else if(c1 == p->ch)
            {
                if(0 == c1)
                {
                    p->result = r;
                }
                else
                {
                    p->eq = this->insert(p->eq, ++begin, end, r, trans);
                }
            }
            else
            {
                p->hi = this->insert(p->hi, begin, end, r, trans);
            }

            return p;
        }

        template<typename BidiIter, typename Trans>
        result_type search(node_ptr const &p, BidiIter &begin, BidiIter end, Trans trans) const
        {
            result_type r = 0;

            if(0 != p)
            {
                BidiIter isave = begin;

                if(begin != end)
                {
                    char_type c1 = trans(*begin);

                    if(c1 == p->ch)
                    {
                        r = this->search(p->eq, ++begin, end, trans);
                    }
                    else if(c1 < p->ch)
                    {
                        r = this->search(p->lo, begin, end, trans);
                    }
                    else // (c1 > p->ch)
                    {
                        r = this->search(p->hi, begin, end, trans);
                    }
                }

                if(0 == r && 0 == p->ch)
                {
                    begin = isave;
                    r = p->result;
                }
            }

            return r;
        }

        node_ptr root;
    };

}}} // namespace boost::xpressive::detail

#endif
