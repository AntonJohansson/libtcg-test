#!/bin/sh

# Run as
#
#   sh build_and_run_x86_64.sh install_dir_of_qemu_libtcg
#
# running without the install dir will assume system
# defaults

install_dir=$1

$CC -c code.c -o code.o
$CC libtcg-test.c -L $install_dir/lib -I $install_dir/include -lqemu-x86_64 -g -o libtcg-test
LD_LIBRARY_PATH=$install_dir/lib ./libtcg-test code.o
