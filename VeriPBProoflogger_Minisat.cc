#include "VeriPBProoflogger.hpp"

//=================================================================================================

template void VeriPbProofLogger::set_objective<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, wght constant_cost);
template void VeriPbProofLogger::set_objective<Minisat::vec<Minisat::Lit>, Minisat::vec<wght>>(const Minisat::vec<Minisat::Lit> &lits, const Minisat::vec<wght> &weights, wght constant_cost);

template void VeriPbProofLogger::add_objective_literal<VeriPB::Lit>(VeriPB::Lit& lit, wght weight);
template void VeriPbProofLogger::add_objective_literal<Minisat::Lit>(Minisat::Lit& lit, wght weight);

template bool VeriPbProofLogger::remove_objective_literal<Minisat::Lit>(Minisat::Lit& lit);
template bool VeriPbProofLogger::remove_objective_literal<VeriPB::Lit>(VeriPB::Lit& lit);

template void VeriPbProofLogger::write_objective_update_diff<std::vector<VeriPB::Lit>, std::vector<signedWght>>(std::vector<VeriPB::Lit>& litsOnewminusold, std::vector<signedWght>& wghtsOnewminusold, signedWght constantOnewminusold = 0);
template void VeriPbProofLogger::write_objective_update_diff_for_literal<VeriPB::Lit>(VeriPB::Lit& literal_to_remove, wght weight, wght constant_for_lit);

template bool VeriPbProofLogger::is_aux_var<VeriPB::Var>(const VeriPB::Var &var);
template bool VeriPbProofLogger::is_aux_var<Minisat::Var>(const Minisat::Var &var);

template std::string VeriPbProofLogger::var_name<VeriPB::Var>(const VeriPB::Var &var);
template std::string VeriPbProofLogger::var_name<Minisat::Var>(const Minisat::Var &var);

template void VeriPbProofLogger::write_weighted_literal<VeriPB::Lit>(const VeriPB::Lit &literal, wght weight);
template void VeriPbProofLogger::write_weighted_literal<Minisat::Lit>(const Minisat::Lit &literal, wght weight);

template std::string VeriPbProofLogger::to_string<VeriPB::Lit>(const VeriPB::Lit &lit);
template std::string VeriPbProofLogger::to_string<Minisat::Lit>(const Minisat::Lit &lit);

template void VeriPbProofLogger::write_literal<VeriPB::Lit>(const VeriPB::Lit &lit);
template void VeriPbProofLogger::write_literal<Minisat::Lit>(const Minisat::Lit &lit);

template void VeriPbProofLogger::write_cardinality_constraint<Minisat::vec<Minisat::Lit>>(const Minisat::vec<Minisat::Lit> &lits, const wght RHS);
template void VeriPbProofLogger::write_cardinality_constraint<Minisat::Clause>(const Minisat::Clause &lits, const wght RHS);
template void VeriPbProofLogger::write_cardinality_constraint<std::vector<Minisat::Lit>>(const std::vector<Minisat::Lit> &lits, const wght RHS);
template void VeriPbProofLogger::write_cardinality_constraint<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &lits, const wght RHS);

template void VeriPbProofLogger::write_clause<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &clause);
template void VeriPbProofLogger::write_clause<std::vector<Minisat::Lit>>(const std::vector<Minisat::Lit> &clause);
template void VeriPbProofLogger::write_clause<Minisat::vec<Minisat::Lit>>(const Minisat::vec<Minisat::Lit> &clause);
template void VeriPbProofLogger::write_clause<Minisat::Clause>(const Minisat::Clause &clause);

template void VeriPbProofLogger::write_PB_constraint<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS);
template void VeriPbProofLogger::write_PB_constraint<Minisat::vec<Minisat::Lit>, Minisat::vec<wght>>(const Minisat::vec<Minisat::Lit> &lits, const Minisat::vec<wght> &weights, const wght RHS);

template void VeriPbProofLogger::rewrite_variable_by_literal<VeriPB::Var, VeriPB::Lit>(const VeriPB::Var& var, const VeriPB::Lit& lit);
template void VeriPbProofLogger::rewrite_variable_by_literal<Minisat::Var, Minisat::Lit>(const Minisat::Var& var, const Minisat::Lit& lit);

