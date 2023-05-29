#include "VeriPBProoflogger.hpp"

//=================================================================================================

template void VeriPbProofLogger::set_objective<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, wght constant_cost);
template void VeriPbProofLogger::set_objective<Glucose::vec<Glucose::Lit>, Glucose::vec<wght>>(const Glucose::vec<Glucose::Lit> &lits, const Glucose::vec<wght> &weights, wght constant_cost);

template void VeriPbProofLogger::add_objective_literal<VeriPB::Lit>(VeriPB::Lit lit, wght weight);
template void VeriPbProofLogger::add_objective_literal<Glucose::Lit>(Glucose::Lit lit, wght weight);

template bool VeriPbProofLogger::is_aux_var<VeriPB::Var>(const VeriPB::Var &var);
template bool VeriPbProofLogger::is_aux_var<Glucose::Var>(const Glucose::Var &var);

template std::string VeriPbProofLogger::var_name<VeriPB::Var>(const VeriPB::Var &var);
template std::string VeriPbProofLogger::var_name<Glucose::Var>(const Glucose::Var &var);

template void VeriPbProofLogger::write_weighted_literal<VeriPB::Lit>(const VeriPB::Lit &literal, wght weight);
template void VeriPbProofLogger::write_weighted_literal<Glucose::Lit>(const Glucose::Lit &literal, wght weight);

template std::string VeriPbProofLogger::to_string<VeriPB::Lit>(const VeriPB::Lit &lit);
template std::string VeriPbProofLogger::to_string<Glucose::Lit>(const Glucose::Lit &lit);

template void VeriPbProofLogger::write_literal<VeriPB::Lit>(const VeriPB::Lit &lit);
template void VeriPbProofLogger::write_literal<Glucose::Lit>(const Glucose::Lit &lit);

template void VeriPbProofLogger::write_cardinality_constraint<Glucose::vec<Glucose::Lit>>(const Glucose::vec<Glucose::Lit> &lits, const wght RHS);
template void VeriPbProofLogger::write_cardinality_constraint<Glucose::Clause>(const Glucose::Clause &lits, const wght RHS);
template void VeriPbProofLogger::write_cardinality_constraint<std::vector<Glucose::Lit>>(const std::vector<Glucose::Lit> &lits, const wght RHS);
template void VeriPbProofLogger::write_cardinality_constraint<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &lits, const wght RHS);

template void VeriPbProofLogger::write_clause<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &clause);
template void VeriPbProofLogger::write_clause<std::vector<Glucose::Lit>>(const std::vector<Glucose::Lit> &clause);
template void VeriPbProofLogger::write_clause<Glucose::vec<Glucose::Lit>>(const Glucose::vec<Glucose::Lit> &clause);
template void VeriPbProofLogger::write_clause<Glucose::Clause>(const Glucose::Clause &clause);

template void VeriPbProofLogger::write_PB_constraint<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS);
template void VeriPbProofLogger::write_PB_constraint<Glucose::vec<Glucose::Lit>, Glucose::vec<wght>>(const Glucose::vec<Glucose::Lit> &lits, const Glucose::vec<wght> &weights, const wght RHS);

template void VeriPbProofLogger::rewrite_variable_by_literal<VeriPB::Var, VeriPB::Lit>(const VeriPB::Var& var, const VeriPB::Lit& lit);
template void VeriPbProofLogger::rewrite_variable_by_literal<Glucose::Var, Glucose::Lit>(const Glucose::Var& var, const Glucose::Lit& lit);

template void VeriPbProofLogger::store_meaningful_name<VeriPB::Var>(const VeriPB::Var &var, const std::string &name);
template void VeriPbProofLogger::store_meaningful_name<Glucose::Var>(const Glucose::Var &var, const std::string &name);

template void VeriPbProofLogger::delete_meaningful_name<VeriPB::Var>(const VeriPB::Var &var);
template void VeriPbProofLogger::delete_meaningful_name<Glucose::Var>(const Glucose::Var &var);

template void VeriPbProofLogger::equals_rule<std::vector<VeriPB::Lit>>(const constraintid constraint_id, const std::vector<VeriPB::Lit> &lits, const wght RHS);
template void VeriPbProofLogger::equals_rule<Glucose::vec<Glucose::Lit>>(const constraintid constraint_id, const Glucose::vec<Glucose::Lit> &lits, const wght RHS);

