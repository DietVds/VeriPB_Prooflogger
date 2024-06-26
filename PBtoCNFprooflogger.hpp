#include "PBtoCNFprooflogger.h"

template <class TLit>
void PBtoCNFprooflogger::define_zerolit(TLit& zero){
    zerolit = toVeriPbLit(zero);

    PL->write_comment("one = " + PL->to_string(neg(zerolit)));
    
    std::vector<VeriPB::Lit> lits; 
    lits.push_back(neg(zerolit));
    substitution witness = PL->get_new_substitution();
    PL->add_boolean_assignment(witness, variable(zerolit), is_negated(zerolit));

    PL->store_meaningful_name(variable(zerolit), is_negated(zerolit) ? "true": "false");

    def_one = PL->redundanceBasedStrengthening(lits, 1, witness );
}

// Functions for deriving sortedness

/**
 * If there exists the following literals: 
 *      l_j <-> X >= aj+b
 *      l_{j+1} <-> X >= a(j+1)+b
 * Then, it is possible to derive the sortedness constraint l_j >= l_{j+1} by cutting planes derivation starting from C<-(l_j) and C->(l_{j+1}). 
*/
template <class TLit>
constraintid PBtoCNFprooflogger::derive_sortedness_from_reified_constraints(TLit& lj, TLit& ljp1, wght a){
    cuttingplanes_derivation cpder;

    cuttingplanes_derivation reifleft_lj = PL->CP_constraintid(PL->getReifiedConstraintLeftImpl(variable(lj)));
    cuttingplanes_derivation reifright_ljp1 = PL->CP_constraintid(PL->getReifiedConstraintRightImpl(variable(ljp1)));

    cpder = PL->CP_addition(reifleft_lj, reifright_ljp1);
    cpder = PL->CP_division(cpder, 1+a);
    cpder = PL->CP_saturation(cpder);

    constraintid cxn = PL->write_CP_derivation(cpder);

    std::vector<VeriPB::Lit> litsC; 

    litsC.push_back(toVeriPbLit(lj));
    litsC.push_back(neg(toVeriPbLit(ljp1)));

    PL->check_last_constraint(litsC);

    return cxn;
}

// Functions to use when the PB-CNF Translation creates a binary tree, derives variables with specific properties for its children and then merges the children into the parent node.

// TODO: rewrite in terms of chain_IO_constraints_binary_recursion
template <class TSeqLit, class TSeqWght>
constraintid PBtoCNFprooflogger::derive_leaves_leq_outputvars_after_binary_recursion(constraintid& leaves_leq_outputs_leftrecursion, constraintid& leaves_leq_outputs_rightrecursion, constraintid& outputs_recursion_leq_outputs, TSeqLit& leaves, TSeqWght& weightsleaves, TSeqLit& output, TSeqWght& weightsOutput){
    PL->write_comment("Derive leaves =< outputvars");
    cuttingplanes_derivation cpder = PL->CP_constraintid(outputs_recursion_leq_outputs);

    if(leaves_leq_outputs_leftrecursion != 0)
        cpder = PL->CP_addition(cpder, PL->CP_constraintid(leaves_leq_outputs_leftrecursion));
    if(leaves_leq_outputs_rightrecursion != 0)
        cpder = PL->CP_addition(cpder, PL->CP_constraintid(leaves_leq_outputs_rightrecursion));

    constraintid cxn_id = PL->write_CP_derivation(cpder);

    // Check derived constraint
    std::vector<VeriPB::Lit> lits_cxn; std::vector<wght> weights_cxn; wght RHS = 0;
    for(int i = 0; i < leaves.size(); i++){
        if(toVeriPbLit(leaves[i]) != zerolit){
            lits_cxn.push_back(neg(toVeriPbLit(leaves[i])));
            weights_cxn.push_back(weightsleaves[i]);
            RHS+=weightsleaves[i];
        }
    }
    for(int i= 0; i < output.size(); i ++){
        if(toVeriPbLit(output[i]) != zerolit){
            lits_cxn.push_back(toVeriPbLit(output[i]));
            weights_cxn.push_back(weightsOutput[i]);
        }
    }
    PL->check_last_constraint(lits_cxn, weights_cxn, RHS);

    return cxn_id;
}

template <class TSeqLit, class TSeqWght>
constraintid PBtoCNFprooflogger::derive_leaves_geq_outputvars_after_binary_recursion(constraintid& leaves_geq_outputs_leftrecursion, constraintid& leaves_geq_outputs_rightrecursion, constraintid& outputs_recursion_geq_outputs, TSeqLit& leaves, TSeqWght& weightsleaves, TSeqLit& output, TSeqWght& weightsOutput){
    PL->write_comment("Derive leaves >= outputvars");
    
    PL->start_intCP_derivation(&cp, outputs_recursion_geq_outputs);
    
    if(leaves_geq_outputs_leftrecursion != 0)
        PL->intCP_add_constraint(&cp, leaves_geq_outputs_leftrecursion);
    if(leaves_geq_outputs_rightrecursion != 0)
        PL->intCP_add_constraint(&cp, leaves_geq_outputs_rightrecursion);
        
    constraintid cxn_id = PL->end_intCP_derivation(&cp);
    
    // Check constraint
    std::vector<VeriPB::Lit> lits_cxn; std::vector<wght> weights_cxn; wght RHS = 0;
    for(int i = 0; i < leaves.size(); i++){
        if(toVeriPbLit(leaves[i]) != zerolit) {
            lits_cxn.push_back(toVeriPbLit(leaves[i]));
            weights_cxn.push_back(weightsleaves[i]);
        }
    }
    for(int i= 0; i < output.size() && toVeriPbLit(output[i]) != zerolit; i ++){
        if(toVeriPbLit(output[i]) != zerolit){
            lits_cxn.push_back(neg(toVeriPbLit(output[i])));
            weights_cxn.push_back(weightsOutput[i]);
            RHS+=weightsOutput[i];
        }
    }
    PL->check_last_constraint(lits_cxn, weights_cxn, RHS);

    return cxn_id;
}

