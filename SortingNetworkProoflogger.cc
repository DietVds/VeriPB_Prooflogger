#include "SortingNetworkProoflogger.hpp"

// Constructor
template SortingNetworkProoflogger::SortingNetworkProoflogger<Glucose::Lit>(VeriPbProofLogger *PL, Glucose::Lit zero, constraintid def_one);


// Recursive sorting networks
template void SortingNetworkProoflogger::derive_UB_for_recursive_sortingnetwork<Glucose::vec<Glucose::Lit>>(ConstraintStoreSort& plcxns, ConstraintStoreSort& plcxns_recursive,  Glucose::vec<Glucose::Lit>& input_recursive_network, Glucose::vec<Glucose::Lit>& input_other_network);
template void SortingNetworkProoflogger::derive_UB_for_mergenetwork<Glucose::vec<Glucose::Lit>>(ConstraintStoreMerge& plcxns_merge, ConstraintStoreSort& plcxns, ConstraintStoreSort& plcxns_left, ConstraintStoreSort& plcxns_right, Glucose::vec<Glucose::Lit>& left_output, Glucose::vec<Glucose::Lit>& right_output );
template void SortingNetworkProoflogger::derive_input_equals_output_sortingnetwork<Glucose::vec<Glucose::Lit>>(ConstraintStoreSort& plcxns, ConstraintStoreSort& plcxns_left, ConstraintStoreSort& plcxns_right, ConstraintStoreMerge& plcxns_merge, Glucose::vec<Glucose::Lit>& inputliterals, Glucose::vec<Glucose::Lit>& outputliterals);

// Merge networks

template void SortingNetworkProoflogger::derive_sortedness_odds<Glucose::vec<Glucose::Lit>>(Glucose::vec<Glucose::Lit>& literals, std::vector<constraintid>& sortedness_literals, std::vector<constraintid>& sortedness_odds);
template void SortingNetworkProoflogger::derive_sortedness_evens<Glucose::vec<Glucose::Lit>>(Glucose::vec<Glucose::Lit>& literals, std::vector<constraintid>& sortedness_literals, std::vector<constraintid>& sortedness_evens);

template void SortingNetworkProoflogger::derive_sortedness_recursive_mergenetworks<Glucose::vec<Glucose::Lit>>(ConstraintStoreMerge& plcxns, ConstraintStoreMerge& plcxns_evens, ConstraintStoreMerge& plcxns_odds, Glucose::vec<Glucose::Lit>& inputA, Glucose::vec<Glucose::Lit>& inputB);

template void SortingNetworkProoflogger::derive_UB_for_recursive_mergenetworks<Glucose::vec<Glucose::Lit>>(ConstraintStoreMerge& plcxns, ConstraintStoreMerge& plcxns_evens, ConstraintStoreMerge& plcxns_odds, Glucose::vec<Glucose::Lit>& inputA, Glucose::vec<Glucose::Lit>& inputB);

template void SortingNetworkProoflogger::derive_input_equals_output_mergenetwork<Glucose::vec<Glucose::Lit>>(ConstraintStoreMerge& plcxns, ConstraintStoreMerge& plcxns_evens, ConstraintStoreMerge& plcxns_odds, Glucose::vec<Glucose::Lit>& inputA, Glucose::vec<Glucose::Lit>& inputB, Glucose::vec<Glucose::Lit>& outputs);


template constraintid SortingNetworkProoflogger::derive_evens_leq_odds<Glucose::vec<Glucose::Lit>, std::vector<constraintid>>(Glucose::vec<Glucose::Lit>& lits, std::vector<constraintid>& sortedness_lits);
template constraintid SortingNetworkProoflogger::derive_odds_leq_evens_plus_1<Glucose::vec<Glucose::Lit>, std::vector<constraintid>>(Glucose::vec<Glucose::Lit>& lits, std::vector<constraintid>& sortedness_lits);

template constraintid SortingNetworkProoflogger::derive_counting_definition_of_outputvars_right<Glucose::vec<Glucose::Lit>, std::vector<constraintid>>(Glucose::vec<Glucose::Lit>& inputlits, Glucose::vec<Glucose::Lit>& outputlits, int j, constraintid output_geq_input, std::vector<constraintid>& sortedness_outputlits);
template constraintid SortingNetworkProoflogger::derive_counting_definition_of_outputvars_left<Glucose::vec<Glucose::Lit>, std::vector<constraintid>>(Glucose::vec<Glucose::Lit>& inputlits, Glucose::vec<Glucose::Lit>& outputlits, int j, constraintid output_geq_input, std::vector<constraintid>& sortedness_outputlits);

template constraintid SortingNetworkProoflogger::derive_evens_leq_odds_merge_input<Glucose::vec<Glucose::Lit>>(constraintid left_evens_leq_odd, constraintid right_evens_leq_odd, Glucose::vec<Glucose::Lit>& leftlits, Glucose::vec<Glucose::Lit>& rightlits );
template constraintid SortingNetworkProoflogger::derive_odds_leq_evens_plus_2_merge_input<Glucose::vec<Glucose::Lit>>(constraintid left_odds_leq_evens_plus1, constraintid right_odds_leq_evens_plus1, Glucose::vec<Glucose::Lit>& leftlits, Glucose::vec<Glucose::Lit>& rightlits );


template constraintid SortingNetworkProoflogger::derive_sortedness_by_rup<Glucose::Lit>(Glucose::Lit& smaller_lit, Glucose::Lit& greater_lit);


template std::string SortingNetworkProoflogger::sequence_to_string<Glucose::vec<Glucose::Lit>>(Glucose::vec<Glucose::Lit>& lits); 