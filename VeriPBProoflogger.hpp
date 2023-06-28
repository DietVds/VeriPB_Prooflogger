#include "VeriPBProoflogger.h"

//=================================================================================================

//prooflogging Library


// ------------- Proof file -------------

void VeriPbProofLogger::set_proof_stream(std::ostream* proof){
    this->proof = proof;
}

void VeriPbProofLogger::write_proof_header(int nbclause, int nbvars)
{
    n_variables = nbvars;

    *proof << "pseudo-Boolean proof version 1.0\n";
    *proof << "f " << nbclause << "\n";
}

void VeriPbProofLogger::write_proof_header(int nbvars)
{
    n_variables = nbvars;

    *proof << "pseudo-Boolean proof version 1.0\n";
    *proof << "f\n";
}

void VeriPbProofLogger::write_proof_header()
{
    *proof << "pseudo-Boolean proof version 1.0\n";
    *proof << "f\n";
}

void VeriPbProofLogger::set_n_variables(int nbvars){

    assert(n_variables == 0);
    n_variables = nbvars;
}

void VeriPbProofLogger::set_n_constraints(int nbconstraints){
    assert(constraint_counter == 0);
    constraint_counter = nbconstraints;
}

template<class TSeqLit, class TSeqWght>
void VeriPbProofLogger::set_objective(const TSeqLit &lits, const TSeqWght &weights, wght constant_cost)
{
    objective_lits.reserve(lits.size());
    objective_weights.reserve(weights.size());

    for(int i = 0; i < lits.size(); i++){
        objective_lits[i] = toVeriPbLit(lits[i]);
    }

    for(int i = 0; i < weights.size(); i++){
        objective_weights[i] = weights[i];
    }
    
    objective_constant_cost = constant_cost;
}

template <class TLit> 
void VeriPbProofLogger::add_objective_literal(TLit lit, wght weight){
    objective_lits.push_back(toVeriPbLit(lit));
    objective_weights.push_back(weight);
}

void VeriPbProofLogger::add_objective_constant(wght weight){
    objective_constant_cost += weight;
}

void VeriPbProofLogger::write_comment_objective_function()
{
    *proof << "* f = ";
    for (int i = 0; i < objective_lits.size(); i++)
        write_weighted_literal(objective_lits[i], objective_weights[i]);
    if(objective_constant_cost != 0)
        *proof << " + " << std::to_string(objective_constant_cost);
    *proof << "\n";
}

wght VeriPbProofLogger::get_best_objective_function(){
    return best_objective_value;
}

void VeriPbProofLogger::increase_n_variables(){
    n_variables++;
}

void VeriPbProofLogger::increase_constraint_counter(){
    constraint_counter++;
}
// ------------- Helping functions -------------

void VeriPbProofLogger::write_comment(const char *comment)
{
    if(comments)
        *proof << "* " << comment << "\n";
}

void VeriPbProofLogger::write_comment(const std::string &comment)
{
    if(comments)
        *proof << "* " << comment << "\n";
}

template <class TVar>
bool VeriPbProofLogger::is_aux_var(const TVar &var)
{
    return (varidx(toVeriPbVar(var)) > n_variables);
}

template <class TVar>
std::string VeriPbProofLogger::var_name(const TVar &var)
{
    std::string name;

    if (meaningful_names_store.find(varidx(toVeriPbVar(var))) != meaningful_names_store.end())
    {
        name = meaningful_names_store[varidx(toVeriPbVar(var))];
    }
    else if (is_aux_var(var))
    {
        name = "y";
        name += std::to_string(varidx(toVeriPbVar(var)));
    }
    else
    {
        name = "x";
        name += std::to_string(varidx(toVeriPbVar(var)));
    }
    return name;
}

template <class TLit>
void VeriPbProofLogger::write_weighted_literal(const TLit &literal, wght weight)
{
    *proof << std::to_string(weight) + " ";
    write_literal<TLit>(literal);
}