/**
 * Let X1, X2, Y1, Y2 and O be linear terms over pairwise disjoint sets of literals and let >=< be either >= or =<. 
 * If the following constraints have been derived: X1 =< Y1, X2 =< Y2, Y1 U Y2 =< O, then it is possible to derive X1 U X2 =< O. 
*/
template <class TSeqLit, class TSeqWght>
constraintid PBtoCNFprooflogger::chain_IO_constraints_binary_recursion_leq(constraintid& X1_leq_Y1, constraintid& X2_leq_Y2, constraintid& Y1_U_Y2_leq_O, TSeqLit& litsX1, TSeqWght& wghtsX1, TSeqLit& litsX2, TSeqWght& wghtsX2, TSeqLit& litsO, TSeqWght& wghtsO ){
    cuttingplanes_derivation cpder = PL->CP_constraintid(Y1_U_Y2_leq_O);

    if(X1_leq_Y1 != 0)
        cpder = PL->CP_addition(cpder, PL->CP_constraintid(X1_leq_Y1));
    if(X2_leq_Y2 != 0)
        cpder = PL->CP_addition(cpder, PL->CP_constraintid(X2_leq_Y2));

    constraintid cxn_id = PL->write_CP_derivation(cpder);

    // Check constraint
    // TODO: rewrite by adding equals rule for non-normalized constraints to VeriPBProoflogger.
    std::vector<VeriPB::Lit> lits_cxn; std::vector<wght> weights_cxn; wght RHS = 0;
    for(int i = 0; i < litsX1.size(); i++){
        if(toVeriPbLit(litsX1[i]) != zerolit && toVeriPbLit(litsX1[i]) != neg(zerolit)) {
            lits_cxn.push_back(neg(toVeriPbLit(litsX1[i])));
            weights_cxn.push_back(wghtsX1[i]);
            RHS += wghtsX1[i];
        }
    }
    for(int i = 0; i < litsX2.size(); i++){
        if(toVeriPbLit(litsX2[i]) != zerolit && toVeriPbLit(litsX2[i]) != neg(zerolit)) {
            lits_cxn.push_back(neg(toVeriPbLit(litsX2[i])));
            weights_cxn.push_back(wghtsX2[i]);
            RHS += wghtsX2[i];
        }
    }
    for(int i= 0; i < litsO.size(); i ++){
        if(toVeriPbLit(litsO[i]) != zerolit && toVeriPbLit(litsO[i]) != neg(zerolit)){
            lits_cxn.push_back(toVeriPbLit(litsO[i]));
            weights_cxn.push_back(wghtsO[i]);
        }
    }
    PL->check_last_constraint(lits_cxn, weights_cxn, RHS);

    return cxn_id;
}

/**
 * Let X1, X2, Y1, Y2 and O be linear terms over pairwise disjoint sets of literals and let >=< be either >= or =<. 
 * If the following constraints have been derived: X1 >= Y1, X2 >= Y2, Y1 U Y2 >= O, then it is possible to derive X1 U X2 >= O. 
*/
template <class TSeqLit, class TSeqWght>
constraintid PBtoCNFprooflogger::chain_IO_constraints_binary_recursion_geq(constraintid& X1_geq_Y1, constraintid& X2_geq_Y2, constraintid& Y1_U_Y2_geq_O, TSeqLit& litsX1, TSeqWght& wghtsX1, TSeqLit& litsX2, TSeqWght& wghtsX2, TSeqLit& litsO, TSeqWght& wghtsO ){
    cuttingplanes_derivation cpder = PL->CP_constraintid(Y1_U_Y2_geq_O);

    if(X1_geq_Y1 != 0)
        cpder = PL->CP_addition(cpder, PL->CP_constraintid(X1_geq_Y1));
    if(X2_geq_Y2 != 0)
        cpder = PL->CP_addition(cpder, PL->CP_constraintid(X2_geq_Y2));

    constraintid cxn_id = PL->write_CP_derivation(cpder);

    // Check constraint
    std::vector<VeriPB::Lit> lits_cxn; std::vector<wght> weights_cxn; wght RHS = 0;
    for(int i = 0; i < litsX1.size(); i++){
        if(toVeriPbLit(litsX1[i]) != zerolit && toVeriPbLit(litsX1[i]) != neg(zerolit)) {
            lits_cxn.push_back(toVeriPbLit(litsX1[i]));
            weights_cxn.push_back(wghtsX1[i]);
        }
    }
    for(int i = 0; i < litsX2.size(); i++){
        if(toVeriPbLit(litsX2[i]) != zerolit && toVeriPbLit(litsX2[i]) != neg(zerolit)) {
            lits_cxn.push_back(toVeriPbLit(litsX2[i]));
            weights_cxn.push_back(wghtsX2[i]);
        }
    }
    for(int i= 0; i < litsO.size(); i ++){
        if(toVeriPbLit(litsO[i]) != zerolit && toVeriPbLit(litsO[i]) != neg(zerolit)){
            lits_cxn.push_back(toVeriPbLit(neg(litsO[i])));
            weights_cxn.push_back(wghtsO[i]);
            RHS += wghtsO[i];
        }
    }
    PL->check_last_constraint(lits_cxn, weights_cxn, RHS);

    return cxn_id;
}




template <class TSeqLit, class TSeqWght>
void PBtoCNFprooflogger::derive_UB_on_recursion_inputs(constraintid& UB_left_node, constraintid& UB_right_node,
                                    constraintid& UB_current_node, 
                                     TSeqLit& leavesLeft, TSeqWght& wghtLeavesL, TSeqLit& leavesRight, TSeqWght& wghtLeavesR, wght UB){
    PL->write_comment("Derive UB for recursion in PB-CNF translation. UB = " + std::to_string(UB));      
    // Left recursion

    PL->start_intCP_derivation(&cp, UB_current_node);
    
    for(int i = 0; i < leavesRight.size(); i++)
        PL->intCP_add_literal_axiom(&cp, leavesRight[i], wghtLeavesR[i]);

    // Check derivation
    std::vector<VeriPB::Lit> lits; std::vector<wght> weights; wght RHS = 0;
    for(int i = 0; i < leavesLeft.size(); i++){
        if(toVeriPbLit(leavesLeft[i]) != zerolit){
            lits.push_back(neg(toVeriPbLit(leavesLeft[i])));
            weights.push_back(wghtLeavesL[i]);
            RHS += wghtLeavesL[i];
        }
    }
    // TODO: check if this works for all edge cases with difference between + 1 and only using UB

    if(RHS > UB)
        RHS = RHS - UB; 
    else // take the maximum value of the weights as UB.
        RHS = 0;

    PL->write_comment("Derive UB for leafs of the left recursion in PB-CNF translation: " + sequence_to_string(leavesLeft, wghtLeavesL)); 
    UB_left_node = PL->end_intCP_derivation(&cp); 
    PL->check_last_constraint(lits, weights, RHS);

    // Right recursion

    PL->start_intCP_derivation(&cp, UB_current_node);
    for(int i = 0; i < leavesLeft.size(); i++)
        PL->intCP_add_literal_axiom(&cp, leavesLeft[i], wghtLeavesL[i]);

    // Check derivation
    lits.clear(); weights.clear(); RHS = 0;
    for(int i = 0; i < leavesRight.size(); i++){
        if(toVeriPbLit(leavesRight[i]) != zerolit){
            lits.push_back(neg(toVeriPbLit(leavesRight[i])));
            weights.push_back(wghtLeavesR[i]);
            RHS += wghtLeavesR[i];
        }
    }

    if(RHS > UB)
        RHS = RHS - UB;
    else // take the maximum value of the weights as UB.
        RHS = 0;
    
    PL->write_comment("Derive UB for leafs of the right recursion in PB-CNF translation: " + sequence_to_string(leavesRight, wghtLeavesR)); 
    UB_right_node = PL->end_intCP_derivation(&cp);
    PL->check_last_constraint(lits, weights, RHS);
}


