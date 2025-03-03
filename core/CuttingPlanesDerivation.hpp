#include "CuttingPlanesDerivation.h"

namespace VeriPB{

std::string CuttingPlanesDerivation::toString() const{
    assert(!_write_directly_to_proof);
    return *_buffer;
}
bool CuttingPlanesDerivation::writing_directly_to_proof() const{
    return _write_directly_to_proof;
}

void CuttingPlanesDerivation::clear(){
    assert(!_write_directly_to_proof);
    _finished=true;
    _buffer->clear();
}

void CuttingPlanesDerivation::start_from_constraint(const constraintid& cxn_id){
    assert(_finished);
    _finished=true;
    if(_write_directly_to_proof){
        *(_pl->proof) << "p ";
        write_number(cxn_id,_pl->proof, false);
    }else{
        *(_buffer) += "p " + number_to_string(cxn_id);
    }
}
template <class TLit>
void CuttingPlanesDerivation::start_from_literal_axiom(const TLit& lit){
    assert(_finished);
    _finished=true;
    VeriPB::Lit l = toVeriPbLit(lit);
    if(_write_directly_to_proof){
        *(_pl->proof) << "p";
        _pl->_varMgr->write_literal(l, _pl->proof, true);
    }else{
        *(_buffer) += "p " + _pl->_varMgr->literal_to_string(l);
    }
}
void CuttingPlanesDerivation::add(const CuttingPlanesDerivation* cp_to_add){
    assert(!cp_to_add->writing_directly_to_proof());

    if(_write_directly_to_proof){
        *(_pl->proof) << " " << cp_to_add->toString() << " +";
    }else{
        *(_buffer) += " " + cp_to_add->toString() + " +";
    }
}
void CuttingPlanesDerivation::start_subderivation_from_constraint(const constraintid& cxn_id){
    if(_write_directly_to_proof){
        write_number(cxn_id, _pl->proof, true);
    }else{
        *(_buffer) += " " + number_to_string(cxn_id);
    }
}
template <class TLit>
void CuttingPlanesDerivation::start_subderivation_from_literal_axiom(const TLit& lit_axiom){
    VeriPB::Lit l = toVeriPbLit(l);

    if(_write_directly_to_proof){
        _pl->_varMgr->write_literal(l, _pl->proof, true);
    }else{
        *(_buffer) += _pl->_varMgr->literal_to_string(l);
    }
}
void CuttingPlanesDerivation::add_subderivation(){
    if(_write_directly_to_proof){
        *(_pl->proof) << " +";
    }else{
        *(_buffer) += " +";
    }
}
template <class TNumber=VeriPB::defaultmultipliertype>
void CuttingPlanesDerivation::add_constraint(const constraintid& cxn_id, const TNumber& mult){
    if(_write_directly_to_proof){
        write_number(cxn_id, _pl->proof, true);
        if(mult != 1){
            write_number(mult, _pl->proof, true);
            *(_pl->proof) << " *";
        }
        *(_pl->proof) << " +";
    }else{
        *(_buffer) += " " + number_to_string(cxn_id);
        if(mult != 1){
            *(_buffer) += number_to_string(mult) + " *";
        }
        *(_buffer) += " +";
    }
}
template <class TLit, class TNumber=VeriPB::defaultmultipliertype>
void CuttingPlanesDerivation::add_literal_axiom(const TLit& lit_axiom, const TNumber& mult){
    VeriPB::Lit l = toVeriPbLit(lit_axiom);
    if(_write_directly_to_proof){
        _pl->_varMgr->write_literal(l, _pl->proof, true);
        if(mult != 1){
            write_number(mult, _pl->proof, true);
            *(_pl->proof) << " *";
        }
        *(_pl->proof) << " +";
    }else{
        *(_buffer) += " " + _pl->_varMgr->literal_to_string(l);
        if(mult != 1){
            *(_buffer) += number_to_string(mult) + " *";
        }
        *(_buffer) += " +";
    }
}

template <class TNumber=VeriPB::defaultmultipliertype>
void CuttingPlanesDerivation::divide(const TNumber& n){
    if(_write_directly_to_proof){
        write_number(n, _pl->proof, true);
        *(_pl->proof) << " d";
    }else{
        *(_buffer) += " " + number_to_string(n) + " d";
    }
}
void CuttingPlanesDerivation::saturate(){}
template <class TNumber=VeriPB::defaultmultipliertype>
void CuttingPlanesDerivation::multiply(const TNumber& n){
    if(_write_directly_to_proof){
        write_number(n, _pl->proof, true);
        *(_pl->proof) << " *";
    }else{
        *(_buffer) += " " + number_to_string(n) + " *";
    }
}
template <class TVar>
void CuttingPlanesDerivation::weaken(const TVar& var){
    VeriPB::Var v = toVeriPbVar(var);
    if(_write_directly_to_proof){
        _pl->_varMgr->write_var_name(v, _pl->proof, true);
        *(_pl->proof) << " w";
    }else{
        *(_buffer) += " " + _pl->_varMgr->var_name(v) + " *";
    }
}

constraintid CuttingPlanesDerivation::end(bool clear){
    if(!_write_directly_to_proof){
        *(_pl->proof) << *(_buffer);
        if(clear)
            this->clear();
    }
    *(_pl->proof) << "\n";
    _finished = true;
    return ++_pl->_constraint_counter;
}

CuttingPlanesDerivation::CuttingPlanesDerivation(Prooflogger* pl, bool write_directly_to_proof) : 
    _pl(pl), _write_directly_to_proof(write_directly_to_proof) 
    {
        if(!write_directly_to_proof){
            _buffer = new std::string();
            _bufferOwned = true;
        }
    };

CuttingPlanesDerivation::CuttingPlanesDerivation(Prooflogger* pl, std::string* buffer) : 
    _buffer(buffer), _write_directly_to_proof(false), _bufferOwned(false), _pl(pl) {};

CuttingPlanesDerivation::~CuttingPlanesDerivation(){
    if(_bufferOwned)
        delete _buffer;
}

}