// The variable is the original variable of which we look to rewrite. In the first call, the literal is over the original variable.
// Next, it is checked whether the variable over which the literal is, should again be rewritten (i.e., variable x should be rewritten by literal ~y, and variable y should be rewritten to literal z).
// Once the variable of litvar is finaly not rewritten or the original variable, the correct literal is written.
std::string VeriPbProofLogger::to_string_rewrite_var_by_literal(VeriPB::Var& var, VeriPB::Lit& lit){
    VeriPB::Var litvar = variable(lit);

    if(map_rewrite_var_by_literal.find(varidx(litvar)) == map_rewrite_var_by_literal.end()){
        return (is_negated(lit) ? "~" : "") + var_name(litvar);
    }
    else{
        VeriPB::Lit lit_to_rewrite_to = map_rewrite_var_by_literal[varidx(litvar)];

        if(is_negated(lit))
            lit_to_rewrite_to = neg(lit_to_rewrite_to);

        if(variable(lit_to_rewrite_to) == var)
            return (is_negated(lit_to_rewrite_to) ? "~" : "") + var_name(variable(lit_to_rewrite_to));
        else
            return to_string_rewrite_var_by_literal(var, lit_to_rewrite_to);
    }
}

template <class TLit>
std::string VeriPbProofLogger::to_string(const TLit &lit)
{
    VeriPB::Lit l = toVeriPbLit(lit);
    VeriPB::Var varl = variable(l);

    return to_string_rewrite_var_by_literal(varl, l);
}


template <class TLit>
void VeriPbProofLogger::write_literal(const TLit &lit)
{
    *proof << to_string(lit) << " ";
}

template <class TSeqLit>
void VeriPbProofLogger::write_cardinality_constraint(const TSeqLit &lits, const wght RHS)
{
    for (int i = 0; i < lits.size(); i++)
        write_weighted_literal(lits[i]);
    *proof << ">= " << RHS;
}

template <class TSeqLit>
void VeriPbProofLogger::write_clause(const TSeqLit& clause){
    write_cardinality_constraint(clause, 1);
}

template <class TSeqLit, class TSeqWght>
void VeriPbProofLogger::write_PB_constraint(const TSeqLit &lits, const TSeqWght &weights, const wght RHS)
{
    for (int i = 0; i < lits.size(); i++)
        write_weighted_literal(lits[i], weights[i]);
    *proof << ">= " << RHS;
}

// ------------- Rewrite variables by literals -------------
template <class TVar, class TLit>
void VeriPbProofLogger::rewrite_variable_by_literal(const TVar& var, const TLit& lit)
{
    map_rewrite_var_by_literal[varidx(toVeriPbVar(var))] = toVeriPbLit(lit);
}

// ------------- Meaningful names -------------

template <class TVar>
void VeriPbProofLogger::store_meaningful_name(const TVar &var, const std::string &name)
{
    meaningful_names_store[varidx(toVeriPbVar(var))] = name;
}

template <class TVar>
void VeriPbProofLogger::delete_meaningful_name(const TVar &var)
{
    meaningful_names_store.erase(varidx(toVeriPbVar(var)));
}

// ------------- Rules for checking constraints -------------

template <class TSeqLit>
void VeriPbProofLogger::equals_rule(const constraintid constraint_id, const TSeqLit &lits, const wght RHS)
{
    *proof << "e " << constraint_id << " ";
    write_cardinality_constraint(lits, RHS);
    *proof << ";\n";
}

template <class TSeqLit, class TSeqWght>
void VeriPbProofLogger::equals_rule(const constraintid constraint_id, const TSeqLit &lits, const TSeqWght &weights, const wght RHS)
{
    *proof << "e " << constraint_id << " ";
    write_PB_constraint(lits, weights, RHS);
    *proof << ";\n";
}

template <class TSeqLit>
void VeriPbProofLogger::check_last_constraint(const TSeqLit &lits, const wght RHS)
{
    equals_rule(-1, lits, RHS);
}

template <class TSeqLit, class TSeqWght>
void VeriPbProofLogger::check_last_constraint(const TSeqLit &lits, const TSeqWght &weights, const wght RHS)
{
    equals_rule(-1, lits, weights, RHS);
}

// ------------- Rules for optimisation -------------

