#include "MaxSATProoflogger.h"

// PL Library

//=================================================================================================
// Constructor

MaxSATProoflogger::MaxSATProoflogger(VeriPbProofLogger *PL) : PL(PL) {}

//=================================================================================================
// MaxSAT to PBO translation

template <class TLit>
void MaxSATProoflogger::add_blocking_literal(TLit lit, constraintid cxn_id){
    PL->store_meaningful_name(variable(lit), "_b" + std::to_string(cxn_id));

    // VeriPB expects the blocking literal added to the clauses to be a negated blocking variable. 
    // If the solver adds a non-negated blocking literal to clauses, all literals over this variable have to be negated throughout the proof.
    if(!is_negated(lit))
        PL->rewrite_variable_by_literal(variable(lit), neg(lit));
}

template <class TLit>
constraintid MaxSATProoflogger::add_unit_clause_blocking_literal(TLit blocking_lit, constraintid cxn_id, TLit unitclause, wght weight_softclause, bool rewrite_objective){
    PL->store_meaningful_name(variable(blocking_lit), "_bu" + std::to_string(cxn_id));

    VeriPB::Lit _blocking_lit = toVeriPbLit(blocking_lit);
    VeriPB::Lit _unitclause = toVeriPbLit(unitclause);

    std::vector<VeriPB::Lit> cls;
    cls.push_back(_unitclause);
    cls.push_back(_blocking_lit);    

    substitution witness = PL->get_new_substitution();
    PL->add_boolean_assignment(witness, variable(_blocking_lit), !is_negated(_blocking_lit));
    
    constraintid c_id = PL->redundanceBasedStrengthening(cls, 1, witness);
    
    if(rewrite_objective){
        bool origf = PL->keep_original_formula;
        PL->keep_original_formula = false;

        PL->move_to_coreset(-1); // Is necessary for the objective update rule.

        cls.clear();
        cls.push_back(neg(_unitclause));
        cls.push_back(neg(_blocking_lit));        

        witness = PL->get_new_substitution();
        PL->add_boolean_assignment(witness, variable(_blocking_lit), is_negated(_blocking_lit));
        
        constraintid c_inverse_id = PL->redundanceBasedStrengthening(cls, 1, witness);
        PL->move_to_coreset(-1);

        PL->add_objective_literal(blocking_lit, weight_softclause);
        TLit neglit = neg(unitclause);
        PL->remove_objective_literal(neglit);
        
        
        std::vector<VeriPB::Lit> litsOnewminusold = {toVeriPbLit(neg(unitclause)), toVeriPbLit(blocking_lit)};
        std::vector<signedWght> wghtsOnewminusold = {-static_cast<signedWght>(weight_softclause), static_cast<signedWght>(weight_softclause)};

        PL->write_objective_update_diff(litsOnewminusold, wghtsOnewminusold);

        if(PL->get_model_improving_constraint() != 0){ //TODO: Test!
            PL->start_intCP_derivation(&cp, PL->get_model_improving_constraint());
            PL->intCP_add_constraint(&cp, -1, weight_softclause);
            PL->update_model_improving_constraint(PL->end_intCP_derivation(&cp));
            // cuttingplanes_derivation cpder = PL->CP_constraintid( PL->get_model_improving_constraint());
            // cpder = PL->CP_addition(cpder, 
            //                PL->CP_multiplication(PL->CP_constraintid(-1), weight_softclause));
            // PL->update_model_improving_constraint(PL->write_CP_derivation(cpder));
        }

        PL->delete_constraint(cls, 1, witness);
        PL->keep_original_formula = true; 
    }

    
    
    return c_id;
}

constraintid MaxSATProoflogger::rewrite_model_improvement_constraint_for_unitsoftclauses(){
    PL->start_intCP_derivation(&cp, PL->get_model_improving_constraint());
    PL->intCP_apply(&cp, rewrite_for_unitsoftclauses);
    rewritten_objective_for_unitsoftclauses = PL->end_intCP_derivation(&cp);
    // cuttingplanes_derivation cpder = PL->CP_apply(PL->CP_constraintid(PL->get_model_improving_constraint()), rewrite_for_unitsoftclauses);
    // rewritten_objective_for_unitsoftclauses = PL->write_CP_derivation(cpder);
    return rewritten_objective_for_unitsoftclauses;
}

