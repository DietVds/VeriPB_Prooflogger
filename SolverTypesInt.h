//===================================================
// Minimal needed for VeriPB Proof Logger:
// ---------------------------------------
// * Implementation of Var (mostly just typedef int Var)
// * Implementation of class Lit. Functions needed:
//          - Var  var(Lit lit); 
//          - bool is_negated(Lit lit); 
//
// Additionally needed for Totalizer Proof Logger:
// -----------------------------------------------
// * For the class Lit:
//          - implementation of Lit(Var var, bool sign = false) constructor
//          - ~ operator
//          
//===================================================
//prooflogging Library

#ifndef SolverTypes_Int_h
#define SolverTypes_Int_h

#include "includes/Glucose/SolverTypes.h"
#include "includes/Minisat/SolverTypes.h"
#include "includes/Minisat/mtl/Vec.h"
#include "includes/VeriPbSolverTypes.h"

inline int variable(int l){return ((l < 0) ? -l : l);}
inline bool is_negated(int l){return (l < 0);}
inline int neg(int l){return -l;}
inline VeriPB::Var toVeriPbVar(int v){return v;}
inline VeriPB::Lit toVeriPbLit(int l){return l;}

inline Glucose30::Var variable(Glucose30::Lit l){return Glucose30::var(l);}
inline bool is_negated(Glucose30::Lit l){return sign(l);}
inline Glucose30::Lit neg(Glucose30::Lit l){return ~l;};
inline VeriPB::Lit toVeriPbLit(Glucose30::Lit l){return (is_negated(l) ? -(variable(l)) : variable(l));}

inline Minisat::Var variable(Minisat::Lit l){return Minisat::var(l);}
inline bool is_negated(Minisat::Lit l){return sign(l);}
inline Minisat::Lit neg(Minisat::Lit l){return ~l;};
inline VeriPB::Lit toVeriPbLit(Minisat::Lit l){return (is_negated(l) ? -(variable(l)) : variable(l));}

#endif