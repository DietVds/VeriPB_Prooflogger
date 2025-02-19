#include "VeriPBProoflogger.h"
// TODO: Check if to_string is used somewhere instead of writing it to an ostream.

//=================================================================================================

//prooflogging Library


// ------------- Proof file -------------

void VeriPbProofLogger::set_proof_stream(std::ostream* proof){
    this->proof = proof;
    proof->rdbuf()->pubsetbuf(write_buffer, write_buffer_size); 
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
    if(nbvars >= nameSolverVars.size()){
        // std::cout << "resize in set_n_variables" << std::endl;
        nameSolverVars.resize(2*nbvars);
        vec_rewrite_solvervar_by_literal.resize(2*nbvars);
    }
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
void VeriPbProofLogger::write_conclusion_UNSAT_optimization(constraintid hint){
    *proof << "output NONE\n"
        << "conclusion BOUNDS INF : " << hint << " INF\n"
        << "end pseudo-Boolean proof\n";
}
void VeriPbProofLogger::write_conclusion_SAT(){
    *proof << "output NONE\n"
        << "conclusion SAT\n"
        << "end pseudo-Boolean proof\n";
}
void VeriPbProofLogger::write_conclusion_OPTIMAL(){
    if(best_objective_value == wght_max){
        write_conclusion_UNSAT_optimization();
    }
    else{
        *proof << "output NONE\n"
            << "conclusion BOUNDS " << std::to_string(best_objective_value) << " " << std::to_string(best_objective_value) << "\n"
            << "end pseudo-Boolean proof\n";
    }
}

void VeriPbProofLogger::write_conclusion_OPTIMAL(constraintid hint){
    if(best_objective_value == wght_max){
        write_conclusion_UNSAT_optimization();
    }
    else{
        *proof << "output NONE\n"
            << "conclusion BOUNDS " << std::to_string(best_objective_value) << " : " << std::to_string(hint) << " " << std::to_string(best_objective_value) << "\n"
            << "end pseudo-Boolean proof\n";
    }    
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
    if(!comments) return;

    *proof << "* f = ";
    for (int i = 0; i < objective_lits.size(); i++)
        write_weighted_literal(objective_lits[i], objective_weights[i]);
    if(objective_constant_cost != 0)
        *proof << " + " << std::to_string(objective_constant_cost);
    *proof << "; Current best solution: " << std::to_string(best_objective_value);
    *proof << "\n";
}

void VeriPbProofLogger::check_model_improving_constraint(constraintid cxn){
    if(cxn == 0) cxn = -1;
    *proof << "e";
    wght sumAllWeights = 0;
    for(int i = 0; i < objective_lits.size(); i++){
        write_weighted_literal(neg(objective_lits[i]), objective_weights[i]);
        sumAllWeights += objective_weights[i];
    }
    *proof << " >=";
    write_weight(sumAllWeights + objective_constant_cost - best_objective_value + 1);
    *proof << "; "; 
    *proof << cxn << "\n";
}

void VeriPbProofLogger::write_objective_update(){
    *proof << "obju new";
    for (int i = 0; i < objective_lits.size(); i++)
        write_weighted_literal(objective_lits[i], objective_weights[i]);
    if(objective_constant_cost != 0)
        *proof << " " << std::to_string(objective_constant_cost);
    if(objective_lits.size() == 0 && objective_constant_cost == 0)
        *proof << ' ';
    *proof << ";\n";
}

template <class TSeqLit, class TSeqSignedWght>
void VeriPbProofLogger::write_objective_update_diff(TSeqLit& litsOnewminusold, TSeqSignedWght& wghtsOnewminusold, signedWght constantOnewminusold){
    *proof << "obju diff";
    for(int i = 0; i < litsOnewminusold.size(); i++){
        write_signedWeight(wghtsOnewminusold[i]); 
        write_literal(litsOnewminusold[i]);
    }
    if(constantOnewminusold != 0)
        write_signedWeight(constantOnewminusold);
    *proof << ";\n";
}

template <class TLit>
void VeriPbProofLogger::write_objective_update_diff_for_literal(TLit& literal_to_remove, wght weight, wght constant_for_lit, bool write_update_model_improving_constraint){
    write_comment("write_objective_update_diff_for_literal. Weight = " + std::to_string(weight));
    *proof << "obju diff -" << weight; // TODO: use write_weight function to write the weight.
    write_literal(literal_to_remove);
    if(constant_for_lit > 0) 
        *proof << " " << constant_for_lit;
    *proof << ";\n";

    if(write_update_model_improving_constraint && get_model_improving_constraint() != 0){
        write_comment("Update model-improving constraint:");
        if(constant_for_lit > 0)
            rup_unit_clause(literal_to_remove, false);
            
        start_intCP_derivation(proof, get_model_improving_constraint());
        if(constant_for_lit > 0){
            if(weight == 1)
                intCP_add_constraint(proof, -1);
            else 
                intCP_add_constraint(proof, -1, weight);
        }
        else{
            if(weight == 1)
                intCP_add_literal_axiom(proof, literal_to_remove);
            else
                intCP_add_literal_axiom(proof, literal_to_remove, weight);
        }
        constraintid cxn = end_intCP_derivation(proof);
        update_model_improving_constraint(cxn);
    }
}

template <class TLit> 
void VeriPbProofLogger::write_objective_update_diff_literal_replacement(TLit& literal_to_remove, TLit& literal_to_add, wght weight, bool write_update_model_improving_constraint){
    *proof << "obju diff -" << weight << " "; //TODO: use the write_weight function to write the weight.
    write_literal(literal_to_remove);
    *proof << " " << weight;
    write_literal(literal_to_add);
    *proof << ";\n";

    if(write_update_model_improving_constraint && get_model_improving_constraint() != 0){
        write_comment("Update model-improving constraint:");
        constraintid cxn_newlit_leq_oldlit = rup_binary_clause(neg(literal_to_add), literal_to_remove);
        start_intCP_derivation(proof, get_model_improving_constraint());
        if(weight == 1)
            intCP_add_constraint(proof, cxn_newlit_leq_oldlit);
        else
            intCP_add_constraint(proof, cxn_newlit_leq_oldlit, weight);
        constraintid cxn = end_intCP_derivation(proof);
        update_model_improving_constraint(cxn);
    }
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
    // proof->flush(); // Can be uncommented for debugging reasons
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

    if(has_meaningful_name(v)){
        if(v.only_known_in_proof){
            return nameOnlyProofVars[v.v];
        }
        else{
            return nameSolverVars[v.v];
        }
    }
    else{
        if(v.only_known_in_proof){
            return "_p" + std::to_string(v.v);
        }
        else{
            return (is_aux_var(v) ? "y" : "x") + std::to_string(v.v);
        }
    }
}

VeriPB::Var VeriPbProofLogger::new_variable_only_in_proof(std::string name){
    VeriPB::Var v; 
    v.only_known_in_proof = true; 
    v.v = ++n_vars_only_known_in_proof;
    return v;
}

template <class TLit>
void VeriPbProofLogger::write_weighted_literal(const TLit &literal, wght weight)
{
    if(weight != 0){
        write_weight(weight);
        write_literal<TLit>(literal);
    }
}

void VeriPbProofLogger::write_weight(const wght weight){ 
    static constexpr size_t buffer_size = 8*sizeof(wght);
    static char             buffer[buffer_size]{}; 

    buffer[0] = ' ';
    std::to_chars_result result = std::to_chars(buffer + 1, buffer + buffer_size, weight);
    assert(result.ec != std::errc::value_too_large); 
    proof->write(buffer, result.ptr - buffer);
}

void VeriPbProofLogger::write_signedWeight(const signedWght weight){
    static constexpr size_t buffer_size = 8*sizeof(signedWght);
    static char             buffer[buffer_size]{}; 

    buffer[0] = ' ';
    std::to_chars_result result = std::to_chars(buffer + 1, buffer + buffer_size, weight);
    assert(result.ec != std::errc::value_too_large); 
    proof->write(buffer, result.ptr - buffer);
}

// The variable is the original variable of which we look to rewrite. In the first call, the literal is over the original variable.
// Next, it is checked whether the variable over which the literal is, should again be rewritten (i.e., variable x should be rewritten by literal ~y, and variable y should be rewritten to literal z).
// Once the variable of litvar is finaly not rewritten or the original variable, the correct literal is written.
std::string VeriPbProofLogger::to_string_rewrite_var_by_literal(VeriPB::Var& var, VeriPB::Lit& lit){
    VeriPB::Var litvar = variable(lit);

    // TODO: Check if var is not greater than size, otherwise, should not search for it.

    std::vector<VeriPB::Lit>* rewriteStorage = litvar.only_known_in_proof ? &vec_rewrite_proofonlyvar_by_literal : &vec_rewrite_solvervar_by_literal;

    if(litvar.v >= rewriteStorage->size() || (*rewriteStorage)[litvar.v] == VeriPB::lit_undef){
        return (is_negated(lit) ? "~" : "") + var_name(litvar);
    }
    else{
        VeriPB::Lit lit_to_rewrite_to = (*rewriteStorage)[litvar.v];

        if(is_negated(lit))
            lit_to_rewrite_to = neg(lit_to_rewrite_to);

        if(variable(lit_to_rewrite_to) == var)
            return (is_negated(lit_to_rewrite_to) ? "~" : "") + var_name(variable(lit_to_rewrite_to));
        else
            return to_string_rewrite_var_by_literal(var, lit_to_rewrite_to);
    }
}

void VeriPbProofLogger::write_varIdx(std::ostream* out, const VeriPB::VarIdx& varidx){
    static constexpr size_t buffer_size = 8*sizeof(VeriPB::VarIdx);
    static char             buffer[buffer_size]{}; 

    std::to_chars_result result = std::to_chars(buffer, buffer + buffer_size, varidx);
    assert(result.ec != std::errc::value_too_large);
    out->write(buffer, result.ptr - buffer);
}

void VeriPbProofLogger::write_var_name(std::ostream* out, const VeriPB::Var& v){
    // TODO: Add buffer as we do for writing weights in write_weighted_literal.
    if(has_meaningful_name(v)){
        if(v.only_known_in_proof){
            *out <<  nameOnlyProofVars[v.v];
        }
        else{
            *out << nameSolverVars[v.v];
        }
    }
    else{
        if(v.only_known_in_proof){
            *out << "_p";
        }
        else{
          *out << (is_aux_var(v) ? 'y' : 'x');
        }
        write_varIdx(out, v.v);
    }
}

void VeriPbProofLogger::write_literal_after_possible_rewrite(std::ostream* out, VeriPB::Var& _var, VeriPB::Lit& _lit){
    VeriPB::Var var = _var;
    VeriPB::Lit lit = _lit;
    while(true) {
        const VeriPB::Var litvar = variable(lit);
        const std::vector<VeriPB::Lit>* rewriteStorage = litvar.only_known_in_proof ? &vec_rewrite_proofonlyvar_by_literal : &vec_rewrite_solvervar_by_literal;

        if(litvar.v >= rewriteStorage->size() || (*rewriteStorage)[litvar.v] == VeriPB::lit_undef) {
            out->put(' ');
            if(is_negated(lit)) out->put('~');
            write_var_name(out, litvar);
            return;
        }

        const VeriPB::Lit lit_to_rewrite_to = is_negated(lit)?neg((*rewriteStorage)[litvar.v]):(*rewriteStorage)[litvar.v];

        if(variable(lit_to_rewrite_to) == var) {
            out->put(' ');
            if(is_negated(lit_to_rewrite_to)) out->put('~');
            write_var_name(out, var);
            return;
        }
        lit = lit_to_rewrite_to;
    }
} 

bool VeriPbProofLogger::write_variable_after_possible_rewrite(std::ostream* out, VeriPB::Var& _var, bool negated){
    const std::vector<VeriPB::Lit>* rewriteStorage = _var.only_known_in_proof ? &vec_rewrite_proofonlyvar_by_literal : &vec_rewrite_solvervar_by_literal;

    VeriPB::Var var = _var;

    while(true) {
    if(var.v >= rewriteStorage->size() || (*rewriteStorage)[var.v] == VeriPB::lit_undef){
        write_var_name(out, var);
        return negated;
    }

    const VeriPB::Lit lit_to_rewrite_to = (*rewriteStorage)[var.v];
    const VeriPB::Var var_to_rewrite_to = variable(lit_to_rewrite_to);

    negated = negated ^ is_negated(lit_to_rewrite_to);

    if(var_to_rewrite_to == var){
        write_var_name(out, var);
        return negated;
    }

    var = var_to_rewrite_to;
    }
    return false;
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
    VeriPB::Lit l = toVeriPbLit(lit);
    VeriPB::Var varl = variable(l);

    write_literal_after_possible_rewrite(proof, varl, l);
}

template <class TLit>
void VeriPbProofLogger::write_literal(std::ostream* out, const TLit &lit){
    VeriPB::Lit l = toVeriPbLit(lit);
    VeriPB::Var varl = variable(l);

    write_literal_after_possible_rewrite(out, varl, l);
}


template <class TSeqLit>
void VeriPbProofLogger::write_cardinality_constraint(const TSeqLit &lits, const wght RHS)
{
    for (int i = 0; i < lits.size(); i++)
        write_weighted_literal(lits[i], 1);
    *proof << " >=";
    write_weight(RHS);
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
    *proof << " >=";
    write_weight(RHS);
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
    *proof << " >=";
    write_weight(RHS + const_smaller - const_greater);
}

// ------------- Rewrite variables by literals -------------
template <class TVar, class TLit>
void VeriPbProofLogger::rewrite_variable_by_literal(const TVar& var, const TLit& lit)
{
    VeriPB::Lit _lit = toVeriPbLit(lit);
    VeriPB::Var _var = toVeriPbVar(var);
    VeriPB::VarIdx _idx = _var.v;

    std::vector<VeriPB::Lit>* rewriteStorage = _var.only_known_in_proof ? &vec_rewrite_proofonlyvar_by_literal : &vec_rewrite_solvervar_by_literal;

    if(_idx >=  rewriteStorage->size() && INIT_NAMESTORAGE > _idx){
        rewriteStorage->resize(INIT_NAMESTORAGE, VeriPB::lit_undef);
    }
    else if(_var.v >= rewriteStorage->size()){
        rewriteStorage->resize(2 * _var.v, VeriPB::lit_undef);
    }

    
    (*rewriteStorage)[_idx] = _lit;
}

// ------------- Meaningful names -------------

bool VeriPbProofLogger::has_meaningful_name(const VeriPB::Var& var){
    std::vector<bool>* meaningfulnameFlag = var.only_known_in_proof ? &onlyproofVarsSpecialNameFlag : &solverVarsSpecialNameFlag;

    return var.v < meaningfulnameFlag->size() && meaningfulnameFlag->at(var.v);
}

template <class TVar>
void VeriPbProofLogger::store_meaningful_name(const TVar &var, const std::string &name)
{
    VeriPB::Var _var = toVeriPbVar(var);
    std::vector<std::string>* nameStorage = _var.only_known_in_proof ? &nameOnlyProofVars : &nameSolverVars;
    std::vector<bool>* meaningfulnameFlag = _var.only_known_in_proof ? &onlyproofVarsSpecialNameFlag : &solverVarsSpecialNameFlag;

    // Increase storage if necessary.
    if(_var.v >= nameStorage->size() && INIT_NAMESTORAGE > _var.v ){
        nameStorage->resize(INIT_NAMESTORAGE);
    }
    else if(_var.v >= nameStorage->size()) {
        nameStorage->resize(2 * _var.v);
    }
    if(_var.v >= meaningfulnameFlag->size() && INIT_NAMESTORAGE > _var.v ){
        meaningfulnameFlag->resize(INIT_NAMESTORAGE);
    }
    else if(_var.v >= meaningfulnameFlag->size()) {
        meaningfulnameFlag->resize(2 * _var.v);
    }

    (*meaningfulnameFlag)[_var.v] = true;
    (*nameStorage)[_var.v] = name;
}

// ------------- Rules for checking constraints -------------

template <class TSeqLit>
void VeriPbProofLogger::equals_rule(const constraintid constraint_id, const TSeqLit &lits, const wght RHS)
{
    *proof << "e";
    write_cardinality_constraint(lits, RHS);
    *proof << "; " << constraint_id << "\n";
}

template <class TSeqLit, class TSeqWght>
void VeriPbProofLogger::equals_rule(const constraintid constraint_id, const TSeqLit &lits, const TSeqWght &weights, const wght RHS)
{
    *proof << "e";
    write_PB_constraint(lits, weights, RHS);
    *proof << "; " << constraint_id << "\n";
}

template <class TSeqLit, class TSeqWght>
void VeriPbProofLogger::equals_rule(const constraintid constraint_id, const TSeqLit& lits_greater, const TSeqWght& weights_greater, const wght const_greater, const TSeqLit& lits_smaller, const TSeqWght& weights_smaller, const wght const_smaller  ){
    
    *proof << "e";
    write_PB_constraint(lits_greater, weights_greater, const_greater, lits_smaller, weights_smaller, const_smaller);    
   *proof << "; " << constraint_id << "\n";
}
template <class TSeqLit>
void VeriPbProofLogger::equals_rule_LEQ(const constraintid constraint_id, const TSeqLit &lits, const wght RHS){
    *proof << "e";
    for(int i = 0; i < lits.size(); i++){
        write_weighted_literal(neg(lits[i]), 1);
    }
    *proof << " >="; 
    write_weight(lits.size() - RHS);
    *proof <<  " ; " << constraint_id << "\n";
}

template <class TSeqLit, class TSeqWght>
void VeriPbProofLogger::equals_rule_LEQ(const constraintid constraint_id, const TSeqLit &lits, const TSeqWght &weights, const wght RHS){
    assert(lits.size() == weights.size());
    wght max = 0;
    *proof << "e";
    for(int i = 0; i < lits.size(); i++){
        max += weights[i];
        write_weighted_literal(neg(lits[i]), 1);
    }
    *proof << " >=";
    write_weight(max - RHS);
    *proof <<  "; " << constraint_id << "\n";
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
    *proof << "e";
    write_cardinality_constraint(lits, RHS);
    *proof << ";\n";
}

template <class TSeqLit, class TSeqWght>
void VeriPbProofLogger::check_constraint_exists(const TSeqLit &lits, const TSeqWght &weights, const wght RHS)
{
    *proof << "e";
    write_PB_constraint(lits, weights, RHS);
    *proof << ";\n";
}

template <class TSeqLit, class TSeqWght>
void VeriPbProofLogger::check_constraint_exists(const TSeqLit& lits_greater, const TSeqWght& weights_greater, const wght const_greater, const TSeqLit& lits_smaller, const TSeqWght& weights_smaller, const wght const_smaller  ){
    *proof << "e";
    write_PB_constraint(lits_greater, weights_greater, const_greater, lits_smaller, weights_smaller, const_smaller);
    *proof << ";\n";
}
// ------------- Rules for adding implied constraints -------------
template <class TSeqLit>
constraintid VeriPbProofLogger::derive_if_implied(const constraintid hint, const TSeqLit &lits, const wght RHS){
    
    *proof << "ia";
    write_cardinality_constraint(lits, RHS);
    *proof << "; " << hint << "\n";
    return ++constraint_counter;
}
template <class TSeqLit, class TSeqWght>
constraintid VeriPbProofLogger::derive_if_implied(const constraintid hint, const TSeqLit &lits, const TSeqWght &weights, const wght RHS){
    *proof << "ia";
    write_PB_constraint(lits, weights, RHS);
    *proof << "; " << hint << "\n";
    return ++constraint_counter;
}
template <class TSeqLit, class TSeqWght>
constraintid VeriPbProofLogger::derive_if_implied(const constraintid hint, const TSeqLit& lits_greater, const TSeqWght& weights_greater, const wght const_greater, const TSeqLit& lits_smaller, const TSeqWght& weights_smaller, const wght const_smaller  ){
    *proof << "ia";
    write_PB_constraint(lits_greater, weights_greater, const_greater, lits_smaller, weights_smaller, const_smaller);    
    *proof << "; " << hint << "\n";
    return ++constraint_counter;
}

template <class TSeqLit>
constraintid VeriPbProofLogger::derive_if_implied(const TSeqLit &lits, const wght RHS){
    *proof << "ia";
    write_cardinality_constraint(lits, RHS);
    *proof << ";\n";
    return ++constraint_counter;
}
template <class TSeqLit, class TSeqWght>
constraintid VeriPbProofLogger::derive_if_implied(const TSeqLit &lits, const TSeqWght &weights, const wght RHS){
    *proof << "ia";
    write_PB_constraint(lits, weights, RHS);
    *proof << ";\n";
    return ++constraint_counter;
}
template <class TSeqLit, class TSeqWght>
constraintid VeriPbProofLogger::derive_if_implied(const TSeqLit& lits_greater, const TSeqWght& weights_greater, const wght const_greater, const TSeqLit& lits_smaller, const TSeqWght& weights_smaller, const wght const_smaller  ){
    *proof << "ia";
    write_PB_constraint(lits_greater, weights_greater, const_greater, lits_smaller, weights_smaller, const_smaller);    
    *proof << ";\n";
    return ++constraint_counter;
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
    if(log_as_comment && !comments) return get_model_improving_constraint();

    write_comment("Solution with objective value: " + std::to_string(objective_value));
    *proof << (log_as_comment ? "*" : "soli");
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

        check_model_improving_constraint(-1);
    }

    return get_model_improving_constraint(); 
}

