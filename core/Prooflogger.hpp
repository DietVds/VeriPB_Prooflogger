#include "VeriPbSolverTypes.hpp"
#include "VariableManager.hpp"
#include "Prooflogger.h"


//=================================================================================================

using namespace VeriPB;


// ------------- Cutting Planes Derivations -------------
std::string CuttingPlanesDerivation::toString() const{
    assert(!_write_directly_to_proof);
    return *_buffer;
}
bool CuttingPlanesDerivation::writing_directly_to_proof() const{
    return _write_directly_to_proof;
}

bool CuttingPlanesDerivation::isEmpty() const{
    return _finished;
}

void CuttingPlanesDerivation::setProoflogger(Prooflogger* pl){
    assert(_pl == nullptr);
    _pl = pl;
}

void CuttingPlanesDerivation::clear(){
    assert(!_write_directly_to_proof);
    _finished=true;
    _buffer->clear();
}

template <class TNumber>
void CuttingPlanesDerivation::start_from_constraint(const constraintid& cxn_id, const TNumber& mult){
    assert(_pl != nullptr);
    assert(_finished);
    _finished=false;
    if(_write_directly_to_proof){
        *(_pl->proof) << "p ";
        write_number(cxn_id,_pl->proof, false);
        if(mult != 1) write_number(mult, _pl->proof, true);
    }else{
        assert(_buffer != nullptr);
        *(_buffer) += number_to_string(cxn_id);
        if(mult != 1)
            *(_buffer) += " " + number_to_string(mult) + " *";
    }
}
template <class TLit>
void CuttingPlanesDerivation::start_from_literal_axiom(const TLit& lit){
    assert(_pl != nullptr);
    assert(_finished);
    _finished=true;
    if(_write_directly_to_proof){
        *(_pl->proof) << "p";
        _pl->_varMgr->write_literal(lit, _pl->proof, true);
    }else{
        *(_buffer) += _pl->_varMgr->literal_to_string(lit);
    }
}

template <class TNumber>
void CuttingPlanesDerivation::add(const CuttingPlanesDerivation* cp_to_add, const TNumber& mult){
    assert(_pl != nullptr);
    assert(!cp_to_add->writing_directly_to_proof());

    if(_write_directly_to_proof){
        *(_pl->proof) << " " << cp_to_add->toString();
        if(mult > 1){ 
            write_number(mult, _pl->proof, true);
            *(_pl->proof) << " *";
        }
        *(_pl->proof) << " +";
    }else{
        *(_buffer) += " " + cp_to_add->toString();
        if(mult > 1)  *(_buffer) += " " + number_to_string(mult) + " *";
        *(_buffer) += " +";
    }
}

void CuttingPlanesDerivation::start_subderivation_from_constraint(const constraintid& cxn_id){
    assert(_pl != nullptr);
    if(_write_directly_to_proof){
        write_number(cxn_id, _pl->proof, true);
    }else{
        *(_buffer) += " " + number_to_string(cxn_id);
    }
}
template <class TLit>
void CuttingPlanesDerivation::start_subderivation_from_literal_axiom(const TLit& lit_axiom){
    assert(_pl != nullptr);

    if(_write_directly_to_proof){
        _pl->_varMgr->write_literal(lit_axiom, _pl->proof, true);
    }else{
        *(_buffer) += _pl->_varMgr->literal_to_string(lit_axiom);
    }
}
void CuttingPlanesDerivation::add_subderivation(){
    assert(_pl != nullptr);
    if(_write_directly_to_proof){
        *(_pl->proof) << " +";
    }else{
        *(_buffer) += " +";
    }
}
template <class TNumber=VeriPB::defaultmultipliertype>
void CuttingPlanesDerivation::add_constraint(const constraintid& cxn_id, const TNumber& mult){
    assert(_pl != nullptr);
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
            *(_buffer) += " " + number_to_string(mult) + " *";
        }
        *(_buffer) += " +";
    }
}
template <class TLit, class TNumber=VeriPB::defaultmultipliertype>
void CuttingPlanesDerivation::add_literal_axiom(const TLit& lit_axiom, const TNumber& mult){
    assert(_pl != nullptr);
    if(_write_directly_to_proof){
        _pl->_varMgr->write_literal(lit_axiom, _pl->proof, true);
        if(mult != 1){
            write_number(mult, _pl->proof, true);
            *(_pl->proof) << " *";
        }
        *(_pl->proof) << " +";
    }else{
        *(_buffer) += " " + _pl->_varMgr->literal_to_string(lit_axiom);
        if(mult != 1){
            *(_buffer) += " " + number_to_string(mult) + " *";
        }
        *(_buffer) += " +";
    }
}

template <class TNumber=VeriPB::defaultmultipliertype>
void CuttingPlanesDerivation::divide(const TNumber& n){
    assert(_pl != nullptr);
    if(_write_directly_to_proof){
        write_number(n, _pl->proof, true);
        *(_pl->proof) << " d";
    }else{
        *(_buffer) += " " + number_to_string(n) + " d";
    }
}
void CuttingPlanesDerivation::saturate(){
    assert(_pl != nullptr);
    if(_write_directly_to_proof){
        *(_pl->proof) << " s";
    }else{
        *(_buffer) += " s";
    }
}

template <class TNumber=VeriPB::defaultmultipliertype>
void CuttingPlanesDerivation::multiply(const TNumber& n){
    assert(_pl != nullptr);
    if(_write_directly_to_proof){
        write_number(n, _pl->proof, true);
        *(_pl->proof) << " *";
    }else{
        *(_buffer) += " " + number_to_string(n) + " *";
    }
}
template <class TVar>
void CuttingPlanesDerivation::weaken(const TVar& var){
    assert(_pl != nullptr);
    if(_write_directly_to_proof){
        _pl->_varMgr->write_var_name(var, _pl->proof, true);
        *(_pl->proof) << " w";
    }else{
        *(_buffer) += " " + _pl->_varMgr->var_name(var) + " w";
    }
}

