#include "VeriPBProoflogger.h"

//=================================================================================================

// ------------- Proof file -------------

void VeriPbProofLogger::start_proof(const std::string filename, int nbclause, int nbvars)
{
    init_proof_file(filename);
    write_proof_header(nbclause, nbvars);
}

// TODO: check usage of this method
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

// TODO: check where this one is used, since another one is used in the prooflogger in certified-cgss
void VeriPbProofLogger::write_proof_header(int nbclause)
{
    proof << "pseudo-Boolean proof version 1.0\n";
    proof << "f " << nbclause << "\n";
}

void VeriPbProofLogger::end_proof()
{
    proof.close();
}

void VeriPbProofLogger::set_objective(const std::vector<int> &lits, const std::vector<int> &weights)
{
    objective_lits = lits;
    objective_weights = weights;
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
    return (var > n_variables);
}
template bool VeriPbProofLogger::is_aux_var<VeriPB::Var>(const VeriPB::Var &var);

template <class TVar>
std::string VeriPbProofLogger::var_name(const TVar &var)
{
    std::string name;

    if (meaningful_names_store.find(var) != meaningful_names_store.end())
    {
        name = meaningful_names_store[var];
    }
    else if (is_aux_var(var))
    {
        name = "y";
        name += std::to_string(var);
    }
    else
    {
        name = "x";
        name += std::to_string(var);
    }
    return name;
}
template std::string VeriPbProofLogger::var_name<int>(const int &var);

template <class TLit>
void VeriPbProofLogger::write_weighted_literal(const TLit &literal, int weight)
{
    proof << std::to_string(weight) + " ";
    write_literal<TLit>(literal);
}
template void VeriPbProofLogger::write_weighted_literal<int>(const int &literal, int weight);
template void VeriPbProofLogger::write_weighted_literal<Glucose30::Lit>(const Glucose30::Lit &literal, int weight);

template <class TLit>
std::string VeriPbProofLogger::to_string(const TLit &lit)
{
    return (is_negated(lit) ? "~" : "") + var_name(variable(lit));
}
template std::string VeriPbProofLogger::to_string<VeriPB::Lit>(const VeriPB::Lit &lit);

template <class TLit>
void VeriPbProofLogger::write_literal(const TLit &lit)
{
    VeriPB::Lit l = toVeriPbLit(lit);
    if(rewrite_var_by_literal.find(variable(l)) != rewrite_var_by_literal.end()){ // the variable needs to be rewritten
        if(is_negated(l))
            proof << to_string(neg(rewrite_var_by_literal[l])) << " ";
        else 
            proof << to_string(rewrite_var_by_literal[l]) << " ";
    }
    else{
        proof << to_string(l) << " ";
    }
}
template void VeriPbProofLogger::write_literal<int>(const int &lit);
template void VeriPbProofLogger::write_literal<Glucose30::Lit>(const Glucose30::Lit &lit);

template <class TLit>
void VeriPbProofLogger::write_cardinality_constraint(const std::vector<TLit> &lits, const int RHS)
{
    for (int i = 0; i < lits.size(); i++)
        write_weighted_literal(lits[i]);
    proof << ">= " << RHS;
}
template void VeriPbProofLogger::write_cardinality_constraint<int>(const std::vector<int> &lits, const int RHS);
template void VeriPbProofLogger::write_cardinality_constraint<Glucose30::Lit>(const std::vector<Glucose30::Lit> &lits, const int RHS);

template <class TLit>
void VeriPbProofLogger::write_clause(const std::vector<TLit> &clause)
{
    write_cardinality_constraint(clause, 1);
}
template void VeriPbProofLogger::write_clause<int>(const std::vector<int> &clause);
template void VeriPbProofLogger::write_clause<Glucose30::Lit>(const std::vector<Glucose30::Lit> &clause);

template <class TLit>
void VeriPbProofLogger::write_PB_constraint(const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS)
{
    for (int i = 0; i < lits.size(); i++)
        write_weighted_literal(lits[i], weights[i]);
    proof << ">= " << RHS;
}
template void VeriPbProofLogger::write_PB_constraint<int>(const std::vector<int> &lits, const std::vector<int> &weights, const int RHS);
template void VeriPbProofLogger::write_PB_constraint<Glucose30::Lit>(const std::vector<Glucose30::Lit> &lits, const std::vector<int> &weights, const int RHS);

// ------------- Meaningful names -------------

template <class TVar>
void VeriPbProofLogger::store_meaningful_name(const TVar &var, const std::string &name)
{
    meaningful_names_store[toVeriPbVar(var)] = name;
}
template void VeriPbProofLogger::store_meaningful_name<int>(const int &var, const std::string &name);

