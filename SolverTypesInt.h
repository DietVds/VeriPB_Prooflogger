//==========================================================================
// What is needed to integrate with the VeriPB prooflogging library:
// ------------------------------------------------------------------
// A type for the variable and for a literal. Suppose Var is the type for the variable and Lit for the literal. 
// Then, the following functions should be present:
//
// 1. Var variable(Lit l) - gets the variable out of the literal
// 2. bool is_negated(Lit l) - true if the literal is negated, false otherwise
// 3. Lit neg(Lit l) - returns a literal that is the negation of l
// 4. VeriPB::Var toVeriPbVar(Var v) - converts a variable to a VeriPB variable
// 5. VeriPB::Lit toVeriPbLit(Lit l) - converts a literal to a VeriPB literal
// 
// To use the TotalizerProoflogger:
// ------------------------------------------------------------------
// 6. Lit create_literal(Var var, bool negated) - Every Literal type should have a mkLit function that creates a literal for a given variable.
//==========================================================================
//prooflogging Library

#ifndef SolverTypes_Int_h
#define SolverTypes_Int_h

#include "VeriPbSolverTypes.h"
#include "core/SolverTypes.h"

inline Glucose::Var variable(Glucose::Lit l){return (Glucose::var(l));}
inline bool is_negated(Glucose::Lit l){return sign(l);}
inline Glucose::Lit neg(Glucose::Lit l){return ~l;};
inline Glucose::Lit create_literal(Glucose::Var var, bool negated){return Glucose::mkLit(var, !negated);}

// Glucose's variables start from 0, whereas VeriPB counts their variables from 1. Therefore, the conversion needs to be done.
// Problem: if we would use them both, then it would be a problem. However, pysat already solves this by not using the 0 variable in the solvers.
inline VeriPB::Var toVeriPbVar(Glucose::Var v){VeriPB::Var pbv; pbv.v= v+1; return pbv;}
inline VeriPB::Lit toVeriPbLit(Glucose::Lit l){VeriPB::Lit pbl; pbl.negated = is_negated(l); pbl.v = toVeriPbVar(variable(l)); return pbl;}

/*******************
 * Functions for the VeriPB types. 
*/
inline VeriPB::Var variable(VeriPB::Lit l){return l.v;}
inline bool is_negated(VeriPB::Lit l){return l.negated;}
inline VeriPB::Lit neg(VeriPB::Lit l){VeriPB::Lit newl; newl.v = l.v; newl.negated = !l.negated; return newl;}
inline VeriPB::Lit create_literal(VeriPB::Var var, bool negated){VeriPB::Lit l; l.v = var; l.negated = negated; return l; }

inline VeriPB::Var toVeriPbVar(VeriPB::Var v){return v;}
inline VeriPB::Lit toVeriPbLit(VeriPB::Lit l){return l;}

// The function varidx takes a VeriPB variable and gets the integer that can be used as an index to all the internal data structures like maps and vectors.
// inline int varidx(VeriPB::Var var){return var.v;}

/*
* Other examples of the SolverTypesInt. 
* TODO: rewrite with new VeriPB Solvertypes.
*/

// inline int variable(int l){return ((l < 0) ? -l : l);}
// inline bool is_negated(int l){return (l < 0);}
// inline int neg(int l){return -l;}
// inline VeriPB::Var toVeriPbVar(int v){return v;}
// inline VeriPB::Lit toVeriPbLit(int l){return l;}

// inline Glucose30::Var variable(Glucose30::Lit l){return Glucose30::var(l);}
// inline bool is_negated(Glucose30::Lit l){return sign(l);}
// inline Glucose30::Lit neg(Glucose30::Lit l){return ~l;};
// inline VeriPB::Lit toVeriPbLit(Glucose30::Lit l){return (is_negated(l) ? -(variable(l)) : variable(l));}

// inline Minisat::Var variable(Minisat::Lit l){return Minisat::var(l);}
// inline bool is_negated(Minisat::Lit l){return sign(l);}
// inline Minisat::Lit neg(Minisat::Lit l){return ~l;};
// inline VeriPB::Lit toVeriPbLit(Minisat::Lit l){return (is_negated(l) ? -(variable(l)) : variable(l));}

#endif