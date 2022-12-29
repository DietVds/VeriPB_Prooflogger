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

template<class TLit>
void VeriPbProofLogger::set_objective(const std::vector<TLit> &lits, const std::vector<int> &weights, int constant_cost)
{
    objective_lits.reserve(lits.size());

    for(int i = 0; i < lits.size(); i++){
        objective_lits[i] = toVeriPbLit(lits[i]);
    }

    objective_weights = weights;
    objective_constant_cost = constant_cost;
}
template void VeriPbProofLogger::set_objective<VeriPB::Lit>(const std::vector<VeriPB::Lit> &lits, const std::vector<int> &weights, int constant_cost);
template void VeriPbProofLogger::set_objective<Glucose::Lit>(const std::vector<Glucose::Lit> &lits, const std::vector<int> &weights, int constant_cost);


template <class TLit> 
void VeriPbProofLogger::add_objective_literal(TLit lit, int weight){
    objective_lits.push_back(toVeriPbLit(lit));
    objective_weights.push_back(weight);
}
template void VeriPbProofLogger::add_objective_literal<VeriPB::Lit>(VeriPB::Lit lit, int weight);
template void VeriPbProofLogger::add_objective_literal<Glucose::Lit>(Glucose::Lit lit, int weight);

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
template bool VeriPbProofLogger::is_aux_var<VeriPB::Var>(const VeriPB::Var &var);
template bool VeriPbProofLogger::is_aux_var<Glucose::Var>(const Glucose::Var &var);

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
template std::string VeriPbProofLogger::var_name<VeriPB::Var>(const VeriPB::Var &var);
template std::string VeriPbProofLogger::var_name<Glucose::Var>(const Glucose::Var &var);

template <class TLit>
void VeriPbProofLogger::write_weighted_literal(const TLit &literal, int weight)
{
    proof << std::to_string(weight) + " ";
    write_literal<TLit>(literal);
}
template void VeriPbProofLogger::write_weighted_literal<VeriPB::Lit>(const VeriPB::Lit &literal, int weight);
template void VeriPbProofLogger::write_weighted_literal<Glucose::Lit>(const Glucose::Lit &literal, int weight);

template <class TLit>
std::string VeriPbProofLogger::to_string(const TLit &lit)
{
    return (is_negated(lit) ? "~" : "") + var_name(variable(lit));
}
template std::string VeriPbProofLogger::to_string<VeriPB::Lit>(const VeriPB::Lit &lit);
template std::string VeriPbProofLogger::to_string<Glucose::Lit>(const Glucose::Lit &lit);


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
template void VeriPbProofLogger::write_literal<VeriPB::Lit>(const VeriPB::Lit &lit);
template void VeriPbProofLogger::write_literal<Glucose::Lit>(const Glucose::Lit &lit);

template <class TLit>
void VeriPbProofLogger::write_cardinality_constraint(const std::vector<TLit> &lits, const int RHS)
{
    for (int i = 0; i < lits.size(); i++)
        write_weighted_literal(lits[i]);
    proof << ">= " << RHS;
}
template void VeriPbProofLogger::write_cardinality_constraint<VeriPB::Lit>(const std::vector<VeriPB::Lit> &lits, const int RHS);
template void VeriPbProofLogger::write_cardinality_constraint<Glucose::Lit>(const std::vector<Glucose::Lit> &lits, const int RHS);

template <class TLit>
void VeriPbProofLogger::write_clause(const std::vector<TLit> &clause)
{
    write_cardinality_constraint(clause, 1);
}
template void VeriPbProofLogger::write_clause<VeriPB::Lit>(const std::vector<VeriPB::Lit> &clause);
template void VeriPbProofLogger::write_clause<Glucose::Lit>(const std::vector<Glucose::Lit> &clause);

template <class TLit>
void VeriPbProofLogger::write_PB_constraint(const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS)
{
    for (int i = 0; i < lits.size(); i++)
        write_weighted_literal(lits[i], weights[i]);
    proof << ">= " << RHS;
}
template void VeriPbProofLogger::write_PB_constraint<VeriPB::Lit>(const std::vector<VeriPB::Lit> &lits, const std::vector<int> &weights, const int RHS);
template void VeriPbProofLogger::write_PB_constraint<Glucose::Lit>(const std::vector<Glucose::Lit> &lits, const std::vector<int> &weights, const int RHS);

