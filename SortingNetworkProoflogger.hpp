#include "SortingNetworkProoflogger.h"
#include <iostream>
#include <cmath>

// Recursive sorting networks 

template <class TSeqLit>
void SortingNetworkProoflogger::derive_UB_for_recursive_sortingnetwork(ConstraintStoreSort& plcxns, ConstraintStoreSort& plcxns_recursive,  TSeqLit& input_recursive_network, TSeqLit& input_other_network){
    assert(plcxns.cxnUBinputs != 0 );
    
    plcxns_recursive.UB = plcxns.UB;
    
    PL->start_CP_derivation(plcxns.cxnUBinputs);
    for(int i = 0; i < input_other_network.size(); i++)
      PL->CP_weakening(variable(input_other_network[i]));
    plcxns_recursive.cxnUBinputs = PL->end_CP_derivation();

    std::vector<VeriPB::Lit> lits_for_check;
    for(int i = 0; i < input_recursive_network.size(); i++){
        lits_for_check.push_back( toVeriPbLit(neg(input_recursive_network[i])));
    }
    PL->check_last_constraint(lits_for_check, input_recursive_network.size() - plcxns_recursive.UB);
}

template <class TSeqLit>
void SortingNetworkProoflogger::derive_UB_for_mergenetwork(ConstraintStoreMerge& plcxns_merge, ConstraintStoreSort& plcxns, ConstraintStoreSort& plcxns_left, ConstraintStoreSort& plcxns_right, TSeqLit& left_output, TSeqLit& right_output ){
    assert(plcxns.cxnUBinputs != 0);
    if(left_output.size() > 1)
        assert(plcxns_left.input_geq_output != 0);
    if(right_output.size() > 1)
        assert(plcxns_right.input_geq_output != 0);
    
    plcxns_merge.UB = plcxns.UB;
    
    PL->write_comment("Derive UB for mergenetwork inputs");
    PL->start_CP_derivation(plcxns.cxnUBinputs);
    if(left_output.size() > 1)
        PL->CP_add_constraint(plcxns_left.input_geq_output);
    if(right_output.size() > 1)
        PL->CP_add_constraint(plcxns_right.input_geq_output);
    plcxns_merge.cxnUBinputs = PL->end_CP_derivation();

    std::vector<VeriPB::Lit> lits_for_check;
    for(int i = 0; i < left_output.size(); i++)
        lits_for_check.push_back(toVeriPbLit(neg(left_output[i])));
    for(int i = 0; i < right_output.size(); i++)
        lits_for_check.push_back(toVeriPbLit(neg(right_output[i])));
    PL->check_last_constraint(lits_for_check, left_output.size() + right_output.size() - plcxns_merge.UB);
}

template <class TSeqLit>
void SortingNetworkProoflogger::derive_input_equals_output_sortingnetwork(ConstraintStoreSort& plcxns, ConstraintStoreSort& plcxns_left, ConstraintStoreSort& plcxns_right, ConstraintStoreMerge& plcxns_merge, TSeqLit& inputliterals, TSeqLit& outputliterals){
    assert(plcxns_merge.input_geq_output != 0);
    assert(plcxns_merge.input_leq_output != 0);
    

    // Deriving input geq output
    PL->start_CP_derivation(plcxns_merge.input_geq_output);
    if(plcxns_left.input_geq_output != 0) // Can be zero in case the number of elements in the left sorting network is one
        PL->CP_add_constraint(plcxns_left.input_geq_output);
    if(plcxns_right.input_geq_output != 0) // Can be zero in case the number of elements in the left sorting network is one
        PL->CP_add_constraint(plcxns_right.input_geq_output);
    plcxns.input_geq_output = PL->end_CP_derivation();

    std::vector<VeriPB::Lit> lits_for_check;
    for(int j = 0; j < inputliterals.size(); j++)
      lits_for_check.push_back(toVeriPbLit(inputliterals[j]));
    for(int j = 0; j < outputliterals.size(); j++)
      lits_for_check.push_back(toVeriPbLit(neg(outputliterals[j])));
    PL->check_last_constraint(lits_for_check, outputliterals.size());

    // Deriving input leq output
    PL->start_CP_derivation(plcxns_merge.input_leq_output);
    if(plcxns_left.input_leq_output != 0)
        PL->CP_add_constraint(plcxns_left.input_leq_output);
    if(plcxns_right.input_leq_output != 0)
        PL->CP_add_constraint(plcxns_right.input_leq_output);
    plcxns.input_leq_output = PL->end_CP_derivation();

    lits_for_check.clear();
    for(int j = 0; j < inputliterals.size(); j++)
      lits_for_check.push_back(toVeriPbLit(neg(inputliterals[j])));
    for(int j = 0; j < outputliterals.size(); j++)
      lits_for_check.push_back(toVeriPbLit(outputliterals[j]));
    PL->check_last_constraint(lits_for_check, inputliterals.size());
}



