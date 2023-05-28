#include "PBtoCNFprooflogger.h"

template <class TLit>
void PBtoCNFprooflogger::define_zerolit(TLit& zero){
    zerolit = toVeriPbLit(zero);

    PL->write_comment("one = " + PL->to_string(neg(zerolit)));
    
    std::vector<VeriPB::Lit> lits; 
    lits.push_back(neg(zerolit));
    substitution<VeriPB::Var> witness; witness.push_back({variable(zerolit), is_negated(zerolit) ? true : false});

    def_one = PL->redundanceBasedStrengthening(lits, 1, witness );
}


// Functions to use when the PB-CNF Translation creates a binary tree, derives variables with specific properties for its children and then merges the children into the parent node.

template <class TSeqLit, class TSeqInt>
constraintid PBtoCNFprooflogger::derive_leaves_leq_outputvars_after_binary_recursion(constraintid& leaves_leq_outputs_leftrecursion, constraintid& leaves_leq_outputs_rightrecursion, constraintid& outputs_recursion_leq_outputs, TSeqLit& leaves, TSeqInt& weightsleaves, TSeqLit& output, TSeqInt& weightsOutput){
    PL->write_comment("Derive leaves =< outputvars");
    cuttingplanes_derivation cpder = PL->CP_constraintid(outputs_recursion_leq_outputs);

    if(leaves_leq_outputs_leftrecursion != 0)
        cpder = PL->CP_addition(cpder, PL->CP_constraintid(leaves_leq_outputs_leftrecursion));
    if(leaves_leq_outputs_rightrecursion != 0)
        cpder = PL->CP_addition(cpder, PL->CP_constraintid(leaves_leq_outputs_rightrecursion));

    constraintid cxn_id = PL->write_CP_derivation(cpder);

    // Check derived constraint
    std::vector<VeriPB::Lit> lits_cxn; std::vector<int> weights_cxn; int RHS = 0;
    for(int i = 0; i < leaves.size(); i++){
        if(toVeriPbLit(leaves[i]) != zerolit){
            lits_cxn.push_back(neg(toVeriPbLit(leaves[i])));
            weights_cxn.push_back(weightsleaves[i]);
            RHS+=weightsleaves[i];
        }
    }
    for(int i= 0; i < output.size(); i ++){
        lits_cxn.push_back(toVeriPbLit(output[i]));
        weights_cxn.push_back(weightsOutput[i]);
    }
    PL->check_last_constraint(lits_cxn, weights_cxn, RHS);

    return cxn_id;
}

template <class TSeqLit, class TSeqInt>
constraintid PBtoCNFprooflogger::derive_leaves_geq_outputvars_after_binary_recursion(constraintid& leaves_geq_outputs_leftrecursion, constraintid& leaves_geq_outputs_rightrecursion, constraintid& outputs_recursion_geq_outputs, TSeqLit& leaves, TSeqInt& weightsleaves, TSeqLit& output, TSeqInt& weightsOutput){
    PL->write_comment("Derive leaves >= outputvars");
    
    cuttingplanes_derivation cpder = PL->CP_constraintid(outputs_recursion_geq_outputs);

    if(leaves_geq_outputs_leftrecursion != 0)
        cpder = PL->CP_addition(cpder, PL->CP_constraintid(leaves_geq_outputs_leftrecursion));
    if(leaves_geq_outputs_rightrecursion != 0)
        cpder = PL->CP_addition(cpder, PL->CP_constraintid(leaves_geq_outputs_rightrecursion));

    constraintid cxn_id = PL->write_CP_derivation(cpder);

    // Check constraint
    std::vector<VeriPB::Lit> lits_cxn; std::vector<int> weights_cxn; int RHS = 0;
    for(int i = 0; i < leaves.size(); i++){
        if(toVeriPbLit(leaves[i]) != zerolit) {
            lits_cxn.push_back(toVeriPbLit(leaves[i]));
            weights_cxn.push_back(weightsleaves[i]);
        }
    }
    for(int i= 0; i < output.size(); i ++){
        lits_cxn.push_back(neg(toVeriPbLit(output[i])));
        weights_cxn.push_back(weightsOutput[i]);
        RHS+=weightsOutput[i];
    }
    PL->check_last_constraint(lits_cxn, weights_cxn, RHS);

    return cxn_id;
}