template void VeriPbProofLogger::store_meaningful_name<VeriPB::Var>(const VeriPB::Var &var, const std::string &name);
template void VeriPbProofLogger::store_meaningful_name<Minisat::Var>(const Minisat::Var &var, const std::string &name);

template void VeriPbProofLogger::equals_rule<std::vector<VeriPB::Lit>>(const constraintid constraint_id, const std::vector<VeriPB::Lit> &lits, const wght RHS);
template void VeriPbProofLogger::equals_rule<Minisat::vec<Minisat::Lit>>(const constraintid constraint_id, const Minisat::vec<Minisat::Lit> &lits, const wght RHS);

template void VeriPbProofLogger::write_PB_constraint<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit>& lits_greater, const std::vector<wght>& weights_greater, const wght const_greater, const std::vector<VeriPB::Lit>& lits_smaller, const std::vector<wght>& weights_smaller, const wght const_smaller );
template void VeriPbProofLogger::write_PB_constraint<Minisat::vec<Minisat::Lit>, Minisat::vec<wght>>(const Minisat::vec<Minisat::Lit>& lits_greater, const Minisat::vec<wght>& weights_greater, const wght const_greater, const Minisat::vec<Minisat::Lit>& lits_smaller, const Minisat::vec<wght>& weights_smaller, const wght const_smaller );

template void VeriPbProofLogger::equals_rule<std::vector<VeriPB::Lit>, std::vector<wght>>(const constraintid constraint_id, const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS);
template void VeriPbProofLogger::equals_rule<Minisat::vec<Minisat::Lit>, Minisat::vec<wght>>(const constraintid constraint_id, const Minisat::vec<Minisat::Lit> &lits, const Minisat::vec<wght> &weights, const wght RHS);

template void VeriPbProofLogger::equals_rule<std::vector<VeriPB::Lit>, std::vector<wght>>(const constraintid constraint_id, const std::vector<VeriPB::Lit>& lits_greater, const std::vector<wght>& weights_greater, const wght const_greater, const std::vector<VeriPB::Lit>& lits_smaller, const std::vector<wght>& weights_smaller, const wght const_smaller  );
template void VeriPbProofLogger::equals_rule<Minisat::vec<Minisat::Lit>, Minisat::vec<wght>>(const constraintid constraint_id, const Minisat::vec<Minisat::Lit>& lits_greater, const Minisat::vec<wght>& weights_greater, const wght const_greater, const Minisat::vec<Minisat::Lit>& lits_smaller, const Minisat::vec<wght>& weights_smaller, const wght const_smaller  );


template void VeriPbProofLogger::check_last_constraint<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &lits, const wght RHS);
template void VeriPbProofLogger::check_last_constraint<Minisat::vec<Minisat::Lit>>(const Minisat::vec<Minisat::Lit> &lits, const wght RHS);

template void VeriPbProofLogger::check_last_constraint<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS);
template void VeriPbProofLogger::check_last_constraint<Minisat::vec<Minisat::Lit>, Minisat::vec<wght>>(const Minisat::vec<Minisat::Lit> &lits, const Minisat::vec<wght> &weights, const wght RHS);

template void VeriPbProofLogger::check_last_constraint<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit>& lits_greater, const std::vector<wght>& weights_greater, const wght const_greater, const std::vector<VeriPB::Lit>& lits_smaller, const std::vector<wght>& weights_smaller, const wght const_smaller  );
template void VeriPbProofLogger::check_last_constraint<Minisat::vec<Minisat::Lit>, Minisat::vec<wght>>(const Minisat::vec<Minisat::Lit>& lits_greater, const Minisat::vec<wght>& weights_greater, const wght const_greater, const Minisat::vec<Minisat::Lit>& lits_smaller, const Minisat::vec<wght>& weights_smaller, const wght const_smaller  );

template void VeriPbProofLogger::check_constraint_exists<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &lits, const wght RHS);
template void VeriPbProofLogger::check_constraint_exists<Minisat::vec<Minisat::Lit>>(const Minisat::vec<Minisat::Lit> &lits, const wght RHS);

