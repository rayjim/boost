# Copyright David Abrahams 2004. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

from b2.build import type

def register ():
    type.register_type('CPP', ['cpp', 'cxx', 'cc'])
    type.register_type('H', ['h'])
    type.register_type('HPP', ['hpp'], 'H')
    type.register_type('C', ['c'])

register ()
