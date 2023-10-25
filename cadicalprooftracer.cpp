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

  cuttingplanes_derivation cpder;
  bool first = true;
  for(int i = 0; i < chain.size(); i++){
    if(first){
      first = false;
      cpder = vPL->CP_constraintid(clauses_vpb[chain[i]]);
    }
    else{
      cpder = vPL->CP_addition(cpder, vPL->CP_constraintid(clauses_vpb[chain[i]]));
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

  bool marked = weakened_clauses.find(id) != weakened_clauses.end();

  if(marked) weakened_clauses.erase(id);

  if (!redundant && checked_deletions && marked) 
    return;
 
  vPL->delete_constraint(clauses_vpb[id]);

  clauses_vpb.erase(id);  
}

void CadicalProofTracer::veripb_strengthen (uint64_t id) {
  if (!checked_deletions)
    return;
  
  vPL->move_to_coreset(clauses_vpb[id]);
}

/*------------------------------------------------------------------------*/

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

/*------------------------------------------------------------------------*/

// } // namespace CaDiCaL
