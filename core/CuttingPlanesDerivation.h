#ifndef CuttingPlanesDerivation_h
#define CuttingPlanesDerivation_h

#include <string>
#include "VeriPbSolverTypes.h"
#include "Prooflogger.h"
#include <cassert>

namespace VeriPB {
    class CuttingPlanesDerivation
    {
        friend Prooflogger; 

        public:
        std::string toString() const;
        bool writing_directly_to_proof() const;

        void clear(); // asserts that not writing to proof.

        void start_from_constraint(const constraintid& cxn_id);
        template <class TLit>
        void start_from_literal_axiom(const TLit& lit);
        void add(const CuttingPlanesDerivation* cp_to_add); 
        void start_subderivation_from_constraint(const constraintid& cxn_id);
        template <class TLit>
        void start_subderivation_from_literal_axiom(const TLit& lit_axiom);
        void add_subderivation();
        template <class TNumber=VeriPB::defaultmultipliertype>
        void add_constraint(const constraintid& cxn_id, const TNumber& mult=1);
        template <class TLit, class TNumber=VeriPB::defaultmultipliertype>
        void add_literal_axiom(const TLit& lit_axiom, const TNumber& mult=1);
        template <class TNumber=VeriPB::defaultmultipliertype>
        void divide(const TNumber& n);
        void saturate();
        template <class TNumber=VeriPB::defaultmultipliertype>
        void multiply(const TNumber& n);
        template <class TVar>
        void weaken(const TVar& var);
        constraintid write_to_proof(bool clear=true);

        CuttingPlanesDerivation(Prooflogger* pl, bool write_directly_to_proof=false);

        private: 
        bool _write_directly_to_proof;
        bool _finished=true; // If derivation is written directly to the proof, a new derivation can only be started when the previous derivation is finished.
        
        Prooflogger* _pl;
        std::string* _buffer;
        bool _bufferOwned;

        CuttingPlanesDerivation(Prooflogger* pl, std::string* buffer);

        public:
        ~CuttingPlanesDerivation();
    };
}

#endif