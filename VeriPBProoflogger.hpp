#include "VeriPBProoflogger.h"

//=================================================================================================

//prooflogging Library


// ------------- Proof file -------------

void VeriPbProofLogger::start_proof(const std::string filename, int nbclause, int nbvars)
{
    init_proof_file(filename);
    write_proof_header(nbclause, nbvars);
}

void VeriPbProofLogger::start_proof(const std::string filename, int nbvars)
{
    init_proof_file(filename);
    write_proof_header(nbvars);
}

void VeriPbProofLogger::init_proof_file(const std::string filename)
{
    proof_file = filename;
    proof.open(filename);
}

void VeriPbProofLogger::write_proof_header(int nbclause, int nbvars)
{
    constraint_counter = (constraintid)nbclause;
    n_variables = nbvars;

    proof << "pseudo-Boolean proof version 1.0\n";
    proof << "f " << nbclause << "\n";
}

void VeriPbProofLogger::write_proof_header(int nbvars)
{
    n_variables = nbvars;

    proof << "pseudo-Boolean proof version 1.0\n";
    proof << "f\n";
}

void VeriPbProofLogger::end_proof()
{
    proof.close();
}

template<class TSeqLit, class TSeqInt>
void VeriPbProofLogger::set_objective(const TSeqLit &lits, const TSeqInt &weights, int constant_cost)
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
void VeriPbProofLogger::add_objective_literal(TLit lit, int weight){
    objective_lits.push_back(toVeriPbLit(lit));
    objective_weights.push_back(weight);
}

void VeriPbProofLogger::add_objective_constant(int weight){
    objective_constant_cost += weight;
}

void VeriPbProofLogger::write_comment_objective_function()
{
    proof << "* f = ";
    for (int i = 0; i < objective_lits.size(); i++)
        write_weighted_literal(objective_lits[i], objective_weights[i]);
    if(objective_constant_cost != 0)
        proof << " + " << std::to_string(objective_constant_cost);
    proof << "\n";
}

void VeriPbProofLogger::increase_n_variables(){
    n_variables++;
}
// ------------- Helping functions -------------

void VeriPbProofLogger::write_comment(const char *comment)
{
    proof << "* " << comment << "\n";
}