template void VeriPbProofLogger::check_constraint_exists<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS);
template void VeriPbProofLogger::check_constraint_exists<Minisat::vec<Minisat::Lit>, Minisat::vec<wght>>(const Minisat::vec<Minisat::Lit> &lits, const Minisat::vec<wght> &weights, const wght RHS);

template void VeriPbProofLogger::check_constraint_exists<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit>& lits_greater, const std::vector<wght>& weights_greater, const wght const_greater, const std::vector<VeriPB::Lit>& lits_smaller, const std::vector<wght>& weights_smaller, const wght const_smaller  );
template void VeriPbProofLogger::check_constraint_exists<Minisat::vec<Minisat::Lit>, Minisat::vec<wght>>(const Minisat::vec<Minisat::Lit>& lits_greater, const Minisat::vec<wght>& weights_greater, const wght const_greater, const Minisat::vec<Minisat::Lit>& lits_smaller, const Minisat::vec<wght>& weights_smaller, const wght const_smaller  );


template wght VeriPbProofLogger::calculate_objective_value<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &model);

template constraintid VeriPbProofLogger::log_solution<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &model, wght objective_value, bool only_original_variables_necessary, bool log_as_comment);
template constraintid VeriPbProofLogger::log_solution<Minisat::vec<Minisat::Lit>>(const Minisat::vec<Minisat::Lit> &model, wght objective_value, bool only_original_variables_necessary, bool log_as_comment);

template constraintid VeriPbProofLogger::log_solution_with_check<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &model, bool only_original_variables_necessary, bool log_as_comment);

template constraintid VeriPbProofLogger::log_solution_lbools<Minisat::vec<Minisat::lbool>>(Minisat::vec<Minisat::lbool> &model, wght objective_value);

template constraintid VeriPbProofLogger::unchecked_assumption<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &lits, const wght RHS);
template constraintid VeriPbProofLogger::unchecked_assumption<Minisat::vec<Minisat::Lit>>(const Minisat::vec<Minisat::Lit> &lits, const wght RHS);

template constraintid VeriPbProofLogger::unchecked_assumption<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS);
template constraintid VeriPbProofLogger::unchecked_assumption<Minisat::vec<Minisat::Lit>, Minisat::vec<wght>>(const Minisat::vec<Minisat::Lit> &lits, const Minisat::vec<wght> &weights, const wght RHS);

template constraintid VeriPbProofLogger::rup<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &lits, const wght RHS);
template constraintid VeriPbProofLogger::rup<Minisat::vec<Minisat::Lit>>(const Minisat::vec<Minisat::Lit> &lits, const wght RHS);
template constraintid VeriPbProofLogger::rup<std::vector<Minisat::Lit>>(const std::vector<Minisat::Lit> &lits, const wght RHS);
template constraintid VeriPbProofLogger::rup<Minisat::Clause>(const Minisat::Clause &lits, const wght RHS);

template constraintid VeriPbProofLogger::rup<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS);
template constraintid VeriPbProofLogger::rup<Minisat::vec<Minisat::Lit>, Minisat::vec<wght>>(const Minisat::vec<Minisat::Lit> &lits, const Minisat::vec<wght> &weights, const wght RHS);

template void VeriPbProofLogger::add_boolean_assignment<Minisat::Var>(substitution &s, const Minisat::Var& var, const bool value);
template void VeriPbProofLogger::add_boolean_assignment<VeriPB::Var>(substitution &s, const VeriPB::Var& var, const bool value);

template constraintid VeriPbProofLogger::redundanceBasedStrengthening<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &lits, const wght RHS, const substitution &witness);
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<Minisat::vec<Minisat::Lit>>(const Minisat::vec<Minisat::Lit> &lits, const wght RHS, const substitution &witness);

template constraintid VeriPbProofLogger::redundanceBasedStrengthening<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS, const substitution &witness);
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<Minisat::vec<Minisat::Lit>, Minisat::vec<wght>>(const Minisat::vec<Minisat::Lit> &lits, const Minisat::vec<wght> &weights, const wght RHS, const substitution &witness);