template <class TSeqLit>
constraintid VeriPbProofLogger::log_solution_with_check(const TSeqLit &model, bool only_original_variables_necessary, bool log_nonimproving_solution_as_comment)
{
    wght current_objective_value = calculate_objective_value(model);
    if (current_objective_value < best_objective_value)
    {
        if(comments){
            write_comment_objective_function();
            write_comment("Objective update from " + std::to_string(best_objective_value) + " to " + std::to_string(current_objective_value));
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

constraintid VeriPbProofLogger::get_model_improving_constraint()
{
    return model_improvement_constraint;
}

void VeriPbProofLogger::update_model_improving_constraint(constraintid newmic){
    model_improvement_constraint = newmic;
    if(comments)
        write_comment("Model improving constraint: " + std::to_string(newmic));
}

wght VeriPbProofLogger::get_best_objective_value(){
    return best_objective_value;
}

template <class TSeqLBool>
constraintid VeriPbProofLogger::log_solution_lbools(TSeqLBool &model, wght objective_value)
{
    if(comments)
        write_comment("Model improvement update with objective value = " + std::to_string(objective_value));

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


// ------------- Unchecked Assumptions -------------
template <class TSeqLit>
constraintid VeriPbProofLogger::unchecked_assumption(const TSeqLit &lits, const wght RHS)
{
    *proof << "a";
    write_cardinality_constraint(lits, RHS);
    *proof << " ;\n";
    return ++constraint_counter;
}

template <class TSeqLit, class TSeqWght>
constraintid VeriPbProofLogger::unchecked_assumption(const TSeqLit &lits, const TSeqWght &weights, const wght RHS)
{
    *proof << "a";
    write_PB_constraint(lits, weights, RHS);
    *proof << ";\n";
    return ++constraint_counter;
}

template <class TLit> 
constraintid VeriPbProofLogger::unchecked_assumption_unit_clause(const TLit& lit, bool core_constraint){
    *proof << "a";
    write_weighted_literal(lit, 1);
    *proof << " >= 1;\n";
    if(core_constraint)
        move_to_coreset(-1);
    return ++constraint_counter;
}

// ------------- Reverse Unit Propagation -------------
template <class TSeqLit>
constraintid VeriPbProofLogger::rup(const TSeqLit &lits, const wght RHS)
{
    *proof << "u";
    write_cardinality_constraint(lits, RHS);
    *proof << ";\n";
    return ++constraint_counter;
}

template <class TSeqLit, class TSeqWght>
constraintid VeriPbProofLogger::rup(const TSeqLit &lits, const TSeqWght &weights, const wght RHS)
{
    *proof << "u";
    write_PB_constraint(lits, weights, RHS);
    *proof << " ;\n";
    return ++constraint_counter;
}

template <class TSeqLit>
constraintid VeriPbProofLogger::rup_clause(const TSeqLit& lits){
    *proof << "u";
    write_weighted_literal(lits[0]);
    for(int i = 1; i < lits.size(); i++){
        if(lits[i] != lits[i-1])
            write_weighted_literal(lits[i]);
    }
    *proof << " >= 1;\n";
    return ++constraint_counter;
}

template <class TLit>
constraintid VeriPbProofLogger::rup_unit_clause(const TLit& lit, bool core_constraint){
    *proof << "u";
    write_weighted_literal(lit);
    *proof << " >= 1;\n";

    if(core_constraint)
        move_to_coreset(-1);

    return ++constraint_counter;
}

template <class TLit>
constraintid VeriPbProofLogger::rup_binary_clause(const TLit& lit1, const TLit& lit2, bool core_constraint){
    *proof << "u";
    write_weighted_literal(lit1);
    write_weighted_literal(lit2);
    *proof << " >= 1;\n";

    if(core_constraint)
        move_to_coreset(-1);

    return ++constraint_counter;
}

template <class TLit> 
constraintid VeriPbProofLogger::rup_ternary_clause(const TLit& lit1, const TLit& lit2, const TLit& lit3, bool core_constraint){
    *proof << "u";
    write_weighted_literal(lit1);
    write_weighted_literal(lit2);
    write_weighted_literal(lit3);
    *proof << " >= 1;\n";

    if(core_constraint)
        move_to_coreset(-1);

    return ++constraint_counter;
}

// **********************

template <class TSeqLit>
constraintid VeriPbProofLogger::rup(const TSeqLit &lits, const wght RHS, std::vector<constraintid>& hints)
{
    *proof << "u";
    write_cardinality_constraint(lits, RHS);
    *proof << ";";
    for(constraintid hint : hints) 
        *proof << ' ' << hint;
    *proof << "\n";
    return ++constraint_counter;
}

template <class TSeqLit, class TSeqWght>
constraintid VeriPbProofLogger::rup(const TSeqLit &lits, const TSeqWght &weights, const wght RHS, std::vector<constraintid>& hints)
{
    *proof << "u";
    write_PB_constraint(lits, weights, RHS);
    *proof << ";";
    for(constraintid hint : hints) 
        *proof << ' ' << hint;
    *proof << "\n";
    return ++constraint_counter;
}

template <class TSeqLit>
constraintid VeriPbProofLogger::rup_clause(const TSeqLit& lits, std::vector<constraintid>& hints){
    *proof << "u";
    write_weighted_literal(lits[0]);
    for(int i = 1; i < lits.size(); i++){
        if(lits[i] != lits[i-1])
            write_weighted_literal(lits[i]);
    }
    *proof << " >= 1;";
    for(constraintid hint : hints) 
        *proof << ' ' << hint ;
    *proof << "\n";
    return ++constraint_counter;
}

template <class TLit>
constraintid VeriPbProofLogger::rup_unit_clause(const TLit& lit, std::vector<constraintid>& hints, bool core_constraint){
    *proof << "u";
    write_weighted_literal(lit);
    *proof << " >= 1;";
    for(constraintid hint : hints) 
        *proof << ' ' << hint ;
    *proof << "\n";

    if(core_constraint)
        move_to_coreset(-1);

    return ++constraint_counter;
}

template <class TLit>
constraintid VeriPbProofLogger::rup_binary_clause(const TLit& lit1, const TLit& lit2, std::vector<constraintid>& hints, bool core_constraint){
    *proof << "u";
    write_weighted_literal(lit1);
    write_weighted_literal(lit2);
    *proof << " >= 1;";
    for(constraintid hint : hints) 
        *proof << ' ' << hint ;
    *proof << "\n";

    if(core_constraint)
        move_to_coreset(-1);

    return ++constraint_counter;
}

template <class TLit> 
constraintid VeriPbProofLogger::rup_ternary_clause(const TLit& lit1, const TLit& lit2, const TLit& lit3, std::vector<constraintid>& hints,  bool core_constraint){
    *proof << "u";
    write_weighted_literal(lit1);
    write_weighted_literal(lit2);
    write_weighted_literal(lit3);
    *proof << " >= 1;";
    for(constraintid hint : hints) 
        *proof << ' ' << hint ;
    *proof << "\n";

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
void VeriPbProofLogger::add_substitution(substitution &sub, const substitution &sub_to_add){
    sub.first.reserve(sub.first.size() + sub_to_add.first.size());
    for(auto lit_ass : sub_to_add.first)
        add_literal_assignment(sub, lit_ass.first, lit_ass.second);
    
    sub.second.reserve(sub.second.size() + sub_to_add.second.size());
    for(auto bool_ass : sub_to_add.second)
        add_boolean_assignment(sub, bool_ass.first, bool_ass.second);
}

void VeriPbProofLogger::write_substitution(const substitution &witness)
{
    VeriPB::Var varfrom, varto;
    VeriPB::Lit lit_to;
    bool rewritten_to_negated_literal, boolto;

    for(auto lit_ass : witness.first){
        varfrom = lit_ass.first;
        lit_to = lit_ass.second;
        varto = variable(lit_to);

        rewritten_to_negated_literal = write_variable_after_possible_rewrite(proof, varfrom);
        *proof << " ->";

        lit_to.negated ^= rewritten_to_negated_literal;
        write_literal_after_possible_rewrite(proof, varto, lit_to);
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
    return false;
}

template <class TVar>
VeriPB::Lit VeriPbProofLogger::get_literal_assignment(substitution &s, const TVar& var){
    for(auto ass : s.first){
        if(ass.first == toVeriPbVar(var))
            return ass.second;
    }
    std::cout << "ERROR: Proof logging library: Could not find literal assignment for variable " << var_name(var) << std::endl;
    assert(false);
    return VeriPB::lit_undef;
}

size_t VeriPbProofLogger::get_substitution_size(const substitution &s){
    return s.first.size() + s.second.size();
}

template <class TSeqLit>
constraintid VeriPbProofLogger::redundanceBasedStrengthening(const TSeqLit &lits, const wght RHS, const substitution &witness)
{
    *proof << "red";
    write_cardinality_constraint(lits, RHS);
    *proof << "; ";
    write_substitution(witness);
    *proof << "\n";
    return ++constraint_counter;
}

template <class TSeqLit, class TSeqWght>
constraintid VeriPbProofLogger::redundanceBasedStrengthening(const TSeqLit &lits, const TSeqWght &weights, const wght RHS, const substitution &witness)
{
    *proof << "red";
    write_PB_constraint(lits, weights, RHS);
    *proof << "; ";
    write_substitution(witness);
    *proof << "\n";
    return ++constraint_counter;
}

template <class TSeqLit, class TSeqWght>
constraintid VeriPbProofLogger::redundanceBasedStrengthening(const TSeqLit &lits, const TSeqWght &weights, const wght RHS, const substitution &witness, std::vector<subproof>& subproofs){
    *proof << "red";
    write_PB_constraint(lits, weights, RHS);
    *proof << "; ";
    write_substitution(witness);

    if(subproofs.size() > 0){
        constraint_counter++; // constraint not C
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
        *proof << "end";
    }
    *proof << "\n";

    return ++constraint_counter;
}


template <class TSeqLit>
constraintid VeriPbProofLogger::redundanceBasedStrengthening(const TSeqLit &lits, const wght RHS, const substitution &witness, std::vector<subproof>& subproofs){
    *proof << "red";
    write_cardinality_constraint(lits, RHS);
    *proof << "; ";
    write_substitution(witness);

    if(subproofs.size() > 0){
        constraint_counter++; // constraint not C
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
        *proof << "end";
    }
    *proof << "\n";

    return ++constraint_counter;
}

template <class TLit>
constraintid VeriPbProofLogger::redundanceBasedStrengtheningUnitClause(const TLit& lit){
    *proof << "red";
    write_weighted_literal(lit);
    *proof << " >= 1; "; 
    
    VeriPB::Var var = toVeriPbVar(variable(lit));
    bool rewritten_to_negated_literal = write_variable_after_possible_rewrite(proof, var, is_negated(lit));
    *proof << " -> ";
    *proof << !rewritten_to_negated_literal << "\n";
    
    write_comment("variable: " + var_name(var) + " negated literal = " + std::to_string(is_negated(lit)) + " rewritten_to_negated_lit = " + std::to_string(rewritten_to_negated_literal));

    return ++constraint_counter;
}


// ------------- Reification Literals -------------
// Proves the constraints encoding the reification constraint l <-> C, with l a literal and C a boolean constraint.
// The right implication is the encoding of l -> C, whereas the left implication means l <- C.
template <class TSeqLit, class TSeqWght, class TLit>
constraintid VeriPbProofLogger::reificationLiteralRightImpl(const TLit& lit, const TSeqLit &lits, const TSeqWght &weights, const wght RHS, bool store_reified_constraint){
    int i;

    if(comments){
        std::string comment = to_string(lit) + " -> " ;
        for(i = 0; i < lits.size(); i++)
            comment += std::to_string(weights[i]) + " " + to_string(lits[i]) + " ";
        comment += ">= " + std::to_string(RHS);
        write_comment(comment);
    }
    
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
        setReifiedConstraintRightImpl(variable(lit), cxnid);

    return cxnid;
}

template <class TSeqLit, class TLit>
constraintid VeriPbProofLogger::reificationLiteralRightImpl(const TLit& lit, const TSeqLit &lits, const wght RHS, bool store_reified_constraint){
    int i;

    if(comments){
        std::string comment = to_string(lit) + " -> " ;
        for(i = 0; i < lits.size(); i++)
            comment +=  "1 " + to_string(lits[i]) + " ";
        comment += ">= " + std::to_string(RHS);
        write_comment(comment);
    }
    
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
        setReifiedConstraintRightImpl(variable(lit), cxnid);

    return cxnid;
}

template <class TSeqLit, class TSeqWght, class TLit>
constraintid VeriPbProofLogger::reificationLiteralRightImpl(const TLit& lit, const TSeqLit &lits, const TSeqWght &weights, const wght RHS, int start_constraint, int end_constraint, bool store_reified_constraint){
    int i;

    if(comments){
        std::string comment = to_string(lit) + " -> " ;
        for(i = start_constraint; i < end_constraint; i++)
            comment += std::to_string(weights[i]) + " " + to_string(lits[i]) + " ";
        comment += ">= " + std::to_string(RHS);
        write_comment(comment);
    }

    std::vector<VeriPB::Lit> _lits; _lits.resize(end_constraint-start_constraint+1);
    std::vector<wght> _weights;  _weights.resize(end_constraint-start_constraint+1);

    VeriPB::Lit _neglit = toVeriPbLit(neg(lit));

    wght sum_of_weights = 0;

    for(i = start_constraint; i < end_constraint; i++){
        sum_of_weights += weights[i];

        _weights[i-start_constraint] = weights[i];
        _lits[i-start_constraint] = toVeriPbLit(lits[i]);
    }
    _lits[i-start_constraint] = _neglit;
    _weights[i-start_constraint] = RHS;

    substitution witness = get_new_substitution();
    add_boolean_assignment(witness, variable(_neglit), !is_negated(_neglit));

    constraintid cxnid = redundanceBasedStrengthening(_lits, _weights, RHS, witness);

    if(store_reified_constraint)
        setReifiedConstraintRightImpl(variable(lit), cxnid);

    return cxnid;
}

// ------
template <class TSeqLit, class TSeqWght, class TLit>
constraintid VeriPbProofLogger::reificationLiteralRightImplLeq(const TLit& lit, const TSeqLit &lits, const TSeqWght &weights, const wght RHS, int start_constraint, int end_constraint, bool store_reified_constraint){
    int i;

    if(comments){
        std::string comment = to_string(lit) + " -> " ;
        for(i = start_constraint; i < end_constraint; i++)
            comment += std::to_string(weights[i]) + " " + to_string(lits[i]) + " ";
        comment += "=< " + std::to_string(RHS);
        write_comment(comment);
    }

    std::vector<VeriPB::Lit> _lits; _lits.resize(end_constraint-start_constraint+1);
    std::vector<wght> _weights;  _weights.resize(end_constraint-start_constraint+1);

    VeriPB::Lit _neglit = toVeriPbLit(neg(lit));

    wght sumofweights=0;

    for(i = start_constraint; i < end_constraint; i++){
        sumofweights += weights[i] ;

        _weights[i-start_constraint] = weights[i];
        _lits[i-start_constraint] = toVeriPbLit(neg(lits[i]));
    }

    uint64_t _RHS = sumofweights - RHS;

    _lits[i-start_constraint] = _neglit;
    _weights[i-start_constraint] = _RHS;

    substitution witness = get_new_substitution();
    add_boolean_assignment(witness, variable(_neglit), !is_negated(_neglit));

    constraintid cxnid = redundanceBasedStrengthening(_lits, _weights, _RHS, witness);

    if(store_reified_constraint)
        setReifiedConstraintRightImpl(variable(lit), cxnid);

    return cxnid;
}

template <class TSeqLit, class TSeqWght, class TLit>
constraintid VeriPbProofLogger::reificationLiteralLeftImpl(const TLit& lit, const TSeqLit &lits, const TSeqWght &weights, const wght RHS, bool store_reified_constraint){
    int i;

    if(comments){
        std::string comment = to_string(lit) + " <- " ;
        for(i = 0; i < lits.size(); i++)
            comment += std::to_string(weights[i]) + " " + to_string(lits[i]) + " ";
        comment += ">= " + std::to_string(RHS);
        write_comment(comment);
    }

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
        setReifiedConstraintLeftImpl(variable(lit), cxnid);

    return cxnid;
}

template <class TSeqLit, class TSeqWght, class TLit>
constraintid VeriPbProofLogger::reificationLiteralLeftImpl(const TLit& lit, const TSeqLit &litsC, const TSeqWght &weights, const wght RHS, int start_constraint, int end_constraint, bool store_reified_constraint){
    int i;

    if(comments){
        std::string comment = to_string(lit) + " <- " ;
        for(i = start_constraint; i < end_constraint; i++)
            comment += std::to_string(weights[i]) + " " + to_string(litsC[i]) + " ";
        comment += ">= " + std::to_string(RHS);
        write_comment(comment);
    }
    std::vector<VeriPB::Lit> _lits; _lits.resize(end_constraint - start_constraint +1);
    std::vector<wght> _weights;  _weights.resize(end_constraint - start_constraint +1);

    VeriPB::Lit _lit = toVeriPbLit(lit);

    wght sum_of_weights = 0;

    for(i = start_constraint; i < end_constraint; i++){
        sum_of_weights += weights[i];

        _weights[i-start_constraint] = weights[i];
        _lits[i-start_constraint] = toVeriPbLit(neg(litsC[i]));
    }
    
    wght j; 
    if(sum_of_weights + 1 >= RHS)
        j = sum_of_weights - RHS + 1 ;
    else
        j = 0;

    _lits[i-start_constraint] = _lit;
    _weights[i-start_constraint] = j;

    substitution witness = get_new_substitution();
    add_boolean_assignment(witness, variable(_lit), !is_negated(_lit));

    constraintid cxnid = redundanceBasedStrengthening(_lits, _weights, RHS, witness);

    if(store_reified_constraint)
        setReifiedConstraintLeftImpl(variable(lit), cxnid);

    return cxnid;
}


template <class TSeqLit, class TSeqWght, class TLit>
constraintid VeriPbProofLogger::reificationLiteralLeftImplLeq(const TLit& lit, const TSeqLit &litsC, const TSeqWght &weights, const wght RHS, int start_constraint, int end_constraint, bool store_reified_constraint){
    int i;

    if(comments){
        std::string comment = to_string(lit) + " <- " ;
        for(i = start_constraint; i < end_constraint; i++)
            comment += std::to_string(weights[i]) + " " + to_string(litsC[i]) + " ";
        comment += "=< " + std::to_string(RHS);
        write_comment(comment);
    }

    std::vector<VeriPB::Lit> _lits; _lits.resize(end_constraint - start_constraint +1);
    std::vector<wght> _weights;  _weights.resize(end_constraint - start_constraint +1);

    VeriPB::Lit _lit = toVeriPbLit(lit);

    for(i = start_constraint; i < end_constraint; i++){
        _weights[i-start_constraint] = weights[i];
        _lits[i-start_constraint] = toVeriPbLit(litsC[i]);
    }
    
    _lits[i-start_constraint] = _lit;
    wght _RHS = RHS + 1;
    _weights[i-start_constraint] = _RHS;

    substitution witness = get_new_substitution();
    add_boolean_assignment(witness, variable(_lit), !is_negated(_lit));

    constraintid cxnid = redundanceBasedStrengthening(_lits, _weights, _RHS, witness);

    if(store_reified_constraint)
        setReifiedConstraintLeftImpl(variable(lit), cxnid);

    return cxnid;
}



template <class TSeqLit, class TLit>
constraintid VeriPbProofLogger::reificationLiteralLeftImpl(const TLit& lit, const TSeqLit &lits, const wght RHS, bool store_reified_constraint){
    int i;

    if(comments){
        std::string comment = to_string(lit) + " <- " ;
        for(i = 0; i < lits.size(); i++)
            comment += "1 " + to_string(lits[i]) + " ";
        comment += ">= " + std::to_string(RHS);
        write_comment(comment);
    }
    
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
        setReifiedConstraintLeftImpl(variable(lit), cxnid);

    return cxnid;
}

template <class TVar>
constraintid VeriPbProofLogger::getReifiedConstraintLeftImpl(const TVar& var){
    VeriPB::Var _var = toVeriPbVar(var);
    std::vector<constraintid>* storage = _var.only_known_in_proof ? &reifiedConstraintLeftImplOnlyProofVars : &reifiedConstraintLeftImpl;
    constraintid cxn = undefcxn;
    if(_var.v < storage->size())
        cxn = (*storage)[_var.v];
    
    // if(cxn == undefcxn)
    //     std::cout << "ERROR: Cannot find left reification constraint for variable " << var_name(var) << std::endl;

    // assert(cxn != undefcxn);

    return cxn;
}

template <class TVar>
constraintid VeriPbProofLogger::getReifiedConstraintRightImpl(const TVar& var){
    VeriPB::Var _var = toVeriPbVar(var);
    std::vector<constraintid>* storage = _var.only_known_in_proof ? &reifiedConstraintRightImplOnlyProofVars : &reifiedConstraintRightImpl;
    constraintid cxn = undefcxn;
    if(_var.v < storage->size())
        cxn = (*storage)[_var.v];
    
    // if(cxn == undefcxn)
    //     std::cout << "ERROR: Cannot find right reification constraint for variable " << var_name(var) << std::endl;
    

    // assert(cxn != undefcxn);

    return cxn;
}

template <class TVar>
void VeriPbProofLogger::setReifiedConstraintLeftImpl(const TVar& var, constraintid cxnId){
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

template <class TVar>
void VeriPbProofLogger::setReifiedConstraintRightImpl(const TVar& var, constraintid cxnId){
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

template <class TVar>
void VeriPbProofLogger::deleteReifiedConstraintLeftImpl(const TVar& var){
    VeriPB::Var _var = toVeriPbVar(var);

    std::vector<constraintid>* storage = _var.only_known_in_proof ? &reifiedConstraintLeftImplOnlyProofVars : &reifiedConstraintLeftImpl;

    if(_var.v <= storage->size()){
        delete_constraint_by_id((*storage)[_var.v]);
        (*storage)[_var.v] = undefcxn;
    }
}

template <class TVar>
void VeriPbProofLogger::deleteReifiedConstraintRightImpl(const TVar& var){
    VeriPB::Var _var = toVeriPbVar(var);

    std::vector<constraintid>* storage = _var.only_known_in_proof ? &reifiedConstraintRightImplOnlyProofVars : &reifiedConstraintRightImpl;

    if(_var.v <= storage->size()){
        delete_constraint_by_id((*storage)[_var.v]);
        (*storage)[_var.v] = undefcxn;
    }
}

/**
 * Remove the right reification constraint from the reification constraint store without deleting it in the proof. 
 * Only needed for not maintaining a constraint id in memory that will not be used in the proof anymore.
*/
template <class TVar>
void VeriPbProofLogger::removeReifiedConstraintRightImplFromConstraintStore(const TVar& var){
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
template <class TVar>
void VeriPbProofLogger::removeReifiedConstraintLeftImplFromConstraintStore(const TVar& var){
    VeriPB::Var _var = toVeriPbVar(var);

    std::vector<constraintid>* storage = _var.only_known_in_proof ? &reifiedConstraintLeftImplOnlyProofVars : &reifiedConstraintLeftImpl;

    if(_var.v <= storage->size()){
        delete_constraint_by_id((*storage)[_var.v]);
        (*storage)[_var.v] = undefcxn;
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

constraintid VeriPbProofLogger::copy_constraint(const constraintid cxn){
    *proof << "p " << cxn << "\n";
    return ++constraint_counter;
}

void VeriPbProofLogger::start_intCP_derivation(const constraintid constraint_id)
{
    pol_string.clear();
    pol_string << "p " << constraint_id;
}

template <class TLit>
void VeriPbProofLogger::start_intCP_derivation_with_lit_axiom(const TLit &lit)
{
    pol_string << "p";
    write_literal(&pol_string, lit);
}

void VeriPbProofLogger::intCP_load_constraint(const constraintid constraint_id)
{
    pol_string << " " << constraint_id;
}

void VeriPbProofLogger::intCP_add()
{
    pol_string << " +";
}

void VeriPbProofLogger::intCP_add_constraint(const constraintid constraint_id)
{
    pol_string << " " << constraint_id << " +";
}

void VeriPbProofLogger::intCP_add_constraint(const constraintid constraint_id, wght mult){
    pol_string << " " << constraint_id << " " << mult << " * +";
}

template <class TLit>
void VeriPbProofLogger::intCP_add_literal_axiom(const TLit &lit){ 
    write_literal(&pol_string, lit);
    pol_string << " +";
}

template <class TLit>
void VeriPbProofLogger::intCP_add_literal_axiom(const TLit &lit, wght mult){
    write_literal(&pol_string, lit); 
    pol_string << " " << mult <<  " * +";
}

void VeriPbProofLogger::intCP_divide(const wght v)
{
    // TODO: modify write_weight such that I can write the weight to pol_string
    pol_string << " " << v << " d";
}
void VeriPbProofLogger::intCP_saturate()
{
    pol_string << " s";
}
void VeriPbProofLogger::intCP_multiply(const wght v)
{
    // TODO: modify write_weight such that I can write the weight to pol_string
    pol_string << " " << v << " *";
}

template <class TVar>
void VeriPbProofLogger::intCP_weaken(const TVar &var)
{
    pol_string.put(' ');
    write_var_name(&pol_string, toVeriPbVar(var));
    pol_string << " w"; 
}

template <class TLit>
void VeriPbProofLogger::intCP_write_literal_axiom(const TLit &lit)
{
    write_literal(&pol_string, lit);
}

void VeriPbProofLogger::intCP_apply(const cuttingplanes_derivation& cpder){
    pol_string << " " << cpder;
}

constraintid VeriPbProofLogger::end_intCP_derivation()
{
    *proof << pol_string.rdbuf() << "\n";
    return ++constraint_counter;
}

void VeriPbProofLogger::clear_intCP_derivation(){
    pol_string.clear();
}
cuttingplanes_derivation VeriPbProofLogger::get_CPder_from_intCP(){
    return pol_string.str();
}

// ---------------------------------

void VeriPbProofLogger::start_intCP_derivation(std::ostream* cp, const constraintid constraint_id)
{
    cp->clear();
    *cp << "p " << constraint_id;
}

template <class TLit>
void VeriPbProofLogger::start_intCP_derivation_with_lit_axiom(std::ostream* cp, const TLit &lit)
{
    *cp << "p ";
    *cp << to_string(lit);
}

void VeriPbProofLogger::intCP_load_constraint(std::ostream* cp, const constraintid constraint_id)
{
    *cp << " " << constraint_id;
}

void VeriPbProofLogger::intCP_add(std::ostream* cp)
{
    *cp << " +";
}

void VeriPbProofLogger::intCP_add_constraint(std::ostream* cp, const constraintid constraint_id)
{
    *cp << " " << constraint_id << " +";
}

void VeriPbProofLogger::intCP_add_constraint(std::ostream* cp, const constraintid constraint_id, wght mult){
    *cp << " " << constraint_id << " " << mult << " * +";
}

template <class TLit>
void VeriPbProofLogger::intCP_add_literal_axiom(std::ostream* cp, const TLit &lit){
    write_literal(cp, lit); *cp << " +";
}

template <class TLit>
void VeriPbProofLogger::intCP_add_literal_axiom(std::ostream* cp, const TLit &lit, wght mult){
    write_literal(cp, lit); 
    *cp << " " << mult <<  " * +"; //TODO: use write_weight
}

void VeriPbProofLogger::intCP_divide(std::ostream* cp, const wght v)
{
    *cp << " " << v << " d";
}
void VeriPbProofLogger::intCP_saturate(std::ostream* cp)
{
    *cp << " s";
}
void VeriPbProofLogger::intCP_multiply(std::ostream* cp, const wght v)
{
    *cp << " " << v << " *"; //TODO: use write_weight
}

template <class TVar>
void VeriPbProofLogger::intCP_weaken(std::ostream* cp, const TVar &var)
{
    cp->put(' ');
    write_var_name(cp, toVeriPbVar(var));
    *cp << " w"; 
}

template <class TLit>
void VeriPbProofLogger::intCP_write_literal_axiom(std::ostream* cp, const TLit &lit){
    *cp << " " << to_string(lit);
}

void VeriPbProofLogger::intCP_apply(std::ostream* cp, const cuttingplanes_derivation& cpder){
    *cp << " " << cpder;
}
    

constraintid VeriPbProofLogger::end_intCP_derivation(std::ostream* cp)
{
    if(cp == proof)
        *proof << "\n";
    else
        *proof << cp->rdbuf() << "\n";
    return ++constraint_counter;
}



void VeriPbProofLogger::clear_intCP_derivation(std::ostream* cp){
    cp->clear();
}


// ------------- Extra Proof Techniques -------------

template <class TSeqLit, class TSeqWght>
constraintid VeriPbProofLogger::prove_by_contradiction(TSeqLit& lits, TSeqWght& weights, wght RHS, std::vector<cuttingplanes_derivation>& cpder){
    std::vector<subproof> subproofs;
    subproofs.push_back({"#1", cpder});
    substitution witness = get_new_substitution();
    return redundanceBasedStrengthening(lits, weights, RHS, witness, subproofs );
}

template <class TSeqLit>
constraintid VeriPbProofLogger::prove_by_contradiction(TSeqLit& lits, wght RHS, std::vector<cuttingplanes_derivation>& cpder){
    std::vector<subproof> subproofs;
    subproofs.push_back({"#1", cpder});
    substitution witness = get_new_substitution();
    return redundanceBasedStrengthening(lits, RHS, witness, subproofs );
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

void VeriPbProofLogger::delete_constraint_by_id(const constraintid constraint_id, bool overrule_keeporiginalformula)
{
    if(!(keep_original_formula && is_original_constraint(constraint_id)) || overrule_keeporiginalformula)
        *proof << "del id " << constraint_id << "\n";
}

void VeriPbProofLogger::delete_constraint_by_id(const std::vector<constraintid> &constraint_ids, bool overrule_keeporiginalformula)
{
    *proof << "del id";
    for (constraintid id : constraint_ids)
    {
        if(!(keep_original_formula && is_original_constraint(id)) || overrule_keeporiginalformula)
            *proof << " " << id;
    }
    *proof << "\n";
}

template <class TSeqLit>
void VeriPbProofLogger::delete_constraint(const TSeqLit &lits, const wght RHS, bool overrule_keeporiginalformula)
{
    assert(!keep_original_formula || overrule_keeporiginalformula);

    *proof << "del spec";
    write_cardinality_constraint(lits, RHS);
    *proof << ";\n";
}

template <class TSeqLit>
void VeriPbProofLogger::delete_constraint(const TSeqLit &lits, const wght RHS, const substitution& witness, bool overrule_keeporiginalformula){
    assert(!keep_original_formula || overrule_keeporiginalformula);
    *proof << "del spec";
    write_cardinality_constraint(lits, RHS);
    *proof << "; ";
    write_substitution(witness);
    *proof << "\n";
}

template <class TSeqLit, class TSeqWght>
void VeriPbProofLogger::delete_constraint(const TSeqLit &lits, const TSeqWght &weights, const wght RHS, bool overrule_keeporiginalformula)
{
    assert(!keep_original_formula || overrule_keeporiginalformula);
    *proof << "del spec";
    write_PB_constraint(lits, weights, RHS);
    *proof << ";\n";
}

template <class TSeqLit, class TSeqWght>
void VeriPbProofLogger::delete_constraint(const TSeqLit &lits, const TSeqWght &weights, const wght RHS, const substitution& witness, bool overrule_keeporiginalformula){
    assert(!keep_original_formula || overrule_keeporiginalformula);
    *proof << "del spec";
    write_PB_constraint(lits, weights, RHS);
    *proof << "; ";
    write_substitution(witness);
    *proof << "\n";
}

// Removal by del find where a literal occuring multiple times in lits is only written once.
// Note: TSeqLit must be sorted.
template <class TSeqLit>
void VeriPbProofLogger::delete_clause(const TSeqLit& lits, bool overrule_keeporiginalformula){
    assert(!keep_original_formula || overrule_keeporiginalformula);
    *proof << "del spec";
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

void VeriPbProofLogger::move_to_coreset(constraintid cxn, bool overrule_keeporiginalformula){
    if(!keep_original_formula || overrule_keeporiginalformula)
        *proof << "core id " << std::to_string(cxn) << "\n";
}
template <class TSeqLit>
void VeriPbProofLogger::move_to_coreset(TSeqLit& lits, wght RHS, bool overrule_keeporiginalformula){
    if(!keep_original_formula || overrule_keeporiginalformula)
        *proof << "core find"; write_cardinality_constraint(lits, RHS); *proof << "\n";
}
template <class TSeqLit, class TSeqWght>
void VeriPbProofLogger::move_to_coreset(TSeqLit& lits, TSeqWght& wghts, wght RHS, bool overrule_keeporiginalformula){
    if(!keep_original_formula || overrule_keeporiginalformula)
        *proof << "core find"; write_PB_constraint(lits, wghts, RHS); *proof << "\n";
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

void VeriPbProofLogger::rup_lower_bound_constraint(){
    rup(objective_lits,objective_weights, best_objective_value);
}
//=================================================================================================