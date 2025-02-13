#include "VeriPBProoflogger.h"
/* TODO: 
   - Instantiate important functions for VeriPB::Var, VeriPB::Lit, constraintid as TNumber, ...
   - Replace number_to_string by write_number as much as possible
*/

//=================================================================================================

//prooflogging Library
using namespace VeriPB;

// ------------- Proof file -------------
template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::set_proof_stream(std::ostream* proof){
    this->proof = proof;
    proof->rdbuf()->pubsetbuf(write_buffer, write_buffer_size); 
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::set_keep_original_formula_on(){
    keep_original_formula = true;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::set_keep_original_formula_off(){
    keep_original_formula = false;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_proof_header()
{
    *proof << "pseudo-Boolean proof version 2.0\n";
    *proof << "f\n";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::set_n_orig_constraints(int nbconstraints){
    assert(constraint_counter == 0);
    _constraint_counter = nbconstraints;
    _n_orig_constraints = nbconstraints;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
bool VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::is_original_constraint(const constraintid& cxn){
    return cxn <= _n_orig_constraints;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::get_constraint_counter(){
    return _constraint_counter;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::set_variable_manager(const VarManager* varMgr){
    _varMgr = varMgr;
}

// ------------- Conclusion -------------
template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_conclusion_NONE(){
    *proof << "output NONE\n"
        << "conclusion NONE\n"
        << "end pseudo-Boolean proof\n";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_conclusion_UNSAT(){
    *proof << "output NONE\n"
        << "conclusion UNSAT\n"
        << "end pseudo-Boolean proof\n";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_conclusion_UNSAT_optimization(){
    *proof << "output NONE\n"
        << "conclusion BOUNDS INF INF\n"
        << "end pseudo-Boolean proof\n";
}


template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_conclusion_UNSAT_optimization(const constraintid& hint){
    *proof << "output NONE\n"
        << "conclusion BOUNDS INF : " << hint << " INF\n"
        << "end pseudo-Boolean proof\n";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_conclusion_SAT(){
    *proof << "output NONE\n"
        << "conclusion SAT\n"
        << "end pseudo-Boolean proof\n";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_conclusion_OPTIMAL(){
    if(!found_solution){
        write_conclusion_UNSAT_optimization();
    }
    else{
        *proof << "output NONE\n"
            << "conclusion BOUNDS " << number_to_string(best_objective_value) << " " << number_to_string(best_objective_value) << "\n"
            << "end pseudo-Boolean proof\n";
    }
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_conclusion_OPTIMAL(const constraintid& hint){
    if(!found_solution){
        write_conclusion_UNSAT_optimization();
    }
    else{
        *proof << "output NONE\n"
            << "conclusion BOUNDS " << number_to_string(best_objective_value) << " : " << number_to_string(hint) << " " << number_to_string(best_objective_value) << "\n"
            << "end pseudo-Boolean proof\n";
    }    
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_conclusion_BOUNDS(const ObjConst& LB, const ObjConst& UB){
    *proof << "output NONE\n"
        << "conclusion BOUNDS " << number_to_string(LB) << " " << number_to_string(UB) << "\n"
        << "end pseudo-Boolean proof\n";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_conclusion_BOUNDS(const ObjConst&  LB, const constraintid& hint, const ObjConst&  UB){
    *proof << "output NONE\n"
        << "conclusion BOUNDS " << number_to_string(LB) << " : " << number_to_string(hint) << " " << number_to_string(UB) << "\n"
        << "end pseudo-Boolean proof\n";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_fail(){
    *proof << "fail\n";
}

// ------------- Objective function manipulation -------------
template<typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::set_objective(const LinTermBoolVars<ObjLit, ObjCoeff, ObjConst>* new_objective)
{
    assert(_objective.size() == 0 && _objective.constant() == 0);
    _objective = new_objective;
}


template<typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::add_objective_literal(const ObjLit& lit, const ObjCoeff weight){
    _objective.add_literal(lit, weight);
}

template<typename ObjLit, typename ObjCoeff, typename ObjConst>
bool VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::remove_objective_literal(const ObjLit& lit){
    _objective.delete_literal(lit);
}

template<typename ObjLit, typename ObjCoeff, typename ObjConst>
ObjCoeff VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::get_objective_weight(const ObjLit& lit){
    int i=0;
    while(lit != _objective.literal(i)) i++;

    return _objective.coefficient(i);
}

template<typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::add_objective_constant(const ObjConst& weight){
    _objective.add_constant(weight);
}

template<typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::subtract_objective_constant(const ObjConst& weight){
    _objective.subtract_constant(weight);
}

template<typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_comment_objective_function()
{
    if(!comments) return;

    *proof << "* f = ";
    for (int i = 0; i < _objective.size(); i++)
        write_weighted_literal(_objective.literal(i), _objective.coefficient(i));
    if(_objective.constant() != 0)
        *proof << " + " << number_to_string(_objective.constant());
    
    if(_found_solution)
        *proof << "; Current best solution: " << number_to_string(_best_objective_value);
    else 
        *proof << "; No solution found yet.";
    *proof << "\n";
}


template<typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::check_model_improving_constraint(const constraintid& cxnid){
    assert(_found_solution);

    VeriPB::Constraint<ObjLit, ObjCoeff, ObjConst> mic(_objective, _best_objective_value-1, Comparison::LEQ);
    
    if(cxnid == undefcxn)
        check_constraint_exists(cxn);
    else 
        equals_rule(cxn, cxnid);
}

template<typename ObjLit, typename ObjCoeff, typename ObjConst>
bool VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::logged_solution(){
    return _found_solution;
}

template<typename ObjLit, typename ObjCoeff, typename ObjConst>
ObjConst VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::get_best_objective_value(){
    return _best_objective_value;
}

// ------------- Solution improving -------------

/// @brief Calculate the objective value of the objective stored in VeriPBProoflogger for a model. This function uses the optimistic assumption that the literals are sorted.
/// @param model Vector of assignments to literals.
/// @return Objective value for model.
// TODO-Dieter: add conversion of ObjCoeff to ObjConst.
// TODO-Dieter: Add example of using lbools.
template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <typename TModel>
ObjConst VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::calculate_objective_value(const TModel& model)
{
    ObjConst objective_value = objective_constant_cost;
    ModelValue v;
    for (int i = 0; i < _objective.size(); i++)
    {
        ObjLit objlit = _objective.literal(i);
        v = model_value(variable(objlit), model, i==0);
        if (v == ModelValue::True)
        {
            objective_value += _objective.coefficient(i); // TODO-Dieter: Conversion
        }
        else if(v == ModelValue::Undef){
            throw out_of_range("[CalculateObjectiveValue] Objective literal " + _varMgr->literal_to_string(objlit) + " not assigned a value." );
        }
        model_start_idx = model_idx;
    }
    return objective_value;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <typename TModel>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::log_solution(const TModel &TModel, const ObjConst objective_value, const bool only_original_variables_necessary, const bool log_as_comment)
{
    if(log_as_comment && !comments) return get_model_improving_constraint();

    write_comment("Solution with objective value: " + number_to_string(objective_value));
    write_model(model, log_as_comment);
    
    if(!log_as_comment){ // Veripb automatically adds an improvement constraint so counter needs to be incremented
        _model_improvement_constraint = ++_constraint_counter;

        if(objective_value < _best_objective_value)
            _best_objective_value = objective_value;

        check_model_improving_constraint(-1);
    }

    return get_model_improving_constraint(); 
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TSeqLit>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::log_solution(const TSeqLit &model, bool only_original_variables_necessary=true, bool log_as_comment=false){
    if(size(_objective) > 0){
        ObjConst objVal = calculate_objective_value(model);
        return log_solution(model, objVal, only_original_variables_necessary, log_as_comment);
    }
    else{
        write_model(model, log_as_comment);

        if(!log_as_comment) 
            return ++_constraint_counter;
        else 
            return undefcxn;
    }
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TModel>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::log_solution_with_check(const TModel &model, bool only_original_variables_necessary, bool log_nonimproving_solution_as_comment)
{
    ObjConst current_objective_value = calculate_objective_value(model);
    if (current_objective_value < _best_objective_value)
    {
        if(comments){
            write_comment_objective_function();
            write_comment("Objective update from " + number_to_string(best_objective_value) + " to " + number_to_string(current_objective_value));
        }
        log_solution(model, current_objective_value, only_original_variables_necessary);
    }
    else if(comments && log_nonimproving_solution_as_comment){
        write_comment_objective_function();
        write_comment("Non-improving solution:");
        log_solution(model, current_objective_value, only_original_variables_necessary, true);
    }

    return get_model_improving_constraint();
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::get_model_improving_constraint()
{
    return _model_improvement_constraint;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::update_model_improving_constraint(const constraintid& newmic){
    model_improvement_constraint = newmic;
    if(comments)
        write_comment("Model improving constraint: " + number_to_string(newmic));
}

// ------------- Objective update -------------

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_objective_update(){
    *proof << "obju new";
    for (int i = 0; i < objective_lits.size(); i++)
        write_weighted_literal(_objective.literal(i), _objective.coefficient(i));
    if(_objective.constant() != 0)
        *proof << number_to_string(_objective.constant());
    if(_objective.size() == 0 && _objective.constant() == 0)
        *proof << ' ';
    *proof << ";\n";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TLinTerm>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_objective_update_diff(TLinTerm& oldObj, TLinTerm& newObj){
    *proof << "obju diff";
    for(int i = 0; i < size(oldObj); i++){
        *proof << " -";
        write_number(coefficient(oldObj,i), false);
        _varMgr->write_literal(literal(oldObj, i), proof, true);
    }
    for(int i = 0; i < size(newObj); i++){
        write_number(coefficient(newObj, i));
        _varMgr->write_literal(literal(newObj, i), proof, true);
    }

    *proof << " -";
    write_number(get_constant(oldObj), false);
    write_number(get_constant(newObj), true);
    *proof << ";\n";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TLit>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_objective_update_diff_for_literal(TLit& literal_to_remove, ObjCoeff weight, ObjConst constant_for_lit, bool write_update_model_improving_constraint){
    write_comment("write_objective_update_diff_for_literal. Weight = " + number_to_string(weight));
    *proof << "obju diff -";
    write_number(weight, false);
    _varMgr->write_literal(literal_to_remove, proof, true);
    if(constant_for_lit > 0) 
        write_number(constant_for_lit, true);
    *proof << ";\n";

    if(write_update_model_improving_constraint && get_model_improving_constraint() != 0){
        write_comment("Update model-improving constraint:");
        if(constant_for_lit > 0)
            rup_unit_clause(literal_to_remove, false);

        start_internal_CP_derivation_cxn(get_model_improving_constraint(), true);
        
        if(constant_for_lit > 0){
            CP_add_cxn(-1, weight);
        }
        else{
            CP_add_litaxiom(literal_to_remove, weight);
        }
        constraintid cxn = write_internal_CP_derivation();
        update_model_improving_constraint(cxn);
    }
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TLit> 
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_objective_update_diff_literal_replacement(TLit& literal_to_remove, TLit& literal_to_add, ObjCoeff weight, bool write_update_model_improving_constraint){
    *proof << "obju diff -";
    write_number(weight, false);
    _varMgr->write_literal(literal_to_remove, proof, true);
    write_number(weight);
    _varMgr->write_literal(literal_to_add, proof, true);
    *proof << ";\n";

    if(write_update_model_improving_constraint && get_model_improving_constraint() != 0){
        write_comment("Update model-improving constraint:");
        constraintid cxn_newlit_leq_oldlit = rup_binary_clause(neg(literal_to_add), literal_to_remove);
        
        start_internal_CP_derivation_cxn(get_model_improving_constraint());
        CP_add_cxn(cxn_newlit_leq_oldlit, weight);
        constraintid cxn = write_internal_CP_derivation();
        update_model_improving_constraint(cxn);
    }
}

// ------------- Cutting Planes derivations -------------
//TODO-Dieter: Change whereever I wrote *proof << var_name(var) to write immediately to the proof.
template <typename ObjLit, typename ObjCoeff, typename ObjConst>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::copy_constraint(const constraintid cxn){
    *proof << "p " << cxn << "\n";
    return ++_constraint_counter;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
cuttingplanes_derivation VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::start_CP_derivation(){
    return "";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
cuttingplanes_derivation VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::start_CP_derivation_cxn(const constraintid& cxn_id){
    return number_to_string(cxn_id);
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TLit>
cuttingplanes_derivation VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::start_CP_derivation_lit_axiom(const TLit& lit_axiom){
    return _varMgr->literal_to_string(lit_axiom);
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::CP_add(cuttingplanes_derivation& cpder1, const cuttingplanes_derivation& cpder2){
    cpder1 += " " + cpder2 + " +";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::CP_start_subderivation_cxn_id(cuttingplanes_derivation& cpder, const constraintid& cxn_id){
    cpder += " " + number_to_string(cxn_id);
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TLit>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::CP_start_subderivation_lit_axiom(cuttingplanes_derivation& cpder, const TLit& lit_axiom){
    cpder += " " + _varMgr->literal_to_string(li);
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::CP_add_subderivation(cuttingplanes_derivation& cpder){
    cpder += " +";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TNumber>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::CP_add_cxn(cuttingplanes_derivation& cpder, const constraintid& cxn_id, const TNumber& mult=1){
    cpder += " " + number_to_string(cxn_id);
    if(mult > 1)
        cpder += " " + number_to_string(mult);
    cpder += " +";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TLit, class TNumber>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::CP_add_litaxiom(cuttingplanes_derivation& cpder, const TLit& lit_axiom, const TNumber& mult=1){
    cpder += " " + _varMgr->literal_to_string(lit_axiom);
    if(mult > 1)
        cpder += " " + number_to_string(mult);
    cpder += " +";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TNumber>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::CP_divide(cuttingplanes_derivation& cp, const TNumber& n){
    cpder += " " + number_to_string(n) + " d";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::CP_saturate(cuttingplanes_derivation& cp){
    cpder += " s";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TNumber>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::CP_multiply(cuttingplanes_derivation& cp, const TNumber& n){
    cpder += " " + number_to_string(n) + " *";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TVar>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::CP_weaken(cuttingplanes_derivation& cp, const TVar& var){
    cpder += " " + var_name(var) + " w";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_CP_derivation(const cuttingplanes_derivation& cp){
    *proof << "p " << cp << "\n";
    return ++_constraint_counter;
}

// internal CP derivation
template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::start_internal_CP_derivation(bool write_directly_to_proof=false){
    _writing_CP_to_proof = write_directly_to_proof;

    if(_writing_CP_to_proof)
        *proof << "p";
    else
        _internal_cuttingplanes_buffer = "";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::start_internal_CP_derivation_cxn(const constraintid& cxn_id,bool write_directly_to_proof=false){
    _writing_CP_to_proof = write_directly_to_proof;

    if(_writing_CP_to_proof){
        *proof << "p";
        write_number(cxn_id);
    }
    else
        _internal_cuttingplanes_buffer = number_to_string(cxn_id);

}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TLit>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::start_internal_CP_derivation_lit_axiom(const TLit& lit_axiom, bool write_directly_to_proof=false){
    _writing_CP_to_proof = write_directly_to_proof;

    if(_writing_CP_to_proof){
        *proof << "p";
        _varMgr->write_literal(lit_axiom, proof, true);
    }
    else
        _internal_cuttingplanes_buffer = _varMgr->literal_to_string(lit_axiom);
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::CP_add(const cuttingplanes_derivation& cpder2){
    if(_writing_CP_to_proof)
        *proof << " " << cpder2 << " +";
    else
        CP_add(_internal_cuttingplanes_buffer, cpder2);
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::CP_start_subderivation_cxn_id(const constraintid& cxn_id){
    if(_writing_CP_to_proof)
        *proof << " " << number_to_string(cxn_id) ;
    else
        CP_start_subderivation_cxn_id(_internal_cuttingplanes_buffer, cxn_id);
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TLit>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::CP_start_subderivation_lit_axiom(const TLit& lit_axiom){
    if(_writing_CP_to_proof){
        _varMgr->write_literal(lit_axiom, proof, true);
    }
    else
        CP_start_subderivation_lit_axiom(_internal_cuttingplanes_buffer, lit_axiom);
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::CP_add_subderivation(){
    if(_writing_CP_to_proof)
        *proof << " +";
    else
        CP_add_subderivation(_internal_cuttingplanes_buffer);
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TNumber>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::CP_add_cxn(const constraintid& cxn_id, const TNumber& mult=1){
    if(_writing_CP_to_proof){
        *proof << " " << number_to_string(cxn_id); 
        if(mult > 1)
            *proof << " " << number_to_string(mult) << " *";
        *proof << " +";
    }
    else
        CP_add_cxn(_internal_cuttingplanes_buffer, cxn_id, mult);
    
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TLit, class TNumber>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::CP_add_litaxiom(const TLit& lit_axiom, const TNumber& mult=1){
    if(_writing_CP_to_proof){
        _varMgr->write_literal(lit_axiom, proof, true);
        if(mult > 1)
            write_number(mult,true); 
        *proof << " +";
    }
    else
        CP_add_litaxiom(_internal_cuttingplanes_buffer, cxn_id, mult);    
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TNumber>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::CP_divide(const TNumber& n){
    if(_writing_CP_to_proof)
        *proof << " " << number_to_string(n) << " d";
    else
        CP_divide(_internal_cuttingplanes_buffer, n);
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::CP_saturate(){
    if(_writing_CP_to_proof)
        *proof << " s";
    else
        CP_saturate(_internal_cuttingplanes_buffer);
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TNumber>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::CP_multiply(const TNumber& n){
    if(_writing_CP_to_proof){
       write_number(n);
       *proof << " *";
    }
    else
        CP_multiplication(_internal_cuttingplanes_buffer, n);
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TVar>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::CP_weaken(const TVar& var){
    if(_writing_CP_to_proof){
        *proof << var_name(var) << " w";
    }
    else
        CP_weaken(_internal_cuttingplanes_buffer, var);
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_internal_CP_derivation(){
    if(_writing_CP_to_proof)
        *proof << "\n";
    else
        write_CP_derivation(_internal_cuttingplanes_buffer);
    return ++_constraint_counter;
    
}

// ------------- Comments -------------
template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_comment(const char *comment)
{
    if(comments)
        *proof << "* " << comment << "\n";
    // proof->flush(); // Can be uncommented for debugging reasons
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_comment(const std::string &comment)
{
    if(comments)
        *proof << "* " << comment << "\n";
    // proof->flush(); // Can be uncommented for debugging reasons
}

// ------------- Rules for checking constraints -------------
template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TConstraint>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::equals_rule(const TConstraint& cxn, const constraintid cxn_id)
{
    *proof << "e ";
    write_constraint(cxn);
    *proof << "; ";
    write_number(cxn_id);
    *proof << "\n";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TConstraint>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::check_last_constraint(const TConstraint& cxn)
{
    equals_rule(cxn, -1);
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TConstraint>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::check_constraint_exists(const TConstraint& cxn)
{
    *proof << "e ";
    write_constraint(cxn);
    *proof << ";\n";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TConstraint>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::check_implied(const TConstraint& cxn, constraintid cxn_id){
    *proof << "i ";
    write_constraint(cxn);
    if(cxn_id == undefcxn)
        *proof << ";\n";
    else{
        *proof << ";";
        write_number(cxn_id);
        *proof << "\n";
    }
        
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TConstraint>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::check_implied(const TConstraint& cxn){
    check_implied(cxn, undefcxn);
}

// ------------- Rules for adding syntactically implied constraints -------------
template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TConstraint>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::derive_if_implied(const TConstraint& cxn){
    derive_if_implied(cxn, undefcxn);
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TConstraint>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::derive_if_implied(const TConstraint& cxn, const constraintid& cxn_id){
    *proof << "ia";
    write_constraint(cxn);
    *proof << ";";
    if(cxn_id != undefcxn)
        write_number(cxn_id);
    *proof << "\n";
    return ++_constraint_counter;
}

// ------------- Unchecked Assumptions -------------
template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TConstraint>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::unchecked_assumption(const TConstraint& cxn)
{
    *proof << "a";
    write_constraint(cxn);
    *proof << " ;\n";
    return ++_constraint_counter;
}

// ------------- Reverse Unit Propagation -------------
template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TConstraint>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::rup(const TConstraint& cxn, bool core_constraint=false){
    *proof << "rup";
    write_constraint(cxn);
    *proof << ";\n";
    if(core_constraint)
        move_to_coreset(-1);
    return ++_constraint_counter;
}
template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TConstraint>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::rup(const TConstraint& cxn, const std::vector<constraintid>& hints, const bool core_constraint=false){
    *proof << "rup";
    write_constraint(cxn);
    *proof << ";";
    write_hints(hints);
    *proof << "\n";
    return ++_constraint_counter;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TConstraint>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::rup_clause(const TConstraint& clause){
    *proof << "rup";
    write_clause(clause)
    *proof << ";\n";
    return ++_constraint_counter;
}
template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TConstraint>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::rup_clause(const TConstraint& clause, std::vector<constraintid>& hints){
    *proof << "rup";
    write_clause(clause);
    *proof << ";";
    write_hints(hints);
    *proof << "\n";
    return ++_constraint_counter;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TLit>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::rup_unit_clause(const TLit& lit, bool core_constraint=true){
    *proof << "rup";
    write_weighted_literal(lit);
    *proof << " >= 1;\n";
    if(core_constraint)
        move_to_coreset(-1);
    return ++_constraint_counter;
}
template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TLit>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::rup_unit_clause(const TLit& lit, std::vector<constraintid>& hints, bool core_constraint=true){
    *proof << "rup";
    write_weighted_literal(lit);
    *proof << " >= 1;";
    write_hints(hints);
    *proof << "\n";
    if(core_constraint)
        move_to_coreset(-1);
    return ++_constraint_counter;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TLit>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::rup_binary_clause(const TLit& lit1, const TLit& lit2, bool core_constraint=false){
    *proof << "rup";
    write_weighted_literal(lit1);
    write_weighted_literal(lit2);
    *proof << " >= 1;\n";
    if(core_constraint)
        move_to_coreset(-1);
    return ++_constraint_counter;
}
template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TLit>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::rup_binary_clause(const TLit& lit1, const TLit& lit2, std::vector<constraintid>& hints,  bool core_constraint=false){
    *proof << "rup";
    write_weighted_literal(lit1);
    write_weighted_literal(lit2);
    *proof << " >= 1;";
    if(core_constraint)
        move_to_coreset(-1);
    write_hints(hints);
    *proof << "\n";
    return ++_constraint_counter;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TLit> 
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::rup_ternary_clause(const TLit& lit1, const TLit& lit2, const TLit& lit3, bool core_constraint=false){
    *proof << "rup";
    write_weighted_literal(lit1);
    write_weighted_literal(lit2);
    write_weighted_literal(lit3);
    *proof << " >= 1;\n";
    if(core_constraint)
        move_to_coreset(-1);
    return ++_constraint_counter;
}
template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TLit> 
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::rup_ternary_clause(const TLit& lit1, const TLit& lit2, const TLit& lit3, std::vector<constraintid>& hints, bool core_constraint=false){
    *proof << "rup";
    write_weighted_literal(lit1);
    write_weighted_literal(lit2);
    write_weighted_literal(lit3);
    *proof << " >= 1;";
    if(core_constraint)
        move_to_coreset(-1);
    write_hints(hints);
    *proof << "\n";
    return ++_constraint_counter;
}

// ------------- Redundance Based Strenghtening -------------
template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::strenghten_to_core_on(){
    *proof << "strengthening_to_core on\n"; 
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::strenghten_to_core_off(){
    *proof << "strengthening_to_core off\n"; 
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
substitution VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::get_new_substitution(){
    substitution s;
    return s;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TVar>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::add_boolean_assignment(substitution &s, const TVar& var, const bool value){
    s.second.push_back(std::make_pair(toVeriPbVar(var), value));
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TVar, class TLit>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::add_literal_assignment(substitution &s, const TVar& var, const TLit& value){
    s.first.push_back(std::make_pair(toVeriPbVar(var), toVeriPbLit(value)));
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::add_substitution(substitution &sub, const substitution &sub_to_add){
    sub.first.reserve(sub.first.size() + sub_to_add.first.size());
    for(auto lit_ass : sub_to_add.first)
        add_literal_assignment(sub, lit_ass.first, lit_ass.second);
    
    sub.second.reserve(sub.second.size() + sub_to_add.second.size());
    for(auto bool_ass : sub_to_add.second)
        add_boolean_assignment(sub, bool_ass.first, bool_ass.second);
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_substitution(const substitution &witness)
{
    VeriPB::Var varfrom, varto;
    VeriPB::Lit lit_to;
    bool rewritten_to_negated_literal, boolto;

    for(auto lit_ass : witness.first){
        varfrom = lit_ass.first;
        lit_to = lit_ass.second;
        varto = variable(lit_to);

        rewritten_to_negated_literal = write_variable_after_possible_rewrite(proof, varfrom);
        *proof << " -> ";

        lit_to.negated ^= rewritten_to_negated_literal;
        _varMgr->write_literal(lit_to, proof, false);
    }
    for(auto bool_ass : witness.second){
        varfrom = bool_ass.first;
        boolto = bool_ass.second;

        rewritten_to_negated_literal = write_variable_after_possible_rewrite(proof, varfrom);
        *proof << " -> ";

        boolto ^= rewritten_to_negated_literal;
        *proof << boolto;
    }   
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TVar>
bool VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::has_boolean_assignment(const substitution &s, const TVar& var){
    for(auto ass : s.second){
        if(ass.first == toVeriPbVar(var))
            return true;
    }
    return false;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TVar>
bool VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::has_literal_assignment(const substitution &s, const TVar& var){
    for(auto ass : s.first){
        if(ass.first == toVeriPbVar(var))
            return true;
    }
    return false;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TVar>
bool VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::get_boolean_assignment(substitution &s, const TVar& var){
    for(auto ass : s.second){
        if(ass.first == toVeriPbVar(var))
            return ass.second;
    }
    std::cout << "ERROR: Proof logging library: Could not find boolean assignment for variable " << var_name(var) << std::endl;
    assert(false);
    return false;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TVar>
VeriPB::Lit VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::get_literal_assignment(substitution &s, const TVar& var){
    for(auto ass : s.first){
        if(ass.first == toVeriPbVar(var))
            return ass.second;
    }
    std::cout << "ERROR: Proof logging library: Could not find literal assignment for variable " << var_name(var) << std::endl;
    assert(false);
    return VeriPB::lit_undef;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
size_t VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::get_substitution_size(const substitution &s){
    return s.first.size() + s.second.size();
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TConstraint>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::redundance_based_strengthening(const TConstraint& cxn, const substitution& witness){
    *proof << "red";
    write_constraint(cxn);
    *proof << "; ";
    write_substitution(witness);
    *proof << "\n";
    return ++constraint_counter;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TLit>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::redundance_based_strengthening_unit_clause(const TLit& lit){
    *proof << "red";
    write_weighted_literal(lit);
    *proof << ">= 1; "; 
    
    VeriPB::Var var = toVeriPbVar(variable(lit));
    bool rewritten_to_negated_literal = write_variable_after_possible_rewrite(proof, var, is_negated(lit));
    *proof << " -> ";
    *proof << !rewritten_to_negated_literal << "\n";
    
    write_comment("variable: " + var_name(var) + " negated literal = " + number_to_string(is_negated(lit)) + " rewritten_to_negated_lit = " + number_to_string(rewritten_to_negated_literal));

    return ++constraint_counter;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <typename TConstraint>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::redundance_based_strengthening(const TConstraint& cxn, const substitution& witness, const std::vector<subproof>& subproofs){
    *proof << "red";
    write_constraint(cxn);
    *proof << "; ";
    write_substitution(witness);

    if(subproofs.size() > 0){
        _constraint_counter++; // constraint not C
        *proof << "; begin \n";
    
        for(int i = 0; i < subproofs.size(); i++){
            subproof p = subproofs[i];
            _constraint_counter++; // constraint C\w
            *proof << "\tproofgoal " << p.proofgoal << "\n";
            for(int i = 0; i < p.derivations.size(); i++){
                *proof << "\t\tp " << p.derivations[i] << "\n";
                constraint_counter++;
            }
            // *proof << "\t\t c -1\n";
            *proof << "\tend -1\n";
        }
        *proof << "end";
    }
    *proof << "\n";

    return ++_constraint_counter;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TConstraint>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::start_redundance_based_strengthening_with_subproofs(const TConstraint& cxn, const substitution& witness){
    *proof << "red";
    write_constraint(cxn);
    *proof << "; ";
    write_substitution(witness);
    *proof << "; begin \n";
    return ++_constraint_counter; // constraint not C
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::start_new_subproof(const std::string& proofgoal){
    *proof << "\tproofgoal " << proofgoal << "\n";
    return ++_constraint_counter; // constraint C\w
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::end_subproof(){
    *proof << "\tend -1\n";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::end_redundance_based_strengthening_with_subproofs(){
    *proof << "end\n";
    return ++_constraint_counter;
}

// ------------- Reification -------------

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TLit, class TConstraint>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::reification_literal_right_implication(const TLit& lit, const TConstraint& cxn, const bool store_reified_constraint){
    int i;

    if(comments){
        // TODO-Dieter: Make comment a string in the PL library to not always have to take the memory again.
        std::string comment = _varMgr->literal_to_string(lit) + " -> " ;
        for(i = 0; i < lits.size(); i++)
            comment += number_to_string(weights[i]) + " " + _varMgr->literal_to_string(lits[i]) + " ";
        comment += (comparison(cxn) == Comparison::GEQ ? ">= " : "=< ") + number_to_string(RHS);
        write_comment(comment);
    }

    *proof << "red";
    if(get_comparison(cxn) == Comparison::GEQ){
        write_weighted_literal(neg(lit), rhs(cxn));
        for(int i = 0; i < size(cxn); i++)
            write_weighted_literal(literal(cxn, i), coefficient(cxn,i));
        *proof << ">= ";
        write_number(rhs(cxn));
        *proof << "; ";
    }
    else{
        ObjConst RHSreif = sum_of_coefficients(cxn) - rhs(cxn);
        write_weighted_literal(lit, RHSreif);
        for(int i = 0; i < size(cxn); i++)
            write_weighted_literal(neg(literal(cxn, i)), coefficient(cxn,i));
        *proof << ">= ";
        write_number(RHSreif);
        *proof << "; ";
    }

    VeriPB::Var var = toVeriPbVar(variable(lit));
    substitution witness = get_new_substitution();
    add_boolean_assignment(witness, var, is_negated(lit)); // Set lit to false makes reification constraint true. So that means that if lit is negated, we need to set the variable true.
    write_substitution(witness);
    *proof << "\n";
    ++_constraint_counter;
    if(store_reified_constraint)
        store_reified_constraint_right_implication(var, _constraint_counter);
    return _constraint_counter;
}     

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TLit, class TConstraint>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::reification_literal_left_implication(const TLit& lit, const TConstraint& cxn, const bool store_reified_constraint){
    int i;

    if(comments){
        std::string comment = _varMgr->literal_to_string(lit) + " <- " ;
        for(i = 0; i < lits.size(); i++)
            comment += number_to_string(weights[i]) + " " + _varMgr->literal_to_string(lits[i]) + " ";
        comment += (comparison(cxn) == Comparison::GEQ ? ">= " : "=< ") + number_to_string(RHS);
        write_comment(comment);
    }

    *proof << "red ";
    if(get_comparison(cxn) == Comparison::GEQ){
        ObjConst RHSreif = sum_of_coefficients(cxn) - rhs(cxn) + 1;
        write_weighted_literal(lit, RHSreif);
        for(int i = 0; i < size(cxn); i++)
            write_weighted_literal(neg(literal(cxn,i)), coefficient(cxn,i));
        *proof << ">= ";
        write_number(RHSreif);
        *proof << "; ";
    }
    else{
        ObjConst RHSreif = rhs(cxn) + 1;
        write_weighted_literal(lit, RHSreif);
        for(int i = 0; i < size(cxn); i++)
            write_weighted_literal(literal(cxn,i), coefficient(cxn,i));
        *proof << ">= ";
        write_number(RHSreif);
        *proof << "; ";
    }

    VeriPB::Var var = toVeriPbVar(variable(lit));
    substitution witness = get_new_substitution();
    add_boolean_assignment(witness, var, !is_negated(lit)); // Set lit to true makes reification constraint true. So that means that if lit is not negated, we need to set the variable true.
    write_substitution(witness);
    *proof << "\n";
    ++_constraint_counter;
    if(store_reified_constraint)
        store_reified_constraint_left_implication(var, _constraint_counter);
    return _constraint_counter;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TVar>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::delete_reified_constraint_left_implication(const TVar& var){
    VeriPB::Var _var = toVeriPbVar(var);

    std::vector<constraintid>* storage = _var.only_known_in_proof ? &reifiedConstraintLeftImplOnlyProofVars : &reifiedConstraintLeftImpl;

    if(_var.v <= storage->size()){
        delete_constraint_by_id((*storage)[_var.v]);
        (*storage)[_var.v] = undefcxn;
    }
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TVar>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::delete_reified_constraint_right_implication(const TVar& var){
    VeriPB::Var _var = toVeriPbVar(var);

    std::vector<constraintid>* storage = _var.only_known_in_proof ? &reifiedConstraintRightImplOnlyProofVars : &reifiedConstraintRightImpl;

    if(_var.v <= storage->size()){
        delete_constraint_by_id((*storage)[_var.v]);
        (*storage)[_var.v] = undefcxn;
    }
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TVar>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::get_reified_constraint_left_implication(const TVar& var){
    VeriPB::Var _var = toVeriPbVar(var);
    std::vector<constraintid>* storage = _var.only_known_in_proof ? &reifiedConstraintLeftImplOnlyProofVars : &reifiedConstraintLeftImpl;
    constraintid cxn = undefcxn;
    if(_var.v < storage->size())
        cxn = (*storage)[_var.v];

    return cxn;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TVar>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::get_reified_constraint_right_implication(const TVar& var){
    VeriPB::Var _var = toVeriPbVar(var);
    std::vector<constraintid>* storage = _var.only_known_in_proof ? &reifiedConstraintRightImplOnlyProofVars : &reifiedConstraintRightImpl;
    constraintid cxn = undefcxn;
    if(_var.v < storage->size())
        cxn = (*storage)[_var.v];

    return cxn;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TVar>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::store_reified_constraint_left_implication(const TVar& var, const constraintid& cxnId){
    VeriPB::Var _var = toVeriPbVar(var);
    std::vector<constraintid>* storage = _var.only_known_in_proof ? &reifiedConstraintLeftImplOnlyProofVars : &reifiedConstraintLeftImpl;
    
    // Increase storage if necessary.
    if(_var.v >= storage->size() && INIT_NAMESTORAGE > _var.v ){
        storage->resize(INIT_NAMESTORAGE, undefcxn);
    }
    else if(_var.v >= storage->size()) {
        storage->resize(2 * _var.v, undefcxn);
    }
    (*storage)[_var.v] = cxnId;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TVar>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::store_reified_constraint_right_implication(const TVar& var, const constraintid& cxnId){
    VeriPB::Var _var = toVeriPbVar(var);
    std::vector<constraintid>* storage = _var.only_known_in_proof ? &reifiedConstraintRightImplOnlyProofVars : &reifiedConstraintRightImpl;
    
    // Increase storage if necessary.
    if(_var.v >= storage->size() && INIT_NAMESTORAGE > _var.v ){
        storage->resize(INIT_NAMESTORAGE, undefcxn);
    }
    else if(_var.v >= storage->size()) {
        storage->resize(2 * _var.v, undefcxn);
    }
    (*storage)[_var.v] = cxnId;
}

/**
 * Remove the right reification constraint from the reification constraint store without deleting it in the proof. 
 * Only needed for not maintaining a constraint id in memory that will not be used in the proof anymore.
*/
template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TVar>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::remove_reified_constraint_right_implication_from_constraintstore(const TVar& var){
    VeriPB::Var _var = toVeriPbVar(var);

    std::vector<constraintid>* storage = _var.only_known_in_proof ? &reifiedConstraintRightImplOnlyProofVars : &reifiedConstraintRightImpl;

    if(_var.v <= storage->size()){
        (*storage)[_var.v] = undefcxn;
    }
}

/**
 * Remove the left reification constraint from the reification constraint store without deleting it in the proof. 
 * Only needed for not maintaining a constraint id in memory that will not be used in the proof anymore.
*/
template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TVar>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::remove_reified_constraint_left_implication_from_constraintstore(const TVar& var){
    VeriPB::Var _var = toVeriPbVar(var);

    std::vector<constraintid>* storage = _var.only_known_in_proof ? &reifiedConstraintLeftImplOnlyProofVars : &reifiedConstraintLeftImpl;

    if(_var.v <= storage->size()){
        delete_constraint_by_id((*storage)[_var.v]);
        (*storage)[_var.v] = undefcxn;
    }
}


// ------------- Proof by contradiction -------------
template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TConstraint>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::start_proof_by_contradiction(const TConstraint& cxn){
    VeriPB::substitution w = get_new_substitution();
    start_redundance_based_strengthening_with_subproofs(cxn, w);
    return start_new_subproof("#1");
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::end_proof_by_contradiction(){
    end_subproof();
    return end_redundance_based_strengthening_with_subproofs();
}

// ------------- Proof by case splitting -------------
template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TConstraint>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::prove_by_casesplitting(const TConstraint& cxn, const constraintid& case1, const constraintid& case2){
    constraintid cxnNeg = start_proof_by_contradiction(cxn);
    start_internal_CP_derivation_cxn(cxnNeg, true);
    CP_add_cxn(case1);
    CP_start_subderivation_cxn_id(cxnNeg);
    CP_add_cxn(case2);
    CP_add_subderivation();
    write_internal_CP_derivation();
    return end_proof_by_contradiction();
}

// ------------- Deleting & Overwriting Constraints -------------
template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::delete_constraint_by_id(const constraintid constraint_id, bool overrule_keeporiginalformula=false){
    *proof << "del id";
    if(!(keep_original_formula && is_original_constraint(id)) || overrule_keeporiginalformula)
        write_number(constraint_id, true);
    *proof << "\n";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::delete_constraint_by_id(const std::vector<constraintid> &constraint_ids, bool overrule_keeporiginalformula)
{
    *proof << "del id";
    for (constraintid id : constraint_ids)
    {
        if(!(keep_original_formula && is_original_constraint(id)) || overrule_keeporiginalformula)
            *proof << " " << id;
    }
    *proof << "\n";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TConstraint>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::delete_constraint(const TConstraint& cxn, const bool overrule_keeporiginalformula)
{
    assert(!keep_original_formula || overrule_keeporiginalformula);

    *proof << "del spec";
    write_constraint(cxn);
    *proof << ";\n";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TConstraint>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::delete_constraint(const TConstraint& cxn, const substitution& witness, bool overrule_keeporiginalformula=false){
    assert(!keep_original_formula || overrule_keeporiginalformula);
    write_constraint(cxn);
    *proof << "; ";
    write_substitution(witness);
    *proof << "\n";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TConstraint>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::delete_clause(const TConstraint& cxn, bool overrule_keeporiginalformula){
    assert(!keep_original_formula || overrule_keeporiginalformula);
    *proof << "del spec";
    write_clause(cxn);
    *proof << ";\n";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TConstraint>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::overwrite_constraint(const constraintid& orig_cxn_id, const TConstraint& new_cxn, bool origclause_in_coreset){
    constraintid new_cxn_id = rup(new_cxn, origclause_in_coreset);
    delete_constraint_by_id(orig_cxn_id);
    return new_cxn_id;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TConstraint>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::overwrite_constraint(const TConstraint& orig_cxn, const TConstraint& new_cxn, bool origclause_in_coreset){
    constraintid new_cxn_id = rup(new_cxn, origclause_in_coreset);
    delete_constraint(orig_cxn);
    return new_cxn_id;
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst> 
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::move_to_coreset(const constraintid& cxn_id, bool overrule_keeporiginalformula){
    if(!keep_original_formula || overrule_keeporiginalformula){
        *proof << "core id ";
        write_number(cxn_id);
        *proof << "\n";
    }

}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TConstraint>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::move_to_coreset(const TConstraint& cxn, bool overrule_keeporiginalformula){
    *proof << "core id ";
    write_constraint(cxn);
    *proof << "\n";
}

// -----------------------------------------------------




#ifdef LBOOLS
template <class TSeqLBool>
constraintid VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::log_solution_lbools(TSeqLBool &model, wght objective_value)
{
    if(comments)
        write_comment("Model improvement update with objective value = " + number_to_string(objective_value));

    VeriPB::Var var;
    VeriPB::Lit lit;

    *proof << "soli";
    for (int i = 0; i < model.size(); i++){
        var = toVeriPbVar(i);
        lit = create_literal(var, !toBool(model[i]));

        if(!is_aux_var(var))
            write_literal(lit);
    }
    *proof << "\n";

    // Veripb automatically adds an improvement constraint so counter needs to be incremented
    model_improvement_constraint = ++constraint_counter;

    if(objective_value < best_objective_value)
        best_objective_value = objective_value;

    return model_improvement_constraint;
}
#endif

// ------------- Helping functions -------------

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <class TLit>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_weighted_literal(const TLit &literal, const ObjCoeff& weight = 1, const bool& add_prefix_space=true){
    write_number(weight, add_prefix_space);
    _varMgr->write_literal(toVeriPbLit(lit), proof, true);
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <typename TModel>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_model(const TModel& model, const bool log_as_comment){
    if(log_as_comment) 
        *proof << "* ";
    *proof << "soli";
    VeriPB::Lit lit; 
    for (int i = 0; i < model_size(model); i++){
        lit = toVeriPbLit(model_literal(i, model));
        if(only_original_variables_necessary && is_aux_var(variable(lit)))
            continue;
        _varMgr->write_literal(lit, proof, true);
    }
    *proof << "\n";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <typename TConstraint>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_constraint(const TConstraint& cxn){
    bool normalized = comparison(cxn) == Comparison::GEQ

    for(int i = 0; i < size(cxn); i++){
        assert(coefficient(cxn,i) > 0);
        if(normalized)
            write_weighted_literal(literal(cxn,i), coefficient(cxn,i));
        else
            write_weighted_literal(neg(literal(cxn,i)), coefficient(cxn,i));
    }

    *proof << " >= ";
    if(normalized)
        write_number(rhs(cxn),false);
    else
        write_number(sum_of_coefficients(cxn) - rhs(cxn), false);
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
template <typename TClause>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_clause(const TClause& cxn){
    VeriPB::Lit prevlit, currentlit;

    for(int i = 0; i < size(clause); i++){
        prevlit = currentlit;
        currentlit = toVeriPbLit(literal(cxn, i));

        assert(coefficient(cxn, i) == 1);
        assert(i == 0 || prevlit <= currentlit);

        if(prevlit != currentlit)
            write_weighted_literal(currentlit);
    }
    *proof << " >= 1";
}

template <typename ObjLit, typename ObjCoeff, typename ObjConst>
void VeriPbProofLogger<ObjLit, ObjCoeff, ObjConst>::write_hints(std::vector<constraintid>& hints){
    for(constraintid cxn_id : hints)
        write_number(cxn_id);
}



//=================================================================================================