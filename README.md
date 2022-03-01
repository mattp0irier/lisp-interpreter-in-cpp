# LISP interpreter in C++
## Matt Poirier and Trevor Russell

This project is for CS 403 Spring 2022. A subset of a LISP interpreter was created in C++. Samuel Kamin's LISP interpreter in Pascal and Robert Nystrom's JLox interpreter from his textbook *Crafting Interpreters* were used as a guide to create our interpreter.

---
## Compilation Instructions
The command used to compile the project is `g++ -std=c++11 lisp-interpreter.cpp -o lisp-interpreter`.

To run in User Input mode:
`./lisp-interpreter`

To run with an input file:
`./lisp-interpreter filename`

---
## Test Cases and Output
Several tests were run on the interpreter in order to verify the interpreter was working. Included test files are:

- simpleStuff: Basic tests of all operations and functions; expected value for each result is printed
- numListOps: A set of operations performed on lists, which demonstrates implicit float to int conversion, defining functions, and several other operations