template <class TSeqLit>
void PBtoCNFprooflogger::derive_UB_on_recursion_inputs(constraintid& UB_left_node, constraintid& UB_right_node,
                                    constraintid& UB_current_node, 
                                    TSeqLit& leavesLeft, TSeqLit& leavesRight, wght UB){

    std::vector<VeriPB::Lit> vleavesLeft; std::vector<wght> wghtLeavesL; 
    std::vector<VeriPB::Lit> vleavesRight; std::vector<wght> wghtLeavesR; 

    for(int i = 0; i < leavesLeft.size(); i++){
        vleavesLeft.push_back(toVeriPbLit(leavesLeft[i]));
        wghtLeavesL.push_back(1);
    }

    for(int i = 0; i < leavesRight.size(); i++){
        vleavesRight.push_back(toVeriPbLit(leavesRight[i]));
        wghtLeavesR.push_back(1);
    }

    derive_UB_on_recursion_inputs(UB_left_node, UB_right_node,
                                    UB_current_node, 
                                    leavesLeft, wghtLeavesL, leavesRight, wghtLeavesR, UB);
}

// If we have derived the constraint outputs <= leaves and leaves =< UB, then we can derive outputs =< UB
template <class TSeqLit, class TSeqWght>
constraintid PBtoCNFprooflogger::derive_UB_on_outputliterals(constraintid& UB_leaves, constraintid& leaves_geq_outputs,
                                    TSeqLit& outputs, TSeqWght& weights, wght UB ){
    
    PL->write_comment("Derive UB on output-variables of encoding.");
    PL->start_intCP_derivation(&cp, UB_leaves);
    PL->intCP_add_constraint(&cp, leaves_geq_outputs);
    constraintid cxn_id = PL->end_intCP_derivation(&cp);
    
    std::vector<VeriPB::Lit> lits; std::vector<wght> lit_weights; wght RHS=0;
    for(int i = 0; i < weights.size(); i ++){
        if(toVeriPbLit(outputs[i]) != zerolit){
            lits.push_back(neg(toVeriPbLit(outputs[i])));
            lit_weights.push_back(weights[i]);
            RHS += weights[i];
        }
    }
    if(RHS > UB)
        RHS = RHS - UB;
    else
        RHS = 0;

    PL->check_last_constraint(lits, lit_weights, RHS);
    return cxn_id;
}


// Functions to use when the encoding has as output variables a unary representation of the sum of the leaves. 

template <class TSeqLit> 
constraintid PBtoCNFprooflogger::derive_leaves_leq_unary_k_from_reification(TSeqLit& countingLits, wght k, TSeqLit& clause, bool trivialcountingvars){
    cuttingplanes_derivation cpder;
    

    if(toVeriPbLit(countingLits[trivialcountingvars? k : k-1]) == zerolit){
        cpder =  PL->CP_constraintid(def_one);
    }
    else{
        cpder = PL->CP_constraintid(PL->get_model_improving_constraint());
        cpder = PL->CP_addition(cpder, PL->CP_constraintid( PL->getReifiedConstraintRightImpl(variable(countingLits[trivialcountingvars? k : k-1]))));
        cpder = PL->CP_division(cpder, k -  PL->get_best_objective_value() + 1);
        cpder = PL->CP_saturation(cpder);
    }
    constraintid cxn = PL->write_CP_derivation(cpder);
    PL->check_last_constraint(clause);
    return cxn;
}


// Binary Adders

template <class TLit>
void PBtoCNFprooflogger::reifyCarryBA(TLit& a, TLit& b, TLit& c, TLit& carry ){
    PL->write_comment("Reification of " + PL->to_string(carry) + " <-> " + PL->to_string(a) + " + " + PL->to_string(b) + " + " + PL->to_string(c) + " >= 2");

    std::vector<VeriPB::Lit> lits; 

    lits.push_back(toVeriPbLit(a)); 
    lits.push_back(toVeriPbLit(b));
    lits.push_back(toVeriPbLit(c));

    PL->reificationLiteralLeftImpl(toVeriPbLit(carry), lits, (wght)2, true);
    PL->reificationLiteralRightImpl(toVeriPbLit(carry), lits, (wght)2, true);
}

template <class TLit>
void PBtoCNFprooflogger::reifySumBA(TLit& a, TLit& b, TLit& c, TLit& carry, TLit& sum ){
    PL->write_comment("Reification of " + PL->to_string(sum) + " <-> " + PL->to_string(a) + " + " + PL->to_string(b) + " + " + PL->to_string(c) + " 2 "  + PL->to_string(neg(carry)) + " >= 3");

    std::vector<VeriPB::Lit> lits; std::vector<wght> weights;

    lits.push_back(toVeriPbLit(a)); weights.push_back(1);
    lits.push_back(toVeriPbLit(b)); weights.push_back(1);
    lits.push_back(toVeriPbLit(c)); weights.push_back(1);
    lits.push_back(toVeriPbLit(neg(carry))); weights.push_back(2);
    PL->reificationLiteralLeftImpl(toVeriPbLit(sum), lits, weights, (wght)3, true);
    PL->reificationLiteralRightImpl(toVeriPbLit(sum), lits, weights, (wght)3, true);
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

    std::vector<VeriPB::Lit> lits; std::vector<wght> weights; wght RHS=0;
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

    PL->start_intCP_derivation(&cp,PL->getReifiedConstraintRightImpl(variable(vcarry)) );
    PL->intCP_multiply(&cp, 2);
    PL->intCP_add_constraint(&cp, PL->getReifiedConstraintRightImpl(variable(vsum)));
    PL->intCP_divide(&cp, 3);
    PL->intCP_add_constraint(&cp, def_one, int(l1 == zerolit) + int(l2==zerolit) + int(l3 == zerolit));
    constraintid inputGeqOutput = PL->end_intCP_derivation(&cp);

    std::vector<VeriPB::Lit> lits; std::vector<wght> weights;
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

    wght p2i = 1;

    for(int i = 1; i < singleAdders_inputLeqOutput.size(); i++ ){
      p2i = p2i * 2;
      cpder = PL->CP_addition(cpder, PL->CP_multiplication(PL->CP_constraintid(singleAdders_inputLeqOutput[i]), p2i));
    }

    constraintid cxnid = PL->write_CP_derivation(cpder);

    std::vector<VeriPB::Lit> lits_cxn; std::vector<wght> weights_cxn; wght RHS = 0;

    for(int i = 0; i < litsleft.size(); i++){
        p2i = (i == 0) ? 1 : p2i * 2;
        if(toVeriPbLit(litsleft[i]) != zerolit ){
            lits_cxn.push_back(neg(toVeriPbLit(litsleft[i])));
            weights_cxn.push_back(p2i);
            RHS += p2i;
        }
    }
        
    for(int i = 0; i < litsright.size(); i++){
        p2i = (i == 0) ? 1 : p2i * 2;
        if(toVeriPbLit(litsright[i]) != zerolit ){
            lits_cxn.push_back(neg(toVeriPbLit(litsright[i])));
            weights_cxn.push_back(p2i);
            RHS += p2i;
        }
    }
    
    for(int i = 0; i < outputs.size(); i++){
        p2i = (i == 0) ? 1 : p2i * 2;
        lits_cxn.push_back(toVeriPbLit(outputs[i]));
        weights_cxn.push_back(p2i);
    }

    PL->check_last_constraint(lits_cxn, weights_cxn, RHS);

    return cxnid;
}