template constraintid VeriPbProofLogger::redundanceBasedStrengthening<Minisat::vec<Minisat::Lit>, Minisat::vec<wght>>(const Minisat::vec<Minisat::Lit> &lits, const Minisat::vec<wght> &weights, const wght RHS, const substitution &witness, std::vector<subproof> subproofs);
// template constraintid VeriPbProofLogger::redundanceBasedStrengthening<Minisat::vec<Minisat::Lit>, Minisat::vec<wght>>(const Minisat::vec<Minisat::Lit> &lits, const Minisat::vec<wght> &weights, const wght RHS, const substitution &witness, std::vector<subproof> subproofs);
template constraintid VeriPbProofLogger::redundanceBasedStrengthening<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS, const substitution &witness, std::vector<subproof> subproofs);

template constraintid VeriPbProofLogger::redundanceBasedStrengtheningUnitClause<VeriPB::Lit>(const VeriPB::Lit& lit);

template constraintid VeriPbProofLogger::reificationLiteralRightImpl<std::vector<VeriPB::Lit>, std::vector<wght>, VeriPB::Lit>(const VeriPB::Lit& lit, const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS, bool store_reified_constraint);
template constraintid VeriPbProofLogger::reificationLiteralRightImpl<std::vector<VeriPB::Lit>, std::vector<wght>, VeriPB::Lit>(const VeriPB::Lit& lit, const std::vector<VeriPB::Lit> &litsC, const std::vector<wght> &weights, const wght RHS, int start_constraint, int end_constraint, bool store_reified_constraint);

template constraintid VeriPbProofLogger::reificationLiteralRightImpl<Minisat::vec<Minisat::Lit>, Minisat::vec<wght>, Minisat::Lit>(const Minisat::Lit& lit, const Minisat::vec<Minisat::Lit> &lits, const Minisat::vec<wght> &weights, const wght RHS, bool store_reified_constraint);
template constraintid VeriPbProofLogger::reificationLiteralRightImpl<Minisat::vec<Minisat::Lit>, Minisat::vec<wght>, VeriPB::Lit>(const VeriPB::Lit& lit, const Minisat::vec<Minisat::Lit> &lits, const Minisat::vec<wght> &weights, const wght RHS, bool store_reified_constraint);

template constraintid VeriPbProofLogger::reificationLiteralRightImpl<std::vector<VeriPB::Lit>, VeriPB::Lit>(const VeriPB::Lit& lit, const std::vector<VeriPB::Lit> &lits, const wght RHS, bool store_reified_constraint);
template constraintid VeriPbProofLogger::reificationLiteralRightImpl<Minisat::vec<Minisat::Lit>, Minisat::Lit>(const Minisat::Lit& lit, const Minisat::vec<Minisat::Lit> &lits, const wght RHS, bool store_reified_constraint);
template constraintid VeriPbProofLogger::reificationLiteralRightImpl<Minisat::vec<Minisat::Lit>, VeriPB::Lit>(const VeriPB::Lit& lit, const Minisat::vec<Minisat::Lit> &lits, const wght RHS, bool store_reified_constraint);

template constraintid VeriPbProofLogger::reificationLiteralRightImplLeq<std::vector<VeriPB::Lit>, std::vector<wght>, VeriPB::Lit>(const VeriPB::Lit& lit, const std::vector<VeriPB::Lit> &litsC, const std::vector<wght> &weights, const wght RHS, int start_constraint, int end_constraint, bool store_reified_constraint);


template constraintid VeriPbProofLogger::reificationLiteralLeftImpl<std::vector<VeriPB::Lit>, std::vector<wght>, VeriPB::Lit>(const VeriPB::Lit& lit, const std::vector<VeriPB::Lit> &litsC, const std::vector<wght> &weights, const wght RHS, int start_constraint, int end_constraint, bool store_reified_constraint);
template constraintid VeriPbProofLogger::reificationLiteralLeftImpl<std::vector<VeriPB::Lit>, std::vector<wght>, VeriPB::Lit>(const VeriPB::Lit& lit, const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS, bool store_reified_constraint);
template constraintid VeriPbProofLogger::reificationLiteralLeftImpl<Minisat::vec<Minisat::Lit>, Minisat::vec<wght>, Minisat::Lit>(const Minisat::Lit& lit, const Minisat::vec<Minisat::Lit> &lits, const Minisat::vec<wght> &weights, const wght RHS, bool store_reified_constraint);
template constraintid VeriPbProofLogger::reificationLiteralLeftImpl<Minisat::vec<Minisat::Lit>, Minisat::vec<wght>, VeriPB::Lit>(const VeriPB::Lit& lit, const Minisat::vec<Minisat::Lit> &lits, const Minisat::vec<wght> &weights, const wght RHS, bool store_reified_constraint);

