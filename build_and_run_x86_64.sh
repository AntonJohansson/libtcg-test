#!/bin/sh

$CC -c code.c -o code.o
$CC libtcg-test.c -lqemu-x86_64 -g -o libtcg-test
./libtcg-test code.o
