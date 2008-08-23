//  Copyright (c) 2001-2008 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_SPIRIT_KARMA_STREAM_MAY_01_2007_0310PM)
#define BOOST_SPIRIT_KARMA_STREAM_MAY_01_2007_0310PM

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once      // MS compatible compilers support #pragma once
#endif

#include <boost/spirit/home/karma/stream/detail/format_manip.hpp>
#include <boost/spirit/home/karma/stream/detail/iterator_ostream.hpp>
#include <boost/spirit/home/support/detail/hold_any.hpp>

#include <iosfwd>

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit
{
    // overload the streaming operators for the unused_type
    template <typename Char, typename Traits>
    inline std::basic_ostream<Char, Traits>&
    operator<< (std::basic_ostream<Char, Traits>& os, unused_type)
    {
        return os;
    }

}}

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit { namespace karma
{
    template <typename Char>
    struct any_stream
    {
        template <typename Component, typename Context, typename Unused>
        struct attribute
        {
            typedef spirit::hold_any type;
        };

        // any_stream has a parameter attached

        // this overload will be used in the normal case (not called from
        // format_manip).
        template <typename Component, typename OutputIterator,
            typename Context, typename Delimiter, typename Parameter>
        static bool
        generate(Component const& component, OutputIterator& sink,
            Context& /*ctx*/, Delimiter const& d, Parameter const& param)
        {
            typedef
                karma::detail::iterator_sink<OutputIterator, Char>
            sink_device;

            iostreams::stream<sink_device> ostr(sink);
            ostr << param << std::flush;        // use existing operator<<()

            if (ostr.good()) {
                karma::delimit(sink, d);        // always do post-delimiting
                return true;
            }
            return false;
        }

        // this is a special overload to detect if the output iterator has been
        // generated by a format_manip object.
        template <
            typename Component, typename T, typename Traits,
            typename Context, typename Delimiter, typename Parameter
        >
        static bool
        generate(Component const& component,
            karma::detail::output_iterator<
                karma::detail::ostream_iterator<T, Char, Traits>
            >& sink, Context&, Delimiter const& d, Parameter const& param)
        {
            typedef karma::detail::output_iterator<
                karma::detail::ostream_iterator<T, Char, Traits>
            > output_iterator;
            typedef
                karma::detail::iterator_sink<output_iterator, Char>
            sink_device;

            iostreams::stream<sink_device> ostr(sink);
            ostr.imbue(sink.get_ostream().getloc());
            ostr << param << std::flush;        // use existing operator<<()

            if (ostr.good()) {
                karma::delimit(sink, d);        // always do post-delimiting
                return true;
            }
            return false;
        }

        // this any_stream has no parameter attached, it needs to have been
        // initialized from a value/variable
        template <typename Component, typename OutputIterator,
            typename Context, typename Delimiter>
        static bool
        generate(Component const&, OutputIterator&, Context&, Delimiter const&,
            unused_type)
        {
            BOOST_MPL_ASSERT_MSG(false, stream__not_usable_without_attribute,
              (Component, OutputIterator, Delimiter));
            return false;
        }

        template <typename Component, typename Context>
        static std::string what(Component const& component, Context const& ctx)
        {
            return "any-stream";
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    template <typename Char>
    struct stream_director
    {
        template <typename Component, typename Context, typename Unused>
        struct attribute
        {
            typedef unused_type type;
        };

        // stream_director has a parameter attached
        template <typename Component, typename OutputIterator,
            typename Context, typename Delimiter, typename Parameter>
        static bool
        generate(Component const& component, OutputIterator& sink,
            Context& /*ctx*/, Delimiter const& d, Parameter const&)
        {
            typedef
                karma::detail::iterator_sink<OutputIterator, Char>
            sink_device;

            // use existing operator<<()
            iostreams::stream<sink_device> ostr(sink);
            ostr << fusion::at_c<0>(component.elements) << std::flush;

            if (ostr.good()) {
                karma::delimit(sink, d);        // always do post-delimiting
                return true;
            }
            return false;
        }

        // this is a special overload to detect if the output iterator has been
        // generated by a format_manip object.
        template <
            typename Component, typename T, typename Traits,
            typename Context, typename Delimiter, typename Parameter
        >
        static bool
        generate(Component const& component,
            karma::detail::output_iterator<
                karma::detail::ostream_iterator<T, Char, Traits>
            >& sink, Context&, Delimiter const& d, Parameter const&)
        {
            typedef karma::detail::output_iterator<
                karma::detail::ostream_iterator<T, Char, Traits>
            > output_iterator;
            typedef
                karma::detail::iterator_sink<output_iterator, Char>
            sink_device;

            // use existing operator<<()
            iostreams::stream<sink_device> ostr(sink);
            ostr.imbue(sink.get_ostream().getloc());
            ostr << fusion::at_c<0>(component.elements) << std::flush;

            if (ostr.good()) {
                karma::delimit(sink, d);        // always do post-delimiting
                return true;
            }
            return false;
        }

        template <typename Component, typename Context>
        static std::string what(Component const& component, Context const& ctx)
        {
            return "stream";
        }
    };

}}}

#endif
