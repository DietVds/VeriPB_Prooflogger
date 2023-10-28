#include "VeriPBProoflogger.hpp"

//=================================================================================================

template void VeriPbProofLogger::set_objective<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, wght constant_cost);
template void VeriPbProofLogger::add_objective_literal<VeriPB::Lit>(VeriPB::Lit lit, wght weight);
template bool VeriPbProofLogger::is_aux_var<VeriPB::Var>(const VeriPB::Var &var);
template std::string VeriPbProofLogger::var_name<VeriPB::Var>(const VeriPB::Var &var);
template void VeriPbProofLogger::write_weighted_literal<VeriPB::Lit>(const VeriPB::Lit &literal, wght weight);
template std::string VeriPbProofLogger::to_string<VeriPB::Lit>(const VeriPB::Lit &lit);
template void VeriPbProofLogger::write_literal<VeriPB::Lit>(const VeriPB::Lit &lit);
template void VeriPbProofLogger::write_cardinality_constraint<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &lits, const wght RHS);

template void VeriPbProofLogger::write_clause<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &clause);
template void VeriPbProofLogger::write_PB_constraint<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS);
template void VeriPbProofLogger::rewrite_variable_by_literal<VeriPB::Var, VeriPB::Lit>(const VeriPB::Var& var, const VeriPB::Lit& lit);
template void VeriPbProofLogger::store_meaningful_name<VeriPB::Var>(const VeriPB::Var &var, const std::string &name);
template void VeriPbProofLogger::delete_meaningful_name<VeriPB::Var>(const VeriPB::Var &var);
template void VeriPbProofLogger::equals_rule<std::vector<VeriPB::Lit>>(const constraintid constraint_id, const std::vector<VeriPB::Lit> &lits, const wght RHS);
template void VeriPbProofLogger::write_PB_constraint<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit>& lits_greater, const std::vector<wght>& weights_greater, const wght const_greater, const std::vector<VeriPB::Lit>& lits_smaller, const std::vector<wght>& weights_smaller, const wght const_smaller );
template void VeriPbProofLogger::equals_rule<std::vector<VeriPB::Lit>, std::vector<wght>>(const constraintid constraint_id, const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS);
template void VeriPbProofLogger::equals_rule<std::vector<VeriPB::Lit>, std::vector<wght>>(const constraintid constraint_id, const std::vector<VeriPB::Lit>& lits_greater, const std::vector<wght>& weights_greater, const wght const_greater, const std::vector<VeriPB::Lit>& lits_smaller, const std::vector<wght>& weights_smaller, const wght const_smaller  );
template void VeriPbProofLogger::check_last_constraint<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &lits, const wght RHS);
template void VeriPbProofLogger::check_last_constraint<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS);
template void VeriPbProofLogger::check_last_constraint<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit>& lits_greater, const std::vector<wght>& weights_greater, const wght const_greater, const std::vector<VeriPB::Lit>& lits_smaller, const std::vector<wght>& weights_smaller, const wght const_smaller  );
template wght VeriPbProofLogger::calculate_objective_value<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &model);
template constraintid VeriPbProofLogger::log_solution<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &model, wght objective_value);
template constraintid VeriPbProofLogger::log_solution_with_check<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &model);
template constraintid VeriPbProofLogger::unchecked_assumption<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &lits, const wght RHS);
template constraintid VeriPbProofLogger::unchecked_assumption<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS);
template constraintid VeriPbProofLogger::rup<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &lits, const wght RHS);
template constraintid VeriPbProofLogger::rup<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS);
template void VeriPbProofLogger::write_witness<VeriPB::Var>(const substitution<VeriPB::Var> &witness);
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<std::vector<VeriPB::Lit>, VeriPB::Var>(const std::vector<VeriPB::Lit> &lits, const wght RHS, const substitution<VeriPB::Var> &witness);
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<std::vector<VeriPB::Lit>, std::vector<wght>, VeriPB::Var>(const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS, const substitution<VeriPB::Var> &witness);
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<std::vector<VeriPB::Lit>, std::vector<wght>, VeriPB::Var>(const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS, const substitution<VeriPB::Var> &witness, std::vector<subproof> subproofs);
template constraintid VeriPbProofLogger::reificationLiteralRightImpl<std::vector<VeriPB::Lit>, std::vector<wght>, VeriPB::Lit>(const VeriPB::Lit& lit, const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS, bool store_reified_constraint);
template constraintid VeriPbProofLogger::reificationLiteralRightImpl<std::vector<VeriPB::Lit>, VeriPB::Lit>(const VeriPB::Lit& lit, const std::vector<VeriPB::Lit> &lits, const wght RHS, bool store_reified_constraint);
template constraintid VeriPbProofLogger::reificationLiteralLeftImpl<std::vector<VeriPB::Lit>, std::vector<wght>, VeriPB::Lit>(const VeriPB::Lit& lit, const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS, bool store_reified_constraint);
template constraintid VeriPbProofLogger::reificationLiteralLeftImpl<std::vector<VeriPB::Lit>, VeriPB::Lit>(const VeriPB::Lit& lit, const std::vector<VeriPB::Lit> &lits, const wght RHS, bool store_reified_constraint);
template constraintid VeriPbProofLogger::getReifiedConstraintLeftImpl<VeriPB::Var>(const VeriPB::Var& var);
template constraintid VeriPbProofLogger::getReifiedConstraintRightImpl<VeriPB::Var>(const VeriPB::Var& var);
template void VeriPbProofLogger::setReifiedConstraintLeftImpl<VeriPB::Var>(const VeriPB::Var& var, constraintid cxnId);
template void VeriPbProofLogger::setReifiedConstraintRightImpl<VeriPB::Var>(const VeriPB::Var& var, constraintid cxnId);
template void VeriPbProofLogger::deleteReifiedConstraintLeftImpl<VeriPB::Var>(const VeriPB::Var& var);
template void VeriPbProofLogger::deleteReifiedConstraintRightImpl<VeriPB::Var>(const VeriPB::Var& var);
template void VeriPbProofLogger::removeReifiedConstraintRightImplFromConstraintStore<VeriPB::Var>(const VeriPB::Var& var);
template void VeriPbProofLogger::removeReifiedConstraintLeftImplFromConstraintStore<VeriPB::Var>(const VeriPB::Var& var);
template cuttingplanes_derivation VeriPbProofLogger::CP_literal_axiom<VeriPB::Lit>(const VeriPB::Lit& lit); 
template cuttingplanes_derivation VeriPbProofLogger::CP_weakening<VeriPB::Var>(const cuttingplanes_derivation& cp, const VeriPB::Var& var);
template cuttingplanes_derivation VeriPbProofLogger::CP_weakening<VeriPB::Var>(const VeriPB::Var& var);
template cuttingplanes_derivation VeriPbProofLogger::CP_weakening<VeriPB::Lit>(const cuttingplanes_derivation& cp, const VeriPB::Lit& lit, const wght& weight);
template cuttingplanes_derivation VeriPbProofLogger::CP_weakening<VeriPB::Lit>(const VeriPB::Lit& lit, const wght& weight);
template void VeriPbProofLogger::start_CP_derivation_with_lit_axiom<VeriPB::Lit>(const VeriPB::Lit &lit);
template void VeriPbProofLogger::CP_weaken<VeriPB::Var>(const VeriPB::Var &var);
template void VeriPbProofLogger::CP_add_literal_axiom<VeriPB::Lit>(const VeriPB::Lit &lit);
template void VeriPbProofLogger::CP_write_literal_axiom<VeriPB::Lit>(const VeriPB::Lit &lit);
template constraintid VeriPbProofLogger::prove_by_contradiction<std::vector<VeriPB::Lit>, std::vector<wght>>(std::vector<VeriPB::Lit>& lits, std::vector<wght>& weights, wght RHS, std::vector<cuttingplanes_derivation> cpder);
template constraintid VeriPbProofLogger::prove_by_casesplitting<std::vector<VeriPB::Lit>, std::vector<wght>>(std::vector<VeriPB::Lit>& lits, std::vector<wght>& weights, wght RHS, constraintid case1, constraintid case2);
template void VeriPbProofLogger::delete_constraint<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &lits, const wght RHS);
template void VeriPbProofLogger::delete_constraint<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS);
template constraintid VeriPbProofLogger::overwrite_constraint<std::vector<VeriPB::Lit>>(const constraintid constraint_id, const std::vector<VeriPB::Lit> &lits, const wght RHS, bool origclause_in_coreset);
template constraintid VeriPbProofLogger::overwrite_constraint<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &lits_orig, const wght RHS_orig, const std::vector<VeriPB::Lit> &lits, const wght RHS, bool origclause_in_coreset);
template constraintid VeriPbProofLogger::overwrite_constraint<std::vector<VeriPB::Lit>, std::vector<wght>>(const constraintid constraint_id, const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS, bool origclause_in_coreset);
template constraintid VeriPbProofLogger::overwrite_constraint<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit> &lits_orig, const std::vector<wght> &weights_orig, const wght RHS_orig, const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS, bool origclause_in_coreset);
template constraintid VeriPbProofLogger::overwrite_constraint<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &lits_orig, const std::vector<VeriPB::Lit> &lits, bool origclause_in_coreset);
template void VeriPbProofLogger::move_to_coreset<std::vector<VeriPB::Lit>>(std::vector<VeriPB::Lit>& lits, wght RHS=1);
template void VeriPbProofLogger::move_to_coreset<std::vector<VeriPB::Lit>, std::vector<wght>>(std::vector<VeriPB::Lit>& lits, std::vector<wght>& wghts, wght RHS=1);


