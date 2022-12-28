#ifndef MaxSATProoflogger_h
#define MaxSATProoflogger_h

#include "VeriPBProoflogger.h"

class MaxSATProoflogger
{
    VeriPbProofLogger *PL;
    std::vector<constraintid> core_lower_bounds;
    std::vector<int> core_weights;
    std::map<VeriPB::Var, int> counting_var_to_core_idx;

    std::vector<constraintid> extended_unitclauses; 

public:
    /// @brief Constructor.
    /// @param PL The main VeriPB prooflogger.
    MaxSATProoflogger(VeriPbProofLogger *PL);

    // Functions for translating MaxSAT to PBO
    template <class TLit>
    void add_blocking_literal(TLit lit, constraintid cxn_id);

    template <class TLit>
    void add_unit_clause_blocking_literal(TLit var, constraintid cxn_id, TLit unitclause);

    constraintid rewrite_model_improvement_constraint_with_extended_unitclauses();

    // Functions to help with objective reformulation

    /// @brief Add the lower bound on the input variables of a core to the database. This functions is intended to add the lower bound up to counter variable with index 2.
    /// @tparam TVar
    /// @param lazy_var Counter variable with index 2.
    /// @param core_id Constraint ID of core or pseudo-Boolean definition for counter variable with index 1 in the direction counter variable implies constraint.
    /// @param pb_definition_id Constraint ID of pseudo-Boolean definition for counter variable with index 2 in the direction counter variable implies constraint.
    /// @param weight Weight of the core.
    /// @return Constraint ID of the lower bound.
    template <class TVar>
    constraintid add_core_lower_bound(const TVar &lazy_var, constraintid core_id, constraintid pb_definition_id, int weight);

    /// @brief Update the lower bound on the input literals with the definition for the new counting variable for an index `bound`.
    /// @tparam TVar
    /// @param old_lazy_var Counting variable with index `bound - 1`
    /// @param new_lazy_var Counting variable with index `bound`
    /// @param pb_definition_id Constraint ID of pseudo-Boolean definition for counter variable with index `bound` in the direction counter variable implies constraint.
    /// @param bound The index of the new counter variable.
    /// @return Constraint ID of the updated lower bound.
    template <class TVar>
    constraintid update_core_lower_bound(const TVar &old_lazy_var, const TVar &new_lazy_var, constraintid pb_definition_id, int bound);

    /// @brief Derive the objective reformulation constraint. This constraint says that the original objective is lower bounded by the reformulated objective.
    /// @param base_reform_id Constraint ID of the base objective reformulation constraint.
    /// @return Constraint ID of the objective reformulation constraint.
    constraintid derive_objective_reformulation_constraint(constraintid base_reform_id);

    /// @brief Proof log the objective reformulation for arguing about the gap between the lower bound and the upper bound.
    /// @param base_reform_id Constraint ID of the base objective reformulation constraint.
    /// @param model_improve_id Constraint ID of the model improving constraint.
    /// @return Constraint ID of the constraint to argue about the gap between the lower and upper bound. The slack (without any assignment) of this constraint is equal to `upper bound - lower bound`.
    constraintid proof_log_objective_reformulation(constraintid base_reform_id, constraintid model_improve_id);
    /// @brief Add a unit core to the base objective reformulation constraint.
    /// @param base_reform_id Constraint ID of the base objective reformulation constraint.
    /// @param core_id Constraint ID of the core constraint.
    /// @param weight Weight of the core.
    /// @return Constraint ID of the updated base objective reformulation constraint.
    constraintid base_reform_unit_core(constraintid base_reform_id, constraintid core_id, int weight);

    // Functions for proof logging at-most-one constraints

    /// @brief Derive the at-most-one constraint as a pseudo-Boolean constraint, which is the sum of the literals is at least the number of literals - 1.
    /// @tparam TLit
    /// @param am1_lits Literals of the at-most-one constraint.
    /// @return Constraint ID of the at-most-one constraint.
    template <class TLit>
    constraintid derive_at_most_one_constraint(const std::vector<TLit> &am1_lits);

    /// @brief Introduce a new variable that represents all variables in the at-most-one constraint being true.
    /// @tparam TLit
    /// @param am1_lits Literals of the at-most-one constraint.
    /// @param select_all_lit Selector literal for the at-most-one constraint. This literal will be true if and only if all the literals in `am1_lits` are true.
    /// @return Constraint ID of the constraint required for objective reformulation, which is a lower bound on the literals in the at-most-one constraint.
    template <class TLit>
    constraintid introduce_at_most_one_selector(const std::vector<TLit> &am1_lits, const TLit &select_all_lit);

    /// @brief Proof log the objective reformulation with the intrinsic at-most-one constraint. This function requires that the at-most-one constraint was detected by propagation.
    /// @tparam TLit
    /// @param base_reform_id Constraint ID of the base objective reformulation constraint.
    /// @param am1_lits Literals of the at-most-one constraint.
    /// @param select_all_lit Selector literal for the at-most-one constraint. This literal will be true if and only if all the literals in `am1_lits` are true.
    /// @param weight Weight of the at most one constraint.
    /// @return Constraint ID of the updated base objective reformulation constraint.
    template <class TLit>
    constraintid proof_log_at_most_one(constraintid base_reform_id, const std::vector<TLit> &am1_lits, const TLit &select_all_lit, int weight);
};

#endif