#include "PBtoCNFprooflogger.hpp"

template constraintid PBtoCNFprooflogger::derive_totalizer_clause<std::vector<VeriPB::Lit>, std::vector<wght>>(std::vector<VeriPB::Lit>& leavesLeft, std::vector<wght>& weightsLeavesLeft, std::vector<VeriPB::Lit>& countingLitsLeft, std::vector<VeriPB::Lit>& leavesRight, std::vector<wght>& weightsLeavesRight,  std::vector<VeriPB::Lit>& countingLitsRight, std::vector<VeriPB::Lit>& countingLits, wght alpha, wght beta, wght sigma, std::vector<VeriPB::Lit>& clause, bool trivialcountingvar, bool clause_contains_zerolits);
template constraintid PBtoCNFprooflogger::derive_totalizer_clause<std::vector<uint32_t>, std::vector<wght>>(std::vector<uint32_t>& leavesLeft, std::vector<wght>& weightsLeavesLeft, std::vector<uint32_t>& countingLitsLeft, std::vector<uint32_t>& leavesRight, std::vector<wght>& weightsLeavesRight,  std::vector<uint32_t>& countingLitsRight, std::vector<uint32_t>& countingLits, wght alpha, wght beta, wght sigma, std::vector<uint32_t>& clause, bool trivialcountingvar, bool clause_contains_zerolits);
template constraintid PBtoCNFprooflogger::derive_totalizer_inverse_clause<std::vector<VeriPB::Lit>, std::vector<wght>>(std::vector<VeriPB::Lit>& leavesLeft, std::vector<wght>& weightsLeavesLeft, std::vector<VeriPB::Lit>& countingLitsLeft, std::vector<VeriPB::Lit>& leavesRight, std::vector<wght>& weightsLeavesRight,  std::vector<VeriPB::Lit>& countingLitsRight, std::vector<VeriPB::Lit>& countingLits, wght alpha, wght beta, wght sigma, std::vector<VeriPB::Lit>& clause, bool trivialcountingvar, bool clause_contains_zerolits);
template constraintid PBtoCNFprooflogger::derive_totalizer_inverse_clause<std::vector<uint32_t>, std::vector<wght>>(std::vector<uint32_t>& leavesLeft, std::vector<wght>& weightsLeavesLeft, std::vector<uint32_t>& countingLitsLeft, std::vector<uint32_t>& leavesRight, std::vector<wght>& weightsLeavesRight,  std::vector<uint32_t>& countingLitsRight, std::vector<uint32_t>& countingLits, wght alpha, wght beta, wght sigma, std::vector<uint32_t>& clause, bool trivialcountingvar, bool clause_contains_zerolits);


// Printing help

template std::string PBtoCNFprooflogger::sequence_to_string<std::vector<VeriPB::Lit>>(std::vector<VeriPB::Lit>& lits); 
template std::string PBtoCNFprooflogger::sequence_to_string<std::vector<VeriPB::Lit>, std::vector<wght>>(std::vector<VeriPB::Lit>& lits, std::vector<wght>& weights); 