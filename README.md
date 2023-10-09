# VeriPBProoflogger

This repository contains useful C++-code to implement proof logging in the VeriPB proof system. 

## SolverTypes
This file implements the classes VeriPB::Lit and VeriPB::Var, which are used in the code. 

## SolvingTypesInt

To use the code in this repository, you first need to start by altering the SolverTypesInt.h file. 
The idea is to include the file implementing the types in the solver and implement the conversion functions to the VeriPB types. 

## VeriPBProoflogger 

Contains code for general proof logging actions, e.g., writing RUP, Redundance-Based Strengthening, Cutting Planes derivations and so on. 
Every Prooflogger in this repository contains of 3 files, a .h, .hpp and a .cc file. 
What is particular is that the .hpp file contains templated functions, such that when you want to use the code, you can alter the .cc file with the types of your own solver, as long as the conversion functions in SolverTypesInt are correctly implemented.

## MaxSATProoflogger

Contains specific code for proof logging MaxSAT solvers. This prooflogger can be used to handle relaxation variables while parsing the input-formula, derive the objective reformulation constraint in certifying OLL, and proving intrinsic at-most-one constraints. 

## PBtoCNFprooflogger 

Contains code for proof logging CNF encodings of pseudo-Boolean constraints. The following encodings are supported: Totalizer encoding, Binary Adder encoding, Modulo-Based Totalizer encoding and partly Cardinality Networks. 

## SortingNetworkProoflogger

Deprecated. This library needs cleaning up and should be combined in the PBtoCNFprooflogger.
