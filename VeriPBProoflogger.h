#ifndef VeriPBProoflogger_h
#define VeriPBProoflogger_h

#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <set>

// NOTE! Should include definition for types Var, Lit and Clause
#include "SolverTypesInt.h"

/******************
 * Posssible template classes:
 * TLit = Type representing a literal
 * TVar = Type representing a variable
 * TSeqLit = A Sequence Container containing literals. 
 *        This means that if lits is of the type TSeqLit, it is possible to retrieve the i'th lit by lits[i].
 *        Examples are std::vector<VeriPB::Lit>, Glucose::vec<Glucose::Lit>, Glucose::Clause, ...
 *        Also necessary: size() function.
 * TSeqInt = A Sequence Container containing Integers. 
 * TSeqLBool = A sequence container containing Minisat's (and Glucose's) lbool's.
 *******************/

//prooflogging Library

/**
 * TODOs
 * Rewrite redundance based for a map from variables to union of literals or boolean value.
 * -> Can use the std::variant (C++ 17).
 * Dominance rule
 */

//=================================================================================================
// Prooflogger

typedef int constraintid;

template<class TVar>
using substitution = std::vector<std::pair<TVar, bool>>;

class VeriPbProofLogger
{
private:
    // Formula information
    // Only used for the variable naming scheme.
    // Variables in the input starts with x,
    // variables created after parsing input starts with y (except for variables having a meaningful name).
    int n_variables = 0;

    // Objective function
    //
    std::vector<VeriPB::Lit> objective_lits;
    std::vector<int> objective_weights;
    int objective_constant_cost = 0;
    int best_objective_value = INT_MAX;
    constraintid best_solution_constraint; // Last model improvement constraint
    constraintid rewritten_best_solution_constraint = 0; // last rewritten model improvement constraint. 0 means that it hasn't been rewritten. 

    // Meaningful variable names
    std::map<VeriPB::VarIdx, std::string> meaningful_names_store;

    // Variables to be rewritten by literals.
    std::map<VeriPB::VarIdx, VeriPB::Lit> map_rewrite_var_by_literal;

    // Constraint counter
    //
    constraintid constraint_counter = 0;

    // Temporary string for reverse polish derivation
    //
    std::stringstream pol_string;

public:

    // Proof file
    std::ofstream proof;
    std::string proof_file = "maxsat_proof.pbp";
    void start_proof(const std::string name, int nbclause, int nbvars);
    void start_proof(const std::string name, int nbvars);
    void init_proof_file(const std::string name);
    void end_proof();
    void write_proof_header(int nbclause, int nbvars);
    void write_proof_header(int nbclause);
    void increase_n_variables();

    // Objective Function 
    template<class TSeqLit, class TSeqInt>
    void set_objective(const TSeqLit &lits, const TSeqInt &weights, int constant_cost);
    template <class TLit> 
    void add_objective_literal(TLit lit, int weight);
    void add_objective_constant(int weight);
    void write_comment_objective_function();

    // ------------- Helping functions -------------
    void write_comment(const char *comment);
    void write_comment(const std::string &comment);
    template <class TVar>
    bool is_aux_var(const TVar &var);
    template <class TVar>
    std::string var_name(const TVar &var);

    template <class TLit>
    void write_weighted_literal(const TLit &literal, int weight = 1);
    template <class TLit>
    std::string to_string(const TLit &lit);
    template <class TLit>
    void write_literal(const TLit &lit);
    template <class TSeqLit, class TSeqInt>
    void write_PB_constraint(const TSeqLit &lits, const TSeqInt &weights, const int RHS);
    template <class TSeqLit>
    void write_cardinality_constraint(const TSeqLit &lits, const int RHS);
    template <class TSeqLit>
    void write_clause(const TSeqLit& clause);


    // ------------- Meaningful names -------------
    template <class TVar>
    void store_meaningful_name(const TVar &var, const std::string &name);
    template <class TVar>
    void delete_meaningful_name(const TVar &var);

    // ------------- Rewrite variables by literals -------------
    template <class TVar, class TLit>
    void rewrite_variable_by_literal(const TVar& var, const TLit& lit);

    // ------------- Rules for checking constraints -------------
    template <class TSeqLit>
    void equals_rule(const constraintid constraint_id, const TSeqLit &lits, const int RHS = 1);
    template <class TSeqLit, class TSeqInt>
    void equals_rule(const constraintid constraint_id, const TSeqLit &lits, const TSeqInt &weights, const int RHS);

    template <class TSeqLit>
    void check_last_constraint(const TSeqLit &lits, const int RHS = 1);
    template <class TSeqLit, class TSeqInt>
    void check_last_constraint(const TSeqLit &lits, const TSeqInt &weights, const int RHS);

    // ------------- Rules for optimisation -------------

    template <class TSeqLit>
    int calculate_objective_value(const TSeqLit &model);
    template <class TSeqLit>
    constraintid log_solution(const TSeqLit &model);
    template <class TSeqLit>
    constraintid log_solution_with_check(const TSeqLit &model);
    constraintid get_best_solution_constraint();
    constraintid get_rewritten_best_solution_constraint();
    void set_rewritten_best_solution_constraint(constraintid cxn);
    void delete_rewritten_best_solution_constraint();