// ------------- Rewrite variables by literals -------------
template <class TVar, class TLit>
void VeriPbProofLogger::rewrite_variable_by_literal(const TVar& var, const TLit& lit)
{
    map_rewrite_var_by_literal[varidx(toVeriPbVar(var))] = toVeriPbLit(lit);
}
template void VeriPbProofLogger::rewrite_variable_by_literal<VeriPB::Var, VeriPB::Lit>(const VeriPB::Var& var, const VeriPB::Lit& lit);
template void VeriPbProofLogger::rewrite_variable_by_literal<Glucose::Var, Glucose::Lit>(const Glucose::Var& var, const Glucose::Lit& lit);

// ------------- Meaningful names -------------

template <class TVar>
void VeriPbProofLogger::store_meaningful_name(const TVar &var, const std::string &name)
{
    meaningful_names_store[varidx(toVeriPbVar(var))] = name;
}
template void VeriPbProofLogger::store_meaningful_name<VeriPB::Var>(const VeriPB::Var &var, const std::string &name);
template void VeriPbProofLogger::store_meaningful_name<Glucose::Var>(const Glucose::Var &var, const std::string &name);

template <class TVar>
void VeriPbProofLogger::delete_meaningful_name(const TVar &var)
{
    meaningful_names_store.erase(varidx(toVeriPbVar(var)));
}
template void VeriPbProofLogger::delete_meaningful_name<VeriPB::Var>(const VeriPB::Var &var);
template void VeriPbProofLogger::delete_meaningful_name<Glucose::Var>(const Glucose::Var &var);

// ------------- Rules for checking constraints -------------

template <class TLit>
void VeriPbProofLogger::equals_rule(const constraintid constraint_id, const std::vector<TLit> &lits, const int RHS)
{
    proof << "e " << constraint_id << " ";
    write_cardinality_constraint(lits, RHS);
    proof << ";\n";
}
template void VeriPbProofLogger::equals_rule<VeriPB::Lit>(const constraintid constraint_id, const std::vector<VeriPB::Lit> &lits, const int RHS);
template void VeriPbProofLogger::equals_rule<Glucose::Lit>(const constraintid constraint_id, const std::vector<Glucose::Lit> &lits, const int RHS);

template <class TLit>
void VeriPbProofLogger::equals_rule(const constraintid constraint_id, const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS)
{
    proof << "e " << constraint_id << " ";
    write_PB_constraint(lits, weights, RHS);
    proof << ";\n";
}
template void VeriPbProofLogger::equals_rule<VeriPB::Lit>(const constraintid constraint_id, const std::vector<VeriPB::Lit> &lits, const std::vector<int> &weights, const int RHS);
template void VeriPbProofLogger::equals_rule<Glucose::Lit>(const constraintid constraint_id, const std::vector<Glucose::Lit> &lits, const std::vector<int> &weights, const int RHS);

template <class TLit>
void VeriPbProofLogger::check_last_constraint(const std::vector<TLit> &lits, const int RHS)
{
    equals_rule(constraint_counter, lits, RHS);
}
template void VeriPbProofLogger::check_last_constraint<VeriPB::Lit>(const std::vector<VeriPB::Lit> &lits, const int RHS);
template void VeriPbProofLogger::check_last_constraint<Glucose::Lit>(const std::vector<Glucose::Lit> &lits, const int RHS);

template <class TLit>
void VeriPbProofLogger::check_last_constraint(const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS)
{
    equals_rule(constraint_counter, lits, weights, RHS);
}
template void VeriPbProofLogger::check_last_constraint<VeriPB::Lit>(const std::vector<VeriPB::Lit> &lits, const std::vector<int> &weights, const int RHS);
template void VeriPbProofLogger::check_last_constraint<Glucose::Lit>(const std::vector<Glucose::Lit> &lits, const std::vector<int> &weights, const int RHS);

// ------------- Rules for optimisation -------------

/// @brief Calculate the objective value of the objective stored in VeriPBProoflogger for a model. This function uses the optimistic assumption that the literals are sorted.
/// @param model Vector of assignments to literals.
/// @return Objective value for model.
template <class TLit>
int VeriPbProofLogger::calculate_objective_value(const std::vector<TLit> &model)
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
template int VeriPbProofLogger::calculate_objective_value<VeriPB::Lit>(const std::vector<VeriPB::Lit> &model);

