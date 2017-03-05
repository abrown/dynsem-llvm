# dynsem-llvm

Experimental project to compile a DynSem interpreter using LLVM; in its current
state it does not yet generate LLVM IR because of the functionality provided by
the [aterms](https://github.com/cwi-swat/aterms) library. __Disclaimer: this 
is not production code--it is very much an experiment.__

# Build

1. See `Dockerfile` for the compilers required to build the application
2. Install the `meta-env` tool (necessary to build the aterms library):

    ```
    git clone https://github.com/cwi-swat/meta-environment
    cd meta-environment/meta-build-env/
    ./reconf
    ./configure && make && make install
    ```
3. Install the `aterms` library:

    ```
    git clone https://github.com/cwi-swat/aterms
    cd aterms/aterm
    ./reconf
    ./configure && make && make install
    ```
4. Build the generator with `make` (test with `make test`; note that this 
project uses Netbeans-generated Makefiles and unit tests so they may hard to 
read).
5. Build an interpreter using the generator: `make interpreter`

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

# Questions

- Can I manipulate the AST directly in memory and `free` pruned nodes as the 
transformation progresses? In other words, should I be changing the pointers of 
the AST or memcopying into a new tree?

    With `aterms` we can worry about this less; it has some garbage collection
    and reuses shared nodes when it can

- Should I generate integer symbols for each constructor so that I can use 
integer comparisons instead of string comparisons in the pattern matching?

    `aterms` hashes the constructor name and does address comparisons

- Does the interpreter need to have knowledge of the DynSem type system or can 
it all be statically checked before it gets to the interpreter?

- Currently the constants are variably-sized so I do a `strncmp` to match them; 
I would like to be able to use int/float comparisons when possible but that 
seems to indicate that I will have more than one `Constant` type (one for each 
DynSem type?)

# To-do

- Generate LLVM IR from DynSem specification instead of hand-coding C (as currently done in tests)
- Generate decision trees based on rules: which optimization heuristic to use, though?
- Add semantic components: I can find generally optimal implementations of maps/lists but they must be reversible--in a rule with three side-effect premises if the third premise fails, the interpreter must be able to "undo" the side-effects (are these the "frames" in the implementation?) 
- Figure out how to install `build-meta-env` in the Dockerfile; running into environment issues
- Use a templating library like [mustach](https://gitlab.com/jobol/mustach) to avoid the vast amount of unreadable `puts` and `fprintf` in the generator

# Documentation

- [Vergu et al: DynSem: A DSL for Dynamic Semantics Specification](http://swerl.tudelft.nl/twiki/pub/Main/TechnicalReports/TUD-SERG-2015-003.pdf)
- [DynSem Implementation](https://github.com/metaborg/dynsem)
- [SIMPL Reference Language](https://github.com/MetaBorgCube/simpl)
- [Tolmach: Notes on Patterns](doc/tolmach-pattern-notes.pdf)
- [Maranget: Compiling Pattern Matching to Good Decision Trees](http://moscova.inria.fr/~maranget/papers/ml05e-maranget.pdf)
- [Scott and Ramsey: When Do Match-Compilation Heuristics Matter?](https://pdfs.semanticscholar.org/b8d1/e3b73db3e6549334cc7c20da060516c3188a.pdf)
