#ifndef PbTransProoflogger_h
#define PbTransProoflogger_h

#include "VeriPBProoflogger.h"

#include <cmath>

class PBtoCNFprooflogger
{
public:
    // Constructor
    PBtoCNFprooflogger(VeriPbProofLogger *PL) : PL(PL) {};

    // an extra literal that is always true is added as a unit clause. We will add this to the proof logger, since we will need it in multiple encodings.
    // The philosophy of dealing with the literals zerolit or ~zerolit is that they are removed from the constraints as early as possible (i.e., in the deepest level of the recursion)
    VeriPB::Lit zerolit;
    constraintid def_one;

    template <class TLit>
    void define_zerolit(TLit& zerolit);

    // Functions to use when the PB-CNF Translation creates a binary tree, derives variables with specific properties for its children and then merges the children into the parent node.


    /**
     * Let X be the leaves of the current node with X = X1 U X2 with X1 and X2 the leaves of a left, respectively right recursive tree to encode a PB translation. 
     * Let O, O1 and O2 be the outputs of respectively the current node, the left node and the right node. If we have derived X1 = O and X2 = O, we can also derive X = O.
     * */
    template <class TSeqLit, class TSeqWght>
    constraintid derive_leaves_leq_outputvars_after_binary_recursion(constraintid& leaves_leq_outputs_leftrecursion, constraintid& leaves_leq_outputs_rightrecursion, 
                                                            constraintid& outputs_recursion_leq_outputs,
                                                            TSeqLit& leaves, TSeqWght& weightsleaves, TSeqLit& output, TSeqWght& weightsOutput);

    template <class TSeqLit, class TSeqWght>
    constraintid derive_leaves_geq_outputvars_after_binary_recursion(constraintid& leaves_geq_outputs_leftrecursion, constraintid& leaves_geq_outputs_rightrecursion, 
                                                            constraintid& outputs_recursion_geq_outputs,
                                                            TSeqLit& leaves, TSeqWght& weightsleaves, TSeqLit& output, TSeqWght& weightsOutput);

    /**
     * If there exists a constraint for the current parent of the form leaves =< k, then it is possible to also derive the leaves(left) =< k and leaves(right) =< k.
    */

    // Given a binary tree encoding and an UB on the leaves of the current node, the UB on the leaves of both children can be derived. 
    template <class TSeqLit, class TSeqWght>
    void derive_UB_on_recursion_inputs(constraintid& UB_left_node, constraintid& UB_right_node,
                                        constraintid& UB_current_node, 
                                        TSeqLit& leavesLeft, TSeqWght& weightsLeft, TSeqLit& leavesRight, TSeqWght& weightsRight);

    template <class TSeqLit>
    void derive_UB_on_recursion_inputs(constraintid& UB_left_node, constraintid& UB_right_node,
                                        constraintid& UB_current_node, 
                                        TSeqLit& leavesLeft, TSeqLit& leavesRight);

    // Given a binary tree encoding and an UB on the leaves of the current node, a
    template <class TSeqLit, class TSeqWght>
    constraintid derive_UB_on_outputliterals(constraintid& UB_leaves, constraintid& leaves_geq_outputs, TSeqLit& outputs, TSeqWght& weights );

    // Functions to use when the encoding has as output variables a unary representation of the sum of the leaves. 

    template <class TSeqLit> 
    constraintid derive_leaves_lessthan_unary_k_from_reification(TSeqLit& countingLits, wght k, TSeqLit& clause, bool trivialcountingvars=false );

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

    // For an adder with three inputs, derive the inequalites representing a + b + c = 2c + s  
    // Note: constraintid's are found by reification constraints of the carry and the sum. 
    template <class TLit>
    constraintid deriveInputLeqOutputBA(TLit& a, TLit& b, TLit& c, TLit& carry, TLit& sum);
    template <class TLit>
    constraintid deriveInputGeqOutputBA(TLit& a, TLit& b, TLit& c, TLit& carry, TLit& sum);

    // For a binary adder with input two sequences <l> and <r> of literals and as output the sequence of literals <o>, 
    // derive the inequalities representing \sum_i 2^i * <l>_i + \sum_i 2^i * <r>_i = \sum_i 2^i * <o>_i
    template <class TSeqLit>
    constraintid deriveBASeqInputLeqOutput(std::vector<constraintid>& singleAdders_inputLeqOutput, TSeqLit& litsleft, TSeqLit& litsright, TSeqLit& outputs);
    template <class TSeqLit>
    constraintid deriveBASeqInputGeqOutput(std::vector<constraintid>& singleAdders_inputGeqOutput, TSeqLit& litsleft, TSeqLit& litsright, TSeqLit& outputs );
    

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
    constraintid derive_totalizer_clause(TSeqLit& leavesLeft, TSeqWght& weightsLeavesLeft, TSeqLit& countingLitsLeft, TSeqLit& leavesRight, TSeqWght& weightsLeavesRight,  TSeqLit& countingLitsRight, TSeqLit& countingLits, wght alpha, wght beta, wght sigma, TSeqLit& clause, bool trivialcountingvar, bool clause_contains_zerolits);

    /**
     * Derives the inverse totalizer clauses. These clauses are the clauses that are called C2 in Bailleux's paper.
     * If the boolean trivialcountingvar is true, countingLitsLeft, countingLitsRight and countingLits are all of the form <~zerolit, ..., zerolit>. 
     *      These extra literals in the countingLits are not necessary, and thus some solvers do not add them.
     * If the boolean clause_contains_zerolits is true, the clause that is derived contains a zero-literal in case alpha or beta is equal to 0, else they are discarded.     
    */
    template <class TSeqLit, class TSeqWght>
    constraintid derive_totalizer_inverse_clause(TSeqLit& leavesLeft, TSeqWght& weightsLeavesLeft, TSeqLit& countingLitsLeft, TSeqLit& leavesRight, TSeqWght& weightsLeavesRight,  TSeqLit& countingLitsRight, TSeqLit& countingLits, wght alpha, wght beta, wght sigma, TSeqLit& clause, bool trivialcountingvar, bool clause_contains_zerolits);


    // Helper functions for commenting
    template<class TSeqLit>
    std::string sequence_to_string(TSeqLit& lits); 

    template<class TSeqLit, class TSeqWght>
    std::string sequence_to_string(TSeqLit& lits, TSeqWght& weights); 
private:
    VeriPbProofLogger *PL;
   
};

#endif