template <class TSeqLit>
constraintid PBtoCNFprooflogger::deriveBASeqInputGeqOutput(std::vector<constraintid>& singleAdders_inputGeqOutput, TSeqLit& litsleft,TSeqLit& litsright,TSeqLit& outputs ){
    PL->write_comment("Binary Adder on Sequences: Derive inputs >= outputs");
     
    PL->start_intCP_derivation(&cp, singleAdders_inputGeqOutput[0]);

    wght p2i = 1;
    for(int i = 1; i < singleAdders_inputGeqOutput.size(); i++ ){
      p2i = p2i*(wght)2;
      PL->intCP_add_constraint(&cp, singleAdders_inputGeqOutput[i], p2i);
    }

    constraintid cxnid = PL->end_intCP_derivation(&cp);

    std::vector<VeriPB::Lit> lits_cxn; std::vector<wght> weights_cxn; wght RHS = 0;

    for(int i = 0; i < litsleft.size(); i++){
        p2i = (i == 0) ? 1 : p2i * 2;
        if(toVeriPbLit(litsleft[i]) != zerolit ){
            lits_cxn.push_back(toVeriPbLit(litsleft[i]));
            weights_cxn.push_back(p2i);
        }
    }
        
    for(int i = 0; i < litsright.size(); i++){
        p2i = (i == 0) ? 1 : p2i * 2;
        if(toVeriPbLit(litsright[i]) != zerolit){
            lits_cxn.push_back(toVeriPbLit(litsright[i]));
            weights_cxn.push_back(p2i);
        }
    }
    
    for(int i = 0; i < outputs.size(); i++){
        p2i = (i == 0) ? 1 : p2i * 2;
        lits_cxn.push_back(neg(toVeriPbLit(outputs[i])));
        weights_cxn.push_back(p2i);
        RHS += p2i;
    }

    PL->check_last_constraint(lits_cxn, weights_cxn, RHS);

    return cxnid;
}

/*
Modulo Totalizer:
Toru Ogawa, YangYang Liu, Ryuzo Hasegawa, Miyuki Koshimura, Hiroshi Fujita,
"Modulo Based CNF Encoding of Cardinality Constraints and Its Application to
MaxSAT Solvers",
ICTAI 2013.
*/

template < class TSeqLit>
VeriPB::Lit PBtoCNFprooflogger::getRemainderLiteral(TSeqLit& countingLiteralsMTO, wght j){
    return toVeriPbLit( countingLiteralsMTO[j]);
}

template <class TSeqLit>
VeriPB::Lit PBtoCNFprooflogger::getQuotientLiteral(TSeqLit& countingLiteralsMTO, wght j, wght div){
    return toVeriPbLit( countingLiteralsMTO[div+j]);
}

template <class TSeqLit>
wght PBtoCNFprooflogger::getNrOfQuotientLiterals(TSeqLit& countingLiteralsMTO, wght div){
    return countingLiteralsMTO.size() - div - 1;
}


template <class TLit, class TSeqLit, class TSeqWght >
void PBtoCNFprooflogger::reifyQuotientLiteralMTO(TLit& quotientliteral, TSeqLit& leaves, TSeqWght& leaves_wght, wght j, wght div){
    PL->reificationLiteralRightImpl(quotientliteral, leaves, leaves_wght, j*div, true);
    PL->reificationLiteralLeftImpl(quotientliteral, leaves, leaves_wght, j*div, true);
}

template <class TLit, class TSeqLit, class TSeqWght>
void PBtoCNFprooflogger::reifyRemainderLiteralMTO(TLit& remainderliteral, TSeqLit& leaves, TSeqWght& leaves_wght, TSeqLit& countingLiteralsMTO, wght j, wght div){
    std::vector<VeriPB::Lit> litsC; std::vector<wght> wghtsC;

    for(int i = 0; i < leaves.size(); i++){
        litsC.push_back(toVeriPbLit(leaves[i]));
        wghtsC.push_back(leaves_wght[i]);
    }

    wght sigma = getNrOfQuotientLiterals(countingLiteralsMTO, div); // Nr of quotient literals

    for(int j=1; j <= sigma; j++){
        litsC.push_back(toVeriPbLit(neg(getQuotientLiteral(countingLiteralsMTO, j, div))));
        wghtsC.push_back(div);
    }

    PL->reificationLiteralRightImpl(toVeriPbLit(remainderliteral), litsC, wghtsC, j + sigma * div, true );
    PL->reificationLiteralLeftImpl(toVeriPbLit(remainderliteral), litsC, wghtsC, j + sigma * div, true );
}

template <class TLit, class TSeqLit>
void PBtoCNFprooflogger::reifyCarryLiteralMTO(TLit& carryLit, TSeqLit& countingLiteralsMTOLeft, TSeqLit& countingLiteralsMTORight, wght div){
    std::vector<VeriPB::Lit> litsC;

    VeriPB::Lit r;
    for(int j = 1; j < div; j++){
        r = getRemainderLiteral(countingLiteralsMTOLeft, j);
        if(r != zerolit)
            litsC.push_back(r);
    }
    for(int j = 1; j < div; j++){
        r = getRemainderLiteral(countingLiteralsMTORight, j);
        if(r != zerolit)
            litsC.push_back(r);
    }
    
    PL->reificationLiteralRightImpl(toVeriPbLit(carryLit), litsC, div, true);
    PL->reificationLiteralLeftImpl(toVeriPbLit(carryLit), litsC, div, true);
}