template <class TSeqLit, class TSeqInt>
void PBtoCNFprooflogger::derive_UB_on_recursion_inputs(constraintid& UB_left_node, constraintid& UB_right_node,
                                    constraintid& UB_current_node, 
                                     TSeqLit& leavesLeft, TSeqInt& weightsLeft, TSeqLit& leavesRight, TSeqInt& weightsRight){

    PL->write_comment("Derive UB for recursion in PB-CNF translation. UB = " + std::to_string(PL->get_best_objective_value() ));      
    // Left recursion

    cuttingplanes_derivation cpder_UBleft = PL->CP_constraintid(UB_current_node);
    for(int i = 0; i < leavesRight.size(); i++)
        cpder_UBleft = PL->CP_weakening(cpder_UBleft, variable(leavesRight[i]));
    

    // Check derivation
    std::vector<VeriPB::Lit> lits; std::vector<int> weights; int RHS = 0;
    for(int i = 0; i < leavesLeft.size(); i++){
        lits.push_back(neg(toVeriPbLit(leavesLeft[i])));
        weights.push_back(weightsLeft[i]);
        RHS += weightsLeft[i];
    }
    RHS = RHS - PL->get_best_objective_value() + 1;

    //if(RHS > 0){
        PL->write_comment("Derive UB for leafs of the left recursion in PB-CNF translation: " + sequence_to_string(leavesLeft, weightsLeft)); 
        UB_left_node = PL->write_CP_derivation(cpder_UBleft); 
        PL->check_last_constraint(lits, weights, RHS);
    // }

    // Right recursion

    cuttingplanes_derivation cpder_UBright = PL->CP_constraintid(UB_current_node);
    for(int i = 0; i < leavesLeft.size(); i++)
        cpder_UBright = PL->CP_weakening(cpder_UBright, variable(leavesLeft[i]));
    

    // Check derivation
    lits.clear(); weights.clear(); RHS = 0;
    for(int i = 0; i < leavesRight.size(); i++){
        lits.push_back(neg(toVeriPbLit(leavesRight[i])));
        weights.push_back(weightsRight[i]);
        RHS += weightsRight[i];
    }
    RHS = RHS - PL->get_best_objective_value() + 1;

    //if(RHS > 0){
        PL->write_comment("Derive UB for leafs of the left recursion in PB-CNF translation: " + sequence_to_string(leavesRight, weightsRight)); 
        UB_right_node = PL->write_CP_derivation(cpder_UBright); 
        PL->check_last_constraint(lits, weights, RHS);
    //}
}


template <class TSeqLit>
void PBtoCNFprooflogger::derive_UB_on_recursion_inputs(constraintid& UB_left_node, constraintid& UB_right_node,
                                    constraintid& UB_current_node, 
                                    TSeqLit& leavesLeft, TSeqLit& leavesRight){

    std::vector<VeriPB::Lit> vleavesLeft; std::vector<int> weightsLeft; 
    std::vector<VeriPB::Lit> vleavesRight; std::vector<int> weightsRight; 

    for(int i = 0; i < leavesLeft.size(); i++){
        vleavesLeft.push_back(toVeriPbLit(leavesLeft[i]));
        weightsLeft.push_back(1);
    }

    for(int i = 0; i < leavesRight.size(); i++){
        vleavesRight.push_back(toVeriPbLit(leavesRight[i]));
        weightsRight.push_back(1);
    }

    derive_UB_on_recursion_inputs(UB_left_node, UB_right_node,
                                    UB_current_node, 
                                    leavesLeft, weightsLeft, leavesRight, weightsRight);

}

// If we have derived the constraint outputs <= leaves and leaves =< UB, then we can derive outputs =< UB
template <class TSeqLit, class TSeqInt>
constraintid PBtoCNFprooflogger::derive_UB_on_outputliterals(constraintid& UB_leaves, constraintid& leaves_geq_outputs,
                                    TSeqLit& outputs, TSeqInt& weights ){
    
    PL->write_comment("Derive UB on output-variables of encoding.");
    cuttingplanes_derivation cpder = PL->CP_addition(PL->CP_constraintid(UB_leaves), PL->CP_constraintid(leaves_geq_outputs));
    constraintid cxn_id = PL->write_CP_derivation(cpder);

    std::vector<VeriPB::Lit> lits; std::vector<int> lit_weights; int RHS=0;
    for(int i = 0; i < weights.size(); i ++){
        lits.push_back(neg(toVeriPbLit(outputs[i])));
        lit_weights.push_back(weights[i]);
        RHS += weights[i];
    }
    RHS = RHS - PL->get_best_objective_function() + 1;

    PL->check_last_constraint(lits, lit_weights, RHS);
    return cxn_id;
}

