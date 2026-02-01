/**
 * NanoBrain WebAssembly Bindings
 *
 * Exposes NanoBrain functionality to JavaScript via Emscripten.
 *
 * Usage in browser:
 *   const nb = await NanoBrain();
 *   const kernel = nb.createKernel();
 *   kernel.initialize();
 */

#include <emscripten/bind.h>
#include <emscripten/emscripten.h>


#include "nanobrain_gog.h"
#include "nanobrain_kernel.h"
#include "nanobrain_ppm.h"
#include "nanobrain_tc_transform.h"
#include "nanobrain_time_crystal.h"
#include "nanobrain_types.h"
#include "nanobrain_unified.h"


using namespace emscripten;

// ================================================================
// Wrapper Classes for JavaScript
// ================================================================

/**
 * JavaScript-friendly wrapper for UnifiedKernel
 */
class JsUnifiedKernel {
public:
  JsUnifiedKernel() {
    config.debug_output = false;
    kernel = std::make_unique<UnifiedNanoBrainKernel>(config);
  }

  void initialize() { kernel->initialize(); }

  void shutdown() { kernel->shutdown(); }

  bool isActive() const { return kernel->is_active(); }

  std::string createAtom(const std::string &type, const std::string &name,
                         float strength, float confidence) {
    return kernel->create_atom(type, name, strength, confidence, {});
  }

  val getMetrics() const {
    auto m = kernel->get_metrics();
    val result = val::object();
    result.set("totalAtoms", m.total_atoms);
    result.set("totalLinks", m.total_links);
    result.set("quantumCoherence", m.quantum_coherence);
    result.set("consciousnessEmergence", m.consciousness_emergence);
    result.set("processingCycles", m.processing_cycles);
    return result;
  }

  void processCycle() { kernel->process_cycle(); }

  void runCycles(int count) { kernel->run_cycles(count); }

  float computeCoherence(const std::vector<int> &primes) {
    return kernel->compute_coherence(primes);
  }

private:
  UnifiedNanoBrainConfig config;
  std::unique_ptr<UnifiedNanoBrainKernel> kernel;
};

/**
 * JavaScript-friendly wrapper for TimeCrystalKernel
 */
class JsTimeCrystalKernel {
public:
  JsTimeCrystalKernel() {
    kernel = std::make_unique<TimeCrystalKernel>(config);
  }

  void initialize() { kernel->initialize(); }

  void shutdown() { kernel->shutdown(); }

  bool isActive() const { return kernel->is_active(); }

  float computePPMCoherence(const std::vector<int> &primes) {
    return kernel->compute_ppm_coherence(primes);
  }

  void processCycle() { kernel->process_cycle(); }

private:
  TimeCrystalConfig config;
  std::unique_ptr<TimeCrystalKernel> kernel;
};

/**
 * JavaScript-friendly TC Transform
 */
class JsTCTransform {
public:
  JsTCTransform() {
    kernelConfig.memory_size = 1024 * 1024; // 1MB for WASM
    kernel = std::make_unique<NanoBrainKernel>(kernelConfig);
    transform = std::make_unique<TCTransformEngine>(kernel.get(), config);
  }

  val transform(const std::vector<float> &signal) {
    auto result = transform->transform(signal);

    val jsResult = val::object();
    jsResult.set("primeSpectrum", val::array(result.prime_spectrum));
    jsResult.set("phaseValues", val::array(result.phase_values));
    jsResult.set("overallCoherence", result.overall_coherence);
    jsResult.set("signalEnergy", result.signal_energy);
    jsResult.set("dominantPrimes", val::array(result.dominant_primes));

    return jsResult;
  }

  std::vector<float> inverseTransform(const std::vector<float> &spectrum,
                                      const std::vector<float> &phases) {
    TCTransformResult result;
    result.prime_spectrum = spectrum;
    result.phase_values = phases;

    // Reconstruct components from spectrum
    auto primes = transform->get_analysis_primes();
    for (size_t i = 0; i < spectrum.size() && i < primes.size(); i++) {
      TCSpectralComponent comp;
      comp.prime = primes[i];
      comp.amplitude = spectrum[i];
      comp.phase = i < phases.size() ? phases[i] : 0.0f;
      result.components.push_back(comp);
    }

    return transform->inverse_transform(result);
  }

private:
  NanoBrainConfig kernelConfig;
  TCTransformConfig config;
  std::unique_ptr<NanoBrainKernel> kernel;
  std::unique_ptr<TCTransformEngine> transform;
};

// ================================================================
// Constants
// ================================================================

val getFundamentalPrimes() {
  std::vector<int> primes(FUNDAMENTAL_PRIMES,
                          FUNDAMENTAL_PRIMES + FUNDAMENTAL_PRIMES_COUNT);
  return val::array(primes);
}

// ================================================================
// Emscripten Bindings
// ================================================================

EMSCRIPTEN_BINDINGS(nanobrain) {
  // Constants
  function("getFundamentalPrimes", &getFundamentalPrimes);
  constant("TIME_CRYSTAL_DIMENSIONS", TIME_CRYSTAL_DIMENSIONS);
  constant("GOLDEN_RATIO", GOLDEN_RATIO);
  constant("PI", PI);

  // Vector types
  register_vector<int>("VectorInt");
  register_vector<float>("VectorFloat");
  register_vector<std::string>("VectorString");

  // UnifiedKernel
  class_<JsUnifiedKernel>("UnifiedKernel")
      .constructor<>()
      .function("initialize", &JsUnifiedKernel::initialize)
      .function("shutdown", &JsUnifiedKernel::shutdown)
      .function("isActive", &JsUnifiedKernel::isActive)
      .function("createAtom", &JsUnifiedKernel::createAtom)
      .function("getMetrics", &JsUnifiedKernel::getMetrics)
      .function("processCycle", &JsUnifiedKernel::processCycle)
      .function("runCycles", &JsUnifiedKernel::runCycles)
      .function("computeCoherence", &JsUnifiedKernel::computeCoherence);

  // TimeCrystalKernel
  class_<JsTimeCrystalKernel>("TimeCrystalKernel")
      .constructor<>()
      .function("initialize", &JsTimeCrystalKernel::initialize)
      .function("shutdown", &JsTimeCrystalKernel::shutdown)
      .function("isActive", &JsTimeCrystalKernel::isActive)
      .function("computePPMCoherence",
                &JsTimeCrystalKernel::computePPMCoherence)
      .function("processCycle", &JsTimeCrystalKernel::processCycle);

  // TCTransform
  class_<JsTCTransform>("TCTransform")
      .constructor<>()
      .function("transform", &JsTCTransform::transform)
      .function("inverseTransform", &JsTCTransform::inverseTransform);
}

// ================================================================
// Main (required for Emscripten)
// ================================================================

int main() {
  // Module initialization
  return 0;
}
