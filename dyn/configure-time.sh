#!/bin/bash

set -e

CCFLAGS="-Wall -L ../build/lib -I ../build/include -O2 -Wl,--as-needed -Wl,--no-undefined"

# This needs to use the same compiler and options as will be used to build the
# PMD:
gcc $CCFLAGS -o link-test link-test.c -libverbs -lmlx5 -lmlx4 -ldl

# Build the header file
# This could be less frail if python or perl was used to do the text processing.
readelf --wide -s link-test | awk '/(@@IBVERBS)|(@@MLX)/{split($8,a,"@@"); printf("#define VERSION_%s \"%s\"\n",a[1],a[2])}' | sort -u > dyn-verbs-symver.h
readelf --wide -d link-test | grep 'NEEDED' | sed -e 's/.*\[\([^.]*\)\(.*\)\].*/#define SONAME_\1 "\1\2"/g' >> dyn-verbs-symver.h

# The above is done once at 'configure' time.
# The rest demonstrates compiling the PMD:

# Adding the -I builds using libdl mode using the shim header files. Note
# order matters, must be first
gcc -I`pwd` $CCFLAGS -g -o pmd-dl dyn-verbs.c pmd.c -ldl

# Otherwise without the -I it builds using direct linking
gcc $CCFLAGS -g -o pmd-direct dyn-verbs.c pmd.c -libverbs -ldl

# Demonstate the functionality using an example from rdma-core
# This needs the __attribute__ ((constructor)) on dl_link_init to work
gcc -I`pwd` $CCFLAGS -g -o ibv_rc_pingpong dyn-verbs.c ../libibverbs/examples/{rc_pingpong.c,pingpong.c} -ldl