// Binary Adders

template <class TLit>
void PBtoCNFprooflogger::reifyCarryBA(TLit& a, TLit& b, TLit& c, TLit& carry ){
    PL->write_comment("Reification of " + PL->to_string(carry) + " <-> " + PL->to_string(a) + " + " + PL->to_string(b) + " + " + PL->to_string(c) + " >= 2");

    std::vector<VeriPB::Lit> lits; 

    lits.push_back(toVeriPbLit(a)); 
    lits.push_back(toVeriPbLit(b));
    lits.push_back(toVeriPbLit(c));

    PL->reificationLiteralLeftImpl(toVeriPbLit(carry), lits, 2, true);
    PL->reificationLiteralRightImpl(toVeriPbLit(carry), lits, 2, true);
}

template <class TLit>
void PBtoCNFprooflogger::reifySumBA(TLit& a, TLit& b, TLit& c, TLit& carry, TLit& sum ){
    PL->write_comment("Reification of " + PL->to_string(sum) + " <-> " + PL->to_string(a) + " + " + PL->to_string(b) + " + " + PL->to_string(c) + "2 "  + PL->to_string(neg(carry)) + " >= 3");

    std::vector<VeriPB::Lit> lits; std::vector<int> weights;

    lits.push_back(toVeriPbLit(a)); weights.push_back(1);
    lits.push_back(toVeriPbLit(b)); weights.push_back(1);
    lits.push_back(toVeriPbLit(c)); weights.push_back(1);
    lits.push_back(toVeriPbLit(neg(carry))); weights.push_back(2);
    PL->reificationLiteralLeftImpl(toVeriPbLit(sum), lits, weights, 3, true);
    PL->reificationLiteralRightImpl(toVeriPbLit(sum), lits, weights, 3, true);
}

template <class TLit>
constraintid PBtoCNFprooflogger::deriveInputLeqOutputBA(TLit& a, TLit& b, TLit& c, TLit& carry, TLit& sum){
    VeriPB::Lit l1 = toVeriPbLit(a);
    VeriPB::Lit l2 = toVeriPbLit(b);
    VeriPB::Lit l3 = toVeriPbLit(c);
    VeriPB::Lit vcarry = toVeriPbLit(carry);
    VeriPB::Lit vsum = toVeriPbLit(sum);

    // Derive a + b + c =< s + 2c
    PL->write_comment("Derive " + (l1 != zerolit ? PL->to_string(l1) : "") + (l2 != zerolit? (l1 != zerolit ? " + " : "") + PL->to_string(l2) : "") + (l3 != zerolit ? (l1 != zerolit || l2 != zerolit ? " + " : "") + PL->to_string(l3) : "") + " =< " + PL->to_string(vsum) + " +  2 " + PL->to_string(vcarry));

    cuttingplanes_derivation cp_inputLEQoutput;
    cp_inputLEQoutput = PL->CP_constraintid(PL->getReifiedConstraintLeftImpl(variable(vcarry))); 
    cp_inputLEQoutput = PL->CP_multiplication(cp_inputLEQoutput, 2);
    cp_inputLEQoutput = PL->CP_addition(cp_inputLEQoutput, PL->CP_constraintid(PL->getReifiedConstraintLeftImpl(variable(vsum))));
    cp_inputLEQoutput = PL->CP_division(cp_inputLEQoutput, 3);
    if(int(l1 == zerolit) + int(l2==zerolit) + int(l3 == zerolit) >= 3)
    cp_inputLEQoutput = PL->CP_addition(cp_inputLEQoutput, PL->CP_multiplication(PL->CP_literal_axiom(zerolit), 3));
    else if(int(l1 == zerolit) + int(l2==zerolit) + int(l3 == zerolit) >= 2)
    cp_inputLEQoutput = PL->CP_addition(cp_inputLEQoutput, PL->CP_multiplication(PL->CP_literal_axiom(zerolit), 2));
    else if(int(l1 == zerolit) + int(l2 == zerolit) + int(l3 == zerolit) >= 1)
        cp_inputLEQoutput = PL->CP_addition(cp_inputLEQoutput, PL->CP_literal_axiom(zerolit));
    
    constraintid inputLeqOutput = PL->write_CP_derivation(cp_inputLEQoutput);

    std::vector<VeriPB::Lit> lits; std::vector<int> weights; int RHS=0;
    if(l1 != zerolit){
        lits.push_back(neg(l1)); weights.push_back(1); RHS++;
    }
    if(l2 != zerolit){
        lits.push_back(neg(l2)); weights.push_back(1); RHS++;
    }
    if(l3 != zerolit){
        lits.push_back(neg(l3)); weights.push_back(1); RHS++;
    }
    lits.push_back(vsum); weights.push_back(1);
    lits.push_back(vcarry); weights.push_back(2);
    PL->check_last_constraint(lits, weights, RHS);

    return inputLeqOutput;
}

