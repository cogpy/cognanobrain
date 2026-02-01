/**
 * NanoBrain Python Bindings via pybind11
 *
 * Exposes core NanoBrain C++ classes to Python.
 *
 * Build: pip install .
 * Usage: import nanobrain
 */

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


#include "../src/cpp/nanobrain_attention.h"
#include "../src/cpp/nanobrain_gog.h"
#include "../src/cpp/nanobrain_kernel.h"
#include "../src/cpp/nanobrain_reasoning.h"
#include "../src/cpp/nanobrain_spontaneous.h"
#include "../src/cpp/nanobrain_tc_transform.h"
#include "../src/cpp/nanobrain_time_crystal.h"
#include "../src/cpp/nanobrain_types.h"
#include "../src/cpp/nanobrain_unified.h"


namespace py = pybind11;

// ================================================================
// Helper functions
// ================================================================

std::vector<float> numpy_to_vector(py::array_t<float> arr) {
  auto buf = arr.request();
  float *ptr = static_cast<float *>(buf.ptr);
  return std::vector<float>(ptr, ptr + buf.size);
}

py::array_t<float> vector_to_numpy(const std::vector<float> &vec) {
  return py::array_t<float>(vec.size(), vec.data());
}

// ================================================================
// Module Definition
// ================================================================

