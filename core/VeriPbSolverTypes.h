#ifndef VeriPB_SolverTypes_h
#define VeriPB_SolverTypes_h

#include <cinttypes>
#include <vector>
#include <string>

#ifndef PL_VarIdx
#define PL_VarIdx uint32_t
#endif

#ifndef PL_litindex
#define PL_litindex size_t
#endif

#ifndef PL_constraintid
#define PL_constraintid int64_t
#endif

namespace VeriPB {

typedef PL_VarIdx VarIdx;
typedef PL_litindex litindex;
typedef PL_constraintid constraintid;

struct Var{
    VarIdx v;
    bool only_known_in_proof = false;
};
static Var var_undef {.v=0, .only_known_in_proof=false};

struct Lit{
    Var v;
    bool negated=false;
};
static Lit lit_undef {.v=var_undef, .negated=false};

template <typename TLit, typename TCoeff, typename TConst>
class LinTermBoolVars {
    public:
        TLit literal(const litindex& idx);
        TCoeff coefficient(const litindex& idx);
        TConst constant();
        TConst sum_of_coefficients();
        size_t size();

        void add_literal(const TLit& lit, const TCoeff& coeff=1);
        bool delete_literal(const TLit& lit);
        bool delete_literal(const litindex& index);
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
        TConst _sum_coeffs;

        bool _all_coeff_one;
        bool _owned;
};

enum Comparison {GEQ, LEQ};

template <typename TLit, typename TCoeff, typename TRhs>
class Constraint {
    public:
        TLit literal(const litindex& idx);
        TCoeff coefficient(const litindex& idx);
        TRhs rhs();
        TRhs sum_of_coefficients();
        Comparison comparison();
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

/*******************
 * Functions for variables and literals 
 * - need to be specialized. Already specialized for VeriPB::Var and VeriPB::Lit.
*/
template <class TVar, class TLit>
TVar variable(const TLit&);

template <typename TLit>
bool is_negated(const TLit&);

template <typename TLit>
TLit neg(const TLit&);

template <typename TLit, typename TVar>
TLit create_literal(const TVar&, bool);

template <typename TVar>
VeriPB::Var toVeriPbVar(const TVar&);

template <typename TLit>
VeriPB::Lit toVeriPbLit(const TLit&);

template <typename TVar>
VeriPB::VarIdx varidx(const TVar&);

/********************
 * Functions for linear terms
 * - need to be specialized
 */

template <typename TLinTerm, typename TLit>
TLit literal(const TLinTerm&, const litindex&);

template <typename TLinTerm, typename TCoeff>
TCoeff coefficient(const TLinTerm&, const VeriPB::litindex&);

template <typename TLinTerm, typename TCoeff>
TCoeff get_constant(const TLinTerm&);

template <typename TLinTerm, typename TConst>
TConst sum_of_coefficients(const TLinTerm&);

/********************
 * Functions for VeriPB linear terms
 * - can be immediately instantiated
 */
template <typename TLit, typename TCoeff, typename TConst>
TLit literal(const VeriPB::LinTermBoolVars<TLit, TCoeff, TConst>&, const VeriPB::litindex&);

template <typename TLit, typename TCoeff, typename TConst>
TCoeff coefficient(const VeriPB::LinTermBoolVars<TLit, TCoeff, TConst>&, const VeriPB::litindex&);

template <typename TLit, typename TCoeff, typename TConst>
TCoeff get_constant(const VeriPB::LinTermBoolVars<TLit, TCoeff, TConst>&);

template <typename TLit, typename TCoeff, typename TConst>
TConst sum_of_coefficients(const VeriPB::LinTermBoolVars<TLit, TCoeff, TConst>&);


/*******************
 * Functions for constraints:
 * - need to be specialized
*/
template <typename TLit, typename TConstraint> 
TLit literal(const TConstraint&, const litindex&);

template <typename TCoeff, typename TConstraint> 
TCoeff coefficient(const TConstraint&, const litindex&);

template <typename TConstraint> 
Comparison comparison(const TConstraint&);

template <typename TConstraint, typename TRhs> 
TRhs rhs(const TConstraint&);

template <typename TConstraint>
size_t size(const TConstraint& cxn);

template <typename TConstraint, typename TRhs>
TRhs sum_of_coefficients(const TConstraint& cxn);

/*******************
 * Functions for VeriPB constraints:
 * - can be immediately instantiated
*/
template <typename TLit, typename TCoeff, typename TRhs> 
TLit VeriPB::literal(const VeriPB::Constraint<TLit, TCoeff, TRhs>&, const VeriPB::litindex&);

template <typename TLit, typename TCoeff, typename TRhs> 
TCoeff VeriPB::coefficient(const VeriPB::Constraint<TLit, TCoeff, TRhs>&, const VeriPB::litindex&);

template <typename TLit, typename TCoeff, typename TRhs> 
VeriPB::Comparison comparison(const VeriPB::Constraint<TLit, TCoeff, TRhs>&);

template <typename TLit, typename TCoeff, typename TRhs> 
TRhs rhs(const VeriPB::Constraint<TLit, TCoeff, TRhs>&);

template <typename TLit, typename TCoeff, typename TRhs>
size_t size(const VeriPB::Constraint<TLit, TCoeff, TRhs>&);

template <typename TLit, typename TCoeff, typename TRhs>
TRhs sum_of_coefficients(const VeriPB::Constraint<TLit, TCoeff, TRhs>&);

/*******************
 * Functions to deal with numbers
 * - These functions can be immediately instantiated for standard C++ datatypes (except for numbers represented with the boost library). 
 */
template <typename TNumber>
void write_number(const TNumber&, const bool add_prefix_space=true);

template <typename TNumber>
std::string number_to_string(const TNumber&);

/*******************
 * Functions for models. 
 * - model_literal and model_size can be immediately instantiated for representations of models as a sequence of literals (i.e., where it works to write model[i] and model.size())
 * - model_value can be immediately instantiated for using the optimistic assumption that model_value will be called multiple times for variables that are placed in an increasing index. 
 */

enum ModelValue {False, True, Undef};

template <typename TModel>
Lit model_literal(const litindex&, const TModel&);

template <typename TModel>
size_t model_size(const TModel& model);

template <typename TVar, typename TModel>
ModelValue model_value(const TVar& var, const TModel& model, bool first_call=false);

}

bool operator==(const VeriPB::Var& lhs, const VeriPB::Var& rhs);
bool operator!=(const VeriPB::Var& lhs, const VeriPB::Var& rhs);
bool operator< (const VeriPB::Var& lhs, const VeriPB::Var& rhs);
bool operator==(const VeriPB::Lit& lhs, const VeriPB::Lit& rhs);
bool operator< (const VeriPB::Lit& lhs, const VeriPB::Lit& rhs);

#endif