constraintid MaxSATProoflogger::get_rewritten_model_improvement_constraint_for_unitsoftclauses(){
    return rewritten_objective_for_unitsoftclauses;
}

void MaxSATProoflogger::derive_blocking_literal_value_by_redundance(constraintid wcnflinenumber, bool value){
    // VeriPB adds blocking literal 1 ~_b2 to a soft clause. Hence, if value == 1, we prove the clause 1 ~_b2 >= 1, otherwise, we prove the clause 1 _b2 >= 1. 
    *(PL->proof) << "red 1 " << (value ? "~" : "") << "_b" << std::to_string(wcnflinenumber) << " >= 1; "  << "_b" << std::to_string(wcnflinenumber) << " -> " << (value ? 0 : 1) << "\n";
    PL->increase_constraint_counter();
    PL->move_to_coreset(-1);
}
//=================================================================================================
// Objective reformulation

template <class TVar>
constraintid MaxSATProoflogger::add_core_lower_bound(
    const TVar &lazy_var, constraintid core_id, constraintid pb_definition_id,
    wght weight)
{
    PL->start_intCP_derivation(core_id);
    PL->intCP_add_constraint(pb_definition_id);
    PL->intCP_divide(2);
    constraintid lower_bound_id = PL->end_intCP_derivation();
    counting_var_to_core_idx[varidx(toVeriPbVar(lazy_var))] = core_lower_bounds.size();
    core_lower_bounds.push_back(lower_bound_id);
    core_weights.push_back(weight);
    return lower_bound_id;
}

template <class TVar>
constraintid MaxSATProoflogger::update_core_lower_bound(const TVar &old_lazy_var, const TVar &new_lazy_var, constraintid pb_definition_id, wght bound)
{
    int core_idx = counting_var_to_core_idx[varidx(toVeriPbVar(old_lazy_var))];

    // Update map
    counting_var_to_core_idx[varidx(toVeriPbVar(new_lazy_var))] = core_idx;

    // Update lower bound
    PL->start_intCP_derivation(core_lower_bounds[core_idx]);
    PL->intCP_multiply(bound);
    PL->intCP_add_constraint(pb_definition_id);
    PL->intCP_divide(bound + 1);
    constraintid new_lower_bound_id = PL->end_intCP_derivation();
    PL->delete_constraint_by_id(core_lower_bounds[core_idx]);
    core_lower_bounds[core_idx] = new_lower_bound_id;
    return new_lower_bound_id;
}

constraintid MaxSATProoflogger::derive_objective_reformulation_constraint(constraintid base_reform_id)
{
    PL->start_intCP_derivation(base_reform_id);
    for (int i = 0; i < core_lower_bounds.size(); i++)
    {
        PL->intCP_load_constraint(core_lower_bounds[i]);
        PL->intCP_multiply(core_weights[i]);
        PL->intCP_add();
    }
    return PL->end_intCP_derivation();
}

constraintid MaxSATProoflogger::proof_log_objective_reformulation(constraintid base_reform_id, constraintid model_improve_id)
{
    // Add the lower bounds for all the cores we collected multiplied by the weight of the cores.
    constraintid objective_reform_id = derive_objective_reformulation_constraint(base_reform_id);

    // Add the model improving constraint to derive the desired constraint.
    PL->start_intCP_derivation(model_improve_id);
    PL->intCP_add_constraint(objective_reform_id);
    constraintid lower_bound_reformulated_objective = PL->end_intCP_derivation();
    PL->delete_constraint_by_id(objective_reform_id);
    return lower_bound_reformulated_objective;
}

constraintid MaxSATProoflogger::base_reform_unit_core(constraintid base_reform_id, constraintid core_id, wght weight)
{
    PL->start_intCP_derivation(core_id);
    PL->intCP_multiply(weight);
    PL->intCP_add_constraint(base_reform_id);
    constraintid new_base_reform_id = PL->end_intCP_derivation();
    PL->delete_constraint_by_id(base_reform_id);
    return new_base_reform_id;
}