// Merge network
template <class TSeqLit>
void SortingNetworkProoflogger::derive_sortedness_odds(TSeqLit& literals, std::vector<constraintid>& sortedness_literals, std::vector<constraintid>& sortedness_odds){
    assert(sortedness_odds.size() == 0);

    for(int i = 0; i < literals.size()-2 && i < sortedness_literals.size()-1 && toVeriPbLit(literals[i+2]) != zerolit; i+=2){ 
        PL->start_CP_derivation(sortedness_literals[i]); 
        PL->CP_add_constraint(sortedness_literals[i+1]);
        sortedness_odds.push_back(PL->end_CP_derivation());

        std::vector<VeriPB::Lit> lits_for_check;
        lits_for_check.push_back(toVeriPbLit(literals[i])); lits_for_check.push_back(neg(toVeriPbLit(literals[i+2]))); 
        PL->check_last_constraint(lits_for_check);
    }
}

template <class TSeqLit>
void SortingNetworkProoflogger::derive_sortedness_evens(TSeqLit& literals, std::vector<constraintid>& sortedness_literals, std::vector<constraintid>& sortedness_evens){
    assert(sortedness_evens.size() == 0);

    for(int i = 1; i < literals.size()-2 && i < sortedness_literals.size()-1 && toVeriPbLit(literals[i+2]) != zerolit; i+=2){ // Odds of inputA
        PL->start_CP_derivation(sortedness_literals[i]); 
        PL->CP_add_constraint(sortedness_literals[i+1]);
        sortedness_evens.push_back(PL->end_CP_derivation());

        std::vector<VeriPB::Lit> lits_for_check;
        lits_for_check.push_back(toVeriPbLit(literals[i])); lits_for_check.push_back(neg(toVeriPbLit(literals[i+2]))); 
        PL->check_last_constraint(lits_for_check);
    }
}


template <class TSeqLit>
void SortingNetworkProoflogger::derive_sortedness_inputs_recursivemergenetworks(ConstraintStoreMerge& plcxns, ConstraintStoreMerge& plcxns_evens, ConstraintStoreMerge& plcxns_odds, TSeqLit& inputA, TSeqLit& inputB){
    derive_sortedness_odds(inputA, plcxns.sortedness_inputA, plcxns_odds.sortedness_inputA);
    derive_sortedness_evens(inputA, plcxns.sortedness_inputA, plcxns_evens.sortedness_inputA);
    derive_sortedness_odds(inputB, plcxns.sortedness_inputB, plcxns_odds.sortedness_inputB);
    derive_sortedness_evens(inputB, plcxns.sortedness_inputB, plcxns_evens.sortedness_inputB);
}