template void VeriPbProofLogger::set_objective<std::vector<uint32_t>, std::vector<wght>>(const std::vector<uint32_t> &lits, const std::vector<wght> &weights, wght constant_cost);
template void VeriPbProofLogger::add_objective_literal<uint32_t>(uint32_t lit, wght weight);
template bool VeriPbProofLogger::is_aux_var<uint32_t>(const uint32_t &var);
template std::string VeriPbProofLogger::var_name<uint32_t>(const uint32_t &var);
template void VeriPbProofLogger::write_weighted_literal<uint32_t>(const uint32_t &literal, wght weight);
template std::string VeriPbProofLogger::to_string<uint32_t>(const uint32_t &lit);
template void VeriPbProofLogger::write_literal<uint32_t>(const uint32_t &lit);
template void VeriPbProofLogger::write_cardinality_constraint<std::vector<uint32_t>>(const std::vector<uint32_t> &lits, const wght RHS);

template void VeriPbProofLogger::write_clause<std::vector<uint32_t>>(const std::vector<uint32_t> &clause);
template void VeriPbProofLogger::write_PB_constraint<std::vector<uint32_t>, std::vector<wght>>(const std::vector<uint32_t> &lits, const std::vector<wght> &weights, const wght RHS);
template void VeriPbProofLogger::rewrite_variable_by_literal<uint32_t, uint32_t>(const uint32_t& var, const uint32_t& lit);
template void VeriPbProofLogger::store_meaningful_name<uint32_t>(const uint32_t &var, const std::string &name);
template void VeriPbProofLogger::delete_meaningful_name<uint32_t>(const uint32_t &var);
template void VeriPbProofLogger::equals_rule<std::vector<uint32_t>>(const constraintid constraint_id, const std::vector<uint32_t> &lits, const wght RHS);
template void VeriPbProofLogger::write_PB_constraint<std::vector<uint32_t>, std::vector<wght>>(const std::vector<uint32_t>& lits_greater, const std::vector<wght>& weights_greater, const wght const_greater, const std::vector<uint32_t>& lits_smaller, const std::vector<wght>& weights_smaller, const wght const_smaller );
template void VeriPbProofLogger::equals_rule<std::vector<uint32_t>, std::vector<wght>>(const constraintid constraint_id, const std::vector<uint32_t> &lits, const std::vector<wght> &weights, const wght RHS);
template void VeriPbProofLogger::equals_rule<std::vector<uint32_t>, std::vector<wght>>(const constraintid constraint_id, const std::vector<uint32_t>& lits_greater, const std::vector<wght>& weights_greater, const wght const_greater, const std::vector<uint32_t>& lits_smaller, const std::vector<wght>& weights_smaller, const wght const_smaller  );
template void VeriPbProofLogger::check_last_constraint<std::vector<uint32_t>>(const std::vector<uint32_t> &lits, const wght RHS);
template void VeriPbProofLogger::check_last_constraint<std::vector<uint32_t>, std::vector<wght>>(const std::vector<uint32_t> &lits, const std::vector<wght> &weights, const wght RHS);
template void VeriPbProofLogger::check_last_constraint<std::vector<uint32_t>, std::vector<wght>>(const std::vector<uint32_t>& lits_greater, const std::vector<wght>& weights_greater, const wght const_greater, const std::vector<uint32_t>& lits_smaller, const std::vector<wght>& weights_smaller, const wght const_smaller  );
template wght VeriPbProofLogger::calculate_objective_value<std::vector<uint32_t>>(const std::vector<uint32_t> &model);
template constraintid VeriPbProofLogger::log_solution<std::vector<uint32_t>>(const std::vector<uint32_t> &model, wght objective_value);
template constraintid VeriPbProofLogger::log_solution_with_check<std::vector<uint32_t>>(const std::vector<uint32_t> &model);
template constraintid VeriPbProofLogger::unchecked_assumption<std::vector<uint32_t>>(const std::vector<uint32_t> &lits, const wght RHS);
template constraintid VeriPbProofLogger::unchecked_assumption<std::vector<uint32_t>, std::vector<wght>>(const std::vector<uint32_t> &lits, const std::vector<wght> &weights, const wght RHS);
template constraintid VeriPbProofLogger::rup<std::vector<uint32_t>>(const std::vector<uint32_t> &lits, const wght RHS);
template constraintid VeriPbProofLogger::rup<std::vector<uint32_t>, std::vector<wght>>(const std::vector<uint32_t> &lits, const std::vector<wght> &weights, const wght RHS);
template void VeriPbProofLogger::write_witness<uint32_t>(const substitution<uint32_t> &witness);
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<std::vector<uint32_t>, uint32_t>(const std::vector<uint32_t> &lits, const wght RHS, const substitution<uint32_t> &witness);
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<std::vector<uint32_t>, std::vector<wght>, uint32_t>(const std::vector<uint32_t> &lits, const std::vector<wght> &weights, const wght RHS, const substitution<uint32_t> &witness);
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<std::vector<uint32_t>, std::vector<wght>, uint32_t>(const std::vector<uint32_t> &lits, const std::vector<wght> &weights, const wght RHS, const substitution<uint32_t> &witness, std::vector<subproof> subproofs);
template constraintid VeriPbProofLogger::reificationLiteralRightImpl<std::vector<uint32_t>, std::vector<wght>, uint32_t>(const uint32_t& lit, const std::vector<uint32_t> &lits, const std::vector<wght> &weights, const wght RHS, bool store_reified_constraint);
template constraintid VeriPbProofLogger::reificationLiteralRightImpl<std::vector<uint32_t>, uint32_t>(const uint32_t& lit, const std::vector<uint32_t> &lits, const wght RHS, bool store_reified_constraint);
template constraintid VeriPbProofLogger::reificationLiteralLeftImpl<std::vector<uint32_t>, std::vector<wght>, uint32_t>(const uint32_t& lit, const std::vector<uint32_t> &lits, const std::vector<wght> &weights, const wght RHS, bool store_reified_constraint);
template constraintid VeriPbProofLogger::reificationLiteralLeftImpl<std::vector<uint32_t>, uint32_t>(const uint32_t& lit, const std::vector<uint32_t> &lits, const wght RHS, bool store_reified_constraint);
template constraintid VeriPbProofLogger::getReifiedConstraintLeftImpl<uint32_t>(const uint32_t& var);
template constraintid VeriPbProofLogger::getReifiedConstraintRightImpl<uint32_t>(const uint32_t& var);
template void VeriPbProofLogger::setReifiedConstraintLeftImpl<uint32_t>(const uint32_t& var, constraintid cxnId);
template void VeriPbProofLogger::setReifiedConstraintRightImpl<uint32_t>(const uint32_t& var, constraintid cxnId);
template void VeriPbProofLogger::deleteReifiedConstraintLeftImpl<uint32_t>(const uint32_t& var);
template void VeriPbProofLogger::deleteReifiedConstraintRightImpl<uint32_t>(const uint32_t& var);
template void VeriPbProofLogger::removeReifiedConstraintRightImplFromConstraintStore<uint32_t>(const uint32_t& var);
template void VeriPbProofLogger::removeReifiedConstraintLeftImplFromConstraintStore<uint32_t>(const uint32_t& var);
template cuttingplanes_derivation VeriPbProofLogger::CP_literal_axiom<uint32_t>(const uint32_t& lit); 
template cuttingplanes_derivation VeriPbProofLogger::CP_weakening<uint32_t>(const cuttingplanes_derivation& cp, const uint32_t& var);
template cuttingplanes_derivation VeriPbProofLogger::CP_weakening<uint32_t>(const uint32_t& var);
template cuttingplanes_derivation VeriPbProofLogger::CP_weakening<uint32_t>(const cuttingplanes_derivation& cp, const uint32_t& lit, const wght& weight);
template cuttingplanes_derivation VeriPbProofLogger::CP_weakening<uint32_t>(const uint32_t& lit, const wght& weight);
template void VeriPbProofLogger::start_CP_derivation_with_lit_axiom<uint32_t>(const uint32_t &lit);
template void VeriPbProofLogger::CP_weaken<uint32_t>(const uint32_t &var);
template void VeriPbProofLogger::CP_add_literal_axiom<uint32_t>(const uint32_t &lit);
template void VeriPbProofLogger::CP_write_literal_axiom<uint32_t>(const uint32_t &lit);
template constraintid VeriPbProofLogger::prove_by_contradiction<std::vector<uint32_t>, std::vector<wght>>(std::vector<uint32_t>& lits, std::vector<wght>& weights, wght RHS, std::vector<cuttingplanes_derivation> cpder);
template constraintid VeriPbProofLogger::prove_by_casesplitting<std::vector<uint32_t>, std::vector<wght>>(std::vector<uint32_t>& lits, std::vector<wght>& weights, wght RHS, constraintid case1, constraintid case2);
template void VeriPbProofLogger::delete_constraint<std::vector<uint32_t>>(const std::vector<uint32_t> &lits, const wght RHS);
template void VeriPbProofLogger::delete_constraint<std::vector<uint32_t>, std::vector<wght>>(const std::vector<uint32_t> &lits, const std::vector<wght> &weights, const wght RHS);
template constraintid VeriPbProofLogger::overwrite_constraint<std::vector<uint32_t>>(const constraintid constraint_id, const std::vector<uint32_t> &lits, const wght RHS, bool origclause_in_coreset);
template constraintid VeriPbProofLogger::overwrite_constraint<std::vector<uint32_t>>(const std::vector<uint32_t> &lits_orig, const wght RHS_orig, const std::vector<uint32_t> &lits, const wght RHS, bool origclause_in_coreset);
template constraintid VeriPbProofLogger::overwrite_constraint<std::vector<uint32_t>, std::vector<wght>>(const constraintid constraint_id, const std::vector<uint32_t> &lits, const std::vector<wght> &weights, const wght RHS, bool origclause_in_coreset);
template constraintid VeriPbProofLogger::overwrite_constraint<std::vector<uint32_t>, std::vector<wght>>(const std::vector<uint32_t> &lits_orig, const std::vector<wght> &weights_orig, const wght RHS_orig, const std::vector<uint32_t> &lits, const std::vector<wght> &weights, const wght RHS, bool origclause_in_coreset);
template constraintid VeriPbProofLogger::overwrite_constraint<std::vector<uint32_t>>(const std::vector<uint32_t> &lits_orig, const std::vector<uint32_t> &lits, bool origclause_in_coreset);
template void VeriPbProofLogger::move_to_coreset<std::vector<uint32_t>>(std::vector<uint32_t>& lits, wght RHS=1);
template void VeriPbProofLogger::move_to_coreset<std::vector<uint32_t>, std::vector<wght>>(std::vector<uint32_t>& lits, std::vector<wght>& wghts, wght RHS=1);

