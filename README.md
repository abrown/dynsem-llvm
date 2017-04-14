# dynsem-llvm

Experimental project to compile a DynSem interpreter using LLVM; in its current
state it does not yet generate LLVM IR because of the functionality provided by
the [aterms](https://github.com/cwi-swat/aterms) library. __Disclaimer: this 
is not production code--it is very much an experiment.__

# Build

1. See `Dockerfile` for the compilers required to build the application
2. Install the `meta-env` tool (necessary to build the aterms library):

    ```
    apt install autoconf make
    git clone https://github.com/cwi-swat/meta-environment
    cd meta-environment/meta-build-env/
    ./reconf
    ./configure && make && make install
    ```
3. Install the `aterms` library:

    ```
    apt install build-essential libtool
    git clone https://github.com/cwi-swat/aterms
    cd aterms/aterm
    ./reconf
    ./configure && make && make install   # use CFLAGS=-m32 on a 64-bit system to avoid errors
    ```

4. Install the `cii` library (Ubuntu-specific):

    ```
    git clone https://github.com/drh/cii
    cd cii
    make -k THREADS=  # if necessary, modify makefile CFLAGS to include -m32
    sudo ln -s ./include /usr/local/include/cii
    sudo ln -s ./libcii.a /usr/local/lib/libcii.a
    ```

5. Build the generator with:

    ```
    apt install bison flex clang
    git clone https://github.com/abrown/dynsem-llvm
    make  # use CFLAGS=-m32 if necessary
    ```
    
    Notes: test with `make test`; this project uses Netbeans-generated Makefiles and 
unit tests so they may hard to read).

6. Generate the interpreter code using the generator and a specification file 
(see `specs` directory): `dist/Debug/[OS]/dynsem-llvm [spec-file.ds]`. This will
generate code in the `generated` directory that you can build with 
`make -C generated`. For an example of this, see the `fibonacci` and `example`
make targets (or run `make fibonacci`/`make example`).

# Run

The generated interpreter sources and binaries will be located in the 
`generated` directory. Run the interpreter by passing it a program, e.g. 
`./interpreter program.aterm`. You should see a sequence of transformations:

```
#0 a(1,2)
#1 b(1)
#2 c(1,0,42)
#3 42
```

# To-do

- Generate LLVM IR from DynSem specification instead of hand-coding C (as currently done in tests)
- Generate decision trees based on rules: which optimization heuristic to use, though?
- Add semantic components: I can find generally optimal implementations of maps/lists but they must be reversible--in a rule with three side-effect premises if the third premise fails, the interpreter must be able to "undo" the side-effects (are these the "frames" in the implementation?) 
- Figure out how to install `build-meta-env` in the Dockerfile; running into environment issues
- Use a templating library like [mustach](https://gitlab.com/jobol/mustach) to avoid the vast amount of unreadable `puts` and `fprintf` in the generator
- Self-contain the third-party libraries as git submodules and install in the `3rd` directory to avoid OS-level installs
- Add better logging for premise-level transformations

# Documentation

- [Vergu et al: DynSem: A DSL for Dynamic Semantics Specification](http://swerl.tudelft.nl/twiki/pub/Main/TechnicalReports/TUD-SERG-2015-003.pdf)
- [DynSem Implementation](https://github.com/metaborg/dynsem)
- [SIMPL Reference Language](https://github.com/MetaBorgCube/simpl)
- [Tolmach: Notes on Patterns](doc/tolmach-pattern-notes.pdf)
- [Maranget: Compiling Pattern Matching to Good Decision Trees](http://moscova.inria.fr/~maranget/papers/ml05e-maranget.pdf)
- [Scott and Ramsey: When Do Match-Compilation Heuristics Matter?](https://pdfs.semanticscholar.org/b8d1/e3b73db3e6549334cc7c20da060516c3188a.pdf)
- [ATerm Library Reference](http://homepages.cwi.nl/~daybuild/daily-books/technology/aterm-guide/aterm-guide.html)
