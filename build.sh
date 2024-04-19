#!/bin/bash

builddir=build
installdir=root
sourcedir=$(pwd)

[ ! -d $installdir ] && mkdir $installdir

if [ ! -d $builddir ]; then
  mkdir $builddir
  pushd $builddir
    ${sourcedir}/qemu-libtcg/configure \
      --prefix=${installdir} \
      --enable-libtcg \
      --disable-werror \
      --disable-kvm \
      --disable-tools \
      --disable-system \
      --disable-libnfs \
      --disable-vde \
      --disable-gnutls \
      --disable-cap-ng \
      -Dvhost_user=disabled \
      -Dxkbcommon=disabled \
      --disable-werror
    make install -j8
  popd
fi

[ ! -d ${installdir}/bin ] && mkdir ${installdir}/bin

for libtcg in $(ls ${installdir}/lib/libtcg-*.so); do
  filename_ext=${libtcg##*/}
  filename=${filename_ext%%.*}
  outfile=${installdir}/bin/${filename}-test

  echo "Building ${outfile}"
  clang ${sourcedir}/libtcg-test.c \
    -I ${installdir}/include \
    -L ${installdir}/lib \
    -l:${filename_ext} \
    -o ${outfile}
done