template <class TLit>
constraintid VeriPbProofLogger::log_solution(const std::vector<TLit> &model)
{
    proof << "o ";
    for (int i = 0; i < model.size(); i++)
        write_literal(model[i]);
    proof << "\n";
    // Veripb automatically adds an improvement constraint so counter needs to be incremented
    best_solution_constraint = ++constraint_counter;
    return constraint_counter;
}
template constraintid VeriPbProofLogger::log_solution<VeriPB::Lit>(const std::vector<VeriPB::Lit> &model);
template constraintid VeriPbProofLogger::log_solution<Glucose::Lit>(const std::vector<Glucose::Lit> &model);

template <class TLit>
constraintid VeriPbProofLogger::log_solution_with_check(const std::vector<TLit> &model)
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
template constraintid VeriPbProofLogger::log_solution_with_check<VeriPB::Lit>(const std::vector<VeriPB::Lit> &model);

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

// ------------- Unchecked Assumptions -------------
template <class TLit>
constraintid VeriPbProofLogger::unchecked_assumption(const std::vector<TLit> &lits, const int RHS)
{
    proof << "a ";
    write_cardinality_constraint(lits, RHS);
    proof << " ;\n";
    return ++constraint_counter;
}
template constraintid VeriPbProofLogger::unchecked_assumption<VeriPB::Lit>(const std::vector<VeriPB::Lit> &lits, const int RHS);
template constraintid VeriPbProofLogger::unchecked_assumption<Glucose::Lit>(const std::vector<Glucose::Lit> &lits, const int RHS);

template <class TLit>
constraintid VeriPbProofLogger::unchecked_assumption(const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS)
{
    proof << "u ";
    write_PB_constraint(lits, weights, RHS);
    proof << " ;\n";
    return ++constraint_counter;
}
template constraintid VeriPbProofLogger::unchecked_assumption<VeriPB::Lit>(const std::vector<VeriPB::Lit> &lits, const std::vector<int> &weights, const int RHS);
template constraintid VeriPbProofLogger::unchecked_assumption<Glucose::Lit>(const std::vector<Glucose::Lit> &lits, const std::vector<int> &weights, const int RHS);

// ------------- Reverse Unit Propagation -------------
template <class TLit>
constraintid VeriPbProofLogger::rup(const std::vector<TLit> &lits, const int RHS)
{
    proof << "u ";
    write_cardinality_constraint(lits, RHS);
    proof << ";\n";
    return ++constraint_counter;
}
template constraintid VeriPbProofLogger::rup<VeriPB::Lit>(const std::vector<VeriPB::Lit> &lits, const int RHS);
template constraintid VeriPbProofLogger::rup<Glucose::Lit>(const std::vector<Glucose::Lit> &lits, const int RHS);

template <class TLit>
constraintid VeriPbProofLogger::rup(const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS)
{
    proof << "u ";
    write_PB_constraint(lits, weights, RHS);
    proof << " ;\n";
    return ++constraint_counter;
}
template constraintid VeriPbProofLogger::rup<VeriPB::Lit>(const std::vector<VeriPB::Lit> &lits, const std::vector<int> &weights, const int RHS);
template constraintid VeriPbProofLogger::rup<Glucose::Lit>(const std::vector<Glucose::Lit> &lits, const std::vector<int> &weights, const int RHS);

//  ------------- Dominance Rule -------------

// TODO

// ------------- Redundance Based Strenghtening -------------

// TODO: add witness that could also map to other variables.

template <class TLit>
void VeriPbProofLogger::write_witness(const TLit &literal)
{
    // convert to VeriPB Datastructures and check if the literal should be negated. 
    VeriPB::Lit lit = toVeriPbLit(literal);
    VeriPB::Var var = variable(lit);
    VeriPB::VarIdx idx_var = varidx(var);

    if(map_rewrite_var_by_literal.find(idx_var) != map_rewrite_var_by_literal.end()){
        if(is_negated(lit))
            lit = neg(map_rewrite_var_by_literal[idx_var]);
        else
            lit = map_rewrite_var_by_literal[idx_var];
    }

    proof << var_name(var) << " -> " << (is_negated(lit) ? "0" : "1");
    
}
template void VeriPbProofLogger::write_witness<VeriPB::Lit>(const VeriPB::Lit &literal);
template void VeriPbProofLogger::write_witness<Glucose::Lit>(const Glucose::Lit &literal);

template <class TLit>
void VeriPbProofLogger::write_witness(const std::vector<TLit> &witness)
{
    for (int i = 0; i < witness.size(); i++)
    {
        write_witness(witness[i]);
    }
}
template void VeriPbProofLogger::write_witness<VeriPB::Lit>(const std::vector<VeriPB::Lit> &witness);
template void VeriPbProofLogger::write_witness<Glucose::Lit>(const std::vector<Glucose::Lit> &witness);