template void VeriPbProofLogger::set_objective<std::vector<int>, std::vector<wght>>(const std::vector<int> &lits, const std::vector<wght> &weights, wght constant_cost);
template void VeriPbProofLogger::add_objective_literal<int>(int lit, wght weight);
template bool VeriPbProofLogger::is_aux_var<int>(const int &var);
template std::string VeriPbProofLogger::var_name<int>(const int &var);
template void VeriPbProofLogger::write_weighted_literal<int>(const int &literal, wght weight);
template std::string VeriPbProofLogger::to_string<int>(const int &lit);
template void VeriPbProofLogger::write_literal<int>(const int &lit);
template void VeriPbProofLogger::write_cardinality_constraint<std::vector<int>>(const std::vector<int> &lits, const wght RHS);

template void VeriPbProofLogger::write_clause<std::vector<int>>(const std::vector<int> &clause);
template void VeriPbProofLogger::write_PB_constraint<std::vector<int>, std::vector<wght>>(const std::vector<int> &lits, const std::vector<wght> &weights, const wght RHS);
template void VeriPbProofLogger::rewrite_variable_by_literal<int, int>(const int& var, const int& lit);
template void VeriPbProofLogger::store_meaningful_name<int>(const int &var, const std::string &name);
template void VeriPbProofLogger::delete_meaningful_name<int>(const int &var);
template void VeriPbProofLogger::equals_rule<std::vector<int>>(const constraintid constraint_id, const std::vector<int> &lits, const wght RHS);
template void VeriPbProofLogger::write_PB_constraint<std::vector<int>, std::vector<wght>>(const std::vector<int>& lits_greater, const std::vector<wght>& weights_greater, const wght const_greater, const std::vector<int>& lits_smaller, const std::vector<wght>& weights_smaller, const wght const_smaller );
template void VeriPbProofLogger::equals_rule<std::vector<int>, std::vector<wght>>(const constraintid constraint_id, const std::vector<int> &lits, const std::vector<wght> &weights, const wght RHS);
template void VeriPbProofLogger::equals_rule<std::vector<int>, std::vector<wght>>(const constraintid constraint_id, const std::vector<int>& lits_greater, const std::vector<wght>& weights_greater, const wght const_greater, const std::vector<int>& lits_smaller, const std::vector<wght>& weights_smaller, const wght const_smaller  );
template void VeriPbProofLogger::check_last_constraint<std::vector<int>>(const std::vector<int> &lits, const wght RHS);
template void VeriPbProofLogger::check_last_constraint<std::vector<int>, std::vector<wght>>(const std::vector<int> &lits, const std::vector<wght> &weights, const wght RHS);
template void VeriPbProofLogger::check_last_constraint<std::vector<int>, std::vector<wght>>(const std::vector<int>& lits_greater, const std::vector<wght>& weights_greater, const wght const_greater, const std::vector<int>& lits_smaller, const std::vector<wght>& weights_smaller, const wght const_smaller  );
template wght VeriPbProofLogger::calculate_objective_value<std::vector<int>>(const std::vector<int> &model);
template constraintid VeriPbProofLogger::log_solution<std::vector<int>>(const std::vector<int> &model, wght objective_value);
template constraintid VeriPbProofLogger::log_solution_with_check<std::vector<int>>(const std::vector<int> &model);
template constraintid VeriPbProofLogger::unchecked_assumption<std::vector<int>>(const std::vector<int> &lits, const wght RHS);
template constraintid VeriPbProofLogger::unchecked_assumption<std::vector<int>, std::vector<wght>>(const std::vector<int> &lits, const std::vector<wght> &weights, const wght RHS);
template constraintid VeriPbProofLogger::rup<std::vector<int>>(const std::vector<int> &lits, const wght RHS);
template constraintid VeriPbProofLogger::rup<std::vector<int>, std::vector<wght>>(const std::vector<int> &lits, const std::vector<wght> &weights, const wght RHS);
template void VeriPbProofLogger::write_witness<int>(const substitution<int> &witness);
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<std::vector<int>, int>(const std::vector<int> &lits, const wght RHS, const substitution<int> &witness);
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<std::vector<int>, std::vector<wght>, int>(const std::vector<int> &lits, const std::vector<wght> &weights, const wght RHS, const substitution<int> &witness);
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<std::vector<int>, std::vector<wght>, int>(const std::vector<int> &lits, const std::vector<wght> &weights, const wght RHS, const substitution<int> &witness, std::vector<subproof> subproofs);
template constraintid VeriPbProofLogger::reificationLiteralRightImpl<std::vector<int>, std::vector<wght>, int>(const int& lit, const std::vector<int> &lits, const std::vector<wght> &weights, const wght RHS, bool store_reified_constraint);
template constraintid VeriPbProofLogger::reificationLiteralRightImpl<std::vector<int>, int>(const int& lit, const std::vector<int> &lits, const wght RHS, bool store_reified_constraint);
template constraintid VeriPbProofLogger::reificationLiteralLeftImpl<std::vector<int>, std::vector<wght>, int>(const int& lit, const std::vector<int> &lits, const std::vector<wght> &weights, const wght RHS, bool store_reified_constraint);
template constraintid VeriPbProofLogger::reificationLiteralLeftImpl<std::vector<int>, int>(const int& lit, const std::vector<int> &lits, const wght RHS, bool store_reified_constraint);
template constraintid VeriPbProofLogger::getReifiedConstraintLeftImpl<int>(const int& var);
template constraintid VeriPbProofLogger::getReifiedConstraintRightImpl<int>(const int& var);
template void VeriPbProofLogger::setReifiedConstraintLeftImpl<int>(const int& var, constraintid cxnId);
template void VeriPbProofLogger::setReifiedConstraintRightImpl<int>(const int& var, constraintid cxnId);
template void VeriPbProofLogger::deleteReifiedConstraintLeftImpl<int>(const int& var);
template void VeriPbProofLogger::deleteReifiedConstraintRightImpl<int>(const int& var);
template void VeriPbProofLogger::removeReifiedConstraintRightImplFromConstraintStore<int>(const int& var);
template void VeriPbProofLogger::removeReifiedConstraintLeftImplFromConstraintStore<int>(const int& var);
template cuttingplanes_derivation VeriPbProofLogger::CP_literal_axiom<int>(const int& lit); 
template cuttingplanes_derivation VeriPbProofLogger::CP_weakening<int>(const cuttingplanes_derivation& cp, const int& var);
template cuttingplanes_derivation VeriPbProofLogger::CP_weakening<int>(const int& var);
template cuttingplanes_derivation VeriPbProofLogger::CP_weakening<int>(const cuttingplanes_derivation& cp, const int& lit, const wght& weight);
template cuttingplanes_derivation VeriPbProofLogger::CP_weakening<int>(const int& lit, const wght& weight);
template void VeriPbProofLogger::start_CP_derivation_with_lit_axiom<int>(const int &lit);
template void VeriPbProofLogger::CP_weaken<int>(const int &var);
template void VeriPbProofLogger::CP_add_literal_axiom<int>(const int &lit);
template void VeriPbProofLogger::CP_write_literal_axiom<int>(const int &lit);
template constraintid VeriPbProofLogger::prove_by_contradiction<std::vector<int>, std::vector<wght>>(std::vector<int>& lits, std::vector<wght>& weights, wght RHS, std::vector<cuttingplanes_derivation> cpder);
template constraintid VeriPbProofLogger::prove_by_casesplitting<std::vector<int>, std::vector<wght>>(std::vector<int>& lits, std::vector<wght>& weights, wght RHS, constraintid case1, constraintid case2);
template void VeriPbProofLogger::delete_constraint<std::vector<int>>(const std::vector<int> &lits, const wght RHS);
template void VeriPbProofLogger::delete_constraint<std::vector<int>, std::vector<wght>>(const std::vector<int> &lits, const std::vector<wght> &weights, const wght RHS);
template constraintid VeriPbProofLogger::overwrite_constraint<std::vector<int>>(const constraintid constraint_id, const std::vector<int> &lits, const wght RHS, bool origclause_in_coreset);
template constraintid VeriPbProofLogger::overwrite_constraint<std::vector<int>>(const std::vector<int> &lits_orig, const wght RHS_orig, const std::vector<int> &lits, const wght RHS, bool origclause_in_coreset);
template constraintid VeriPbProofLogger::overwrite_constraint<std::vector<int>, std::vector<wght>>(const constraintid constraint_id, const std::vector<int> &lits, const std::vector<wght> &weights, const wght RHS, bool origclause_in_coreset);
template constraintid VeriPbProofLogger::overwrite_constraint<std::vector<int>, std::vector<wght>>(const std::vector<int> &lits_orig, const std::vector<wght> &weights_orig, const wght RHS_orig, const std::vector<int> &lits, const std::vector<wght> &weights, const wght RHS, bool origclause_in_coreset);
template constraintid VeriPbProofLogger::overwrite_constraint<std::vector<int>>(const std::vector<int> &lits_orig, const std::vector<int> &lits, bool origclause_in_coreset);
template void VeriPbProofLogger::move_to_coreset<std::vector<int>>(std::vector<int>& lits, wght RHS=1);
template void VeriPbProofLogger::move_to_coreset<std::vector<int>, std::vector<wght>>(std::vector<int>& lits, std::vector<wght>& wghts, wght RHS=1);

