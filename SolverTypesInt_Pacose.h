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
#include <cmath>
#include <cstdint>

// the type that is presenting weights in the solver. This should be defined by the solver developer.
#define wght uint64_t
#define wght_max UINT64_MAX
#define signedWght int64_t 
// typedef long long int wght; 


// Pacose's Solvertypes:

inline uint32_t variable(uint32_t l){return l >> 1;}
inline bool is_negated(uint32_t l){return l & 1;}
inline uint32_t neg(uint32_t l){return l ^ 1; }
inline uint32_t create_literal(uint32_t var, bool negated){return var << 1 ^ negated;}

inline VeriPB::Var toVeriPbVar(uint32_t v){VeriPB::Var pbv; pbv.v= v; return pbv;}
inline VeriPB::Lit toVeriPbLit(uint32_t l){VeriPB::Lit pbl; pbl.negated = is_negated(l); pbl.v = toVeriPbVar(variable(l)); return pbl;}

// Cadical's solvertypes:
inline int variable(int l){return abs(l);};
inline bool is_negated(int l){return (l < 0);}
inline int neg(int l){return -l;}
inline int create_literal(int var, bool negated){return (negated ? -var:var);}

inline VeriPB::Var toVeriPbVar(int v){VeriPB::Var pbv; pbv.v= v; return pbv;}
inline VeriPB::Lit toVeriPbLit(int l){VeriPB::Lit pbl; pbl.negated = is_negated(l); pbl.v = toVeriPbVar(variable(l)); return pbl;}

/*******************
 * Functions for the VeriPB types. 
*/
inline VeriPB::Var variable(VeriPB::Lit l){return l.v;}
inline bool is_negated(VeriPB::Lit l){return l.negated;}
inline VeriPB::Lit neg(VeriPB::Lit l){VeriPB::Lit newl; newl.v = l.v; newl.negated = !l.negated; return newl;}
inline VeriPB::Lit create_literal(VeriPB::Var var, bool negated){VeriPB::Lit l; l.v = var; l.negated = negated; return l; }

inline VeriPB::Var toVeriPbVar(VeriPB::Var v){return v;}
inline VeriPB::Lit toVeriPbLit(VeriPB::Lit l){return l;}

#endif