template <class TSeqLit, class TSeqWght>
constraintid PBtoCNFprooflogger::derive_leaves_leq_countinglits_MTO(TSeqLit& countingLiteralsMTO, TSeqLit& leaves, TSeqWght& wght_leaves, wght div){
    cuttingplanes_derivation cpder;

    wght sigma = getNrOfQuotientLiterals(countingLiteralsMTO, div);
    

    // Derive the constraint pH + (p-1) >= X, or equivalently, pH + ~X >= |X| - p + 1
     wght sizeX = 0;
    for(int i = 0; i < wght_leaves.size(); i++){
        sizeX += wght_leaves[i];
    }

    std::vector<VeriPB::Lit> litsBase; std::vector<wght> wghtsBase;
    for(int i = 0; i < leaves.size(); i++){
        litsBase.push_back(toVeriPbLit(neg(leaves[i])));
        wghtsBase.push_back(wght_leaves[i]);
    }

    // Base case in derivation of pH + (p-1) >= X,
    VeriPB::Lit h_j = getQuotientLiteral(countingLiteralsMTO, sigma , div);
    cpder = PL->CP_constraintid(PL->getReifiedConstraintLeftImpl(variable(h_j)));
    if(sizeX - sigma * div + 1 <= div)
        cpder = PL->CP_addition(cpder, PL->CP_multiplication(PL->CP_literal_axiom(h_j), div - (sizeX - sigma * div + 1)));
    
    litsBase.push_back(h_j); wghtsBase.push_back(div);

    constraintid cxn_jp1 = PL->write_CP_derivation(cpder);
    constraintid cxn_j;
    constraintid cxn_reif_j;
    constraintid case1, case2;

    constraintid base;

    cuttingplanes_derivation cpder_case1, cpder_case2;

    for(int j = sigma-1; j > 0; j--){
        h_j = getQuotientLiteral(countingLiteralsMTO, j, div);

        litsBase.push_back(h_j); wghtsBase.push_back(div);

        cxn_reif_j = PL->getReifiedConstraintLeftImpl(variable(h_j));
        
        PL->write_comment("case 1");

        cpder_case1 = PL->CP_constraintid(cxn_jp1);
        cpder_case1 = PL->CP_addition(cpder_case1, PL->CP_multiplication(PL->CP_literal_axiom(neg(h_j)), sizeX - j * div + 1 - div));
        case1 = PL->write_CP_derivation(cpder_case1);

        PL->write_comment("case 2");

        cpder_case2 = PL->CP_constraintid(cxn_reif_j);
        VeriPB::Lit h_i;

        for(int i = j; i <= sigma; i++){
            h_i = getQuotientLiteral(countingLiteralsMTO, i, div);
            cpder_case2 = PL->CP_addition(cpder_case2, PL->CP_multiplication(PL->CP_literal_axiom(h_i), div));
        }
        case2 = PL->write_CP_derivation(cpder_case2);

        cxn_j = PL->prove_by_casesplitting(litsBase, wghtsBase, sizeX-j*div+1, case1, case2);

        PL->delete_constraint_by_id(case1);
        PL->delete_constraint_by_id(case2);
        PL->delete_constraint_by_id(cxn_jp1);

        cxn_jp1 = cxn_j;
    }
    
    base = cxn_jp1;

    PL->equals_rule(base, litsBase, wghtsBase, sizeX-div+1);

    PL->write_comment("Derive R + p H + ~X >= |X|");

    // Derive the constraint R + p H + ~X >= |X| by iteratively building constraint 
    // p H + ~X + \sum_{i=j+1}^{p-1} r_i >+ |X| - j
    cpder = PL->CP_constraintid(base);

    for(int j = div-1; j >= 1; j--){
        PL->write_CP_derivation(cpder); cpder = PL->CP_constraintid(-1);
        cpder = PL->CP_multiplication(cpder, sizeX-j);
        cpder = PL->CP_addition(cpder, PL->CP_constraintid(PL->getReifiedConstraintLeftImpl(toVeriPbVar(variable(getRemainderLiteral(countingLiteralsMTO, j))))));
        cpder = PL->CP_division(cpder, sizeX-j+1);
    }
    constraintid cxn = PL->write_CP_derivation(cpder);

    // Check constraint
    std::vector<VeriPB::Lit> litsC; std::vector<wght> wghtsC; wght RHS = 0;
    
    for(int j = 1; j <= sigma; j++){
        litsC.push_back(getQuotientLiteral(countingLiteralsMTO, j, div));
        wghtsC.push_back(div);
    }
    for(int j = 1; j < div; j++){
        litsC.push_back(getRemainderLiteral(countingLiteralsMTO, j));
        wghtsC.push_back(1);
    }
    for(int i = 0; i < leaves.size(); i++){
        litsC.push_back(toVeriPbLit(neg(leaves[i])));
        wghtsC.push_back(wght_leaves[i]);
        RHS += wght_leaves[i];
    }
    PL->check_last_constraint(litsC, wghtsC, RHS);

    PL->delete_constraint_by_id(base);

    return cxn;
}

template <class TSeqLit, class TSeqWght>
constraintid PBtoCNFprooflogger::derive_leaves_geq_countinglits_MTO(TSeqLit& countingLiteralsMTO, TSeqLit& leaves, TSeqWght& wght_leaves, wght div){
    wght sigma = getNrOfQuotientLiterals(countingLiteralsMTO, div); // Number of quotient literals.

    cuttingplanes_derivation cpder, cpder2;

    //cpder = PL->CP_constraintid(PL->getReifiedConstraintRightImpl(variable(getRemainderLiteral(countingLiteralsMTO, 1))));

    std::vector<VeriPB::Lit> litsC; std::vector<wght> wghtsC; wght RHS=0;

    // Proving X >= pH
    constraintid constraint_for_j = PL->getReifiedConstraintRightImpl(variable(getQuotientLiteral(countingLiteralsMTO, 1, div)));
    for(int j = 1; j < sigma; j++){
        // Derive the case that represents the constraint ~h_j+1 -> p \sum_{1 =< i =< j+1} ~h_j + X >= (j+1)p
        cpder = PL->CP_constraintid(constraint_for_j);
        cpder2 = PL->CP_multiplication(PL->CP_literal_axiom(getQuotientLiteral(countingLiteralsMTO, j+1, div)), j*div);
        cpder = PL->CP_addition(cpder, cpder2);
        constraintid case_hjp1_false = PL->write_CP_derivation(cpder);

        // Derive the case that represents the constraint h_j+1 -> p \sum_{1 =< i =< j+1} ~h_j + X >= (j+1)p
        cpder = PL->CP_constraintid(PL->getReifiedConstraintRightImpl(variable(getQuotientLiteral(countingLiteralsMTO, j+1, div))));
        for(int i = 1; i <= j+1; i++ ){
            cpder2 = PL->CP_multiplication(PL->CP_literal_axiom(neg(getQuotientLiteral(countingLiteralsMTO, i, div))), div);
            cpder = PL->CP_addition(cpder, cpder2);
        }
        constraintid case_hjp1_true = PL->write_CP_derivation(cpder);

        // Derive \sum_{1 =< i =< j+1} ~h_j + X >= (j+1)p by using case splitting with previously derived constraints.
        litsC.clear(); wghtsC.clear(); RHS=(j+1)*div;
        
        for(int i = 1; i <= j + 1; i++ ){
            litsC.push_back(neg(getQuotientLiteral(countingLiteralsMTO, i, div)));
            wghtsC.push_back(div);
        }
        for(int i = 0; i < leaves.size(); i++){
            litsC.push_back(toVeriPbLit(leaves[i]));
            wghtsC.push_back(wght_leaves[i]);
        }

        PL->delete_constraint_by_id(constraint_for_j);
        constraint_for_j = PL->prove_by_casesplitting(litsC, wghtsC, RHS, case_hjp1_false, case_hjp1_true);
        PL->delete_constraint_by_id(case_hjp1_false);
        PL->delete_constraint_by_id(case_hjp1_true);
    }

    cpder = PL->CP_constraintid(constraint_for_j);

    // Induction steps for j for the constraint \sum_{1 =< i =< j} ~r_i + p~H + X >= p*|H| + j
    for(int j = 0; j < div-1; j++){
        cpder = PL->CP_multiplication(cpder, j + sigma * div);
        cpder = PL->CP_addition(cpder, PL->CP_constraintid(PL->getReifiedConstraintRightImpl(variable(getRemainderLiteral(countingLiteralsMTO, j+1)))));
        cpder = PL->CP_division(cpder, j + sigma * div + 1);
    }

    constraintid cxn = PL->write_CP_derivation(cpder);

    // Check constraint
    litsC.clear(); wghtsC.clear(); RHS = 0;

    for(int i = 0; i < leaves.size(); i++){
        litsC.push_back(toVeriPbLit(leaves[i]));
        wghtsC.push_back(wght_leaves[i]);
    }
    
    VeriPB::Lit l;

    for(wght j = 1; j <= sigma; j++){
        litsC.push_back(neg(getQuotientLiteral(countingLiteralsMTO, j, div)));
        wghtsC.push_back(div);
        RHS+=div;
    }
    
    for(wght j = 1; j < div; j++){
        litsC.push_back(toVeriPbLit(neg(getRemainderLiteral(countingLiteralsMTO, j))));
        wghtsC.push_back(1);
        RHS++;
    }
    
    PL->check_last_constraint(litsC, wghtsC, RHS);

    PL->delete_constraint_by_id(constraint_for_j);
    
    return cxn;
}


