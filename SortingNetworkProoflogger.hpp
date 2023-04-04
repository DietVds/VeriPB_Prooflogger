#include "SortingNetworkProoflogger.h"
#include <iostream>
#include <cmath>

// Recursive sorting networks 

template <class TSeqLit>
constraintid SortingNetworkProoflogger::derive_UB_constraint_recursive_sortingnetwork(constraintid& cxnUB, TSeqLit& blockings_recursive_network, TSeqLit& blockings_other_network, int UBproof ){
    PL->start_CP_derivation(cxnUB);
    for(int i = 0; i < blockings_other_network.size(); i++)
      PL->CP_weakening(variable(blockings_other_network[i]));
    constraintid cxn = PL->end_CP_derivation();


    lits_for_check.clear(); 
    for(int i = 0; i < blockings_recursive_network.size(); i++){
        lits_for_check.push_back( toVeriPbLit(neg(blockings_recursive_network[i])));
    }
    PL->check_last_constraint(lits_for_check, blockings_recursive_network.size() - UBproof);
    
    return cxn;
}    

template <class TSeqLit>
constraintid SortingNetworkProoflogger::derive_UB_constraint_on_mergenetwork_input(constraintid cxnUB, TSeqLit& left_recursive_sort_output, constraintid input_geq_output_left, TSeqLit& right_recursive_sort_output, constraintid input_geq_output_right, int UBproof ){
    PL->start_CP_derivation(cxnUB);
    PL->CP_add_constraint(input_geq_output_left);
    PL->CP_add_constraint(input_geq_output_right);
    constraintid cxn = PL->end_CP_derivation();

    lits_for_check.clear();
    for(int i = 0; i < left_recursive_sort_output.size(); i++)
        lits_for_check.push_back(toVeriPbLit(neg(left_recursive_sort_output[i])));
    for(int i = 0; i < right_recursive_sort_output.size(); i++)
        lits_for_check.push_back(toVeriPbLit(neg(right_recursive_sort_output[i])));
    PL->check_last_constraint(lits_for_check, left_recursive_sort_output.size() + right_recursive_sort_output.size() - UBproof);

    return cxn;
}

template <class TSeqLit>
constraintid SortingNetworkProoflogger::derive_input_geq_output_sortingnetwork(constraintid merge_input_geq_output, constraintid input_geq_output_left_recursive_sort, constraintid input_geq_output_right_recursive_sort, TSeqLit& inputliterals, TSeqLit& outputliterals){
    PL->start_CP_derivation(merge_input_geq_output);
    PL->CP_add_constraint(input_geq_output_left_recursive_sort);
    PL->CP_add_constraint(input_geq_output_right_recursive_sort);
    constraintid cxn = PL->end_CP_derivation();

    lits_for_check.clear();
    for(int j = 0; j < inputliterals.size(); j++)
      lits_for_check.push_back(toVeriPbLit(inputliterals[j]));
    for(int j = 0; j < outputliterals.size(); j++)
      lits_for_check.push_back(toVeriPbLit(neg(outputliterals[j])));
    PL->check_last_constraint(lits_for_check, outputliterals.size());

    return cxn;
}

template <class TSeqLit>
constraintid SortingNetworkProoflogger::derive_input_leq_output_sortingnetwork(constraintid merge_input_leq_output, constraintid input_leq_output_left_recursive_sort, constraintid input_leq_output_right_recursive_sort, TSeqLit& inputliterals, TSeqLit& outputliterals){
    PL->start_CP_derivation(merge_input_leq_output);
    PL->CP_add_constraint(input_leq_output_left_recursive_sort);
    PL->CP_add_constraint(input_leq_output_right_recursive_sort);
    constraintid cxn = PL->end_CP_derivation();

    //TODO: take into account that the linkingVars can be smaller than the blockings!
    lits_for_check.clear();
    for(int j = 0; j < inputliterals.size(); j++)
      lits_for_check.push_back(toVeriPbLit(neg(inputliterals[j])));
    for(int j = 0; j < outputliterals.size(); j++)
      lits_for_check.push_back(toVeriPbLit(outputliterals[j]));
    PL->check_last_constraint(lits_for_check, inputliterals.size());

    return cxn;
}