template void VeriPbProofLogger::equals_rule<std::vector<VeriPB::Lit>, std::vector<wght>>(const constraintid constraint_id, const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS);
template void VeriPbProofLogger::equals_rule<Glucose::vec<Glucose::Lit>, Glucose::vec<wght>>(const constraintid constraint_id, const Glucose::vec<Glucose::Lit> &lits, const Glucose::vec<wght> &weights, const wght RHS);

template void VeriPbProofLogger::check_last_constraint<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &lits, const wght RHS);
template void VeriPbProofLogger::check_last_constraint<Glucose::vec<Glucose::Lit>>(const Glucose::vec<Glucose::Lit> &lits, const wght RHS);

template void VeriPbProofLogger::check_last_constraint<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS);
template void VeriPbProofLogger::check_last_constraint<Glucose::vec<Glucose::Lit>, Glucose::vec<wght>>(const Glucose::vec<Glucose::Lit> &lits, const Glucose::vec<wght> &weights, const wght RHS);

template wght VeriPbProofLogger::calculate_objective_value<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &model);

template constraintid VeriPbProofLogger::log_solution<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &model, wght objective_value);
template constraintid VeriPbProofLogger::log_solution<Glucose::vec<Glucose::Lit>>(const Glucose::vec<Glucose::Lit> &model, wght objective_value);

template constraintid VeriPbProofLogger::log_solution_with_check<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &model);

template constraintid VeriPbProofLogger::log_solution_lbools<Glucose::vec<Glucose::lbool>>(Glucose::vec<Glucose::lbool> &model, wght objective_value);

template constraintid VeriPbProofLogger::unchecked_assumption<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &lits, const wght RHS);
template constraintid VeriPbProofLogger::unchecked_assumption<Glucose::vec<Glucose::Lit>>(const Glucose::vec<Glucose::Lit> &lits, const wght RHS);

template constraintid VeriPbProofLogger::unchecked_assumption<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS);
template constraintid VeriPbProofLogger::unchecked_assumption<Glucose::vec<Glucose::Lit>, Glucose::vec<wght>>(const Glucose::vec<Glucose::Lit> &lits, const Glucose::vec<wght> &weights, const wght RHS);

template constraintid VeriPbProofLogger::rup<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &lits, const wght RHS);
template constraintid VeriPbProofLogger::rup<Glucose::vec<Glucose::Lit>>(const Glucose::vec<Glucose::Lit> &lits, const wght RHS);

template constraintid VeriPbProofLogger::rup<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS);
template constraintid VeriPbProofLogger::rup<Glucose::vec<Glucose::Lit>, Glucose::vec<wght>>(const Glucose::vec<Glucose::Lit> &lits, const Glucose::vec<wght> &weights, const wght RHS);

template void VeriPbProofLogger::write_witness<VeriPB::Var>(const substitution<VeriPB::Var> &witness);
template void VeriPbProofLogger::write_witness<Glucose::Var>(const substitution<Glucose::Var> &witness);

template constraintid VeriPbProofLogger::redundanceBasedStrengthening<std::vector<VeriPB::Lit>, VeriPB::Var>(const std::vector<VeriPB::Lit> &lits, const wght RHS, const substitution<VeriPB::Var> &witness);
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<Glucose::vec<Glucose::Lit>, Glucose::Var>(const Glucose::vec<Glucose::Lit> &lits, const wght RHS, const substitution<Glucose::Var> &witness);

template constraintid VeriPbProofLogger::redundanceBasedStrengthening<std::vector<VeriPB::Lit>, std::vector<wght>, VeriPB::Var>(const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS, const substitution<VeriPB::Var> &witness);
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<Glucose::vec<Glucose::Lit>, Glucose::vec<wght>, Glucose::Var>(const Glucose::vec<Glucose::Lit> &lits, const Glucose::vec<wght> &weights, const wght RHS, const substitution<Glucose::Var> &witness);