template <class TLit, class TSeqLit, class TSeqWght>
void PBtoCNFprooflogger::derive_modulo_sum_constraints(constraintid& out_modulo_sum_constraint_quotient, constraintid& out_modulo_sum_constraint_remainder, constraintid modsum_input_geq_output, constraintid modsum_input_leq_output, TSeqLit& countingLits, TSeqLit& countingLitsL, TSeqLit& countingLitsR, TLit& carry, TSeqLit& leavesL, TSeqWght& wghtLeavesL, TSeqLit& leavesR, TSeqWght& wghtLeavesR, wght divisor){
    VeriPB::Lit r, q;
    std::vector<VeriPB::Lit> litsC; std::vector<wght> wghtsC; wght RHS=0;

    // Derive H' + H'' + c =< H
    PL->write_comment("Derive H' + H'' + c =< H");
    cuttingplanes_derivation cpder;
    cpder = PL->CP_addition(PL->CP_constraintid(modsum_input_leq_output), PL->CP_constraintid(PL->getReifiedConstraintRightImpl(variable(carry))));
    for(int j = 1; j <= divisor-1; j++){
      r = getRemainderLiteral(countingLits, j);
      cpder = PL->CP_weakening(cpder, neg(r), 1);
    }
    cpder = PL->CP_division(cpder, divisor);
    out_modulo_sum_constraint_quotient = PL->write_CP_derivation(cpder);

    litsC.clear(); RHS=0;
    for(int j = 1; j <= getNrOfQuotientLiterals(countingLits, divisor); j++){
      litsC.push_back(toVeriPbLit(getQuotientLiteral(countingLits, j, divisor)));
    }
    for(int j = 1; j <= getNrOfQuotientLiterals(countingLitsL, divisor); j++){
      litsC.push_back(neg(toVeriPbLit(getQuotientLiteral(countingLitsL, j, divisor))));
      RHS++;
    }
    for(int j = 1; j <= getNrOfQuotientLiterals(countingLitsR, divisor); j++){
      litsC.push_back(neg(toVeriPbLit(getQuotientLiteral(countingLitsR, j, divisor))));
      RHS++;
    }
    litsC.push_back(toVeriPbLit(neg(carry))); RHS++;
    
    PL->check_last_constraint(litsC, RHS);

    // Derive H' + H'' + c >= H
    PL->write_comment("Derive H' + H'' + c >= H");
    PL->write_comment("H' = " + sequence_to_string(countingLitsL) + " H'' = " + sequence_to_string(countingLitsR) + " H = " + sequence_to_string(countingLits) + " carry = " + PL->to_string(carry));
    cpder.clear();

    wght remainder_size_left = 0; 
    wght remainder_size_right = 0;

    for(int j = 1; j <= divisor-1; j++){
      if(getRemainderLiteral(countingLitsL, j) != zerolit)
        remainder_size_left++;
      if(getRemainderLiteral(countingLitsR, j) != zerolit)
        remainder_size_right++;
    }

    if(remainder_size_left + remainder_size_right >= divisor){
      cpder = PL->CP_addition(PL->CP_constraintid(modsum_input_geq_output), PL->CP_constraintid(PL->getReifiedConstraintLeftImpl(variable(carry))));
    }
    else{
      cpder = PL->CP_constraintid(modsum_input_geq_output);
      for(int j = 1; j <= divisor-1; j++){
        r = getRemainderLiteral(countingLitsL, j);
        if(r != zerolit)
          cpder = PL->CP_weakening(cpder, neg(r), 1);
          // cpder = PL->CP_addition(cpder, PL->CP_literal_axiom(neg(r))); // Weakening
      }
      for(int j = 1; j <= divisor-1; j++){
        r = getRemainderLiteral(countingLitsR, j);
        if(r != zerolit)
          cpder = PL->CP_weakening(cpder, neg(r), 1);
          // cpder = PL->CP_addition(cpder, PL->CP_literal_axiom(neg(r)));// Weakening
      }
      cpder = PL->CP_addition(cpder, PL->CP_literal_axiom(carry));
    }
    for(int j = 1; j <= divisor-1; j++){ 
      r = getRemainderLiteral(countingLits, j);
      cpder = PL->CP_weakening(cpder, r, 1);
      // cpder = PL->CP_addition(cpder, PL->CP_literal_axiom(r)); // Weakening
    }
    cpder = PL->CP_division(cpder, divisor);
    constraintid modsum_quotient_geq_cxn = PL->write_CP_derivation(cpder);

    litsC.clear(); RHS=0;
    for(int j = 1; j <= getNrOfQuotientLiterals(countingLits, divisor); j++){
      litsC.push_back(toVeriPbLit(neg(getQuotientLiteral(countingLits, j, divisor))));
      RHS++;
    }
    for(int j = 1; j <= getNrOfQuotientLiterals(countingLitsL, divisor); j++){
      litsC.push_back(toVeriPbLit(getQuotientLiteral(countingLitsL, j, divisor)));
    }
    for(int j = 1; j <= getNrOfQuotientLiterals(countingLitsR, divisor); j++){
      litsC.push_back(toVeriPbLit(getQuotientLiteral(countingLitsR, j, divisor)));
    }
    litsC.push_back(toVeriPbLit(carry)); //RHS++;

    PL->check_last_constraint(litsC, RHS);

    // Derivation of remainder sum constraint
    // R + pC >= R' + R''

    PL->write_comment("Derive R + pC >= R' + R''");

    cpder.clear();

    cpder = PL->CP_addition(PL->CP_multiplication(PL->CP_constraintid(modsum_quotient_geq_cxn), divisor), PL->CP_constraintid(modsum_input_leq_output));
    out_modulo_sum_constraint_remainder = PL->write_CP_derivation(cpder);

    litsC.clear(); RHS = 0; wghtsC.clear();
    for(int j = 1; j < divisor; j++){
      r = getRemainderLiteral(countingLitsL, j);
      if(r != zerolit){
        litsC.push_back(neg(r));
        wghtsC.push_back(1);
        RHS++;
      }
      r = getRemainderLiteral(countingLitsR, j);
      if(r != zerolit){
        litsC.push_back(neg(r));
        wghtsC.push_back(1);
        RHS++;
      }
      r = getRemainderLiteral(countingLits, j);
      litsC.push_back(r);
      wghtsC.push_back(1);
    }
    litsC.push_back(toVeriPbLit(carry));
    wghtsC.push_back(divisor);

    PL->check_last_constraint(litsC, wghtsC, RHS);
}


/*
Totalizer:
Olivier Bailleux and Yacine Boufkhad,
"Efficient CNF Encoding of Boolean Cardinality Constraints",
CP 2003, LNCS 2833, pp.108-122, 2003
*/

