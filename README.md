# libtcg-test

Feeds code in `code.c` through `qemu-libtcg` and dumps the output.

The `build_and_run_x86_64.sh` script assumes you have `CC` set to some C-compiler, and have `qemu-libtcg` installed which was configured with
```
../configure --target-list=x86_64-linux-user --disable-capstone --disable-werror --enable-libtcg
```
