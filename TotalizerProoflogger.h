#ifndef TotalizerProoflogger_h
#define TotalizerProoflogger_h

#include "VeriPBProoflogger.h"

//=================================================================================================
// Prooflogger

class TotalizerProoflogger
{
public:
    // Constructor
    TotalizerProoflogger(VeriPbProofLogger *PL);
    
    // Constraint stores of the PB definitions of counting variables
    std::map<VeriPB::Var, constraintid> PB_impl_cxn_store;
    std::map<VeriPB::Var, constraintid> PB_invImpl_cxn_store;

    template <class TVar> constraintid get_PbDef_Impl_CxnId(TVar& var);
    template <class TVar> constraintid get_PbDef_invImpl_CxnId(TVar& var);

    // A linking variable in a totalizer can be used by more than one parents in case of structure sharing. 
    // Therefore, we will keep track of how many parents are using this variable. Only no other parents are still using it, then we can delete it.
    std::map<VeriPB::Var, int> PB_impl_nrUsed_store;
    std::map<VeriPB::Var, int> PB_invImpl_nrUsed_store;

    // Meaningful names
    bool meaningful_names_counting_vars = true;
    template <class TVar, class TLit> void store_meaningful_name_counting_var(const TVar& var, const int n, const std::vector<TLit>& leafs);
    
    // Totalizer functions: PB definitions
    template <class TVar, class TLit>   void write_PBdefs(const TVar& var, const std::vector<TLit> &leafs, const int j);
    template <class TVar, class TLit>   void write_PBdef_impl(const TVar& var, const std::vector<TLit> &leafs, const int j);
    template <class TVar, class TLit>   void write_PBdef_invImpl(const TVar& var, const std::vector<TLit> &leafs, const int j);

    // Totalizer functions: CP derivations of totalizer clauses
    // TODO: add the constraints that should be derived as a extra argumentT
    template <class TVar, class TLit> void prove_binary_implCls(const TVar &var, const TVar &varchild, const std::vector<TLit> &leafs_other_child, const std::vector<TLit>& clause_to_derive);
    template <class TVar, class TLit> void prove_ternary_implCls(const TVar &var, const TVar &varchild1, const TVar &varchild2, const std::vector<TLit>& clause_to_derive);
    template <class TVar, class TLit> void prove_binary_invImplCls(const TVar &var, const TVar &varchild, const std::vector<TLit> &leafs_other_child, const std::vector<TLit>& clause_to_derive);
    template <class TVar, class TLit> void prove_ternary_invImplCls(const TVar &var, const TVar &varchild1, const TVar &varchild2, const std::vector<TLit>& clause_to_derive);
    template <class TLit> void prove_unitclause_constraining_totalizer(const TLit& clause, const int n, const int bestval, const constraintid best_solution_constraint);

    // Structure Sharing
    template <class TVar> void add_parent_using_pb_def(const TVar& var, std::map<VeriPB::Var, constraintid>& pb_def_store, std::map<VeriPB::Var, constraintid>& pb_def_parents);
    template <class TVar> void ss_set_nr_parents_using_pb_def(const TVar& var, int n);
    template <class TVar> void ss_add_parent_using_pb_def(const TVar& var);

    // Deletes (Totalizer)
    template <class TVar> void delete_PB_definitions(const TVar &var);
    template <class TVar> void delete_PB_Impl_def(const TVar &var);
    template <class TVar> void delete_PB_invImpl_def(const TVar &var);
    template <class TVar> void delete_P_definition(const TVar& var, std::map<TVar, constraintid>& pb_cxn_store, std::map<TVar, int>& pb_nrUsed_store);
    template <class TVar> void delete_unnecessary_PBdefs(const std::vector<TVar> &av, const std::vector<TVar> &bv, const std::vector<TVar> &ov, const bool iterative_encoded = true);
private: 
    VeriPbProofLogger* PL;
};

//=================================================================================================

#endif