/// @brief Calculate the objective value of the objective stored in VeriPBProoflogger for a model. This function uses the optimistic assumption that the literals are sorted.
/// @param model Vector of assignments to literals.
/// @return Objective value for model.
template <class TSeqLit>
wght VeriPbProofLogger::calculate_objective_value(const TSeqLit &model)
{
    wght objective_value = objective_constant_cost;
    int model_start_idx = 0;
    for (int objective_idx = 0; objective_idx < objective_lits.size(); objective_idx++)
    {
        VeriPB::Lit objective_lit = objective_lits[objective_idx];
        VeriPB::Var var = variable(objective_lit);
        int model_idx = model_start_idx;
        while (var != toVeriPbVar(variable(model[model_idx])))
        {
            model_idx++;
            if (model_idx >= model.size())
            {
                model_idx = 0;
            }
            if (model_start_idx == model_idx)
            {
                printf("Solution check: Literal in objective not assigned!");
                return wght_max;
            }
        }
        if (objective_lit == toVeriPbLit(model[model_idx]))
        {
            objective_value += objective_weights[objective_idx];
        }
        model_start_idx = model_idx;
    }
    return objective_value;
}

template <class TSeqLit>
constraintid VeriPbProofLogger::log_solution(const TSeqLit &model, wght objective_value)
{
    *proof << "o ";
    for (int i = 0; i < model.size(); i++)
        if(!is_aux_var(variable(model[i]))) 
            write_literal(model[i]);
    *proof << "\n";
    // Veripb automatically adds an improvement constraint so counter needs to be incremented
    model_improvement_constraint = ++constraint_counter;

    if(objective_value < best_objective_value)
        best_objective_value = objective_value;

    if(CP_modelimprovingconstraint_rewrite != "")
        rewrite_model_improving_constraint();

    return get_model_improving_constraint(); 
}

template <class TSeqLit>
constraintid VeriPbProofLogger::log_solution_with_check(const TSeqLit &model)
{
    int current_objective_value = calculate_objective_value(model);
    if (current_objective_value < best_objective_value)
    {
        write_comment("Objective update from " + std::to_string(best_objective_value) + " to " + std::to_string(current_objective_value));
        log_solution(model, current_objective_value);
    }

    return get_model_improving_constraint();
}

constraintid VeriPbProofLogger::rewrite_model_improving_constraint(){
    assert(CP_modelimprovingconstraint_rewrite != "" && rewritten_model_improvement_constraint < model_improvement_constraint );

    write_comment("Rewrite model improving constraint");
    if(rewritten_model_improvement_constraint > 0) delete_constraint(rewritten_model_improvement_constraint);

    cuttingplanes_derivation cpder = CP_apply(CP_constraintid(model_improvement_constraint), CP_modelimprovingconstraint_rewrite);
    rewritten_model_improvement_constraint = write_CP_derivation(cpder);

    return rewritten_model_improvement_constraint;
}

constraintid VeriPbProofLogger::get_model_improving_constraint()
{
    if(CP_modelimprovingconstraint_rewrite == "")
        return model_improvement_constraint;
    else if(rewritten_model_improvement_constraint > model_improvement_constraint) // Already rewritten
        return rewritten_model_improvement_constraint;
    else{
        return rewrite_model_improving_constraint();
    }

}

wght VeriPbProofLogger::get_best_objective_value(){
    return best_objective_value;
}

cuttingplanes_derivation VeriPbProofLogger::get_rewrite_model_improvement_constraint(){
    return CP_modelimprovingconstraint_rewrite;
}

void VeriPbProofLogger::set_rewrite_model_improvement_constraint(cuttingplanes_derivation cpder){
    CP_modelimprovingconstraint_rewrite = cpder;
    if(model_improvement_constraint > 0)
        rewrite_model_improving_constraint();
}

// constraintid VeriPbProofLogger::get_rewritten_best_solution_constraint(){
//     return rewritten_best_solution_constraint;
// }
// void VeriPbProofLogger::rewrite_model_improving_constraint(){
//     rewritten_best_solution_constraint = write_CP_derivation(
//                 CP_apply(
//                     CP_constraintid(get_model_improving_constraint()), 
//                     CP_modelimprovingconstraint_rewrite));
// }
// 
// void VeriPbProofLogger::reset_rewritten_best_solution_constraint(){
//     rewritten_best_solution_constraint = 0;
// }

