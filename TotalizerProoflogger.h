#ifndef TotalizerProoflogger_h
#define TotalizerProoflogger_h

#include "VeriPBProoflogger.h"

//=================================================================================================
// Prooflogger

//prooflogging Library

class TotalizerProoflogger
{
private: 
    // Constraint stores of the PB definitions of counting variables
    // TODO: use VeriPBProoflogger's reification constraint store for this.
    std::map<VeriPB::VarIdx, constraintid> PB_impl_cxn_store;
    std::map<VeriPB::VarIdx, constraintid> PB_invImpl_cxn_store;

    // A linking variable in a totalizer can be used by more than one parents in case of structure sharing. 
    // Therefore, we will keep track of how many parents are using this variable. Only no other parents are still using it, then we can delete it.
    std::map<VeriPB::VarIdx, int> PB_impl_nrUsed_store;
    std::map<VeriPB::VarIdx, int> PB_invImpl_nrUsed_store;

public:
    // Constructor
    TotalizerProoflogger(VeriPbProofLogger *PL);
    
    template <class TVar> constraintid get_PbDef_Impl_CxnId(TVar& var);
    template <class TVar> constraintid get_PbDef_invImpl_CxnId(TVar& var);

    // Meaningful names
    bool meaningful_names_counting_vars = true;
    template <class TVar, class TSeqLit> 
    void store_meaningful_name_counting_var(const TVar& var, const wght n, const TSeqLit& leafs);
    
    // Totalizer functions: PB definitions
    template <class TVar, class TSeqLit>
    void write_PBdefs(const TVar& var, const TSeqLit &leafs, const wght j);
    template <class TVar, class TSeqLit>
    void write_PBdef_impl(const TVar& var, const TSeqLit &leafs, const wght j);
    template <class TVar, class TSeqLit>
    void write_PBdef_invImpl(const TVar& var, const TSeqLit &leafs, const wght j);

    // Totalizer functions: CP derivations of totalizer clauses
    template <class TVar, class TSeqLit>
    void prove_binary_implCls(const TVar &var, const TVar &varchild, const TSeqLit &leafs_varchild, const TSeqLit &leafs_other_child, const TSeqLit& clause_to_derive);
    template <class TVar, class TSeqLit>
    void prove_ternary_implCls(const TVar &var, const TVar &varchild1, const TVar &varchild2, const TSeqLit &leafs1, const TSeqLit &leafs2, const TSeqLit& clause_to_derive);
    template <class TVar, class TSeqLit>
    void prove_binary_invImplCls(const TVar &var, const TVar &varchild, const TSeqLit &leafs_varchild, const TSeqLit &leafs_other_child, const TSeqLit& clause_to_derive);
    template <class TVar, class TSeqLit>
    void prove_ternary_invImplCls(const TVar &var, const TVar &varchild1, const TVar &varchild2, const TSeqLit &leafs1, const TSeqLit &leafs2, const TSeqLit& clause_to_derive);
    template <class TLit>
    void prove_unitclause_constraining_totalizer(const TLit& clause, const wght n, const wght bestval, const constraintid best_solution_constraint);

    // Structure Sharing
    template <class TVar> 
    void add_parent_using_pb_def(const TVar& var, std::map<VeriPB::VarIdx, constraintid>& pb_def_store, std::map<VeriPB::VarIdx, constraintid>& pb_def_parents);
    template <class TVar>
    void ss_set_nr_parents_using_pb_def(const TVar& var, int n);
    template <class TVar>
    int ss_get_nr_parents_using_pb_def(const TVar& var);
    template <class TVar>
    void ss_add_parent_using_pb_def(const TVar& var);

    // Deletes (Totalizer)
    template <class TVar>
    void delete_PB_definitions(const TVar &var);
    template <class TVar>
    void delete_PB_Impl_def(const TVar &var);
    template <class TVar>
    void delete_PB_invImpl_def(const TVar &var);
    template <class TVar>
    void delete_P_definition(const TVar& var, std::map<VeriPB::VarIdx, constraintid>& pb_cxn_store, std::map<VeriPB::VarIdx, int>& pb_nrUsed_store);
    template <class TVar>
    void delete_unnecessary_PBdefs_childnodes(const std::vector<TVar> &av, const std::vector<TVar> &bv, const int parent_nr_lits_previous, const int parent_nr_lits_current,  const bool iterative_encoded = true);
private: 
    VeriPbProofLogger* PL;
};

//=================================================================================================

#endif