/**
 * Derives the totalizer clauses. 
 * If the boolean trivialcountingvar is true, countingLitsLeft, countingLitsRight and countingLits are all of the form <~zerolit, ..., zerolit>. 
 *      These extra literals in the countingLits are not necessary, and thus some solvers do not add them.
 * If the boolean clause_contains_zerolits is true, the clause that is derived contains a zero-literal in case alpha or beta is equal to 0, else they are discarded.     
*/
template <class TSeqLit, class TSeqWght>
constraintid PBtoCNFprooflogger::derive_totalizer_clause(TSeqLit& leavesLeft, TSeqWght& weightsLeavesLeft, TSeqLit& countingLitsLeft, TSeqLit& leavesRight, TSeqWght& weightsLeavesRight,  TSeqLit& countingLitsRight, TSeqLit& countingLits, wght alpha, wght beta, wght sigma, TSeqLit& clause, bool trivialcountingvar, bool clause_contains_zerolits){
    cuttingplanes_derivation cpder;
    constraintid cxn = 0;

    if(sigma==0){
        cpder = PL->CP_constraintid(def_one);
        if(alpha == 0 && clause_contains_zerolits){
            cpder = PL->CP_addition(cpder, PL->CP_literal_axiom(zerolit));
        }
        else{
            cpder = PL->CP_addition(cpder, PL->CP_literal_axiom(countingLitsLeft[trivialcountingvar ? alpha : alpha-1]));
        }
        if(beta == 0 && clause_contains_zerolits){
            cpder = PL->CP_addition(cpder, PL->CP_literal_axiom(zerolit));
        }
        else{
            cpder = PL->CP_addition(cpder, PL->CP_literal_axiom(countingLitsRight[trivialcountingvar ? beta : beta-1]));
        }
    }
    else{
        
        cpder = PL->CP_constraintid(PL->getReifiedConstraintLeftImpl(variable(countingLits[trivialcountingvar ? sigma : sigma - 1])));

        if(alpha == 0){
            for(int i = 0; i < leavesLeft.size(); i++)
                cpder = PL->CP_weakening(cpder, leavesLeft[i], weightsLeavesLeft[i]);
                //cpder = PL->CP_weakening(cpder, variable(leavesLeft[i]));
            if(clause_contains_zerolits)
                cpder = PL->CP_addition(cpder, PL->CP_literal_axiom(zerolit));
        }
        else if(leavesLeft.size() == 1 && (trivialcountingvar ?  (countingLitsLeft.size()-2 > 1) : (countingLitsLeft.size() > 1))) 
                // Left node has only one leaf but more countingLiterals, which means a weighted instance. That case, countingLitsLeft consist of multiple times the same literal.
                // Weakening the reified constraint by adding the leave its weight minus alpha results in the same derivation as it would be for internal nodes.
            cpder = PL->CP_addition(cpder, PL->CP_multiplication(PL->CP_literal_axiom(leavesLeft[0]), weightsLeavesLeft[0]-alpha));
        else if(leavesLeft.size() > 1)
            cpder = PL->CP_addition(cpder, PL->CP_constraintid(PL->getReifiedConstraintRightImpl(variable(countingLitsLeft[trivialcountingvar ?  alpha : alpha - 1]))));

        if(beta == 0){
            for(int i = 0; i < leavesRight.size(); i++)
                cpder = PL->CP_weakening(cpder, leavesRight[i], weightsLeavesRight[i]);
                //cpder = PL->CP_weakening(cpder, variable(leavesRight[i]));
            if(clause_contains_zerolits)
                cpder = PL->CP_addition(cpder, PL->CP_literal_axiom(zerolit));
        }
        else if(leavesRight.size() == 1 && (trivialcountingvar ? (countingLitsRight.size()-2 > 1) : (countingLitsRight.size() > 1)) )
            cpder = PL->CP_addition(cpder, PL->CP_multiplication(PL->CP_literal_axiom(leavesRight[0]), weightsLeavesRight[0]-beta));
        else if(leavesRight.size() > 1)
            cpder = PL->CP_addition(cpder, PL->CP_constraintid(PL->getReifiedConstraintRightImpl(variable(countingLitsRight[trivialcountingvar ?  beta : beta - 1]))));

        cpder = PL->CP_saturation(cpder );

    }
    cxn = PL->write_CP_derivation(cpder);
    PL->check_last_constraint(clause);
    return cxn;
}

/**
 * Derives the inverse totalizer clauses. These clauses are the clauses that are called C2 in Bailleux's paper.
 * If the boolean trivialcountingvar is true, countingLitsLeft, countingLitsRight and countingLits are all of the form <~zerolit, ..., zerolit>. 
 *      These extra literals in the countingLits are not necessary, and thus some solvers do not add them.
 * If the boolean clause_contains_zerolits is true, the clause that is derived contains a zero-literal in case alpha or beta is equal to 0, else they are discarded.     
*/
template <class TSeqLit, class TSeqWght>
constraintid PBtoCNFprooflogger::derive_totalizer_inverse_clause(TSeqLit& leavesLeft, TSeqWght& weightsLeavesLeft, TSeqLit& countingLitsLeft, TSeqLit& leavesRight, TSeqWght& weightsLeavesRight,  TSeqLit& countingLitsRight, TSeqLit& countingLits, wght alpha, wght beta, wght sigma, TSeqLit& clause, bool trivialcountingvar, bool clause_contains_zerolits){
    PL->write_comment("Derive inverse totalizer clause");
    
    cuttingplanes_derivation cpder;
    constraintid cxn = 0;
    
    wght nbCountingLitsLeft = trivialcountingvar ?  countingLitsLeft.size() - 2 : countingLitsLeft.size();
    wght nbCountingLitsRight = trivialcountingvar ?  countingLitsRight.size() - 2 : countingLitsRight.size();
    wght nbCountingLits = trivialcountingvar ?  countingLits.size() - 2 : countingLits.size();

    if(sigma == nbCountingLits){
        cpder = PL->CP_constraintid(def_one);
        if(alpha == nbCountingLitsLeft && clause_contains_zerolits){
            cpder = PL->CP_addition(cpder, PL->CP_literal_axiom(zerolit));
        }
        else{
            cpder = PL->CP_addition(cpder, PL->CP_literal_axiom(neg(countingLitsLeft[trivialcountingvar ? alpha+1 : alpha])));
        }
        if(beta == nbCountingLitsRight && clause_contains_zerolits){
            cpder = PL->CP_addition(cpder, PL->CP_literal_axiom(zerolit));
        }
        else{
            cpder = PL->CP_addition(cpder, PL->CP_literal_axiom(neg(countingLitsRight[trivialcountingvar ? beta+1 : beta])));
        }
    }
    else{
        cpder = PL->CP_constraintid(PL->getReifiedConstraintRightImpl(variable(countingLits[trivialcountingvar ? sigma+1 : sigma])));

        if(alpha == nbCountingLitsLeft){
            for(int i = 0; i < leavesLeft.size(); i++)
                cpder = PL->CP_weakening(cpder, leavesLeft[i], weightsLeavesLeft[i]);
                //cpder = PL->CP_weakening(cpder, variable(leavesLeft[i]));
            if(clause_contains_zerolits)
                cpder = PL->CP_addition(cpder, PL->CP_literal_axiom(zerolit));
        }
        else if(leavesLeft.size() == 1 && (trivialcountingvar ?  (countingLitsLeft.size()-2 > 1) : (countingLitsLeft.size() > 1))) 
                // Left node has only one leaf but more countingLiterals, which means a weighted instance. That case, countingLitsLeft consist of multiple times the same literal.
                // Weakening the reified constraint by adding the leave alpha times results in the same derivation as it would be for internal nodes.
            cpder = PL->CP_addition(cpder, PL->CP_multiplication(PL->CP_literal_axiom(neg(leavesLeft[0])), alpha));
        else if(leavesLeft.size() > 1)
            cpder = PL->CP_addition(cpder, PL->CP_constraintid(PL->getReifiedConstraintLeftImpl(variable(countingLitsLeft[trivialcountingvar ?  alpha+1 : alpha]))));

        if(beta == nbCountingLitsRight){
            for(int i = 0; i < leavesRight.size(); i++)
                cpder = PL->CP_weakening(cpder, leavesRight[i], weightsLeavesRight[i]);
                //cpder = PL->CP_weakening(cpder, variable(leavesRight[i]));
            if(clause_contains_zerolits)
                cpder = PL->CP_addition(cpder, PL->CP_literal_axiom(zerolit));
        }
        else if(leavesRight.size() == 1 && (trivialcountingvar ? (countingLitsRight.size()-2 > 1) : (countingLitsRight.size() > 1)) )
            cpder = PL->CP_addition(cpder, PL->CP_multiplication(PL->CP_literal_axiom(neg(leavesRight[0])), beta));
        else if(leavesRight.size() > 1)
            cpder = PL->CP_addition(cpder, PL->CP_constraintid(PL->getReifiedConstraintLeftImpl(variable(countingLitsRight[trivialcountingvar ?  beta+1 : beta]))));

        cpder = PL->CP_saturation(cpder );
    }
    cxn = PL->write_CP_derivation(cpder);
    PL->check_last_constraint(clause);
    return cxn;
}

