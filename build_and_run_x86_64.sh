#!/bin/sh

# Run as
#
#   sh build_and_run_x86_64.sh install_dir_of_qemu_libtcg
#
# running without the install dir will assume system
# defaults

install_dir=$1

echo $install_dir/lib

#$CC -c code.c -o code.o -mcpu=aarch64
$CC -g -o libtcg-test -I $install_dir/include -L$install_dir/lib libtcg-test.c -lqemu-x86_64
#aarch64-linux-gnu-gcc main.c -g -o main
$CC main.c -g -O0 -o main

#SHELL=$(which bash) gdb --eval-command="set env LD_LIBRARY_PATH ${install_dir}/lib" --args ./libtcg-test main
#SHELL=$(which bash) gdb --eval-command="set env LD_LIBRARY_PATH ${install_dir}/lib" --args ./libtcg-test main
LD_LIBRARY_PATH=${install_dir}/lib ./libtcg-test main

#LD_LIBRARY_PATH=${install_dir}/lib ./libtcg-test /home/aj/git/work/qemu-hexagon/qemu_upstream/build/tests/tcg/aarch64-linux-user/linux-test
#SHELL=$(which bash) gdb --eval-command="set env LD_LIBRARY_PATH ${install_dir}/lib" --args ./libtcg-test /home/aj/git/work/qemu-hexagon/qemu_upstream/build/tests/tcg/aarch64-linux-user/linux-test

#LD_LIBRARY_PATH=$install_dir/lib ldd libtcg-test