PYBIND11_MODULE(_core, m) {
  m.doc() = "NanoBrain C++ cognitive architecture bindings";

  // ----------------------------------------------------------------
  // Basic Types
  // ----------------------------------------------------------------

  py::class_<TruthValue>(m, "TruthValue")
      .def(py::init<>())
      .def(py::init<float, float, float>())
      .def_readwrite("strength", &TruthValue::strength)
      .def_readwrite("confidence", &TruthValue::confidence)
      .def_readwrite("count", &TruthValue::count)
      .def("__repr__", [](const TruthValue &tv) {
        return "<TruthValue s=" + std::to_string(tv.strength) +
               " c=" + std::to_string(tv.confidence) + ">";
      });

  py::class_<AttentionValue>(m, "AttentionValue")
      .def(py::init<>())
      .def(py::init<float, float, float>())
      .def_readwrite("sti", &AttentionValue::sti)
      .def_readwrite("lti", &AttentionValue::lti)
      .def_readwrite("vlti", &AttentionValue::vlti)
      .def("__repr__", [](const AttentionValue &av) {
        return "<AttentionValue sti=" + std::to_string(av.sti) + ">";
      });

  // ----------------------------------------------------------------
  // Atom & Link (basic structures)
  // ----------------------------------------------------------------

  py::class_<Atom>(m, "Atom")
      .def(py::init<>())
      .def_readwrite("id", &Atom::id)
      .def_readwrite("type", &Atom::type)
      .def_readwrite("name", &Atom::name)
      .def_property(
          "truth_value",
          [](const Atom &a) {
            return TruthValue{a.truth_value[0], a.truth_value[1],
                              a.truth_value[2]};
          },
          [](Atom &a, const TruthValue &tv) {
            a.truth_value[0] = tv.strength;
            a.truth_value[1] = tv.confidence;
            a.truth_value[2] = tv.count;
          })
      .def("__repr__", [](const Atom &a) {
        return "<Atom " + a.type + ":'" + a.name + "'>";
      });

  py::class_<Link>(m, "Link")
      .def(py::init<>())
      .def_readwrite("id", &Link::id)
      .def_readwrite("type", &Link::type)
      .def_readwrite("outgoing", &Link::outgoing)
      .def("__repr__", [](const Link &l) {
        return "<Link " + l.type +
               " arity=" + std::to_string(l.outgoing.size()) + ">";
      });

  // ----------------------------------------------------------------
  // NanoBrain Kernel
  // ----------------------------------------------------------------

  py::class_<NanoBrainConfig>(m, "KernelConfig")
      .def(py::init<>())
      .def_readwrite("memory_size", &NanoBrainConfig::memory_size)
      .def_readwrite("use_gpu", &NanoBrainConfig::use_gpu);

  py::class_<NanoBrainKernel>(m, "Kernel")
      .def(py::init<const NanoBrainConfig &>())
      .def("compute_coherence",
           [](NanoBrainKernel &k, std::vector<int> primes) {
             return k.compute_coherence(primes);
           })
      .def("__repr__",
           [](const NanoBrainKernel &) { return "<NanoBrainKernel>"; });

  // ----------------------------------------------------------------
  // Time Crystal
  // ----------------------------------------------------------------

  py::class_<TimeCrystalConfig>(m, "TimeCrystalConfig")
      .def(py::init<>())
      .def_readwrite("memory_size", &TimeCrystalConfig::memory_size)
      .def_readwrite("quantum_coherence_threshold",
                     &TimeCrystalConfig::quantum_coherence_threshold)
      .def_readwrite("attention_decay_rate",
                     &TimeCrystalConfig::attention_decay_rate)
      .def_readwrite("resource_budget", &TimeCrystalConfig::resource_budget);

  py::class_<TimeCrystalAtom>(m, "TimeCrystalAtom")
      .def(py::init<>())
      .def_readwrite("id", &TimeCrystalAtom::id)
      .def_readwrite("type", &TimeCrystalAtom::type)
      .def_readwrite("name", &TimeCrystalAtom::name)
      .def_readwrite("truth_value", &TimeCrystalAtom::truth_value)
      .def_readwrite("attention_value", &TimeCrystalAtom::attention_value)
      .def_readwrite("prime_encoding", &TimeCrystalAtom::prime_encoding)
      .def("__repr__", [](const TimeCrystalAtom &a) {
        return "<TimeCrystalAtom " + a.type + ":'" + a.name + "'>";
      });

  py::class_<TimeCrystalKernel>(m, "TimeCrystalKernel")
      .def(py::init<const TimeCrystalConfig &>())
      .def("initialize", &TimeCrystalKernel::initialize)
      .def("shutdown", &TimeCrystalKernel::shutdown)
      .def("is_active", &TimeCrystalKernel::is_active)
      .def("create_atom", &TimeCrystalKernel::create_atom, py::arg("type"),
           py::arg("name"), py::arg("tv"), py::arg("av"),
           py::arg("prime_encoding"), py::arg("geometry"))
      .def("get_atom", &TimeCrystalKernel::get_atom,
           py::return_value_policy::reference)
      .def("get_all_atom_ids", &TimeCrystalKernel::get_all_atom_ids)
      .def("compute_ppm_coherence", &TimeCrystalKernel::compute_ppm_coherence)
      .def("process_cycle", &TimeCrystalKernel::process_cycle)
      .def("__repr__", [](const TimeCrystalKernel &k) {
        return "<TimeCrystalKernel atoms=" +
               std::to_string(k.get_all_atom_ids().size()) + ">";
      });

  // ----------------------------------------------------------------
  // Unified Kernel
  // ----------------------------------------------------------------

  py::class_<UnifiedNanoBrainConfig>(m, "UnifiedConfig")
      .def(py::init<>())
      .def_readwrite("memory_size", &UnifiedNanoBrainConfig::memory_size)
      .def_readwrite("time_crystal_dimensions",
                     &UnifiedNanoBrainConfig::time_crystal_dimensions)
      .def_readwrite("max_reasoning_depth",
                     &UnifiedNanoBrainConfig::max_reasoning_depth)
      .def_readwrite("enable_meta_cognition",
                     &UnifiedNanoBrainConfig::enable_meta_cognition)
      .def_readwrite("debug_output", &UnifiedNanoBrainConfig::debug_output);

  py::class_<UnifiedNanoBrainMetrics>(m, "UnifiedMetrics")
      .def(py::init<>())
      .def_readwrite("total_atoms", &UnifiedNanoBrainMetrics::total_atoms)
      .def_readwrite("total_links", &UnifiedNanoBrainMetrics::total_links)
      .def_readwrite("quantum_coherence",
                     &UnifiedNanoBrainMetrics::quantum_coherence)
      .def_readwrite("consciousness_emergence",
                     &UnifiedNanoBrainMetrics::consciousness_emergence)
      .def("__repr__", [](const UnifiedNanoBrainMetrics &m) {
        return "<Metrics atoms=" + std::to_string(m.total_atoms) +
               " consciousness=" + std::to_string(m.consciousness_emergence) +
               ">";
      });

  py::class_<UnifiedNanoBrainKernel>(m, "UnifiedKernel")
      .def(py::init<const UnifiedNanoBrainConfig &>())
      .def("initialize", &UnifiedNanoBrainKernel::initialize)
      .def("shutdown", &UnifiedNanoBrainKernel::shutdown)
      .def("is_active", &UnifiedNanoBrainKernel::is_active)
      .def("create_atom", &UnifiedNanoBrainKernel::create_atom, py::arg("type"),
           py::arg("name"), py::arg("strength"), py::arg("confidence"),
           py::arg("prime_encoding") = std::vector<int>{})
      .def("get_atom", &UnifiedNanoBrainKernel::get_atom,
           py::return_value_policy::reference)
      .def("get_all_atom_ids", &UnifiedNanoBrainKernel::get_all_atom_ids)
      .def("start_reasoning", &UnifiedNanoBrainKernel::start_reasoning)
      .def("execute_reasoning_step",
           &UnifiedNanoBrainKernel::execute_reasoning_step)
      .def("process_cycle", &UnifiedNanoBrainKernel::process_cycle)
      .def("run_cycles", &UnifiedNanoBrainKernel::run_cycles)
      .def("compute_coherence", &UnifiedNanoBrainKernel::compute_coherence)
      .def("get_metrics", &UnifiedNanoBrainKernel::get_metrics)
      .def("get_config", &UnifiedNanoBrainKernel::get_config,
           py::return_value_policy::reference)
      .def("__repr__", [](const UnifiedNanoBrainKernel &k) {
        auto m = k.get_metrics();
        return "<UnifiedKernel atoms=" + std::to_string(m.total_atoms) +
               " active=" + (k.is_active() ? "true" : "false") + ">";
      });

  // ----------------------------------------------------------------
  // Garden of Gardens
  // ----------------------------------------------------------------

  py::class_<GOGConfig>(m, "GOGConfig")
      .def(py::init<>())
      .def_readwrite("max_depth", &GOGConfig::max_depth)
      .def_readwrite("petals_per_node", &GOGConfig::petals_per_node)
      .def_readwrite("meander_frequency", &GOGConfig::meander_frequency);

  py::class_<GOGMetrics>(m, "GOGMetrics")
      .def_readonly("total_gardens", &GOGMetrics::total_gardens)
      .def_readonly("total_petals", &GOGMetrics::total_petals)
      .def_readonly("average_coherence", &GOGMetrics::average_coherence);

  py::class_<GardenOfGardens>(m, "GardenOfGardens")
      .def(py::init<NanoBrainKernel *, const GOGConfig &>())
      .def("initialize", &GardenOfGardens::initialize)
      .def("shutdown", &GardenOfGardens::shutdown)
      .def("is_active", &GardenOfGardens::is_active)
      .def("create_garden", &GardenOfGardens::create_garden)
      .def("get_all_garden_ids", &GardenOfGardens::get_all_garden_ids)
      .def("process_cycle", &GardenOfGardens::process_cycle)
      .def("get_metrics", &GardenOfGardens::get_metrics)
      .def("__repr__", [](const GardenOfGardens &g) {
        return "<GardenOfGardens gardens=" +
               std::to_string(g.get_all_garden_ids().size()) + ">";
      });

  // ----------------------------------------------------------------
  // TC Transform
  // ----------------------------------------------------------------

  py::class_<TCTransformConfig>(m, "TCTransformConfig")
      .def(py::init<>())
      .def_readwrite("num_prime_components",
                     &TCTransformConfig::num_prime_components)
      .def_readwrite("normalize_output", &TCTransformConfig::normalize_output);

  py::class_<TCTransformResult>(m, "TCTransformResult")
      .def_readonly("prime_spectrum", &TCTransformResult::prime_spectrum)
      .def_readonly("phase_values", &TCTransformResult::phase_values)
      .def_readonly("overall_coherence", &TCTransformResult::overall_coherence)
      .def_readonly("dominant_primes", &TCTransformResult::dominant_primes);

  py::class_<TCTransformEngine>(m, "TCTransformEngine")
      .def(py::init<NanoBrainKernel *, const TCTransformConfig &>())
      .def("transform", &TCTransformEngine::transform)
      .def("inverse_transform", &TCTransformEngine::inverse_transform)
      .def("extract_prime_encoding", &TCTransformEngine::extract_prime_encoding)
      .def("__repr__",
           [](const TCTransformEngine &) { return "<TCTransformEngine>"; });

  // ----------------------------------------------------------------
  // Module-level constants
  // ----------------------------------------------------------------

  m.attr("FUNDAMENTAL_PRIMES") = py::cast(
      std::vector<int>(FUNDAMENTAL_PRIMES.begin(), FUNDAMENTAL_PRIMES.end()));
  m.attr("TIME_CRYSTAL_DIMENSIONS") = TIME_CRYSTAL_DIMENSIONS;
  m.attr("GOLDEN_RATIO") = GOLDEN_RATIO;
  m.attr("PI") = PI;

  // Version
  m.attr("__version__") = "1.0.0";
}
