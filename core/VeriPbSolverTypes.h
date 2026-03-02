#ifndef VeriPB_SolverTypes_h
#define VeriPB_SolverTypes_h

#include <cinttypes>
#include <vector>
#include <string>
#include <cassert>

#ifndef PL_VarIdx
#define PL_VarIdx size_t
#ifndef PL_litindex
#define NOINIT_LITINDEX // If PL_litindex is not yet defined, it will get the same type.
#endif
#endif

#ifndef PL_litindex
#define PL_litindex size_t
#endif

#ifndef PL_constraintid
#define PL_constraintid int64_t
#endif

#ifndef PL_defaultmult
#define PL_defaultmultipliertype uint32_t
#endif

/**
 * TODO:
 * - Conversion from TCoeff to TConst (also in Prooflogger)
 */

namespace VeriPB {

typedef PL_VarIdx VarIdx;
typedef PL_litindex litindex;
typedef PL_constraintid constraintid;
typedef PL_defaultmultipliertype defaultmultipliertype;

struct Var{
    VarIdx v;
    bool only_known_in_proof = false;
};
static Var var_undef {.v=0, .only_known_in_proof=false};
bool operator==(const Var& lhs, const Var& rhs);
bool operator!=(const Var& lhs, const Var& rhs);
bool operator< (const Var& lhs, const Var& rhs);
bool operator<=(const Var& lhs, const Var& rhs);
bool operator> (const Var& lhs, const Var& rhs);
bool operator>=(const Var& lhs, const Var& rhs);

struct Lit{
    Var v;
    bool negated=false;
};
static Lit lit_undef {.v=var_undef, .negated=false};
bool operator==(const Lit& lhs, const Lit& rhs);
bool operator!=(const Lit& lhs, const Lit& rhs);
bool operator< (const Lit& lhs, const Lit& rhs);
bool operator<=(const Lit& lhs, const Lit& rhs);
bool operator> (const Lit& lhs, const Lit& rhs);
bool operator>=(const Lit& lhs, const Lit& rhs);

template <typename TLit, typename TCoeff, typename TConst>
class LinTermBoolVars {
    public:
        TLit literal(const litindex& idx) const;
        TCoeff coefficient(const litindex& idx) const;
        TConst constant() const;
        TConst max_val() const;
        TConst min_val() const;
        size_t size() const;

        void add_literal(const TLit& lit, const TCoeff& coeff=1);
        bool delete_literal(const TLit& lit);
        bool delete_literal(const litindex& index);
        void add_constant(const TConst& constant);
        void subtract_constant(const TConst& constant);
        void clear(const bool all_coeff_one=false, const TConst& new_const=0 );

        LinTermBoolVars(const bool all_coeff_one = false);
        LinTermBoolVars(std::vector<TLit>* lits, std::vector<TCoeff>* coeff, TConst constant = 0);
        LinTermBoolVars(std::vector<TLit>* lits, TConst constant = 0);

        ~LinTermBoolVars();

        LinTermBoolVars(const LinTermBoolVars&);
        LinTermBoolVars& operator=(const LinTermBoolVars&);
        LinTermBoolVars(LinTermBoolVars&&);
        LinTermBoolVars& operator=(LinTermBoolVars&&);
    
    private: 
        std::vector<TLit>* _literals;
        std::vector<TCoeff>* _coefficients;
        TConst _constant;
        TConst _max_val;
        TConst _min_val;

        bool _all_coeff_one;
        bool _owned;
};

enum Comparison {GEQ, LEQ};
inline std::string to_string(const Comparison& comp){
    if(comp == Comparison::GEQ) return ">=";
    else if(comp == Comparison::LEQ) return "<=";
    else assert(false); 
}

template <typename TLit, typename TCoeff, typename TRhs>
class Constraint {
    public:
        TLit literal(const litindex& idx) const;
        TCoeff coefficient(const litindex& idx) const;
        TRhs rhs() const;
        TRhs max_val_lhs() const;
        TRhs min_val_lhs() const;
        Comparison comparison() const;
        size_t size() const;

        void add_literal(const TLit& lit, const TCoeff& coeff=1);
        void add_RHS(const TRhs& rhs_to_add);
        void subtract_RHS(const TRhs& rhs_to_subtract);
        void clear(const bool cardinality_constraint=false, const TRhs& new_RHS=0, const Comparison& new_comparison = Comparison::GEQ );
        void set_comp(const Comparison);

        Constraint(const bool cardinality_constraint=false, const TRhs& rhs=0, enum Comparison comp = Comparison::GEQ);
        Constraint(LinTermBoolVars<TLit, TCoeff, TRhs>* term, TRhs rhs, enum Comparison comp = Comparison::GEQ);
        Constraint(std::vector<TLit>* lits, std::vector<TCoeff>* coeff, TRhs rhs, enum Comparison comp = Comparison::GEQ);
        Constraint(std::vector<TLit>* lits, TRhs rhs=1, enum Comparison comp = Comparison::GEQ);

