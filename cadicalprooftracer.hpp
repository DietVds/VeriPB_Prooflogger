#ifndef _cadicalprooftracer_h_INCLUDED
#define _cadicalprooftracer_h_INCLUDED

#include "VeriPBProoflogger.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

#include "../solver-proxy/cadical/src/cadical.hpp"
#include "../solver-proxy/cadical/src/tracer.hpp"


using namespace std;

// namespace CaDiCaL {
using namespace CaDiCaL;

class CadicalProofTracer : public Tracer {

  VeriPbProofLogger *vPL;

  bool with_antecedents;
  bool checked_deletions;

  // hash table for checked deletions
  //
  uint64_t num_clauses;  // number of clauses in hash table
  uint64_t size_clauses; // size of clause hash table
  std::unordered_map<uint64_t, constraintid> clauses_vpb; // connect VeriPB id to cadical id 
  std::unordered_set<uint64_t> weakened_clauses; // save all clause identifiers that can later be stored

  int64_t added, deleted;

  vector<uint64_t> delete_ids;

  // support veriPB
  void veripb_add_derived_clause (uint64_t, bool redundant,
                                  const vector<int> &clause,
                                  const vector<uint64_t> &chain);
  void veripb_add_derived_clause (uint64_t, bool redundant,
                                  const vector<int> &clause);
  void veripb_delete_clause (uint64_t id, bool redundant);
  void veripb_finalize_proof (uint64_t conflict_id);
  void veripb_strengthen (uint64_t);

public:
  // own and delete 'file'
  CadicalProofTracer(bool antecedents, bool check_deletion, VeriPbProofLogger *vPL);
  ~CadicalProofTracer ();

  void add_original_clause (uint64_t, bool, const vector<int> &,
                            bool = false) override;

  void add_derived_clause (uint64_t, bool, const vector<int> &,
                           const vector<uint64_t> &) override;

  void delete_clause (uint64_t, bool, const vector<int> &) override;
  void finalize_clause (uint64_t, const vector<int> &) override {} // skip

  void weaken_minus (uint64_t, const vector<int> &) override;
  void strengthen (uint64_t) override;
};


// } // namespace CaDiCaL

#endif
