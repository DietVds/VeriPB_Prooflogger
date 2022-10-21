#include "MaxSATProoflogger.h"

//=================================================================================================
// Constructor

MaxSATProoflogger::MaxSATProoflogger(VeriPbProofLogger *PL) : PL(PL) {}

//=================================================================================================
// MaxSAT to PBO translation

template <class TVar>
void MaxSATProoflogger::add_blocking_literal_for_var(TVar var, constraintid cxn_id, bool negated_blocking_literal){
    PL->store_meaningful_name(var, "_b" + std::to_string(cxn_id));

    // VeriPB expects the blocking literal added to the clauses to be a negated blocking variable. 
    // If the solver adds a non-negated blocking literal to clauses, all literals over this variable have to be negated throughout the proof.
    if(!negated_blocking_literal)
        PL->rewrite_var_by_literal[var] = neg(var);
}
template void MaxSATProoflogger::add_blocking_literal_for_var<int>(int var, constraintid cxn_id, bool negated_blocking_literal);

//=================================================================================================
// Objective reformulation

template <class TVar>
constraintid MaxSATProoflogger::add_core_lower_bound(const TVar &lazy_var, constraintid core_id, constraintid pb_definition_id, int weight)
{
    PL->start_CP_derivation(core_id);
    PL->CP_add_constraint(pb_definition_id);
    PL->CP_divide(2);
    constraintid lower_bound_id = PL->end_CP_derivation();
    counting_var_to_core_idx[lazy_var] = core_lower_bounds.size();
    core_lower_bounds.push_back(lower_bound_id);
    core_weights.push_back(weight);
    return lower_bound_id;
}
template constraintid MaxSATProoflogger::add_core_lower_bound<int>(const int &lazy_var, constraintid core_id, constraintid pb_definition_id, int weight);

template <class TVar>
constraintid MaxSATProoflogger::update_core_lower_bound(const TVar &old_lazy_var, const TVar &new_lazy_var, constraintid pb_definition_id, int bound)
{
    int core_idx = counting_var_to_core_idx[old_lazy_var];

    // Update map
    counting_var_to_core_idx[new_lazy_var] = core_idx;

    // Update lower bound
    PL->start_CP_derivation(core_lower_bounds[core_idx]);
    PL->CP_multiply(bound - 1);
    PL->CP_add_constraint(pb_definition_id);
    PL->CP_divide(bound);
    constraintid new_lower_bound_id = PL->end_CP_derivation();
    PL->delete_constraint<int>(core_lower_bounds[core_idx]);
    core_lower_bounds[core_idx] = new_lower_bound_id;
    return new_lower_bound_id;
}
template constraintid MaxSATProoflogger::update_core_lower_bound<int>(const int &old_lazy_var, const int &new_lazy_var, constraintid pb_definition_id, int bound);

constraintid MaxSATProoflogger::derive_objective_reformulation_constraint(constraintid base_reform_id)
{
    PL->start_CP_derivation(base_reform_id);
    for (int i = 0; i < core_lower_bounds.size(); i++)
    {
        PL->CP_load_constraint(core_lower_bounds[i]);
        PL->CP_multiply(core_weights[i]);
        PL->CP_add();
    }
    return PL->end_CP_derivation();
}

constraintid MaxSATProoflogger::proof_log_objective_reformulation(constraintid base_reform_id, constraintid model_improve_id)
{
    constraintid objective_reform_id = derive_objective_reformulation_constraint(base_reform_id);

    PL->start_CP_derivation(model_improve_id);
    PL->CP_add_constraint(objective_reform_id);
    constraintid reformulated_objective = PL->end_CP_derivation();
    PL->delete_constraint<int>(objective_reform_id);
    return reformulated_objective;
}

constraintid MaxSATProoflogger::base_reform_unit_core(constraintid base_reform_id, constraintid core_id, int weight)
{
    PL->start_CP_derivation(core_id);
    PL->CP_multiply(weight);
    PL->CP_add_constraint(base_reform_id);
    constraintid new_base_reform_id = PL->end_CP_derivation();
    PL->delete_constraint<int>(base_reform_id);
    return new_base_reform_id;
}

