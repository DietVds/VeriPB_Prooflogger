#include <pybind11/functional.h>
#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include "VeriPBProoflogger.h"
#include "TotalizerProoflogger.h"
#include "MaxSATProoflogger.h"

template <typename... Args>
using overload_cast_ = pybind11::detail::overload_cast_impl<Args...>;

py::capsule getCapsuleVeriPbPL(VeriPbProofLogger &PL)
{
    auto pybind_capsule = py::capsule(&PL, "prooflogger", nullptr);
    return pybind_capsule;
}

py::capsule getCapsuleTotalizerPL(TotalizerProoflogger &PL)
{
    auto pybind_capsule = py::capsule(&PL, "totalizerprooflogger", nullptr);
    return pybind_capsule;
}

py::capsule getCapsuleMaxSATPL(MaxSATProoflogger &PL)
{
    auto pybind_capsule = py::capsule(&PL, "maxsatprooflogger", nullptr);
    return pybind_capsule;
}

PYBIND11_MODULE(pyprooflogger, m)
{
    m.doc() = "Pseudo-Boolean proof logging library.";
    py::class_<VeriPbProofLogger>(m, "VeriPbProoflogger")
        .def(py::init<>())
        .def("start_proof", overload_cast_<const std::string, int, int>()(&VeriPbProofLogger::start_proof))
        .def("start_proof", overload_cast_<const std::string, int>()(&VeriPbProofLogger::start_proof))
        .def("init_proof_file", &VeriPbProofLogger::init_proof_file)
        .def("end_proof", &VeriPbProofLogger::end_proof)
        .def("write_proof_header", overload_cast_<int, int>()(&VeriPbProofLogger::write_proof_header))
        .def("write_proof_header", overload_cast_<int>()(&VeriPbProofLogger::write_proof_header))
        .def("set_objective", &VeriPbProofLogger::set_objective)
        .def("write_comment", overload_cast_<const char *>()(&VeriPbProofLogger::write_comment))
        .def("write_comment", overload_cast_<const std::string &>()(&VeriPbProofLogger::write_comment))
        .def("equals_rule", overload_cast_<const constraintid, const std::vector<int> &, const int>()(&VeriPbProofLogger::equals_rule<int>))
        .def("equals_rule", overload_cast_<const constraintid, const std::vector<int> &, const std::vector<int> &, const int>()(&VeriPbProofLogger::equals_rule<int>))
        .def("check_last_constraint", overload_cast_<const std::vector<int> &, const int>()(&VeriPbProofLogger::check_last_constraint<int>))
        .def("check_last_constraint", overload_cast_<const std::vector<int> &, const std::vector<int> &, const int>()(&VeriPbProofLogger::check_last_constraint<int>))
        .def("log_solution", &VeriPbProofLogger::log_solution<int>)
        .def("log_solution_with_check", &VeriPbProofLogger::log_solution_with_check<int>)
        .def("unchecked_assumption", overload_cast_<const std::vector<int> &, const int>()(&VeriPbProofLogger::unchecked_assumption<int>))
        .def("unchecked_assumption", overload_cast_<const std::vector<int> &, const std::vector<int> &, const int>()(&VeriPbProofLogger::unchecked_assumption<int>))
        .def("rup_constraint", overload_cast_<const std::vector<int> &, const int>()(&VeriPbProofLogger::rup<int>))
        .def("rup_constraint", overload_cast_<const std::vector<int> &, const std::vector<int> &, const int>()(&VeriPbProofLogger::rup<int>))
        .def("redundancy_based_strengthening", overload_cast_<const std::vector<int> &, const int, const int &>()(&VeriPbProofLogger::redundanceBasedStrengthening<int>))
        .def("redundancy_based_strengthening", overload_cast_<const std::vector<int> &, const int, const std::vector<int> &>()(&VeriPbProofLogger::redundanceBasedStrengthening<int>))
        .def("redundancy_based_strengthening", overload_cast_<const std::vector<int> &, const std::vector<int> &, const int, const int &>()(&VeriPbProofLogger::redundanceBasedStrengthening<int>))
        .def("redundancy_based_strengthening", overload_cast_<const std::vector<int> &, const std::vector<int> &, const int, const std::vector<int> &>()(&VeriPbProofLogger::redundanceBasedStrengthening<int>))
        .def("start_pol_derivation", &VeriPbProofLogger::start_CP_derivation)
        .def("start_pol_derivation_with_lit_axiom", &VeriPbProofLogger::start_CP_derivation_with_lit_axiom<int>)
        .def("pol_add_constraint", &VeriPbProofLogger::CP_add_constraint)
        .def("pol_divide", &VeriPbProofLogger::CP_divide)
        .def("pol_saturate", &VeriPbProofLogger::CP_saturate)
        .def("pol_multiply", &VeriPbProofLogger::CP_multiply)
        .def("pol_weakening", &VeriPbProofLogger::CP_weakening<int>)
        .def("pol_literal_axiom", &VeriPbProofLogger::CP_literal_axiom<int>)
        .def("end_pol_derivation", &VeriPbProofLogger::end_CP_derivation)
        .def("delete_constraint", overload_cast_<const constraintid>()(&VeriPbProofLogger::delete_constraint<int>))
        .def("delete_constraint", overload_cast_<const std::vector<constraintid> &>()(&VeriPbProofLogger::delete_constraint<int>))
        .def("delete_constraint", overload_cast_<const std::vector<int> &, const int>()(&VeriPbProofLogger::delete_constraint<int>))
        .def("delete_constraint", overload_cast_<const std::vector<int> &, const std::vector<int> &, const int>()(&VeriPbProofLogger::delete_constraint<int>))
        // .def("overwrite_constraint", &VeriPbProofLogger::overwrite_constraint)
        .def("write_previous_constraint_contradiction", &VeriPbProofLogger::write_previous_constraint_contradiction)
        .def("rup_empty_clause", &VeriPbProofLogger::rup_empty_clause);

    py::class_<TotalizerProoflogger>(m, "TotalizerProoflogger")
        .def(py::init<VeriPbProofLogger *>())
        .def("get_PbDef_Impl_CxnId", &TotalizerProoflogger::get_PbDef_Impl_CxnId<int>)
        .def("get_PbDef_invImpl_CxnId", &TotalizerProoflogger::get_PbDef_invImpl_CxnId<int>);

    py::class_<MaxSATProoflogger>(m, "MaxSATProoflogger")
        .def(py::init<VeriPbProofLogger *>())
        .def("add_blocking_literal_for_var", &MaxSATProoflogger::add_blocking_literal_for_var<int>)
        .def("add_core_lower_bound", &MaxSATProoflogger::add_core_lower_bound<int>)
        .def("update_core_lower_bound", &MaxSATProoflogger::update_core_lower_bound<int>)
        .def("derive_objective_reformulation_constraint", &MaxSATProoflogger::derive_objective_reformulation_constraint)
        .def("proof_log_objective_reformulation", &MaxSATProoflogger::proof_log_objective_reformulation)
        .def("derive_at_most_one_constraint", &MaxSATProoflogger::derive_at_most_one_constraint<int>)
        .def("introduce_at_most_one_selector", &MaxSATProoflogger::introduce_at_most_one_selector<int>)
        .def("proof_log_at_most_one", &MaxSATProoflogger::proof_log_at_most_one<int>)
        .def("base_reform_unit_core", &MaxSATProoflogger::base_reform_unit_core);

    m.def("getCapsuleVeriPbPL", getCapsuleVeriPbPL);
    m.def("getCapsuleTotalizerPL", getCapsuleTotalizerPL);
    m.def("getCapsuleMaxSATPL", getCapsuleMaxSATPL);
}
