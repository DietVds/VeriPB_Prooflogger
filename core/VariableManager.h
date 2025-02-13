#ifndef VeriPB_VariableManager_h
#define VeriPB_VariableManager_h

#include "VeriPbSolverTypes.h"
#include <sstream>
#include <cassert>

#ifndef INIT_NAMESTORAGE
#define INIT_NAMESTORAGE 500
#endif

class VarManager {
public:
    virtual void write_var_name(const VeriPB::Var&, std::ostream*, bool add_prefix_space=false);
    virtual std::string var_name(const VeriPB::Var&);
    virtual void write_literal(const VeriPB::Lit&, std::ostream*, bool add_prefix_space=false);
    virtual std::string literal_to_string(const VeriPB::Lit&);
    
    void set_number_original_variables(size_t& n);
    virtual bool is_aux_var(const VeriPB::Var&);

    VeriPB::Var new_variable_only_in_proof();
    size_t get_number_variables_only_in_proof();
private: 
    VeriPB::VarIdx _n_vars_only_known_in_proof=0;
    size_t _n_orig_vars = 0;
};

class VarManagerWithVarRewriting : public VarManager {
public: 
    virtual void write_var_name(const VeriPB::Var&, std::ostream*, bool add_prefix_space=false) override;
    virtual std::string var_name(const VeriPB::Var&) override;
    virtual void write_literal(const VeriPB::Lit&, std::ostream*, bool add_prefix_space=false) override;
    virtual std::string literal_to_string(const VeriPB::Lit&) override;

    void store_variable_name(const VeriPB::Var&, const std::string&);
    void store_rewrite_var_by_lit(const VeriPB::Var&, const VeriPB::Lit&);
private:
    bool has_meaningful_name(const VeriPB::Var&);
    bool needs_rewrite(const VeriPB::Var&);

    std::vector<bool> _solverVarsSpecialNameFlag;
    std::vector<bool> _onlyproofVarsSpecialNameFlag;
    std::vector<std::string> _solverVarsNameStorage; 
    std::vector<std::string> _proofVarsNameStorage;

    std::vector<bool> _solverVarsRewriteFlag;
    std::vector<bool> _proofVarsRewriteFlag;
    std::vector<VeriPB::Lit> _solverVarsRewriteStorage;
    std::vector<VeriPB::Lit> _proofVarsRewriteStorage;
};

/**********
 * Implementation for default VarManager
 */

void VarManager::write_var_name(const VeriPB::Var& var, std::ostream* s, bool add_prefix_space){
    if(add_prefix_space)
        *s << ' ';
    if(var.only_known_in_proof)
        *s << "_p";
    else if(is_aux_var(var))
        *s << 'y';
    else
        *s << 'x';
    VeriPB::write_number(varidx(var));
}

std::string VarManager::var_name(const VeriPB::Var& var){
    std::string prefix;
    if(var.only_known_in_proof)
        prefix =  "_p";
    else if(is_aux_var(var))
        prefix =  'y';
    else
        prefix = 'x';
    return prefix + VeriPB::number_to_string(varidx(var));
}


void VarManager::write_literal(const VeriPB::Lit& lit, std::ostream* s, bool add_prefix_space){
    if(add_prefix_space)
        *s << ' ';
    if(is_negated(lit))
        *s << '~';
    write_var_name(VeriPB::variable<VeriPB::Var, VeriPB::Lit>(lit), s);
}

std::string VarManager::literal_to_string(const VeriPB::Lit& lit){
    return (is_negated(lit) ? "~" : "") + var_name(VeriPB::variable<VeriPB::Var, VeriPB::Lit>(lit));
}

void VarManager::set_number_original_variables(size_t& n){
    assert(n == 0);
    _n_orig_vars = n;
}

bool VarManager::is_aux_var(const VeriPB::Var& var){
    return (varidx(var) > _n_orig_vars) || var.only_known_in_proof;
}

VeriPB::Var VarManager::new_variable_only_in_proof(){
    VeriPB::Var v;
    v.only_known_in_proof=true;
    v.v = ++_n_vars_only_known_in_proof;
}

size_t VarManager::get_number_variables_only_in_proof(){
    return _n_vars_only_known_in_proof;
}

/***********
 * Implementation for VarManagerWithVarRewriting
 */

void VarManagerWithVarRewriting::write_var_name(const VeriPB::Var& var, std::ostream* s, bool add_prefix_space){
    if(has_meaningful_name(var)){
        if(add_prefix_space)
            *s << ' ';

        if(var.only_known_in_proof)
            *s << _proofVarsNameStorage[var.v];
        else
            *s << _solverVarsNameStorage[var.v];
    }
    else{
        VarManager::var_name(var);
    }
}

std::string VarManagerWithVarRewriting::var_name(const VeriPB::Var& var){
    if(has_meaningful_name(var)){
        if(var.only_known_in_proof)
            return _proofVarsNameStorage[var.v];
        else
            return _solverVarsNameStorage[var.v];
    }
    else{
        return VarManager::var_name(var);
    }
}