template constraintid VeriPbProofLogger::reificationLiteralLeftImpl<std::vector<VeriPB::Lit>, VeriPB::Lit>(const VeriPB::Lit& lit, const std::vector<VeriPB::Lit> &lits, const wght RHS, bool store_reified_constraint);
template constraintid VeriPbProofLogger::reificationLiteralLeftImpl<Minisat::vec<Minisat::Lit>, Minisat::Lit>(const Minisat::Lit& lit, const Minisat::vec<Minisat::Lit> &lits, const wght RHS, bool store_reified_constraint);
template constraintid VeriPbProofLogger::reificationLiteralLeftImpl<Minisat::vec<Minisat::Lit>, VeriPB::Lit>(const VeriPB::Lit& lit, const Minisat::vec<Minisat::Lit> &lits, const wght RHS, bool store_reified_constraint);

template constraintid VeriPbProofLogger::reificationLiteralLeftImplLeq<std::vector<VeriPB::Lit>, std::vector<wght>, VeriPB::Lit>(const VeriPB::Lit& lit, const std::vector<VeriPB::Lit> &litsC, const std::vector<wght> &weights, const wght RHS, int start_constraint, int end_constraint, bool store_reified_constraint);


template constraintid VeriPbProofLogger::getReifiedConstraintLeftImpl<Minisat::Var>(const Minisat::Var& var);
template constraintid VeriPbProofLogger::getReifiedConstraintLeftImpl<VeriPB::Var>(const VeriPB::Var& var);
template constraintid VeriPbProofLogger::getReifiedConstraintRightImpl<Minisat::Var>(const Minisat::Var& var);
template constraintid VeriPbProofLogger::getReifiedConstraintRightImpl<VeriPB::Var>(const VeriPB::Var& var);
    
template void VeriPbProofLogger::setReifiedConstraintLeftImpl<VeriPB::Var>(const VeriPB::Var& var, constraintid cxnId);
template void VeriPbProofLogger::setReifiedConstraintLeftImpl<Minisat::Var>(const Minisat::Var& var, constraintid cxnId);
template void VeriPbProofLogger::setReifiedConstraintRightImpl<VeriPB::Var>(const VeriPB::Var& var, constraintid cxnId);
template void VeriPbProofLogger::setReifiedConstraintRightImpl<Minisat::Var>(const Minisat::Var& var, constraintid cxnId);
template void VeriPbProofLogger::deleteReifiedConstraintLeftImpl<VeriPB::Var>(const VeriPB::Var& var);
template void VeriPbProofLogger::deleteReifiedConstraintLeftImpl<Minisat::Var>(const Minisat::Var& var);
template void VeriPbProofLogger::deleteReifiedConstraintRightImpl<VeriPB::Var>(const VeriPB::Var& var);
template void VeriPbProofLogger::deleteReifiedConstraintRightImpl<Minisat::Var>(const Minisat::Var& var);
template void VeriPbProofLogger::removeReifiedConstraintRightImplFromConstraintStore<VeriPB::Var>(const VeriPB::Var& var);
template void VeriPbProofLogger::removeReifiedConstraintRightImplFromConstraintStore<Minisat::Var>(const Minisat::Var& var);
template void VeriPbProofLogger::removeReifiedConstraintLeftImplFromConstraintStore<VeriPB::Var>(const VeriPB::Var& var);
template void VeriPbProofLogger::removeReifiedConstraintLeftImplFromConstraintStore<Minisat::Var>(const Minisat::Var& var);