template <class TLit>
constraintid VeriPbProofLogger::redundanceBasedStrengthening(const std::vector<TLit> &lits, const int RHS, const TLit &witness)
{
    proof << "red ";
    write_cardinality_constraint(lits, RHS);
    proof << "; ";
    write_witness(witness);
    proof << "\n";
    return ++constraint_counter;
}
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<VeriPB::Lit>(const std::vector<VeriPB::Lit> &lits, const int RHS, const VeriPB::Lit &witness);
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<Glucose::Lit>(const std::vector<Glucose::Lit> &lits, const int RHS, const Glucose::Lit &witness);

template <class TLit>
constraintid VeriPbProofLogger::redundanceBasedStrengthening(const std::vector<TLit> &lits, const int RHS, const std::vector<TLit> &witness)
{
    proof << "red ";
    write_cardinality_constraint(lits, RHS);
    proof << "; ";
    write_witness(witness);
    proof << "\n";
    return ++constraint_counter;
}
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<VeriPB::Lit>(const std::vector<VeriPB::Lit> &lits, const int RHS, const std::vector<VeriPB::Lit> &witness);
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<Glucose::Lit>(const std::vector<Glucose::Lit> &lits, const int RHS, const std::vector<Glucose::Lit> &witness);

template <class TLit>
constraintid VeriPbProofLogger::redundanceBasedStrengthening(const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS, const TLit &witness)
{
    proof << "red ";
    write_PB_constraint(lits, weights, RHS);
    proof << "; ";
    write_witness(witness);
    proof << "\n";
    return ++constraint_counter;
}
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<VeriPB::Lit>(const std::vector<VeriPB::Lit> &lits, const std::vector<int> &weights, const int RHS, const VeriPB::Lit &witness);
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<Glucose::Lit>(const std::vector<Glucose::Lit> &lits, const std::vector<int> &weights, const int RHS, const Glucose::Lit &witness);

template <class TLit>
constraintid VeriPbProofLogger::redundanceBasedStrengthening(const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS, const std::vector<TLit> &witness)
{
    proof << "red ";
    write_PB_constraint(lits, weights, RHS);
    proof << "; ";
    write_witness(witness);
    proof << "\n";
    return ++constraint_counter;
}
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<VeriPB::Lit>(const std::vector<VeriPB::Lit> &lits, const std::vector<int> &weights, const int RHS, const std::vector<VeriPB::Lit> &witness);
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<Glucose::Lit>(const std::vector<Glucose::Lit> &lits, const std::vector<int> &weights, const int RHS, const std::vector<Glucose::Lit> &witness);

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
template void VeriPbProofLogger::start_CP_derivation_with_lit_axiom<VeriPB::Lit>(const VeriPB::Lit &lit);
template void VeriPbProofLogger::start_CP_derivation_with_lit_axiom<Glucose::Lit>(const Glucose::Lit &lit);

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
    pol_string << " " << var_name(variable(var)) << " w";
}
template void VeriPbProofLogger::CP_weakening<Glucose::Lit>(const Glucose::Lit &var);
template void VeriPbProofLogger::CP_weakening<VeriPB::Lit>(const VeriPB::Lit &var);

template <class TLit>
void VeriPbProofLogger::CP_literal_axiom(const TLit &lit)
{
    pol_string << " " << to_string(lit);
}
template void VeriPbProofLogger::CP_literal_axiom<VeriPB::Lit>(const VeriPB::Lit &lit);
template void VeriPbProofLogger::CP_literal_axiom<Glucose::Lit>(const Glucose::Lit &lit);

constraintid VeriPbProofLogger::end_CP_derivation()
{
    proof << pol_string.rdbuf() << "\n";
    return ++constraint_counter;
}

//  ------------- Deleting & Overwriting Constraints -------------

// template <class TLit>
void VeriPbProofLogger::delete_constraint(const constraintid constraint_id)
{
    proof << "del id " << constraint_id << "\n";
}
// template void VeriPbProofLogger::delete_constraint<VeriPB::Lit>(const constraintid constraint_id);
// template void VeriPbProofLogger::delete_constraint<Glucose::Lit>(const constraintid constraint_id);

