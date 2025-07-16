#include "Prooflogger.hpp"

template class VeriPB::LinTermBoolVars<VeriPB::Lit, unsigned int, unsigned int>;
template class VeriPB::LinTermBoolVars<VeriPB::Lit, int, int>;
template class VeriPB::Constraint<VeriPB::Lit, unsigned int, unsigned int>;
template class VeriPB::Constraint<VeriPB::Lit, int, int>;

template long VeriPB::Prooflogger::unchecked_assumption<VeriPB::Constraint<VeriPB::Lit, unsigned int, unsigned int> >(VeriPB::Constraint<VeriPB::Lit, unsigned int, unsigned int> const&);
template long VeriPB::Prooflogger::unchecked_assumption<VeriPB::Constraint<VeriPB::Lit, int, int> >(VeriPB::Constraint<VeriPB::Lit, int, int> const&);

template unsigned long VeriPB::size<VeriPB::Lit, unsigned int, unsigned int>(VeriPB::LinTermBoolVars<VeriPB::Lit, unsigned int, unsigned int> const&);
template unsigned int VeriPB::coefficient<VeriPB::Lit, unsigned int, unsigned int>(VeriPB::LinTermBoolVars<VeriPB::Lit, unsigned int, unsigned int> const&, unsigned long const&);
template VeriPB::Lit VeriPB::literal<VeriPB::Lit, unsigned int, unsigned int>(VeriPB::LinTermBoolVars<VeriPB::Lit, unsigned int, unsigned int> const&, unsigned long const&);
template long VeriPB::Prooflogger::reification_literal_left_implication<VeriPB::Lit, VeriPB::Constraint<VeriPB::Lit, unsigned int, unsigned int> >(VeriPB::Lit const&, VeriPB::Constraint<VeriPB::Lit, unsigned int, unsigned int> const&, bool);
template long VeriPB::Prooflogger::reification_literal_right_implication<VeriPB::Lit, VeriPB::Constraint<VeriPB::Lit, unsigned int, unsigned int> >(VeriPB::Lit const&, VeriPB::Constraint<VeriPB::Lit, unsigned int, unsigned int> const&, bool);
template long VeriPB::Prooflogger::start_proof_by_contradiction<VeriPB::Constraint<VeriPB::Lit, unsigned int, unsigned int> >(VeriPB::Constraint<VeriPB::Lit, unsigned int, unsigned int> const&);
template void VeriPB::CuttingPlanesDerivation::add_constraint<unsigned int>(long const&, unsigned int const&);
template void VeriPB::CuttingPlanesDerivation::multiply<unsigned long>(unsigned long const&);
template void VeriPB::CuttingPlanesDerivation::weaken<VeriPB::Var>(VeriPB::Var const&);
template VeriPB::Prooflogger::BinaryEncodingForLinearTerm<unsigned int, unsigned int> VeriPB::Prooflogger::create_binary_encoding<VeriPB::LinTermBoolVars<VeriPB::Lit, unsigned int, unsigned int>, unsigned int, unsigned int>(VeriPB::LinTermBoolVars<VeriPB::Lit, unsigned int, unsigned int> const&, long const&);
template VeriPB::Prooflogger::BinaryEncodingForLinearTerm<unsigned int, unsigned int> VeriPB::Prooflogger::create_binary_addition<VeriPB::LinTermBoolVars<VeriPB::Lit, unsigned int, unsigned int>, unsigned int, unsigned int>(VeriPB::LinTermBoolVars<VeriPB::Lit, unsigned int, unsigned int> const&, VeriPB::LinTermBoolVars<VeriPB::Lit, unsigned int, unsigned int> const&);

template std::string VeriPB::number_to_string<int>(int const&);