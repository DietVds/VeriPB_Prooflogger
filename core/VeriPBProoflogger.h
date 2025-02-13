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
#include <charconv>
#include <climits>
#include <stdexcept>

#include<iostream>

// NOTE! Should include definition for types Var, Lit and Clause
#include "VeriPbSolverTypes.h"
#include "VariableManager.h"




//prooflogging Library

/**
 * TODOs
 * - Dominance rule
   - Instantiate important functions for VeriPB::Var, VeriPB::Lit, constraintid as TNumber, ...
   - Replace number_to_string by write_number as much as possible
   - Constructors
 */

//=================================================================================================
// Prooflogger

// Forward declaration of MaxSATProoflogger as to make it a friend class.
class MaxSATProoflogger;

namespace VeriPB {
    
    #define undefcxn 0
    typedef std::string cuttingplanes_derivation;
    typedef std::pair<std::vector<std::pair<VeriPB::Var, VeriPB::Lit>>, std::vector<std::pair<VeriPB::Var, bool>>> substitution;

    struct subproof {
        std::string proofgoal;
        std::vector<cuttingplanes_derivation> derivations;
    };

    template <typename ObjLit, typename ObjCoeff, typename ObjConst>
    class VeriPbProofLogger
    {
        friend MaxSATProoflogger;

    public:
        // ------------- Proof file manipulation -------------
        void set_proof_stream(std::ostream* proof);
        void set_keep_original_formula_on();
        void set_keep_original_formula_off();
        void write_proof_header();
        void set_n_orig_constraints(int nbconstraints);
        bool is_original_constraint(const constraintid& cxn);
        constraintid get_constraint_counter();
        void set_variable_manager(const VarManager* varMgr);

        // ------------- Conclusion -------------
        void write_conclusion_NONE();
        void write_conclusion_UNSAT();
        void write_conclusion_UNSAT_optimization();
        void write_conclusion_UNSAT_optimization(const constraintid& hint);
        void write_conclusion_SAT();
        void write_conclusion_OPTIMAL();
        void write_conclusion_OPTIMAL(const constraintid& hint);
        void write_conclusion_BOUNDS(const ObjConst& LB, const ObjConst& UB);
        void write_conclusion_BOUNDS(const ObjConst& LB, const constraintid& hint, const ObjConst& UB);
        void write_fail();

        // ------------- Objective function manipulation -------------
        void set_objective(const LinTermBoolVars<ObjLit, ObjCoeff, ObjConst>* new_objective);
        void add_objective_literal(const ObjLit& lit, const ObjCoeff weight);
        bool remove_objective_literal(const ObjLit& lit);
        ObjCoeff get_objective_weight(const ObjLit& lit);
        void add_objective_constant(const ObjConst& weight);
        void subtract_objective_constant(const ObjConst& weight);
        void write_comment_objective_function();
        void check_model_improving_constraint(const constraintid& cxnid=undefcxn);
        bool logged_solution();
        ObjConst get_best_objective_value();

        // ------------- Solution improving -------------
        template <typename TModel>
        ObjConst calculate_objective_value(const TModel& model);
        template <typename TModel>
        constraintid log_solution(const TModel& model, const ObjConst objective_value, const bool only_original_variables_necessary=true, const bool log_as_comment=false);
        template <class TModel>
        constraintid log_solution(const TModel& model, bool only_original_variables_necessary=true, bool log_as_comment=false); // TODO-Dieter: if no objective is set, we need to use this one.
        template <class TModel>
        constraintid log_solution_with_check(const TModel &model, bool only_original_variables_necessary=true, bool log_nonimproving_solution_as_comment=false);
        constraintid get_model_improving_constraint();
        void update_model_improving_constraint(const constraintid& newmic);
        
        // ------------- Objective update -------------
        void write_objective_update();
        //TODO-Dieter: Also create objective update with subproofs/hints.
        template <class TLinTerm>
        void write_objective_update_diff(TLinTerm& oldObj, TLinTerm& newObj);
        template <class TLit>
        void write_objective_update_diff_for_literal(TLit& literal_to_remove, ObjCoeff weight = 1, ObjConst constant_for_lit = 0, bool update_model_improving_constraint=false);
        template <class TLit> 
        void write_objective_update_diff_literal_replacement(TLit& literal_to_remove, TLit& literal_to_add, ObjCoeff weight=1, bool update_model_improving_constraint=false);

        // ------------- Cutting Planes derivations -------------

