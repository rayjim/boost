#!/usr/bin/python

from BoostBuild import Tester, List
from time import sleep

t = Tester()

t.set_tree("test2")
t.run_build_system("-sBOOST_BUILD_PATH=" + t.original_workdir + "/..")

file_list = List("bin/foo/gcc/debug/runtime-link-dynamic/") * List("foo foo.o")
t.expect_addition(file_list)

sleep(2)

t.write("foo.cpp", "int main(int, char**) {}\n")
t.run_build_system("-d2 -sBOOST_BUILD_PATH=" + t.original_workdir + "/..")
t.expect_touch(file_list)
t.pass_test()

