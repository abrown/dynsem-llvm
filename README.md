# dynsem-llvm

Experimental project to compile a DynSem interpreter using LLVM. Disclaimer: this is not production code--it is very much an experiment.

# Run

The main application currently does nothing (e.g. `return 0`) but it can be build using `make` (uses Netbeans-generated Makefiles so they may hard to read). Tests (i.e. where the actual development is showcased) are run using `make test`; note that Netbeans auto-generates some interesting test output (e.g. `%%TEST_STARTED%% ...`) but the return codes should be sane.

# Questions

- Can I manipulate the AST directly in memory and `free` pruned nodes as the transformation progresses? In other words, should I be changing the pointers of the AST or memcopying into a new tree?
- Should I generate integer symbols for each constructor so that I can use integer comparisons instead of string comparisons in the pattern matching?
- Does the interpreter need to have knowledge of the DynSem type system or can it all be statically checked before it gets to the interpreter?
- Currently the constants are variably-sized so I do a `strncmp` to match them; I would like to be able to use int/float comparisons when possible but that seems to indicate that I will have more than one `Constant` type (one for each DynSem type?)

# To-do

- Generate LLVM IR from DynSem specification instead of hand-coding C (as currently done in tests)
- Generate decision trees based on rules: which optimization heuristic to use, though?
- Add premises to rules: this doesn't look too complicated from page 10 of the DSL paper but I am still unclear how/whether the semantic components can affect the ordering of the premises.
- Add semantic components: I can find generally optimal implementations of maps/lists but they must be reversible--in a rule with three side-effect premises if the third premise fails, the interpreter must be able to "undo" the side-effects (are these the "frames" in the implementation?) 

# Doc

- [Vergu et al: DynSem: A DSL for Dynamic Semantics Specification](http://swerl.tudelft.nl/twiki/pub/Main/TechnicalReports/TUD-SERG-2015-003.pdf)
- [DynSem Implementation](https://github.com/metaborg/dynsem)
- [Tolmach: Notes on Patterns](doc/tolmach-pattern-notes.pdf)
- [Maranget: Compiling Pattern Matching to Good Decision Trees](http://moscova.inria.fr/~maranget/papers/ml05e-maranget.pdf)
- [Scott and Ramsey: When Do Match-Compilation Heuristics Matter?](https://pdfs.semanticscholar.org/b8d1/e3b73db3e6549334cc7c20da060516c3188a.pdf)
