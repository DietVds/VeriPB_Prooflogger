#ifndef VeriPB_SolverTypes_h
#define VeriPB_SolverTypes_h

// I define the veripb data structures as a struct instead of a typedef to have a clear type that is relied to the VeriPB datatypes.

namespace VeriPB {
    typedef int VarIdx; // The VarIdx type is used as an identifier for a variable that will be used as index in maps and vectors.

    struct Var{
        int v;
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

inline bool operator==(const VeriPB::Lit& lhs, const VeriPB::Lit& rhs){
    return lhs.v == rhs.v && lhs.negated == rhs.negated;
}

inline bool operator!=(const VeriPB::Lit& lhs, const VeriPB::Lit& rhs){
    return lhs.v != rhs.v || lhs.negated != rhs.negated;
}

#endif