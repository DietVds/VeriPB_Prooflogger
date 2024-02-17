#include "VeriPBProoflogger.h"


//=================================================================================================

//prooflogging Library


// ------------- Proof file -------------

void VeriPbProofLogger::set_proof_stream(std::ostream* proof){
    this->proof = proof;
}

void VeriPbProofLogger::set_keep_original_formula(){
    keep_original_formula = true;
}

void VeriPbProofLogger::write_proof_header(int nbclause, int nbvars)
{
    n_variables = nbvars;

    *proof << "pseudo-Boolean proof version 2.0\n";
    *proof << "f " << nbclause << "\n";
}

void VeriPbProofLogger::write_proof_header(int nbvars)
{
    n_variables = nbvars;

    *proof << "pseudo-Boolean proof version 2.0\n";
    *proof << "f\n";
}

void VeriPbProofLogger::write_proof_header()
{
    *proof << "pseudo-Boolean proof version 2.0\n";
    *proof << "f\n";
}

void VeriPbProofLogger::set_n_variables(int nbvars){

    assert(n_variables == 0);
    n_variables = nbvars;
}

void VeriPbProofLogger::set_n_constraints(int nbconstraints){
    assert(constraint_counter == 0);
    constraint_counter = nbconstraints;
    n_original_constraints = nbconstraints;
}

// Conclusion
void VeriPbProofLogger::write_conclusion_NONE(){
    *proof << "output NONE\n"
        << "conclusion NONE\n"
        << "end pseudo-Boolean proof\n";
}
void VeriPbProofLogger::write_conclusion_UNSAT(){
    *proof << "output NONE\n"
        << "conclusion UNSAT\n"
        << "end pseudo-Boolean proof\n";
}
void VeriPbProofLogger::write_conclusion_UNSAT_optimization(){
    *proof << "output NONE\n"
        << "conclusion BOUNDS INF INF\n"
        << "end pseudo-Boolean proof\n";
}
void VeriPbProofLogger::write_conclusion_SAT(){
    *proof << "output NONE\n"
        << "conclusion SAT\n"
        << "end pseudo-Boolean proof\n";
}
void VeriPbProofLogger::write_conclusion_OPTIMAL(){
    *proof << "output NONE\n"
        << "conclusion BOUNDS " << std::to_string(best_objective_value) << " " << std::to_string(best_objective_value) << "\n"
        << "end pseudo-Boolean proof\n";
}

void VeriPbProofLogger::write_conclusion_OPTIMAL(constraintid hint){
    *proof << "output NONE\n"
        << "conclusion BOUNDS " << std::to_string(best_objective_value) << " : " << std::to_string(hint) << " " << std::to_string(best_objective_value) << "\n"
        << "end pseudo-Boolean proof\n";
}


void VeriPbProofLogger::write_conclusion_BOUNDS(wght LB, wght UB){
    *proof << "output NONE\n"
        << "conclusion BOUNDS " << std::to_string(LB) << " " << std::to_string(UB) << "\n"
        << "end pseudo-Boolean proof\n";
}

void VeriPbProofLogger::write_conclusion_BOUNDS(wght LB, constraintid hint, wght UB){
    *proof << "output NONE\n"
        << "conclusion BOUNDS " << std::to_string(LB) << " : " << std::to_string(hint) << " " << std::to_string(UB) << "\n"
        << "end pseudo-Boolean proof\n";
}

