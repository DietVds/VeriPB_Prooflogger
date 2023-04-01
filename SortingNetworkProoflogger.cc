#include "SortingNetworkProoflogger.hpp"

template constraintid SortingNetworkProoflogger::derive_evens_leq_odds<Glucose::vec<Glucose::Lit>, Glucose::vec<constraintid>>(Glucose::vec<Glucose::Lit>& lits, Glucose::vec<constraintid>& sortedness_lits);
template constraintid SortingNetworkProoflogger::derive_odds_leq_evens_plus_1<Glucose::vec<Glucose::Lit>, Glucose::vec<constraintid>>(Glucose::vec<Glucose::Lit>& lits, Glucose::vec<constraintid>& sortedness_lits);

template constraintid SortingNetworkProoflogger::derive_counting_definition_of_outputvars_right<Glucose::vec<Glucose::Lit>, Glucose::vec<constraintid>>(Glucose::vec<Glucose::Lit>& inputlits, Glucose::vec<Glucose::Lit>& outputlits, int j, constraintid output_geq_input, Glucose::vec<constraintid>& sortedness_outputlits);
template constraintid SortingNetworkProoflogger::derive_counting_definition_of_outputvars_left<Glucose::vec<Glucose::Lit>, Glucose::vec<constraintid>>(Glucose::vec<Glucose::Lit>& inputlits, Glucose::vec<Glucose::Lit>& outputlits, int j, constraintid output_geq_input, Glucose::vec<constraintid>& sortedness_outputlits);