template <class TVar>
void VeriPbProofLogger::delete_meaningful_name(const TVar &var)
{
    meaningful_names_store.erase(toVeriPbVar(var));
}
template void VeriPbProofLogger::delete_meaningful_name<int>(const int &var);

// ------------- Rules for checking constraints -------------

template <class TLit>
void VeriPbProofLogger::equals_rule(const constraintid constraint_id, const std::vector<TLit> &lits, const int RHS)
{
    proof << "e " << constraint_id << " ";
    write_cardinality_constraint(lits, RHS);
    proof << ";\n";
}
template void VeriPbProofLogger::equals_rule<int>(const constraintid constraint_id, const std::vector<int> &lits, const int RHS);
template void VeriPbProofLogger::equals_rule<Glucose30::Lit>(const constraintid constraint_id, const std::vector<Glucose30::Lit> &lits, const int RHS);

template <class TLit>
void VeriPbProofLogger::equals_rule(const constraintid constraint_id, const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS)
{
    proof << "e " << constraint_id << " ";
    write_PB_constraint(lits, weights, RHS);
    proof << ";\n";
}
template void VeriPbProofLogger::equals_rule<int>(const constraintid constraint_id, const std::vector<int> &lits, const std::vector<int> &weights, const int RHS);
template void VeriPbProofLogger::equals_rule<Glucose30::Lit>(const constraintid constraint_id, const std::vector<Glucose30::Lit> &lits, const std::vector<int> &weights, const int RHS);

template <class TLit>
void VeriPbProofLogger::check_last_constraint(const std::vector<TLit> &lits, const int RHS)
{
    equals_rule(constraint_counter, lits, RHS);
}
template void VeriPbProofLogger::check_last_constraint<int>(const std::vector<int> &lits, const int RHS);
template void VeriPbProofLogger::check_last_constraint<Glucose30::Lit>(const std::vector<Glucose30::Lit> &lits, const int RHS);

template <class TLit>
void VeriPbProofLogger::check_last_constraint(const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS)
{
    equals_rule(constraint_counter, lits, weights, RHS);
}
template void VeriPbProofLogger::check_last_constraint<int>(const std::vector<int> &lits, const std::vector<int> &weights, const int RHS);
template void VeriPbProofLogger::check_last_constraint<Glucose30::Lit>(const std::vector<Glucose30::Lit> &lits, const std::vector<int> &weights, const int RHS);

// ------------- Rules for optimisation -------------

/// @brief Calculate the objective value of the objective stored in VeriPBProoflogger for a model. This function uses the optimistic assumption that the literals are sorted.
/// @param model Vector of assignments to literals.
/// @return Objective value for model.
template <class TLit>
int VeriPbProofLogger::calculate_objective_value(const std::vector<TLit> &model)
{
    int objective_value = 0;
    int model_start_idx = 0;
    for (int objective_idx = 0; objective_idx < objective_lits.size(); objective_idx++)
    {
        TLit objective_lit = objective_lits[objective_idx];
        auto var = variable(objective_lit);
        int model_idx = model_start_idx;
        while (var != variable(model[model_idx]))
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
        if (objective_lit == model[model_idx])
        {
            objective_value += objective_weights[objective_idx];
        }
        model_start_idx = model_idx;
    }
    return objective_value;
}
template int VeriPbProofLogger::calculate_objective_value<int>(const std::vector<int> &model);

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
template constraintid VeriPbProofLogger::log_solution<int>(const std::vector<int> &model);
template constraintid VeriPbProofLogger::log_solution<Glucose30::Lit>(const std::vector<Glucose30::Lit> &model);

template <class TLit>
constraintid VeriPbProofLogger::log_solution_with_check(const std::vector<TLit> &model)
{
    int current_objective_value = calculate_objective_value(model);
    if (current_objective_value < best_objective_value)
    {
        best_solution_constraint = log_solution(model);
        best_objective_value = current_objective_value;
    }

    return best_solution_constraint;
}
template constraintid VeriPbProofLogger::log_solution_with_check<int>(const std::vector<int> &model);

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
    delete_constraint<int>(rewritten_best_solution_constraint);

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
template constraintid VeriPbProofLogger::unchecked_assumption<int>(const std::vector<int> &lits, const int RHS);
template constraintid VeriPbProofLogger::unchecked_assumption<Glucose30::Lit>(const std::vector<Glucose30::Lit> &lits, const int RHS);

