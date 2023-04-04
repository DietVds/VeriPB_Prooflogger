#ifndef SortingNetworkProoflogger_h
#define SortingNetworkProoflogger_h

#include "VeriPBProoflogger.h"

class SortingNetworkProoflogger
{
    

public:
    // Constructor
    //SortingNetworkProoflogger(VeriPbProofLogger *PL);
    template <class TLit>
    SortingNetworkProoflogger(VeriPbProofLogger *PL, TLit zero) : PL(PL), zerolit(toVeriPbLit(zero)) {}

    std::vector<VeriPB::Lit> lits_for_check;

    // Recursive sorting networks 
    template <class TSeqLit>
    constraintid derive_UB_constraint_recursive_sortingnetwork(constraintid& cxnUB, TSeqLit& blockings_recursive_network, TSeqLit& blockings_other_network, int UBproof );

    template <class TSeqLit>
    constraintid derive_UB_constraint_on_mergenetwork_input(constraintid cxnUB, TSeqLit& left_recursive_sort_output, constraintid input_geq_output_left, TSeqLit& right_recursive_sort_output, constraintid input_geq_output_right, int UBproof );

    template <class TSeqLit>
    constraintid derive_input_geq_output_sortingnetwork(constraintid merge_input_geq_output, constraintid input_geq_output_left_recursive_sort, constraintid input_geq_output_right_recursive_sort, TSeqLit& inputliterals, TSeqLit& outputliterals);

    template <class TSeqLit>
    constraintid derive_input_leq_output_sortingnetwork(constraintid merge_input_leq_output, constraintid input_leq_output_left_recursive_sort, constraintid input_leq_output_right_recursive_sort, TSeqLit& inputliterals, TSeqLit& outputliterals);


    // Merge network 
    template <class TSeqLit, class TSeqCxnId>
    constraintid derive_evens_leq_odds(TSeqLit& lits, TSeqCxnId& sortedness_lits);

    template <class TSeqLit, class TSeqCxnId>
    constraintid derive_odds_leq_evens_plus_1(TSeqLit& lits, TSeqCxnId& sortedness_lits);

    template <class TSeqLit>
    constraintid derive_evens_leq_odds_merge_input(constraintid left_evens_leq_odd, constraintid right_evens_leq_odd, TSeqLit& leftlits, TSeqLit& rightlits );

    template <class TSeqLit>
    constraintid derive_odds_leq_evens_plus_2_merge_input(constraintid left_odds_leq_evens_plus1, constraintid right_odds_leq_evens_plus1, TSeqLit& leftlits, TSeqLit& rightlits );


    template <class TSeqLit, class TSeqCxnId>
    constraintid derive_counting_definition_of_outputvars_right(TSeqLit& inputlits, TSeqLit& outputlits, int j, constraintid input_geq_output, TSeqCxnId& sortedness_outputlits);

    template <class TSeqLit, class TSeqCxnId>
    constraintid derive_counting_definition_of_outputvars_left(TSeqLit& inputlits, TSeqLit& outputlits, int j, constraintid output_geq_input, TSeqCxnId& sortedness_outputlits);

    template <class TLit>
    constraintid derive_sortedness_by_rup(TLit& smaller_lit, TLit& greater_lit);

private:
    VeriPbProofLogger *PL;
    VeriPB::Lit zerolit;
};

#endif