template cuttingplanes_derivation VeriPbProofLogger::CP_literal_axiom<VeriPB::Lit>(const VeriPB::Lit& lit); 
template cuttingplanes_derivation VeriPbProofLogger::CP_literal_axiom<Minisat::Lit>(const Minisat::Lit& lit); 
template cuttingplanes_derivation VeriPbProofLogger::CP_weakening<VeriPB::Var>(const cuttingplanes_derivation& cp, const VeriPB::Var& var);
template cuttingplanes_derivation VeriPbProofLogger::CP_weakening<Minisat::Var>(const cuttingplanes_derivation& cp, const Minisat::Var& var);
template cuttingplanes_derivation VeriPbProofLogger::CP_weakening<VeriPB::Var>(const VeriPB::Var& var);
template cuttingplanes_derivation VeriPbProofLogger::CP_weakening<Minisat::Var>(const Minisat::Var& var);
template cuttingplanes_derivation VeriPbProofLogger::CP_weakening<VeriPB::Lit>(const cuttingplanes_derivation& cp, const VeriPB::Lit& lit, const wght& weight);
template cuttingplanes_derivation VeriPbProofLogger::CP_weakening<Minisat::Lit>(const cuttingplanes_derivation& cp, const Minisat::Lit& lit, const wght& weight);
template cuttingplanes_derivation VeriPbProofLogger::CP_weakening<VeriPB::Lit>(const VeriPB::Lit& lit, const wght& weight);
template cuttingplanes_derivation VeriPbProofLogger::CP_weakening<Minisat::Lit>(const Minisat::Lit& lit, const wght& weight);

template void VeriPbProofLogger::start_intCP_derivation_with_lit_axiom<VeriPB::Lit>(const VeriPB::Lit &lit);
template void VeriPbProofLogger::start_intCP_derivation_with_lit_axiom<Minisat::Lit>(const Minisat::Lit &lit);

template void VeriPbProofLogger::intCP_weaken<Minisat::Var>(const Minisat::Var &var);
template void VeriPbProofLogger::intCP_weaken<VeriPB::Var>(const VeriPB::Var &var);

template void VeriPbProofLogger::intCP_add_literal_axiom<VeriPB::Lit>(const VeriPB::Lit &lit);
template void VeriPbProofLogger::intCP_add_literal_axiom<Minisat::Lit>(const Minisat::Lit &lit);

template void VeriPbProofLogger::intCP_write_literal_axiom<VeriPB::Lit>(const VeriPB::Lit &lit);
template void VeriPbProofLogger::intCP_write_literal_axiom<Minisat::Lit>(const Minisat::Lit &lit);

template constraintid VeriPbProofLogger::prove_by_contradiction<std::vector<VeriPB::Lit>, std::vector<wght>>(std::vector<VeriPB::Lit>& lits, std::vector<wght>& weights, wght RHS, std::vector<cuttingplanes_derivation> cpder);
template constraintid VeriPbProofLogger::prove_by_contradiction<Minisat::vec<Minisat::Lit>, Minisat::vec<wght>>(Minisat::vec<Minisat::Lit>& lits, Minisat::vec<wght>& weights, wght RHS, std::vector<cuttingplanes_derivation> cpder);

template constraintid VeriPbProofLogger::prove_by_casesplitting<std::vector<VeriPB::Lit>, std::vector<wght>>(std::vector<VeriPB::Lit>& lits, std::vector<wght>& weights, wght RHS, constraintid case1, constraintid case2);
template constraintid VeriPbProofLogger::prove_by_casesplitting<Minisat::vec<Minisat::Lit>, Minisat::vec<wght>>(Minisat::vec<Minisat::Lit>& lits, Minisat::vec<wght>& weights, wght RHS, constraintid case1, constraintid case2);

template void VeriPbProofLogger::delete_constraint<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &lits, const wght RHS, bool overrule_keeporiginalformula);
template void VeriPbProofLogger::delete_constraint<Minisat::vec<VeriPB::Lit>>(const Minisat::vec<VeriPB::Lit> &lits, const wght RHS, bool overrule_keeporiginalformula);
template void VeriPbProofLogger::delete_constraint<Minisat::vec<Minisat::Lit>>(const Minisat::vec<Minisat::Lit> &lits, const wght RHS, bool overrule_keeporiginalformula);
template void VeriPbProofLogger::delete_constraint<Minisat::Clause>(const Minisat::Clause &lits, const wght RHS, bool overrule_keeporiginalformula);
template void VeriPbProofLogger::delete_constraint<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &lits, const wght RHS, const substitution& witness, bool overrule_keeporiginalformula=false);    
    