template <class TLit>
constraintid PBtoCNFprooflogger::deriveInputGeqOutputBA(TLit& a, TLit& b, TLit& c, TLit& carry, TLit& sum){
    VeriPB::Lit l1 = toVeriPbLit(a);
    VeriPB::Lit l2 = toVeriPbLit(b);
    VeriPB::Lit l3 = toVeriPbLit(c);
    VeriPB::Lit vcarry = toVeriPbLit(carry);
    VeriPB::Lit vsum = toVeriPbLit(sum);

    PL->write_comment("Derive " + (l1 != zerolit ? PL->to_string(l1) : "") + (l2 != zerolit? (l1 != zerolit ? " + " : "") + PL->to_string(l2) : "") + (l3 != zerolit ? (l1 != zerolit || l2 != zerolit ? " + " : "") + PL->to_string(l3) : "") + " >= " + PL->to_string(vsum) + " +  2 " + PL->to_string(vcarry));


    cuttingplanes_derivation cp_inputGEQoutput;
    cp_inputGEQoutput = PL->CP_constraintid(PL->getReifiedConstraintRightImpl(variable(vcarry))); 
    cp_inputGEQoutput = PL->CP_multiplication(cp_inputGEQoutput, 2);
    cp_inputGEQoutput = PL->CP_addition(cp_inputGEQoutput, PL->CP_constraintid(PL->getReifiedConstraintRightImpl(variable(vsum))));
    cp_inputGEQoutput = PL->CP_division(cp_inputGEQoutput, 3);
    if(int(l1 == zerolit) + int(l2==zerolit) + int(l3 == zerolit) >= 3)
        cp_inputGEQoutput = PL->CP_addition(cp_inputGEQoutput, PL->CP_multiplication(PL->CP_constraintid(def_one), 3));
    else if(int(l1 == zerolit) + int(l2==zerolit) + int(l3 == zerolit) >= 2)
        cp_inputGEQoutput = PL->CP_addition(cp_inputGEQoutput, PL->CP_multiplication(PL->CP_constraintid(def_one), 2));
    else if(int(l1 == zerolit) + int(l2 == zerolit) + int(l3 == zerolit) >= 1)
        cp_inputGEQoutput = PL->CP_addition(cp_inputGEQoutput, PL->CP_constraintid(def_one));

    constraintid inputGeqOutput = PL->write_CP_derivation(cp_inputGEQoutput);

    std::vector<VeriPB::Lit> lits; std::vector<int> weights;
    if(l1 != zerolit){
        lits.push_back(toVeriPbLit(l1)); weights.push_back(1);
    }
    if(l2 != zerolit){
        lits.push_back(l2); weights.push_back(1);
    }
    if(l3 != zerolit){
        lits.push_back(l3); weights.push_back(1);
    }
    lits.push_back(neg(vsum)); weights.push_back(1);
    lits.push_back(neg(vcarry)); weights.push_back(2);
    PL->check_last_constraint(lits, weights, 3);

    return inputGeqOutput;
}

// For a binary adder with input two sequences <l> and <r> of literals and as output the sequence of literals <o>, 
    // derive the inequalities representing \sum_i 2^i * <l>_i + \sum_i 2^i * <r>_i = \sum_i 2^i * <o>_i
