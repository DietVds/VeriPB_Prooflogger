#include "PBtoCNFprooflogger.hpp"

template void PBtoCNFprooflogger::define_zerolit<Glucose::Lit>(Glucose::Lit& zero);
template void PBtoCNFprooflogger::define_zerolit<VeriPB::Lit>(VeriPB::Lit& zero);

// Functions for deriving sortedness
template constraintid PBtoCNFprooflogger::derive_sortedness_from_reified_constraints<Glucose::Lit>(Glucose::Lit& lj, Glucose::Lit& ljp1, wght a);
template constraintid PBtoCNFprooflogger::derive_sortedness_from_reified_constraints<VeriPB::Lit>(VeriPB::Lit& lj, VeriPB::Lit& ljp1, wght a);


// Functions to use when the PB-CNF Translation creates a binary tree, derives variables with specific properties for its children and then merges the children into the parent node.

template constraintid PBtoCNFprooflogger::derive_leaves_leq_outputvars_after_binary_recursion<Glucose::vec<Glucose::Lit>, Glucose::vec<wght>>(constraintid& leaves_leq_outputs_leftrecursion, constraintid& leaves_leq_outputs_rightrecursion, 
                                                        constraintid& outputs_recursion_leq_outputs,
                                                        Glucose::vec<Glucose::Lit>& leaves, Glucose::vec<wght>& weightsleaves, Glucose::vec<Glucose::Lit>& output, Glucose::vec<wght>& weightsOutput);

template constraintid PBtoCNFprooflogger::derive_leaves_geq_outputvars_after_binary_recursion<Glucose::vec<Glucose::Lit>, Glucose::vec<wght>>(constraintid& leaves_geq_outputs_leftrecursion, constraintid& leaves_geq_outputs_rightrecursion, 
                                                        constraintid& outputs_recursion_geq_outputs,
                                                        Glucose::vec<Glucose::Lit>& leaves,  Glucose::vec<wght>& weightsleaves, Glucose::vec<Glucose::Lit>& output, Glucose::vec<wght>& weightsOutput);

template constraintid PBtoCNFprooflogger::derive_leaves_leq_outputvars_after_binary_recursion<std::vector<VeriPB::Lit>, std::vector<wght>>(constraintid& leaves_leq_outputs_leftrecursion, constraintid& leaves_leq_outputs_rightrecursion, 
                                                        constraintid& outputs_recursion_leq_outputs,
                                                        std::vector<VeriPB::Lit>& leaves, std::vector<wght>& weightsleaves, std::vector<VeriPB::Lit>& output, std::vector<wght>& weightsOutput);

template constraintid PBtoCNFprooflogger::derive_leaves_geq_outputvars_after_binary_recursion<std::vector<VeriPB::Lit>, std::vector<wght>>(constraintid& leaves_geq_outputs_leftrecursion, constraintid& leaves_geq_outputs_rightrecursion, 
                                                        constraintid& outputs_recursion_geq_outputs,
                                                        std::vector<VeriPB::Lit>& leaves,  std::vector<wght>& weightsleaves, std::vector<VeriPB::Lit>& output, std::vector<wght>& weightsOutput);

template void PBtoCNFprooflogger::derive_UB_on_recursion_inputs<Glucose::vec<Glucose::Lit>, Glucose::vec<wght>>(constraintid& UB_left_node, constraintid& UB_right_node,
                                    constraintid& UB_current_node, 
                                     Glucose::vec<Glucose::Lit>& leavesLeft, Glucose::vec<wght>& weightsLeft, Glucose::vec<Glucose::Lit>& leavesRight, Glucose::vec<wght>& weightsRight);
template void PBtoCNFprooflogger::derive_UB_on_recursion_inputs<std::vector<VeriPB::Lit>, std::vector<wght>>(constraintid& UB_left_node, constraintid& UB_right_node,
                                    constraintid& UB_current_node, 
                                     std::vector<VeriPB::Lit>& leavesLeft, std::vector<wght>& weightsLeft, std::vector<VeriPB::Lit>& leavesRight, std::vector<wght>& weightsRight);


template void PBtoCNFprooflogger::derive_UB_on_recursion_inputs<Glucose::vec<Glucose::Lit>>(constraintid& UB_left_node, constraintid& UB_right_node,
                                    constraintid& UB_current_node, 
                                    Glucose::vec<Glucose::Lit>& leavesLeft, Glucose::vec<Glucose::Lit>& leavesRight);