    template <class TSeqLBool>
    constraintid log_solution_lbools(TSeqLBool &model);

    // TODO: write calculate_objective_value and log_solution_with_check for TSeqLBool 

    // ------------- Unchecked Assumptions -------------
    template <class TSeqLit>
    constraintid unchecked_assumption(const TSeqLit &lits, const int RHS = 1);
    template <class TSeqLit, class TSeqInt>
    constraintid unchecked_assumption(const TSeqLit &lits, const TSeqInt &weights, const int RHS);

    
    // ------------- Reverse Unit Propagation -------------

    template <class TSeqLit>
    constraintid rup(const TSeqLit &lits, const int RHS = 1);
    template <class TSeqLit, class TSeqInt>
    constraintid rup(const TSeqLit &lits, const TSeqInt &weights, const int RHS);

    // ------------- Redundance Based Strenghtening -------------
    template <class TVar>
    void write_witness(const substitution<TVar> &witness);

    template <class TSeqLit, class TVar>
    constraintid redundanceBasedStrengthening(const TSeqLit &lits, const int RHS, const substitution<TVar> &witness);
    template <class TSeqLit, class TSeqInt, class TVar>
    constraintid redundanceBasedStrengthening(const TSeqLit &lits, const TSeqInt &weights, const int RHS, const substitution<TVar> &witness);

    // ------------- Reification Variables -------------
    // Proves the constraints encoding the reification constraint l <-> C, with l a literal and C a boolean constraint.
    // The right implication is the encoding of l -> C, whereas the left implication means l <- C.
    std::map<VeriPB::VarIdx, constraintid> reifiedConstraintLeftImpl;
    std::map<VeriPB::VarIdx, constraintid> reifiedConstraintRightImpl;

    template <class TSeqLit, class TSeqInt, class TLit>
    constraintid reificationLiteralRightImpl(const TLit& lit, const TSeqLit &litsC, const TSeqInt &weights, const int RHS, bool store_reified_constraint=false);
    template <class TSeqLit, class TLit>
    constraintid reificationLiteralRightImpl(const TLit& lit, const TSeqLit &litsC, const int RHS, bool store_reified_constraint=false);

    template <class TSeqLit, class TSeqInt, class TLit>
    constraintid reificationLiteralLeftImpl(const TLit& lit, const TSeqLit &litsC, const TSeqInt &weights, const int RHS, bool store_reified_constraint=false);
    template <class TSeqLit, class TLit>
    constraintid reificationLiteralLeftImpl(const TLit& lit, const TSeqLit &litsC, const int RHS, bool store_reified_constraint=false);

    template <class TVar>
    constraintid getReifiedConstraintLeftImpl(const TVar& var);
    template <class TVar>
    constraintid getReifiedConstraintRightImpl(const TVar& var);

    // ------------- Cutting Planes derivations -------------
    void start_CP_derivation(const constraintid constraint_id);
    template <class TLit>
    void start_CP_derivation_with_lit_axiom(const TLit &lit);
    void CP_load_constraint(const constraintid constraint_id);
    void CP_add();
    void CP_add_constraint(const constraintid constraint_id);
    template <class TLit>
    void CP_add_literal_axiom(const TLit &lit);
    void CP_divide(const int v);
    void CP_saturate();
    void CP_multiply(const int v);
    template <class TVar>
    void CP_weakening(const TVar &var);
    template <class TLit>
    void CP_literal_axiom(const TLit &lit);
    constraintid end_CP_derivation();

    // ------------- Deleting & Overwriting Constraints -------------
    void delete_constraint(const constraintid constraint_id);
    void delete_constraint(const std::vector<constraintid> &constraint_ids);
    template <class TSeqLit>
    void delete_constraint(const TSeqLit &lits, const int RHS);
    template <class TSeqLit, class TSeqInt>
    void delete_constraint(const TSeqLit &lits, const TSeqInt &weights, const int RHS);

    //Minisat:
    void delete_constraint(Glucose::Clause &clause);
    template <template <class T> class TVec, class TLit>
    void delete_constraint(TVec<TLit> &clause);

    template <class TSeqLit>
    constraintid overwrite_constraint(const constraintid constraint_id, const TSeqLit &lits, const int RHS = 1);
    template <class TSeqLit>
    constraintid overwrite_constraint(const TSeqLit &lits_orig, const int RHS_orig, const TSeqLit &lits, const int RHS);
    template <class TSeqLit, class TSeqInt>
    constraintid overwrite_constraint(const constraintid constraint_id, const TSeqLit &lits, const TSeqInt &weights, const int RHS);
    template <class TSeqLit, class TSeqInt>
    constraintid overwrite_constraint(const TSeqLit &lits_orig, const TSeqInt &weights_orig, const int RHS_orig, const TSeqLit &lits, const TSeqInt &weights, const int RHS);
    template <class TSeqLit>
    constraintid overwrite_constraint(const TSeqLit &lits_orig, const TSeqLit &lits);

    // ------------- Handling contradiction -------------
    void write_previous_constraint_contradiction();
    void rup_empty_clause();  
    

};

//=================================================================================================

#endif