        constraintid copy_constraint(const constraintid cxn);
        
        cuttingplanes_derivation start_CP_derivation();
        cuttingplanes_derivation start_CP_derivation_cxn(const constraintid& cxn_id); 
        template <class TLit>
        cuttingplanes_derivation start_CP_derivation_lit_axiom(const TLit& lit_axiom);
        void CP_add(cuttingplanes_derivation& cpder1, const cuttingplanes_derivation& cpder2);
        void CP_start_subderivation_cxn_id(cuttingplanes_derivation& cpder, const constraintid& cxn_id);
        template <class TLit>
        void CP_start_subderivation_lit_axiom(cuttingplanes_derivation& cpder, const TLit& lit_axiom);
        void CP_add_subderivation(cuttingplanes_derivation& cpder);
        template <class TNumber>
        void CP_add_cxn(cuttingplanes_derivation& cpder, const constraintid& cxn_id, const TNumber& mult=1);
        template <class TLit, class TNumber>
        void CP_add_litaxiom(cuttingplanes_derivation& cpder, const TLit& lit_axiom, const TNumber& mult=1);
        template <class TNumber>
        void CP_divide(cuttingplanes_derivation& cp, const TNumber& n);
        void CP_saturate(cuttingplanes_derivation& cp);
        template <class TNumber>
        void CP_multiply(cuttingplanes_derivation& cp, const TNumber& n);
        template <class TVar>
        void CP_weaken(cuttingplanes_derivation& cp, const TVar& var);
        constraintid write_CP_derivation(const cuttingplanes_derivation& cp);

        // internal CP derivation
        void start_internal_CP_derivation(bool write_directly_to_proof=false);
        void start_internal_CP_derivation_cxn(const constraintid& cxn_id,bool write_directly_to_proof=false);
        template <class TLit>
        void start_internal_CP_derivation_lit_axiom(const TLit& lit_axiom, bool write_directly_to_proof=false);
        void CP_add(const cuttingplanes_derivation& cpder2);
        void CP_start_subderivation_cxn_id(const constraintid& cxn_id);
        template <class TLit>
        void CP_start_subderivation_lit_axiom(const TLit& lit_axiom);
        void CP_add_subderivation();
        template <class TNumber>
        void CP_add_cxn(const constraintid& cxn_id, const TNumber& mult=1);
        template <class TLit, class TNumber>
        void CP_add_litaxiom(const TLit& lit_axiom, const TNumber& mult=1);
        template <class TNumber>
        void CP_divide(const TNumber& n);
        void CP_saturate();
        template <class TNumber>
        void CP_multiply(const TNumber& n);
        template <class TVar>
        void CP_weaken(const TVar& var);
        constraintid write_internal_CP_derivation();

        // ------------- Comments -------------
        void write_comment(const char *comment);
        void write_comment(const std::string &comment);

        // ------------- Rules for checking constraints -------------
        template <class TConstraint>
        void equals_rule(const TConstraint& cxn, const constraintid cxn_id);
        template <class TConstraint>
        void check_last_constraint(const TConstraint& cxn);
        template <class TConstraint>
        void check_constraint_exists(const TConstraint& cxn);
        template <class TConstraint>
        void check_implied(const TConstraint& cxn);
        template <class TConstraint>
        void check_implied(const TConstraint& cxn, constraintid cxn_id);

        // ------------- Rules for adding syntactically implied constraints -------------
        template <class TConstraint>
        constraintid derive_if_implied(const TConstraint& cxn);
        template <class TConstraint>
        constraintid derive_if_implied(const TConstraint& cxn, const constraintid& cxn_id);

        // ------------- Unchecked Assumptions -------------
        template <class TConstraint>
        constraintid unchecked_assumption(const TConstraint& cxn);

        // ------------- Reverse Unit Propagation -------------
        template <class TConstraint>
        constraintid rup(const TConstraint& cxn, bool core_constraint=false);
        template <class TConstraint>
        constraintid rup(const TConstraint& cxn, const std::vector<constraintid>& hints, const bool core_constraint=false);

        template <class TConstraint>
        constraintid rup_clause(const TConstraint& lits);
        template <class TConstraint>
        constraintid rup_clause(const TConstraint& lits, std::vector<constraintid>& hints);
        
        template <class TLit>
        constraintid rup_unit_clause(const TLit& lit, bool core_constraint=true);
        template <class TLit>
        constraintid rup_unit_clause(const TLit& lit, std::vector<constraintid>& hints, bool core_constraint=true);
        
