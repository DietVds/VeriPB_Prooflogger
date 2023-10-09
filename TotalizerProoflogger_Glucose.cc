#include "TotalizerProoflogger.hpp"

template constraintid TotalizerProoflogger::get_PbDef_Impl_CxnId<VeriPB::Var>(VeriPB::Var& var);
template constraintid TotalizerProoflogger::get_PbDef_Impl_CxnId<Glucose::Var>(Glucose::Var& var);

template constraintid TotalizerProoflogger::get_PbDef_invImpl_CxnId<VeriPB::Var>(VeriPB::Var& var);
template constraintid TotalizerProoflogger::get_PbDef_invImpl_CxnId<Glucose::Var>(Glucose::Var& var);

template void TotalizerProoflogger::store_meaningful_name_counting_var<VeriPB::Var, std::vector<VeriPB::Lit>>(const VeriPB::Var& var, const wght n, const std::vector<VeriPB::Lit>& leafs);

template void TotalizerProoflogger::write_PBdefs<VeriPB::Var, std::vector<VeriPB::Lit>>(const VeriPB::Var& var, const std::vector<VeriPB::Lit> &leafs, const wght j);
template void TotalizerProoflogger::write_PBdefs<Glucose::Var, Glucose::vec<Glucose::Lit>>(const Glucose::Var& var, const Glucose::vec<Glucose::Lit> &leafs, const wght j);

template void TotalizerProoflogger::write_PBdef_impl<VeriPB::Var, std::vector<VeriPB::Lit>>(const VeriPB::Var& var, const std::vector<VeriPB::Lit> &leafs, const wght j);
template void TotalizerProoflogger::write_PBdef_impl<Glucose::Var, Glucose::vec<Glucose::Lit>>(const Glucose::Var& var, const Glucose::vec<Glucose::Lit> &leafs, const wght j);

template void TotalizerProoflogger::write_PBdef_invImpl<VeriPB::Var, std::vector<VeriPB::Lit>>(const VeriPB::Var& var, const std::vector<VeriPB::Lit> &leafs, const wght j);
template void TotalizerProoflogger::write_PBdef_invImpl<Glucose::Var, Glucose::vec<Glucose::Lit>>(const Glucose::Var& var, const Glucose::vec<Glucose::Lit> &leafs, const wght j);

template void TotalizerProoflogger::prove_binary_implCls<VeriPB::Var, std::vector<VeriPB::Lit>>(const VeriPB::Var &var, const VeriPB::Var &varchild, const std::vector<VeriPB::Lit> &leafs_varchild, const std::vector<VeriPB::Lit> &leafs_other_child, const std::vector<VeriPB::Lit>& clause_to_derive);
template void TotalizerProoflogger::prove_binary_implCls<Glucose::Var, Glucose::vec<Glucose::Lit>>(const Glucose::Var &var, const Glucose::Var &varchild, const Glucose::vec<Glucose::Lit> &leafs_varchild, const Glucose::vec<Glucose::Lit> &leafs_other_child, const Glucose::vec<Glucose::Lit>& clause_to_derive);

template void TotalizerProoflogger::prove_ternary_implCls<VeriPB::Var, std::vector<VeriPB::Lit>>(const VeriPB::Var &var, const VeriPB::Var &varchild1, const VeriPB::Var &varchild2, const std::vector<VeriPB::Lit> &leafs1, const std::vector<VeriPB::Lit> &leafs2, const std::vector<VeriPB::Lit>& clause_to_derive);
template void TotalizerProoflogger::prove_ternary_implCls<Glucose::Var, Glucose::vec<Glucose::Lit>>(const Glucose::Var &var, const Glucose::Var &varchild1, const Glucose::Var &varchild2, const Glucose::vec<Glucose::Lit> &leafs1, const Glucose::vec<Glucose::Lit> &leafs2, const Glucose::vec<Glucose::Lit>& clause_to_derive);

template void TotalizerProoflogger::prove_binary_invImplCls<VeriPB::Var, std::vector<VeriPB::Lit>>(const VeriPB::Var &var, const VeriPB::Var &varchild, const std::vector<VeriPB::Lit> &leafs_varchild, const std::vector<VeriPB::Lit> &leafs_other_child, const std::vector<VeriPB::Lit>& clause_to_derive);
template void TotalizerProoflogger::prove_binary_invImplCls<Glucose::Var, Glucose::vec<Glucose::Lit>>(const Glucose::Var &var, const Glucose::Var &varchild, const Glucose::vec<Glucose::Lit> &leafs_varchild, const Glucose::vec<Glucose::Lit> &leafs_other_child, const Glucose::vec<Glucose::Lit>& clause_to_derive);

