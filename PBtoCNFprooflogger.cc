#include "PBtoCNFprooflogger.hpp"

template void PBtoCNFprooflogger::define_zerolit<Glucose::Lit>(Glucose::Lit& zero);
template void PBtoCNFprooflogger::define_zerolit<VeriPB::Lit>(VeriPB::Lit& zero);

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


template std::string PBtoCNFprooflogger::sequence_to_string<Glucose::vec<Glucose::Lit>>(Glucose::vec<Glucose::Lit>& lits); 
template std::string PBtoCNFprooflogger::sequence_to_string<Glucose::vec<Glucose::Lit>, Glucose::vec<long long>>(Glucose::vec<Glucose::Lit>& lits, Glucose::vec<long long>& weights); 
template std::string PBtoCNFprooflogger::sequence_to_string<std::vector<VeriPB::Lit>>(std::vector<VeriPB::Lit>& lits); 
template std::string PBtoCNFprooflogger::sequence_to_string<std::vector<VeriPB::Lit>, std::vector<wght>>(std::vector<VeriPB::Lit>& lits, std::vector<wght>& weights); 