template <class TSeqLit> 
void SortingNetworkProoflogger::derive_UB_for_recursive_mergenetworks(ConstraintStoreMerge& plcxns, ConstraintStoreMerge& plcxns_evens, ConstraintStoreMerge& plcxns_odds, TSeqLit& inputA, TSeqLit& inputB){
    
    
    //PROOF: Derive evens(inputA/inputB) =< odds(inputA/inputB)
    plcxns_evens.UB = plcxns.UB/2;
    plcxns_odds.UB = plcxns.UB/2+1;

    //PROOF: the UB constraint for the recursive network for the evens
    std::vector<VeriPB::Lit> lits_for_check; int RHS=0;
    for(int j = 1; j < inputA.size() && toVeriPbLit(inputA[j]) != zerolit; j+=2){
      lits_for_check.push_back(toVeriPbLit(neg(inputA[j])));
      RHS++;
    }  
    for(int j = 1; j < inputB.size() && toVeriPbLit(inputB[j]) != zerolit; j+=2){
      lits_for_check.push_back(toVeriPbLit(neg(inputB[j])));
      RHS++;
    }

    if(plcxns_evens.UB < RHS){  // If the UBproofEvens is smaller than the number of elements in evens, derive the UB
      assert(plcxns.cxnUBinputs != 0);

      if(plcxns.inputA_evens_leq_odds == 0){
          PL->write_comment("Derive evens(inputA) =< odds(inputA)");
          plcxns.inputA_evens_leq_odds = derive_evens_leq_odds(inputA, plcxns.sortedness_inputA);
      }
      if(plcxns.inputB_evens_leq_odds == 0){
          PL->write_comment("Derive evens(inputB) =< odds(inputB)");
          plcxns.inputB_evens_leq_odds = derive_evens_leq_odds(inputB, plcxns.sortedness_inputB);
      }
      if(plcxns.inputs_evens_leq_odds == 0){
          PL->write_comment("evens(inputA) + evens(inputB) =< odds(inputA) + odds(inputB)");
          plcxns.inputs_evens_leq_odds = derive_evens_leq_odds_merge_input(plcxns.inputA_evens_leq_odds, plcxns.inputB_evens_leq_odds, inputA, inputB);
      }

      PL->write_comment("Derive UB for evens");
      std::cout << "Derive UB for evens" << std::endl;

      PL->start_CP_derivation(plcxns.cxnUBinputs);
      PL->CP_add_constraint(plcxns.inputs_evens_leq_odds);
      PL->CP_divide(2);
      plcxns_evens.cxnUBinputs = PL->end_CP_derivation();
      PL->check_last_constraint(lits_for_check, RHS - plcxns_evens.UB);
    }

    //PROOF: the UB constraint for the recursive network for the odds
    lits_for_check.clear(); RHS=0;
    for(int j = 0; j < inputA.size() && toVeriPbLit(inputA[j]) != zerolit; j+=2){
      lits_for_check.push_back(toVeriPbLit(neg(inputA[j])));
      RHS++;
    }
    for(int j = 0; j < inputB.size() && toVeriPbLit(inputB[j]) != zerolit; j+=2){
      lits_for_check.push_back(toVeriPbLit(neg(inputB[j])));
      RHS++;
    }
    if(plcxns_odds.UB < RHS){

        assert(plcxns.cxnUBinputs != 0);

        if(plcxns.inputA_odds_leq_evens_plus1 == 0){
            PL->write_comment("Derive odds(inputA) =< evens(inputA) + 1");
            plcxns.inputA_odds_leq_evens_plus1 = derive_odds_leq_evens_plus_1(inputA, plcxns.sortedness_inputA);
        }
        if(plcxns.inputB_odds_leq_evens_plus1 == 0){
            PL->write_comment("Derive odds(inputB) =< evens(inputB) + 1");
            plcxns.inputB_odds_leq_evens_plus1 = derive_odds_leq_evens_plus_1(inputB, plcxns.sortedness_inputB);
        }
        //PROOF: Derive odds(inputA) + odds(inputB) =< evens(inputA) + evens(inputB) + 2
        if(plcxns.inputs_odds_leq_evens_plus2 == 0){
            PL->write_comment("Derive odds(inputA) + odds(inputB) =< evens(inputA) + evens(inputB) + 2");
            plcxns.inputs_odds_leq_evens_plus2 = derive_odds_leq_evens_plus_2_merge_input(plcxns.inputA_odds_leq_evens_plus1, plcxns.inputB_odds_leq_evens_plus1, inputA, inputB);
        }

      PL->write_comment("Derive UB for odds");
      std::cout << "Derive UB for odds" << std::endl;

      PL->start_CP_derivation(plcxns.cxnUBinputs);
      PL->CP_add_constraint(plcxns.inputs_odds_leq_evens_plus2);
      PL->CP_divide(2);
      plcxns_odds.cxnUBinputs = PL->end_CP_derivation();

      PL->check_last_constraint(lits_for_check, RHS - plcxns_odds.UB);
    }
    PL->write_comment("Done deriving UB for recursive merge networks");
}