template void PBtoCNFprooflogger::derive_UB_on_recursion_inputs<std::vector<VeriPB::Lit>>(constraintid& UB_left_node, constraintid& UB_right_node,
                                    constraintid& UB_current_node, 
                                    std::vector<VeriPB::Lit>& leavesLeft, std::vector<VeriPB::Lit>& leavesRight);

template constraintid PBtoCNFprooflogger::derive_UB_on_outputliterals<Glucose::vec<Glucose::Lit>, Glucose::vec<wght>>(constraintid& UB_leaves, constraintid& leaves_geq_outputs, Glucose::vec<Glucose::Lit>& outputs, Glucose::vec<wght>& weights );
template constraintid PBtoCNFprooflogger::derive_UB_on_outputliterals<std::vector<VeriPB::Lit>, std::vector<wght>>(constraintid& UB_leaves, constraintid& leaves_geq_outputs, std::vector<VeriPB::Lit>& outputs, std::vector<wght>& weights );

template constraintid PBtoCNFprooflogger::chain_IO_constraints_binary_recursion_leq<Glucose::vec<Glucose::Lit>, Glucose::vec<wght>>(constraintid& X1_leq_Y1, constraintid& X2_leq_Y2, constraintid& Y1_U_Y2_leq_O, Glucose::vec<Glucose::Lit>& litsX1, Glucose::vec<wght>& wghtsX1, Glucose::vec<Glucose::Lit>& litsX2, Glucose::vec<wght>& wghtsX2, Glucose::vec<Glucose::Lit>& litsO, Glucose::vec<wght>& wghtsO );
template constraintid PBtoCNFprooflogger::chain_IO_constraints_binary_recursion_leq<std::vector<VeriPB::Lit>, std::vector<wght>>(constraintid& X1_leq_Y1, constraintid& X2_leq_Y2, constraintid& Y1_U_Y2_leq_O, std::vector<VeriPB::Lit>& litsX1, std::vector<wght>& wghtsX1, std::vector<VeriPB::Lit>& litsX2, std::vector<wght>& wghtsX2, std::vector<VeriPB::Lit>& litsO, std::vector<wght>& wghtsO );
template constraintid PBtoCNFprooflogger::chain_IO_constraints_binary_recursion_geq<Glucose::vec<Glucose::Lit>, Glucose::vec<wght>>(constraintid& X1_geq_Y1, constraintid& X2_geq_Y2, constraintid& Y1_U_Y2_geq_O, Glucose::vec<Glucose::Lit>& litsX1, Glucose::vec<wght>& wghtsX1, Glucose::vec<Glucose::Lit>& litsX2, Glucose::vec<wght>& wghtsX2, Glucose::vec<Glucose::Lit>& litsO, Glucose::vec<wght>& wghtsO );
template constraintid PBtoCNFprooflogger::chain_IO_constraints_binary_recursion_geq<std::vector<VeriPB::Lit>, std::vector<wght>>(constraintid& X1_geq_Y1, constraintid& X2_geq_Y2, constraintid& Y1_U_Y2_geq_O, std::vector<VeriPB::Lit>& litsX1, std::vector<wght>& wghtsX1, std::vector<VeriPB::Lit>& litsX2, std::vector<wght>& wghtsX2, std::vector<VeriPB::Lit>& litsO, std::vector<wght>& wghtsO );


// Functions to use when the encoding has as output variables a unary representation of the sum of the leaves. 

template constraintid PBtoCNFprooflogger::derive_leaves_leq_unary_k_from_reification<Glucose::vec<Glucose::Lit>>(Glucose::vec<Glucose::Lit>& countingLits, wght k, Glucose::vec<Glucose::Lit>& clause, bool trivialcountingvars=false );

template constraintid PBtoCNFprooflogger::derive_input_geq_unary_output_from_output_definitions<Glucose::vec<Glucose::Lit>, Glucose::vec<wght>>(Glucose::vec<Glucose::Lit>& inputlits, Glucose::vec<wght>& inputweights, Glucose::vec<Glucose::Lit>& outputlits, bool trivialcountingvar=false);
template constraintid PBtoCNFprooflogger::derive_input_leq_unary_output_from_output_definitions<Glucose::vec<Glucose::Lit>, Glucose::vec<wght>>(Glucose::vec<Glucose::Lit>& inputlits, Glucose::vec<wght>& inputweights, Glucose::vec<Glucose::Lit>& outputlits, bool trivialcountingvar=false);

// Binary Adders

