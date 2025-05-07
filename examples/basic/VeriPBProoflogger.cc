#include "ProofloggerOptimization.hpp"

template class VeriPB::LinTermBoolVars<VeriPB::Lit, unsigned int, unsigned int>;
template class VeriPB::LinTermBoolVars<VeriPB::Lit, int, int>;
template class VeriPB::Constraint<VeriPB::Lit, unsigned int, unsigned int>;
template class VeriPB::Constraint<VeriPB::Lit, int, int>;
template class VeriPB::ProofloggerOpt<VeriPB::Lit, unsigned int, unsigned int>;

template long VeriPB::Prooflogger::unchecked_assumption<VeriPB::Constraint<VeriPB::Lit, unsigned int, unsigned int> >(VeriPB::Constraint<VeriPB::Lit, unsigned int, unsigned int> const&);
template long VeriPB::Prooflogger::unchecked_assumption<VeriPB::Constraint<VeriPB::Lit, int, int> >(VeriPB::Constraint<VeriPB::Lit, int, int> const&);

template long VeriPB::Prooflogger::reification_literal_right_implication<VeriPB::Lit, VeriPB::Constraint<VeriPB::Lit, unsigned int, unsigned int> >(VeriPB::Lit const&, VeriPB::Constraint<VeriPB::Lit, unsigned int, unsigned int> const&, bool);
template long VeriPB::Prooflogger::reification_literal_left_implication<VeriPB::Lit, VeriPB::Constraint<VeriPB::Lit, unsigned int, unsigned int> >(VeriPB::Lit const&, VeriPB::Constraint<VeriPB::Lit, unsigned int, unsigned int> const&, bool);

template long VeriPB::Prooflogger::reification_literal_right_implication<VeriPB::Lit, VeriPB::Constraint<VeriPB::Lit, int, int> >(VeriPB::Lit const&, VeriPB::Constraint<VeriPB::Lit, int, int> const&, bool);
template long VeriPB::Prooflogger::reification_literal_left_implication<VeriPB::Lit, VeriPB::Constraint<VeriPB::Lit, int, int> >(VeriPB::Lit const&, VeriPB::Constraint<VeriPB::Lit, int, int> const&, bool);