template <class TSeqLBool>
constraintid VeriPbProofLogger::log_solution_lbools(TSeqLBool &model, wght objective_value)
{
    write_comment("Model improvement update with objective value = " + std::to_string(objective_value));

    VeriPB::Var var;
    VeriPB::Lit lit;

    *proof << "o ";
    for (int i = 0; i < model.size(); i++){
        var = toVeriPbVar(i);
        lit = create_literal(var, !toBool(model[i]));

        if(!is_aux_var(var))
            write_literal(lit);
    }
    *proof << "\n";

    // Veripb automatically adds an improvement constraint so counter needs to be incremented
    model_improvement_constraint = ++constraint_counter;
    
    if(CP_modelimprovingconstraint_rewrite != "")
        rewrite_model_improving_constraint();

    if(objective_value < best_objective_value)
        best_objective_value = objective_value;

    return model_improvement_constraint;
}


// ------------- Unchecked Assumptions -------------
template <class TSeqLit>
constraintid VeriPbProofLogger::unchecked_assumption(const TSeqLit &lits, const wght RHS)
{
    *proof << "a ";
    write_cardinality_constraint(lits, RHS);
    *proof << " ;\n";
    return ++constraint_counter;
}

template <class TSeqLit, class TSeqWght>
constraintid VeriPbProofLogger::unchecked_assumption(const TSeqLit &lits, const TSeqWght &weights, const wght RHS)
{
    *proof << "u ";
    write_PB_constraint(lits, weights, RHS);
    *proof << " ;\n";
    return ++constraint_counter;
}

// ------------- Reverse Unit Propagation -------------
template <class TSeqLit>
constraintid VeriPbProofLogger::rup(const TSeqLit &lits, const wght RHS)
{
    *proof << "u ";
    write_cardinality_constraint(lits, RHS);
    *proof << ";\n";
    return ++constraint_counter;
}

template <class TSeqLit, class TSeqWght>
constraintid VeriPbProofLogger::rup(const TSeqLit &lits, const TSeqWght &weights, const wght RHS)
{
    *proof << "u ";
    write_PB_constraint(lits, weights, RHS);
    *proof << " ;\n";
    return ++constraint_counter;
}

//  ------------- Dominance Rule -------------

// TODO

// ------------- Redundance Based Strenghtening -------------

// TODO: add witness that could also map to other variables.

template <class TVar>
void VeriPbProofLogger::write_witness(const substitution<TVar> &witness)
{
    for(int i = 0; i < witness.size(); i++){
        VeriPB::Var var = toVeriPbVar(witness[i].first);
        bool to = witness[i].second;

        VeriPB::VarIdx idx_var = varidx(var);

        // If the variable in the witness has to be rewritten by a negated literal, than it should be witnessed on the negation.
        if(map_rewrite_var_by_literal.find(idx_var) != map_rewrite_var_by_literal.end()){
            var = variable(map_rewrite_var_by_literal[idx_var]);
            if(is_negated(map_rewrite_var_by_literal[idx_var]))
                to = !to;
        }

        *proof << var_name(var) << " -> " << std::to_string(to) ;
    }    
}


template <class TSeqLit, class TVar>
constraintid VeriPbProofLogger::redundanceBasedStrengthening(const TSeqLit &lits, const wght RHS, const substitution<TVar> &witness)
{
    *proof << "red ";
    write_cardinality_constraint(lits, RHS);
    *proof << "; ";
    write_witness(witness);
    *proof << "\n";
    return ++constraint_counter;
}

template <class TSeqLit, class TSeqWght, class TVar>
constraintid VeriPbProofLogger::redundanceBasedStrengthening(const TSeqLit &lits, const TSeqWght &weights, const wght RHS, const substitution<TVar> &witness)
{
    *proof << "red ";
    write_PB_constraint(lits, weights, RHS);
    *proof << "; ";
    write_witness(witness);
    *proof << "\n";
    return ++constraint_counter;
}

