Project Summary
===============

This project began with the stated goal to build a generator for low-level Dynsem interpreters to compare its performance against the actual Java-based Dynsem implementation. The hypothesis was that LLVM might have features (i.e. JIT) providing exceptionally fast performance due to their low-level nature. Bottom line up front: the LLVM JIT did not seem to offer great performance advantages (e.g. http://stackoverflow.com/questions/6833068) and building the interpreter generator was a difficult enough task so this initial implementation abandoned the LLVM work and focused on this latter task. In any case, since the goal was largely an optimization and "premature optimization is the root of all evil," it seemed wise to implement a working, high-level version first and then profile it to focus on the hot spots.

My initial attempts at programming even a simple interpreter in pure LLVM were insightful but very inefficient. The absence of higher-level abstractions and my inexperience with LLVM made this painfully slow. Switching to C, however, allowed me to implement a sample interpreter that performed the essential functions of a Dynsem interpreter: _match_ a given term against a set of rules, _transform_ the term using a matching rule, and _repeat_ until matching fails. 

Dynsem rules look like `t1 -> t2 where ps`; this means that if the current program matches a term `t1` as well as its optional premises `ps`, the interpreter will instantiate and return `t2`. For example, given the rule `a(b(x)) -> x` and a program `a(b(42))`, the interpreter would reduce this program to `42`. Dynsem has other features, such as signatures and semantic components, but these were not planned for this project. 

With a sample C interpreter in hand, ... 

