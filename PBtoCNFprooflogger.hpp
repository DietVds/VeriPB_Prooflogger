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
void PBtoCNFprooflogger::derive_UB_on_recursion_inputs(constraintid& UB_left_node, constraintid& UB_right_node,
                                    constraintid& UB_current_node, 
                                     TSeqLit& leafsLeft, TSeqInt& weightsLeft, TSeqLit& leafsRight, TSeqInt& weightsRight, int& UB){

    PL->write_comment("Derive UB for recursion in PB-CNF translation. ");                     
    cuttingplanes_derivation cpder_UBleft = PL->CP_constraintid(UB_current_node);
    for(int i = 0; i < leafsRight.size(); i++)
        cpder_UBleft = PL->CP_weakening(cpder_UBleft, variable(leafsRight[i]));
    UB_left_node = PL->write_CP_derivation(cpder_UBleft); 

    std::vector<VeriPB::Lit> lits; std::vector<int> weights; int RHS = 0;
    for(int i = 0; i < leafsLeft.size(); i++){
        lits.push_back(neg(toVeriPbLit(leafsLeft[i])));
        weights.push_back(weightsLeft[i]);
        RHS += weightsLeft[i];
    }
    RHS = RHS - UB;
    PL->check_last_constraint(lits, weights, RHS);

    cuttingplanes_derivation cpder_UBright = PL->CP_constraintid(UB_current_node);
    for(int i = 0; i < leafsLeft.size(); i++)
        cpder_UBright = PL->CP_weakening(cpder_UBright, variable(leafsLeft[i]));
    UB_left_node = PL->write_CP_derivation(cpder_UBright); 

    lits.clear(); weights.clear(); RHS = 0;
    for(int i = 0; i < leafsRight.size(); i++){
        lits.push_back(neg(toVeriPbLit(leafsRight[i])));
        weights.push_back(weightsRight[i]);
        RHS += weightsRight[i];
    }
    RHS = RHS - UB;
    PL->check_last_constraint(lits, weights, RHS);
    


}


template <class TSeqLit>
void PBtoCNFprooflogger::derive_UB_on_recursion_inputs(constraintid& UB_left_node, constraintid& UB_right_node,
                                    constraintid& UB_current_node, 
                                    TSeqLit& leafsLeft, TSeqLit& leafsRight, int& UB){

    std::vector<VeriPB::Lit> vLeafsLeft; std::vector<int> weightsLeft; 
    std::vector<VeriPB::Lit> vLeafsRight; std::vector<int> weightsRight; 

    for(int i = 0; i < leafsLeft.size(); i++){
        vLeafsLeft.push_back(toVeriPbLit(leafsLeft[i]));
        weightsLeft.push_back(1);
    }

    for(int i = 0; i < leafsRight.size(); i++){
        vLeafsRight.push_back(toVeriPbLit(leafsRight[i]));
        weightsRight.push_back(1);
    }

    derive_UB_on_recursion_inputs(UB_left_node, UB_right_node,
                                    UB_current_node, 
                                    leafsLeft, weightsLeft, leafsRight, weightsRight, UB);

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
void PBtoCNFprooflogger::deriveInputEqOutputBA(constraintid& inputLeqOutput, constraintid& inputGeqOutput, TLit& a, TLit& b, TLit& c, TLit& carry, TLit& sum){
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
    
    inputLeqOutput = PL->write_CP_derivation(cp_inputLEQoutput);

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

    // Derive a + b + c >= s + 2c
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

    inputGeqOutput = PL->write_CP_derivation(cp_inputGEQoutput);

    lits.clear(); weights.clear();
    if(l1 != zerolit){
        lits.push_back(l1); weights.push_back(1);
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
  
}