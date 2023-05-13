#ifndef PbTransProoflogger_h
#define PbTransProoflogger_h

#include "VeriPBProoflogger.h"

class PBtoCNFprooflogger
{
public:
    // Constructor
    //SortingNetworkProoflogger(VeriPbProofLogger *PL);
    // template <class TLit>
    //PbTransProoflogger(VeriPbProofLogger *PL, TLit zero, constraintid def_one) : PL(PL), zerolit(toVeriPbLit(zero)), def_one(def_one) {}
    PBtoCNFprooflogger(VeriPbProofLogger *PL) : PL(PL) {};


    VeriPB::Lit zerolit;
    constraintid def_one;

    template <class TLit>
    void define_zerolit(TLit& zerolit);

    // Functions to use when the PB-CNF Translation creates a binary tree, derives variables with specific properties for its children and then merges the children into the parent node.


    /**
     * Let X be the leafs of the current node with X = X1 U X2 with X1 and X2 the leafs of a left, respectively right recursive tree to encode a PB translation. 
     * Let O, O1 and O2 be the outputs of respectively the current node, the left node and the right node. If we have derived X1 = O and X2 = O, we can also derive X = O.
     * */
    template <class TSeqLit, class TSeqInt>
    void derive_leafs_leq_outputvars_after_binary_recursion(constraintid& leafs_leq_outputs_leftrecursion, constraintid& leafs_leq_outputs_rightrecursion, 
                                                            constraintid& outputs_recursion_leq_outputs,
                                                            TSeqLit& leafs, TSeqInt& weightsLeafs, TSeqLit& output, TSeqInt& weightsOutput);
    template <class TSeqLit, class TSeqInt>
    void derive_leafs_leq_outputvars_after_binary_recursion(constraintid& leafs_leq_outputs_leftrecursion, constraintid& leafs_leq_outputs_rightrecursion, 
                                                            constraintid& outputs_recursion_leq_outputs,
                                                            TSeqLit& leafs, TSeqLit& output);

    template <class TSeqLit, class TSeqInt>
    void derive_leafs_geq_outputvars_after_binary_recursion(constraintid& leafs_geq_outputs_leftrecursion, constraintid& leafs_geq_outputs_rightrecursion, 
                                                            constraintid& outputs_recursion_geq_outputs,
                                                            TSeqLit& leafs, TSeqInt& weightsLeafs, TSeqLit& output, TSeqInt& weightsOutput);
    template <class TSeqLit, class TSeqInt>
    void derive_leafs_geq_outputvars_after_binary_recursion(constraintid& leafs_geq_outputs_leftrecursion, constraintid& leafs_geq_outputs_rightrecursion, 
                                                            constraintid& outputs_recursion_geq_outputs,
                                                            TSeqLit& leafs, TSeqLit& output);    

    /**
     * If there exists a constraint for the current parent of the form leafs =< k, then it is possible to also derive the leafs(left) =< k and leafs(right) =< k.
    */
    template <class TSeqLit, class TSeqInt>
    void derive_UB_on_recursion_inputs(constraintid& UB_left_node, constraintid& UB_right_node,
                                        constraintid& UB_current_node, 
                                        TSeqLit& leafsLeft, TSeqInt& weightsLeft, TSeqLit& leafsRight, TSeqInt& weightsRight, int& UB);

    template <class TSeqLit>
    void derive_UB_on_recursion_inputs(constraintid& UB_left_node, constraintid& UB_right_node,
                                        constraintid& UB_current_node, 
                                        TSeqLit& leafsLeft, TSeqLit& leafsRight, int& UB);

    template <class TSeqLit, class TSeqInt>
    void derive_UB_on_recursion_outputs(constraintid& UB_recursion_outputs,
                                        constraintid& UB_leafs_left_node, constraintid& UB_leafs_right_node, constraintid& leafs_leq_outputs_left, constraintid& leafs_leq_outputs_right,
                                        TSeqLit& outputsLeft, TSeqInt& weightsLeft, TSeqLit& outputsRight, TSeqInt& weightsRight, int UB );

    

    /*
    Binary Adder:
    Joost P. Warners, "A linear-time transformation of linear inequalities
    into conjunctive normal form",
    Information Processing Letters 68 (1998) 63-69
    */
    template <class TLit>
    void reifyCarryBA(TLit& a, TLit& b, TLit& c, TLit& carry );

    template <class TLit>
    void reifySumBA(TLit& a, TLit& b, TLit& c, TLit& carry, TLit& sum );
    
    template <class TLit>
    void deriveInputEqOutputBA(constraintid& inputLeqOutput, constraintid& inputGeqOutput, TLit& a, TLit& b, TLit& c, TLit& carry, TLit& sum);

private:
    VeriPbProofLogger *PL;
   
};

#endif