// ------------- Reification Literals -------------
// Proves the constraints encoding the reification constraint l <-> C, with l a literal and C a boolean constraint.
// The right implication is the encoding of l -> C, whereas the left implication means l <- C.
template <class TSeqLit, class TSeqWght, class TLit>
constraintid VeriPbProofLogger::reificationLiteralRightImpl(const TLit& lit, const TSeqLit &lits, const TSeqWght &weights, const wght RHS, bool store_reified_constraint){
    int i;

    std::string comment = to_string(lit) + " -> " ;
    for(i = 0; i < lits.size(); i++)
        comment += std::to_string(weights[i]) + " " + to_string(lits[i]) + " ";
    comment += ">= " + std::to_string(RHS);
    write_comment(comment);
    
    std::vector<VeriPB::Lit> _lits; _lits.resize(lits.size()+1);
    std::vector<wght> _weights;  _weights.resize(weights.size()+1);

    VeriPB::Lit neglit = toVeriPbLit(neg(lit));

    for(i = 0; i < lits.size(); i++){
        _weights[i] = weights[i];
        _lits[i] = toVeriPbLit(lits[i]);
    }    

    _lits[i] = neglit;
    _weights[i] = RHS;

    substitution<VeriPB::Var> witness;
    witness.push_back({variable(neglit), !is_negated(neglit)});

    constraintid cxnid = redundanceBasedStrengthening(_lits, _weights, RHS, witness);

    if(store_reified_constraint)
        reifiedConstraintRightImpl[varidx(toVeriPbVar(variable(lit)))] = cxnid;

    return cxnid;
}

template <class TSeqLit, class TLit>
constraintid VeriPbProofLogger::reificationLiteralRightImpl(const TLit& lit, const TSeqLit &lits, const wght RHS, bool store_reified_constraint){
    int i;

    std::string comment = to_string(lit) + " -> " ;
    for(i = 0; i < lits.size(); i++)
        comment +=  "1 " + to_string(lits[i]) + " ";
    comment += ">= " + std::to_string(RHS);
    write_comment(comment);
    
    std::vector<VeriPB::Lit> _lits; _lits.resize(lits.size()+1);
    std::vector<wght> _weights;  _weights.resize(lits.size()+1);

    VeriPB::Lit _neglit = toVeriPbLit(neg(lit));

    for(i = 0; i < lits.size(); i++){
        _weights[i] = 1;
        _lits[i] = toVeriPbLit(lits[i]);
    }
    
    _lits[i] = _neglit;
    _weights[i] = RHS;

    substitution<VeriPB::Var> witness;
    witness.push_back({variable(_neglit), !is_negated(_neglit)});

    constraintid cxnid = redundanceBasedStrengthening(_lits, _weights, RHS, witness);

    if(store_reified_constraint)
        reifiedConstraintRightImpl[varidx(toVeriPbVar(variable(lit)))] = cxnid;

    return cxnid;
}


template <class TSeqLit, class TSeqWght, class TLit>
constraintid VeriPbProofLogger::reificationLiteralLeftImpl(const TLit& lit, const TSeqLit &lits, const TSeqWght &weights, const wght RHS, bool store_reified_constraint){
    int i;

    std::string comment = to_string(lit) + " <- " ;
    for(i = 0; i < lits.size(); i++)
        comment += std::to_string(weights[i]) + " " + to_string(lits[i]) + " ";
    comment += ">= " + std::to_string(RHS);
    write_comment(comment);

    std::vector<VeriPB::Lit> _lits; _lits.resize(lits.size()+1);
    std::vector<wght> _weights;  _weights.resize(weights.size()+1);

    VeriPB::Lit _lit = toVeriPbLit(lit);

    wght sum_of_weights = 0;

    for(i = 0; i < lits.size(); i++){
        sum_of_weights += weights[i];

        _weights[i] = weights[i];
        _lits[i] = toVeriPbLit(neg(lits[i]));
    }
    

    wght j = sum_of_weights - RHS + 1 ;

    _lits[i] = _lit;
    _weights[i] = j;

    substitution<VeriPB::Var> witness;
    witness.push_back({variable(_lit), !is_negated(_lit)});

    constraintid cxnid = redundanceBasedStrengthening(_lits, _weights, j, witness);

    if(store_reified_constraint)
        reifiedConstraintLeftImpl[varidx(toVeriPbVar(variable(lit)))] = cxnid;

    return cxnid;
}