template <class TSeqLit>
constraintid PBtoCNFprooflogger::deriveBASeqInputLeqOutput(std::vector<constraintid>& singleAdders_inputLeqOutput, TSeqLit& litsleft, TSeqLit& litsright,  TSeqLit& outputs ){

    PL->write_comment("Binary Adder on Sequences: Derive inputs =< outputs");
     
    cuttingplanes_derivation cpder;
    cpder = PL->CP_constraintid(singleAdders_inputLeqOutput[0]);

    for(int i = 1; i < singleAdders_inputLeqOutput.size(); i++ ){
      cpder = PL->CP_addition(cpder, PL->CP_multiplication(PL->CP_constraintid(singleAdders_inputLeqOutput[i]), pow(2, i)));
    }

    constraintid cxnid = PL->write_CP_derivation(cpder);

    std::vector<VeriPB::Lit> lits_cxn; std::vector<int> weights_cxn; int RHS = 0;

    for(int i = 0; i < litsleft.size(); i++){
        if(toVeriPbLit(litsleft[i]) != zerolit ){
            lits_cxn.push_back(neg(toVeriPbLit(litsleft[i])));
            weights_cxn.push_back(pow(2,i));
            RHS += pow(2,i);
        }
    }
        
    for(int i = 0; i < litsright.size(); i++){
        if(toVeriPbLit(litsright[i]) != zerolit ){
            lits_cxn.push_back(neg(toVeriPbLit(litsright[i])));
            weights_cxn.push_back(pow(2,i));
            RHS += pow(2,i);
        }
    }
    
    for(int i = 0; i < outputs.size(); i++){
        lits_cxn.push_back(toVeriPbLit(outputs[i]));
        weights_cxn.push_back(pow(2,i));
    }

    PL->check_last_constraint(lits_cxn, weights_cxn, RHS);

    return cxnid;
}

template <class TSeqLit>
constraintid PBtoCNFprooflogger::deriveBASeqInputGeqOutput(std::vector<constraintid>& singleAdders_inputGeqOutput, TSeqLit& litsleft,TSeqLit& litsright,TSeqLit& outputs ){
    PL->write_comment("Binary Adder on Sequences: Derive inputs >= outputs");
     
    cuttingplanes_derivation cpder;
    cpder = PL->CP_constraintid(singleAdders_inputGeqOutput[0]);

    for(int i = 1; i < singleAdders_inputGeqOutput.size(); i++ ){
      cpder = PL->CP_addition(cpder, PL->CP_multiplication(PL->CP_constraintid(singleAdders_inputGeqOutput[i]), pow(2, i)));
    }

    constraintid cxnid = PL->write_CP_derivation(cpder);

    std::vector<VeriPB::Lit> lits_cxn; std::vector<int> weights_cxn; int RHS = 0;

    for(int i = 0; i < litsleft.size(); i++){
        if(toVeriPbLit(litsleft[i]) != zerolit ){
            lits_cxn.push_back(toVeriPbLit(litsleft[i]));
            weights_cxn.push_back(pow(2,i));
        }
    }
        
    for(int i = 0; i < litsright.size(); i++){
        if(toVeriPbLit(litsright[i]) != zerolit){
            lits_cxn.push_back(toVeriPbLit(litsright[i]));
            weights_cxn.push_back(pow(2,i));
        }
    }
    
    for(int i = 0; i < outputs.size(); i++){
        lits_cxn.push_back(neg(toVeriPbLit(outputs[i])));
        weights_cxn.push_back(pow(2,i));
        RHS += pow(2,i);
    }

    PL->check_last_constraint(lits_cxn, weights_cxn, RHS);

    return cxnid;
}

// Helper functions for commenting
template<class TSeqLit>
std::string PBtoCNFprooflogger::sequence_to_string(TSeqLit& lits){
    std::string res = "< " + (lits.size() > 0 ? PL->to_string(lits[0]) : "");
    for(int i = 1; i < lits.size(); i++)
        res += ", " + PL->to_string(lits[i]);
    return res + ">"; 
}

template<class TSeqLit, class TSeqInt>
std::string PBtoCNFprooflogger::sequence_to_string(TSeqLit& lits, TSeqInt& weights){
    assert(lits.size() == weights.size());

    std::string res = "< " + (lits.size() > 0 ? ("(" + PL->to_string(lits[0]) + ", " +  std::to_string(weights[0]) + ")") : "");
    for(int i = 1; i < lits.size(); i++)
        res += ", (" + PL->to_string(lits[i]) + ", " + std::to_string(weights[i]) + ")";
    return res + ">"; 
}
