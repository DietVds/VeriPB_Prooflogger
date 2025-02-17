#include "VeriPbSolverTypes.hpp"
#include "VariableManager.hpp"
#include "VeriPBProoflogger.hpp"

template class VeriPB::LinTermBoolVars<VeriPB::Lit, unsigned int, unsigned int>;
template class VeriPB::Constraint<VeriPB::Lit, unsigned int, unsigned int>;
template class VeriPB::VeriPbProofLogger<VeriPB::Lit, unsigned int, unsigned int>;

template long VeriPB::VeriPbProofLogger<VeriPB::Lit, unsigned int, unsigned int>::unchecked_assumption<VeriPB::Constraint<VeriPB::Lit, unsigned int, unsigned int> >(VeriPB::Constraint<VeriPB::Lit, unsigned int, unsigned int> const&);