void VeriPbProofLogger::write_comment(const std::string &comment)
{
    proof << "* " << comment << "\n";
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
void VeriPbProofLogger::write_weighted_literal(const TLit &literal, int weight)
{
    proof << std::to_string(weight) + " ";
    write_literal<TLit>(literal);
}

template <class TLit>
std::string VeriPbProofLogger::to_string(const TLit &lit)
{
    return (is_negated(lit) ? "~" : "") + var_name(variable(lit));
}


template <class TLit>
void VeriPbProofLogger::write_literal(const TLit &lit)
{
    VeriPB::Lit l = toVeriPbLit(lit);
    VeriPB::Var litvar = variable(l);

    if(map_rewrite_var_by_literal.find(varidx(litvar)) != map_rewrite_var_by_literal.end()){ // the variable needs to be rewritten
        if(is_negated(l))
            proof << to_string(neg(map_rewrite_var_by_literal[varidx(litvar)])) << " ";
        else 
            proof << to_string(map_rewrite_var_by_literal[varidx(litvar)]) << " ";
    }
    else{
        proof << to_string(l) << " ";
    }
}

template <class TSeqLit>
void VeriPbProofLogger::write_cardinality_constraint(const TSeqLit &lits, const int RHS)
{
    for (int i = 0; i < lits.size(); i++)
        write_weighted_literal(lits[i]);
    proof << ">= " << RHS;
}

template <class TSeqLit>
void VeriPbProofLogger::write_clause(const TSeqLit& clause){
    write_cardinality_constraint(clause, 1);
}

template <class TSeqLit, class TSeqInt>
void VeriPbProofLogger::write_PB_constraint(const TSeqLit &lits, const TSeqInt &weights, const int RHS)
{
    for (int i = 0; i < lits.size(); i++)
        write_weighted_literal(lits[i], weights[i]);
    proof << ">= " << RHS;
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
void VeriPbProofLogger::equals_rule(const constraintid constraint_id, const TSeqLit &lits, const int RHS)
{
    proof << "e " << constraint_id << " ";
    write_cardinality_constraint(lits, RHS);
    proof << ";\n";
}

template <class TSeqLit, class TSeqInt>
void VeriPbProofLogger::equals_rule(const constraintid constraint_id, const TSeqLit &lits, const TSeqInt &weights, const int RHS)
{
    proof << "e " << constraint_id << " ";
    write_PB_constraint(lits, weights, RHS);
    proof << ";\n";
}

template <class TSeqLit>
void VeriPbProofLogger::check_last_constraint(const TSeqLit &lits, const int RHS)
{
    equals_rule(constraint_counter, lits, RHS);
}

template <class TSeqLit, class TSeqInt>
void VeriPbProofLogger::check_last_constraint(const TSeqLit &lits, const TSeqInt &weights, const int RHS)
{
    equals_rule(constraint_counter, lits, weights, RHS);
}

// ------------- Rules for optimisation -------------

/// @brief Calculate the objective value of the objective stored in VeriPBProoflogger for a model. This function uses the optimistic assumption that the literals are sorted.
/// @param model Vector of assignments to literals.
/// @return Objective value for model.
template <class TSeqLit>
int VeriPbProofLogger::calculate_objective_value(const TSeqLit &model)
{
    int objective_value = objective_constant_cost;
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
                return INT_MAX;
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
constraintid VeriPbProofLogger::log_solution(const TSeqLit &model)
{
    proof << "o ";
    for (int i = 0; i < model.size(); i++)
        write_literal(model[i]);
    proof << "\n";
    // Veripb automatically adds an improvement constraint so counter needs to be incremented
    best_solution_constraint = ++constraint_counter;
    return constraint_counter;
}

template <class TSeqLit>
constraintid VeriPbProofLogger::log_solution_with_check(const TSeqLit &model)
{
    int current_objective_value = calculate_objective_value(model);
    if (current_objective_value < best_objective_value)
    {
        write_comment("Objective update from " + std::to_string(best_objective_value) + " to " + std::to_string(current_objective_value));
        best_solution_constraint = log_solution(model);
        best_objective_value = current_objective_value;
    }

    return best_solution_constraint;
}

constraintid VeriPbProofLogger::get_best_solution_constraint()
{
    return best_solution_constraint;
}

constraintid VeriPbProofLogger::get_rewritten_best_solution_constraint(){
    return rewritten_best_solution_constraint;
}
void VeriPbProofLogger::set_rewritten_best_solution_constraint(constraintid cxn){
    rewritten_best_solution_constraint = cxn;
}
void VeriPbProofLogger::delete_rewritten_best_solution_constraint(){
    //delete_constraint<int>(rewritten_best_solution_constraint);
    delete_constraint(rewritten_best_solution_constraint);

    rewritten_best_solution_constraint = 0;
}

template <class TSeqLBool>
constraintid VeriPbProofLogger::log_solution(TSeqLBool &model)
{
    VeriPB::Var var;
    VeriPB::Lit lit;

    proof << "o ";
    for (int i = 0; i < model.size(); i++){
        var = toVeriPbVar(i);
        lit = create_literal(var, !toBool(model[i]));

        write_literal(lit);
    }
    proof << "\n";

    // Veripb automatically adds an improvement constraint so counter needs to be incremented
    best_solution_constraint = ++constraint_counter;
    return constraint_counter;
}


// ------------- Unchecked Assumptions -------------
template <class TSeqLit>
constraintid VeriPbProofLogger::unchecked_assumption(const TSeqLit &lits, const int RHS)
{
    proof << "a ";
    write_cardinality_constraint(lits, RHS);
    proof << " ;\n";
    return ++constraint_counter;
}

template <class TSeqLit, class TSeqInt>
constraintid VeriPbProofLogger::unchecked_assumption(const TSeqLit &lits, const TSeqInt &weights, const int RHS)
{
    proof << "u ";
    write_PB_constraint(lits, weights, RHS);
    proof << " ;\n";
    return ++constraint_counter;
}

// ------------- Reverse Unit Propagation -------------
template <class TSeqLit>
constraintid VeriPbProofLogger::rup(const TSeqLit &lits, const int RHS)
{
    proof << "u ";
    write_cardinality_constraint(lits, RHS);
    proof << ";\n";
    return ++constraint_counter;
}

template <class TSeqLit, class TSeqInt>
constraintid VeriPbProofLogger::rup(const TSeqLit &lits, const TSeqInt &weights, const int RHS)
{
    proof << "u ";
    write_PB_constraint(lits, weights, RHS);
    proof << " ;\n";
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

        proof << var_name(var) << " -> " << std::to_string(to) ;
    }    
}


template <class TSeqLit, class TVar>
constraintid VeriPbProofLogger::redundanceBasedStrengthening(const TSeqLit &lits, const int RHS, const substitution<TVar> &witness)
{
    proof << "red ";
    write_cardinality_constraint(lits, RHS);
    proof << "; ";
    write_witness(witness);
    proof << "\n";
    return ++constraint_counter;
}

template <class TSeqLit, class TSeqInt, class TVar>
constraintid VeriPbProofLogger::redundanceBasedStrengthening(const TSeqLit &lits, const TSeqInt &weights, const int RHS, const substitution<TVar> &witness)
{
    proof << "red ";
    write_PB_constraint(lits, weights, RHS);
    proof << "; ";
    write_witness(witness);
    proof << "\n";
    return ++constraint_counter;
}

// ------------- Reification Literals -------------
// Proves the constraints encoding the reification constraint l <-> C, with l a literal and C a boolean constraint.
// The right implication is the encoding of l -> C, whereas the left implication means l <- C.
template <class TSeqLit, class TSeqInt, class TLit>
constraintid VeriPbProofLogger::reificationLiteralRightImpl(const TLit& lit, const TSeqLit &lits, const TSeqInt &weights, const int RHS, bool store_reified_constraint){
    int i;

    std::string comment = to_string(lit) + " -> " ;
    for(i = 0; i < lits.size(); i++)
        comment += std::to_string(weights[i]) + " " + to_string(lits[i]) + " ";
    comment += ">= " + std::to_string(RHS);
    write_comment(comment);
    
    std::vector<VeriPB::Lit> _lits; _lits.resize(lits.size()+1);
    std::vector<int> _weights;  _weights.resize(weights.size()+1);

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
constraintid VeriPbProofLogger::reificationLiteralRightImpl(const TLit& lit, const TSeqLit &lits, const int RHS, bool store_reified_constraint){
    int i;

    std::string comment = to_string(lit) + " -> " ;
    for(i = 0; i < lits.size(); i++)
        comment +=  "1 " + to_string(lits[i]) + " ";
    comment += ">= " + std::to_string(RHS);
    write_comment(comment);
    
    std::vector<VeriPB::Lit> _lits; _lits.resize(lits.size()+1);
    std::vector<int> _weights;  _weights.resize(lits.size()+1);

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


template <class TSeqLit, class TSeqInt, class TLit>
constraintid VeriPbProofLogger::reificationLiteralLeftImpl(const TLit& lit, const TSeqLit &lits, const TSeqInt &weights, const int RHS, bool store_reified_constraint){
    int i;

    std::string comment = to_string(lit) + " <- " ;
    for(i = 0; i < lits.size(); i++)
        comment += std::to_string(weights[i]) + " " + to_string(lits[i]) + " ";
    comment += ">= " + std::to_string(RHS);
    write_comment(comment);

    std::vector<VeriPB::Lit> _lits; _lits.resize(lits.size()+1);
    std::vector<int> _weights;  _weights.resize(weights.size()+1);

    VeriPB::Lit _lit = toVeriPbLit(lit);

    int sum_of_weights = 0;

    for(i = 0; i < lits.size(); i++){
        sum_of_weights += weights[i];

        _weights[i] = weights[i];
        _lits[i] = toVeriPbLit(neg(lits[i]));
    }
    

    int j = sum_of_weights - RHS + 1 ;

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
constraintid VeriPbProofLogger::reificationLiteralLeftImpl(const TLit& lit, const TSeqLit &lits, const int RHS, bool store_reified_constraint){
    int i;
    
    std::string comment = to_string(lit) + " <- " ;
    for(i = 0; i < lits.size(); i++)
        comment += "1 " + to_string(lits[i]) + " ";
    comment += ">= " + std::to_string(RHS);
    write_comment(comment);
    
    std::vector<VeriPB::Lit> _lits; _lits.resize(lits.size()+1);
    std::vector<int> _weights;  _weights.resize(lits.size()+1);

    VeriPB::Lit _lit = toVeriPbLit(lit);

    for(i = 0; i < lits.size(); i++){
        _weights[i] = 1;
        _lits[i] = toVeriPbLit(neg(lits[i]));
    }
    
    int j = lits.size() - RHS + 1 ;

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

// ------------- Cutting Planes derivations -------------

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

void VeriPbProofLogger::CP_divide(const int v)
{
    pol_string << " " << v << " d";
}
void VeriPbProofLogger::CP_saturate()
{
    pol_string << " s";
}
void VeriPbProofLogger::CP_multiply(const int v)
{
    pol_string << " " << v << " *";
}

template <class TVar>
void VeriPbProofLogger::CP_weakening(const TVar &var)
{
    pol_string << " " << var_name(var) << " w";
}

template <class TLit>
void VeriPbProofLogger::CP_literal_axiom(const TLit &lit)
{
    pol_string << " " << to_string(lit);
}

constraintid VeriPbProofLogger::end_CP_derivation()
{
    proof << pol_string.rdbuf() << "\n";
    return ++constraint_counter;
}

//  ------------- Deleting & Overwriting Constraints -------------

void VeriPbProofLogger::delete_constraint(const constraintid constraint_id)
{
    proof << "del id " << constraint_id << "\n";
}

void VeriPbProofLogger::delete_constraint(const std::vector<constraintid> &constraint_ids)
{
    proof << "del id";
    for (int i = 0; i < constraint_ids.size(); i++)
    {
        proof << " " << constraint_ids[i];
    }
    proof << "\n";
}

template <class TSeqLit>
void VeriPbProofLogger::delete_constraint(const TSeqLit &lits, const int RHS)
{
    proof << "del find ";
    write_cardinality_constraint(lits, RHS);
    proof << ";\n";
}


template <class TSeqLit, class TSeqInt>
void VeriPbProofLogger::delete_constraint(const TSeqLit &lits, const TSeqInt &weights, const int RHS)
{
    proof << "del find ";
    write_PB_constraint(lits, weights, RHS);
    proof << ";\n";
}

template <class TSeqLit>
constraintid VeriPbProofLogger::overwrite_constraint(const constraintid constraint_id, const TSeqLit &lits, const int RHS)
{
    constraintid newconstraint = rup(lits, RHS);
    delete_constraint(constraint_id);
    return newconstraint;
}

template <class TSeqLit>
constraintid VeriPbProofLogger::overwrite_constraint(const TSeqLit &lits_orig, const int RHS_orig, const TSeqLit &lits, const int RHS)
{
    constraintid newconstraint = rup(lits, RHS);
    delete_constraint(lits_orig, RHS_orig);
    return newconstraint;
}

template <class TSeqLit, class TSeqInt>
constraintid VeriPbProofLogger::overwrite_constraint(const constraintid constraint_id, const TSeqLit &lits, const TSeqInt &weights, const int RHS)
{
    constraintid newconstraint = rup(lits, weights, RHS);
    delete_constraint(constraint_id);
    return newconstraint;
}

template <class TSeqLit, class TSeqInt>
constraintid VeriPbProofLogger::overwrite_constraint(const TSeqLit &lits_orig, const TSeqInt &weights_orig, const int RHS_orig, const TSeqLit &lits, const TSeqInt &weights, const int RHS)
{
    constraintid newconstraint = rup(lits, weights, RHS);
    delete_constraint(lits_orig, weights_orig, RHS_orig);
    return newconstraint;
}

template <class TSeqLit>
constraintid VeriPbProofLogger::overwrite_constraint(const TSeqLit&lits_orig, const TSeqLit &lits)
{
    constraintid newconstraint = rup(lits);
    delete_constraint(lits_orig, 1);
    return newconstraint;
}

// ------------- Handling contradiction -------------

void VeriPbProofLogger::write_previous_constraint_contradiction()
{
    proof << "c " << constraint_counter << "\n";
}

void VeriPbProofLogger::rup_empty_clause()
{
    proof << "u >= 1;\n";
    constraint_counter++;
}
//=================================================================================================