template <class TSeqLit, class TLit>
constraintid VeriPbProofLogger::reificationLiteralLeftImpl(const TLit& lit, const TSeqLit &lits, const wght RHS, bool store_reified_constraint){
    int i;
    
    std::string comment = to_string(lit) + " <- " ;
    for(i = 0; i < lits.size(); i++)
        comment += "1 " + to_string(lits[i]) + " ";
    comment += ">= " + std::to_string(RHS);
    write_comment(comment);
    
    std::vector<VeriPB::Lit> _lits; _lits.resize(lits.size()+1);
    std::vector<wght> _weights;  _weights.resize(lits.size()+1);

    VeriPB::Lit _lit = toVeriPbLit(lit);

    for(i = 0; i < lits.size(); i++){
        _weights[i] = 1;
        _lits[i] = toVeriPbLit(neg(lits[i]));
    }
    
    wght j = lits.size() - RHS + 1 ;

    _lits[i] = _lit;
    _weights[i] = j;

    substitution<VeriPB::Var> witness;
    witness.push_back({variable(_lit), !is_negated(_lit)});

    constraintid cxnid = redundanceBasedStrengthening(_lits, _weights, j, witness);

    if(store_reified_constraint)
        reifiedConstraintLeftImpl[varidx(toVeriPbVar(variable(lit)))] = cxnid;

    return cxnid;
}

template <class TVar>
constraintid VeriPbProofLogger::getReifiedConstraintLeftImpl(const TVar& var){
    VeriPB::VarIdx id = varidx(toVeriPbVar(var));

    assert(reifiedConstraintLeftImpl.find(id) != reifiedConstraintLeftImpl.end());

    return reifiedConstraintLeftImpl[id];
}

template <class TVar>
constraintid VeriPbProofLogger::getReifiedConstraintRightImpl(const TVar& var){
    VeriPB::VarIdx id = varidx(toVeriPbVar(var));

    assert(reifiedConstraintRightImpl.find(id) != reifiedConstraintRightImpl.end());

    return reifiedConstraintRightImpl[id];
}

template <class TVar>
void VeriPbProofLogger::setReifiedConstraintLeftImpl(const TVar& var, constraintid cxnId){
    VeriPB::VarIdx id = varidx(toVeriPbVar(var));
    reifiedConstraintLeftImpl[id] = cxnId;
}

template <class TVar>
void VeriPbProofLogger::setReifiedConstraintRightImpl(const TVar& var, constraintid cxnId){
    VeriPB::VarIdx id = varidx(toVeriPbVar(var));
    reifiedConstraintRightImpl[id] = cxnId;
}

template <class TVar>
void VeriPbProofLogger::deleteReifiedConstraintLeftImpl(const TVar& var){
    VeriPB::VarIdx id = varidx(toVeriPbVar(var));
    if(reifiedConstraintLeftImpl.find(id) != reifiedConstraintLeftImpl.end()){
        delete_constraint(reifiedConstraintLeftImpl[id]);
        reifiedConstraintLeftImpl.erase(id);
    }
}

template <class TVar>
void VeriPbProofLogger::deleteReifiedConstraintRightImpl(const TVar& var){
    VeriPB::VarIdx id = varidx(toVeriPbVar(var));
    if(reifiedConstraintRightImpl.find(id) != reifiedConstraintRightImpl.end()){
        delete_constraint(reifiedConstraintRightImpl[id]);
        reifiedConstraintRightImpl.erase(id);
    }
}

/**
 * Remove the right reification constraint from the reification constraint store without deleting it in the proof. 
 * Only needed for not maintaining a constraint id in memory that will not be used in the proof anymore.
*/
template <class TVar>
void VeriPbProofLogger::removeReifiedConstraintRightImplFromConstraintStore(const TVar& var){
    VeriPB::VarIdx id = varidx(toVeriPbVar(var));
    if(reifiedConstraintRightImpl.find(id) != reifiedConstraintRightImpl.end()){
        reifiedConstraintRightImpl.erase(id);
    }
}

/**
 * Remove the left reification constraint from the reification constraint store without deleting it in the proof. 
 * Only needed for not maintaining a constraint id in memory that will not be used in the proof anymore.
*/
template <class TVar>
void VeriPbProofLogger::removeReifiedConstraintLeftImplFromConstraintStore(const TVar& var){
    VeriPB::VarIdx id = varidx(toVeriPbVar(var));
    if(reifiedConstraintLeftImpl.find(id) != reifiedConstraintLeftImpl.end()){
        reifiedConstraintLeftImpl.erase(id);
    }
}

// ------------- Cutting Planes derivations -------------