template <class TSeqLit>
void SortingNetworkProoflogger::derive_input_equals_output_mergenetwork(ConstraintStoreMerge& plcxns, ConstraintStoreMerge& plcxns_evens, ConstraintStoreMerge& plcxns_odds, TSeqLit& inputA, TSeqLit& inputB, TSeqLit& outputs){
    assert(plcxns_evens.input_geq_output != 0); assert(plcxns_odds.input_geq_output != 0);
    //PROOF: Deriving the input >= output constraint
    PL->write_comment("Deriving the input >= output constraint");
    PL->start_CP_derivation(plcxns_evens.input_geq_output);
    PL->CP_add_constraint(plcxns_odds.input_geq_output);
    for(int j = 0; j < plcxns.comparatormodules.size(); j++)
      PL->CP_add_constraint(plcxns.comparatormodules[j].input_geq_output);
    plcxns.input_geq_output = PL->end_CP_derivation();

    std::vector<VeriPB::Lit> lits_for_check; int RHS = 0;
    for(int i = 0; i < inputA.size() && toVeriPbLit(inputA[i]) != zerolit; i++)
        lits_for_check.push_back(toVeriPbLit(inputA[i]));
    for(int i = 0; i < inputB.size() && toVeriPbLit(inputB[i])!= zerolit; i++)
        lits_for_check.push_back(toVeriPbLit(inputB[i]));
    for(int i = 0; i < outputs.size() && toVeriPbLit(outputs[i]) != zerolit; i++){
        lits_for_check.push_back(toVeriPbLit(neg(outputs[i]))); 
        RHS++;
    }
    PL->check_last_constraint(lits_for_check, RHS);

    //PROOF: Deriving the input =< output constraint
    PL->write_comment("Deriving the input =< output constraint");
    // std::cout << "Deriving the input =< output constraint" << std::endl;
    PL->start_CP_derivation(plcxns_evens.input_leq_output);
    PL->CP_add_constraint(plcxns_odds.input_leq_output);
    for(int j = 0; j < plcxns.comparatormodules.size(); j++)
      PL->CP_add_constraint(plcxns.comparatormodules[j].input_leq_output);
    plcxns.input_leq_output = PL->end_CP_derivation();   

    lits_for_check.clear(); RHS = 0;
    for(int i = 0; i < inputA.size() && toVeriPbLit(inputA[i]) != zerolit; i++){
        lits_for_check.push_back(toVeriPbLit(neg(inputA[i])));
        RHS++;
    }
    for(int i = 0; i < inputB.size() && toVeriPbLit(inputB[i])!= zerolit; i++){
        lits_for_check.push_back(toVeriPbLit(neg(inputB[i])));
        RHS++;
    }    
    for(int i = 0; i < outputs.size() && toVeriPbLit(outputs[i]) != zerolit; i++){
        lits_for_check.push_back(toVeriPbLit(outputs[i])); 
    }
    PL->check_last_constraint(lits_for_check, RHS);
}

