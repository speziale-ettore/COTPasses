
COT -- Code Optimization and Transformation Examples
====================================================

This is a simple [LLVM][www/llvm] project containing code used during the
classes. Some passes makes no sense (e.g. function-erase), but they are used to
show how to use LLVM API.

Quick Start
-----------

It is strongly encouraged to keep source tree separated from build tree, so we
assume the following directory structure:

* Let `LLVM_WORK` be the working directory
* Let `LLVM_SRC` the LLVM sources directory
* Let `LLVM_BUILD` the LLVM objects directory
* Let `LLVM_ROOT` the LLVM root installation directory

You can use whichever working directory you want. The other directory are
assumed to be children of `LLVM_WORK`. From now on:

* Let `LLVM_SRC` be `$LLMV_WORK/src`
* Let `LLVM_BUILD` be `$LLVM_WORK/build`
* Let `LLVM_ROOT` be `$LLVM_ROOT/root`

First we create the working directory:

    $ mkdir $LLVM_WORK

LLVM-3.0 and CLANG-3.0 are needed. We can get them from Git repositories:

    $ cd $LLVM_WORK
    $ git clone -n -o llvm-upstream --depth 1 \
                http://llvm.org/git/llvm.git $LLVM_SRC
    $ cd $LLVM_SRC
    $ git checkout -t remotes/llvm-upstream/release_30
    $ git clone -n -o clang-upstream --depth 1 \
                http://llvm.org/git/clang.git tools/clang
    $ cd tools/clang
    $ git checkout -t remotes/clang-upstream/release_30

We will build in `LLVM_BUILD`:

    $ mkdir $LLVM_BUILD
    $ cd $LLVM_BUILD
    $ $LLVM_SRC/configure --prefix=$LLVM_ROOT --disable-optimized
    $ make -j $N
    $ make install

Using the `--disable-optimized` configure switch, the compiler is built in debug
mode. This usually requires a lot of disk space. If you have space problems, try
using the optimized version, given `--enable-optimized` instead of
`--disable-optimized`.

Building with `make -J $N` allows to spawn multiple compiler in parallel, thus
speeding up the compilation process. Usually `N` is set to twice the number of
available CPUs.

In order to start hacking with COT passes you have to clone the COT master
repository on [GitHub][www/cotRepository] into LLVM `projects` directory:

    $ cd $LLVM_SRC/projects
    $ git clone REPO cot

You need to build the `configure` script:

    $ cd cot/autoconf
    $ ./AutoRegen.sh

Finally you have to build it:

    $ cd $LLVM_BUILD/projects
    $ mkdir cot
    $ cd cot
    $ $LLVM_SRC/projects/cot/configure --prefix=$LLVM_ROOT
    $ make -j $N

After building, tests are run using:

    $ make check

That is all, you are ready start coding!

[www/llvm]:          http://www.llvm.org
[www/cotRepository]: REPO