// Merge network
template <class TSeqLit, class TSeqCxnId>
constraintid SortingNetworkProoflogger::derive_evens_leq_odds(TSeqLit& seq, TSeqCxnId& sortedness_seq){
    assert(seq.size() > 0);  
    
    int sequence_size = sortedness_seq.size() + 1; // Note that the sequence seq can contain zero literals. We will have to ensure that sortedness_seq are only constraints for the  non-zero elements in seq.

    constraintid cxn = 0;

    if(sequence_size > 1){
        PL->start_CP_derivation(sortedness_seq[0]);
        for(int j = 2; j < sortedness_seq.size(); j+=2)
            PL->CP_add_constraint(sortedness_seq[j]);
        if(sequence_size % 2 == 1) // If the size of the sequence is odd, add the last variable as literal axiom
            PL->CP_add_literal_axiom(seq[sortedness_seq.size()]); 
        cxn = PL->end_CP_derivation();
    }
    else{
        PL->start_CP_derivation_with_lit_axiom(seq[0]);
        cxn = PL->end_CP_derivation();   
    }

    std::vector<VeriPB::Lit> lits; lits.clear();
    for(int j = 0; j <= sortedness_seq.size(); j++)
        lits.push_back(j % 2 == 0 ? toVeriPbLit(seq[j]) : toVeriPbLit(neg(seq[j])));
    PL->check_last_constraint(lits, floor((float)(sortedness_seq.size()+1) / 2.0));
    
    return cxn;
}

template <class TSeqLit, class TSeqCxnId>
constraintid SortingNetworkProoflogger::derive_odds_leq_evens_plus_1(TSeqLit& seq, TSeqCxnId& sortedness_seq){
    assert(seq.size() > 0); // assert(sortedness_seq.size() > 0);
    
    int sequence_size = sortedness_seq.size() + 1;

    PL->start_CP_derivation_with_lit_axiom(neg(seq[0])); 
    for(int j = 1; j < sortedness_seq.size(); j+=2)
        PL->CP_add_constraint(sortedness_seq[j]);
    if(sequence_size % 2 == 0 && sequence_size > 1) // If seq.size() is even, add the last literal of the list to the equation
        PL->CP_add_literal_axiom(seq[sequence_size-1]);
    constraintid cxn = PL->end_CP_derivation();

    std::vector<VeriPB::Lit> lits; lits.clear();
    for(int j = 0; j < sequence_size; j++)
        lits.push_back(j % 2 == 0 ? neg(toVeriPbLit(seq[j])) : toVeriPbLit(seq[j]));
    PL->check_last_constraint(lits, ceil((float)sequence_size / 2.0)-1);

    return cxn;
}

template <class TSeqLit> 
constraintid SortingNetworkProoflogger::derive_evens_leq_odds_merge_input(constraintid left_evens_leq_odd, constraintid right_evens_leq_odd, TSeqLit& leftlits, TSeqLit& rightlits ){
    PL->start_CP_derivation(left_evens_leq_odd);
    PL->CP_add_constraint(right_evens_leq_odd);
    constraintid cxn = PL->end_CP_derivation();

    lits_for_check.clear(); int RHS=0; // TODO: RHS should be of type weight, which should be declared in the SolverTypesInt file.

    for(int j = 0; j < leftlits.size() && toVeriPbLit(leftlits[j]) != zerolit; j++){
      lits_for_check.push_back(j % 2 == 0 ? toVeriPbLit(leftlits[j]) : toVeriPbLit(neg(leftlits[j])));
      if(j % 2 == 1) RHS++;
    }
    for(int j = 0; j < rightlits.size() && toVeriPbLit(rightlits[j]) != zerolit; j++){
      lits_for_check.push_back(j % 2 == 0 ? toVeriPbLit(rightlits[j]) : toVeriPbLit(neg(rightlits[j])));
      if(j % 2 == 1) RHS++; 
    }
    PL->check_last_constraint(lits_for_check, RHS);

    return cxn;
}

template <class TSeqLit>
constraintid SortingNetworkProoflogger::derive_odds_leq_evens_plus_2_merge_input(constraintid left_odds_leq_evens_plus1, constraintid right_odds_leq_evens_plus1, TSeqLit& leftlits, TSeqLit& rightlits ){
    PL->start_CP_derivation(left_odds_leq_evens_plus1);
    PL->CP_add_constraint(right_odds_leq_evens_plus1);
    constraintid cxn = PL->end_CP_derivation();

    lits_for_check.clear(); int RHS=0;
    for(int j = 0; j < leftlits.size() && toVeriPbLit(leftlits[j]) != zerolit; j++){
      lits_for_check.push_back(j % 2 == 0 ? toVeriPbLit(neg(leftlits[j])) : toVeriPbLit(leftlits[j]));
      if(j % 2 == 0) RHS++;
    }
    for(int j = 0; j < rightlits.size() && toVeriPbLit(rightlits[j]) != zerolit; j++){
      lits_for_check.push_back(j % 2 == 0 ? toVeriPbLit(neg(rightlits[j])) : toVeriPbLit(rightlits[j]));
      if(j % 2 == 0) RHS++;
    }
    PL->check_last_constraint(lits_for_check, RHS-2);

    return cxn;
}


