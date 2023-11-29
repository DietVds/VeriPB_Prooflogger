#include "MaxSATProoflogger.hpp"

template void MaxSATProoflogger::add_blocking_literal<VeriPB::Lit>(VeriPB::Lit lit, constraintid cxn_id);
template void MaxSATProoflogger::add_blocking_literal<Minisat::Lit>(Minisat::Lit lit, constraintid cxn_id);

template constraintid MaxSATProoflogger::add_unit_clause_blocking_literal<VeriPB::Lit>(VeriPB::Lit blocking_lit, constraintid cxn_id, VeriPB::Lit unitclause);
template constraintid MaxSATProoflogger::add_unit_clause_blocking_literal<Minisat::Lit>(Minisat::Lit blocking_lit, constraintid cxn_id, Minisat::Lit unitclause);

template constraintid MaxSATProoflogger::add_core_lower_bound<VeriPB::Var>(const VeriPB::Var &lazy_var, constraintid core_id, constraintid pb_definition_id, wght weight);
template constraintid MaxSATProoflogger::add_core_lower_bound<Minisat::Var>(const Minisat::Var &lazy_var, constraintid core_id, constraintid pb_definition_id, wght weight);

template constraintid MaxSATProoflogger::update_core_lower_bound<VeriPB::Var>(const VeriPB::Var &old_lazy_var, const VeriPB::Var &new_lazy_var, constraintid pb_definition_id, wght bound);
template constraintid MaxSATProoflogger::update_core_lower_bound<Minisat::Var>(const Minisat::Var &old_lazy_var, const Minisat::Var &new_lazy_var, constraintid pb_definition_id, wght bound);

template constraintid MaxSATProoflogger::derive_at_most_one_constraint<std::vector<VeriPB::Lit>>(const std::vector<VeriPB::Lit> &am1_lits);
template constraintid MaxSATProoflogger::derive_at_most_one_constraint<Minisat::vec<Minisat::Lit>>(const Minisat::vec<Minisat::Lit> &am1_lits);

template constraintid MaxSATProoflogger::introduce_at_most_one_selector<std::vector<VeriPB::Lit>,VeriPB::Lit>(const std::vector<VeriPB::Lit> &am1_lits, const VeriPB::Lit &select_all_lit);
template constraintid MaxSATProoflogger::introduce_at_most_one_selector<Minisat::vec<Minisat::Lit>, Minisat::Lit>(const Minisat::vec<Minisat::Lit> &am1_lits, const Minisat::Lit &select_all_lit);

template constraintid MaxSATProoflogger::proof_log_at_most_one<std::vector<VeriPB::Lit>, VeriPB::Lit>(constraintid base_reform_id, const class std::vector<VeriPB::Lit> &am1_lits, const VeriPB::Lit &selector_all_lit, wght weight);
template constraintid MaxSATProoflogger::proof_log_at_most_one<Minisat::vec<Minisat::Lit>, Minisat::Lit>(constraintid base_reform_id, const Minisat::vec<Minisat::Lit> &am1_lits, const Minisat::Lit &select_all_lit, wght weight);
