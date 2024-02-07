#include "TotalizerProoflogger.h"
#include <iostream>

//=================================================================================================
// Constructor

//prooflogging Library


TotalizerProoflogger::TotalizerProoflogger(VeriPbProofLogger *PL) : PL(PL){}

// ------------- Constraint Stores -------------
template <class TVar> 
constraintid TotalizerProoflogger::get_PbDef_Impl_CxnId(TVar& var){
    assert(PB_impl_cxn_store.find(varidx(toVeriPbVar(var))) != PB_impl_cxn_store.end());

    return PB_impl_cxn_store[varidx(toVeriPbVar(var))];
}


template <class TVar> 
constraintid TotalizerProoflogger::get_PbDef_invImpl_CxnId(TVar& var){
    assert(PB_invImpl_cxn_store.find(varidx(toVeriPbVar(var))) != PB_invImpl_cxn_store.end());

    return PB_invImpl_cxn_store[varidx(toVeriPbVar(var))];
}

// ------------- Meaningful names -------------
template <class TVar, class TSeqLit>
void TotalizerProoflogger::store_meaningful_name_counting_var(const TVar& var, const wght n, const TSeqLit& leafs){
    if(meaningful_names_counting_vars){
        std::string mn = PL->var_name(var) + "_v" + std::to_string(n) ;
        for(int i = 0; i < leafs.size(); i++) 
            mn += "-x" + PL->to_string(leafs[i]);
        PL->store_meaningful_name(var, mn);
    }
}

// ------------- Totalizer functions: PB definitions -------------
template <class TVar, class TSeqLit>
void TotalizerProoflogger::write_PBdefs(const TVar& var, const TSeqLit &leafs, const wght j)
{
    write_PBdef_impl(var, leafs, j); // Only necessary with int because totalizers are made using the pysat datastructures.
    write_PBdef_invImpl(var, leafs, j);// Only necessary with int because totalizers are made using the pysat datastructures.
}

template <class TVar, class TSeqLit>
void TotalizerProoflogger::write_PBdef_impl(const TVar& var, const TSeqLit &leafs, const wght j)
{
    VeriPB::Var _var = toVeriPbVar(var);
    VeriPB::Lit _litvar = create_literal(_var, false);

    std::string strleafs;
    for(int i = 0; i < leafs.size(); i++) strleafs += " " + PL->to_string(leafs[i]);
    PL->write_comment("P->" + std::to_string(j) + " for var " + PL->to_string(_litvar) + " with leafs: " + strleafs);

    std::vector<VeriPB::Lit> lits; lits.resize(leafs.size() + 1);
    std::vector<wght> weights; weights.resize(leafs.size() + 1);

    for (int i = 0; i < leafs.size(); i++){
        lits[i] = toVeriPbLit(neg(leafs[i]));
        weights[i] = 1;
    }

    wght v = leafs.size() - j + 1;
    lits[leafs.size()] = _litvar;
    weights[leafs.size()] = v;

    substitution<VeriPB::Var> witness;
    witness.push_back({_var, true});

    constraintid cxn = PL->redundanceBasedStrengthening(lits, weights, v, witness);

    PB_impl_cxn_store[varidx(_var)] = cxn;

}

template <class TVar, class TSeqLit>
void TotalizerProoflogger::write_PBdef_invImpl(const TVar& var, const TSeqLit &leafs, const wght j)
{
    VeriPB::Var _var = toVeriPbVar(var);
    VeriPB::Lit _litvar = create_literal(_var, false);

    std::string strleafs;
    for(int i = 0; i < leafs.size(); i++) strleafs += " " + PL->to_string(leafs[i]);
    PL->write_comment("P<-" + std::to_string(j) + " for var " + PL->to_string(_litvar) + " with leafs: " + strleafs);

    std::vector<VeriPB::Lit> lits; lits.resize(leafs.size() + 1);
    std::vector<wght> weights; weights.resize(leafs.size() + 1);

    for(int i = 0; i < leafs.size(); i++){
        lits[i] = toVeriPbLit(leafs[i]);
        weights[i] = 1;
    }

    lits[leafs.size()] = neg(_litvar);
    weights[leafs.size()] = j;

    substitution<VeriPB::Var> witness;
    witness.push_back({_var, false});

    constraintid cxn = PL->redundanceBasedStrengthening(lits, weights, j, witness);

    PB_invImpl_cxn_store[varidx(_var)] = cxn;
}