        Constraint(const Constraint&);
        Constraint& operator=(const Constraint&);

        ~Constraint();
    protected:
        LinTermBoolVars<TLit, TCoeff, TRhs>* _linterm;
        TRhs _rhs;
        enum Comparison _comp;
        bool _owned;
        TCoeff _coeff_left_reif;
        TCoeff _coeff_right_reif;
};

template <typename TLit>
class Clause : public Constraint<TLit, uint8_t, uint8_t> {
    public:
        void clear();
        Clause();
        ~Clause();
};

/*******************
 * Functions for VeriPB variables and literals 
*/
Var variable(const Lit& l);
bool is_negated(const Lit& l);
Lit neg(const Lit& l);
Lit create_literal(const Var& var, bool negated);

Var toVeriPbVar(const Var& v);
Lit toVeriPbLit(const Lit& l);

VarIdx varidx(const Var& var);


/********************
 * Functions for VeriPB linear terms
 * - can be immediately instantiated
 */
template <typename TLit, typename TCoeff, typename TConst>
TLit literal(const LinTermBoolVars<TLit, TCoeff, TConst>&, const litindex&);

template <typename TLit, typename TCoeff, typename TConst>
TCoeff coefficient(const LinTermBoolVars<TLit, TCoeff, TConst>&, const litindex&);

template <typename TLit, typename TCoeff, typename TConst>
TCoeff get_constant(const LinTermBoolVars<TLit, TCoeff, TConst>&);

template <typename TLit, typename TCoeff, typename TConst>
TConst max_val(const LinTermBoolVars<TLit, TCoeff, TConst>&);

template <typename TLit, typename TCoeff, typename TConst>
TConst min_val(const LinTermBoolVars<TLit, TCoeff, TConst>&);

template <typename TLit, typename TCoeff, typename TConst>
size_t size(const LinTermBoolVars<TLit, TCoeff, TConst>&);

/*******************
 * Functions for VeriPB constraints:
 * - can be immediately instantiated
*/
template <typename TLit, typename TCoeff, typename TRhs> 
TLit literal(const Constraint<TLit, TCoeff, TRhs>&, const litindex&);

template <typename TLit, typename TCoeff, typename TRhs> 
TCoeff coefficient(const Constraint<TLit, TCoeff, TRhs>&, const litindex&);

template <typename TLit, typename TCoeff, typename TRhs> 
Comparison comparison(const Constraint<TLit, TCoeff, TRhs>&);

template <typename TLit, typename TCoeff, typename TRhs> 
TRhs rhs(const Constraint<TLit, TCoeff, TRhs>&);

template <typename TLit, typename TCoeff, typename TRhs>
size_t size(const Constraint<TLit, TCoeff, TRhs>&);

template <typename TLit, typename TCoeff, typename TRhs>
TRhs max_val_lhs(const Constraint<TLit, TCoeff, TRhs>&);

template <typename TLit, typename TCoeff, typename TRhs>
TRhs min_val_lhs(const Constraint<TLit, TCoeff, TRhs>&);



/***************
 * Functions for reifying constraints: 
 * - Can be immediately instantiated for any constraints for which the following functions are implemented: max_val_lhs, min_val_lhs, comparison and rhs.
 */

template <typename TRhs, typename TConstraint>
TRhs coeff_left_reif(const TConstraint&);
template <typename TRhs, typename TConstraint>
TRhs coeff_right_reif(const TConstraint&);


/*******************
 * Functions to deal with numbers
 * - These functions can be immediately instantiated for standard C++ datatypes (except for specific number types, such as for example represented with the boost library). 
 */
template <typename TNumber>
void write_number(const TNumber&, std::ostream*, const bool add_prefix_space=true);

template <typename TNumber>
std::string number_to_string(const TNumber&);

#ifndef NONUMBERCONVERSION
template <typename TNumber1, typename TNumber2>
TNumber2 convert_number(const TNumber1&);
#endif

/*******************
 * Functions for models. 
 * - model_literal and model_size can be immediately instantiated for representations of models as a sequence of literals (i.e., where it works to write model[i] and model.size())
 * - model_value can be immediately instantiated for using the optimistic assumption that model_value will be called multiple times for variables that are placed in an increasing index. 
 */

enum ModelValue {False, True, Undef};

template <typename TModel>
Lit model_literal(const TModel&, const litindex&);

template <typename TModel>
size_t model_size(const TModel& model);

template <typename TVar, typename TModel>
ModelValue model_value(const TVar& var, const TModel& model, bool first_call=false);

}

#endif