template void PBtoCNFprooflogger::reifyCarryBA<Glucose::Lit>(Glucose::Lit& a, Glucose::Lit& b, Glucose::Lit& c, Glucose::Lit& carry );
template void PBtoCNFprooflogger::reifyCarryBA<VeriPB::Lit>(VeriPB::Lit& a, VeriPB::Lit& b, VeriPB::Lit& c, VeriPB::Lit& carry );

template void PBtoCNFprooflogger::reifySumBA<Glucose::Lit>(Glucose::Lit& a, Glucose::Lit& b, Glucose::Lit& c, Glucose::Lit& carry, Glucose::Lit& sum );
template void PBtoCNFprooflogger::reifySumBA<VeriPB::Lit>(VeriPB::Lit& a, VeriPB::Lit& b, VeriPB::Lit& c, VeriPB::Lit& carry, VeriPB::Lit& sum );

template constraintid PBtoCNFprooflogger::deriveInputLeqOutputBA<Glucose::Lit>(Glucose::Lit& a, Glucose::Lit& b, Glucose::Lit& c, Glucose::Lit& carry, Glucose::Lit& sum);
template constraintid PBtoCNFprooflogger::deriveInputGeqOutputBA<Glucose::Lit>(Glucose::Lit& a, Glucose::Lit& b, Glucose::Lit& c, Glucose::Lit& carry, Glucose::Lit& sum);
template constraintid PBtoCNFprooflogger::deriveInputLeqOutputBA<VeriPB::Lit>(VeriPB::Lit& a, VeriPB::Lit& b, VeriPB::Lit& c, VeriPB::Lit& carry, VeriPB::Lit& sum);
template constraintid PBtoCNFprooflogger::deriveInputGeqOutputBA<VeriPB::Lit>(VeriPB::Lit& a, VeriPB::Lit& b, VeriPB::Lit& c, VeriPB::Lit& carry, VeriPB::Lit& sum);

// For a binary adder with input two sequences <l> and <r> of literals and as output the sequence of literals <o>, 
// derive the inequalities representing \sum_i 2^i * <l>_i + \sum_i 2^i * <r>_i = \sum_i 2^i * <o>_i
template constraintid PBtoCNFprooflogger::deriveBASeqInputLeqOutput<Glucose::vec<Glucose::Lit>>(std::vector<constraintid>& singleAdders_inputLeqOutput, Glucose::vec<Glucose::Lit>& litsleft, Glucose::vec<Glucose::Lit>& litsright, Glucose::vec<Glucose::Lit>& outputs);
template constraintid PBtoCNFprooflogger::deriveBASeqInputGeqOutput<Glucose::vec<Glucose::Lit>>(std::vector<constraintid>& singleAdders_inputGeqOutput, Glucose::vec<Glucose::Lit>& litsleft, Glucose::vec<Glucose::Lit>& litsright, Glucose::vec<Glucose::Lit>& outputs);
template constraintid PBtoCNFprooflogger::deriveBASeqInputLeqOutput<std::vector<VeriPB::Lit>>(std::vector<constraintid>& singleAdders_inputLeqOutput, std::vector<VeriPB::Lit>& litsleft, std::vector<VeriPB::Lit>& litsright, std::vector<VeriPB::Lit>& outputs);
template constraintid PBtoCNFprooflogger::deriveBASeqInputGeqOutput<std::vector<VeriPB::Lit>>(std::vector<constraintid>& singleAdders_inputGeqOutput, std::vector<VeriPB::Lit>& litsleft, std::vector<VeriPB::Lit>& litsright, std::vector<VeriPB::Lit>& outputs );

// Totalizer encoding
template constraintid PBtoCNFprooflogger::derive_totalizer_clause<Glucose::vec<Glucose::Lit>, Glucose::vec<wght>>(Glucose::vec<Glucose::Lit>& leavesLeft, Glucose::vec<wght>& weightsLeavesLeft, Glucose::vec<Glucose::Lit>& countingLitsLeft, Glucose::vec<Glucose::Lit>& leavesRight, Glucose::vec<wght>& weightsLeavesRight,  Glucose::vec<Glucose::Lit>& countingLitsRight, Glucose::vec<Glucose::Lit>& countingLits, wght alpha, wght beta, wght sigma, Glucose::vec<Glucose::Lit>& clause, bool trivialcountingvar, bool clause_contains_zerolits);
template constraintid PBtoCNFprooflogger::derive_totalizer_inverse_clause<Glucose::vec<Glucose::Lit>, Glucose::vec<wght>>(Glucose::vec<Glucose::Lit>& leavesLeft, Glucose::vec<wght>& weightsLeavesLeft, Glucose::vec<Glucose::Lit>& countingLitsLeft, Glucose::vec<Glucose::Lit>& leavesRight, Glucose::vec<wght>& weightsLeavesRight,  Glucose::vec<Glucose::Lit>& countingLitsRight, Glucose::vec<Glucose::Lit>& countingLits, wght alpha, wght beta, wght sigma, Glucose::vec<Glucose::Lit>& clause, bool trivialcountingvar, bool clause_contains_zerolits);


