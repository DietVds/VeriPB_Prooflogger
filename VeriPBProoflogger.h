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

/**
 * TODOs
 * Rewrite redundance based for a map from variables to union of literals or boolean value.
 * -> Can use the std::variant (C++ 17).
 * Dominance rule
 * Check where the rewritten best solution is needed! Probably for using with QMaxSAT! Add this to MaxSATProoflogger.
 */

//=================================================================================================
// Prooflogger

typedef int constraintid;

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
    int best_objective_value = INT_MAX;
    constraintid best_solution_constraint; // Last model improvement constraint
    constraintid rewritten_best_solution_constraint = 0; // last rewritten model improvement constraint. 0 means that it hasn't been rewritten. 

    // Meaningful variable names
    std::map<VeriPB::Var, std::string> meaningful_names_store;

    // Variables to be rewritten by literals.
    std::map<VeriPB::Var,VeriPB::Lit> map_rewrite_var_by_literal;

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

    template<class TLit>
    void set_objective(const std::vector<TLit> &lits, const std::vector<int> &weights);

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
    template <class TLit>
    void write_PB_constraint(const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS);
    template <class TLit>
    void write_cardinality_constraint(const std::vector<TLit> &lits, const int RHS);
    template <class TLit>
    void write_clause(const std::vector<TLit> &clause);

    // ------------- Meaningful names -------------
    template <class TVar>
    void store_meaningful_name(const TVar &var, const std::string &name);
    template <class TVar>
    void delete_meaningful_name(const TVar &var);

    // ------------- Rewrite variables by literals -------------
    template <class TVar, class TLit>
    void rewrite_variable_by_literal(const TVar& var, const TLit& lit);

    // ------------- Rules for checking constraints -------------
    template <class TLit>
    void equals_rule(const constraintid constraint_id, const std::vector<TLit> &lits, const int RHS = 1);
    template <class TLit>
    void equals_rule(const constraintid constraint_id, const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS);

    template <class TLit>
    void check_last_constraint(const std::vector<TLit> &lits, const int RHS = 1);
    template <class TLit>
    void check_last_constraint(const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS);

    // ------------- Rules for optimisation -------------

    template <class TLit>
    int calculate_objective_value(const std::vector<TLit> &model);
    template <class TLit>
    constraintid log_solution(const std::vector<TLit> &model);
    template <class TLit>
    constraintid log_solution_with_check(const std::vector<TLit> &model);
    constraintid get_best_solution_constraint();
    constraintid get_rewritten_best_solution_constraint();
    void set_rewritten_best_solution_constraint(constraintid cxn);
    void delete_rewritten_best_solution_constraint();

    // ------------- Unchecked Assumptions -------------
    template <class TLit>
    constraintid unchecked_assumption(const std::vector<TLit> &lits, const int RHS = 1);
    template <class TLit>
    constraintid unchecked_assumption(const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS);

    // ------------- Reverse Unit Propagation -------------

    template <class TLit>
    constraintid rup(const std::vector<TLit> &lits, const int RHS = 1);
    template <class TLit>
    constraintid rup(const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS);

    // ------------- Redundance Based Strenghtening -------------
    template <class TLit>
    void write_witness(const TLit &literal);
    template <class TLit>
    void write_witness(const std::vector<TLit> &witness);

    template <class TLit>
    constraintid redundanceBasedStrengthening(const std::vector<TLit> &lits, const int RHS, const TLit &witness);
    template <class TLit>
    constraintid redundanceBasedStrengthening(const std::vector<TLit> &lits, const int RHS, const std::vector<TLit> &witness);
    template <class TLit>
    constraintid redundanceBasedStrengthening(const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS, const TLit &witness);
    template <class TLit>
    constraintid redundanceBasedStrengthening(const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS, const std::vector<TLit> &witness);

    // constraintid redundanceBasedStrengthening(const std::vector<Lit>& lits, const int RHS, const std::map<Lit, std::variant<Lit, bool>>& witness);
    // constraintid redundanceBasedStrengthening(const std::vector<Lit>& lits, const std::vector<int>& weights, const int RHS, const std::vector<Lit>& witness);

    // ------------- Cutting Planes derivations -------------
    void start_CP_derivation(const constraintid constraint_id);
    template <class TLit>
    void start_CP_derivation_with_lit_axiom(const TLit &lit);
    void CP_load_constraint(const constraintid constraint_id);
    void CP_add();
    void CP_add_constraint(const constraintid constraint_id);
    void CP_divide(const int v);
    void CP_saturate();
    void CP_multiply(const int v);
    template <class TVar>
    void CP_weakening(const TVar &var);
    template <class TLit>
    void CP_literal_axiom(const TLit &lit);
    constraintid end_CP_derivation();

    // ------------- Deleting & Overwriting Constraints -------------
    template <class TLit>
    void delete_constraint(const constraintid constraint_id);
    template <class TLit>
    void delete_constraint(const std::vector<constraintid> &constraint_ids);
    template <class TLit>
    void delete_constraint(const std::vector<TLit> &lits, const int RHS = 1);
    template <class TLit>
    void delete_constraint(const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS);

    template <class TLit>
    constraintid overwrite_constraint(const constraintid constraint_id, const std::vector<TLit> &lits, const int RHS = 1);
    template <class TLit>
    constraintid overwrite_constraint(const std::vector<TLit> &lits_orig, const int RHS_orig, const std::vector<TLit> &lits, const int RHS);
    template <class TLit>
    constraintid overwrite_constraint(const constraintid constraint_id, const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS);
    template <class TLit>
    constraintid overwrite_constraint(const std::vector<TLit> &lits_orig, const std::vector<int> &weights_orig, const int RHS_orig, const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS);
    template <class TLit>
    constraintid overwrite_constraint(const std::vector<TLit> &lits_orig, const std::vector<TLit> &lits);

    // ------------- Handling contradiction -------------
    void write_previous_constraint_contradiction();
    void rup_empty_clause();

    /****************************************
     * For MINISAT compatibility
     ****************************************/

    void write_clause(Glucose30::Clause &clause);
    void write_clause(Minisat::Clause &clause);
    template <template <class T> class TVec, class TLit>
    void write_clause(TVec<TLit> &clause);
    template <class TLBool>
    void write_literal_assignment(TLBool assignment, int var);
    template <template <class T> class TVec, class TLBool>
    constraintid log_solution(TVec<TLBool> &model);
    template <template <class T> class TVec, class TLit>
    constraintid rup(TVec<TLit> &clause);
    template <template <class T> class TVec, class TLit>
    void overwrite_constraint(TVec<TLit> &orig, TVec<TLit> &clause);
    void delete_constraint(Glucose30::Clause &clause);
    void delete_constraint(Minisat::Clause &clause);
    template <template <class T> class TVec, class TLit>
    void delete_constraint(TVec<TLit> &clause);
};

//=================================================================================================

#endif