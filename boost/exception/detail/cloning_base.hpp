//Copyright (c) 2006-2008 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_F7D5662CCB0F11DCA353CAC656D89593
#define UUID_F7D5662CCB0F11DCA353CAC656D89593

namespace
boost
    {
    namespace
    exception_detail
        {
        class clone_base;

        class
        cloning_base
            {
            public:

            virtual clone_base const * clone() const = 0;

            protected:

#ifdef __GNUC__
virtual //Disable bogus GCC warning.
//Would someone at GCC finally get the protected, non-virtual destructor approach?
#endif
            ~cloning_base() throw()
                {
                }
            };
        }
    }

#endif
