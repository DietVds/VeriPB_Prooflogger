// #include "internal.hpp"
#include "cadicalprooftracer.hpp"

// namespace CaDiCaL {
using namespace CaDiCaL;

/*------------------------------------------------------------------------*/

CadicalProofTracer::CadicalProofTracer (bool antecedents, bool check_deletion, VeriPbProofLogger *vPL)
    : vPL (vPL), with_antecedents (antecedents), checked_deletions (check_deletion),
      num_clauses (0), size_clauses (0), added (0), deleted (0) {  
}

CadicalProofTracer::~CadicalProofTracer () {}

/*------------------------------------------------------------------------*/

void CadicalProofTracer::veripb_add_derived_clause(
    uint64_t id, bool redundant, const vector<int> &clause,
    const vector<uint64_t> &chain) {
  vPL->write_comment("Cadical PT: add derived clause with id " + std::to_string(id) + " and veripb id " + std::to_string(vPL->constraint_counter+1));
  
  cuttingplanes_derivation cpder;
  bool first = true;
  for (auto p = chain.rbegin (); p != chain.rend (); p++) {
    auto cid = *p;
    if(first){
      first = false;
      cpder = vPL->CP_constraintid(clauses_vpb[cid]);
    }
    else{
      cpder = vPL->CP_addition(cpder, vPL->CP_constraintid(clauses_vpb[cid]));
      cpder = vPL->CP_saturation(cpder);
    }
  }

  constraintid cxn = vPL->write_CP_derivation(cpder);

  clauses_vpb[id] = cxn;

  vPL->check_last_constraint(clause);

  if (!redundant && checked_deletions) {
    vPL->move_to_coreset(cxn);
  }
}

void CadicalProofTracer::veripb_add_derived_clause (uint64_t id, bool redundant,
                                              const vector<int> &clause) {

  constraintid cxn = vPL->rup(clause);

  // assert(checked_deletions);
  if (!redundant && checked_deletions) {
    vPL->move_to_coreset(cxn);
  }

  clauses_vpb[id] = cxn;
}

void CadicalProofTracer::veripb_delete_clause (uint64_t id, bool redundant) {

  vPL->write_comment("Cadical PT: delete clause with id " + std::to_string(id) );
  
  bool marked = weakened_clauses.find(id) != weakened_clauses.end();

  if(marked) weakened_clauses.erase(id);

  if (!redundant && checked_deletions && marked) 
    return;
 
  vPL->delete_constraint_by_id(clauses_vpb[id]);

  clauses_vpb.erase(id);  
}

void CadicalProofTracer::veripb_strengthen (uint64_t id) {
  if (!checked_deletions)
    return;
  
  vPL->move_to_coreset(clauses_vpb[id]);
}

/*------------------------------------------------------------------------*/

void CadicalProofTracer::add_original_clause (uint64_t id, bool redundant, const vector<int> &clause,
                            bool restored){
  constraintid cxnid = next_cxnid == undefcxn ? vPL->constraint_counter : next_cxnid;
  vPL->write_comment("Cadical PT: add original clause with id " + std::to_string(id) + " and veripb id " + std::to_string(cxnid));
  clauses_vpb[id] = cxnid;
  next_cxnid = undefcxn;
  added++;
}       

void CadicalProofTracer::add_derived_clause (uint64_t id, bool redundant,
                                       const vector<int> &clause,
                                       const vector<uint64_t> &chain) {
  if (with_antecedents)
    veripb_add_derived_clause (id, redundant, clause, chain);
  else
    veripb_add_derived_clause (id, redundant, clause);
  added++;
}

void CadicalProofTracer::delete_clause (uint64_t id, bool redundant,
                                  const vector<int> &) {
  veripb_delete_clause (id, redundant);
  deleted++;
}

void CadicalProofTracer::weaken_minus (uint64_t id, const vector<int> &) {
  if (!checked_deletions)
    return;
  weakened_clauses.insert(id);
}

void CadicalProofTracer::strengthen (uint64_t id) {
  veripb_strengthen (id);
}

void CadicalProofTracer::add_with_constraintid(constraintid veripb_id){
  next_cxnid = veripb_id;
}

uint64_t CadicalProofTracer::last_clause_id(){
  return added;
}

constraintid CadicalProofTracer::getVeriPbConstraintId(uint64_t clauseid){
  return clauses_vpb[clauseid];
}
/*------------------------------------------------------------------------*/

// } // namespace CaDiCaL
