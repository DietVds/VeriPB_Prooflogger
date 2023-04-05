#ifndef SortingNetworkProoflogger_h
#define SortingNetworkProoflogger_h

#include "VeriPBProoflogger.h"

typedef struct {
    constraintid sortedness = 0;
    constraintid input_geq_output = 0; 
    constraintid input_leq_output = 0;

    constraintid reif_left_output1 = 0;
    constraintid reif_right_output1 = 0;
    constraintid reif_left_output2 = 0;
    constraintid reif_right_output2 = 0;
} ConstraintStoreComparator;

typedef struct {
    std::vector<constraintid> sortedness_inputA;
    std::vector<constraintid> sortedness_inputB;
    constraintid cxnUBinputs = 0;
    int UB; 

    std::vector<constraintid> sortedness_output;
    constraintid input_geq_output = 0;
    constraintid input_leq_output = 0;

    constraintid inputA_evens_leq_odds = 0;
    constraintid inputB_evens_leq_odds = 0;
    constraintid inputA_odds_leq_evens_plus1 = 0;
    constraintid inputB_odds_leq_evens_plus1 = 0;

    constraintid inputs_evens_leq_odds = 0;
    constraintid inputs_odds_leq_evens_plus2 = 0;

    constraintid outputs_recursivenetworks_evens_leq_odds = 0;
    constraintid outputs_recursivenetworks_odds_leq_evens_plus2 = 0;

    std::vector<ConstraintStoreComparator> comparatormodules;

    constraintid outputs_cxnUB = 0;
    std::vector<constraintid> constraints_removed_wires;
} ConstraintStoreMerge;

typedef struct {
    std::vector<constraintid> sortedness_output;
    constraintid input_geq_output = 0;
    constraintid input_leq_output = 0;

    int UB;
    constraintid cxnUBinputs = 0;
} ConstraintStoreSort;

// PROOF LOGGING FUNCTIONS: 

class SortingNetworkProoflogger
{
public:
    // Constructor
    //SortingNetworkProoflogger(VeriPbProofLogger *PL);
    template <class TLit>
    SortingNetworkProoflogger(VeriPbProofLogger *PL, TLit zero, constraintid def_one) : PL(PL), zerolit(toVeriPbLit(zero)), def_one(def_one) {}

    constraintid def_one;

    std::vector<VeriPB::Lit> lits_for_check;

    // Recursive sorting networks 
    template <class TSeqLit>
    constraintid derive_UB_constraint_recursive_sortingnetwork(constraintid& cxnUB, TSeqLit& blockings_recursive_network, TSeqLit& blockings_other_network, int UBproof );

    template <class TSeqLit>
    void derive_UB_for_recursive_sortingnetwork(ConstraintStoreSort& plcxn, ConstraintStoreSort& plcxn_recursive,  TSeqLit& input_recursive_network, TSeqLit& input_other_network);


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