template void VeriPbProofLogger::delete_constraint<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS, bool overrule_keeporiginalformula);
template void VeriPbProofLogger::delete_constraint<Minisat::vec<Minisat::Lit>, Minisat::vec<wght>>(const Minisat::vec<Minisat::Lit> &lits, const Minisat::vec<wght> &weights, const wght RHS, bool overrule_keeporiginalformula);

template void VeriPbProofLogger::delete_clause<Minisat::vec<Minisat::Lit>>(const Minisat::vec<Minisat::Lit>& lits, bool overrule_keeporiginalformula);

template constraintid VeriPbProofLogger::overwrite_constraint<std::vector<VeriPB::Lit>>(const constraintid constraint_id, const std::vector<VeriPB::Lit> &lits, const wght RHS, bool origclause_in_coreset);
template constraintid VeriPbProofLogger::overwrite_constraint<Minisat::vec<Minisat::Lit>>(const constraintid constraint_id, const Minisat::vec<Minisat::Lit> &lits, const wght RHS, bool origclause_in_coreset);

template constraintid VeriPbProofLogger::overwrite_constraint<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &lits_orig, const wght RHS_orig, const std::vector<VeriPB::Lit> &lits, const wght RHS, bool origclause_in_coreset);
template constraintid VeriPbProofLogger::overwrite_constraint<Minisat::vec<Minisat::Lit>>(const Minisat::vec<Minisat::Lit> &lits_orig, const wght RHS_orig, const Minisat::vec<Minisat::Lit> &lits, const wght RHS, bool origclause_in_coreset);

template constraintid VeriPbProofLogger::overwrite_constraint<std::vector<VeriPB::Lit>, std::vector<wght>>(const constraintid constraint_id, const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS, bool origclause_in_coreset);
template constraintid VeriPbProofLogger::overwrite_constraint<Minisat::vec<Minisat::Lit>, Minisat::vec<wght>>(const constraintid constraint_id, const Minisat::vec<Minisat::Lit> &lits, const Minisat::vec<wght> &weights, const wght RHS, bool origclause_in_coreset);

template constraintid VeriPbProofLogger::overwrite_constraint<std::vector<VeriPB::Lit>, std::vector<wght>>(const std::vector<VeriPB::Lit> &lits_orig, const std::vector<wght> &weights_orig, const wght RHS_orig, const std::vector<VeriPB::Lit> &lits, const std::vector<wght> &weights, const wght RHS, bool origclause_in_coreset);
template constraintid VeriPbProofLogger::overwrite_constraint<Minisat::vec<Minisat::Lit>, Minisat::vec<wght>>(const Minisat::vec<Minisat::Lit> &lits_orig, const Minisat::vec<wght> &weights_orig, const wght RHS_orig, const Minisat::vec<Minisat::Lit> &lits, const Minisat::vec<wght> &weights, const wght RHS, bool origclause_in_coreset);

template constraintid VeriPbProofLogger::overwrite_constraint<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &lits_orig, const std::vector<VeriPB::Lit> &lits, bool origclause_in_coreset);
template constraintid VeriPbProofLogger::overwrite_constraint<Minisat::vec<Minisat::Lit>>(const Minisat::vec<Minisat::Lit> &lits_orig, const Minisat::vec<Minisat::Lit> &lits, bool origclause_in_coreset);

template void VeriPbProofLogger::move_to_coreset<std::vector<VeriPB::Lit>>(std::vector<VeriPB::Lit>& lits, wght RHS, bool overrule_keeporiginalformula);
template void VeriPbProofLogger::move_to_coreset<Minisat::vec<Minisat::Lit>>(Minisat::vec<Minisat::Lit>& lits, wght RHS, bool overrule_keeporiginalformula);
template void VeriPbProofLogger::move_to_coreset<std::vector<VeriPB::Lit>, std::vector<wght>>(std::vector<VeriPB::Lit>& lits, std::vector<wght>& wghts, wght RHS, bool overrule_keeporiginalformula);
template void VeriPbProofLogger::move_to_coreset<Minisat::vec<Minisat::Lit>, Minisat::vec<wght>>(Minisat::vec<Minisat::Lit>& lits, Minisat::vec<wght>& wghts, wght RHS, bool overrule_keeporiginalformula);