cuttingplanes_derivation VeriPbProofLogger::CP_constraintid(const constraintid& constraint_id){
    return std::to_string(constraint_id);
}
template <class TLit>
cuttingplanes_derivation VeriPbProofLogger::CP_literal_axiom(const TLit& lit){
    return to_string(lit);
}
cuttingplanes_derivation VeriPbProofLogger::CP_addition(const cuttingplanes_derivation& left, const cuttingplanes_derivation& right){
    return left + " " + right + " +";
}
cuttingplanes_derivation VeriPbProofLogger::CP_addition(const cuttingplanes_derivation& cp){
    return cp + " +";
}
cuttingplanes_derivation VeriPbProofLogger::CP_division(const cuttingplanes_derivation& cp, const wght& n){
    return cp + " " + std::to_string(n) + " d";
}
cuttingplanes_derivation VeriPbProofLogger::CP_division(const wght& n){
    return " " + std::to_string(n) + " d";
}
cuttingplanes_derivation VeriPbProofLogger::CP_saturation(const cuttingplanes_derivation& cp){
    return cp + " s";
}
cuttingplanes_derivation VeriPbProofLogger::CP_saturation(){
    return " s";
}
cuttingplanes_derivation VeriPbProofLogger::CP_multiplication(const cuttingplanes_derivation& cp, const wght& n){
    return cp + " " + std::to_string(n) + " *";
}
cuttingplanes_derivation VeriPbProofLogger::CP_multiplication(const wght& n){
    return " " + std::to_string(n) + " *";
}
template <class TVar>
cuttingplanes_derivation VeriPbProofLogger::CP_weakening(const cuttingplanes_derivation& cp, const TVar& var){
    return cp + " " + var_name(var) + " w";
}
template <class TVar>
cuttingplanes_derivation VeriPbProofLogger::CP_weakening(const TVar& var){
    return " " + var_name(var) + " w";
}
cuttingplanes_derivation VeriPbProofLogger::CP_apply(const cuttingplanes_derivation& cp_start, const cuttingplanes_derivation& cp_to_be_applied){
    return cp_start + " " + cp_to_be_applied;
}
constraintid VeriPbProofLogger::write_CP_derivation(const cuttingplanes_derivation& cp){
    *proof << "p " << cp << "\n";
    return ++constraint_counter;
}

void VeriPbProofLogger::start_CP_derivation(const constraintid constraint_id)
{
    pol_string.clear();
    pol_string << "p " << constraint_id;
}

template <class TLit>
void VeriPbProofLogger::start_CP_derivation_with_lit_axiom(const TLit &lit)
{
    pol_string << "p ";
    pol_string << to_string(lit);
}

void VeriPbProofLogger::CP_load_constraint(const constraintid constraint_id)
{
    pol_string << " " << constraint_id;
}

void VeriPbProofLogger::CP_add()
{
    pol_string << " +";
}

void VeriPbProofLogger::CP_add_constraint(const constraintid constraint_id)
{
    pol_string << " " << constraint_id << " +";
}

template <class TLit>
void VeriPbProofLogger::CP_add_literal_axiom(const TLit &lit){
    pol_string << " " << to_string(lit) << " +";
}

void VeriPbProofLogger::CP_divide(const wght v)
{
    pol_string << " " << v << " d";
}
void VeriPbProofLogger::CP_saturate()
{
    pol_string << " s";
}
void VeriPbProofLogger::CP_multiply(const wght v)
{
    pol_string << " " << v << " *";
}

template <class TVar>
void VeriPbProofLogger::CP_weaken(const TVar &var)
{
    pol_string << " " << var_name(var) << " w";
}

template <class TLit>
void VeriPbProofLogger::CP_write_literal_axiom(const TLit &lit)
{
    pol_string << " " << to_string(lit);
}

constraintid VeriPbProofLogger::end_CP_derivation()
{
    *proof << pol_string.rdbuf() << "\n";
    return ++constraint_counter;
}

//  ------------- Deleting & Overwriting Constraints -------------

void VeriPbProofLogger::delete_constraint(const constraintid constraint_id)
{
    *proof << "del id " << constraint_id << "\n";
}

void VeriPbProofLogger::delete_constraint(const std::vector<constraintid> &constraint_ids)
{
    *proof << "del id";
    for (int i = 0; i < constraint_ids.size(); i++)
    {
        *proof << " " << constraint_ids[i];
    }
    *proof << "\n";
}

