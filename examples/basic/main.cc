#include "ProofloggerOptimization.h"

int main() { 
    // Original variables
    VeriPB::Var x1 = {.v=1, .only_known_in_proof=false}, 
                x2 = {.v=2, .only_known_in_proof=false}, 
                x3 = {.v=3, .only_known_in_proof=false},
                x4 = {.v=4, .only_known_in_proof=false},
                x5 = {.v=5, .only_known_in_proof=false},
                x6 = {.v=6, .only_known_in_proof=false};

    // Original constraints
    VeriPB::Constraint<VeriPB::Lit, uint32_t, uint32_t> C1, C2(true, VeriPB::Comparison::LEQ), C3;

    // 3 x1 + 2 x2 + 2 x3 >= 3
    C1.add_literal(VeriPB::create_literal(x1, false), 3);
    C1.add_literal(VeriPB::create_literal(x2, true), 2);
    C1.add_literal(VeriPB::create_literal(x3, true), 2);
    C1.add_RHS(3);

    // x1 + x2 + x3 + x4 =< 2
    C2.add_literal(VeriPB::create_literal(x1, false));
    C2.add_literal(VeriPB::create_literal(x2, false));
    C2.add_literal(VeriPB::create_literal(x3, false));
    C2.add_literal(VeriPB::create_literal(x4, false));
    C2.add_RHS(2);

    // ~x1 + ~x2 >= 1
    C3.add_literal(VeriPB::create_literal(x1, true));
    C3.add_literal(VeriPB::create_literal(x2, true));
    C3.add_RHS(1);

    
    // Set VarMgr
    VeriPB::VarManager varMgr;
    varMgr.set_number_original_variables(6);

    // Creation of PL object
    VeriPB::ProofloggerOpt<VeriPB::Lit, uint32_t, uint32_t> vPL("testproof.pbp", &varMgr);

    // Set original constraints
    vPL.set_n_orig_constraints(0);
    vPL.write_proof_header();

    // Set objective to ~x1 + ~x2 + ~x3 + ~x4
    vPL.add_objective_literal(VeriPB::create_literal(x1, true),1);
    vPL.add_objective_literal(VeriPB::create_literal(x2, true),1);
    vPL.add_objective_literal(VeriPB::create_literal(x3, true),1);
    vPL.add_objective_literal(VeriPB::create_literal(x4, true),1);

    // Unchecked assumption of the three constraints.

    vPL.unchecked_assumption(C1);
    vPL.unchecked_assumption(C2);
    vPL.unchecked_assumption(C3);

    // Introduction of reification constraints
    VeriPB::Var y5 = {.v=11, .only_known_in_proof=false};
    VeriPB::Lit ly5 = VeriPB::create_literal(y5, false);

    vPL.reification_literal_right_implication(ly5, C1);
    vPL.reification_literal_left_implication(ly5, C1);

    VeriPB::Constraint<VeriPB::Lit, uint32_t, uint32_t> C1p(false, 0, VeriPB::Comparison::LEQ);
    for(int i = 0; i < C1.size(); i++) 
        C1p.add_literal(C1.literal(i), C1.coefficient(i));
    C1p.add_RHS(C1.rhs()); 

    VeriPB::Var y6 = {.v=12, .only_known_in_proof=false};
    VeriPB::Lit ly6 = VeriPB::create_literal(y6, false);

    vPL.reification_literal_right_implication(ly6, C1p);
    vPL.reification_literal_left_implication(ly6, C1p);


    // Reifications with negative coefficients and/or constant
    vPL.write_comment("Reifications with negative coefficients and/or constant");
    
    VeriPB::Var y1 = {.v=7, .only_known_in_proof=false}, 
                y2 = {.v=8, .only_known_in_proof=false}, 
                y3 = {.v=9, .only_known_in_proof=false},
                y4 = {.v=10, .only_known_in_proof=false};

    VeriPB::Lit ly1 = VeriPB::create_literal(y1, false),
                ly2 = VeriPB::create_literal(y2, false),
                ly3 = VeriPB::create_literal(y3, false),
                ly4 = VeriPB::create_literal(y4, false);

    // Define linear term 3 x1 + 5 ~x2 + 7 ~x3 - 2 x4 - 8 x5 - 6 ~x6 
    VeriPB::LinTermBoolVars<VeriPB::Lit, int, int> T1;
    T1.add_literal(VeriPB::create_literal(x1, false), 3);
    T1.add_literal(VeriPB::create_literal(x2, true), 5);
    T1.add_literal(VeriPB::create_literal(x3, true), 7);
    T1.add_literal(VeriPB::create_literal(x4, false), -2);
    T1.add_literal(VeriPB::create_literal(x5, false), -8);
    T1.add_literal(VeriPB::create_literal(x6, true), -6);

    VeriPB::Constraint<VeriPB::Lit, int, int>   C4(&T1, 3, VeriPB::Comparison::GEQ), // 3 x1 + 5 ~x2 + 7 ~x3 - 2 x4 - 8 x5 - 6 ~x6 >= 3
                                                C5(&T1, 2, VeriPB::Comparison::LEQ), // 3 x1 + 5 ~x2 + 7 ~x3 - 2 x4 - 8 x5 - 6 ~x6 <= 2
                                                C6(&T1, -3, VeriPB::Comparison::GEQ), // 3 x1 + 5 ~x2 + 7 ~x3 - 2 x4 - 8 x5 - 6 ~x6 >= -3
                                                C7(&T1, -3, VeriPB::Comparison::LEQ); // 3 x1 + 5 ~x2 + 7 ~x3 - 2 x4 - 8 x5 - 6 ~x6 <= -3

    
    
    vPL.reification_literal_right_implication(ly1, C4);
    vPL.reification_literal_left_implication(ly1, C4);

    vPL.reification_literal_right_implication(ly2, C5);
    vPL.reification_literal_left_implication(ly2, C5);

    vPL.reification_literal_right_implication(ly3, C6);
    vPL.reification_literal_left_implication(ly3, C6);

    vPL.reification_literal_right_implication(ly4, C7);
    vPL.reification_literal_left_implication(ly4, C7);

    vPL.write_conclusion_NONE();

    return 0;
}