void VeriPbProofLogger::write_fail(){
    *proof << "fail\n";
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
void VeriPbProofLogger::add_objective_literal(TLit& lit, wght weight){
    objective_lits.push_back(toVeriPbLit(lit));
    objective_weights.push_back(weight);
}

template <class TLit>
bool VeriPbProofLogger::remove_objective_literal(TLit& lit){
    int i=0;
    VeriPB::Lit vLit = toVeriPbLit(lit);
    bool found = false;

    write_comment("objective before removal: "); write_comment_objective_function();
    write_comment("size objective literals = " + std::to_string(objective_lits.size()));

    while(i < objective_lits.size() && vLit != objective_lits[i]) i++;

    if(i < objective_lits.size() && vLit == objective_lits[i]) found=true; 

    if(found) write_comment("removal of objective literal " + to_string(vLit) + " and objective_lits[i] = " + to_string(objective_lits[i]));

    while(found && i+1 < objective_lits.size()) {
        objective_lits[i] = objective_lits[i+1];
        objective_weights[i] = objective_weights[i+1];
        i++;
    }
    
    if(found && objective_lits.size() > 0)
        objective_lits.resize(objective_lits.size()-1);
    if(found && objective_lits.size() > 0)
        objective_weights.resize(objective_weights.size()-1);

    write_comment("objective after removal: "); write_comment_objective_function();
    write_comment("found = " + std::to_string(found));
    write_comment("size objective literals = " + std::to_string(objective_lits.size()));

    return found;
}

template <class TLit>
wght VeriPbProofLogger::get_objective_weight(TLit& lit){
    int i=0;
    while(toVeriPbLit(lit) != objective_lits[i]) i++;

    return objective_weights[i];
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

void VeriPbProofLogger::write_objective_update(){
    *proof << "obju new ";
    for (int i = 0; i < objective_lits.size(); i++)
        write_weighted_literal(objective_lits[i], objective_weights[i]);
    if(objective_constant_cost != 0)
        *proof << std::to_string(objective_constant_cost);
    *proof << ";\n";
}

template <class TSeqLit, class TSeqSignedWght>
void VeriPbProofLogger::write_objective_update_diff(TSeqLit& litsOnewminusold, TSeqSignedWght& wghtsOnewminusold, signedWght constantOnewminusold){
    *proof << "obju diff ";
    for(int i = 0; i < litsOnewminusold.size(); i++){
        *proof << std::to_string(wghtsOnewminusold[i]) << " "; 
        write_literal(litsOnewminusold[i]);
    }
    *proof << (constantOnewminusold == 0 ? "" : std::to_string(constantOnewminusold)) << ";\n";
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

bool VeriPbProofLogger::is_original_constraint(constraintid cxn){
    return cxn <= n_original_constraints;
}
// ------------- Helping functions -------------

void VeriPbProofLogger::write_comment(const char *comment)
{
    if(comments)
        *proof << "* " << comment << "\n";
    // proof->flush(); // Can be uncommented for debugging reasons
}

void VeriPbProofLogger::write_comment(const std::string &comment)
{
    if(comments)
        *proof << "* " << comment << "\n";
}

template <class TVar>
bool VeriPbProofLogger::is_aux_var(const TVar &var)
{
    VeriPB::Var v = toVeriPbVar(var);
    return (v.v > n_variables) || v.only_known_in_proof;
}

template <class TVar>
std::string VeriPbProofLogger::var_name(const TVar &var)
{
    VeriPB::Var v = toVeriPbVar(var);
    VeriPB::VarIdx v_idx = varidx(v);

    if (meaningful_names_store.find(v_idx) != meaningful_names_store.end())
    {
        return  meaningful_names_store[v_idx];
    }
    else if (v.only_known_in_proof){
        return "_p" + std::to_string(v.v);
    }
    else if (is_aux_var(var))
    {
        return  "y" +  std::to_string(v.v);
    }
    else
    {
        return "x" + std::to_string(v.v);
    }
}

VeriPB::Var VeriPbProofLogger::new_variable_only_in_proof(std::string name){
    VeriPB::Var v; 
    v.only_known_in_proof = true; 
    v.v = ++n_vars_only_known_in_proof;
    if(name != "")
        meaningful_names_store[varidx(v)] = name;
    return v;
}

template <class TLit>
void VeriPbProofLogger::write_weighted_literal(const TLit &literal, wght weight)
{   
    if(weight != 0){
        *proof << std::to_string(weight) << " ";
        write_literal<TLit>(literal);
    }
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
        write_weighted_literal(lits[i], 1);
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

template <class TSeqLit, class TSeqWght>
void VeriPbProofLogger::write_PB_constraint(const TSeqLit& lits_greater, const TSeqWght& weights_greater, const wght const_greater, const TSeqLit& lits_smaller, const TSeqWght& weights_smaller, const wght const_smaller ){
    assert(lits_greater.size() == weights_greater.size());
    assert(lits_smaller.size() == weights_smaller.size());

    for(int i = 0; i < lits_greater.size(); i++){
        write_weighted_literal(lits_greater[i], weights_greater[i]);
    }
    wght RHS = 0;
    for(int i =0; i < lits_smaller.size(); i++){
        write_weighted_literal(neg(lits_smaller[i]), weights_smaller[i]);
        RHS+= weights_smaller[i];
    }
    *proof << " >= " << std::to_string(RHS + const_smaller - const_greater);
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
    *proof << "e " << constraint_id << " : ";
    write_cardinality_constraint(lits, RHS);
    *proof << ";\n";
}

template <class TSeqLit, class TSeqWght>
void VeriPbProofLogger::equals_rule(const constraintid constraint_id, const TSeqLit &lits, const TSeqWght &weights, const wght RHS)
{
    *proof << "e " << constraint_id << " : ";
    write_PB_constraint(lits, weights, RHS);
    *proof << ";\n";
}

template <class TSeqLit, class TSeqWght>
void VeriPbProofLogger::equals_rule(const constraintid constraint_id, const TSeqLit& lits_greater, const TSeqWght& weights_greater, const wght const_greater, const TSeqLit& lits_smaller, const TSeqWght& weights_smaller, const wght const_smaller  ){
    *proof << "e " << std::to_string(constraint_id) << " : " ;
    write_PB_constraint(lits_greater, weights_greater, const_greater, lits_smaller, weights_smaller, const_smaller);    
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

template <class TSeqLit, class TSeqWght>
void VeriPbProofLogger::check_last_constraint(const TSeqLit& lits_greater, const TSeqWght& weights_greater, const wght const_greater, const TSeqLit& lits_smaller, const TSeqWght& weights_smaller, const wght const_smaller  ){
    equals_rule(-1, lits_greater, weights_greater, const_greater, lits_smaller, weights_smaller, const_smaller);
}

template <class TSeqLit>
void VeriPbProofLogger::check_constraint_exists(const TSeqLit &lits, const wght RHS)
{
    *proof << "e ";
    write_cardinality_constraint(lits, RHS);
    *proof << ";\n";
}

template <class TSeqLit, class TSeqWght>
void VeriPbProofLogger::check_constraint_exists(const TSeqLit &lits, const TSeqWght &weights, const wght RHS)
{
    *proof << "e ";
    write_PB_constraint(lits, weights, RHS);
    *proof << ";\n";
}

template <class TSeqLit, class TSeqWght>
void VeriPbProofLogger::check_constraint_exists(const TSeqLit& lits_greater, const TSeqWght& weights_greater, const wght const_greater, const TSeqLit& lits_smaller, const TSeqWght& weights_smaller, const wght const_smaller  ){
    *proof << "e ";
    write_PB_constraint(lits_greater, weights_greater, const_greater, lits_smaller, weights_smaller, const_smaller);
    *proof << ";\n";
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

// TODO - Dieter: Add argument for writing full model.
template <class TSeqLit>
constraintid VeriPbProofLogger::log_solution(const TSeqLit &model, wght objective_value, bool only_original_variables_necessary, bool log_as_comment)
{
    write_comment("Solution with objective value: " + std::to_string(objective_value));
    *proof << (log_as_comment ? "* " : "soli ");
    for (int i = 0; i < model.size(); i++){
        if(only_original_variables_necessary && is_aux_var(variable(model[i])))
            continue;
        write_literal(model[i]);
    }
    *proof << "\n";
    // Veripb automatically adds an improvement constraint so counter needs to be incremented
    if(!log_as_comment){
        model_improvement_constraint = ++constraint_counter;

        if(objective_value < best_objective_value)
            best_objective_value = objective_value;

        std::vector<VeriPB::Lit> litsMIC;
        wght RHS = 0;
        for(uint32_t i = 0; i < objective_lits.size(); i++){
            litsMIC.push_back(neg(objective_lits[i]));
            RHS += objective_weights[i];
        }
        RHS = RHS + objective_constant_cost - best_objective_value + 1;
        check_last_constraint(litsMIC, objective_weights, RHS);
    }

    return get_model_improving_constraint(); 
}

template <class TSeqLit>
constraintid VeriPbProofLogger::log_solution_with_check(const TSeqLit &model, bool only_original_variables_necessary, bool log_nonimproving_solution_as_comment)
{
    int current_objective_value = calculate_objective_value(model);
    if (current_objective_value < best_objective_value)
    {
        write_comment_objective_function();
        write_comment("Objective update from " + std::to_string(best_objective_value) + " to " + std::to_string(current_objective_value));
        log_solution(model, current_objective_value, only_original_variables_necessary);
    }
    else if(log_nonimproving_solution_as_comment){
        write_comment_objective_function();
        write_comment("Non-improving solution:");
        log_solution(model, current_objective_value, only_original_variables_necessary, true);
    }

    return get_model_improving_constraint();
}

constraintid VeriPbProofLogger::get_model_improving_constraint()
{
    return model_improvement_constraint;
}

wght VeriPbProofLogger::get_best_objective_value(){
    return best_objective_value;
}

template <class TSeqLBool>
constraintid VeriPbProofLogger::log_solution_lbools(TSeqLBool &model, wght objective_value)
{
    write_comment("Model improvement update with objective value = " + std::to_string(objective_value));

    VeriPB::Var var;
    VeriPB::Lit lit;

    *proof << "soli ";
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
    *proof << "a ";
    write_PB_constraint(lits, weights, RHS);
    *proof << " ;\n";
    return ++constraint_counter;
}

template <class TLit> 
constraintid VeriPbProofLogger::unchecked_assumption_unit_clause(const TLit& lit, bool core_constraint){
    *proof << "a ";
    write_weighted_literal(lit, 1);
    *proof << ">= 1 ;\n";
    if(core_constraint)
        move_to_coreset(-1);
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

template <class TLit>
constraintid VeriPbProofLogger::rup_unit_clause(const TLit& lit, bool core_constraint){
    *proof << "u ";
    write_weighted_literal(lit);
    *proof << " >= 1;\n";

    if(core_constraint)
        move_to_coreset(-1);

    return ++constraint_counter;
}

template <class TLit>
constraintid VeriPbProofLogger::rup_binary_clause(const TLit& lit1, const TLit& lit2, bool core_constraint){
    *proof << "u ";
    write_weighted_literal(lit1);
    write_weighted_literal(lit2);
    *proof << " >= 1;\n";

    if(core_constraint)
        move_to_coreset(-1);

    return ++constraint_counter;
}

template <class TLit> 
constraintid VeriPbProofLogger::rup_ternary_clause(const TLit& lit1, const TLit& lit2, const TLit& lit3, bool core_constraint){
    *proof << "u ";
    write_weighted_literal(lit1);
    write_weighted_literal(lit2);
    write_weighted_literal(lit3);
    *proof << " >= 1;\n";

    if(core_constraint)
        move_to_coreset(-1);

    return ++constraint_counter;
}

//  ------------- Dominance Rule -------------

// TODO

// ------------- Redundance Based Strenghtening -------------

// TODO: add witness that could also map to other variables.

void VeriPbProofLogger::strenghten_to_core(){
    *proof << "strengthening_to_core on\n"; 
}

substitution VeriPbProofLogger::get_new_substitution(){
    substitution s;
    return s;
}

template <class TVar>
void VeriPbProofLogger::add_boolean_assignment(substitution &s, const TVar& var, const bool value){
    s.second.push_back(std::make_pair(toVeriPbVar(var), value));
}
template <class TVar, class TLit>
void VeriPbProofLogger::add_literal_assignment(substitution &s, const TVar& var, const TLit& value){
    s.first.push_back(std::make_pair(toVeriPbVar(var), toVeriPbLit(value)));
}

void VeriPbProofLogger::write_substitution(const substitution &witness)
{
    for(auto lit_ass : witness.first){
        *proof << var_name(lit_ass.first) << " -> " << to_string(lit_ass.second) << " ";
    }
    for(auto bool_ass : witness.second){
        *proof << var_name(bool_ass.first) << " -> " << (bool_ass.second ? " 1 " : " 0 ");
    }   
}

template <class TVar>
bool VeriPbProofLogger::has_boolean_assignment(const substitution &s, const TVar& var){
    for(auto ass : s.second){
        if(ass.first == toVeriPbVar(var))
            return true;
    }
    return false;
}

template <class TVar>
bool VeriPbProofLogger::has_literal_assignment(const substitution &s, const TVar& var){
    for(auto ass : s.first){
        if(ass.first == toVeriPbVar(var))
            return true;
    }
    return false;
}

template <class TVar>
bool VeriPbProofLogger::get_boolean_assignment(substitution &s, const TVar& var){
    for(auto ass : s.second){
        if(ass.first == toVeriPbVar(var))
            return ass.second;
    }
    std::cout << "ERROR: Proof logging library: Could not find boolean assignment for variable " << var_name(var) << std::endl;
    assert(false);
}

template <class TVar>
VeriPB::Lit VeriPbProofLogger::get_literal_assignment(substitution &s, const TVar& var){
    for(auto ass : s.first){
        if(ass.first == toVeriPbVar(var))
            return ass.second;
    }
    std::cout << "ERROR: Proof logging library: Could not find literal assignment for variable " << var_name(var) << std::endl;
    assert(false);
}

size_t VeriPbProofLogger::get_substitution_size(const substitution &s){
    return s.first.size() + s.second.size();
}

template <class TSeqLit>
constraintid VeriPbProofLogger::redundanceBasedStrengthening(const TSeqLit &lits, const wght RHS, const substitution &witness)
{
    *proof << "red ";
    write_cardinality_constraint(lits, RHS);
    *proof << "; ";
    write_substitution(witness);
    *proof << "\n";
    return ++constraint_counter;
}

template <class TSeqLit, class TSeqWght>
constraintid VeriPbProofLogger::redundanceBasedStrengthening(const TSeqLit &lits, const TSeqWght &weights, const wght RHS, const substitution &witness)
{
    *proof << "red ";
    write_PB_constraint(lits, weights, RHS);
    *proof << "; ";
    write_substitution(witness);
    *proof << "\n";
    return ++constraint_counter;
}

template <class TSeqLit, class TSeqWght>
constraintid VeriPbProofLogger::redundanceBasedStrengthening(const TSeqLit &lits, const TSeqWght &weights, const wght RHS, const substitution &witness, std::vector<subproof> subproofs){
    *proof << "red ";
    write_PB_constraint(lits, weights, RHS);
    *proof << "; ";
    write_substitution(witness);

    constraint_counter++; // constraint not C

    if(subproofs.size() > 0){
        *proof << "; begin \n";
    
        for(int i = 0; i < subproofs.size(); i++){
            subproof p = subproofs[i];
            constraint_counter++; // constraint C\w
            *proof << "\tproofgoal " << p.proofgoal << "\n";
            for(int i = 0; i < p.derivations.size(); i++){
                *proof << "\t\tp " << p.derivations[i] << "\n";
                constraint_counter++;
            }
            // *proof << "\t\t c -1\n";
            *proof << "\tend -1\n";
        }
        *proof << "end\n";
    }

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

    substitution witness = get_new_substitution();
    add_boolean_assignment(witness, variable(neglit), !is_negated(neglit));

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

    substitution witness = get_new_substitution();
    add_boolean_assignment(witness, variable(_neglit), !is_negated(_neglit));

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
    
    wght j; 
    if(sum_of_weights + 1 >= RHS)
        j = sum_of_weights - RHS + 1 ;
    else
        j = 0;

    _lits[i] = _lit;
    _weights[i] = j;

    substitution witness = get_new_substitution();
    add_boolean_assignment(witness, variable(_lit), !is_negated(_lit));

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
    
    wght j; 
    if(lits.size() + 1 >= RHS)
        j = lits.size() - RHS + 1 ;
    else
        j = 0;

    _lits[i] = _lit;
    _weights[i] = j;

    substitution witness = get_new_substitution();
    add_boolean_assignment(witness, variable(_lit), !is_negated(_lit));
    

    constraintid cxnid = redundanceBasedStrengthening(_lits, _weights, j, witness);

    if(store_reified_constraint)
        reifiedConstraintLeftImpl[varidx(toVeriPbVar(variable(lit)))] = cxnid;

    return cxnid;
}

template <class TVar>
constraintid VeriPbProofLogger::getReifiedConstraintLeftImpl(const TVar& var){
    VeriPB::VarIdx id = varidx(toVeriPbVar(var));

    if(reifiedConstraintLeftImpl.find(id) == reifiedConstraintLeftImpl.end())
        std::cout << "ERROR: Cannot find left reification constraint for variable " << var_name(var) << std::endl;

    assert(reifiedConstraintLeftImpl.find(id) != reifiedConstraintLeftImpl.end());

    return reifiedConstraintLeftImpl[id];
}

template <class TVar>
constraintid VeriPbProofLogger::getReifiedConstraintRightImpl(const TVar& var){
    VeriPB::VarIdx id = varidx(toVeriPbVar(var));

    if(reifiedConstraintRightImpl.find(id) == reifiedConstraintRightImpl.end())
        std::cout << "ERROR: Cannot find right reification constraint for variable " << var_name(var) << std::endl;
    

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
        delete_constraint_by_id(reifiedConstraintLeftImpl[id]);
        reifiedConstraintLeftImpl.erase(id);
    }
}

template <class TVar>
void VeriPbProofLogger::deleteReifiedConstraintRightImpl(const TVar& var){
    VeriPB::VarIdx id = varidx(toVeriPbVar(var));
    if(reifiedConstraintRightImpl.find(id) != reifiedConstraintRightImpl.end()){
        delete_constraint_by_id(reifiedConstraintRightImpl[id]);
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
template <class TLit>
cuttingplanes_derivation VeriPbProofLogger::CP_weakening(const cuttingplanes_derivation& cp, const TLit& lit, const wght& weight){
    return CP_addition(cp, CP_multiplication(CP_literal_axiom(lit), weight));
}
template <class TLit>
cuttingplanes_derivation VeriPbProofLogger::CP_weakening(const TLit& lit, const wght& weight){
    return CP_multiplication(CP_literal_axiom(lit), weight);
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

// ------------- Extra Proof Techniques -------------

template <class TSeqLit, class TSeqWght>
constraintid VeriPbProofLogger::prove_by_contradiction(TSeqLit& lits, TSeqWght& weights, wght RHS, std::vector<cuttingplanes_derivation> cpder){
    std::vector<subproof> subproofs;
    subproofs.push_back({"#1", cpder});
    substitution witness = get_new_substitution();
    return redundanceBasedStrengthening(lits, weights, RHS, witness, subproofs );
}

template <class TSeqLit, class TSeqWght>
constraintid VeriPbProofLogger::prove_by_casesplitting(TSeqLit& lits, TSeqWght& weights, wght RHS, constraintid case1, constraintid case2){
    std::vector<cuttingplanes_derivation> p;

    p.push_back(CP_saturation(CP_addition(CP_constraintid(-1), CP_constraintid(case1))));
    p.push_back(CP_saturation(CP_addition(CP_constraintid(-2), CP_constraintid(case2))));
    p.push_back(CP_addition(CP_constraintid(-1), CP_constraintid(-2)));
    return prove_by_contradiction(lits, weights, RHS, p);
}


//  ------------- Deleting & Overwriting Constraints -------------

void VeriPbProofLogger::delete_constraint_by_id(const constraintid constraint_id)
{
    if(!(keep_original_formula && is_original_constraint(constraint_id)))
        *proof << "del id " << constraint_id << "\n";
}

void VeriPbProofLogger::delete_constraint_by_id(const std::vector<constraintid> &constraint_ids)
{
    *proof << "del id";
    for (constraintid id : constraint_ids)
    {
        if(!(keep_original_formula && is_original_constraint(id)))
            *proof << " " << id;
    }
    *proof << "\n";
}

template <class TSeqLit>
void VeriPbProofLogger::delete_constraint(const TSeqLit &lits, const wght RHS)
{
    assert(!keep_original_formula);

    *proof << "del spec ";
    write_cardinality_constraint(lits, RHS);
    *proof << ";\n";
}

template <class TSeqLit>
void VeriPbProofLogger::delete_constraint(const TSeqLit &lits, const wght RHS, const substitution& witness){
    assert(!keep_original_formula);
    *proof << "del spec ";
    write_cardinality_constraint(lits, RHS);
    *proof << "; ";
    write_substitution(witness);
    *proof << "\n";
}

template <class TSeqLit, class TSeqWght>
void VeriPbProofLogger::delete_constraint(const TSeqLit &lits, const TSeqWght &weights, const wght RHS)
{
    assert(!keep_original_formula);
    *proof << "del spec ";
    write_PB_constraint(lits, weights, RHS);
    *proof << ";\n";
}

template <class TSeqLit, class TSeqWght>
void VeriPbProofLogger::delete_constraint(const TSeqLit &lits, const TSeqWght &weights, const wght RHS, const substitution& witness){
    assert(!keep_original_formula);
    *proof << "del spec ";
    write_PB_constraint(lits, weights, RHS);
    *proof << "; ";
    write_substitution(witness);
    *proof << "\n";
}

// Removal by del find where a literal occuring multiple times in lits is only written once.
// Note: TSeqLit must be sorted.
template <class TSeqLit>
void VeriPbProofLogger::delete_clause(const TSeqLit& lits){
    assert(!keep_original_formula);
    *proof << "del spec ";
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
    delete_constraint_by_id(constraint_id);
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
    delete_constraint_by_id(constraint_id);
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
    if(!keep_original_formula)
        *proof << "core id " << std::to_string(cxn) << "\n";
}
template <class TSeqLit>
void VeriPbProofLogger::move_to_coreset(TSeqLit& lits, wght RHS){
    if(!keep_original_formula)
        *proof << "core find "; write_cardinality_constraint(lits, RHS); *proof << "\n";
}
template <class TSeqLit, class TSeqWght>
void VeriPbProofLogger::move_to_coreset(TSeqLit& lits, TSeqWght& wghts, wght RHS){
    if(!keep_original_formula)
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