template <class TLit>
constraintid VeriPbProofLogger::unchecked_assumption(const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS)
{
    proof << "u ";
    write_PB_constraint(lits, weights, RHS);
    proof << " ;\n";
    return ++constraint_counter;
}
template constraintid VeriPbProofLogger::unchecked_assumption<int>(const std::vector<int> &lits, const std::vector<int> &weights, const int RHS);
template constraintid VeriPbProofLogger::unchecked_assumption<Glucose30::Lit>(const std::vector<Glucose30::Lit> &lits, const std::vector<int> &weights, const int RHS);

// ------------- Reverse Unit Propagation -------------
template <class TLit>
constraintid VeriPbProofLogger::rup(const std::vector<TLit> &lits, const int RHS)
{
    proof << "u ";
    write_cardinality_constraint(lits, RHS);
    proof << ";\n";
    return ++constraint_counter;
}
template constraintid VeriPbProofLogger::rup<int>(const std::vector<int> &lits, const int RHS);
template constraintid VeriPbProofLogger::rup<Glucose30::Lit>(const std::vector<Glucose30::Lit> &lits, const int RHS);

template <class TLit>
constraintid VeriPbProofLogger::rup(const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS)
{
    proof << "u ";
    write_PB_constraint(lits, weights, RHS);
    proof << " ;\n";
    return ++constraint_counter;
}
template constraintid VeriPbProofLogger::rup<int>(const std::vector<int> &lits, const std::vector<int> &weights, const int RHS);
template constraintid VeriPbProofLogger::rup<Glucose30::Lit>(const std::vector<Glucose30::Lit> &lits, const std::vector<int> &weights, const int RHS);

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

    if(rewrite_var_by_literal.find(var) != rewrite_var_by_literal.end()){
        if(is_negated(lit))
            lit = neg(rewrite_var_by_literal[var]);
        else
            lit = rewrite_var_by_literal[var];
    }

    proof << var_name(variable(lit)) << " -> " << (is_negated(lit) ? "0" : "1");
    
}
template void VeriPbProofLogger::write_witness<int>(const int &literal);
template void VeriPbProofLogger::write_witness<Glucose30::Lit>(const Glucose30::Lit &literal);

template <class TLit>
void VeriPbProofLogger::write_witness(const std::vector<TLit> &witness)
{
    for (int i = 0; i < witness.size(); i++)
    {
        write_witness(witness[i]);
    }
}
template void VeriPbProofLogger::write_witness<int>(const std::vector<int> &witness);
template void VeriPbProofLogger::write_witness<Glucose30::Lit>(const std::vector<Glucose30::Lit> &witness);

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
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<int>(const std::vector<int> &lits, const int RHS, const int &witness);
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<Glucose30::Lit>(const std::vector<Glucose30::Lit> &lits, const int RHS, const Glucose30::Lit &witness);

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
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<int>(const std::vector<int> &lits, const int RHS, const std::vector<int> &witness);
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<Glucose30::Lit>(const std::vector<Glucose30::Lit> &lits, const int RHS, const std::vector<Glucose30::Lit> &witness);

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
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<int>(const std::vector<int> &lits, const std::vector<int> &weights, const int RHS, const int &witness);
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<Glucose30::Lit>(const std::vector<Glucose30::Lit> &lits, const std::vector<int> &weights, const int RHS, const Glucose30::Lit &witness);

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
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<int>(const std::vector<int> &lits, const std::vector<int> &weights, const int RHS, const std::vector<int> &witness);
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<Glucose30::Lit>(const std::vector<Glucose30::Lit> &lits, const std::vector<int> &weights, const int RHS, const std::vector<Glucose30::Lit> &witness);

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
template void VeriPbProofLogger::start_CP_derivation_with_lit_axiom<int>(const int &lit);
template void VeriPbProofLogger::start_CP_derivation_with_lit_axiom<Glucose30::Lit>(const Glucose30::Lit &lit);

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
template void VeriPbProofLogger::CP_weakening<int>(const int &var);

template <class TLit>
void VeriPbProofLogger::CP_literal_axiom(const TLit &lit)
{
    pol_string << " " << to_string(lit);
}
template void VeriPbProofLogger::CP_literal_axiom<int>(const int &lit);
template void VeriPbProofLogger::CP_literal_axiom<Glucose30::Lit>(const Glucose30::Lit &lit);

constraintid VeriPbProofLogger::end_CP_derivation()
{
    proof << pol_string.rdbuf() << "\n";
    return ++constraint_counter;
}

//  ------------- Deleting & Overwriting Constraints -------------