// ------------- Totalizer functions: CP derivations of totalizer clauses -------------
template <class TVar, class TSeqLit>
void TotalizerProoflogger::prove_binary_implCls(const TVar &var, const TVar &varchild, const TSeqLit &leafs_varchild, const TSeqLit &leafs_other_child, const TSeqLit& clause_to_derive)
{
    PL->write_comment("prove_binary_implCls - var = " + PL->to_string(create_literal(var, false)) + " varchild = " + PL->to_string(create_literal(varchild, false)));
    PL->start_CP_derivation(PB_impl_cxn_store[varidx(toVeriPbVar(var))]);

    if (leafs_varchild.size() > 1 && PB_invImpl_cxn_store.find(varidx(toVeriPbVar(varchild))) != PB_invImpl_cxn_store.end())
        PL->CP_add_constraint(PB_invImpl_cxn_store[varidx(toVeriPbVar(varchild))]);

    for (int i = 0; i < leafs_other_child.size(); i++)
        PL->CP_weaken(variable(leafs_other_child[i]));

    PL->CP_saturate();
    PL->end_CP_derivation(); 

    PL->check_last_constraint(clause_to_derive);   
}


template <class TVar, class TSeqLit>
void TotalizerProoflogger::prove_ternary_implCls(const TVar &var, const TVar &varchild1, const TVar &varchild2, const TSeqLit &leafs1, const TSeqLit &leafs2, const TSeqLit& clause_to_derive)
{
    PL->write_comment("prove_ternary_implCls");
    PL->start_CP_derivation(PB_impl_cxn_store[varidx(toVeriPbVar(var))]);

    if (leafs1.size() > 1 && PB_invImpl_cxn_store.find(varidx(toVeriPbVar(varchild1))) != PB_invImpl_cxn_store.end()) // Don't write the P1/P2 definitions for alpha=1, since the counting variable is then equal to the variable itself.
        PL->CP_add_constraint(PB_invImpl_cxn_store[varidx(toVeriPbVar(varchild1))]);
    
    if (leafs2.size() > 1 && PB_invImpl_cxn_store.find(varidx(toVeriPbVar(varchild2))) != PB_invImpl_cxn_store.end()) // Don't write the P1/P2 definitions for beta=1, since the counting variable is then equal to the variable itself.
        PL->CP_add_constraint(PB_invImpl_cxn_store[varidx(toVeriPbVar(varchild2))]);

    PL->CP_saturate();
    PL->end_CP_derivation();

    PL->check_last_constraint(clause_to_derive);
}

template <class TVar, class TSeqLit>
void TotalizerProoflogger::prove_binary_invImplCls(const TVar &var, const TVar &varchild, const TSeqLit &leafs_varchild, const TSeqLit &leafs_other_child, const TSeqLit& clause_to_derive)
{
    PL->write_comment("prove_binary_invImplCls");
    PL->start_CP_derivation(PB_invImpl_cxn_store[varidx(toVeriPbVar(var))]);

    if (leafs_varchild.size() > 1 && PB_impl_cxn_store.find(varidx(toVeriPbVar(varchild))) != PB_impl_cxn_store.end())
        PL->CP_add_constraint(PB_impl_cxn_store[varidx(toVeriPbVar(varchild))]);

    for (int i = 0; i < leafs_other_child.size(); i++) // Weaken the leafs of the other child
        PL->CP_weaken(variable(leafs_other_child[i]));
    
    PL->CP_saturate();
    PL->end_CP_derivation();

    PL->check_last_constraint(clause_to_derive);
}