template constraintid VeriPbProofLogger::reificationLiteralRightImpl<std::vector<VeriPB::Lit>, std::vector<wght>, VeriPB::Lit>(const VeriPB::Lit& lit, const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS, bool store_reified_constraint);
template constraintid VeriPbProofLogger::reificationLiteralRightImpl<Glucose::vec<Glucose::Lit>, Glucose::vec<wght>, Glucose::Lit>(const Glucose::Lit& lit, const Glucose::vec<Glucose::Lit> &lits, const Glucose::vec<wght> &weights, const wght RHS, bool store_reified_constraint);

template constraintid VeriPbProofLogger::reificationLiteralRightImpl<std::vector<VeriPB::Lit>, VeriPB::Lit>(const VeriPB::Lit& lit, const std::vector<VeriPB::Lit> &lits, const wght RHS, bool store_reified_constraint);
template constraintid VeriPbProofLogger::reificationLiteralRightImpl<Glucose::vec<Glucose::Lit>, Glucose::Lit>(const Glucose::Lit& lit, const Glucose::vec<Glucose::Lit> &lits, const wght RHS, bool store_reified_constraint);

template constraintid VeriPbProofLogger::reificationLiteralLeftImpl<std::vector<VeriPB::Lit>, std::vector<wght>, VeriPB::Lit>(const VeriPB::Lit& lit, const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS, bool store_reified_constraint);
template constraintid VeriPbProofLogger::reificationLiteralLeftImpl<Glucose::vec<Glucose::Lit>, Glucose::vec<wght>, Glucose::Lit>(const Glucose::Lit& lit, const Glucose::vec<Glucose::Lit> &lits, const Glucose::vec<wght> &weights, const wght RHS, bool store_reified_constraint);

template constraintid VeriPbProofLogger::reificationLiteralLeftImpl<std::vector<VeriPB::Lit>, VeriPB::Lit>(const VeriPB::Lit& lit, const std::vector<VeriPB::Lit> &lits, const wght RHS, bool store_reified_constraint);
template constraintid VeriPbProofLogger::reificationLiteralLeftImpl<Glucose::vec<Glucose::Lit>, Glucose::Lit>(const Glucose::Lit& lit, const Glucose::vec<Glucose::Lit> &lits, const wght RHS, bool store_reified_constraint);

template constraintid VeriPbProofLogger::getReifiedConstraintLeftImpl<Glucose::Var>(const Glucose::Var& var);
template constraintid VeriPbProofLogger::getReifiedConstraintLeftImpl<VeriPB::Var>(const VeriPB::Var& var);
template constraintid VeriPbProofLogger::getReifiedConstraintRightImpl<Glucose::Var>(const Glucose::Var& var);
template constraintid VeriPbProofLogger::getReifiedConstraintRightImpl<VeriPB::Var>(const VeriPB::Var& var);
    
template void VeriPbProofLogger::setReifiedConstraintLeftImpl<VeriPB::Var>(const VeriPB::Var& var, constraintid cxnId);
template void VeriPbProofLogger::setReifiedConstraintLeftImpl<Glucose::Var>(const Glucose::Var& var, constraintid cxnId);
template void VeriPbProofLogger::setReifiedConstraintRightImpl<VeriPB::Var>(const VeriPB::Var& var, constraintid cxnId);
template void VeriPbProofLogger::setReifiedConstraintRightImpl<Glucose::Var>(const Glucose::Var& var, constraintid cxnId);
template void VeriPbProofLogger::deleteReifiedConstraintLeftImpl<VeriPB::Var>(const VeriPB::Var& var);
template void VeriPbProofLogger::deleteReifiedConstraintLeftImpl<Glucose::Var>(const Glucose::Var& var);
template void VeriPbProofLogger::deleteReifiedConstraintRightImpl<VeriPB::Var>(const VeriPB::Var& var);
template void VeriPbProofLogger::deleteReifiedConstraintRightImpl<Glucose::Var>(const Glucose::Var& var);

template cuttingplanes_derivation VeriPbProofLogger::CP_literal_axiom<VeriPB::Lit>(const VeriPB::Lit& lit); 
template cuttingplanes_derivation VeriPbProofLogger::CP_literal_axiom<Glucose::Lit>(const Glucose::Lit& lit); 
template cuttingplanes_derivation VeriPbProofLogger::CP_weakening<VeriPB::Var>(const cuttingplanes_derivation& cp, const VeriPB::Var& var);
template cuttingplanes_derivation VeriPbProofLogger::CP_weakening<Glucose::Var>(const cuttingplanes_derivation& cp, const Glucose::Var& var);
template cuttingplanes_derivation VeriPbProofLogger::CP_weakening<VeriPB::Var>(const VeriPB::Var& var);
template cuttingplanes_derivation VeriPbProofLogger::CP_weakening<Glucose::Var>(const Glucose::Var& var);

