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

  if(vPL->comments)
    vPL->write_comment("Cadical PT: add derived clause with id " + std::literal_to_string(id) + " and veripb id " + std::literal_to_string(vPL->constraint_counter+1));
  
  *vPL->proof << "rup ";
  for(auto lit : clause) 
    vPL->write_weighted_literal(lit);
  
  *vPL->proof << " >= 1 ; ";
  
  for (auto p = chain.rbegin (); p != chain.rend (); p++) {
    auto cid = *p;

    *vPL->proof << clauses_vpb[cid] << " ";
  }

  *vPL->proof << "\n";
  constraintid cxn = ++vPL->constraint_counter;

  clauses_vpb[id] = cxn;

  if (!redundant && checked_deletions) {
    vPL->move_to_coreset(cxn);
  }
}

void CadicalProofTracer::veripb_add_derived_clause (uint64_t id, bool redundant,
                                              const vector<int> &clause) {
  if(vPL->comments)
    vPL->write_comment("Cadical PT: add derived clause with id " + std::literal_to_string(id) + " and veripb id " + std::literal_to_string(vPL->constraint_counter+1));
  constraintid cxn = vPL->rup(clause);

  // assert(checked_deletions);
  if (!redundant && checked_deletions) {
    vPL->move_to_coreset(cxn);
  }

  clauses_vpb[id] = cxn;
}

void CadicalProofTracer::veripb_delete_clause (uint64_t id, bool redundant) {
  if(vPL->comments)
    vPL->write_comment("Cadical PT: delete clause with id " + std::literal_to_string(id) + " and veripb id " + std::literal_to_string(clauses_vpb[id]));
  
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
  if(vPL->comments)  
    vPL->write_comment("Cadical PT: add original clause with id " + std::literal_to_string(id) + " and veripb id " + std::literal_to_string(cxnid));
  // vPL->write_comment("next_cxnid = " + std::literal_to_string(next_cxnid));
  // std::string strcls = "Added clause:";
  // for(int l : clause){
  //   strcls += " " + vPL->literal_to_string(l);
  // }
  // vPL->write_comment(strcls);
  // vPL->write_comment("Is redundant? " + std::literal_to_string(redundant) + " Is restored?  "  + std::literal_to_string(restored));

  if(restored) return; // A restored clause keeps its original clause id's. 

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
