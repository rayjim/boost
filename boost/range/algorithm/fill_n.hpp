//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef BOOST_RANGE_ALGORITHM_FILL_N_HPP_INCLUDED
#define BOOST_RANGE_ALGORITHM_FILL_N_HPP_INCLUDED

#include <boost/assert.hpp>
#include <boost/concept_check.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/concepts.hpp>
#include <algorithm>

namespace boost
{
    /// \brief template function fill_n
    ///
    /// range-based version of the fill_n std algorithm
    ///
    /// \pre ForwardRange is a model of the ForwardRangeConcept
    template< class ForwardRange, class Size, class Value >
    inline ForwardRange& fill_n(ForwardRange& rng, Size n, const Value& val)
    {
        boost::function_requires< ForwardRangeConcept<ForwardRange> >();
        BOOST_ASSERT( static_cast<Size>(std::distance(boost::begin(rng), boost::end(rng))) >= n );
        std::fill_n(boost::begin(rng), n, val);
        return rng;
    }
}

#endif // include guard