template <class TSeqLit, class TSeqCxnId>
constraintid SortingNetworkProoflogger::derive_counting_definition_of_outputvars_right(TSeqLit& inputlits, TSeqLit& outputlits, int j, constraintid input_geq_output, TSeqCxnId& sortedness_outputlits){
    std::string comment = "Encoding the constraint: " + PL->to_string(outputlits[j-1]) + " ->" ;
    for(int i = 0; i < inputlits.size(); i++)
        comment += " " + PL->to_string(inputlits[i]);
    comment += " >= " + std::to_string(j);
    PL->write_comment(comment);
    
    PL->start_CP_derivation(input_geq_output);

    for(int i = 0; i < j-1; i++){
        PL->CP_load_constraint(sortedness_outputlits[i]);
        PL->CP_multiply(i+1);
        PL->CP_add();
    }

    for(int i = j; i < outputlits.size(); i++)
        PL->CP_weakening(variable(outputlits[i]));
    
    constraintid cxn = PL->end_CP_derivation();

    // Check constraint:
    std::vector<VeriPB::Lit> lits; std::vector<int> wghts; lits.clear(); wghts.clear();
    for(int i = 0; i < inputlits.size(); i++){
        lits.push_back(toVeriPbLit(inputlits[i]));
        wghts.push_back(1);
    }

    lits.push_back(toVeriPbLit(neg(outputlits[j-1])));
    wghts.push_back(j);
    PL->check_last_constraint(lits, wghts, j);

    return cxn;
}

template <class TSeqLit, class TSeqCxnId>
constraintid SortingNetworkProoflogger::derive_counting_definition_of_outputvars_left(TSeqLit& inputlits, TSeqLit& outputlits, int j, constraintid output_geq_input, TSeqCxnId& sortedness_outputlits){
    std::string comment = "Encoding the constraint: " + PL->to_string(outputlits[j-1]) + " <-" ;
    for(int i = 0; i < inputlits.size(); i++)
        comment += " " + PL->to_string(inputlits[i]);
    comment += " >= " + std::to_string(j);
    PL->write_comment(comment);

    
    PL->start_CP_derivation(output_geq_input);

    for(int i = 0; i < j-1; i++) 
        PL->CP_weakening(variable(outputlits[i])); 

    for(int i = 1; i <= outputlits.size()-j; i++){
        PL->CP_load_constraint(sortedness_outputlits[sortedness_outputlits.size()-i]);
        PL->CP_multiply(i);
        PL->CP_add();
    }

    if(inputlits.size() > outputlits.size()){
        PL->CP_literal_axiom(outputlits[j-1]);
        PL->CP_multiply(inputlits.size()-outputlits.size());
        PL->CP_add();
    }

    if(inputlits.size() < outputlits.size()){
        PL->CP_saturate();
    }

    constraintid cxn = PL->end_CP_derivation();

    // Check constraint:
    std::vector<VeriPB::Lit> lits; std::vector<int> wghts; lits.clear(); wghts.clear();
    for(int i = 0; i < inputlits.size(); i++){
        lits.push_back(toVeriPbLit(neg(inputlits[i])));
        wghts.push_back(1);
    }

    lits.push_back(toVeriPbLit(outputlits[j-1]));
    int RHS = inputlits.size() - j + 1;
    wghts.push_back(RHS);
    PL->check_last_constraint(lits, wghts, RHS);

    return cxn;
}

template <class TLit>
constraintid SortingNetworkProoflogger::derive_sortedness_by_rup(TLit& smaller_lit, TLit& greater_lit){
    std::vector<VeriPB::Lit> lits; lits.clear();

    lits.push_back(toVeriPbLit(neg(smaller_lit)));
    lits.push_back(toVeriPbLit(greater_lit));
    return PL->rup(lits);
}
