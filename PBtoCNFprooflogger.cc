#include "PBtoCNFprooflogger.hpp"

template void PBtoCNFprooflogger::define_zerolit<Glucose::Lit>(Glucose::Lit& zero);
template void PBtoCNFprooflogger::define_zerolit<VeriPB::Lit>(VeriPB::Lit& zero);

// Functions to use when the PB-CNF Translation creates a binary tree, derives variables with specific properties for its children and then merges the children into the parent node.

template void PBtoCNFprooflogger::derive_UB_on_recursion_inputs<Glucose::vec<Glucose::Lit>, Glucose::vec<int>>(constraintid& UB_left_node, constraintid& UB_right_node,
                                    constraintid& UB_current_node, 
                                     Glucose::vec<Glucose::Lit>& leafsLeft, Glucose::vec<int>& weightsLeft, Glucose::vec<Glucose::Lit>& leafsRight, Glucose::vec<int>& weightsRight, int& UB);
template void PBtoCNFprooflogger::derive_UB_on_recursion_inputs<std::vector<VeriPB::Lit>, std::vector<int>>(constraintid& UB_left_node, constraintid& UB_right_node,
                                    constraintid& UB_current_node, 
                                     std::vector<VeriPB::Lit>& leafsLeft, std::vector<int>& weightsLeft, std::vector<VeriPB::Lit>& leafsRight, std::vector<int>& weightsRight, int& UB);


template void PBtoCNFprooflogger::derive_UB_on_recursion_inputs<Glucose::vec<Glucose::Lit>>(constraintid& UB_left_node, constraintid& UB_right_node,
                                    constraintid& UB_current_node, 
                                    Glucose::vec<Glucose::Lit>& leafsLeft, Glucose::vec<Glucose::Lit>& leafsRight, int& UB);
template void PBtoCNFprooflogger::derive_UB_on_recursion_inputs<std::vector<VeriPB::Lit>>(constraintid& UB_left_node, constraintid& UB_right_node,
                                    constraintid& UB_current_node, 
                                    std::vector<VeriPB::Lit>& leafsLeft, std::vector<VeriPB::Lit>& leafsRight, int& UB);

// Binary Adders

template void PBtoCNFprooflogger::reifyCarryBA<Glucose::Lit>(Glucose::Lit& a, Glucose::Lit& b, Glucose::Lit& c, Glucose::Lit& carry );
template void PBtoCNFprooflogger::reifyCarryBA<VeriPB::Lit>(VeriPB::Lit& a, VeriPB::Lit& b, VeriPB::Lit& c, VeriPB::Lit& carry );

template void PBtoCNFprooflogger::reifySumBA<Glucose::Lit>(Glucose::Lit& a, Glucose::Lit& b, Glucose::Lit& c, Glucose::Lit& carry, Glucose::Lit& sum );
template void PBtoCNFprooflogger::reifySumBA<VeriPB::Lit>(VeriPB::Lit& a, VeriPB::Lit& b, VeriPB::Lit& c, VeriPB::Lit& carry, VeriPB::Lit& sum );

template void PBtoCNFprooflogger::deriveInputEqOutputBA<Glucose::Lit>(constraintid& inputLeqOutput, constraintid& inputGeqOutput, Glucose::Lit& a, Glucose::Lit& b, Glucose::Lit& c, Glucose::Lit& carry, Glucose::Lit& sum);
template void PBtoCNFprooflogger::deriveInputEqOutputBA<VeriPB::Lit>(constraintid& inputLeqOutput, constraintid& inputGeqOutput, VeriPB::Lit& a, VeriPB::Lit& b, VeriPB::Lit& c, VeriPB::Lit& carry, VeriPB::Lit& sum);