template <class TVar, class TSeqLit>
void TotalizerProoflogger::prove_ternary_invImplCls(const TVar &var, const TVar &varchild1, const TVar &varchild2, const TSeqLit &leafs1, const TSeqLit &leafs2, const TSeqLit& clause_to_derive)
{
    PL->write_comment("prove_ternary_invImplCls");
    PL->start_CP_derivation(PB_invImpl_cxn_store[varidx(toVeriPbVar(var))]);

    if (leafs1.size() > 1 && PB_impl_cxn_store.find(varidx(toVeriPbVar(varchild1))) != PB_impl_cxn_store.end()) // Don't write the P1/P2 definitions for alpha=1, since the counting variable is then equal to the variable itself.
        PL->CP_add_constraint(PB_impl_cxn_store[varidx(toVeriPbVar(varchild1))]);
    
    if (leafs2.size() > 1 &&  PB_impl_cxn_store.find(varidx(toVeriPbVar(varchild2))) != PB_impl_cxn_store.end()) // Don't write the P1/P2 definitions for beta=1, since the counting variable is then equal to the variable itself.
        PL->CP_add_constraint(PB_impl_cxn_store[varidx(toVeriPbVar(varchild2))]);
    
    PL->CP_saturate();
    PL->end_CP_derivation();

    PL->check_last_constraint(clause_to_derive, 1);
}

template <class TLit>
void TotalizerProoflogger::prove_unitclause_constraining_totalizer(const TLit& c, const wght n, const wght bestval, const constraintid best_solution_constraint)
{
    if (PB_invImpl_cxn_store.find(varidx(toVeriPbVar(variable(c)))) != PB_invImpl_cxn_store.end()){ 
        constraintid pbdef_var = PB_invImpl_cxn_store[varidx(toVeriPbVar(variable(c)))];
        
        PL->start_CP_derivation(pbdef_var);
        PL->CP_add_constraint(best_solution_constraint);
        PL->CP_divide(n-bestval+1);
        PL->CP_saturate();
        PL->end_CP_derivation();

        // The P2-constraint is not necessary anymore and can therefore be deleted.
        PL->delete_constraint_by_id(pbdef_var);
        PB_invImpl_cxn_store.erase(pbdef_var);

        std::vector<VeriPB::Lit> cls; cls.push_back(toVeriPbLit(c));
        PL->check_last_constraint(cls, 1);  
    }
}


// ------------- Structure Sharing -------------
template <class TVar> 
void TotalizerProoflogger::ss_add_parent_using_pb_def(const TVar& var){
    // This function can also be called from a node for which the children are leafs. There are no PBdefs defined for variables in the leafs.
    // Therefore, we only add a parent in case the pbdef exists.
    // TODO: check what happens when we do not add the nr parents? Is it okay to just add it here? What happens in the delete function?
    add_parent_using_pb_def(var, PB_impl_cxn_store, PB_impl_nrUsed_store);
    add_parent_using_pb_def(var, PB_invImpl_cxn_store, PB_invImpl_nrUsed_store);
}

template <class TVar>
void TotalizerProoflogger::add_parent_using_pb_def(const TVar& var, std::map<VeriPB::VarIdx, constraintid>& pb_def_store, std::map<VeriPB::VarIdx, constraintid>& pb_def_parents_store){
    if(pb_def_store.find(varidx(toVeriPbVar(var))) != pb_def_store.end()){ 
        if(pb_def_parents_store.find(varidx(toVeriPbVar(var))) == pb_def_parents_store.end())
            pb_def_parents_store[varidx(toVeriPbVar(var))] = 1;
        else
            pb_def_parents_store[varidx(toVeriPbVar(var))]++;
    }
}

template <class TVar> 
void TotalizerProoflogger::ss_set_nr_parents_using_pb_def(const TVar& var, int n){
    assert(PB_impl_cxn_store.find(varidx(toVeriPbVar(var))) != PB_impl_cxn_store.end());
    assert(PB_invImpl_cxn_store.find(varidx(toVeriPbVar(var))) != PB_invImpl_cxn_store.end());
    
    PB_impl_nrUsed_store[varidx(toVeriPbVar(var))] = n;
    PB_invImpl_nrUsed_store[varidx(toVeriPbVar(var))] = n;
}