template <class TLit>
void VeriPbProofLogger::delete_constraint(const constraintid constraint_id)
{
    proof << "del id " << constraint_id << "\n";
}
template void VeriPbProofLogger::delete_constraint<int>(const constraintid constraint_id);
template void VeriPbProofLogger::delete_constraint<Glucose30::Lit>(const constraintid constraint_id);

template <class TLit>
void VeriPbProofLogger::delete_constraint(const std::vector<constraintid> &constraint_ids)
{
    proof << "del id";
    for (int i = 0; i < constraint_ids.size(); i++)
    {
        proof << " " << constraint_ids[i];
    }
    proof << "\n";
}
template void VeriPbProofLogger::delete_constraint<int>(const std::vector<constraintid> &constraint_ids);
template void VeriPbProofLogger::delete_constraint<Glucose30::Lit>(const std::vector<constraintid> &constraint_ids);

template <class TLit>
void VeriPbProofLogger::delete_constraint(const std::vector<TLit> &lits, const int RHS)
{
    proof << "del find ";
    write_cardinality_constraint(lits, RHS);
    proof << ";\n";
}
template void VeriPbProofLogger::delete_constraint<int>(const std::vector<int> &lits, const int RHS);
template void VeriPbProofLogger::delete_constraint<Glucose30::Lit>(const std::vector<Glucose30::Lit> &lits, const int RHS);

template <class TLit>
void VeriPbProofLogger::delete_constraint(const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS)
{
    proof << "del find ";
    write_PB_constraint(lits, weights, RHS);
    proof << ";\n";
}
template void VeriPbProofLogger::delete_constraint<int>(const std::vector<int> &lits, const std::vector<int> &weights, const int RHS);
template void VeriPbProofLogger::delete_constraint<Glucose30::Lit>(const std::vector<Glucose30::Lit> &lits, const std::vector<int> &weights, const int RHS);

template <class TLit>
constraintid VeriPbProofLogger::overwrite_constraint(const constraintid constraint_id, const std::vector<TLit> &lits, const int RHS)
{
    constraintid newconstraint = rup(lits, RHS);
    delete_constraint<TLit>(constraint_id);
    return newconstraint;
}
template constraintid VeriPbProofLogger::overwrite_constraint<int>(const constraintid constraint_id, const std::vector<int> &lits, const int RHS);
template constraintid VeriPbProofLogger::overwrite_constraint<Glucose30::Lit>(const constraintid constraint_id, const std::vector<Glucose30::Lit> &lits, const int RHS);

template <class TLit>
constraintid VeriPbProofLogger::overwrite_constraint(const std::vector<TLit> &lits_orig, const int RHS_orig, const std::vector<TLit> &lits, const int RHS)
{
    constraintid newconstraint = rup(lits, RHS);
    delete_constraint<TLit>(lits_orig, RHS_orig);
    return newconstraint;
}
template constraintid VeriPbProofLogger::overwrite_constraint<int>(const std::vector<int> &lits_orig, const int RHS_orig, const std::vector<int> &lits, const int RHS);
template constraintid VeriPbProofLogger::overwrite_constraint<Glucose30::Lit>(const std::vector<Glucose30::Lit> &lits_orig, const int RHS_orig, const std::vector<Glucose30::Lit> &lits, const int RHS);

template <class TLit>
constraintid VeriPbProofLogger::overwrite_constraint(const constraintid constraint_id, const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS)
{
    constraintid newconstraint = rup(lits, weights, RHS);
    delete_constraint<TLit>(constraint_id);
    return newconstraint;
}
template constraintid VeriPbProofLogger::overwrite_constraint<int>(const constraintid constraint_id, const std::vector<int> &lits, const std::vector<int> &weights, const int RHS);
template constraintid VeriPbProofLogger::overwrite_constraint<Glucose30::Lit>(const constraintid constraint_id, const std::vector<Glucose30::Lit> &lits, const std::vector<int> &weights, const int RHS);

template <class TLit>
constraintid VeriPbProofLogger::overwrite_constraint(const std::vector<TLit> &lits_orig, const std::vector<int> &weights_orig, const int RHS_orig, const std::vector<TLit> &lits, const std::vector<int> &weights, const int RHS)
{
    constraintid newconstraint = rup(lits, weights, RHS);
    delete_constraint<TLit>(lits_orig, weights_orig, RHS_orig);
    return newconstraint;
}
template constraintid VeriPbProofLogger::overwrite_constraint<int>(const std::vector<int> &lits_orig, const std::vector<int> &weights_orig, const int RHS_orig, const std::vector<int> &lits, const std::vector<int> &weights, const int RHS);
template constraintid VeriPbProofLogger::overwrite_constraint<Glucose30::Lit>(const std::vector<Glucose30::Lit> &lits_orig, const std::vector<int> &weights_orig, const int RHS_orig, const std::vector<Glucose30::Lit> &lits, const std::vector<int> &weights, const int RHS);