void VarManagerWithVarRewriting::write_literal(const VeriPB::Lit& lit, std::ostream* s, bool add_prefix_space){
    if(add_prefix_space)
        *s << ' ';

    VeriPB::Lit l = lit;
    while(true) {
        VeriPB::Var v = VeriPB::variable<VeriPB::Var, VeriPB::Lit>(l);

        if(!needs_rewrite(v)) {
           VarManager::write_literal(lit, s, add_prefix_space);
           return;
        }

        const std::vector<VeriPB::Lit>* rewriteStorage = v.only_known_in_proof ? &_proofVarsRewriteStorage : &_solverVarsRewriteStorage;
        const VeriPB::Lit lit_to_rewrite_to = is_negated(lit) ? neg((*rewriteStorage)[v.v]) : (*rewriteStorage)[v.v];

        if(VeriPB::variable<VeriPB::Var, VeriPB::Lit>(lit_to_rewrite_to) == v) {
            VarManager::write_literal(lit, s, add_prefix_space);
            return;
        }
        l = lit_to_rewrite_to;
    }
}

std::string VarManagerWithVarRewriting::literal_to_string(const VeriPB::Lit& lit){
    std::string res;

    VeriPB::Lit l = lit;
    while(true) {
        VeriPB::Var v = VeriPB::variable<VeriPB::Var, VeriPB::Lit>(l);

        if(!needs_rewrite(v)) {
            return VarManager::literal_to_string(l);
        }

        const std::vector<VeriPB::Lit>* rewriteStorage = v.only_known_in_proof ? &_proofVarsRewriteStorage : &_solverVarsRewriteStorage;
        const VeriPB::Lit lit_to_rewrite_to = is_negated(lit) ? neg((*rewriteStorage)[v.v]) : (*rewriteStorage)[v.v];

        if(VeriPB::variable<VeriPB::Var, VeriPB::Lit>(lit_to_rewrite_to) == v) {
            return VarManager::literal_to_string(lit_to_rewrite_to);
        }
        l = lit_to_rewrite_to;
    }
}

void VarManagerWithVarRewriting::store_variable_name(const VeriPB::Var& var, const std::string& name){
    std::vector<std::string>* nameStorage = var.only_known_in_proof ? &_proofVarsNameStorage : &_solverVarsNameStorage;
    std::vector<bool>* meaningfulnameFlag = var.only_known_in_proof ? &_onlyproofVarsSpecialNameFlag : &_solverVarsSpecialNameFlag;

    // Increase storage if necessary.
    if(var.v >= nameStorage->size() && INIT_NAMESTORAGE > var.v ){
        nameStorage->resize(INIT_NAMESTORAGE);
    }
    else if(var.v >= nameStorage->size()) {
        nameStorage->resize(2 * var.v);
    }
    if(var.v >= meaningfulnameFlag->size() && INIT_NAMESTORAGE > var.v ){
        meaningfulnameFlag->resize(INIT_NAMESTORAGE);
    }
    else if(var.v >= meaningfulnameFlag->size()) {
        meaningfulnameFlag->resize(2 * var.v);
    }

    (*meaningfulnameFlag)[var.v] = true;
    (*nameStorage)[var.v] = name;
}
void VarManagerWithVarRewriting::store_rewrite_var_by_lit(const VeriPB::Var& var, const VeriPB::Lit& lit){
    std::vector<VeriPB::Lit>* rewriteStorage = var.only_known_in_proof ? &_solverVarsRewriteStorage : &_proofVarsRewriteStorage;
    std::vector<bool>* rewriteFlag = var.only_known_in_proof ? &_proofVarsRewriteFlag : &_solverVarsRewriteFlag;

    if(var.v >=  rewriteStorage->size() && INIT_NAMESTORAGE > var.v){
        rewriteStorage->resize(INIT_NAMESTORAGE, VeriPB::lit_undef);
    }
    else if(var.v >= rewriteStorage->size()){
        rewriteStorage->resize(2 * var.v, VeriPB::lit_undef);
    }

    if(var.v >=  rewriteFlag->size() && INIT_NAMESTORAGE > var.v){
        rewriteFlag->resize(INIT_NAMESTORAGE);
    }
    else if(var.v >= rewriteFlag->size()){
        rewriteFlag->resize(2 * var.v);
    }

    (*rewriteFlag)[var.v] = true;
    (*rewriteStorage)[var.v] = lit;
}

bool VarManagerWithVarRewriting::has_meaningful_name(const VeriPB::Var& var){
    std::vector<bool>* meaningfulnameFlag = var.only_known_in_proof ? &_onlyproofVarsSpecialNameFlag : &_solverVarsSpecialNameFlag;

    return var.v < meaningfulnameFlag->size() && meaningfulnameFlag->at(var.v);
}

bool VarManagerWithVarRewriting::needs_rewrite(const VeriPB::Var& var){
    std::vector<bool>* rewriteFlag = var.only_known_in_proof ? &_solverVarsRewriteFlag : &_proofVarsRewriteFlag;

    return var.v < rewriteFlag->size() && rewriteFlag->at(var.v);
}

#endif