template void TotalizerProoflogger::prove_ternary_invImplCls<VeriPB::Var, std::vector<VeriPB::Lit>>(const VeriPB::Var &var, const VeriPB::Var &varchild1, const VeriPB::Var &varchild2, const std::vector<VeriPB::Lit> &leafs1, const std::vector<VeriPB::Lit> &leafs2, const std::vector<VeriPB::Lit>& clause_to_derive);
template void TotalizerProoflogger::prove_ternary_invImplCls<Glucose::Var, Glucose::vec<Glucose::Lit>>(const Glucose::Var &var, const Glucose::Var &varchild1, const Glucose::Var &varchild2, const Glucose::vec<Glucose::Lit> &leafs1, const Glucose::vec<Glucose::Lit> &leafs2, const Glucose::vec<Glucose::Lit>& clause_to_derive);

template void TotalizerProoflogger::prove_unitclause_constraining_totalizer<VeriPB::Lit>(const VeriPB::Lit& c, const wght n, const wght bestval, const constraintid best_solution_constraint);
template void TotalizerProoflogger::prove_unitclause_constraining_totalizer<Glucose::Lit>(const Glucose::Lit& c, const wght n, const wght bestval, const constraintid best_solution_constraint);

//template void TotalizerProoflogger::ss_add_parent_using_pb_def<Glucose::Var>(const Glucose::Var& var);
template void TotalizerProoflogger::ss_add_parent_using_pb_def<VeriPB::Var>(const VeriPB::Var& var);

//template void TotalizerProoflogger::add_parent_using_pb_def<Glucose::Var>(const Glucose::Var& var, std::map<VeriPB::VarIdx, constraintid>& pb_def_store, std::map<VeriPB::VarIdx, constraintid>& pb_def_parents_store);
template void TotalizerProoflogger::add_parent_using_pb_def<VeriPB::Var>(const VeriPB::Var& var, std::map<VeriPB::VarIdx, constraintid>& pb_def_store, std::map<VeriPB::VarIdx, constraintid>& pb_def_parents_store);

template void TotalizerProoflogger::ss_set_nr_parents_using_pb_def<VeriPB::Var>(const VeriPB::Var& var, int n);
//template void TotalizerProoflogger::ss_set_nr_parents_using_pb_def<Glucose::Var>(const Glucose::Var& var, int n);

template int TotalizerProoflogger::ss_get_nr_parents_using_pb_def<VeriPB::Var>(const VeriPB::Var& var);
//template int TotalizerProoflogger::ss_get_nr_parents_using_pb_def<Glucose::Var>(const Glucose::Var& var);

template void TotalizerProoflogger::delete_PB_definitions<VeriPB::Var>(const VeriPB::Var& var);
//template void TotalizerProoflogger::delete_PB_definitions<Glucose::Var>(const Glucose::Var& var);

//template void TotalizerProoflogger::delete_PB_Impl_def<Glucose::Var>(const Glucose::Var &var);
template void TotalizerProoflogger::delete_PB_Impl_def<VeriPB::Var>(const VeriPB::Var &var);

//template void TotalizerProoflogger::delete_PB_invImpl_def<Glucose::Var>(const Glucose::Var &var);
template void TotalizerProoflogger::delete_PB_invImpl_def<VeriPB::Var>(const VeriPB::Var &var);

//template void TotalizerProoflogger::delete_P_definition<Glucose::Var>(const Glucose::Var& var, std::map<VeriPB::VarIdx, constraintid>& pb_cxn_store, std::map<VeriPB::VarIdx, int>& pb_nrUsed_store);
template void TotalizerProoflogger::delete_P_definition<VeriPB::Var>(const VeriPB::Var& var, std::map<VeriPB::VarIdx, constraintid>& pb_cxn_store, std::map<VeriPB::VarIdx, int>& pb_nrUsed_store);

//template void TotalizerProoflogger::delete_unnecessary_PBdefs_childnodes<Glucose::Var>(const std::vector<Glucose::Var>& av, const std::vector<Glucose::Var>& bv, const int parent_nr_lits_current, const int parent_nr_lits_previous, const bool iterative_encoding_needed);
template void TotalizerProoflogger::delete_unnecessary_PBdefs_childnodes<VeriPB::Var>(const std::vector<VeriPB::Var>& av, const std::vector<VeriPB::Var>& bv, const int parent_nr_lits_current, const int parent_nr_lits_previous, const bool iterative_encoding_needed);
