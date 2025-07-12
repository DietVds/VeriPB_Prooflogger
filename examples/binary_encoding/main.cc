#include "Prooflogger.h"


int main() { 
    // Original variables
    VeriPB::Var x1 = {.v=1, .only_known_in_proof=false}, 
                x2 = {.v=2, .only_known_in_proof=false}, 
                x3 = {.v=3, .only_known_in_proof=false},
                x4 = {.v=4, .only_known_in_proof=false},
                x5 = {.v=5, .only_known_in_proof=false},
                x6 = {.v=6, .only_known_in_proof=false};

    // Set VarMgr
    VeriPB::VarManager _varMgr;
    _varMgr.set_number_original_variables(6);

    // Creation of PL object
    VeriPB::Prooflogger pl("testproof.pbp", &_varMgr);

    // Set original constraints
    pl.set_n_orig_constraints(1);
    pl.write_proof_header();

// -----------------------------------------------------------------------------------------------------

    VeriPB::LinTermBoolVars<VeriPB::Lit, uint32_t, uint32_t> T;
    T.add_literal(VeriPB::create_literal(x1, false), 5);
    T.add_literal(VeriPB::create_literal(x2, true), 3);
    T.add_literal(VeriPB::create_literal(x3, true), 7);
    
    pl.write_comment("start create_binary_encoding");
    VeriPB::Prooflogger::BinaryEncodingForLinearTerm<uint32_t, uint32_t> b1 = pl.create_binary_encoding<VeriPB::LinTermBoolVars<VeriPB::Lit, uint32_t, uint32_t>, uint32_t, uint32_t>(T, 1);
    pl.write_comment("end create_binary_encoding");
    std::string comment_binenc = "Created binary encoding";
    for(int i = 0; i < b1.T.size(); i++) comment_binenc += (i == 0 ? " " :  " + ") + VeriPB::number_to_string(VeriPB::coefficient(b1.T, i)) + " " + pl.get_variable_manager()->literal_to_string(VeriPB::literal(b1.T, i));
    comment_binenc += " for ";
    for(int i = 0; i < T.size(); i++) comment_binenc += (i == 0 ? " " :  " + ") + VeriPB::number_to_string(VeriPB::coefficient(T, i)) + " " + pl.get_variable_manager()->literal_to_string(VeriPB::literal(T, i));
    pl.write_comment(comment_binenc);
    std::cout << "b1.bin_geq_input " << b1.bin_geq_input << " b1.bin_leq_input " << b1.bin_leq_input << std::endl;

    T.clear();
    T.add_literal(VeriPB::create_literal(x4, false), 4);
    T.add_literal(VeriPB::create_literal(x5, true), 2);
    T.add_literal(VeriPB::create_literal(x6, true), 8);

    pl.write_comment("start create_binary_encoding");
    VeriPB::Prooflogger::BinaryEncodingForLinearTerm<uint32_t, uint32_t> b2 = pl.create_binary_encoding<VeriPB::LinTermBoolVars<VeriPB::Lit, uint32_t, uint32_t>, uint32_t, uint32_t>(T, 2);
    pl.write_comment("end create_binary_encoding");
    comment_binenc = "Created binary encoding";
    for(int i = 0; i < b2.T.size(); i++) comment_binenc += (i == 0 ? " " :  " + ") + VeriPB::number_to_string(VeriPB::coefficient(b2.T, i)) + " " + pl.get_variable_manager()->literal_to_string(VeriPB::literal(b2.T, i));
    comment_binenc += " for ";
    for(int i = 0; i < T.size(); i++) comment_binenc += (i == 0 ? " " :  " + ") + VeriPB::number_to_string(VeriPB::coefficient(T, i)) + " " + pl.get_variable_manager()->literal_to_string(VeriPB::literal(T, i));
    pl.write_comment(comment_binenc);
    std::cout << "b2.bin_geq_input " << b2.bin_geq_input << " b2.bin_leq_input " << b2.bin_leq_input << std::endl;

    pl.write_comment("start create_binary_addition");
    VeriPB::Prooflogger::BinaryEncodingForLinearTerm<uint32_t, uint32_t> b3 = pl.create_binary_addition<VeriPB::LinTermBoolVars<VeriPB::Lit, uint32_t, uint32_t>, uint32_t, uint32_t>(b1.T, b2.T);
    pl.write_comment("end create_binary_addition");
    comment_binenc = "Resulting in sum";
    for(int i = 0; i < b3.T.size(); i++) comment_binenc += (i == 0 ? " " :  " + ") + VeriPB::number_to_string(VeriPB::coefficient(b3.T, i)) + " " + pl.get_variable_manager()->literal_to_string(VeriPB::literal(b3.T, i));
    pl.write_comment(comment_binenc);
    std::cout << "b3.bin_geq_input " << b3.bin_geq_input << " b3.bin_leq_input " << b3.bin_leq_input << std::endl;

    VeriPB::CuttingPlanesDerivation CP = pl.new_cuttingplanes_derivation(true);
    pl.write_comment("Derive binary number >= input");
    CP.start_from_constraint(b3.bin_geq_input);
    if(b1.bin_geq_input != VeriPB::undefcxn) CP.add_constraint(b1.bin_geq_input);
    if(b2.bin_geq_input != VeriPB::undefcxn) CP.add_constraint(b2.bin_geq_input);
    CP.end();
    pl.write_comment("Derive binary number =< input");
    CP.start_from_constraint(b3.bin_leq_input);
    if(b1.bin_leq_input != VeriPB::undefcxn) CP.add_constraint(b1.bin_leq_input);
    if(b2.bin_leq_input != VeriPB::undefcxn) CP.add_constraint(b2.bin_leq_input);
    CP.end();

    pl.flush_proof();

// ----------------------------------------------------------------------------------------------------
    

    pl.write_conclusion_NONE();

    return 0;
}