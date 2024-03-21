#ifndef VeriPBProoflogger_h
#define VeriPBProoflogger_h

#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <sstream>
#include <set>
#include <cassert>
#include <variant>

#include<iostream>

// NOTE! Should include definition for types Var, Lit and Clause
#ifdef SOLVERTYPES_MAXCDCL
    #include "SolverTypesInt_Minisat.h"
#else
    #include "SolverTypesInt_Pacose.h"
#endif

/******************
 * Posssible template classes:
 * TLit = Type representing a literal
 * TVar = Type representing a variable
 * TSeqLit = A Sequence Container containing literals. 
 *        This means that if lits is of the type TSeqLit, it is possible to retrieve the i'th lit by lits[i].
 *        Examples are std::vector<VeriPB::Lit>, Glucose::vec<Glucose::Lit>, Glucose::Clause, ...
 *        Also necessary: size() function.
 * TSeqWght = A Sequence Container containing Integers. 
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

// Forward declaration of MaxSATProoflogger as to make it a friend class.
class MaxSATProoflogger;

typedef int constraintid;
#define undefcxn -1

typedef std::pair<std::vector<std::pair<VeriPB::Var, VeriPB::Lit>>, std::vector<std::pair<VeriPB::Var, bool>>> nsub;

typedef std::pair<std::vector<std::pair<VeriPB::Var, VeriPB::Lit>>, std::vector<std::pair<VeriPB::Var, bool>>> substitution;
typedef std::string cuttingplanes_derivation;

typedef struct {
    std::string proofgoal;
    std::vector<cuttingplanes_derivation> derivations;
} subproof;

class VeriPbProofLogger
{
    friend MaxSATProoflogger;

private:
    bool keep_original_formula = false; // If true, the proof logging library will never delete any constraint that is an original constraint and will never move a constraint to the core set. 


    // Formula information
    // Only used for the variable naming scheme.
    // Variables in the input starts with x,
    // variables created after parsing input starts with y (except for variables having a meaningful name).
    int n_variables = 0;
    int n_original_constraints = 0;

    // Objective function
    //
    std::vector<VeriPB::Lit> objective_lits;
    std::vector<wght> objective_weights;
    wght objective_constant_cost = 0;
    wght best_objective_value = wght_max;
    constraintid model_improvement_constraint = 0; // Last model improvement constraint

    // Meaningful variable names
    std::unordered_map<VeriPB::VarIdx, std::string> meaningful_names_store;
    uint32_t n_vars_only_known_in_proof;
    

    // Variables to be rewritten by literals.
    std::unordered_map<VeriPB::VarIdx, VeriPB::Lit> map_rewrite_var_by_literal;

    std::string to_string_rewrite_var_by_literal(VeriPB::Var& variable, VeriPB::Lit& literal); 

    // Constraint counter
    //
public:
    constraintid constraint_counter = 0;
private:
    // Temporary string for reverse polish derivation
    //
    std::stringstream pol_string;

       
public:
    // Option to write comments. If false, all comments will be discarded
    bool comments=true; 

    // Proof file
    std::ostream* proof;
    void set_proof_stream(std::ostream* proof);
    void set_keep_original_formula();
    void write_proof_header(int nbclause, int nbvars);
    void write_proof_header(int nbclause);
    void write_proof_header();
    void set_n_variables(int nbvars);
    void set_n_constraints(int nbconstraints);
    void increase_n_variables();
    void increase_constraint_counter();
    bool is_original_constraint(constraintid cxn);

    // Conclusion
    void write_conclusion_NONE();
    void write_conclusion_UNSAT();
    void write_conclusion_UNSAT_optimization();
    void write_conclusion_SAT();
    void write_conclusion_OPTIMAL();
    void write_conclusion_OPTIMAL(constraintid hint);
    void write_conclusion_BOUNDS(wght LB, wght UB);
    void write_conclusion_BOUNDS(wght LB, constraintid hint, wght UB);
    void write_fail();

    // Objective Function 
    template<class TSeqLit, class TSeqWght>
    void set_objective(const TSeqLit &lits, const TSeqWght &weights, wght constant_cost);
    template <class TLit> 
    void add_objective_literal(TLit& lit, wght weight);
    template <class TLit>
    bool remove_objective_literal(TLit& lit);
    template <class TLit>
    wght get_objective_weight(TLit& lit);
    void add_objective_constant(wght weight);
    void subtract_objective_constant(wght weight);
    void write_comment_objective_function();
    void check_model_improving_constraint(const constraintid cxn);
    wght get_best_objective_function();
    void write_objective_update();
    template <class TSeqLit, class TSeqSignedWght>
    void write_objective_update_diff(TSeqLit& litsOnewminusold, TSeqSignedWght& wghtsOnewminusold, signedWght constantOnewminusold = 0);
    // TODO: 
    // void write_objective_update(subproof& new_geq_old, subproof& new_leq_old);



    // ------------- Helping functions -------------
    void write_comment(const char *comment);
    void write_comment(const std::string &comment);
    template <class TVar>
    bool is_aux_var(const TVar &var);
    template <class TVar>
    std::string var_name(const TVar &var);

    VeriPB::Var new_variable_only_in_proof(std::string name="");

    template <class TLit>
    void write_weighted_literal(const TLit &literal, wght weight = 1);
    template <class TLit>
    std::string to_string(const TLit &lit);
    template <class TLit>
    void write_literal(const TLit &lit);
    template <class TSeqLit, class TSeqWght>
    void write_PB_constraint(const TSeqLit &lits, const TSeqWght &weights, const wght RHS);
    template <class TSeqLit, class TSeqWght>
    void write_PB_constraint(const TSeqLit& lits_greater, const TSeqWght& weights_greater, const wght const_greater, const TSeqLit& lits_smaller, const TSeqWght& weights_smaller, const wght const_smaller );
    template <class TSeqLit>
    void write_cardinality_constraint(const TSeqLit &lits, const wght RHS);
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
    void equals_rule(const constraintid constraint_id, const TSeqLit &lits, const wght RHS = 1);
    template <class TSeqLit, class TSeqWght>
    void equals_rule(const constraintid constraint_id, const TSeqLit &lits, const TSeqWght &weights, const wght RHS);
    template <class TSeqLit, class TSeqWght>
    void equals_rule(const constraintid constraint_id, const TSeqLit& lits_greater, const TSeqWght& weights_greater, const wght const_greater, const TSeqLit& lits_smaller, const TSeqWght& weights_smaller, const wght const_smaller  );

    template <class TSeqLit>
    void check_last_constraint(const TSeqLit &lits, const wght RHS = 1);
    template <class TSeqLit, class TSeqWght>
    void check_last_constraint(const TSeqLit &lits, const TSeqWght &weights, const wght RHS);
    template <class TSeqLit, class TSeqWght>
    void check_last_constraint(const TSeqLit& lits_greater, const TSeqWght& weights_greater, const wght const_greater, const TSeqLit& lits_smaller, const TSeqWght& weights_smaller, const wght const_smaller  );

    template <class TSeqLit>
    void check_constraint_exists(const TSeqLit &lits, const wght RHS = 1);
    template <class TSeqLit, class TSeqWght>
    void check_constraint_exists(const TSeqLit &lits, const TSeqWght &weights, const wght RHS);
    template <class TSeqLit, class TSeqWght>
    void check_constraint_exists(const TSeqLit& lits_greater, const TSeqWght& weights_greater, const wght const_greater, const TSeqLit& lits_smaller, const TSeqWght& weights_smaller, const wght const_smaller  );

    // ------------- Rules for adding implied constraints -------------
    template <class TSeqLit>
    constraintid derive_if_implied(const constraintid hint, const TSeqLit &lits, const wght RHS = 1);
    template <class TSeqLit, class TSeqWght>
    constraintid derive_if_implied(const constraintid hint, const TSeqLit &lits, const TSeqWght &weights, const wght RHS);
    template <class TSeqLit, class TSeqWght>
    constraintid derive_if_implied(const constraintid hint, const TSeqLit& lits_greater, const TSeqWght& weights_greater, const wght const_greater, const TSeqLit& lits_smaller, const TSeqWght& weights_smaller, const wght const_smaller  );    

    template <class TSeqLit>
    constraintid derive_if_implied(const TSeqLit &lits, const wght RHS = 1);
    template <class TSeqLit, class TSeqWght>
    constraintid derive_if_implied(const TSeqLit &lits, const TSeqWght &weights, const wght RHS);
    template <class TSeqLit, class TSeqWght>
    constraintid derive_if_implied(const TSeqLit& lits_greater, const TSeqWght& weights_greater, const wght const_greater, const TSeqLit& lits_smaller, const TSeqWght& weights_smaller, const wght const_smaller  );    

    // ------------- Rules for optimisation -------------

    template <class TSeqLit>
    wght calculate_objective_value(const TSeqLit &model);
    template <class TSeqLit>
    constraintid log_solution(const TSeqLit &model, wght objective_value=wght_max, bool only_original_variables_necessary=true, bool log_as_comment=false);
    template <class TSeqLit>
    constraintid log_solution_with_check(const TSeqLit &model, bool only_original_variables_necessary=true, bool log_nonimproving_solution_as_comment=false);
    constraintid get_model_improving_constraint();
    wght get_best_objective_value();
    void update_model_improving_constraint(constraintid newmic);

    template <class TSeqLBool>
    constraintid log_solution_lbools(TSeqLBool &model, wght objective_value=wght_max);

    // TODO: write calculate_objective_value and log_solution_with_check for TSeqLBool 

    // ------------- Unchecked Assumptions -------------
    template <class TSeqLit>
    constraintid unchecked_assumption(const TSeqLit &lits, const wght RHS = 1);
    template <class TSeqLit, class TSeqWght>
    constraintid unchecked_assumption(const TSeqLit &lits, const TSeqWght &weights, const wght RHS);
    template <class TLit> 
    constraintid unchecked_assumption_unit_clause(const TLit& lit, bool core_constraint=true);

    
    // ------------- Reverse Unit Propagation -------------

    template <class TSeqLit>
    constraintid rup(const TSeqLit &lits, const wght RHS = 1);
    template <class TSeqLit, class TSeqWght>
    constraintid rup(const TSeqLit &lits, const TSeqWght &weights, const wght RHS);
    template <class TLit>
    constraintid rup_unit_clause(const TLit& lit, bool core_constraint=true);
    template <class TLit>
    constraintid rup_binary_clause(const TLit& lit1, const TLit& lit2, bool core_constraint=false);
    template <class TLit> 
    constraintid rup_ternary_clause(const TLit& lit1, const TLit& lit2, const TLit& lit3, bool core_constraint=false);

    // ------------- Redundance Based Strenghtening -------------
    void strenghten_to_core();

    void write_substitution(const substitution &witness);
    substitution get_new_substitution();
    template <class TVar>
    void add_boolean_assignment(substitution &s, const TVar& var, const bool value);
    template <class TVar, class TLit>
    void add_literal_assignment(substitution &s, const TVar& var, const TLit& value);
    template <class TVar>
    bool has_boolean_assignment(const substitution &s, const TVar& var);
    template <class TVar>
    bool has_literal_assignment(const substitution &s, const TVar& var);
    template <class TVar>
    bool get_boolean_assignment(substitution &s, const TVar& var);
    template <class TVar>
    VeriPB::Lit get_literal_assignment(substitution &s, const TVar& var);
    size_t get_substitution_size(const substitution &s);

    // Assumption here is that these can prove the implication F ^ not C |- F\w ^ C\w trivially,
    // i.e., for every constraint C' in F\w ^ C\w, C' is either in F or that w only assigns literals to true in C'.
    template <class TSeqLit>
    constraintid redundanceBasedStrengthening(const TSeqLit &lits, const wght RHS, const substitution &witness);
    template <class TSeqLit, class TSeqWght>
    constraintid redundanceBasedStrengthening(const TSeqLit &lits, const TSeqWght &weights, const wght RHS, const substitution &witness);

    // In contrast to all other overloads, this overload of redundanceBasedStrenghtening suspects that at least one proof goal is not trivial (and that it should be proven by either RUP or by an explicit cutting planes proof). 
    // This is important for counting the number of constraints, since constraints are created while proving non-trivial proofgoals.
    template <class TSeqLit, class TSeqWght>
    constraintid redundanceBasedStrengthening(const TSeqLit &lits, const TSeqWght &weights, const wght RHS, const substitution &witness, std::vector<subproof> subproofs);
    template <class TSeqLit>
    constraintid redundanceBasedStrengthening(const TSeqLit &lits, const wght RHS, const substitution &witness, std::vector<subproof> subproofs);

    // ------------- Reification Variables -------------
    // Proves the constraints encoding the reification constraint l <-> C, with l a literal and C a boolean constraint.
    // The right implication is the encoding of l -> C, whereas the left implication means l <- C.
private:
    std::map<VeriPB::VarIdx, constraintid> reifiedConstraintLeftImpl;
    std::map<VeriPB::VarIdx, constraintid> reifiedConstraintRightImpl;
public:
    template <class TSeqLit, class TSeqWght, class TLit>
    constraintid reificationLiteralRightImpl(const TLit& lit, const TSeqLit &litsC, const TSeqWght &weights, const wght RHS, bool store_reified_constraint);
    template <class TSeqLit, class TLit>
    constraintid reificationLiteralRightImpl(const TLit& lit, const TSeqLit &litsC, const wght RHS, bool store_reified_constraint);

    template <class TSeqLit, class TSeqWght, class TLit>
    constraintid reificationLiteralLeftImpl(const TLit& lit, const TSeqLit &litsC, const TSeqWght &weights, const wght RHS, bool store_reified_constraint);
    template <class TSeqLit, class TLit>
    constraintid reificationLiteralLeftImpl(const TLit& lit, const TSeqLit &litsC, const wght RHS, bool store_reified_constraint);

    template <class TVar>
    constraintid getReifiedConstraintLeftImpl(const TVar& var);
    template <class TVar>
    constraintid getReifiedConstraintRightImpl(const TVar& var);
    template <class TVar>
    void setReifiedConstraintLeftImpl(const TVar& var, constraintid cxnId);
    template <class TVar>
    void setReifiedConstraintRightImpl(const TVar& var, constraintid cxnId);
    template <class TVar>
    void deleteReifiedConstraintLeftImpl(const TVar& var);
    template <class TVar>
    void deleteReifiedConstraintRightImpl(const TVar& var);

    /**
     * Remove the right reification constraint from the reification constraint store without deleting it in the proof. 
     * Only needed for not maintaining a constraint id in memory that will not be used in the proof anymore.
    */
    template <class TVar>
    void removeReifiedConstraintRightImplFromConstraintStore(const TVar& var);
    /**
     * Remove the left reification constraint from the reification constraint store without deleting it in the proof. 
     * Only needed for not maintaining a constraint id in memory that will not be used in the proof anymore.
    */
    template <class TVar>
    void removeReifiedConstraintLeftImplFromConstraintStore(const TVar& var);

    // ------------- Cutting Planes derivations -------------

    cuttingplanes_derivation CP_constraintid(const constraintid& constraint_id);
    template <class TLit>
    cuttingplanes_derivation CP_literal_axiom(const TLit& lit);
    cuttingplanes_derivation CP_addition(const cuttingplanes_derivation& left, const cuttingplanes_derivation& right);
    cuttingplanes_derivation CP_addition(const cuttingplanes_derivation& cp);
    cuttingplanes_derivation CP_division(const cuttingplanes_derivation& cp, const wght& n);
    cuttingplanes_derivation CP_division(const wght& n);
    cuttingplanes_derivation CP_saturation(const cuttingplanes_derivation& cp);
    cuttingplanes_derivation CP_saturation();
    cuttingplanes_derivation CP_multiplication(const cuttingplanes_derivation& cp, const wght& n);
    cuttingplanes_derivation CP_multiplication(const wght& n);
    template <class TVar>
    cuttingplanes_derivation CP_weakening(const cuttingplanes_derivation& cp, const TVar& var);
    template <class TVar>
    cuttingplanes_derivation CP_weakening(const TVar& var);
    template <class TLit>
    cuttingplanes_derivation CP_weakening(const cuttingplanes_derivation& cp, const TLit& lit, const wght& weight);
    template <class TLit>
    cuttingplanes_derivation CP_weakening(const TLit& lit, const wght& weight);
    cuttingplanes_derivation CP_apply(const cuttingplanes_derivation& cp_start, const cuttingplanes_derivation& cp_to_be_applied);
    constraintid write_CP_derivation(const cuttingplanes_derivation& cp);
    
    constraintid copy_constraint(const constraintid cxn);
    // OLD:
    void start_intCP_derivation(const constraintid constraint_id);
    template <class TLit>
    void start_intCP_derivation_with_lit_axiom(const TLit &lit);
    void intCP_load_constraint(const constraintid constraint_id);
    void intCP_add();
    void intCP_add_constraint(const constraintid constraint_id);
    template <class TLit>
    void intCP_add_literal_axiom(const TLit &lit);
    void intCP_divide(const wght v);
    void intCP_saturate();
    void intCP_multiply(const wght v);
    template <class TVar>
    void intCP_weaken(const TVar &var);
    template <class TLit>
    void intCP_write_literal_axiom(const TLit &lit);
    constraintid end_intCP_derivation();

    // ------------- Extra Proof Techniques -------------
    /**
     * Derives a constraint C by assuming the negation of C and showing by means of a cutting planes derivation that a 
     * conflict is derived if the negation of C is derived.
     * This is done by proving C using substitution redundancy with an empty witness. 
     * If C is proven by substitution redundancy using any witness w, it should be proven that for a formula F : F ^ ~C |= F\w  ^ C\w.
     * Hence, if w is empty, then it has to be proven that F ^ ~C |= C, which is indeed a contradiction. 
     * 
    */
    template <class TSeqLit, class TSeqWght>
    constraintid prove_by_contradiction(TSeqLit& lits, TSeqWght& weights, wght RHS, std::vector<cuttingplanes_derivation> cpder);
    
    /**
     * If it is possible to derive the following two constraints:
     *      k x + t >= k
     *      k ~x + t >= k
     * then it is possible to derive t >= k. 
     * 
     * Lits and weights are then the literals and weights in t, whereas the RHS coincides with k.   
     * case1 and case2 are the constraintid's for the constraints depicted above.
    */
    template <class TSeqLit, class TSeqWght>
    constraintid prove_by_casesplitting(TSeqLit& lits, TSeqWght& weights, wght RHS, constraintid case1, constraintid case2);

    // ------------- Deleting & Overwriting Constraints -------------
    void delete_constraint_by_id(const constraintid constraint_id, bool overrule_keeporiginalformula=false);
    void delete_constraint_by_id(const constraintid constraint_id, const substitution& witness, bool overrule_keeporiginalformula=false); // TODO-Dieter
    void delete_constraint_by_id(const std::vector<constraintid> &constraint_ids, bool overrule_keeporiginalformula=false);
    template <class TSeqLit>
    void delete_constraint(const TSeqLit &lits, const wght RHS, bool overrule_keeporiginalformula=false);
    template <class TSeqLit>
    void delete_constraint(const TSeqLit &lits, const wght RHS, const substitution& witness, bool overrule_keeporiginalformula=false);    
    template <class TSeqLit, class TSeqWght>
    void delete_constraint(const TSeqLit &lits, const TSeqWght &weights, const wght RHS, bool overrule_keeporiginalformula=false);
    template <class TSeqLit, class TSeqWght>
    void delete_constraint(const TSeqLit &lits, const TSeqWght &weights, const wght RHS, const substitution& witness, bool overrule_keeporiginalformula=false);    

    //Minisat:
    // void delete_constraint(Glucose::Clause &clause);
    // template <c>
    // void delete_constraint(TVec<TLit> &clause, bool overrule_keeporiginalformula=false);

    // Removal by del find where a literal occuring multiple times in lits is only written once.
    template <class TSeqLit>
    void delete_clause(const TSeqLit& lits, bool overrule_keeporiginalformula=false);

    template <class TSeqLit>
    constraintid overwrite_constraint(const constraintid constraint_id, const TSeqLit &lits, const wght RHS = 1, bool origclause_in_coreset=false);
    template <class TSeqLit>
    constraintid overwrite_constraint(const TSeqLit &lits_orig, const wght RHS_orig, const TSeqLit &lits, const wght RHS, bool origclause_in_coreset=false);
    template <class TSeqLit, class TSeqWght>
    constraintid overwrite_constraint(const constraintid constraint_id, const TSeqLit &lits, const TSeqWght &weights, const wght RHS, bool origclause_in_coreset);
    template <class TSeqLit, class TSeqWght>
    constraintid overwrite_constraint(const TSeqLit &lits_orig, const TSeqWght &weights_orig, const wght RHS_orig, const TSeqLit &lits, const TSeqWght &weights, const wght RHS, bool origclause_in_coreset);
    template <class TSeqLit>
    constraintid overwrite_constraint(const TSeqLit &lits_orig, const TSeqLit &lits, bool origclause_in_coreset=false);

    void move_to_coreset(constraintid cxn, bool overrule_keeporiginalformula=false);
    template <class TSeqLit>
    void move_to_coreset(TSeqLit& lits, wght RHS=1, bool overrule_keeporiginalformula=false);
    template <class TSeqLit, class TSeqWght>
    void move_to_coreset(TSeqLit& lits, TSeqWght& wghts, wght RHS=1, bool overrule_keeporiginalformula=false);
    // ------------- Handling contradiction -------------
    void write_previous_constraint_contradiction();
    void write_contradiction(constraintid cxnid);
    void rup_empty_clause();  
    

};

//=================================================================================================

#endif
