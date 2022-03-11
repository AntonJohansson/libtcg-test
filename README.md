# libtcg-test

Feeds code in `code.c` through `qemu-libtcg` and dumps the output.

The `build_and_run_x86_64.sh` script assumes you have `CC` set to some C-compiler, and have `qemu-libtcg` installed which was configured with
```
qemu-libtcg/build> ../configure --target-list=x86_64-linux-user --disable-capstone --disable-werror --enable-libtcg
```
The install-dir of `qemu-libtcg` can be changed via
```
qemu-libtcg/build> meson configure -Dprefix=/my/install/dir
```
and the `build_and_run_x86_64.sh` script is run as
```
libtcg-test> sh build_and_run_x86_64.sh /my/install/dir
```