template void VeriPbProofLogger::start_CP_derivation_with_lit_axiom<VeriPB::Lit>(const VeriPB::Lit &lit);
template void VeriPbProofLogger::start_CP_derivation_with_lit_axiom<Glucose::Lit>(const Glucose::Lit &lit);

template void VeriPbProofLogger::CP_weaken<Glucose::Var>(const Glucose::Var &var);
template void VeriPbProofLogger::CP_weaken<VeriPB::Var>(const VeriPB::Var &var);

template void VeriPbProofLogger::CP_add_literal_axiom<VeriPB::Lit>(const VeriPB::Lit &lit);
template void VeriPbProofLogger::CP_add_literal_axiom<Glucose::Lit>(const Glucose::Lit &lit);

template void VeriPbProofLogger::CP_write_literal_axiom<VeriPB::Lit>(const VeriPB::Lit &lit);
template void VeriPbProofLogger::CP_write_literal_axiom<Glucose::Lit>(const Glucose::Lit &lit);

template void VeriPbProofLogger::delete_constraint<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &lits, const wght RHS);
template void VeriPbProofLogger::delete_constraint<Glucose::vec<Glucose::Lit>>(const Glucose::vec<Glucose::Lit> &lits, const wght RHS);
template void VeriPbProofLogger::delete_constraint<Glucose::Clause>(const Glucose::Clause &lits, const wght RHS);

template void VeriPbProofLogger::delete_constraint<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS);
template void VeriPbProofLogger::delete_constraint<Glucose::vec<Glucose::Lit>, Glucose::vec<wght>>(const Glucose::vec<Glucose::Lit> &lits, const Glucose::vec<wght> &weights, const wght RHS);

template constraintid VeriPbProofLogger::overwrite_constraint<std::vector<VeriPB::Lit>>(const constraintid constraint_id, const std::vector<VeriPB::Lit> &lits, const wght RHS);
template constraintid VeriPbProofLogger::overwrite_constraint<Glucose::vec<Glucose::Lit>>(const constraintid constraint_id, const Glucose::vec<Glucose::Lit> &lits, const wght RHS);

template constraintid VeriPbProofLogger::overwrite_constraint<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &lits_orig, const wght RHS_orig, const std::vector<VeriPB::Lit> &lits, const wght RHS);
template constraintid VeriPbProofLogger::overwrite_constraint<Glucose::vec<Glucose::Lit>>(const Glucose::vec<Glucose::Lit> &lits_orig, const wght RHS_orig, const Glucose::vec<Glucose::Lit> &lits, const wght RHS);

template constraintid VeriPbProofLogger::overwrite_constraint<std::vector<VeriPB::Lit>, std::vector<wght>>(const constraintid constraint_id, const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS);
template constraintid VeriPbProofLogger::overwrite_constraint<Glucose::vec<Glucose::Lit>, Glucose::vec<wght>>(const constraintid constraint_id, const Glucose::vec<Glucose::Lit> &lits, const Glucose::vec<wght> &weights, const wght RHS);

template constraintid VeriPbProofLogger::overwrite_constraint<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit> &lits_orig, const std::vector<wght> &weights_orig, const wght RHS_orig, const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS);
template constraintid VeriPbProofLogger::overwrite_constraint<Glucose::vec<Glucose::Lit>, Glucose::vec<wght>>(const Glucose::vec<Glucose::Lit> &lits_orig, const Glucose::vec<wght> &weights_orig, const wght RHS_orig, const Glucose::vec<Glucose::Lit> &lits, const Glucose::vec<wght> &weights, const wght RHS);

template constraintid VeriPbProofLogger::overwrite_constraint<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &lits_orig, const std::vector<VeriPB::Lit> &lits);
template constraintid VeriPbProofLogger::overwrite_constraint<Glucose::vec<Glucose::Lit>>(const Glucose::vec<Glucose::Lit> &lits_orig, const Glucose::vec<Glucose::Lit> &lits);

