#!/usr/bin/python

# Copyright (C) 2013 Steven Watanabe
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

import sys

def create(t):
  t.write('''mockinfo.py''', '''
import re
import optparse
import os

parser = optparse.OptionParser()
parser.add_option('-o', dest="output_file")
parser.add_option('-x', dest="language")
parser.add_option('-c', dest="compile", action="store_true")
parser.add_option('-I', dest="includes", action="append")
parser.add_option('-L', dest="library_path", action="append")
parser.add_option('--dll', dest="dll", action="store_true")
parser.add_option('--archive', dest="archive", action="store_true")
parser.add_option('--static-lib', dest="static_libraries", action="append")
parser.add_option('--shared-lib', dest="shared_libraries", action="append")

class MockInfo(object):
  def __init__(self):
    self.files = dict()
    self.commands = list()
  def source_file(self, name, pattern):
    self.files[name] = pattern
  def action(self, command, status=0):
    self.commands.append((command, status))
  def check(self, command):
    for (raw, status) in self.commands:
      if self.matches(raw, command):
        return status
  def matches(self, raw, command):
    (expected_options, expected_args) = parser.parse_args(raw.split())
    options = command[0]
    input_files = list(command[1])
    if len(expected_args) != len(input_files):
      return False
    for arg in expected_args:
      if arg.startswith('$'):
        fileid = arg[1:]
        pattern = self.files[fileid] if fileid in self.files else fileid
        matching_file = None
        for input_file in input_files:
          with open(input_file, 'r') as f:
            contents = f.read()
          if pattern == contents:
            matching_file = input_file
            break
        if matching_file is not None:
          input_files.remove(matching_file)
        else:
          return False
      else:
        if arg in input_files:
          input_files.remove(arg)
        else:
          return False

    if options.language != expected_options.language:
      return False

    if options.compile != expected_options.compile:
      return False

    # order matters
    if options.includes is None:
      options.includes = []
    if expected_options.includes is None:
      expected_options.includes = []
    if map(os.path.abspath, options.includes) != \
        map(os.path.abspath, expected_options.includes):
      return False

    if options.library_path is None:
      options.library_path = []
    if expected_options.library_path is None:
      expected_options.library_path = []
    if map(os.path.abspath, options.library_path) != \
        map(os.path.abspath, expected_options.library_path):
      return False

    if options.static_libraries != expected_options.static_libraries:
      return False

    if options.shared_libraries != expected_options.shared_libraries:
      return False

    if options.dll != expected_options.dll:
      return False

    if options.archive != expected_options.archive:
      return False

    # The output must be handled after everything else
    # is validated
    if expected_options.output_file is not None:
      if options.output_file is not None:
        if expected_options.output_file.startswith('$'):
          fileid = expected_options.output_file[1:]
          if fileid not in self.files:
            self.files[fileid] = fileid
          else:
            assert(self.files[fileid] == fileid)
          with open(options.output_file, 'w') as output:
            output.write(fileid)
      else:
        return False
    elif options.output_file is not None:
      return False

    # if we've gotten here, then everything matched
    return True
''')

  t.write('mock.py', '''
import mockinfo
import markup
import sys

status = markup.info.check(mockinfo.parser.parse_args())
if status is not None:
  exit(status)
else:
  print("Unrecognized command: " + ' '.join(sys.argv))
  exit(1)
''')

  t.write('mock.jam', '''
import feature ;
import toolset ;

python-cmd = "\"%s\"" ;

rule init ( )
{
}

feature.extend toolset : mock ;

generators.register-c-compiler mock.compile.c++ : CPP : OBJ : <toolset>mock ;
generators.register-c-compiler mock.compile.c : C : OBJ : <toolset>mock ;

generators.register-linker mock.link : LIB OBJ : EXE : <toolset>mock ;
generators.register-linker mock.link.dll : LIB OBJ : SHARED_LIB : <toolset>mock ;
generators.register-archiver mock.archive : OBJ : STATIC_LIB : <toolset>mock ;

toolset.flags mock.compile INCLUDES <include> ;

actions compile.c
{
   $(python-cmd) mock.py -c -x c -I$(INCLUDES) $(>) -o $(<)
}

actions compile.c++
{
    $(python-cmd) mock.py -c -x c++ -I$(INCLUDES) $(>) -o $(<)
}

toolset.flags mock.link USER_OPTIONS <linkflags> ;
toolset.flags mock.link FINDLIBS-STATIC <find-static-library> ;
toolset.flags mock.link FINDLIBS-SHARED <find-shared-library> ;
toolset.flags mock.link LINK_PATH <library-path> ;
toolset.flags mock.link LIBRARIES <library-file> ;

actions link
{
    $(python-cmd) mock.py $(>) -o $(<) $(USER_OPTIONS) -L$(LINK_PATH) --static-lib=$(FINDLIBS-STATIC) --shared-lib=$(FINDLIBS-SHARED)
}

actions archive
{
    $(python-cmd) mock.py --archive $(>) -o $(<) $(USER_OPTIONS)
}

actions link.dll
{
    $(python-cmd) mock.py --dll $(>) -o $(<) $(USER_OPTIONS) -L$(LINK_PATH) --static-lib=$(FINDLIBS-STATIC) --shared-lib=$(FINDLIBS-SHARED)
}

''' % sys.executable.replace('\\', '\\\\'))

def set_expected(t, markup):
  t.write('markup.py', '''
import mockinfo
info = mockinfo.MockInfo()
def source_file(name, contents):
  info.source_file(name, contents)
def action(command, status=0):
  info.action(command, status)
''' + markup)