constraintid MaxSATProoflogger::reformulate_with_unprocessed_cores(constraintid base_reform_id, std::vector<constraintid> core_ids, std::vector<wght> core_weights)
{
    PL->start_intCP_derivation(base_reform_id);

    for (int i = 0; i < core_ids.size(); i++)
    {
        PL->intCP_load_constraint(core_ids[i]);
        PL->intCP_multiply(core_weights[i]);
        PL->intCP_add();
    }

    constraintid reform_constraint_with_unprocessed_cores_id = PL->end_intCP_derivation();
    PL->delete_constraint_by_id(base_reform_id);
    return reform_constraint_with_unprocessed_cores_id;
}

//=================================================================================================
// At-most-one constraints

template <class TSeqLit>
constraintid MaxSATProoflogger::derive_at_most_one_constraint(const TSeqLit &am1_lits, const bool am1_sign)
{
    assert(am1_lits.size() > 1);
    std::vector<constraintid> binary_clauses;

    if(am1_lits.size() == 2){
        std::vector<VeriPB::Lit> lits; 
        lits.push_back(neg(toVeriPbLit(am1_lits[0])));
        lits.push_back(neg(toVeriPbLit(am1_lits[1])));
        return PL->rup(lits, 1);
    }

    for (int new_lit_idx = 1; new_lit_idx < am1_lits.size(); new_lit_idx++)
    {
        if (new_lit_idx != 1) // first constraint is not multiplied
        {
            PL->intCP_multiply(new_lit_idx - 1);
        }

        for (int lit_idx = 0; lit_idx < new_lit_idx; lit_idx++)
        {
            constraintid binary_clause_id; 
            if(am1_sign)
                binary_clause_id = PL->rup(std::vector<VeriPB::Lit>{neg(toVeriPbLit(am1_lits[new_lit_idx])), neg(toVeriPbLit(am1_lits[lit_idx]))});
            else
                binary_clause_id = PL->rup(std::vector<VeriPB::Lit>{toVeriPbLit(am1_lits[new_lit_idx]), toVeriPbLit(am1_lits[lit_idx])});
            
            binary_clauses.push_back(binary_clause_id);

            // the first binary constraint to start the derivation
            if (new_lit_idx == 1)
            {
                PL->start_intCP_derivation(binary_clause_id);
            }
            else
            {
                PL->intCP_add_constraint(binary_clause_id);
            }
        }
        PL->intCP_divide(new_lit_idx);
    }
    constraintid am1_constraint_id = PL->end_intCP_derivation();
    PL->delete_constraint_by_id(binary_clauses);
    return am1_constraint_id;
}

template <class TSeqLit, class TLit>
constraintid MaxSATProoflogger::introduce_at_most_one_selector(const TSeqLit &am1_lits, const TLit &selector_all_lit)
{
    std::vector<VeriPB::Lit> constraint_lits;
    for(int i = 0; i < am1_lits.size(); i++)
        constraint_lits[i] = toVeriPbLit(am1_lits[i]);
    
    VeriPB::Lit sel = toVeriPbLit(selector_all_lit);

    constraint_lits.push_back(sel);

    substitution witness = PL->get_new_substitution();
    PL->add_boolean_assignment(witness, variable(sel), !is_negated(sel));

    constraintid reified_am1_constraint_id = PL->redundanceBasedStrengthening(constraint_lits, am1_lits.size(), witness);

    for (int i = 0; i < constraint_lits.size(); i++)
    {
        constraint_lits[i] = neg(constraint_lits[i]);
    }

    witness =  PL->get_new_substitution();
    PL->add_boolean_assignment(witness, variable(sel), is_negated(sel));
    PL->redundanceBasedStrengthening(constraint_lits, 1, witness);

    return reified_am1_constraint_id;
}

template <class TSeqLit, class TLit>
constraintid MaxSATProoflogger::proof_log_at_most_one(constraintid base_reform_id, const TSeqLit &am1_lits, const TLit &selector_all_lit, wght weight)
{
    constraintid am1_constraint = derive_at_most_one_constraint(am1_lits);
    constraintid am1_lower_bound = introduce_at_most_one_selector(am1_lits, selector_all_lit);
    PL->delete_constraint_by_id(am1_constraint);

    PL->start_intCP_derivation(am1_lower_bound);
    PL->intCP_multiply(weight);
    PL->intCP_add_constraint(base_reform_id);
    constraintid new_base_reform_id = PL->end_intCP_derivation();
    PL->delete_constraint_by_id(base_reform_id);
    PL->delete_constraint_by_id(am1_lower_bound);
    return new_base_reform_id;
}
