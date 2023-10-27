#include "MaxSATProoflogger.hpp"

template void MaxSATProoflogger::add_blocking_literal<VeriPB::Lit>(VeriPB::Lit lit, constraintid cxn_id);
template constraintid MaxSATProoflogger::add_unit_clause_blocking_literal<VeriPB::Lit>(VeriPB::Lit blocking_lit, constraintid cxn_id, VeriPB::Lit unitclause);
template constraintid MaxSATProoflogger::add_core_lower_bound<VeriPB::Var>(const VeriPB::Var &lazy_var, constraintid core_id, constraintid pb_definition_id, wght weight);
template constraintid MaxSATProoflogger::update_core_lower_bound<VeriPB::Var>(const VeriPB::Var &old_lazy_var, const VeriPB::Var &new_lazy_var, constraintid pb_definition_id, wght bound);
template constraintid MaxSATProoflogger::derive_at_most_one_constraint<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &am1_lits);
template constraintid MaxSATProoflogger::introduce_at_most_one_selector<std::vector<VeriPB::Lit>,VeriPB::Lit>(const std::vector<VeriPB::Lit> &am1_lits, const VeriPB::Lit &select_all_lit);
template constraintid MaxSATProoflogger::proof_log_at_most_one<std::vector<VeriPB::Lit>, VeriPB::Lit>(constraintid base_reform_id, const class std::vector<VeriPB::Lit> &am1_lits, const VeriPB::Lit &selector_all_lit, wght weight);

template void MaxSATProoflogger::add_blocking_literal<uint32_t>(uint32_t lit, constraintid cxn_id);
template constraintid MaxSATProoflogger::add_unit_clause_blocking_literal<uint32_t>(uint32_t blocking_lit, constraintid cxn_id, uint32_t unitclause);
template constraintid MaxSATProoflogger::add_core_lower_bound<uint32_t>(const uint32_t &lazy_var, constraintid core_id, constraintid pb_definition_id, wght weight);
template constraintid MaxSATProoflogger::update_core_lower_bound<uint32_t>(const uint32_t &old_lazy_var, const uint32_t &new_lazy_var, constraintid pb_definition_id, wght bound);
template constraintid MaxSATProoflogger::derive_at_most_one_constraint<std::vector<uint32_t>>(const std::vector<uint32_t> &am1_lits);
template constraintid MaxSATProoflogger::introduce_at_most_one_selector<std::vector<uint32_t>,uint32_t>(const std::vector<uint32_t> &am1_lits, const uint32_t &select_all_lit);
template constraintid MaxSATProoflogger::proof_log_at_most_one<std::vector<uint32_t>, uint32_t>(constraintid base_reform_id, const class std::vector<uint32_t> &am1_lits, const uint32_t &selector_all_lit, wght weight);

template void MaxSATProoflogger::add_blocking_literal<int>(int lit, constraintid cxn_id);
template constraintid MaxSATProoflogger::add_unit_clause_blocking_literal<int>(int blocking_lit, constraintid cxn_id, int unitclause);
template constraintid MaxSATProoflogger::add_core_lower_bound<int>(const int &lazy_var, constraintid core_id, constraintid pb_definition_id, wght weight);
template constraintid MaxSATProoflogger::update_core_lower_bound<int>(const int &old_lazy_var, const int &new_lazy_var, constraintid pb_definition_id, wght bound);
template constraintid MaxSATProoflogger::derive_at_most_one_constraint<std::vector<int>>(const std::vector<int> &am1_lits);
template constraintid MaxSATProoflogger::introduce_at_most_one_selector<std::vector<int>,int>(const std::vector<int> &am1_lits, const int &select_all_lit);
template constraintid MaxSATProoflogger::proof_log_at_most_one<std::vector<int>, int>(constraintid base_reform_id, const class std::vector<int> &am1_lits, const int &selector_all_lit, wght weight);