        template <class TLit>
        constraintid rup_binary_clause(const TLit& lit1, const TLit& lit2, bool core_constraint=false);
        template <class TLit>
        constraintid rup_binary_clause(const TLit& lit1, const TLit& lit2, std::vector<constraintid>& hints,  bool core_constraint=false);
        
        template <class TLit> 
        constraintid rup_ternary_clause(const TLit& lit1, const TLit& lit2, const TLit& lit3, bool core_constraint=false);
        template <class TLit> 
        constraintid rup_ternary_clause(const TLit& lit1, const TLit& lit2, const TLit& lit3, std::vector<constraintid>& hints, bool core_constraint=false);

        // ------------- Redundance Based Strenghtening -------------
        void strenghten_to_core_on();
        void strenghten_to_core_off();

        substitution get_new_substitution();
        template <class TVar>
        void add_boolean_assignment(substitution &s, const TVar& var, const bool value);
        template <class TVar, class TLit>
        void add_literal_assignment(substitution &s, const TVar& var, const TLit& value);
        void add_substitution(substitution &sub, const substitution &sub_to_add);
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
        template <class TConstraint>
        constraintid redundance_based_strengthening(const TConstraint& cxn, const substitution& witness);
        template <class TLit>
        constraintid redundance_based_strengthening_unit_clause(const TLit& lit);
        
        template <class TConstraint>
        constraintid redundance_based_strengthening(const TConstraint& cxn, const substitution& witness, const std::vector<subproof>& subproofs);
        

        template <class TConstraint>
        constraintid start_redundance_based_strengthening_with_subproofs(const TConstraint& cxn, const substitution& witness);
        constraintid start_new_subproof(const std::string& proofgoal);
        void end_subproof();
        constraintid end_redundance_based_strengthening_with_subproofs();

        // ------------- Reification -------------
        template <class TLit, class TConstraint>
        constraintid reification_literal_right_implication(const TLit& lit, const TConstraint& cxn, const bool store_reified_constraint=false);
        template <class TLit, class TConstraint>
        constraintid reification_literal_left_implication(const TLit& lit, const TConstraint& cxn, const bool store_reified_constraint=false);
        
        template <class TVar>
        void delete_reified_constraint_left_implication(const TVar& var);
        template <class TVar>
        void delete_reified_constraint_right_implication(const TVar& var);

        template <class TVar>
        constraintid get_reified_constraint_left_implication(const TVar& var);
        template <class TVar>
        constraintid get_reified_constraint_right_implication(const TVar& var);
        template <class TVar>
        void store_reified_constraint_left_implication(const TVar& var, const constraintid& cxnId);
        template <class TVar>
        void store_reified_constraint_right_implication(const TVar& var, const constraintid& cxnId);

        /**
         * Remove the right reification constraint from the reification constraint store without deleting it in the proof. 
         * Only needed for not maintaining a constraint id in memory that will not be used in the proof anymore.
        */
        template <class TVar>
        void remove_reified_constraint_right_implication_from_constraintstore(const TVar& var);
        /**
         * Remove the left reification constraint from the reification constraint store without deleting it in the proof. 
         * Only needed for not maintaining a constraint id in memory that will not be used in the proof anymore.
        */
        template <class TVar>
        void remove_reified_constraint_left_implication_from_constraintstore(const TVar& var);

        // ------------- Proof by contradiction -------------
        /**
         * Derives a constraint C by assuming the negation of C and showing by means of a cutting planes derivation that a 
         * conflict is derived if the negation of C is derived.
         * This is done by proving C using substitution redundancy with an empty witness. 
         * If C is proven by substitution redundancy using any witness w, it should be proven that for a formula F : F ^ ~C |= F\w  ^ C\w.
         * Hence, if w is empty, then it has to be proven that F ^ ~C |= C, which is indeed a contradiction. 
         * 
        */
        template <class TConstraint>
        constraintid start_proof_by_contradiction(const TConstraint& cxn);
        constraintid end_proof_by_contradiction();
        
        // ------------- Proof by case splitting -------------
        /**
         * If it is possible to derive the following two constraints:
         *      k x + t >= k
         *      k ~x + t >= k
         * then it is possible to derive t >= k. 
         * 
         * Lits and weights are then the literals and weights in t, whereas the RHS coincides with k.   
         * case1 and case2 are the constraintid's for the constraints depicted above.
        */
        template <class TConstraint>
        constraintid prove_by_casesplitting(const TConstraint& cxn, const constraintid& case1, const constraintid& case2);