template<class TSeqLit>
void SortingNetworkProoflogger::derive_outputs_recursivemergenetworks_evens_leq_odds(ConstraintStoreMerge& plcxns, ConstraintStoreMerge& plcxns_evens, ConstraintStoreMerge& plcxns_odds, TSeqLit& e, TSeqLit& o, TSeqLit& outputs, TSeqLit& inputA, TSeqLit& inputB){
    assert(plcxns_evens.input_geq_output != 0);
    assert(plcxns_odds.input_leq_output != 0);
    
    // PROOF: If not derived, derive the needed constraints.
    if(plcxns.inputA_evens_leq_odds == 0){
        PL->write_comment("Derive evens(inputA) =< odds(inputA)");
        plcxns.inputA_evens_leq_odds = derive_evens_leq_odds(inputA, plcxns.sortedness_inputA);
    }

    if(plcxns.inputB_evens_leq_odds == 0){
        PL->write_comment("Derive evens(inputB) =< odds(inputB)");
        plcxns.inputB_evens_leq_odds = derive_evens_leq_odds(inputB, plcxns.sortedness_inputB);
    }

    if(plcxns.inputs_evens_leq_odds == 0){
        PL->write_comment("evens(inputA) + evens(inputB) =< odds(inputA) + odds(inputB)");
	    plcxns.inputs_evens_leq_odds = derive_evens_leq_odds_merge_input(plcxns.inputA_evens_leq_odds, plcxns.inputB_evens_leq_odds, inputA, inputB);
    }
    
    //PROOF: Derive sum of e =< sum of o 
    PL->write_comment("Derive sum of e =< sum of d");
    std::cout << "Derive sum of e =< sum of d" << std::endl;

    PL->start_CP_derivation(plcxns.inputs_evens_leq_odds);
    PL->CP_add_constraint(plcxns_evens.input_geq_output);
    PL->CP_add_constraint(plcxns_odds.input_leq_output);
    plcxns.outputs_recursivenetworks_evens_leq_odds = PL->end_CP_derivation();
    
    std::vector<VeriPB::Lit> lits_for_check;
    int RHS = 0;
    for(int j = 0; j < e.size() && toVeriPbLit(e[j]) != zerolit; j++){
      // When d.size() > e.size(), e ends with zero literals
      lits_for_check.push_back(toVeriPbLit(neg(e[j])));
      RHS++;
    }
    for(int j = 0; j < o.size() && toVeriPbLit(o[j]) != zerolit ; j++)
      lits_for_check.push_back(toVeriPbLit(o[j]));
    PL->check_last_constraint(lits_for_check, RHS);
}

// TODO: Change name of lits_for_check and add weights and RHS.
template<class TSeqLit>
void SortingNetworkProoflogger::derive_outputs_recursivemergenetworks_odds_leq_evens_plus2(ConstraintStoreMerge& plcxns, ConstraintStoreMerge& plcxns_evens, ConstraintStoreMerge& plcxns_odds, TSeqLit& e, TSeqLit& o, TSeqLit& outputs, TSeqLit& inputA, TSeqLit& inputB){
    assert(plcxns_evens.input_leq_output != 0);
    assert(plcxns_odds.input_geq_output != 0);

    //PROOF: Derive odds(inputA/inputB) =< evens(inputA/inputB) + 1
    if(plcxns.inputB_odds_leq_evens_plus1 == 0){
        PL->write_comment("Derive odds(inputB) =< evens(inputB) + 1");
        plcxns.inputB_odds_leq_evens_plus1 = derive_odds_leq_evens_plus_1(inputB, plcxns.sortedness_inputB);
    }
 
    if(plcxns.inputA_odds_leq_evens_plus1 == 0){
        PL->write_comment("Derive odds(inputA) =< evens(inputA) + 1");
        plcxns.inputA_odds_leq_evens_plus1 = derive_odds_leq_evens_plus_1(inputA, plcxns.sortedness_inputA);
    }

    //PROOF: Derive odds(inputA) + odds(inputB) =< evens(inputA) + evens(inputB) + 2
    if(plcxns.inputs_odds_leq_evens_plus2 == 0){
        PL->write_comment("Derive odds(inputA) + odds(inputB) =< evens(inputA) + evens(inputB) + 2");
        plcxns.inputs_odds_leq_evens_plus2 = derive_odds_leq_evens_plus_2_merge_input(plcxns.inputA_odds_leq_evens_plus1, plcxns.inputB_odds_leq_evens_plus1, inputA, inputB);
    }
    
    //PROOF: Derive sum of d =< sum of e + 2
    PL->write_comment("Derive sum of d =< sum of e + 2");
    std::cout << "Derive sum of d =< sum of e + 2" << std::endl;
    PL->start_CP_derivation(plcxns.inputs_odds_leq_evens_plus2);
    PL->CP_add_constraint(plcxns_evens.input_leq_output);
    PL->CP_add_constraint(plcxns_odds.input_geq_output);
    plcxns.outputs_recursivenetworks_odds_leq_evens_plus2 = PL->end_CP_derivation();

    std::vector<VeriPB::Lit> lits_for_check; int RHS=0;
    for(int j = 0; j < e.size() && toVeriPbLit(e[j]) != zerolit; j++)
      lits_for_check.push_back(toVeriPbLit(e[j]));
    for(int j = 0; j < o.size() && toVeriPbLit(o[j]) != zerolit; j++){
      lits_for_check.push_back(toVeriPbLit(neg(o[j])));
      RHS++;
    }
    PL->check_last_constraint(lits_for_check, RHS-2);
}


