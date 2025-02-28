#include "VeriPbSolverTypes.h"
#include "VeriPBProoflogger.h"

int main() { 
    // Original variables
    VeriPB::Var x1 = {.v=1, .only_known_in_proof=false}, 
                x2 = {.v=2, .only_known_in_proof=false}, 
                x3 = {.v=3, .only_known_in_proof=false},
                x4 = {.v=4, .only_known_in_proof=false};

    // Original constraints
    VeriPB::Constraint<VeriPB::Lit, uint32_t, uint32_t> C1, C2(true, VeriPB::Comparison::LEQ), C3;

    // 3 x1 + 2 x2 + 2 x3 >= 3
    C1.add_literal(VeriPB::create_literal<VeriPB::Lit, VeriPB::Var>(x1, false), 3);
    C1.add_literal(VeriPB::create_literal<VeriPB::Lit, VeriPB::Var>(x2, true), 2);
    C1.add_literal(VeriPB::create_literal<VeriPB::Lit, VeriPB::Var>(x3, true), 2);
    C1.add_RHS(3);

    // x1 + x2 + x3 + x4 =< 2
    C2.add_literal(VeriPB::create_literal<VeriPB::Lit, VeriPB::Var>(x1, false));
    C2.add_literal(VeriPB::create_literal<VeriPB::Lit, VeriPB::Var>(x2, false));
    C2.add_literal(VeriPB::create_literal<VeriPB::Lit, VeriPB::Var>(x3, false));
    C2.add_literal(VeriPB::create_literal<VeriPB::Lit, VeriPB::Var>(x4, false));
    C2.add_RHS(2);

    // ~x1 + ~x2 >= 1
    C3.add_literal(VeriPB::create_literal<VeriPB::Lit, VeriPB::Var>(x1, true));
    C3.add_literal(VeriPB::create_literal<VeriPB::Lit, VeriPB::Var>(x2, true));
    C3.add_RHS(1);

    
    // Creation of PL object
    VeriPB::ProofloggerOpt<VeriPB::Lit, uint32_t, uint32_t> vPL;

    // Set proof stream
    std::ofstream prooffilestream;
    prooffilestream.open("maxsat_proof.pbp");
    vPL.set_proof_stream(&prooffilestream);

    // Set VarMgr
    VeriPB::VarManager varMgr;
    varMgr.set_number_original_variables(4);
    vPL.set_variable_manager(&varMgr);

    // Set original constraints
    vPL.set_n_orig_constraints(0);
    vPL.write_proof_header();

    // Set objective to ~x1 + ~x2 + ~x3 + ~x4
    vPL.add_objective_literal(VeriPB::create_literal<VeriPB::Lit, VeriPB::Var>(x1, true),1);
    vPL.add_objective_literal(VeriPB::create_literal<VeriPB::Lit, VeriPB::Var>(x2, true),1);
    vPL.add_objective_literal(VeriPB::create_literal<VeriPB::Lit, VeriPB::Var>(x3, true),1);
    vPL.add_objective_literal(VeriPB::create_literal<VeriPB::Lit, VeriPB::Var>(x4, true),1);

    // Unchecked assumption of the three constraints.

    vPL.unchecked_assumption(C1);
    vPL.unchecked_assumption(C2);
    vPL.unchecked_assumption(C3);

    vPL.write_conclusion_NONE();

    prooffilestream.close();

    return 0;
}