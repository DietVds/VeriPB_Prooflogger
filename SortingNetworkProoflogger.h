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
    std::vector<VeriPB::Lit> removed_wires;
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

    //std::vector<VeriPB::Lit> lits_for_check;

    // Recursive sorting networks 
    template <class TSeqLit>
    void derive_UB_for_recursive_sortingnetwork(ConstraintStoreSort& plcxns, ConstraintStoreSort& plcxns_recursive,  TSeqLit& input_recursive_network, TSeqLit& input_other_network);

    template <class TSeqLit>
    void derive_UB_for_mergenetwork(ConstraintStoreMerge& plcxns_merge, ConstraintStoreSort& plcxns, ConstraintStoreSort& plcxns_left_sort, ConstraintStoreSort& plcxns_right_sort, TSeqLit& left_output, TSeqLit& right_output );

    template <class TSeqLit>
    void derive_input_equals_output_sortingnetwork(ConstraintStoreSort& plcxns, ConstraintStoreSort& plcxns_left, ConstraintStoreSort& plcxns_right, ConstraintStoreMerge& plcxns_merge, TSeqLit& inputs, TSeqLit& outputs);


    // Merge network 
    template <class TSeqLit>
    void derive_sortedness_odds(TSeqLit& literals, std::vector<constraintid>& sortedness_literals, std::vector<constraintid>& sortedness_odds);
    template <class TSeqLit>
    void derive_sortedness_evens(TSeqLit& literals, std::vector<constraintid>& sortedness_literals, std::vector<constraintid>& sortedness_evens);


    template <class TSeqLit>
    void derive_sortedness_inputs_recursivemergenetworks(ConstraintStoreMerge& plcxns, ConstraintStoreMerge& plcxns_evens, ConstraintStoreMerge& plcxns_odds, TSeqLit& inputA, TSeqLit& inputB);

    template <class TSeqLit> 
    void derive_UB_for_recursive_mergenetworks(ConstraintStoreMerge& plcxns, ConstraintStoreMerge& plcxns_evens, ConstraintStoreMerge& plcxns_odds, TSeqLit& inputA, TSeqLit& inputB);

    template <class TSeqLit>
    void derive_input_equals_output_mergenetwork(ConstraintStoreMerge& plcxns, ConstraintStoreMerge& plcxns_evens, ConstraintStoreMerge& plcxns_odds, TSeqLit& inputA, TSeqLit& inputB, TSeqLit& outputs);


    template<class TSeqLit>
    void derive_outputs_recursivemergenetworks_evens_leq_odds(ConstraintStoreMerge& plcxns, ConstraintStoreMerge& plcxns_evens, ConstraintStoreMerge& plcxns_odds, TSeqLit& outputs_recursivemerge_evens, TSeqLit& outputs_recursivemerge_odds, TSeqLit& outputs, TSeqLit& inputA, TSeqLit& inputB);
    template<class TSeqLit>
    void derive_outputs_recursivemergenetworks_odds_leq_evens_plus2(ConstraintStoreMerge& plcxns, ConstraintStoreMerge& plcxns_evens, ConstraintStoreMerge& plcxns_odds, TSeqLit& outputs_recursivemerge_evens, TSeqLit& outputs_recursivemerge_odds, TSeqLit& outputs, TSeqLit& inputA, TSeqLit& inputB);


    template <class TSeqLit>
    void derive_sortedness_output_mergenetwork(ConstraintStoreMerge& plcxns, ConstraintStoreMerge& plcxns_evens, ConstraintStoreMerge& plcxns_odds, TSeqLit& outputs_recursivemerge_evens, TSeqLit& outputs_recursivemerge_odds, TSeqLit& outputs, TSeqLit& inputsA, TSeqLit& inputsB);

    template<class TSeqLit>
    void derive_UB_mergenetwork_output(ConstraintStoreMerge& plcxns, TSeqLit& outputs);

    template<class TLit>
    void remove_wire_mergenetwork_output(ConstraintStoreMerge& plcxns, TLit& lit);
    template <class TSeqLit>
    void update_input_equals_output_mergenetwork_after_removing_wires(ConstraintStoreMerge& plcxns, TSeqLit& inputA, TSeqLit& inputB, TSeqLit& output);
    void remove_sortedness_constraints_mergenetwork_after_removing_wires(ConstraintStoreMerge& plcxns);
    

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

    template<class TSeqLit>
    std::string sequence_to_string(TSeqLit& lits);  

private:
    VeriPbProofLogger *PL;
    VeriPB::Lit zerolit;
   
};



#endif