# Status: stub, just enough to make tests work.
#
# Named os_j to avoid conflicts with standard 'os'. See
# project.py:import for special-casing.
#
# Copyright 2001, 2002, 2003, 2005 Dave Abrahams
# Copyright 2006 Rene Rivera
# Copyright 2003, 2005 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)


import os

def name():
    return os.name
