#ifndef VeriPB_SolverTypes_h
#define VeriPB_SolverTypes_h

#include <cinttypes>


// The veripb data structures are defined as a struct instead of a typedef to have a clear type that is relied to the VeriPB datatypes.

namespace VeriPB {
    typedef uint32_t VarIdx; // The VarIdx type is used as an identifier for a variable that will be used as index in maps and vectors.

    struct Var{
        VarIdx v;
        bool only_known_in_proof = false;
    };
    struct Lit{
        Var v;
        bool negated=false;
    };
    
    static Var var_undef {.v=0, .only_known_in_proof=false};
    static Lit lit_undef {.v=var_undef, .negated=false};
}

inline bool operator==(const VeriPB::Var& lhs, const VeriPB::Var& rhs){
	return lhs.v == rhs.v && lhs.only_known_in_proof == rhs.only_known_in_proof;
}

inline bool operator!=(const VeriPB::Var& lhs, const VeriPB::Var& rhs){
	return !(lhs == rhs);
}

inline bool operator< (const VeriPB::Var& lhs, const VeriPB::Var& rhs){
    return (!lhs.only_known_in_proof && rhs.only_known_in_proof) 
                || (lhs.only_known_in_proof == rhs.only_known_in_proof && lhs.v < rhs.v);
}

inline bool operator==(const VeriPB::Lit& lhs, const VeriPB::Lit& rhs){
    return lhs.v == rhs.v && lhs.negated == rhs.negated;
}

inline bool operator!=(const VeriPB::Lit& lhs, const VeriPB::Lit& rhs){
    return !(lhs == rhs);
}

inline bool operator< (const VeriPB::Lit& lhs, const VeriPB::Lit& rhs){
    return lhs.v < rhs.v || (lhs.v == rhs.v && lhs.negated && !rhs.negated);
}

/*******************
 * Functions for the VeriPB types. 
*/

//TODO: add default variable function to make variable naming faster.

inline VeriPB::Var variable(VeriPB::Lit l){return l.v;}
inline bool is_negated(VeriPB::Lit l){return l.negated;}
inline VeriPB::Lit neg(VeriPB::Lit l){VeriPB::Lit newl; newl.v = l.v; newl.negated = !l.negated; return newl;}
inline VeriPB::Lit create_literal(VeriPB::Var var, bool negated){VeriPB::Lit l; l.v = var; l.negated = negated; return l; }

inline VeriPB::Var toVeriPbVar(VeriPB::Var v){return v;}
inline VeriPB::Lit toVeriPbLit(VeriPB::Lit l){return l;}

inline VeriPB::VarIdx varidx(VeriPB::Var var){return var.v << 1 ^ var.only_known_in_proof;}

#endif