template <class TSeqLit>
void SortingNetworkProoflogger::derive_sortedness_output_mergenetwork(ConstraintStoreMerge& plcxns, ConstraintStoreMerge& plcxns_evens, ConstraintStoreMerge& plcxns_odds, TSeqLit& e, TSeqLit& o, TSeqLit& outputs, TSeqLit& inputsA, TSeqLit& inputsB){
    derive_outputs_recursivemergenetworks_evens_leq_odds(plcxns, plcxns_evens, plcxns_odds, e, o, outputs, inputsA, inputsB);
    derive_outputs_recursivemergenetworks_odds_leq_evens_plus2(plcxns, plcxns_evens, plcxns_odds, e, o, outputs, inputsA, inputsB);
  
    std::vector<VeriPB::Lit> lits_for_check;

    //PROOF: Derive o_i >= e_i 
    PL->write_comment("o_i >= e_i");
    std::cout << "Derive sum of d =< sum of e + 2" << std::endl;
    for(int j = 0; j < std::min(e.size(), o.size()); j++){
      lits_for_check.clear();
      lits_for_check.push_back(toVeriPbLit(o[j])); lits_for_check.push_back(toVeriPbLit(neg(e[j]))); 
      PL->rup(lits_for_check); // TODO: Do we need to keep these somewhere to be able to remove them?
    }

    
    PL->write_comment("e_i >= o_{i + 2}");
    std::cout << "e_i >= o_{i + 2}" << std::endl;
    for(int j = 0; j < std::min(e.size()-2, o.size()); j++){
      lits_for_check.clear(); lits_for_check.push_back(toVeriPbLit(e[j])); lits_for_check.push_back(toVeriPbLit(neg(o[j+2])));
      PL->rup(lits_for_check);
    } 
    
    //PROOF: Derive sortedness of outputs
    PL->write_comment("derive sortedness of left-over outputs");
    PL->write_comment("outputs = " + sequence_to_string( outputs));
    std::cout << "derive sortedness of left-over outputs" << std::endl;
    for(int j = 0; j < outputs.size()-1 && toVeriPbLit(outputs[j+1]) != zerolit; j++){
      if(j % 2 == 1){ // sortedness of outputs[j] and outputs[j+1] was already derived by the comparator module.
        plcxns.sortedness_output.push_back(plcxns.comparatormodules[j/2].sortedness);
      }
      else{
        constraintid cxn_temp = 0;

        // The extra constraint is not necessary to be derived for the first and last sortedness constraint.
        bool firstone = j == 0;
        bool lastone = j+2 >= outputs.size() || toVeriPbLit(outputs[j+2]) == zerolit;

        if(!firstone && !lastone){
          lits_for_check.clear(); 
          lits_for_check.push_back(toVeriPbLit(outputs[j])); 
          lits_for_check.push_back(toVeriPbLit(neg(outputs[j+1]))); 
          lits_for_check.push_back(toVeriPbLit(neg(e[j/2])));
          cxn_temp = PL->rup(lits_for_check);
        }

        lits_for_check.clear(); 
        lits_for_check.push_back(toVeriPbLit(outputs[j])); 
        lits_for_check.push_back(toVeriPbLit(neg(outputs[j+1])));
        plcxns.sortedness_output.push_back(PL->rup(lits_for_check));

        if(cxn_temp != 0)
          PL->delete_constraint(cxn_temp);

      }
    }
}