// Modulo Totalizer
template VeriPB::Lit PBtoCNFprooflogger::getRemainderLiteral<Glucose::vec<Glucose::Lit>>(Glucose::vec<Glucose::Lit>& countingLiteralsMTO, wght j);
template VeriPB::Lit PBtoCNFprooflogger::getQuotientLiteral<Glucose::vec<Glucose::Lit>>(Glucose::vec<Glucose::Lit>& countingLiteralsMTO, wght j, wght div);
template wght PBtoCNFprooflogger::getNrOfQuotientLiterals<Glucose::vec<Glucose::Lit>>(Glucose::vec<Glucose::Lit>& countingLiteralsMTO, wght div);

template void PBtoCNFprooflogger::reifyQuotientLiteralMTO<Glucose::Lit, Glucose::vec<Glucose::Lit>, Glucose::vec<wght> >(Glucose::Lit& quotientliteral, Glucose::vec<Glucose::Lit>& leaves, Glucose::vec<wght>& leaves_wght, wght j, wght div);
template void PBtoCNFprooflogger::reifyQuotientLiteralMTO<VeriPB::Lit, Glucose::vec<Glucose::Lit>, Glucose::vec<wght> >(VeriPB::Lit& quotientliteral, Glucose::vec<Glucose::Lit>& leaves, Glucose::vec<wght>& leaves_wght, wght j, wght div);
template void PBtoCNFprooflogger::reifyRemainderLiteralMTO<Glucose::Lit, Glucose::vec<Glucose::Lit>, Glucose::vec<wght>>(Glucose::Lit& remainderliteral, Glucose::vec<Glucose::Lit>& leaves, Glucose::vec<wght>& leaves_wght, Glucose::vec<Glucose::Lit>& countingLiteralsMTO, wght j, wght div);
template void PBtoCNFprooflogger::reifyRemainderLiteralMTO<VeriPB::Lit, Glucose::vec<Glucose::Lit>, Glucose::vec<wght>>(VeriPB::Lit& remainderliteral, Glucose::vec<Glucose::Lit>& leaves, Glucose::vec<wght>& leaves_wght, Glucose::vec<Glucose::Lit>& countingLiteralsMTO, wght j, wght div);
template void PBtoCNFprooflogger::reifyCarryLiteralMTO<Glucose::Lit, Glucose::vec<Glucose::Lit>>(Glucose::Lit& carryLit, Glucose::vec<Glucose::Lit>& countingLiteralsMTOLeft, Glucose::vec<Glucose::Lit>& countingLiteralsMTORight, wght div);
template constraintid PBtoCNFprooflogger::derive_leaves_geq_countinglits_MTO<Glucose::vec<Glucose::Lit>, Glucose::vec<wght>>(Glucose::vec<Glucose::Lit>& countingLiteralsMTO, Glucose::vec<Glucose::Lit>& leaves, Glucose::vec<wght>& wght_leaves, wght div);
template constraintid PBtoCNFprooflogger::derive_leaves_leq_countinglits_MTO<Glucose::vec<Glucose::Lit>, Glucose::vec<wght>>(Glucose::vec<Glucose::Lit>& countingLiteralsMTO, Glucose::vec<Glucose::Lit>& leaves, Glucose::vec<wght>& wght_leaves, wght div);


// Printing help

template std::string PBtoCNFprooflogger::sequence_to_string<Glucose::vec<Glucose::Lit>>(Glucose::vec<Glucose::Lit>& lits); 
template std::string PBtoCNFprooflogger::sequence_to_string<Glucose::vec<Glucose::Lit>, Glucose::vec<long long>>(Glucose::vec<Glucose::Lit>& lits, Glucose::vec<long long>& weights); 
template std::string PBtoCNFprooflogger::sequence_to_string<std::vector<VeriPB::Lit>>(std::vector<VeriPB::Lit>& lits); 
template std::string PBtoCNFprooflogger::sequence_to_string<std::vector<VeriPB::Lit>, std::vector<wght>>(std::vector<VeriPB::Lit>& lits, std::vector<wght>& weights); 