        // ------------- Deleting & Overwriting Constraints -------------
        void delete_constraint_by_id(const constraintid constraint_id, bool overrule_keeporiginalformula=false);
        void delete_constraint_by_id(const constraintid constraint_id, const substitution& witness, bool overrule_keeporiginalformula=false); // TODO-Dieter
        void delete_constraint_by_id(const constraintid constraint_id, const substitution& witness, const subproof& subproof, bool overrule_keeporiginalformula=false); // TODO-Dieter
        void delete_constraint_by_id(const constraintid constraint_id, const substitution& witness, const std::vector<subproof>& subproofs, bool overrule_keeporiginalformula=false); // TODO-Dieter
        void delete_constraint_by_id(const std::vector<constraintid> &constraint_ids, bool overrule_keeporiginalformula=false);
        
        template <class TConstraint> 
        void delete_constraint(const TConstraint& cxn, const bool overrule_keeporiginalformula=false);
        template <class TConstraint>
        void delete_constraint(const TConstraint& cxn, const substitution& witness, bool overrule_keeporiginalformula=false);
        template <class TConstraint> 
        void delete_constraint(const TConstraint& cxn, const substitution& witness, const subproof& subproof, bool overrule_keeporiginalformula=false); // TODO-Dieter
        template <class TConstraint> 
        void delete_constraint(const TConstraint& cxn, const substitution& witness, const std::vector<subproof>& subproofs, bool overrule_keeporiginalformula=false); // TODO-Dieter
        
        // Removal by del find where a literal occuring multiple times in lits is only written once.
        template <class TConstraint>
        void delete_clause(const TConstraint& cxn, bool overrule_keeporiginalformula=false);

        template <class TConstraint>
        constraintid overwrite_constraint(const constraintid& orig_cxn_id, const TConstraint& new_cxn, bool origclause_in_coreset=false);
        template <class TConstraint>
        constraintid overwrite_constraint(const TConstraint& orig_cxn, const TConstraint& new_cxn, bool origclause_in_coreset=false);

        void move_to_coreset(const constraintid& cxn, bool overrule_keeporiginalformula=false);
        template <class TConstraint>
        void move_to_coreset(const TConstraint& cxn, bool overrule_keeporiginalformula=false);
    
        // ------------- Constructor -------------
        // TODO  
    private:
        // ------------- Variable Manager -------------
        VarManager* _varMgr;        

        // ------------- Formula information -------------
        bool _keep_original_formula = false; // If true, the proof logging library will never delete any constraint that is an original constraint and will never move a constraint to the core set. 
        int _n_orig_constraints = 0;
        constraintid _constraint_counter = 0;

        // ------------- Formula stream -------------
        std::ostream* proof;
        int _write_buffer_size = 32 * 1024 * 1024;
        char* _write_buffer = new char[write_buffer_size]; // Buffer for the proof.

        
        // ------------- Objective function -------------
        // Objective function
        LinTermBoolVars<ObjLit, ObjCoeff, ObjConst> _objective;
        ObjConst _best_objective_value;
        bool _found_solution=false; // TODO: Keep track of bookkeeping of already found solution.
        constraintid _model_improvement_constraint = 0; // Last model improvement constraint
 
        // ------------- Cutting plane derivations -------------
        std::string _internal_cuttingplanes_buffer;
        bool _writing_CP_to_proof=false;

        // ------------- Commenting -------------
        bool _comments=true;  //TODO: add compile definition instead
       

        // ------------- Writing -------------
        template <class TLit>
        void write_weighted_literal(const TLit &literal, const ObjCoeff& weight = 1, const bool& add_prefix_space=true);
        template <typename TModel>
        void write_model(const TModel& model, const bool log_as_comment);
        template <typename TConstraint>
        void write_constraint(const TConstraint& cxn);
        template <typename TClause>
        void write_clause(const TClause& cxn);
 
        void write_hints(std::vector<constraintid>& hints);
        void write_substitution(const substitution &witness);
        
        // ------------- Reification Variables -------------
        std::vector<constraintid> _reifiedConstraintLeftImpl;
        std::vector<constraintid> _reifiedConstraintRightImpl;
        std::vector<constraintid> _reifiedConstraintLeftImplOnlyProofVars;
        std::vector<constraintid> _reifiedConstraintRightImplOnlyProofVars;
    };
}

//=================================================================================================
#endif