template<class TSeqLit>
void SortingNetworkProoflogger::derive_UB_mergenetwork_output(ConstraintStoreMerge& plcxns, TSeqLit& outputs){
    // We assume here that if the number of inputs is larger than the UB on the inputs, 
    // and thus the constraint UB is needed to delete wires, the UB constraint on the inputs was derived before by 
    // calling the function  derive_UB_for_recursive_mergenetworks. 
    // Note that for example a merge network with three real input-wires, the fourth wire is deleted. 
    // Proving the deletion of this wire is done by RUP with respect to the reified constraint of the fourth wire.
    if(plcxns.cxnUBinputs != 0){ 
        PL->write_comment("Derive UB = " + std::to_string(plcxns.UB) + " on output variables.");
        PL->start_CP_derivation(plcxns.cxnUBinputs);
        PL->CP_add_constraint(plcxns.input_geq_output);
        PL->end_CP_derivation();

        std::vector<VeriPB::Lit> lits_for_check; int RHS=0;
        for(int i = 0; i < outputs.size() && toVeriPbLit(outputs[i]) != zerolit; i++){
            lits_for_check.push_back(toVeriPbLit(neg(outputs[i])));
            RHS++;
        }
        PL->check_last_constraint(lits_for_check, RHS-plcxns.UB);
    }
}

template<class TLit>
void SortingNetworkProoflogger::remove_wire_mergenetwork_output(ConstraintStoreMerge& plcxns, TLit& removed_wire){
    // Output-wires that can be deleted due to the extra created input-wires are RUP with respect to the reified constraints defining the output-wire.
    // Output-wires that can be deleted because of the UB are RUP with respect to the sortedness of the output-wires and the UB constraint on the output-wires.
    // Only need to derive the removal of non-zero wires.
    if(toVeriPbLit(removed_wire) != zerolit){
        plcxns.removed_wires.push_back(toVeriPbLit(removed_wire));

        std::vector<VeriPB::Lit> lits_for_check;
        lits_for_check.push_back(toVeriPbLit(neg(removed_wire)));
        plcxns.constraints_removed_wires.push_back(PL->rup(lits_for_check));
    }
}