//=================================================================================================
// At-most-one constraints

template <class TLit>
constraintid MaxSATProoflogger::derive_at_most_one_constraint(const std::vector<TLit> &am1_lits)
{
    std::vector<constraintid> binary_clauses;

    for (int new_lit_idx = 1; new_lit_idx < am1_lits.size(); new_lit_idx++)
    {
        if (new_lit_idx != 1) // first constraint is not multiplied
        {
            PL->CP_multiply(new_lit_idx - 1);
        }
        for (int lit_idx = 0; lit_idx < new_lit_idx; lit_idx++)
        {
            constraintid binary_clause_id = PL->rup(std::vector<TLit>{am1_lits[new_lit_idx], am1_lits[lit_idx]});
            binary_clauses.push_back(binary_clause_id);

            if (new_lit_idx == 1) // the first binary constraint to start the derivation
            {
                PL->start_CP_derivation(binary_clause_id);
            }
            else
            {
                PL->CP_add_constraint(binary_clause_id);
            }
        }
        PL->CP_divide(new_lit_idx);
    }

    constraintid am1_constraint_id = PL->end_CP_derivation();
    PL->delete_constraint<int>(binary_clauses);
    return am1_constraint_id;
}
template constraintid MaxSATProoflogger::derive_at_most_one_constraint<int>(const std::vector<int> &am1_lits);
template constraintid MaxSATProoflogger::derive_at_most_one_constraint<Glucose30::Lit>(const std::vector<Glucose30::Lit> &am1_lits);

template <class TLit>
constraintid MaxSATProoflogger::introduce_at_most_one_selector(const std::vector<TLit> &am1_lits, const TLit &select_all_lit)
{
    std::vector<TLit> constraint_lits = am1_lits;
    constraint_lits.push_back(neg(select_all_lit));

    constraintid reified_am1_constraint_id = PL->redundanceBasedStrengthening(constraint_lits, am1_lits.size(), neg(select_all_lit));

    for (int i = 0; i < constraint_lits.size(); i++)
    {
        constraint_lits[i] = neg(constraint_lits[i]);
    }
    PL->redundanceBasedStrengthening(constraint_lits, 1, select_all_lit);

    return reified_am1_constraint_id;
}
template constraintid MaxSATProoflogger::introduce_at_most_one_selector<int>(const std::vector<int> &am1_lits, const int &select_all_lit);
template constraintid MaxSATProoflogger::introduce_at_most_one_selector<Glucose30::Lit>(const std::vector<Glucose30::Lit> &am1_lits, const Glucose30::Lit &select_all_lit);

template <class TLit>
constraintid MaxSATProoflogger::proof_log_at_most_one(constraintid base_reform_id, const std::vector<TLit> &am1_lits, const TLit &select_all_lit, int weight)
{
    constraintid am1_constraint = derive_at_most_one_constraint(am1_lits);
    constraintid am1_lower_bound = introduce_at_most_one_selector(am1_lits, select_all_lit);
    PL->delete_constraint<int>(am1_constraint);

    PL->start_CP_derivation(am1_lower_bound);
    PL->CP_multiply(weight);
    PL->CP_add_constraint(base_reform_id);
    constraintid new_base_reform_id = PL->end_CP_derivation();
    PL->delete_constraint<int>(base_reform_id);
    PL->delete_constraint<int>(am1_lower_bound);
    return new_base_reform_id;
}
template constraintid MaxSATProoflogger::proof_log_at_most_one<int>(constraintid base_reform_id, const std::vector<int> &am1_lits, const int &select_all_lit, int weight);
template constraintid MaxSATProoflogger::proof_log_at_most_one<Glucose30::Lit>(constraintid base_reform_id, const std::vector<Glucose30::Lit> &am1_lits, const Glucose30::Lit &select_all_lit, int weight);