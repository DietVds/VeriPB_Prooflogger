#ifndef VeriPB_SolverTypes_h
#define VeriPB_SolverTypes_h

#include <cinttypes>
#include <vector>

// The veripb data structures are defined as a struct instead of a typedef to have a clear type that is relied to the VeriPB datatypes.
typedef int litindex ;



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

    template <typename TLit, typename TCoeff, typename TConst>
    class LinTermBoolVars {
        public:
            TLit get_literal(const litindex& idx);
            TCoeff get_coefficient(const litindex& idx);
            TConst get_constant();
            size_t size();

            void add_literal(const TLit& lit, const TCoeff& coeff=1);
            void add_constant(const TConst& constant);
            void subtract_constant(const TConst& constant);

            LinTermBoolVars(const bool all_coeff_one = false);
            LinTermBoolVars(std::vector<TLit>* lits, std::vector<TCoeff>* coeff, TConst& constant = 0);
            LinTermBoolVars(std::vector<TLit>* lits, TConst& constant = 0);

            ~LinTermBoolVars();
        
        private: 
            std::vector<TLit>* _literals;
            std::vector<TCoeff>* _coefficients;
            TConst _constant;

            bool _all_coeff_one;
            bool _owned;
    };

    enum Comparison {GEQ, LEQ};

    template <typename TLit, typename TCoeff, typename TRhs>
    class Constraint {
        public:
            TLit get_literal(const litindex& idx);
            TCoeff get_coefficient(const litindex& idx);
            TRhs get_RHS();
            Comparison get_comparison();
            size_t size();

            void add_literal(const TLit& lit, const TCoeff& coeff=1);
            void add_RHS(const TRhs& rhs_to_add);
            void subtract_RHS(const TRhs& rhs_to_subtract);

            Constraint(const bool cardinality_constraint=false, enum Comparison comp = Comparison::GEQ);
            Constraint(LinTermBoolVars<TLit, TCoeff, TRhs>* term, TRhs rhs, enum Comparison comp = Comparison::GEQ);
            Constraint(std::vector<TLit>* lits, std::vector<TCoeff>* coeff, TRhs rhs, enum Comparison comp = Comparison::GEQ);
            Constraint(std::vector<TLit>* lits, TRhs rhs=1, enum Comparison comp = Comparison::GEQ);

            ~Constraint();
        private:
            LinTermBoolVars<TLit, TCoeff, TRhs>* _linterm;
            TRhs _rhs;
            enum Comparison _comp;
            bool _owned;
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

inline VeriPB::Var variable(VeriPB::Lit l){return l.v;}
inline bool is_negated(VeriPB::Lit l){return l.negated;}
inline VeriPB::Lit neg(VeriPB::Lit l){VeriPB::Lit newl; newl.v = l.v; newl.negated = !l.negated; return newl;}
inline VeriPB::Lit create_literal(VeriPB::Var var, bool negated){VeriPB::Lit l; l.v = var; l.negated = negated; return l; }

inline VeriPB::Var toVeriPbVar(VeriPB::Var v){return v;}
inline VeriPB::Lit toVeriPbLit(VeriPB::Lit l){return l;}

inline VeriPB::VarIdx varidx(VeriPB::Var var){return var.v << 1 ^ var.only_known_in_proof;}

// Functions necessary for all types of constraints:
template <typename TLit, typename TCoeff, typename TRhs> 
inline TLit get_literal(const VeriPB::Constraint<TLit, TCoeff, TRhs>& cxn, const litindex& index){ 
    return cxn.get_literal(index);
}

template <typename TLit, typename TCoeff, typename TRhs> 
inline TCoeff get_coefficient(const VeriPB::Constraint<TLit, TCoeff, TRhs>& cxn, const litindex& index){ 
    return cxn.get_coefficient(index);
}

template <typename TLit, typename TCoeff, typename TRhs> 
inline VeriPB::Comparison get_comparison(const VeriPB::Constraint<TLit, TCoeff, TRhs>& cxn){
    return cxn.get_comparison();
}

template <typename TLit, typename TCoeff, typename TRhs> 
inline TRhs rhs(const VeriPB::Constraint<TLit, TCoeff, TRhs>& cxn){
    return cxn.get_RHS(cxn);
}

template <typename TLit, typename TCoeff, typename TRhs>
inline size_t size(const VeriPB::Constraint<TLit, TCoeff, TRhs>& cxn){
    return cxn.size();
}


/**
 * Implementation for Linear Terms
 */

template <typename TLit, typename TCoeff, typename TConst>
inline TLit VeriPB::LinTermBoolVars<TLit, TCoeff, TConst>::get_literal(const litindex& idx){
    return _literals->at(idx);
}

template <typename TLit, typename TCoeff, typename TConst>
inline TCoeff VeriPB::LinTermBoolVars<TLit, TCoeff, TConst>::get_coefficient(const litindex& idx){
    if(_all_coeff_one)
        return 1;
    else
        return _coefficients->at(idx);
}

template <typename TLit, typename TCoeff, typename TConst>
inline TConst VeriPB::LinTermBoolVars<TLit, TCoeff, TConst>::get_constant(){
    return _constant;
}

template <typename TLit, typename TCoeff, typename TConst>
inline size_t VeriPB::LinTermBoolVars<TLit, TCoeff, TConst>::size(){
    return _literals->size();
}

template <typename TLit, typename TCoeff, typename TConst>
inline void VeriPB::LinTermBoolVars<TLit, TCoeff, TConst>::add_literal(const TLit& lit, const TCoeff& coeff){
    assert(!(_all_coeff_one && coeff > 1));

    _literals->push_back(lit);
    if(!_all_coeff_one)
        _coefficients->push_back(coeff);
}
template <typename TLit, typename TCoeff, typename TConst>
inline void VeriPB::LinTermBoolVars<TLit, TCoeff, TConst>::add_constant(const TConst& constant){
    _constant += constant;
}
template <typename TLit, typename TCoeff, typename TConst>
inline void VeriPB::LinTermBoolVars<TLit, TCoeff, TConst>::subtract_constant(const TConst& constant){
    _constant -= constant;
}

template <typename TLit, typename TCoeff, typename TConst>
inline VeriPB::LinTermBoolVars<TLit, TCoeff, TConst>::LinTermBoolVars(const bool all_coeff_one) : 
    _literals(new std::vector<TLit>),
    _all_coeff_one(all_coeff_one),
    _constant(0),
    _owned(true)
{
    if(!all_coeff_one)
        _coefficients = new std::vector<TCoeff>;
}

template <typename TLit, typename TCoeff, typename TConst>
inline VeriPB::LinTermBoolVars<TLit, TCoeff, TConst>::LinTermBoolVars(std::vector<TLit>* lits, std::vector<TCoeff>* coeff, TConst& constant) :
    _literals(lits),
    _coefficients(coeff),
    _constant(constant),
    _all_coeff_one(false),
    _owned(false)
{ }

template <typename TLit, typename TCoeff, typename TConst>
inline VeriPB::LinTermBoolVars<TLit, TCoeff, TConst>::LinTermBoolVars(std::vector<TLit>* lits, TConst& constant) :
    _literals(lits),
    _coefficients(nullptr),
    _constant(constant),
    _all_coeff_one(true),
    _owned(false)
{ }

template <typename TLit, typename TCoeff, typename TConst>
inline VeriPB::LinTermBoolVars<TLit, TCoeff, TConst>::~LinTermBoolVars(){
    if(_owned)
        delete _coefficients;
        delete _literals;
}

/**
 * Implementation for Constraint
 */


template <typename TLit, typename TCoeff, typename TRhs>
inline TLit VeriPB::Constraint<TLit, TCoeff, TRhs>::get_literal(const litindex& idx){
    return _linterm->get_literal(idx);
}

template <typename TLit, typename TCoeff, typename TRhs>
inline TCoeff VeriPB::Constraint<TLit, TCoeff, TRhs>::get_coefficient(const litindex& idx){
    return _linterm->get_coefficient(idx);
}
template <typename TLit, typename TCoeff, typename TRhs>
inline TRhs VeriPB::Constraint<TLit, TCoeff, TRhs>::get_RHS(){
    assert(_linterm->get_constant() < RHS);

    return _rhs - _linterm->get_constant();
}
template <typename TLit, typename TCoeff, typename TRhs>
inline VeriPB::Comparison VeriPB::Constraint<TLit, TCoeff, TRhs>::get_comparison(){
    return _comp;
}

template <typename TLit, typename TCoeff, typename TRhs>
inline size_t VeriPB::Constraint<TLit, TCoeff, TRhs>::size(){
    return _linterm->size();
}

template <typename TLit, typename TCoeff, typename TRhs>
inline void VeriPB::Constraint<TLit, TCoeff, TRhs>::add_literal(const TLit& lit, const TCoeff& coeff){
    _linterm->add_literal(lit, coeff);
}
template <typename TLit, typename TCoeff, typename TRhs>
inline void VeriPB::Constraint<TLit, TCoeff, TRhs>::add_RHS(const TRhs& rhs_to_add){
    _rhs += rhs;
}
template <typename TLit, typename TCoeff, typename TRhs>
inline void VeriPB::Constraint<TLit, TCoeff, TRhs>::subtract_RHS(const TRhs& rhs_to_subtract){
    _rhs -= rhs;
}

template <typename TLit, typename TCoeff, typename TRhs>
inline VeriPB::Constraint<TLit, TCoeff, TRhs>::Constraint(const bool cardinality_constraint, enum Comparison comp) : 
    _linterm(new VeriPB::LinTermBoolVars<TLit, TCoeff, TRhs>(cardinality_constraint)),
    _rhs(0),
    _comp(comp),
    _owned(true)
{ }

template <typename TLit, typename TCoeff, typename TRhs>
inline VeriPB::Constraint<TLit, TCoeff, TRhs>::Constraint(LinTermBoolVars<TLit, TCoeff, TRhs>* term, TRhs rhs, enum Comparison comp) :
    _linterm(term),
    _rhs(rhs),
    _comp(comp),
    _owned(false)

{ }
template <typename TLit, typename TCoeff, typename TRhs>
inline VeriPB::Constraint<TLit, TCoeff, TRhs>::Constraint(std::vector<TLit>* lits, std::vector<TCoeff>* coeff, TRhs rhs, enum Comparison comp) :
    _linterm(new VeriPB::LinTermBoolVars<TLit, TCoeff, TRhs>(lits, coeff)),
    _rhs(rhs),
    _comp(comp),
    _owned(false)
{ }
template <typename TLit, typename TCoeff, typename TRhs>
inline VeriPB::Constraint<TLit, TCoeff, TRhs>::Constraint(std::vector<TLit>* lits, TRhs rhs, enum Comparison comp) :
    _linterm(new VeriPB::LinTermBoolVars<TLit, TCoeff, TRhs>(lits)),
    _rhs(rhs),
    _comp(comp),
    _owned(false)
{ }

template <typename TLit, typename TCoeff, typename TRhs>
inline VeriPB::Constraint<TLit, TCoeff, TRhs>::~Constraint(){
    if(_owned)
        delete _linterm;
}


#endif