//template <class TLit>
void VeriPbProofLogger::delete_constraint(const std::vector<constraintid> &constraint_ids)
{
    proof << "del id";
    for (int i = 0; i < constraint_ids.size(); i++)
    {
        proof << " " << constraint_ids[i];
    }
    proof << "\n";
}
// template void VeriPbProofLogger::delete_constraint<int>(const std::vector<constraintid> &constraint_ids);
// template void VeriPbProofLogger::delete_constraint<Glucose::Lit>(const std::vector<constraintid> &constraint_ids);

template <class TLit>
void VeriPbProofLogger::delete_constraint(const std::vector<TLit> &lits, const int RHS)
{
    proof << "del find ";
    write_cardinality_constraint(lits, RHS);
    proof << ";\n";
}
template void VeriPbProofLogger::delete_constraint<VeriPB::Lit>(const std::vector<VeriPB::Lit> &lits, const int RHS);
template void VeriPbProofLogger::delete_constraint<Glucose::Lit>(const std::vector<Glucose::Lit> &lits, const int RHS);

template <class TLit>
void VeriPbProofLogger::delete_constraint(const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS)
{
    proof << "del find ";
    write_PB_constraint(lits, weights, RHS);
    proof << ";\n";
}
template void VeriPbProofLogger::delete_constraint<VeriPB::Lit>(const std::vector<VeriPB::Lit> &lits, const std::vector<int> &weights, const int RHS);
template void VeriPbProofLogger::delete_constraint<Glucose::Lit>(const std::vector<Glucose::Lit> &lits, const std::vector<int> &weights, const int RHS);

template <class TLit>
constraintid VeriPbProofLogger::overwrite_constraint(const constraintid constraint_id, const std::vector<TLit> &lits, const int RHS)
{
    constraintid newconstraint = rup(lits, RHS);
    delete_constraint(constraint_id);
    return newconstraint;
}
template constraintid VeriPbProofLogger::overwrite_constraint<VeriPB::Lit>(const constraintid constraint_id, const std::vector<VeriPB::Lit> &lits, const int RHS);
template constraintid VeriPbProofLogger::overwrite_constraint<Glucose::Lit>(const constraintid constraint_id, const std::vector<Glucose::Lit> &lits, const int RHS);

template <class TLit>
constraintid VeriPbProofLogger::overwrite_constraint(const std::vector<TLit> &lits_orig, const int RHS_orig, const std::vector<TLit> &lits, const int RHS)
{
    constraintid newconstraint = rup(lits, RHS);
    delete_constraint(lits_orig, RHS_orig);
    return newconstraint;
}
template constraintid VeriPbProofLogger::overwrite_constraint<VeriPB::Lit>(const std::vector<VeriPB::Lit> &lits_orig, const int RHS_orig, const std::vector<VeriPB::Lit> &lits, const int RHS);
template constraintid VeriPbProofLogger::overwrite_constraint<Glucose::Lit>(const std::vector<Glucose::Lit> &lits_orig, const int RHS_orig, const std::vector<Glucose::Lit> &lits, const int RHS);

template <class TLit>
constraintid VeriPbProofLogger::overwrite_constraint(const constraintid constraint_id, const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS)
{
    constraintid newconstraint = rup(lits, weights, RHS);
    delete_constraint(constraint_id);
    return newconstraint;
}
template constraintid VeriPbProofLogger::overwrite_constraint<VeriPB::Lit>(const constraintid constraint_id, const std::vector<VeriPB::Lit> &lits, const std::vector<int> &weights, const int RHS);
template constraintid VeriPbProofLogger::overwrite_constraint<Glucose::Lit>(const constraintid constraint_id, const std::vector<Glucose::Lit> &lits, const std::vector<int> &weights, const int RHS);

template <class TLit>
constraintid VeriPbProofLogger::overwrite_constraint(const std::vector<TLit> &lits_orig, const std::vector<int> &weights_orig, const int RHS_orig, const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS)
{
    constraintid newconstraint = rup(lits, weights, RHS);
    delete_constraint(lits_orig, weights_orig, RHS_orig);
    return newconstraint;
}
template constraintid VeriPbProofLogger::overwrite_constraint<VeriPB::Lit>(const std::vector<VeriPB::Lit> &lits_orig, const std::vector<int> &weights_orig, const int RHS_orig, const std::vector<VeriPB::Lit> &lits, const std::vector<int> &weights, const int RHS);
template constraintid VeriPbProofLogger::overwrite_constraint<Glucose::Lit>(const std::vector<Glucose::Lit> &lits_orig, const std::vector<int> &weights_orig, const int RHS_orig, const std::vector<Glucose::Lit> &lits, const std::vector<int> &weights, const int RHS);

