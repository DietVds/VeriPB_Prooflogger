#include "PBtoCNFprooflogger.hpp"

template constraintid PBtoCNFprooflogger::derive_totalizer_clause<std::vector<VeriPB::Lit>, std::vector<wght>>(std::vector<VeriPB::Lit>& leavesLeft, std::vector<wght>& weightsLeavesLeft, std::vector<VeriPB::Lit>& countingLitsLeft, std::vector<VeriPB::Lit>& leavesRight, std::vector<wght>& weightsLeavesRight,  std::vector<VeriPB::Lit>& countingLitsRight, std::vector<VeriPB::Lit>& countingLits, wght alpha, wght beta, wght sigma, std::vector<VeriPB::Lit>& clause, bool trivialcountingvar, bool clause_contains_zerolits);
template constraintid PBtoCNFprooflogger::derive_totalizer_clause<std::vector<uint32_t>, std::vector<wght>>(std::vector<uint32_t>& leavesLeft, std::vector<wght>& weightsLeavesLeft, std::vector<uint32_t>& countingLitsLeft, std::vector<uint32_t>& leavesRight, std::vector<wght>& weightsLeavesRight,  std::vector<uint32_t>& countingLitsRight, std::vector<uint32_t>& countingLits, wght alpha, wght beta, wght sigma, std::vector<uint32_t>& clause, bool trivialcountingvar, bool clause_contains_zerolits);
template constraintid PBtoCNFprooflogger::derive_totalizer_inverse_clause<std::vector<VeriPB::Lit>, std::vector<wght>>(std::vector<VeriPB::Lit>& leavesLeft, std::vector<wght>& weightsLeavesLeft, std::vector<VeriPB::Lit>& countingLitsLeft, std::vector<VeriPB::Lit>& leavesRight, std::vector<wght>& weightsLeavesRight,  std::vector<VeriPB::Lit>& countingLitsRight, std::vector<VeriPB::Lit>& countingLits, wght alpha, wght beta, wght sigma, std::vector<VeriPB::Lit>& clause, bool trivialcountingvar, bool clause_contains_zerolits);
template constraintid PBtoCNFprooflogger::derive_totalizer_inverse_clause<std::vector<uint32_t>, std::vector<wght>>(std::vector<uint32_t>& leavesLeft, std::vector<wght>& weightsLeavesLeft, std::vector<uint32_t>& countingLitsLeft, std::vector<uint32_t>& leavesRight, std::vector<wght>& weightsLeavesRight,  std::vector<uint32_t>& countingLitsRight, std::vector<uint32_t>& countingLits, wght alpha, wght beta, wght sigma, std::vector<uint32_t>& clause, bool trivialcountingvar, bool clause_contains_zerolits);

template void PBtoCNFprooflogger::reifyCarryBA<VeriPB::Lit>(VeriPB::Lit& a, VeriPB::Lit& b, VeriPB::Lit& c, VeriPB::Lit& carry );
template void PBtoCNFprooflogger::reifySumBA<VeriPB::Lit>(VeriPB::Lit& a, VeriPB::Lit& b, VeriPB::Lit& c, VeriPB::Lit& carry, VeriPB::Lit& sum );
template void PBtoCNFprooflogger::define_zerolit<uint32_t>(uint32_t &zerolit);

template constraintid PBtoCNFprooflogger::deriveInputGeqOutputBA<VeriPB::Lit>(VeriPB::Lit& a, VeriPB::Lit& b, VeriPB::Lit& c, VeriPB::Lit& carry, VeriPB::Lit& sum);
template constraintid PBtoCNFprooflogger::deriveInputGeqOutputBA<uint32_t>(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& carry, uint32_t& sum);

template constraintid PBtoCNFprooflogger::deriveBASeqInputGeqOutput<std::vector<uint32_t>>(std::vector<constraintid>& singleAdders_inputGeqOutput, std::vector<uint32_t>& litsleft, std::vector<uint32_t>& litsright, std::vector<uint32_t>& outputs );
template constraintid PBtoCNFprooflogger::derive_leaves_geq_outputvars_after_binary_recursion<std::vector<uint32_t>, std::vector<long long>>(constraintid& leaves_geq_outputs_leftrecursion, constraintid& leaves_geq_outputs_rightrecursion, constraintid& outputs_recursion_geq_outputs, std::vector<uint32_t>& leaves, std::vector<long long>& weightsleaves, std::vector<uint32_t>& output, std::vector<long long>& weightsOutput);
template void PBtoCNFprooflogger::derive_UB_on_recursion_inputs<std::vector<uint32_t>, std::vector<long long>>(constraintid& UB_left_node, constraintid& UB_right_node,
                                    constraintid& UB_current_node, 
                                     std::vector<uint32_t>& leavesLeft, std::vector<long long>& wghtLeavesL, std::vector<uint32_t>& leavesRight, std::vector<long long>& wghtLeavesR, wght UB);
template constraintid PBtoCNFprooflogger::derive_UB_on_outputliterals<std::vector<uint32_t>, std::vector<long long>>(constraintid& UB_leaves, constraintid& leaves_geq_outputs,
                                    std::vector<uint32_t>& outputs, std::vector<long long>& weights, wght UB );
// Printing help

template std::string PBtoCNFprooflogger::sequence_to_string<std::vector<VeriPB::Lit>>(std::vector<VeriPB::Lit>& lits); 
template std::string PBtoCNFprooflogger::sequence_to_string<std::vector<VeriPB::Lit>, std::vector<wght>>(std::vector<VeriPB::Lit>& lits, std::vector<wght>& weights); 
template std::string PBtoCNFprooflogger::sequence_to_string<std::vector<uint32_t>>(std::vector<uint32_t>& lits); 
template std::string PBtoCNFprooflogger::sequence_to_string<std::vector<uint32_t>, std::vector<long long>>(std::vector<uint32_t>& lits, std::vector<long long>& weights); 