constraintid CuttingPlanesDerivation::end(bool clear){
    assert(_pl != nullptr);
    if(!_write_directly_to_proof){
        *(_pl->proof) << "p " << *(_buffer);
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
        else{
            _bufferOwned=false;
        }
    };

CuttingPlanesDerivation::CuttingPlanesDerivation(Prooflogger* pl, std::string* buffer) : 
    _buffer(buffer), _write_directly_to_proof(false), _bufferOwned(false), _pl(pl) {};

CuttingPlanesDerivation::~CuttingPlanesDerivation(){
    if(_bufferOwned)
        delete _buffer;
}

CuttingPlanesDerivation& CuttingPlanesDerivation::copyTo(CuttingPlanesDerivation& target) const{
    if(&target != this){
        target._write_directly_to_proof = _write_directly_to_proof;
        target._finished = _finished;
        target._bufferOwned = _bufferOwned;
        target._pl = _pl;

        // If buffer owned, need to make a copy of the actual string instead of copying the pointer.
        if(!_write_directly_to_proof && _bufferOwned){
            *(target._buffer) = *_buffer;
        }
        else if(!_write_directly_to_proof && !_bufferOwned)
            target._buffer = _buffer;
    }
    return target;
}

CuttingPlanesDerivation& CuttingPlanesDerivation::operator=(const CuttingPlanesDerivation& other){
    return other.copyTo(*this);
}

// ------------- Proof file -------------
void Prooflogger::set_proof_stream(std::ostream* proof){
    this->proof = proof;
    proof->rdbuf()->pubsetbuf(_write_buffer, _write_buffer_size); 
}

void Prooflogger::set_keep_original_formula_on(){
    _keep_original_formula = true;
}

void Prooflogger::set_keep_original_formula_off(){
    _keep_original_formula = false;
}

void Prooflogger::write_proof_header()
{
    *proof << "pseudo-Boolean proof version 2.0\n";
    *proof << "f\n";
}

void Prooflogger::set_n_orig_constraints(int nbconstraints){
    assert(_constraint_counter == 0);
    _constraint_counter = nbconstraints;
    _n_orig_constraints = nbconstraints;
}

bool Prooflogger::is_original_constraint(const constraintid& cxn){
    return cxn <= _n_orig_constraints;
}

constraintid Prooflogger::get_constraint_counter(){
    return _constraint_counter;
}

constraintid Prooflogger::increase_constraint_counter(){
    return ++_constraint_counter;
}

void Prooflogger::set_variable_manager(VarManager* varMgr){
    _varMgr = varMgr;
}

VarManager* Prooflogger::get_variable_manager(){
    return _varMgr;
}

void Prooflogger::end_proof(){
    flush_proof();
    if(_proofOwned)
        static_cast<std::ofstream*>(proof)->close();
}

void Prooflogger::flush_proof(){
    proof->flush();
}

// ------------- Conclusion -------------
void Prooflogger::write_conclusion_NONE(){
    *proof << "output NONE\n"
        << "conclusion NONE\n"
        << "end pseudo-Boolean proof\n";
}

void Prooflogger::write_conclusion_UNSAT(){
    *proof << "output NONE\n"
        << "conclusion UNSAT\n"
        << "end pseudo-Boolean proof\n";
}

void Prooflogger::write_conclusion_SAT(){
    *proof << "output NONE\n"
        << "conclusion SAT\n"
        << "end pseudo-Boolean proof\n";
}

void Prooflogger::write_fail(){
    *proof << "fail\n";
}

// ------------- Solution logging -------------

bool Prooflogger::logged_solution(){
    return _found_solution;
}

template <class TModel>
constraintid Prooflogger::log_solution(const TModel& model, const bool derive_excluding_constraint, const bool only_print_original_variables, const bool log_as_comment){
    assert(!(derive_excluding_constraint && log_as_comment));
    _log_solution(model, (derive_excluding_constraint ? "solx" : "sol"), only_print_original_variables, log_as_comment);
    if(derive_excluding_constraint && !log_as_comment)
        return ++_constraint_counter;
    else
        return undefcxn;
}   

// ------------- Cutting Planes derivations -------------
constraintid Prooflogger::copy_constraint(const constraintid cxn){
    *proof << "p " << cxn << "\n";
    return ++_constraint_counter;
}

CuttingPlanesDerivation Prooflogger::new_cuttingplanes_derivation(bool write_directly_to_proof, bool use_internal_buffer){
    if(write_directly_to_proof){
        return CuttingPlanesDerivation(this, true);
    }
    else if(use_internal_buffer) {
        return CuttingPlanesDerivation(this, &_cuttingplanes_buffer);
    }
    else{
        return CuttingPlanesDerivation(this);
    }
}

// ------------- Comments -------------
void Prooflogger::set_comments(bool write_comments){
    _comments = write_comments;
}

void Prooflogger::write_comment(const char *comment)
{
    if(_comments)
        *proof << "* " << comment << "\n";
    #ifdef FLUSHPROOFCOMMENTS
        proof->flush(); // Can be uncommented for debugging reasons
    #endif
}

void Prooflogger::write_comment(const std::string &comment)
{
    if(_comments)
        *proof << "* " << comment << "\n";
    #ifdef FLUSHPROOFCOMMENTS
        proof->flush(); // Can be uncommented for debugging reasons
    #endif
}

#ifndef NOVPBTIMETRACKING
section_id Prooflogger::create_new_timed_checking_section(std::string& name){
    _timed_checking_sections.push_back(name);
    return _timed_checking_sections.size()-1;
}
section_id Prooflogger::create_new_timed_checking_section(const char* name){
    _timed_checking_sections.push_back(name);
    return _timed_checking_sections.size()-1;
}
void Prooflogger::start_timed_checking_section(section_id id){
    *proof << "start_time " << _timed_checking_sections[id] << "\n";
}
void Prooflogger::end_timed_checking_section(section_id id){
    *proof << "end_time " << _timed_checking_sections[id] << "\n";
}

section_id Prooflogger::create_new_timed_solving_section(std::string& name){
    _timed_solving_sections.push_back(std::pair<std::string, uint64_t>{name,0});
    _timed_solving_section_start.emplace_back();
    return _timed_solving_sections.size()-1;
}
section_id Prooflogger::create_new_timed_solving_section(const char* name){
    _timed_solving_sections.push_back(std::pair<std::string, uint64_t>{name,0});
    _timed_solving_section_start.emplace_back();
    return _timed_solving_sections.size()-1;
}
void Prooflogger::start_timed_solving_section(section_id id){
    _timed_solving_section_start[id] = high_resolution_clock::now();
}
void Prooflogger::end_timed_solving_section(section_id id){
    time_point<high_resolution_clock> end = high_resolution_clock::now();
    _timed_solving_sections[id].second += duration_cast<microseconds>( end - _timed_solving_section_start[id]).count();
}

std::vector<std::pair<std::string, uint64_t>> Prooflogger::get_solving_timers(){
    return _timed_solving_sections;
}
#endif
// ------------- Rules for checking constraints -------------
template <class TConstraint>
void Prooflogger::equals_rule(const TConstraint& cxn, const constraintid cxn_id)
{
    *proof << "e ";
    write_constraint(cxn);
    *proof << "; ";
    write_number(cxn_id, proof);
    *proof << "\n";
}

template <class TConstraint>
void Prooflogger::check_last_constraint(const TConstraint& cxn)
{
    equals_rule(cxn, -1);
}

template <class TConstraint>
void Prooflogger::check_constraint_exists(const TConstraint& cxn)
{
    *proof << "e ";
    write_constraint(cxn);
    *proof << ";\n";
}

template <class TConstraint>
constraintid Prooflogger::check_constraint_exists_and_add(const TConstraint& cxn){
    *proof << "ea ";
    write_constraint(cxn);
    *proof << ";\n";
    return ++_constraint_counter;
}

template <class TConstraint>
void Prooflogger::check_implied(const TConstraint& cxn, constraintid cxn_id){
    *proof << "i ";
    write_constraint(cxn);
    if(cxn_id == undefcxn)
        *proof << ";\n";
    else{
        *proof << ";";
        write_number(cxn_id, proof);
        *proof << "\n";
    }
        
}

template <class TConstraint>
void Prooflogger::check_implied(const TConstraint& cxn){
    check_implied(cxn, undefcxn);
}

// ------------- Rules for adding syntactically implied constraints -------------
template <class TConstraint>
constraintid Prooflogger::derive_if_implied(const TConstraint& cxn){
    return derive_if_implied(cxn, undefcxn);
}

template <class TConstraint>
constraintid Prooflogger::derive_if_implied(const TConstraint& cxn, const constraintid& cxn_id){
    *proof << "ia";
    write_constraint(cxn);
    *proof << ";";
    if(cxn_id != undefcxn)
        write_number(cxn_id,proof);
    *proof << "\n";
    return ++_constraint_counter;
}

// ------------- Unchecked Assumptions -------------

template <class TConstraint>
constraintid Prooflogger::unchecked_assumption(const TConstraint& cxn)
{
    *proof << "a";
    write_constraint(cxn);
    *proof << " ;\n";
    return ++_constraint_counter;
}

// ------------- Reverse Unit Propagation -------------

template <class TConstraint>
constraintid Prooflogger::rup(const TConstraint& cxn, bool core_constraint){
    *proof << "rup";
    write_constraint(cxn);
    *proof << ";\n";
    if(core_constraint)
        move_to_coreset_by_id(-1);
    return ++_constraint_counter;
}

template <class TConstraint, class TSeqHints>
constraintid Prooflogger::rup(const TConstraint& cxn, const TSeqHints& hints, const bool core_constraint){
    *proof << "rup";
    write_constraint(cxn);
    *proof << ";";
    write_hints(hints);
    *proof << "\n";
    return ++_constraint_counter;
}


template <class TConstraint>
constraintid Prooflogger::rup_clause(const TConstraint& clause){
    *proof << "rup";
    write_clause(clause)
    *proof << ";\n";
    return ++_constraint_counter;
}

template <class TConstraint, class TSeqHints>
constraintid Prooflogger::rup_clause(const TConstraint& clause, const TSeqHints& hints){
    *proof << "rup";
    write_clause(clause);
    *proof << ";";
    write_hints(hints);
    *proof << "\n";
    return ++_constraint_counter;
}


template <class TLit>
constraintid Prooflogger::rup_unit_clause(const TLit& lit, bool core_constraint){
    *proof << "rup";
    write_weighted_literal(lit);
    *proof << " >= 1;\n";
    if(core_constraint)
        move_to_coreset_by_id(-1);
    return ++_constraint_counter;
}

template <class TLit, class TSeqHints>
constraintid Prooflogger::rup_unit_clause(const TLit& lit, const TSeqHints& hints, bool core_constraint){
    *proof << "rup";
    write_weighted_literal(lit);
    *proof << " >= 1;";
    write_hints(hints);
    *proof << "\n";
    if(core_constraint)
        move_to_coreset_by_id(-1);
    return ++_constraint_counter;
}


template <class TLit>
constraintid Prooflogger::rup_binary_clause(const TLit& lit1, const TLit& lit2, bool core_constraint){
    *proof << "rup";
    write_weighted_literal(lit1);
    write_weighted_literal(lit2);
    *proof << " >= 1;\n";
    if(core_constraint)
        move_to_coreset_by_id(-1);
    return ++_constraint_counter;
}

template <class TLit, class TSeqHints>
constraintid Prooflogger::rup_binary_clause(const TLit& lit1, const TLit& lit2, const TSeqHints& hints,  bool core_constraint){
    *proof << "rup";
    write_weighted_literal(lit1);
    write_weighted_literal(lit2);
    *proof << " >= 1;";
    if(core_constraint)
        move_to_coreset_by_id(-1);
    write_hints(hints);
    *proof << "\n";
    return ++_constraint_counter;
}


template <class TLit> 
constraintid Prooflogger::rup_ternary_clause(const TLit& lit1, const TLit& lit2, const TLit& lit3, bool core_constraint){
    *proof << "rup";
    write_weighted_literal(lit1);
    write_weighted_literal(lit2);
    write_weighted_literal(lit3);
    *proof << " >= 1;\n";
    if(core_constraint)
        move_to_coreset_by_id(-1);
    return ++_constraint_counter;
}

template <class TLit, class TSeqHints> 
constraintid Prooflogger::rup_ternary_clause(const TLit& lit1, const TLit& lit2, const TLit& lit3, const TSeqHints& hints, bool core_constraint){
    *proof << "rup";
    write_weighted_literal(lit1);
    write_weighted_literal(lit2);
    write_weighted_literal(lit3);
    *proof << " >= 1;";
    if(core_constraint)
        move_to_coreset_by_id(-1);
    write_hints(hints);
    *proof << "\n";
    return ++_constraint_counter;
}

constraintid Prooflogger::rup_empty_clause(){
    *proof << "rup >= 1;\n";
    return ++_constraint_counter;
}

// ------------- Redundance Based Strenghtening -------------
void Prooflogger::strenghten_to_core_on(){
    *proof << "strengthening_to_core on\n"; 
}

void Prooflogger::strenghten_to_core_off(){
    *proof << "strengthening_to_core off\n"; 
}

substitution Prooflogger::get_new_substitution(){
    substitution s;
    return s;
}

template <class TVar>
void Prooflogger::add_boolean_assignment(substitution &s, const TVar& var, const bool value){
    s.second.push_back(std::make_pair(toVeriPbVar(var), value));
}

template <class TVar, class TLit>
void Prooflogger::add_literal_assignment(substitution &s, const TVar& var, const TLit& value){
    s.first.push_back(std::make_pair(toVeriPbVar(var), toVeriPbLit(value)));
}

void Prooflogger::add_substitution(substitution &sub, const substitution &sub_to_add){
    sub.first.reserve(sub.first.size() + sub_to_add.first.size());
    for(auto lit_ass : sub_to_add.first)
        add_literal_assignment(sub, lit_ass.first, lit_ass.second);
    
    sub.second.reserve(sub.second.size() + sub_to_add.second.size());
    for(auto bool_ass : sub_to_add.second)
        add_boolean_assignment(sub, bool_ass.first, bool_ass.second);
}

void Prooflogger::write_substitution(const substitution &witness)
{
    VeriPB::Var varfrom, varto;
    VeriPB::Lit lit_to;
    bool boolto;

    for(auto lit_ass : witness.first){
        varfrom = lit_ass.first;
        lit_to = lit_ass.second;
        _varMgr->write_var_to_lit(varfrom, lit_to, proof);
    }
    for(auto bool_ass : witness.second){
        varfrom = bool_ass.first;
        boolto = bool_ass.second;
        _varMgr->write_var_to_bool(varfrom, boolto, proof);
    }   
}

template <class TVar>
bool Prooflogger::has_boolean_assignment(const substitution &s, const TVar& var){
    for(auto ass : s.second){
        if(ass.first == toVeriPbVar(var))
            return true;
    }
    return false;
}

template <class TVar>
bool Prooflogger::has_literal_assignment(const substitution &s, const TVar& var){
    for(auto ass : s.first){
        if(ass.first == toVeriPbVar(var))
            return true;
    }
    return false;
}

template <class TVar>
bool Prooflogger::get_boolean_assignment(substitution &s, const TVar& var){
    for(auto ass : s.second){
        if(ass.first == toVeriPbVar(var))
            return ass.second;
    }
    std::cout << "ERROR: Proof logging library: Could not find boolean assignment for variable " << _varMgr->var_name(var) << std::endl;
    assert(false);
    return false;
}

template <class TVar>
VeriPB::Lit Prooflogger::get_literal_assignment(substitution &s, const TVar& var){
    for(auto ass : s.first){
        if(ass.first == toVeriPbVar(var))
            return ass.second;
    }
    std::cout << "ERROR: Proof logging library: Could not find literal assignment for variable " << _varMgr->var_name(var) << std::endl;
    assert(false);
    return VeriPB::lit_undef;
}

size_t Prooflogger::get_substitution_size(const substitution &s){
    return s.first.size() + s.second.size();
}

template <class TConstraint>
constraintid Prooflogger::redundance_based_strengthening(const TConstraint& cxn, const substitution& witness){
    *proof << "red";
    write_constraint(cxn);
    *proof << "; ";
    write_substitution(witness);
    *proof << "\n";
    return ++_constraint_counter;
}

template <class TLit>
constraintid Prooflogger::redundance_based_strengthening_unit_clause(const TLit& lit){
    *proof << "red";
    write_weighted_literal(lit);
    *proof << " >= 1; "; 
    _varMgr->write_var_to_bool(variable(lit), !is_negated(lit), proof);
    *proof << "\n";
    return ++_constraint_counter;
}

template <typename TConstraint>
constraintid Prooflogger::redundance_based_strengthening(const TConstraint& cxn, const substitution& witness, const std::vector<subproof>& subproofs){
    *proof << "red";
    write_constraint(cxn);
    *proof << "; ";
    write_substitution(witness);

    if(subproofs.size() > 0){
        _constraint_counter++; // constraint not C
        *proof << "; begin \n";
    
        for(int i = 0; i < subproofs.size(); i++){
            subproof p = subproofs[i];
            _constraint_counter++; // constraint C\w
            *proof << "\tproofgoal " << p.proofgoal << "\n";
            for(int i = 0; i < p.derivations.size(); i++){
                *proof << "\t\tp " << p.derivations[i] << "\n";
                _constraint_counter++;
            }
            // *proof << "\t\t c -1\n";
            *proof << "\tend -1\n";
        }
        *proof << "end";
    }
    *proof << "\n";

    return ++_constraint_counter;
}

template <class TConstraint>
constraintid Prooflogger::start_redundance_based_strengthening_with_subproofs(const TConstraint& cxn, const substitution& witness){
    *proof << "red";
    write_constraint(cxn);
    *proof << "; ";
    write_substitution(witness);
    *proof << "; begin \n";
    return ++_constraint_counter; // constraint not C
}

constraintid Prooflogger::start_new_subproof(const std::string& proofgoal){
    *proof << "\tproofgoal " << proofgoal << "\n";
    return ++_constraint_counter; // constraint C\w
}

void Prooflogger::end_subproof(){
    *proof << "\tend -1\n";
}

constraintid Prooflogger::end_redundance_based_strengthening_with_subproofs(){
    *proof << "end\n";
    return ++_constraint_counter;
}

// ------------- Reification -------------

template <class TLit, class TConstraint>
constraintid Prooflogger::reification_literal_right_implication(const TLit& lit, const TConstraint& cxn, const bool store_reified_constraint){
    int i;

    if(_comments){
        // TODO-Dieter: Make comment a string in the PL library to not always have to take the memory again.
        std::string comment = _varMgr->literal_to_string(lit) + " -> " ;
        for(i = 0; i < size(cxn); i++)
            comment += number_to_string(coefficient(cxn,i)) + " " + _varMgr->literal_to_string(literal(cxn,i)) + " ";
        comment += to_string(comparison(cxn)) + " " + number_to_string(rhs(cxn));
        write_comment(comment);
    }

    *proof << "red";
    if(comparison(cxn) == GEQ){
        write_weighted_literal(neg(lit), rhs(cxn)-min_val_lhs(cxn));
    }
    else if(comparison(cxn) == LEQ){
        *proof << " -";
        write_weighted_literal(neg(lit), max_val_lhs(cxn)-rhs(cxn), false); // Literal should have negative coefficient, even when coefficients of constraint itself are unsigned types.
    }
    write_constraint(cxn);
    *proof << "; ";
    
    VeriPB::Var var = toVeriPbVar(variable(lit));
    substitution witness = get_new_substitution();
    add_boolean_assignment(witness, var, is_negated(lit)); // Set lit to false makes reification constraint true. So that means that if lit is negated, we need to set the variable true.
    write_substitution(witness);
    *proof << "\n";
    ++_constraint_counter;
    if(store_reified_constraint)
        store_reified_constraint_right_implication(var, _constraint_counter);
    return _constraint_counter;
}     

template <class TLit, class TConstraint>
constraintid Prooflogger::reification_literal_left_implication(const TLit& lit, const TConstraint& cxn, const bool store_reified_constraint){
    int i;

    if(_comments){
        std::string comment = _varMgr->literal_to_string(lit) + " <- " ;
        for(i = 0; i < size(cxn); i++)
            comment += number_to_string(coefficient(cxn,i)) + " " + _varMgr->literal_to_string(literal(cxn,i)) + " ";
        comment += to_string(comparison(cxn)) + " " + number_to_string(rhs(cxn));
        write_comment(comment);
    }

    *proof << "red";
    if(comparison(cxn) == Comparison::GEQ){
        *proof << " -";
       write_weighted_literal(lit, max_val_lhs(cxn) - rhs(cxn) + 1, false); // Literal should have negative coefficient, even when coefficients of constraint itself are unsigned types.
    }
    else if(comparison(cxn) == Comparison::LEQ){
        write_weighted_literal(lit, rhs(cxn) + 1 - min_val_lhs(cxn));
    }

    for(int i = 0; i < size(cxn); i++)
        write_weighted_literal(literal(cxn, i), coefficient(cxn,i));

    if(comparison(cxn) == Comparison::GEQ){
        *proof << ' ' << to_string(Comparison::LEQ);
        write_number(rhs(cxn) - 1, proof);
    }
    else if(comparison(cxn) == Comparison::LEQ){
        *proof <<  ' ' << to_string(Comparison::GEQ);
        write_number(rhs(cxn) + 1, proof);
    }
    *proof << "; ";

    VeriPB::Var var = toVeriPbVar(variable(lit));
    substitution witness = get_new_substitution();
    add_boolean_assignment(witness, var, !is_negated(lit)); // Set lit to true makes reification constraint true. So that means that if lit is not negated, we need to set the variable true.
    write_substitution(witness);
    *proof << "\n";
    ++_constraint_counter;
    if(store_reified_constraint)
        store_reified_constraint_left_implication(var, _constraint_counter);
    return _constraint_counter;
}

template <class TVar>
void Prooflogger::delete_reified_constraint_left_implication(const TVar& var){
    VeriPB::Var _var = toVeriPbVar(var);

    std::vector<constraintid>* storage = _var.only_known_in_proof ? &_reifiedConstraintLeftImplOnlyProofVars : &_reifiedConstraintLeftImpl;

    if(_var.v <= storage->size()){
        delete_constraint_by_id((*storage)[_var.v]);
        (*storage)[_var.v] = undefcxn;
    }
}

template <class TVar>
void Prooflogger::delete_reified_constraint_right_implication(const TVar& var){
    VeriPB::Var _var = toVeriPbVar(var);

    std::vector<constraintid>* storage = _var.only_known_in_proof ? &_reifiedConstraintRightImplOnlyProofVars : &_reifiedConstraintRightImpl;

    if(_var.v <= storage->size()){
        delete_constraint_by_id((*storage)[_var.v]);
        (*storage)[_var.v] = undefcxn;
    }
}

template <class TVar>
constraintid Prooflogger::get_reified_constraint_left_implication(const TVar& var){
    VeriPB::Var _var = toVeriPbVar(var);
    std::vector<constraintid>* storage = _var.only_known_in_proof ? &_reifiedConstraintLeftImplOnlyProofVars : &_reifiedConstraintLeftImpl;
    constraintid cxn = undefcxn;
    if(_var.v < storage->size())
        cxn = (*storage)[_var.v];

    return cxn;
}

template <class TVar>
constraintid Prooflogger::get_reified_constraint_right_implication(const TVar& var){
    VeriPB::Var _var = toVeriPbVar(var);
    std::vector<constraintid>* storage = _var.only_known_in_proof ? &_reifiedConstraintRightImplOnlyProofVars : &_reifiedConstraintRightImpl;
    constraintid cxn = undefcxn;
    if(_var.v < storage->size())
        cxn = (*storage)[_var.v];

    return cxn;
}

template <class TVar>
void Prooflogger::store_reified_constraint_left_implication(const TVar& var, const constraintid& cxnId){
    VeriPB::Var _var = toVeriPbVar(var);
    std::vector<constraintid>* storage = _var.only_known_in_proof ? &_reifiedConstraintLeftImplOnlyProofVars : &_reifiedConstraintLeftImpl;
    
    // Increase storage if necessary.
    if(_var.v >= storage->size() && INIT_NAMESTORAGE > _var.v ){
        storage->resize(INIT_NAMESTORAGE, undefcxn);
    }
    else if(_var.v >= storage->size()) {
        storage->resize(2 * _var.v, undefcxn);
    }
    (*storage)[_var.v] = cxnId;
}

template <class TVar>
void Prooflogger::store_reified_constraint_right_implication(const TVar& var, const constraintid& cxnId){
    VeriPB::Var _var = toVeriPbVar(var);
    std::vector<constraintid>* storage = _var.only_known_in_proof ? &_reifiedConstraintRightImplOnlyProofVars : &_reifiedConstraintRightImpl;
    
    // Increase storage if necessary.
    if(_var.v >= storage->size() && INIT_NAMESTORAGE > _var.v ){
        storage->resize(INIT_NAMESTORAGE, undefcxn);
    }
    else if(_var.v >= storage->size()) {
        storage->resize(2 * _var.v, undefcxn);
    }
    (*storage)[_var.v] = cxnId;
}

/**
 * Remove the right reification constraint from the reification constraint store without deleting it in the proof. 
 * Only needed for not maintaining a constraint id in memory that will not be used in the proof anymore.
*/
template <class TVar>
void Prooflogger::remove_reified_constraint_right_implication_from_constraintstore(const TVar& var){
    VeriPB::Var _var = toVeriPbVar(var);

    std::vector<constraintid>* storage = _var.only_known_in_proof ? &_reifiedConstraintRightImplOnlyProofVars : &_reifiedConstraintRightImpl;

    if(_var.v <= storage->size()){
        (*storage)[_var.v] = undefcxn;
    }
}

/**
 * Remove the left reification constraint from the reification constraint store without deleting it in the proof. 
 * Only needed for not maintaining a constraint id in memory that will not be used in the proof anymore.
*/
template <class TVar>
void Prooflogger::remove_reified_constraint_left_implication_from_constraintstore(const TVar& var){
    VeriPB::Var _var = toVeriPbVar(var);

    std::vector<constraintid>* storage = _var.only_known_in_proof ? &_reifiedConstraintLeftImplOnlyProofVars : &_reifiedConstraintLeftImpl;

    if(_var.v <= storage->size()){
        delete_constraint_by_id((*storage)[_var.v]);
        (*storage)[_var.v] = undefcxn;
    }
}


// ------------- Proof by contradiction -------------
template <class TConstraint>
constraintid Prooflogger::start_proof_by_contradiction(const TConstraint& cxn){
    VeriPB::substitution w = get_new_substitution();
    start_redundance_based_strengthening_with_subproofs(cxn, w);
    return start_new_subproof("#1");
}

constraintid Prooflogger::end_proof_by_contradiction(){
    end_subproof();
    return end_redundance_based_strengthening_with_subproofs();
}

// ------------- Proof by case splitting -------------
template <class TConstraint>
constraintid Prooflogger::prove_by_casesplitting(const TConstraint& cxn, const constraintid& case1, const constraintid& case2){
    constraintid cxnNeg = start_proof_by_contradiction(cxn);
    _cpder->start_from_constraint(cxnNeg);
    _cpder->add_constraint(case1);
    _cpder->start_subderivation_from_constraint(cxnNeg);
    _cpder->add_constraint(case2);
    _cpder->add_subderivation();
    _cpder->end();
    return end_proof_by_contradiction();
}

// ------------- Deleting & Overwriting Constraints -------------
void Prooflogger::delete_constraint_by_id(const constraintid cxn_id, bool overrule_keeporiginalformula){
    *proof << "del id";
    if(!(_keep_original_formula && is_original_constraint(cxn_id)) || overrule_keeporiginalformula)
        write_number(cxn_id, proof, true);
    *proof << "\n";
}

void Prooflogger::delete_constraint_by_id(const std::vector<constraintid> &constraint_ids, bool overrule_keeporiginalformula)
{
    *proof << "del id";
    for (constraintid id : constraint_ids)
    {
        if(!(_keep_original_formula && is_original_constraint(id)) || overrule_keeporiginalformula)
            *proof << " " << id;
    }
    *proof << "\n";
}

template <class TConstraint>
void Prooflogger::delete_constraint(const TConstraint& cxn, const bool overrule_keeporiginalformula)
{
    assert(!_keep_original_formula || overrule_keeporiginalformula);

    *proof << "del spec";
    write_constraint(cxn);
    *proof << ";\n";
}

template <class TConstraint>
void Prooflogger::delete_constraint(const TConstraint& cxn, const substitution& witness, bool overrule_keeporiginalformula){
    assert(!_keep_original_formula || overrule_keeporiginalformula);
    write_constraint(cxn);
    *proof << "; ";
    write_substitution(witness);
    *proof << "\n";
}

template <class TConstraint>
void Prooflogger::delete_clause(const TConstraint& cxn, bool overrule_keeporiginalformula){
    assert(!_keep_original_formula || overrule_keeporiginalformula);
    *proof << "del spec";
    write_clause(cxn);
    *proof << ";\n";
}

template <class TConstraint>
constraintid Prooflogger::overwrite_constraint(const constraintid& orig_cxn_id, const TConstraint& new_cxn, bool origclause_in_coreset){
    constraintid new_cxn_id = rup(new_cxn, origclause_in_coreset);
    delete_constraint_by_id(orig_cxn_id);
    return new_cxn_id;
}

template <class TConstraint>
constraintid Prooflogger::overwrite_constraint(const TConstraint& orig_cxn, const TConstraint& new_cxn, bool origclause_in_coreset){
    constraintid new_cxn_id = rup(new_cxn, origclause_in_coreset);
    delete_constraint(orig_cxn);
    return new_cxn_id;
}

void Prooflogger::move_to_coreset_by_id(const constraintid& cxn_id, bool overrule_keeporiginalformula){
    if(!_keep_original_formula || overrule_keeporiginalformula){
        *proof << "core id ";
        write_number(cxn_id,proof);
        *proof << "\n";
    }

}

template <class TConstraint>
void Prooflogger::move_to_coreset(const TConstraint& cxn, bool overrule_keeporiginalformula){
    *proof << "core id ";
    write_constraint(cxn);
    *proof << "\n";
}

template <class TVar>
void Prooflogger::save_propagation_constraint(const TVar& var, const constraintid cxn){
    // After that, update storage with new propagation.
    // Increase storage if necessary.
    VeriPB::Var _var = toVeriPbVar(var);
    std::vector<constraintid>* storage = _var.only_known_in_proof ? &_storage_var_onlyknowninproof_to_cxn : &_storage_var_to_cxn;
    
    // Increase storage if necessary.
    size_t n_orig_vars = _varMgr->get_number_original_vars();
    if(_var.v >= storage->size() && n_orig_vars > _var.v )
        storage->resize(2 * n_orig_vars, undefcxn);
    else if(_var.v >= storage->size())
        storage->resize(2 * _var.v, undefcxn);

    (*storage)[_var.v] = cxn;
}

template <class TVar>
constraintid Prooflogger::get_propagation_constraint(const TVar& var){
    VeriPB::Var _var = toVeriPbVar(var);
    std::vector<constraintid>* storage = _var.only_known_in_proof ? &_storage_var_onlyknowninproof_to_cxn : &_storage_var_to_cxn;

    if(_var.v >= storage->size())
        return undefcxn;
    else    
        return (*storage)[_var.v];
}
        
// ------------- Deriving a Binary Encoding for a Linear Term -------------
template <typename TLinTerm, typename TCoeff, typename TConst>
Prooflogger::BinaryEncodingForLinearTerm<TCoeff, TConst> Prooflogger::create_binary_encoding(const TLinTerm& T, const constraintid& AMO){
    assert(AMO != undefcxn);

    BinaryEncodingForLinearTerm<TCoeff, TConst> res;
    LinTermBoolVars<VeriPB::Lit, TCoeff, TConst>& Tres = res.T;
    Tres.add_constant(get_constant(T));


    TCoeff max_coeff = 0;
    TCoeff c;
    for(int i = 0; i < size(T); i++){
        c = coefficient(T,i);
        if(c > max_coeff) 
            max_coeff = c;
    } 

    VeriPB::Lit b_i; 
    VeriPB::Constraint<VeriPB::Lit, TCoeff, TCoeff> C;
    constraintid reif_left, reif_right;
    constraintid geq;
    CuttingPlanesDerivation CP = new_cuttingplanes_derivation(true), 
                            CP_leq = new_cuttingplanes_derivation() , 
                            CP_geq = new_cuttingplanes_derivation();


    for(TCoeff bucket = 1; bucket <= max_coeff; bucket <<= 1){
        write_comment("Introduce variable for bucket " + VeriPB::number_to_string(bucket));
        C.clear(true, 1);

        for(int i = 0; i < size(T); i++){
            if(coefficient(T, i) & bucket) 
                C.add_literal(toVeriPbLit(literal(T, i)));
        }

        if(size(C) == 0) 
            continue;

        if(size(C) == 1){
            Tres.add_literal(literal(C,0), bucket);
            continue;
        }
        

        b_i = VeriPB::create_literal(_varMgr->new_variable_only_in_proof(), false);
        reif_left = reification_literal_left_implication(b_i, C);
        reif_right = reification_literal_right_implication(b_i, C);

        write_comment("Derive GEQ for bucket " + VeriPB::number_to_string(bucket));

        C.clear();
        C.add_literal(b_i);

        size_t n_bucket = 0;
        for(int i = 0; i < size(T); i++){
            if(coefficient(T, i) & bucket){ 
                C.add_literal(neg(literal(T, i)));
                n_bucket++;
            }
        }

        C.add_RHS(n_bucket);

        VeriPB::constraintid neg_cxn = start_proof_by_contradiction(C);
        CP.start_from_constraint(neg_cxn);
        CP.add_constraint(AMO);
        CP.multiply(size(T));
        CP.add_constraint(reif_left);
        CP.add_constraint(neg_cxn);
        CP.weaken(variable(b_i));
        CP.end();
        geq = end_proof_by_contradiction();

        if(CP_geq.isEmpty())
            CP_geq.start_from_constraint(geq, bucket);
        else
            CP_geq.add_constraint(geq, bucket);

        if(CP_leq.isEmpty())
            CP_leq.start_from_constraint(reif_right, bucket);
        else
            CP_leq.add_constraint(reif_right, bucket);

        Tres.add_literal(b_i, bucket);
    }

    write_comment("Derivation of BinEnc >= T");
    if(!CP_geq.isEmpty())
        res.bin_geq_input = CP_geq.end();


    write_comment("Derivation of BinEnc =< T");
    if(!CP_leq.isEmpty())
        res.bin_leq_input = CP_leq.end();

    return res;
}

Prooflogger::BinaryAdderResult Prooflogger::half_adder(const VeriPB::Lit& l1, const VeriPB::Lit& l2){
    return full_adder(l1, l2, VeriPB::lit_undef);
}


Prooflogger::BinaryAdderResult Prooflogger::full_adder(const VeriPB::Lit& l1, const VeriPB::Lit& l2, const VeriPB::Lit& l3){
    write_comment("full_adder");
    Prooflogger::BinaryAdderResult res;
    res.sum = create_literal(_varMgr->new_variable_only_in_proof(), false);
    res.carry = create_literal(_varMgr->new_variable_only_in_proof(), false);

    Constraint<VeriPB::Lit, int8_t, int8_t> C;
    assert(l1 != lit_undef); assert(l2 != lit_undef);
    C.add_literal(l1); 
    C.add_literal(l2);
    if(l3 != lit_undef) 
        C.add_literal(l3); 
    C.add_RHS(2);
    res.reif_left_carry = reification_literal_left_implication(res.carry, C);
    res.reif_right_carry = reification_literal_right_implication(res.carry, C);

    C.add_literal(res.carry, -2); C.add_RHS(-1);
    res.reif_left_sum = reification_literal_left_implication(res.sum, C);
    res.reif_right_sum = reification_literal_right_implication(res.sum, C);

    CuttingPlanesDerivation CP = new_cuttingplanes_derivation(true);
    CP.start_from_constraint(res.reif_left_sum);
    CP.add_constraint(res.reif_left_carry, 2);
    CP.divide(3);
    res.bin_geq_input = CP.end();

    CP.start_from_constraint(res.reif_right_sum);
    CP.add_constraint(res.reif_right_carry, 2);
    CP.divide(3);
    res.bin_leq_input = CP.end();
    write_comment("full_adder - end");

    return res;
}

template <typename TLinTerm, typename TCoeff, typename TConst>
Prooflogger::BinaryEncodingForLinearTerm<TCoeff, TConst> Prooflogger::create_binary_addition(const TLinTerm& T1, const TLinTerm& T2){
    write_comment("Start creating binary addition");
    BinaryEncodingForLinearTerm<TCoeff, TConst> res;
    LinTermBoolVars<VeriPB::Lit, TCoeff, TConst>& Tres = res.T;

    CuttingPlanesDerivation CP_geq = new_cuttingplanes_derivation(),
                            CP_leq = new_cuttingplanes_derivation();


    TCoeff coeff_carry = 0;
    VeriPB::Lit carry = lit_undef;
    BinaryAdderResult binadd_res;
    auto process_binary_addition_result = [&CP_geq, &CP_leq, &Tres, &carry, &coeff_carry, &binadd_res] (TCoeff coeff_binary_addition) {
        Tres.add_literal(binadd_res.sum, coeff_binary_addition);
        if(CP_geq.isEmpty())
            CP_geq.start_from_constraint(binadd_res.bin_geq_input, coeff_binary_addition);
        else
            CP_geq.add_constraint(binadd_res.bin_geq_input, coeff_binary_addition);
        if(CP_leq.isEmpty())
            CP_leq.start_from_constraint(binadd_res.bin_leq_input, coeff_binary_addition);
        else
            CP_leq.add_constraint(binadd_res.bin_leq_input, coeff_binary_addition);
        carry = binadd_res.carry;
        coeff_carry = coeff_binary_addition << 1;
    };
    auto add_carry_if_necessary = [&carry, &coeff_carry, &Tres](){
        if(coeff_carry != 0){
            Tres.add_literal(carry, coeff_carry);
            coeff_carry = 0; carry = lit_undef;
        }
    };


    size_t i=0, j=0;

    while(i < size(T1) || j < size(T2)){
        assert(i == 0 || i == size(T1) || coefficient(T1, i-1) < coefficient(T1, i));
        assert(j == 0 || j == size(T2) || coefficient(T2, j-1) < coefficient(T2, j));

        if(i < size(T1) && (j >= size(T2) || coefficient(T1, i) < coefficient(T2,j))){ // process literal in T1
            if(coeff_carry < coefficient(T1,i)){ // Coefficient of previous carry literal is smaller than the coefficient of current literal handled, and so both need to be added.
                add_carry_if_necessary();
                Tres.add_literal(literal(T1, i), coefficient(T1,i));
            }
            else{ // Create half adder 
                assert(coeff_carry == coefficient(T1,i));
                binadd_res = half_adder(literal(T1, i), carry);
                process_binary_addition_result(coeff_carry);
            }
            i++;
        }
        else if(j < size(T2) && (i >= size(T1) || coefficient(T1,i) > coefficient(T2, j))){ // process literal in T2 
            if(coeff_carry < coefficient(T2,j)){ // Coefficient of previous carry literal is smaller than the coefficient of current literal handled, and so both need to be added.
                add_carry_if_necessary();
                Tres.add_literal(literal(T2, j), coefficient(T2,j));
            }
            else{ // Create half adder 
                assert(coeff_carry == coefficient(T2,j));
                binadd_res = half_adder(literal(T2, j), carry);
                process_binary_addition_result(coeff_carry);
            }
            j++;
        }
        else{ // process both literals
            assert(coefficient(T1,i) == coefficient(T2,j));
            if(coeff_carry < coefficient(T1,i)){
                add_carry_if_necessary();
                binadd_res = half_adder(literal(T1,i), literal(T2,j) );
                process_binary_addition_result(coefficient(T1,i));
            }
            else{
                assert(coeff_carry = coefficient(T1,i));
                binadd_res = full_adder(literal(T1,i), literal(T2,j), carry);
                process_binary_addition_result(coeff_carry);
            }
            i++; j++;
        }
    }
    add_carry_if_necessary();
    
    write_comment("Derive binenc_geq_term");
    res.bin_geq_input = CP_geq.end();
    write_comment("Derive binenc_leq_term");
    res.bin_leq_input = CP_leq.end();

    return res;
}
// -----------------------------------------------------




#ifdef LBOOLS
template <class TSeqLBool>
constraintid Prooflogger::log_solution_lbools(TSeqLBool &model, wght objective_value)
{
    if(_comments)
        write_comment("Model improvement update with objective value = " + number_to_string(objective_value));

    VeriPB::Var var;
    VeriPB::Lit lit;

    *proof << "soli";
    for (int i = 0; i < model.size(); i++){
        var = toVeriPbVar(i);
        lit = create_literal(var, !toBool(model[i]));

        if(!is_aux_var(var))
            write_literal(lit);
    }
    *proof << "\n";

    // Veripb automatically adds an improvement constraint so counter needs to be incremented
    model_improvement_constraint = ++constraint_counter;

    if(objective_value < best_objective_value)
        best_objective_value = objective_value;

    return model_improvement_constraint;
}
#endif

// ------------- Helping functions -------------

template <class TLit, class TNumber>
void Prooflogger::write_weighted_literal(const TLit &lit, const TNumber& weight, const bool& add_prefix_space){
    write_number(weight, proof, add_prefix_space);
    _varMgr->write_literal(lit, proof, true);
}

template <typename TModel>
void Prooflogger::_log_solution(const TModel& model, const std::string& log_command, const bool only_original_variables_necessary, const bool log_as_comment){
    if(log_as_comment) 
        *proof << "* ";
    else
        _found_solution = true;
        
    *proof << log_command;
    VeriPB::Lit lit; 
    for (int i = 0; i < model_size(model); i++){
        lit = toVeriPbLit(model_literal(model, i));
        if(only_original_variables_necessary && _varMgr->is_aux_var(variable(lit)))
            continue;
        _varMgr->write_literal(lit, proof, true);
    }
    *proof << "\n";
}

template <typename TConstraint>
void Prooflogger::write_constraint(const TConstraint& cxn){
    for(int i = 0; i < size(cxn); i++){
        if(coefficient(cxn,i) == 0) continue;
        write_weighted_literal(literal(cxn,i), coefficient(cxn,i));
    }

    if(comparison(cxn) == Comparison::GEQ)
        *proof << " >= ";
    else if(comparison(cxn) == Comparison::LEQ)
        *proof << " <= ";

    write_number(rhs(cxn), proof, false);
}

template <typename TClause>
void Prooflogger::write_clause(const TClause& cxn){
    VeriPB::Lit prevlit, currentlit = lit_undef;

    for(int i = 0; i < size(cxn); i++){
        prevlit = currentlit;
        currentlit = toVeriPbLit(literal(cxn, i));

        assert(coefficient(cxn, i) == 1);
        assert(i == 0 || prevlit <= currentlit);

        if(prevlit != currentlit)
            write_weighted_literal(currentlit);
    }
    *proof << " >= 1";
}

template <typename TSeqHints>
void Prooflogger::write_hints(const TSeqHints& hints){
    *proof << " ~";
    for(constraintid cxn_id : hints)
        write_number(cxn_id,proof);
}

Prooflogger::Prooflogger(const std::string& prooffile, VarManager* varMgr) :
    proof(new std::ofstream(prooffile)), 
    _proofOwned(true),
    _varMgr(varMgr), 
    _keep_original_formula(false), 
    _n_orig_constraints(0), 
    _constraint_counter(0), 
    _found_solution(false), 
    _comments(true),
    _cpder(new CuttingPlanesDerivation(this, false))
{ }

Prooflogger::Prooflogger(const std::string& prooffile, VarManager* varMgr, int n_orig_constraints, bool keep_original_formula, bool comments) : 
    proof(new std::ofstream(prooffile)), 
    _proofOwned(true),
    _varMgr(varMgr), 
    _keep_original_formula(keep_original_formula), 
    _n_orig_constraints(n_orig_constraints), 
    _constraint_counter(n_orig_constraints), 
    _found_solution(false), 
    _comments(comments),
    _cpder(new CuttingPlanesDerivation(this, false))
{ }
Prooflogger::Prooflogger(std::ostream* proof, VarManager* varMgr) :
    proof(proof),
    _proofOwned(false),
    _varMgr(varMgr), 
    _keep_original_formula(false), 
    _n_orig_constraints(0), 
    _constraint_counter(0), 
    _found_solution(false), 
    _comments(true),
    _cpder(new CuttingPlanesDerivation(this, false))
{ }

Prooflogger::Prooflogger(std::ostream* proof, VarManager* varMgr, int n_orig_constraints, bool keep_original_formula, bool comments) :
    proof(proof),
    _proofOwned(false),
    _varMgr(varMgr), 
    _keep_original_formula(keep_original_formula), 
    _n_orig_constraints(n_orig_constraints), 
    _constraint_counter(n_orig_constraints), 
    _found_solution(false), 
    _comments(comments),
    _cpder(new CuttingPlanesDerivation(this, false))
{ }

Prooflogger::~Prooflogger(){
    if(_proofOwned){
        std::ofstream* fproof = static_cast<std::ofstream*>(proof);
        if(fproof->is_open()){
            end_proof();
        }
        delete proof;
    }    
    delete _cpder;
}


//=================================================================================================