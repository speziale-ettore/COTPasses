
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
    $ git clone git://github.com/speziale-ettore/COTPasses.git cot

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

That is all, you are ready to start coding!

Project Structure
-----------------

Compiler code-bases are huge! In order to effectively working with them, coding
conventions are needed. This projects follows LLVM coding conventions. In
particular:

* It is a set of _libraries_
* Each library is implemented in a private subdirectory of `lib`
* Library public files are put under `include/cot`

In addition:

* Each compiler pass consists of a static library
* An LLVM `opt` module, `COTPasses`, contains all available passes

Please follow the same convention in your project!

Adding a Pass
-------------

To add a new pass to this project:

1. Create the pass implementation directory under `lib`
2. Add pass implementation stub, using another pass, e.g. `InstructionCount`, as
   reference
3. Write a `Makefile` for the pass. This usually requires copying another pass
   Makefile and changing `LIBRARYNAME` to match your pass name
4. Modify `lib/Makefile` in order to visit your pass directory
5. If needed, put your pass public header files under `include/cot`
6. Add pass creation and initialization, `Create*` and `initialize*`
   respectively, prototypes to `include/code/AllPasses.h`

In order to be usable, the pass must be linked inside `COTPasses`:

1. Add pass static library to `tools/COTPasses/Makefile` `USEDLIBS` variable
2. Edit `tools/COTPasses/ForceLinking.cpp`, adding calls to pass creation and
   initialization functions to `::ForceLinking::ForceLinking` and
   `::ForceInitialization::ForceInitialization` respectively

Pass tests are under the `test` directory:

1. Create a directory for your test passes under `test/COTPasses`
2. Mark the new directory as a test directory by putting inside it a `dg.exp`
   file. You can just copy `test/COTPasses/InstructionCount/dg.exp`
3. Add a test using LLVM testing infrastructure

Testing in LLVM are simple. Comments inside a bytecode file contains commands to
execute tests. Usually, there is an header, telling how to perform the test, for
example:

    ; RUN: opt -load %projshlibdir/COTPasses.so \
    ; RUN:     -instruction-count -analyze      \
    ; RUN:     -S -o - %s | FileCheck %s
    ; REQUIRES: loadable_module

Then, there are checks:

    ;CHECK:      Printing analysis 'Count instructions' for function 'fact':
    ;CHECK-NEXT:   Instruction count: 14

Refer to the [LLVM Testing Infrastructure Guide][www/llvmTest] for further
information.

Additional Info for Students
----------------------------

Your project must be an extension of this project. You are required to use Git
to version your code. A good tutorial is available [here][www/proGit].

Tests are a part of the project, so you have also to add them in order to prove
that your pass is working correctly. Tests must be added according to the
aforementioned testing infrastructure. Obviously, code transformed/generated by
your pass must be compilable and runnable.

Please notice that LLVM is very well documented: code is very readable and
different howtos and reference manuals are available [here][www/llvmDoc].

[www/llvm]:          http://www.llvm.org
[www/llvmDoc]:       http://llvm.org/releases/3.0/docs/index.html
[www/llvmTest]:      http://llvm.org/releases/3.0/docs/TestingGuide.html
[www/proGit]:        http://progit.org/book
[www/cotRepository]: https://github.com/speziale-ettore/COTPasses
