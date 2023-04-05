#include "SortingNetworkProoflogger.hpp"

// Constructor
template SortingNetworkProoflogger::SortingNetworkProoflogger<Glucose::Lit>(VeriPbProofLogger *PL, Glucose::Lit zero, constraintid def_one);


// Recursive sorting networks
template constraintid SortingNetworkProoflogger::derive_UB_constraint_recursive_sortingnetwork<Glucose::vec<Glucose::Lit>>(constraintid& cxnUB, Glucose::vec<Glucose::Lit>& blockings_recursive_network, Glucose::vec<Glucose::Lit>& blockings_other_network, int UBproof );
template void SortingNetworkProoflogger::derive_UB_for_recursive_sortingnetwork<Glucose::vec<Glucose::Lit>>(ConstraintStoreSort& plcxn, ConstraintStoreSort& plcxn_recursive,  Glucose::vec<Glucose::Lit>& input_recursive_network, Glucose::vec<Glucose::Lit>& input_other_network);

template constraintid SortingNetworkProoflogger::derive_UB_constraint_on_mergenetwork_input<Glucose::vec<Glucose::Lit>>(constraintid cxnUB, Glucose::vec<Glucose::Lit>& left_recursive_sort_output, constraintid input_geq_output_left, Glucose::vec<Glucose::Lit>& right_recursive_sort_output, constraintid input_geq_output_right, int UBproof );
template constraintid SortingNetworkProoflogger::derive_input_geq_output_sortingnetwork<Glucose::vec<Glucose::Lit>>(constraintid merge_input_geq_output, constraintid input_geq_output_left_recursive_sort, constraintid input_geq_output_right_recursive_sort, Glucose::vec<Glucose::Lit>& inputliterals, Glucose::vec<Glucose::Lit>& outputliterals);
template constraintid SortingNetworkProoflogger::derive_input_leq_output_sortingnetwork<Glucose::vec<Glucose::Lit>>(constraintid merge_input_leq_output, constraintid input_leq_output_left_recursive_sort, constraintid input_leq_output_right_recursive_sort, Glucose::vec<Glucose::Lit>& inputliterals, Glucose::vec<Glucose::Lit>& outputliterals);

// Merge networks

template constraintid SortingNetworkProoflogger::derive_evens_leq_odds<Glucose::vec<Glucose::Lit>, std::vector<constraintid>>(Glucose::vec<Glucose::Lit>& lits, std::vector<constraintid>& sortedness_lits);
template constraintid SortingNetworkProoflogger::derive_odds_leq_evens_plus_1<Glucose::vec<Glucose::Lit>, std::vector<constraintid>>(Glucose::vec<Glucose::Lit>& lits, std::vector<constraintid>& sortedness_lits);

template constraintid SortingNetworkProoflogger::derive_counting_definition_of_outputvars_right<Glucose::vec<Glucose::Lit>, std::vector<constraintid>>(Glucose::vec<Glucose::Lit>& inputlits, Glucose::vec<Glucose::Lit>& outputlits, int j, constraintid output_geq_input, std::vector<constraintid>& sortedness_outputlits);
template constraintid SortingNetworkProoflogger::derive_counting_definition_of_outputvars_left<Glucose::vec<Glucose::Lit>, std::vector<constraintid>>(Glucose::vec<Glucose::Lit>& inputlits, Glucose::vec<Glucose::Lit>& outputlits, int j, constraintid output_geq_input, std::vector<constraintid>& sortedness_outputlits);

template constraintid SortingNetworkProoflogger::derive_evens_leq_odds_merge_input<Glucose::vec<Glucose::Lit>>(constraintid left_evens_leq_odd, constraintid right_evens_leq_odd, Glucose::vec<Glucose::Lit>& leftlits, Glucose::vec<Glucose::Lit>& rightlits );
template constraintid SortingNetworkProoflogger::derive_odds_leq_evens_plus_2_merge_input<Glucose::vec<Glucose::Lit>>(constraintid left_odds_leq_evens_plus1, constraintid right_odds_leq_evens_plus1, Glucose::vec<Glucose::Lit>& leftlits, Glucose::vec<Glucose::Lit>& rightlits );


template constraintid SortingNetworkProoflogger::derive_sortedness_by_rup<Glucose::Lit>(Glucose::Lit& smaller_lit, Glucose::Lit& greater_lit);