/*
    Functions for handling the unary representation of a value.
*/

/**
 * Given a set of counting literals L = {l_j | 1 =< j =< |X|} for which every literal l_j is reified as l_j -> \sum_{i = 1}^{|X|} a_i x_i >= j,
 * it is possible to derive the constraint \sum_{i = 1}^{|X|} l_i =<  \sum_{i = 1}^{|X|} a_i x_i .
*/

template <class TSeqLit, class TSeqWght>
constraintid PBtoCNFprooflogger::derive_input_geq_unary_output_from_output_definitions(TSeqLit& inputlits, TSeqWght& inputweights, TSeqLit& outputlits, bool trivialcountingvar){
    cuttingplanes_derivation cpder;

    assert(trivialcountingvar ? outputlits.size() > 1 : outputlits.size() > 0);
    wght size_output = trivialcountingvar ? outputlits.size()-2 : outputlits.size();

    cpder = PL->CP_constraintid(PL->getReifiedConstraintRightImpl(variable(outputlits[trivialcountingvar ? 1 : 0])));

    for(int j = 1; j < size_output; j++){ 
        cpder = PL->CP_multiplication(cpder, j);
        cpder = PL->CP_addition(cpder, PL->CP_constraintid(PL->getReifiedConstraintRightImpl(variable(outputlits[trivialcountingvar ? j+1 : j]))));
        cpder = PL->CP_division(cpder, j+1);
    }

    constraintid cxn = PL->write_CP_derivation(cpder);

    std::vector<VeriPB::Lit> lits; std::vector<wght> weights; wght RHS=0;
    for(int i = 0; i < outputlits.size(); i++){
        if(toVeriPbLit(outputlits[i]) != zerolit && toVeriPbLit(outputlits[i]) != neg(zerolit)){
            lits.push_back(toVeriPbLit(neg(outputlits[i])));
            weights.push_back(1);
            RHS++;
        }
    }
    for(int i = 0; i < inputlits.size(); i++){
        lits.push_back(toVeriPbLit(inputlits[i]));
        weights.push_back(inputweights[i]);
    }
    PL->check_last_constraint(lits, weights, RHS);

    return cxn;

}

/**
 * Given a set of counting literals L = {l_j | 1 =< j =< |X|} for which every literal l_j is reified as l_j <- \sum_{i = 1}^{|X|} a_i x_i >= j,
 * it is possible to derive the constraint \sum_{i = 1}^{|X|} l_i >=  \sum_{i = 1}^{|X|} a_i x_i .
*/
template <class TSeqLit, class TSeqWght>
constraintid PBtoCNFprooflogger::derive_input_leq_unary_output_from_output_definitions(TSeqLit& inputlits, TSeqWght& inputweights, TSeqLit& outputlits, bool trivialcountingvar){
    cuttingplanes_derivation cpder;

    assert(trivialcountingvar ? outputlits.size() > 1 : outputlits.size() > 0);
    wght size_output = trivialcountingvar ? outputlits.size()-2 : outputlits.size();

    cpder = PL->CP_constraintid(PL->getReifiedConstraintLeftImpl(variable(outputlits[trivialcountingvar ? outputlits.size()-2 : outputlits.size()-1])));

    for(int j = 1; j < size_output; j++){ 
        cpder = PL->CP_multiplication(cpder, j);
        cpder = PL->CP_addition(cpder, PL->CP_constraintid(PL->getReifiedConstraintLeftImpl(variable(outputlits[trivialcountingvar ? outputlits.size()-1-(j+1) : outputlits.size()-(j+1)]))));
        cpder = PL->CP_division(cpder, j+1);
    }

    constraintid cxn = PL->write_CP_derivation(cpder);

    std::vector<VeriPB::Lit> lits; std::vector<wght> weights; wght RHS=0;
    for(int i = 0; i < outputlits.size(); i++){
        if(toVeriPbLit(outputlits[i]) != zerolit && toVeriPbLit(outputlits[i]) != neg(zerolit)){
            lits.push_back(toVeriPbLit(outputlits[i]));
            weights.push_back(1);
        }
    }
    for(int i = 0; i < inputlits.size(); i++){
        lits.push_back(toVeriPbLit(neg(inputlits[i])));
        weights.push_back(inputweights[i]);
        RHS += inputweights[i];
    }
    PL->check_last_constraint(lits, weights, RHS);

    return cxn;
}

// Helper functions for commenting
template<class TSeqLit>
std::string PBtoCNFprooflogger::sequence_to_string(TSeqLit& lits){
    std::string res = "< " + (lits.size() > 0 ? PL->to_string(lits[0]) : "");
    for(int i = 1; i < lits.size(); i++)
        res += ", " + PL->to_string(lits[i]);
    return res + ">"; 
}

template<class TSeqLit, class TSeqWght>
std::string PBtoCNFprooflogger::sequence_to_string(TSeqLit& lits, TSeqWght& weights){
    assert(lits.size() == weights.size());

    std::string res = "< " + (lits.size() > 0 ? ("(" + PL->to_string(lits[0]) + ", " +  std::to_string(weights[0]) + ")") : "");
    for(int i = 1; i < lits.size(); i++)
        res += ", (" + PL->to_string(lits[i]) + ", " + std::to_string(weights[i]) + ")";
    return res + ">"; 
}
