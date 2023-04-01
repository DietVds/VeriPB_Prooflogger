#ifndef SortingNetworkProoflogger_h
#define SortingNetworkProoflogger_h

#include "VeriPBProoflogger.h"

class SortingNetworkProoflogger
{
    

public:
    // Constructor
    SortingNetworkProoflogger(VeriPbProofLogger *PL);

    template <class TSeqLit, class TSeqCxnId>
    constraintid derive_evens_leq_odds(TSeqLit& lits, TSeqCxnId& sortedness_lits);

    template <class TSeqLit, class TSeqCxnId>
    constraintid derive_odds_leq_evens_plus_1(TSeqLit& lits, TSeqCxnId& sortedness_lits);

    template <class TSeqLit, class TSeqCxnId>
    constraintid derive_counting_definition_of_outputvars_right(TSeqLit& inputlits, TSeqLit& outputlits, int j, constraintid input_geq_output, TSeqCxnId& sortedness_outputlits);

    template <class TSeqLit, class TSeqCxnId>
    constraintid derive_counting_definition_of_outputvars_left(TSeqLit& inputlits, TSeqLit& outputlits, int j, constraintid output_geq_input, TSeqCxnId& sortedness_outputlits);


private:
    VeriPbProofLogger *PL;
};

#endif