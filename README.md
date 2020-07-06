<b>C89 Compiler and Python Translator </b>
=======================================
This Compiler/Translator was created from scratch for the coursework section of the  second year module <b>Language Processors</b>, which is a part of Electronic and Information Engineering at Imperial College London.  The original specification for the project can be found [*here*](doc/original-spec.md). 

Overview
-------------------------
This program fulfills two functions:
1. Compile programs written in C89 (Also known as C90 or ANSI C) to MIPS Assembly.
2. Translate programs written in a subset of C89 to Python. 

    
Input C files are lexically analysed using *Flex* and parsed using *GNU Bison*. The Bison grammar is self written, but does take numerous cues from the original C89 grammar written for use with Yacc (found *[here](https://www.lysator.liu.se/c/ANSI-C-grammar-y.html)*).

The Parser generates an *Abstract Syntax Tree (AST)* of the C program. Each node is represented as a C++ class, with each different construct (e.g. variable, switch case, function declaration) having its own class which inherits from the AST_Node base class. These derived classes each override a pure virtual compilation function (and a Python translation function where required) defined in the base class. By having a parent node call the compilation functions of their children, the tree is traversed and correct assembly/Python code is generated.
    
How to run the program
=======================

Prequisites
------------

- Flex and Bison
- Make
- GCC/G++ (to compile the compiler)


Install
--------
<b>Clone the repository</b> by either downloading the zip file using the Git GUI or using the command line:
    
    git clone https://github.com/adidesh20/C89-Compiler-and-Python-Translator.git

Then, from the top of the directory tree, run:
    
    make bin/c_compiler

Compilation to MIPS
--------------------
The compilation function is invoked using the flag `-S`, with the source file and output file specified on the command line:

    bin/c_compiler -S [source-file.c] -o [dest-file.s]

Translation to Python
----------------------
The translator function is invoked using the flag `--translate`, with the source file and output file specified on the command line:

    bin/c_compiler --translate [source-file.c] -o [dest-file.py]

Translator Subset
====================
<b>C to Python translation works for the following C constructs: </b>

Variables
- Local integer variables (though not nested scopes)
- Global integer variables

Functions
- Definition of functions taking 0 or more integer parameters and with void or      integer return type
- Explicit support for main special functionality

 Statements
- if
- if-else
 - while
- return
- sequences

Expressions
- Decimal integer constants
- Integer arithmetic: `*`,`+`,`-`
- Logical operations: `&&`,`||`
- Comparison operations: `<`,`==`
- Invocation of functions
- Assignment (but only as a direct expression statement)