template <class TSeqLit>
void SortingNetworkProoflogger::update_input_equals_output_mergenetwork_after_removing_wires(ConstraintStoreMerge& plcxns, TSeqLit& inputA, TSeqLit& inputB, TSeqLit& output){
    if(plcxns.constraints_removed_wires.size() > 0){
        assert(plcxns.input_geq_output != 0);
        
        PL->write_comment("Update the input >= output constraint");
        std::cout << "Update the input >= output constraint" << std::endl;
        constraintid old_input_geq_output = plcxns.input_geq_output;
        PL->start_CP_derivation(old_input_geq_output);
        for(int i = 0; i < plcxns.removed_wires.size(); i++) //weakening because linkingVar[j] is false and at the right hand side of >= (occurs negatively in normalized constraint)
            PL->CP_weakening(variable(plcxns.removed_wires[i]));
        plcxns.input_geq_output = PL->end_CP_derivation();

        std::vector<VeriPB::Lit> lits_for_check; int RHS=0;
        for(int i = 0; i < inputA.size() && toVeriPbLit(inputA[i]) != zerolit; i++ ){
            lits_for_check.push_back(toVeriPbLit( inputA[i]));
        }
        for(int i = 0; i < inputB.size() && toVeriPbLit(inputB[i]) != zerolit; i++ ){
            lits_for_check.push_back(toVeriPbLit(inputB[i]));
        }
        for(int i = 0; i < output.size() && toVeriPbLit(output[i]) != zerolit; i++ ){
            lits_for_check.push_back(toVeriPbLit(neg(output[i])));
            RHS++;
        }
        PL->check_last_constraint(lits_for_check, RHS);

        PL->delete_constraint(old_input_geq_output);

        PL->write_comment("Update the input =< output constraint");
        std::cout << "Update the input =< output constraint" << std::endl;
        constraintid old_input_leq_output = plcxns.input_leq_output;
        PL->start_CP_derivation(old_input_leq_output);
        for(int j = 0; j <  plcxns.constraints_removed_wires.size(); j++)
            PL->CP_add_constraint(plcxns.constraints_removed_wires[j]);
        plcxns.input_leq_output = PL->end_CP_derivation();

        lits_for_check.clear(); RHS=0;
        for(int i = 0; i < inputA.size() && toVeriPbLit(inputA[i]) != zerolit; i++ ){
            lits_for_check.push_back(toVeriPbLit(neg(inputA[i])));
            RHS++;
        }
        for(int i = 0; i < inputB.size() && toVeriPbLit(inputB[i]) != zerolit; i++ ){
            lits_for_check.push_back(toVeriPbLit(neg(inputB[i])));
            RHS++;
        }
        for(int i = 0; i < output.size() && toVeriPbLit(output[i]) != zerolit; i++ ){
            lits_for_check.push_back(toVeriPbLit(output[i]));
        }
        PL->check_last_constraint(lits_for_check, RHS);

        PL->delete_constraint(old_input_leq_output);        
    }
}

void SortingNetworkProoflogger::remove_sortedness_constraints_mergenetwork_after_removing_wires(ConstraintStoreMerge& plcxns){
    for(int i = plcxns.sortedness_output.size() - plcxns.removed_wires.size(); i < plcxns.sortedness_output.size(); i++ )
        PL->delete_constraint(plcxns.sortedness_output[i]);
    plcxns.sortedness_output.resize(plcxns.sortedness_output.size()-plcxns.removed_wires.size());
}

template <class TSeqLit, class TSeqCxnId>
constraintid SortingNetworkProoflogger::derive_evens_leq_odds(TSeqLit& seq, TSeqCxnId& sortedness_seq){
    assert(seq.size() > 0);  

    std::string comment = "seq = " ;
    for(int i = 0; i < seq.size(); i++)
        comment += PL->to_string(seq[i]) + " ";
    comment += " sortedness_seq.size() = " + std::to_string(sortedness_seq.size());
    PL->write_comment(comment);

    
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

    std::string comment = "leftlits = ";
    for(int i = 0; i < leftlits.size(); i++)
        comment += PL->to_string(leftlits[i]) + " ";
    comment += "; rightlits = ";
    for(int i = 0; i < rightlits.size(); i++)
        comment += PL->to_string(rightlits[i]) + " ";
    PL->write_comment(comment);

    std::vector<VeriPB::Lit> lits_for_check; int RHS=0; // TODO: RHS should be of type weight, which should be declared in the SolverTypesInt file.

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

    std::vector<VeriPB::Lit> lits_for_check; int RHS=0;
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


template<class TSeqLit>
std::string SortingNetworkProoflogger::sequence_to_string(TSeqLit& lits){
    std::string res = "< " + (lits.size() > 0 ? PL->to_string(lits[0]) : "");
    for(int i = 1; i < lits.size(); i++)
        res += ", " + PL->to_string(lits[i]);
    return res + ">"; 
}