template <class TLit>
constraintid VeriPbProofLogger::overwrite_constraint(const std::vector<TLit> &lits_orig, const std::vector<TLit> &lits)
{
    constraintid newconstraint = rup(lits);
    delete_constraint(lits_orig, 1);
    return newconstraint;
}
template constraintid VeriPbProofLogger::overwrite_constraint<VeriPB::Lit>(const std::vector<VeriPB::Lit> &lits_orig, const std::vector<VeriPB::Lit> &lits);
template constraintid VeriPbProofLogger::overwrite_constraint<Glucose::Lit>(const std::vector<Glucose::Lit> &lits_orig, const std::vector<Glucose::Lit> &lits);

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

/****************************************
 * For MINISAT/GLUCOSE compatibility
 ****************************************/

// template <class TClause>
void VeriPbProofLogger::write_clause(Glucose::Clause &clause)
{
    for (int i = 0; i < clause.size(); i++)
        write_weighted_literal(clause[i]);
    proof << " >= 1";
}
// template void VeriPbProofLogger::write_clause<Glucose::Clause>(Glucose::Clause &clause);

template <template <class T> class TVec, class TLit>
void VeriPbProofLogger::write_clause(TVec<TLit> &clause)
{
    for (int i = 0; i < clause.size(); i++)
        write_weighted_literal(clause[i]);
    proof << " >= 1";
}
template void VeriPbProofLogger::write_clause<Glucose::vec, Glucose::Lit>(Glucose::vec<Glucose::Lit> &clause);

template <class TLBool>
void VeriPbProofLogger::write_literal_assignment(TLBool assignment, int var)
{
    proof << (toInt(assignment) == 0 ? "" : "~") << var_name(var) << " "; // l_true and g3l_true is defined as 0.
}
template void VeriPbProofLogger::write_literal_assignment<Glucose::lbool>(Glucose::lbool assignment, int var);

template <template <class T> class TVec, class TLBool>
constraintid VeriPbProofLogger::log_solution(TVec<TLBool> &model)
{
    proof << "o ";
    for (int i = 0; i < model.size(); i++)
        write_literal_assignment(model[i], i);
    proof << "\n";

    // Veripb automatically adds an improvement constraint so counter needs to be incremented
    best_solution_constraint = ++constraint_counter;
    return constraint_counter;
}
template constraintid VeriPbProofLogger::log_solution<Glucose::vec, Glucose::lbool>(Glucose::vec<Glucose::lbool> &model);

constraintid VeriPbProofLogger::unchecked_assumption(const Glucose::vec<Glucose::Lit> &lits, const int RHS)
{
    proof << "a ";
    for (int i = 0; i < lits.size(); i++)
        write_weighted_literal(lits[i]);
    proof << ">= 1;\n";
    return ++constraint_counter;
}

template <template <class T> class TVec, class TLit>
constraintid VeriPbProofLogger::rup(TVec<TLit> &clause)
{
    proof << "u ";
    write_clause(clause);
    proof << ";\n";
    return ++constraint_counter;
}
template constraintid VeriPbProofLogger::rup<Glucose::vec, Glucose::Lit>(Glucose::vec<Glucose::Lit> &clause);

template <template <class T> class TVec, class TLit>
void VeriPbProofLogger::overwrite_constraint(TVec<TLit> &orig, TVec<TLit> &clause)
{
    rup(clause);
    delete_constraint(orig);
}
template void VeriPbProofLogger::overwrite_constraint<Glucose::vec, Glucose::Lit>(Glucose::vec<Glucose::Lit> &orig, Glucose::vec<Glucose::Lit> &clause);

// template <class TClause>
void VeriPbProofLogger::delete_constraint(Glucose::Clause &clause)
{
    proof << "del find ";
    write_clause(clause);
    proof << ";\n";
}
// template void VeriPbProofLogger::delete_constraint<Glucose::Clause>(Glucose::Clause &clause);

template <template <class T> class TVec, class TLit>
void VeriPbProofLogger::delete_constraint(TVec<TLit> &clause)
{
    proof << "del find ";
    write_clause(clause);
    proof << ";\n";
}
template void VeriPbProofLogger::delete_constraint<Glucose::vec, Glucose::Lit>(Glucose::vec<Glucose::Lit> &clause);

//=================================================================================================