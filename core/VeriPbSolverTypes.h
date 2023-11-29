#ifndef VeriPB_SolverTypes_h
#define VeriPB_SolverTypes_h

#include <cstdint>


// The veripb data structures are defined as a struct instead of a typedef to have a clear type that is relied to the VeriPB datatypes.

namespace VeriPB {
    typedef int VarIdx; // The VarIdx type is used as an identifier for a variable that will be used as index in maps and vectors.

    struct Var{
        uint32_t v;
    };
    struct Lit{
        Var v;
        bool negated;
    };
}

inline VeriPB::VarIdx varidx(VeriPB::Var var){return var.v;}

inline bool operator==(const VeriPB::Var& lhs, const VeriPB::Var& rhs){
	return lhs.v == rhs.v;
}

inline bool operator!=(const VeriPB::Var& lhs, const VeriPB::Var& rhs){
	return lhs.v != rhs.v;
}

inline bool operator< (const VeriPB::Var& lhs, const VeriPB::Var& rhs){
    return (lhs.v < rhs.v);
}

inline bool operator< (const VeriPB::Var lhs, const VeriPB::Var rhs){
    return (lhs.v < rhs.v);
}

inline bool operator==(const VeriPB::Lit& lhs, const VeriPB::Lit& rhs){
    return lhs.v == rhs.v && lhs.negated == rhs.negated;
}

inline bool operator!=(const VeriPB::Lit& lhs, const VeriPB::Lit& rhs){
    return lhs.v != rhs.v || lhs.negated != rhs.negated;
}

/**
 * Interface functions. These functions also need to be defined for the types in the solver.
 * 
*/

inline VeriPB::Var variable(VeriPB::Lit l){return l.v;}
inline bool is_negated(VeriPB::Lit l){return l.negated;}
inline VeriPB::Lit neg(VeriPB::Lit l){VeriPB::Lit newl; newl.v = l.v; newl.negated = !l.negated; return newl;}
inline VeriPB::Lit create_literal(VeriPB::Var var, bool negated){VeriPB::Lit l; l.v = var; l.negated = negated; return l; }

inline VeriPB::Var toVeriPbVar(VeriPB::Var v){return v;}
inline VeriPB::Lit toVeriPbLit(VeriPB::Lit l){return l;}

#endif