template <class TSeqLit>
void VeriPbProofLogger::delete_constraint(const TSeqLit &lits, const wght RHS)
{
    *proof << "del find ";
    write_cardinality_constraint(lits, RHS);
    *proof << ";\n";
}


template <class TSeqLit, class TSeqWght>
void VeriPbProofLogger::delete_constraint(const TSeqLit &lits, const TSeqWght &weights, const wght RHS)
{
    *proof << "del find ";
    write_PB_constraint(lits, weights, RHS);
    *proof << ";\n";
}

// Removal by del find where a literal occuring multiple times in lits is only written once.
// Note: TSeqLit must be sorted.
template <class TSeqLit>
void VeriPbProofLogger::delete_clause(const TSeqLit& lits){
    *proof << "del find ";
    write_weighted_literal(lits[0]);
    for(int i = 1; i < lits.size(); i++){
        if(lits[i] != lits[i-1])
            write_weighted_literal(lits[i]);
    }
    *proof << " >= 1 ;\n";
}

template <class TSeqLit>
constraintid VeriPbProofLogger::overwrite_constraint(const constraintid constraint_id, const TSeqLit &lits, const wght RHS, bool origclause_in_coreset)
{
    constraintid newconstraint = rup(lits, RHS);
    if(origclause_in_coreset)
        move_to_coreset(-1);
    delete_constraint(constraint_id);
    return newconstraint;
}

template <class TSeqLit>
constraintid VeriPbProofLogger::overwrite_constraint(const TSeqLit &lits_orig, const wght RHS_orig, const TSeqLit &lits, const wght RHS, bool origclause_in_coreset)
{
    constraintid newconstraint = rup(lits, RHS);
    if(origclause_in_coreset)
        move_to_coreset(-1);
    delete_constraint(lits_orig, RHS_orig);
    return newconstraint;
}

template <class TSeqLit, class TSeqWght>
constraintid VeriPbProofLogger::overwrite_constraint(const constraintid constraint_id, const TSeqLit &lits, const TSeqWght &weights, const wght RHS, bool origclause_in_coreset)
{
    constraintid newconstraint = rup(lits, weights, RHS);
    if(origclause_in_coreset)
        move_to_coreset(-1);
    delete_constraint(constraint_id);
    return newconstraint;
}

template <class TSeqLit, class TSeqWght>
constraintid VeriPbProofLogger::overwrite_constraint(const TSeqLit &lits_orig, const TSeqWght &weights_orig, const wght RHS_orig, const TSeqLit &lits, const TSeqWght &weights, const wght RHS, bool origclause_in_coreset)
{
    constraintid newconstraint = rup(lits, weights, RHS);
    if(origclause_in_coreset)
        move_to_coreset(-1);
    delete_constraint(lits_orig, weights_orig, RHS_orig);
    return newconstraint;
}

template <class TSeqLit>
constraintid VeriPbProofLogger::overwrite_constraint(const TSeqLit&lits_orig, const TSeqLit &lits, bool origclause_in_coreset)
{
    constraintid newconstraint = rup(lits);
    if(origclause_in_coreset)
        move_to_coreset(-1);
    delete_constraint(lits_orig, 1);
    return newconstraint;
}

void VeriPbProofLogger::move_to_coreset(constraintid cxn){
    *proof << "core id " << std::to_string(cxn) << "\n";
}
template <class TSeqLit>
void VeriPbProofLogger::move_to_coreset(TSeqLit& lits, wght RHS){
    *proof << "core find "; write_cardinality_constraint(lits, RHS); *proof << "\n";
}
template <class TSeqLit, class TSeqWght>
void VeriPbProofLogger::move_to_coreset(TSeqLit& lits, TSeqWght& wghts, wght RHS){
    *proof << "core find "; write_PB_constraint(lits, wghts, RHS); *proof << "\n";
}

// ------------- Handling contradiction -------------
void VeriPbProofLogger::write_contradiction(constraintid cxnid){
    *proof << "c " << cxnid << "\n";
}

void VeriPbProofLogger::write_previous_constraint_contradiction()
{
    *proof << "c -1 \n";
}

void VeriPbProofLogger::rup_empty_clause()
{
    *proof << "u >= 1;\n";
    constraint_counter++;
}
//=================================================================================================