template <class TVar> 
int TotalizerProoflogger::ss_get_nr_parents_using_pb_def(const TVar& var){
    assert(PB_impl_cxn_store.find(varidx(toVeriPbVar(var))) != PB_impl_cxn_store.end());

    return PB_impl_nrUsed_store[varidx(toVeriPbVar(var))];
}

// ------------- Deletes (Totalizer) -------------
template <class TVar> 
void TotalizerProoflogger::delete_PB_definitions(const TVar& var){
    delete_PB_Impl_def(var);
    delete_PB_invImpl_def(var);
}

template <class TVar> 
void TotalizerProoflogger::delete_PB_Impl_def(const TVar &var){
    delete_P_definition(var, PB_impl_cxn_store, PB_impl_nrUsed_store);
}


template <class TVar> 
void TotalizerProoflogger::delete_PB_invImpl_def(const TVar &var){
    delete_P_definition(var, PB_invImpl_cxn_store, PB_invImpl_nrUsed_store);
}

template <class TVar> 
void TotalizerProoflogger::delete_P_definition(const TVar& var, std::map<VeriPB::VarIdx, constraintid>& pb_cxn_store, std::map<VeriPB::VarIdx, int>& pb_nrUsed_store){
    if (pb_cxn_store.find(varidx(toVeriPbVar(var))) != pb_cxn_store.end()){ // If the pb def exists, only then we could delete it.
        if(pb_nrUsed_store.find(varidx(toVeriPbVar(var))) != pb_nrUsed_store.end() && pb_nrUsed_store[varidx(toVeriPbVar(var))] > 1){ 
            pb_nrUsed_store[varidx(toVeriPbVar(var))]--;
        }
        else{
            constraintid c_id = pb_cxn_store[varidx(toVeriPbVar(var))];
            PL->delete_constraint_by_id(c_id); 

            pb_cxn_store.erase(varidx(toVeriPbVar(var)));
            pb_nrUsed_store.erase(varidx(toVeriPbVar(var)));
        }   
    }    
}

template <class TVar> 
void TotalizerProoflogger::delete_unnecessary_PBdefs_childnodes(const std::vector<TVar>& av, const std::vector<TVar>& bv, const int parent_nr_lits_previous, const int parent_nr_lits_current, const bool iterative_encoding_needed){
    // Deleting unnecessary PB variable definitions
	PL->write_comment("Deleting unnecessary PB variable definitions");

	if (!iterative_encoding_needed 
			// || av.size() + bv.size() == ov.size() // This is already implied by the way max_delete_a is calculated.
	)
	{
		// Delete all the PB definitions of the leafs.
		for (int i = 0; i < av.size(); i++)
			delete_PB_definitions(av[i]);
            
		for (int i = 0; i < bv.size(); i++)
            delete_PB_definitions(bv[i]);
        
	}
	else
	{
		// Only delete the PB definitions that could not be used to extend the tree

		// If a value a exists s.t. a + bv.size() <= o.size(), 
		// then the definitions of the variable representing at least a true input variables 
		// is not needed anymore. It will not be used to define any value greater than ov.size, 
		// which is the current largest value already.
		int max_delete_a = parent_nr_lits_current - bv.size(); 
		int max_delete_b = parent_nr_lits_current - av.size();
        int min_deletes_a = parent_nr_lits_previous - bv.size(); // TODO: Check this!!!
        int min_deletes_b = parent_nr_lits_previous - av.size();
        
        for (int i = std::max(0, min_deletes_a); i < max_delete_a; i++) {
            delete_PB_definitions(av[i]);
        }
			
		for (int i = std::max(0, min_deletes_b); i < max_delete_b; i++){
			delete_PB_definitions(bv[i]);
        }
	}
}
// -------------------------------------------------------