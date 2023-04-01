#include "SortingNetworkProoflogger.h"
#include <iostream>
#include <cmath>

SortingNetworkProoflogger::SortingNetworkProoflogger(VeriPbProofLogger *PL) : PL(PL){}

template <class TSeqLit, class TSeqCxnId>
constraintid SortingNetworkProoflogger::derive_evens_leq_odds(TSeqLit& seq, TSeqCxnId& sortedness_seq){
    assert(seq.size() > 1); assert(sortedness_seq.size() > 0);

    PL->start_CP_derivation(sortedness_seq[0]);
    for(int j = 2; j < seq.size() - 1; j+=2)
        PL->CP_add_constraint(sortedness_seq[j]);
    if(seq.size() % 2 == 1) // If the size of seq is odd, add the last variable as literal axiom
        PL->CP_add_literal_axiom(seq[seq.size()-1]); 
    constraintid cxn = PL->end_CP_derivation();
    
    std::vector<VeriPB::Lit> lits; lits.clear();
    for(int j = 0; j < seq.size(); j++)
        lits.push_back(j % 2 == 0 ? toVeriPbLit(seq[j]) : toVeriPbLit(neg(seq[j])));
    PL->check_last_constraint(lits, floor((float)seq.size() / 2.0));
    
    return cxn;
}

template <class TSeqLit, class TSeqCxnId>
constraintid SortingNetworkProoflogger::derive_odds_leq_evens_plus_1(TSeqLit& seq, TSeqCxnId& sortedness_seq){
    assert(seq.size() > 1); assert(sortedness_seq.size() > 0);
    
    PL->start_CP_derivation_with_lit_axiom(neg(seq[0])); 
    for(int j = 1; j < sortedness_seq.size(); j+=2)
        PL->CP_add_constraint(sortedness_seq[j]);
    if(seq.size() % 2 == 0) // If seq.size() is even, add the last literal of the list to the equation
        PL->CP_add_literal_axiom(seq[seq.size()-1]);
    constraintid cxn = PL->end_CP_derivation();

    std::vector<VeriPB::Lit> lits; lits.clear();
    for(int j = 0; j < seq.size(); j++)
        lits.push_back(j % 2 == 0 ? neg(toVeriPbLit(seq[j])) : toVeriPbLit(seq[j]));
    PL->check_last_constraint(lits, ceil((float)seq.size() / 2.0)-1);

    return cxn;
}

template <class TSeqLit, class TSeqCxnId>
constraintid SortingNetworkProoflogger::derive_counting_definition_of_outputvars_right(TSeqLit& inputlits, TSeqLit& outputlits, int j, constraintid input_geq_output, TSeqCxnId& sortedness_outputlits){
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
    PL->start_CP_derivation(output_geq_input);

    for(int i = 0; i < j-1; i++) 
        PL->CP_weakening(variable(outputlits[i])); 

    for(int i = 1; i <= outputlits.size()-j; i++){
        PL->CP_load_constraint(sortedness_outputlits[sortedness_outputlits.size()-i]);
        PL->CP_multiply(i);
        PL->CP_add();
    }

    PL->CP_literal_axiom(outputlits[j-1]);
    PL->CP_multiply(inputlits.size()-outputlits.size());
    PL->CP_add();

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