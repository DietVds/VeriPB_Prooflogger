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
     * Let X1, X2, Y1, Y2 and O be linear terms over pairwise disjoint sets of literals and let >=< be either >= or =<. 
     * If the following constraints have been derived: X1 =< Y1, X2 =< Y2, Y1 U Y2 =< O, then it is possible to derive X1 U X2 =< O. 
    */
    template <class TSeqLit, class TSeqWght>
    constraintid chain_IO_constraints_binary_recursion_leq(constraintid& X1_leq_Y1, constraintid& X2_leq_Y2, constraintid& Y1_U_Y2_leq_O, TSeqLit& litsX1, TSeqWght& wghtsX1, TSeqLit& litsX2, TSeqWght& wghtsX2, TSeqLit& litsO, TSeqWght& wghtsO );

    /**
     * Let X1, X2, Y1, Y2 and O be linear terms over pairwise disjoint sets of literals and let >=< be either >= or =<. 
     * If the following constraints have been derived: X1 >= Y1, X2 >= Y2, Y1 U Y2 >= O, then it is possible to derive X1 U X2 >= O. 
    */
    template <class TSeqLit, class TSeqWght>
    constraintid chain_IO_constraints_binary_recursion_geq(constraintid& X1_geq_Y1, constraintid& X2_geq_Y2, constraintid& Y1_U_Y2_geq_O, TSeqLit& litsX1, TSeqWght& wghtsX1, TSeqLit& litsX2, TSeqWght& wghtsX2, TSeqLit& litsO, TSeqWght& wghtsO );


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
    constraintid derive_leaves_leq_unary_k_from_reification(TSeqLit& countingLits, wght k, TSeqLit& clause, bool trivialcountingvars=false );

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
    // Note: after reification, the constraintid's can be found by the reification constraints database of the carry and the sum. 
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
    Modulo Totalizer:
    Toru Ogawa, YangYang Liu, Ryuzo Hasegawa, Miyuki Koshimura, Hiroshi Fujita,
    "Modulo Based CNF Encoding of Cardinality Constraints and Its Application to
    MaxSAT Solvers",
    ICTAI 2013.
    */

    /*
    Important: the code here is tailored towards the following representation used in QMaxSAT: [~zerolit, r_1, ..., r_{d-1}, ~zerolit, q_1, ..., q_{floor(n/d)}].
    The rest of the code can be adapted quickly to other representations by modifying the following three functions. 
    */
    template <class TSeqLit>
    VeriPB::Lit getRemainderLiteral(TSeqLit& countingLiteralsMTO, wght j);
    template <class TSeqLit>
    VeriPB::Lit getQuotientLiteral(TSeqLit& countingLiteralsMTO, wght j, wght div);
    template <class TSeqLit>
    wght getNrOfQuotientLiterals(TSeqLit& countingLiteralsMTO, wght div);

    /**
     * Given a number d, set of literals L with associated weights W, it is possible to derive the following 
     * reified constraints for a literal q_j:
     * q_j <-> \sum_{l in L, w in W} w*l >= j*d
    */
    template <class TLit, class TSeqLit, class TSeqWght >
    void reifyQuotientLiteralMTO(TLit& quotientliteral, TSeqLit& leaves, TSeqWght& leaves_wght, wght j, wght div);
    /**
     * Given a number d, a set of literals L with associated weights W, and a set of literals Q =  {q_j | 1 =< j =< floor(|W|/div)} it is possible to derive the following 
     * reified constraints for a literal r_j:
     * r_j <-> \sum_{l in L, w in W} w*l - \sum{q_j in Q} q_j >= j
    */
    template <class TLit, class TSeqLit, class TSeqWght>
    void reifyRemainderLiteralMTO(TLit& remainderliteral, TSeqLit& leaves, TSeqWght& leaves_wght, TSeqLit& countingLiteralsMTO, wght j, wght div);
    /**
     * Given a number d, two sets of literals R' and R'', it is possible to derive the reified constraints for a literal c:
     * c <-> \sum_{r in R'} r + \sum_{r in R''} r >= d
     * 
    */
    template <class TLit, class TSeqLit>
    void reifyCarryLiteralMTO(TLit& carryLit, TSeqLit& countingLiteralsMTOLeft, TSeqLit& countingLiteralsMTORight, wght div);

    template <class TLit, class TSeqLit, class TSeqWght>
    constraintid derive_leaves_leq_countinglits_MTO(TSeqLit& countingLiteralsMTO, TSeqLit& leaves, TSeqWght& wght_leaves, wght div);

    template <class TLit, class TSeqLit, class TSeqWght>
    constraintid derive_leaves_geq_countinglits_MTO(TSeqLit& countingLiteralsMTO, TSeqLit& leaves, TSeqWght& wght_leaves, wght div);

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

    /*
        Functions for handling the unary representation of a value.
    */

    /**
     * Given a set of counting literals L = {l_j | 1 =< j =< |X|} for which every literal l_j is reified as l_j -> \sum_{i = 1}^{|X|} a_i x_i >= j,
     * it is possible to derive the constraint \sum_{i = 1}^{|X|} l_i =<  \sum_{i = 1}^{|X|} a_i x_i .
    */
    template <class TSeqLit, class TSeqWght>
    constraintid derive_input_geq_unary_output_from_output_definitions(TSeqLit& inputlits, TSeqWght& inputweights, TSeqLit& outputlits, bool trivialcountingvar=false);

    /**
     * Given a set of counting literals L = {l_j | 1 =< j =< |X|} for which every literal l_j is reified as l_j <- \sum_{i = 1}^{|X|} a_i x_i >= j,
     * it is possible to derive the constraint \sum_{i = 1}^{|X|} l_i >=  \sum_{i = 1}^{|X|} a_i x_i .
    */
    template <class TSeqLit, class TSeqWght>
    constraintid derive_input_leq_unary_output_from_output_definitions(TSeqLit& inputlits, TSeqWght& inputweights, TSeqLit& outputlits, bool trivialcountingvar=false);

    /**
     * TODO: Given a sequence of literals L = <l1 l2 ...> for which the constraint \sum_{i = 1}^{|X|} l_i >=  \sum_{i = 1}^{|X|} a_i x_i is derived and for which sortedness has been derived, i.e., for all i such that 1 =< i < |L|,
     * it is possible to derive the reified variable definition l_j <- \sum_{i = 1}^{|X|} a_i x_i >= j.
    */

    /**
     * TODO: Given a sequence of literals L = <l1 l2 ...> for which the constraint \sum_{i = 1}^{|X|} l_i =<  \sum_{i = 1}^{|X|} a_i x_i is derived and for which sortedness has been derived, i.e., for all i such that 1 =< i < |L|,
     * it is possible to derive the reified variable definition l_j -> \sum_{i = 1}^{|X|} a_i x_i >= j.
    */

    // Helper functions for commenting
    template<class TSeqLit>
    std::string sequence_to_string(TSeqLit& lits); 

    template<class TSeqLit, class TSeqWght>
    std::string sequence_to_string(TSeqLit& lits, TSeqWght& weights); 
private:
    VeriPbProofLogger *PL;
   
};

#endif