template <class TLit>
constraintid VeriPbProofLogger::overwrite_constraint(const std::vector<TLit> &lits_orig, const std::vector<TLit> &lits)
{
    constraintid newconstraint = rup(lits);
    delete_constraint<TLit>(lits_orig);
    return newconstraint;
}
template constraintid VeriPbProofLogger::overwrite_constraint<int>(const std::vector<int> &lits_orig, const std::vector<int> &lits);
template constraintid VeriPbProofLogger::overwrite_constraint<Glucose30::Lit>(const std::vector<Glucose30::Lit> &lits_orig, const std::vector<Glucose30::Lit> &lits);

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
void VeriPbProofLogger::write_clause(Glucose30::Clause &clause)
{
    for (int i = 0; i < clause.size(); i++)
        write_weighted_literal(clause[i]);
    proof << " >= 1";
}
// template void VeriPbProofLogger::write_clause<Glucose30::Clause>(Glucose30::Clause &clause);

void VeriPbProofLogger::write_clause(Minisat::Clause &clause)
{
    for (int i = 0; i < clause.size(); i++)
        write_weighted_literal(clause[i]);
    proof << " >= 1";
}

template <template <class T> class TVec, class TLit>
void VeriPbProofLogger::write_clause(TVec<TLit> &clause)
{
    for (int i = 0; i < clause.size(); i++)
        write_weighted_literal(clause[i]);
    proof << " >= 1";
}
template void VeriPbProofLogger::write_clause<Glucose30::vec, Glucose30::Lit>(Glucose30::vec<Glucose30::Lit> &clause);

template <class TLBool>
void VeriPbProofLogger::write_literal_assignment(TLBool assignment, int var)
{
    proof << (toInt(assignment) == 0 ? "" : "~") << var_name(var) << " "; // l_true and g3l_true is defined as 0.
}
template void VeriPbProofLogger::write_literal_assignment<Glucose30::lbool>(Glucose30::lbool assignment, int var);

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
template constraintid VeriPbProofLogger::log_solution<Glucose30::vec, Glucose30::lbool>(Glucose30::vec<Glucose30::lbool> &model);
template constraintid VeriPbProofLogger::log_solution<Minisat::vec, Minisat::lbool>(Minisat::vec<Minisat::lbool> &model);

template <template <class T> class TVec, class TLit>
constraintid VeriPbProofLogger::rup(TVec<TLit> &clause)
{
    proof << "u ";
    write_clause(clause);
    proof << ";\n";
    return ++constraint_counter;
}
template constraintid VeriPbProofLogger::rup<Glucose30::vec, Glucose30::Lit>(Glucose30::vec<Glucose30::Lit> &clause);

template <template <class T> class TVec, class TLit>
void VeriPbProofLogger::overwrite_constraint(TVec<TLit> &orig, TVec<TLit> &clause)
{
    rup(clause);
    delete_constraint(orig);
}
template void VeriPbProofLogger::overwrite_constraint<Glucose30::vec, Glucose30::Lit>(Glucose30::vec<Glucose30::Lit> &orig, Glucose30::vec<Glucose30::Lit> &clause);
template void VeriPbProofLogger::overwrite_constraint<Minisat::vec, Minisat::Lit>(Minisat::vec<Minisat::Lit> &orig, Minisat::vec<Minisat::Lit> &clause);

// template <class TClause>
void VeriPbProofLogger::delete_constraint(Glucose30::Clause &clause)
{
    proof << "del find ";
    write_clause(clause);
    proof << ";\n";
}
// template void VeriPbProofLogger::delete_constraint<Glucose30::Clause>(Glucose30::Clause &clause);

void VeriPbProofLogger::delete_constraint(Minisat::Clause &clause)
{
    proof << "del find ";
    write_clause(clause);
    proof << ";\n";
}

template <template <class T> class TVec, class TLit>
void VeriPbProofLogger::delete_constraint(TVec<TLit> &clause)
{
    proof << "del find ";
    write_clause(clause);
    proof << ";\n";
}
template void VeriPbProofLogger::delete_constraint<Glucose30::vec, Glucose30::Lit>(Glucose30::vec<Glucose30::Lit> &clause);

//=================================================================================================