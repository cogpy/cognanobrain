#include "nanobrain_hinductor.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <sstream>

// ================================================================
// Hinductor Implementation
// ================================================================

Hinductor::Hinductor(NanoBrainKernel *kernel, const HinductorConfig &config)
    : kernel(kernel), config(config), current_time(0.0f) {
  reset();
}

Hinductor::~Hinductor() {
  // Tensor cleanup handled by kernel
}

void Hinductor::reset() {
  state.magnetic_flux = config.initial_flux;
  state.vortex_charge = 0.0f;
  state.capacitance = config.capacitance_base;
  state.inductance = config.inductance_base;
  state.resistance = config.resistance_base;
  state.power_dissipation = 0.0f;
  state.flux_history.clear();
  state.resistance_history.clear();
  state.light_intensity = 0.0f;
  state.light_frequency = MAGNETIC_LIGHT_FREQUENCY;
  state.polarization = {1.0f, 0.0f, 0.0f}; // Linear polarization
  current_time = 0.0f;
}

float Hinductor::vortex_resistance(float magnetic_flux) {
  update_state(magnetic_flux);
  return state.resistance;
}

float Hinductor::calculate_vortex_resistance(float flux) {
  // Vortex-based resistance model using golden ratio harmonics
  // R = R_base * (1 + α * |Φ|^β * sin(γ * Φ))

  float phi_normalized = flux / (flux + 1.0f); // Normalize to [0, 1)

  // Golden ratio modulation
  float golden_term = std::sin(GOLDEN_RATIO * M_PI * phi_normalized);

  // Vortex coupling nonlinearity
  float vortex_term = std::pow(std::abs(phi_normalized), 0.5f);

  // Combine terms
  float modulation = 1.0f + config.vortex_coupling * vortex_term * golden_term;

  // Apply hysteresis if enabled
  if (config.enable_hysteresis && !state.flux_history.empty()) {
    float avg_flux = 0.0f;
    for (float f : state.flux_history) {
      avg_flux += f;
    }
    avg_flux /= state.flux_history.size();

    // Hysteresis adds memory effect
    float hysteresis_factor = 1.0f + 0.1f * std::tanh(flux - avg_flux);
    modulation *= hysteresis_factor;
  }

  return config.resistance_base * modulation;
}

void Hinductor::oscillate_capacitance(float delta_time) {
  current_time += delta_time;
  state.capacitance = calculate_oscillating_capacitance(current_time);

  // Update inductance based on LC coupling
  // L * C = 1 / ω²
  float omega = config.oscillation_frequency * 2.0f * M_PI;
  state.inductance = 1.0f / (omega * omega * state.capacitance);
}

float Hinductor::calculate_oscillating_capacitance(float time) {
  // Oscillating capacitance with prime harmonic modulation
  float omega = config.oscillation_frequency * 2.0f * M_PI;

  // Base oscillation
  float base_osc = std::cos(omega * time);

  // Add prime harmonics (2, 3, 5, 7)
  float prime_harmonic = 0.0f;
  prime_harmonic += 0.3f * std::cos(2.0f * omega * time);
  prime_harmonic += 0.2f * std::cos(3.0f * omega * time);
  prime_harmonic += 0.1f * std::cos(5.0f * omega * time);
  prime_harmonic += 0.05f * std::cos(7.0f * omega * time);

  // Combine with nonlinear modulation
  float modulation = 0.5f * (1.0f + base_osc) + 0.2f * prime_harmonic;

  // Ensure positive capacitance
  return config.capacitance_base * (0.5f + 0.5f * modulation);
}

void Hinductor::generate_magnetic_light() { emit_magnetic_light(); }

void Hinductor::emit_magnetic_light() {
  // Calculate light emission based on vortex dynamics
  // I ∝ |dΦ/dt|² (Faraday induction analog)

  float dflux_dt = 0.0f;
  if (state.flux_history.size() >= 2) {
    size_t n = state.flux_history.size();
    dflux_dt = state.flux_history[n - 1] - state.flux_history[n - 2];
  }

  state.light_intensity = std::pow(std::abs(dflux_dt), 2.0f);

  // Frequency modulation based on vortex charge
  state.light_frequency =
      MAGNETIC_LIGHT_FREQUENCY * (1.0f + 0.01f * state.vortex_charge);

  // Update polarization based on flux direction
  float flux_angle = std::atan2(state.magnetic_flux, 1.0f);
  state.polarization[0] = std::cos(flux_angle);
  state.polarization[1] = std::sin(flux_angle);
  state.polarization[2] = 0.0f;
}

void Hinductor::update_state(float flux) {
  state.magnetic_flux = flux;
  state.resistance = calculate_vortex_resistance(flux);

  // Update hysteresis history
  if (config.enable_hysteresis) {
    state.flux_history.push_back(flux);
    state.resistance_history.push_back(state.resistance);

    // Limit history length
    while (state.flux_history.size() >
           static_cast<size_t>(config.history_length)) {
      state.flux_history.erase(state.flux_history.begin());
      state.resistance_history.erase(state.resistance_history.begin());
    }
  }

  // Power dissipation (assuming unit current)
  state.power_dissipation = state.resistance; // P = I²R with I=1

  // Update vortex charge
  state.vortex_charge += 0.1f * std::sin(flux);
}

void Hinductor::update_hysteresis(float flux) { update_state(flux); }

float Hinductor::compute_hysteresis_area() const {
  if (state.flux_history.size() < 3)
    return 0.0f;

  // Compute enclosed area using shoelace formula
  float area = 0.0f;
  size_t n = state.flux_history.size();

  for (size_t i = 0; i < n; i++) {
    size_t j = (i + 1) % n;
    area += state.flux_history[i] * state.resistance_history[j];
    area -= state.flux_history[j] * state.resistance_history[i];
  }

  return std::abs(area) / 2.0f;
}

void Hinductor::inject_vortex(float circulation) {
  state.vortex_charge += circulation;
}

void Hinductor::annihilate_vortex() { state.vortex_charge = 0.0f; }

void Hinductor::update_config(const HinductorConfig &new_config) {
  config = new_config;
}

// ================================================================
// Magnetic Knot Generator Implementation
// ================================================================

MagneticKnotGenerator::MagneticKnotGenerator(NanoBrainKernel *kernel,
                                             const KnotGeneratorConfig &config)
    : kernel(kernel), config(config), knot_counter(0) {
  init_knot_names();
}

MagneticKnotGenerator::~MagneticKnotGenerator() {
  // Tensor cleanup handled by kernel
}

void MagneticKnotGenerator::init_knot_names() {
  knot_names[KnotType::Trefoil] = "Trefoil Knot (3₁)";
  knot_names[KnotType::FigureEight] = "Figure-Eight Knot (4₁)";
  knot_names[KnotType::Cinquefoil] = "Cinquefoil Knot (5₁)";
  knot_names[KnotType::ThreeTwist] = "Three-Twist Knot (5₂)";
  knot_names[KnotType::Granny] = "Granny Knot";
  knot_names[KnotType::Square] = "Square Knot";
  knot_names[KnotType::Borromean] = "Borromean Rings";
  knot_names[KnotType::Hopf] = "Hopf Link";
  knot_names[KnotType::Whitehead] = "Whitehead Link";
}

MagneticKnot MagneticKnotGenerator::create_base_knot(KnotType type) {
  MagneticKnot knot;
  knot.id = "knot_" + std::to_string(knot_counter++);
  knot.type = type;
  knot.name = knot_names.count(type) ? knot_names[type] : "Unknown Knot";

  // Initialize topological invariants based on type
  switch (type) {
  case KnotType::Trefoil:
    knot.crossing_number = 3;
    knot.writhe = 3; // Right-handed trefoil
    break;
  case KnotType::FigureEight:
    knot.crossing_number = 4;
    knot.writhe = 0; // Achiral
    break;
  case KnotType::Cinquefoil:
    knot.crossing_number = 5;
    knot.writhe = 5;
    break;
  case KnotType::ThreeTwist:
    knot.crossing_number = 5;
    knot.writhe = -1;
    break;
  default:
    knot.crossing_number = 0;
    knot.writhe = 0;
  }

  knot.linking_number = 0;
  knot.rope_length = 2.0f * M_PI; // Will be updated
  knot.curvature = 1.0f;
  knot.torsion = 0.5f;
  knot.energy = 0.0f;
  knot.magnetic_flux = 0.0f;
  knot.current_density = 1.0f;
  knot.inductance = 1.0e-9f;
  knot.embedding = nullptr;
  knot.field = nullptr;

  return knot;
}

std::vector<std::array<float, 3>>
MagneticKnotGenerator::generate_trefoil_curve() {
  std::vector<std::array<float, 3>> points;
  points.reserve(config.resolution);

  for (int i = 0; i < config.resolution; i++) {
    float t = 2.0f * M_PI * i / config.resolution;

    // Trefoil parametric equations: (2+cos(3t))cos(2t), (2+cos(3t))sin(2t),
    // sin(3t)
    std::array<float, 3> p;
    p[0] = (2.0f + std::cos(3.0f * t)) * std::cos(2.0f * t);
    p[1] = (2.0f + std::cos(3.0f * t)) * std::sin(2.0f * t);
    p[2] = std::sin(3.0f * t);

    points.push_back(p);
  }

  return points;
}

std::vector<std::array<float, 3>>
MagneticKnotGenerator::generate_figure_eight_curve() {
  std::vector<std::array<float, 3>> points;
  points.reserve(config.resolution);

  for (int i = 0; i < config.resolution; i++) {
    float t = 2.0f * M_PI * i / config.resolution;

    // Figure-8 parametric: (2+cos(2t))cos(3t), (2+cos(2t))sin(3t), sin(4t)
    std::array<float, 3> p;
    p[0] = (2.0f + std::cos(2.0f * t)) * std::cos(3.0f * t);
    p[1] = (2.0f + std::cos(2.0f * t)) * std::sin(3.0f * t);
    p[2] = std::sin(4.0f * t);

    points.push_back(p);
  }

  return points;
}

std::vector<std::array<float, 3>>
MagneticKnotGenerator::generate_cinquefoil_curve() {
  std::vector<std::array<float, 3>> points;
  points.reserve(config.resolution);

  for (int i = 0; i < config.resolution; i++) {
    float t = 2.0f * M_PI * i / config.resolution;

    // Cinquefoil (5,2) torus knot
    std::array<float, 3> p;
    p[0] = (2.0f + std::cos(5.0f * t)) * std::cos(2.0f * t);
    p[1] = (2.0f + std::cos(5.0f * t)) * std::sin(2.0f * t);
    p[2] = std::sin(5.0f * t);

    points.push_back(p);
  }

  return points;
}

MagneticKnot MagneticKnotGenerator::generate_spin_knot(KnotType type) {
  MagneticKnot knot = create_base_knot(type);

  // Generate curve based on type
  std::vector<std::array<float, 3>> curve;
  switch (type) {
  case KnotType::Trefoil:
    curve = generate_trefoil_curve();
    break;
  case KnotType::FigureEight:
    curve = generate_figure_eight_curve();
    break;
  case KnotType::Cinquefoil:
    curve = generate_cinquefoil_curve();
    break;
  default:
    curve = generate_trefoil_curve(); // Default to trefoil
  }

  // Create embedding tensor (N x 3)
  knot.embedding = kernel->create_tensor({static_cast<int>(curve.size()), 3});
  std::vector<float> embed_data;
  embed_data.reserve(curve.size() * 3);
  for (const auto &p : curve) {
    embed_data.push_back(p[0]);
    embed_data.push_back(p[1]);
    embed_data.push_back(p[2]);
  }
  kernel->set_data(knot.embedding, embed_data);

  // Compute rope length (arc length)
  float length = 0.0f;
  for (size_t i = 0; i < curve.size(); i++) {
    size_t j = (i + 1) % curve.size();
    float dx = curve[j][0] - curve[i][0];
    float dy = curve[j][1] - curve[i][1];
    float dz = curve[j][2] - curve[i][2];
    length += std::sqrt(dx * dx + dy * dy + dz * dz);
  }
  knot.rope_length = length;

  // Compute energy if requested
  if (config.optimize_energy) {
    knot.energy = compute_knot_energy(knot);
  }

  // Compute magnetic field if requested
  if (config.compute_field) {
    knot.field = compute_magnetic_field(knot);
  }

  return knot;
}

MagneticKnot MagneticKnotGenerator::generate_custom_knot(
    const std::vector<float> &crossing_sequence) {
  // Create a knot from crossing sequence
  MagneticKnot knot = create_base_knot(KnotType::Unknown);
  knot.crossing_number = static_cast<int>(crossing_sequence.size());
  knot.name =
      "Custom Knot (" + std::to_string(knot.crossing_number) + " crossings)";

  // Generate approximate curve from crossings
  std::vector<std::array<float, 3>> curve;
  curve.reserve(config.resolution);

  for (int i = 0; i < config.resolution; i++) {
    float t = 2.0f * M_PI * i / config.resolution;
    std::array<float, 3> p;
    p[0] = std::cos(t);
    p[1] = std::sin(t);
    p[2] = 0.0f;

    // Add crossing perturbations
    for (size_t j = 0; j < crossing_sequence.size(); j++) {
      float phase = 2.0f * M_PI * j / crossing_sequence.size();
      p[2] += crossing_sequence[j] * 0.2f *
              std::sin(knot.crossing_number * t + phase);
    }

    curve.push_back(p);
  }

  // Create tensor as for standard knots
  knot.embedding = kernel->create_tensor({static_cast<int>(curve.size()), 3});
  // ... populate tensor

  return knot;
}

MagneticKnot MagneticKnotGenerator::synthesize_super_coil(
    const std::vector<MagneticKnot> &knots) {
  if (knots.empty()) {
    return create_base_knot(KnotType::Unknown);
  }

  // Create super-coil by combining knots
  MagneticKnot super_coil = create_base_knot(KnotType::Unknown);
  super_coil.name =
      "Super-coil (" + std::to_string(knots.size()) + " components)";

  // Accumulate topological properties
  int total_crossings = 0;
  int total_writhe = 0;
  float total_length = 0.0f;
  float total_energy = 0.0f;

  for (const auto &k : knots) {
    total_crossings += k.crossing_number;
    total_writhe += k.writhe;
    total_length += k.rope_length;
    total_energy += k.energy;
  }

  super_coil.crossing_number = total_crossings;
  super_coil.writhe = total_writhe;
  super_coil.linking_number = static_cast<int>(knots.size()) - 1; // Simplified
  super_coil.rope_length = total_length;
  super_coil.energy = total_energy;

  return super_coil;
}

MagneticKnot MagneticKnotGenerator::compose_knots(const MagneticKnot &k1,
                                                  const MagneticKnot &k2) {
  MagneticKnot composed = create_base_knot(KnotType::Unknown);
  composed.name = "Composed: " + k1.name + " # " + k2.name;
  composed.crossing_number = k1.crossing_number + k2.crossing_number;
  composed.writhe = k1.writhe + k2.writhe;
  composed.rope_length = k1.rope_length + k2.rope_length;
  composed.energy = k1.energy + k2.energy; // Approximate

  return composed;
}

VortexAtom MagneticKnotGenerator::create_vortex_atom(const MagneticKnot &knot) {
  VortexAtom atom;
  atom.id = "vortex_from_" + knot.id;
  atom.source_knot = knot;

  // Derive vortex properties from knot
  atom.circulation = 2.0f * M_PI * knot.writhe; // Quantized circulation
  atom.core_radius = config.rope_thickness;
  atom.vorticity_magnitude =
      std::abs(atom.circulation) / (M_PI * atom.core_radius * atom.core_radius);

  // Axis from knot principal direction (simplified)
  atom.vortex_axis = {0.0f, 0.0f, 1.0f};

  // Phase derived from knot topology
  atom.phase = std::fmod(knot.writhe * M_PI / 3.0f, 2.0f * M_PI);
  atom.phase_velocity = knot.energy > 0 ? 1.0f / std::sqrt(knot.energy) : 0.1f;

  // Quantum-like spinor
  atom.spinor[0] = std::cos(atom.phase / 2.0f);
  atom.spinor[1] = std::sin(atom.phase / 2.0f);

  // Time crystal coherence
  atom.tc_coherence = 1.0f / (1.0f + knot.crossing_number * 0.1f);
  atom.prime_resonance = {2, 3, 5}; // Default prime resonance

  return atom;
}

std::vector<VortexAtom>
MagneticKnotGenerator::create_vortex_atoms(const MagneticKnot &knot,
                                           int count) {
  std::vector<VortexAtom> atoms;
  atoms.reserve(count);

  for (int i = 0; i < count; i++) {
    VortexAtom atom = create_vortex_atom(knot);
    atom.id += "_" + std::to_string(i);
    atom.phase += 2.0f * M_PI * i / count;
    atom.spinor[0] = std::cos(atom.phase / 2.0f);
    atom.spinor[1] = std::sin(atom.phase / 2.0f);
    atoms.push_back(atom);
  }

  return atoms;
}

float MagneticKnotGenerator::compute_knot_energy(const MagneticKnot &knot) {
  // Simplified Möbius energy calculation
  // E = ∫∫ (1/|x-y|² - 1/D(x,y)²) dx dy
  // Use crossing number as proxy
  return std::pow(knot.crossing_number, 1.5f) * 10.0f;
}

float MagneticKnotGenerator::compute_writhe(const MagneticKnot &knot) {
  return static_cast<float>(knot.writhe);
}

bool MagneticKnotGenerator::are_isotopic(const MagneticKnot &k1,
                                         const MagneticKnot &k2) {
  // Simple check: same type and crossing number
  return k1.type == k2.type && k1.crossing_number == k2.crossing_number;
}

NanoBrainTensor *
MagneticKnotGenerator::compute_magnetic_field(const MagneticKnot &knot) {
  // Create field tensor (grid resolution x 3)
  int grid_size = 32;
  NanoBrainTensor *field =
      kernel->create_tensor({grid_size, grid_size, grid_size, 3});

  // Simplified Biot-Savart calculation would go here
  // For now, return zero-initialized tensor
  std::vector<float> field_data(grid_size * grid_size * grid_size * 3, 0.0f);
  kernel->set_data(field, field_data);

  return field;
}

// ================================================================
// Tomasch Oscillator Implementation
// ================================================================

TomaschOscillator::TomaschOscillator(NanoBrainKernel *kernel,
                                     const TomaschConfig &config)
    : kernel(kernel), config(config), harvested_energy(0.0f),
      accumulated_signal(0.0f), current_efficiency(0.0f),
      signal_to_noise(0.0f) {}

TomaschOscillator::~TomaschOscillator() {}

float TomaschOscillator::harvest_noise_energy(float noise_amplitude) {
  if (noise_amplitude < config.noise_threshold) {
    return 0.0f;
  }

  // Rectify and filter the noise
  float rectified = rectify_signal(noise_amplitude);
  float filtered = apply_filter(rectified);

  // Harvest energy with efficiency
  float harvested = filtered * config.harvest_efficiency;
  harvested_energy += harvested;
  accumulated_signal += filtered;

  // Update SNR estimate
  signal_to_noise = filtered / (noise_amplitude + 1e-6f);
  current_efficiency = harvested / (noise_amplitude + 1e-6f);

  return harvested;
}

float TomaschOscillator::harvest_from_quantum_state(
    NanoBrainTensor *state_tensor) {
  if (!state_tensor || !state_tensor->ggml_tensor) {
    return 0.0f;
  }

  // Compute state tensor and extract energy
  kernel->compute(state_tensor);

  // Sum squared amplitudes (|ψ|² energy)
  float total_energy = 0.0f;
  float *data = (float *)state_tensor->ggml_tensor->data;
  int64_t size = ggml_nelements(state_tensor->ggml_tensor);

  for (int64_t i = 0; i < size; i++) {
    total_energy += data[i] * data[i];
  }

  // Harvest a fraction
  float harvested = total_energy * config.harvest_efficiency * 0.01f;
  harvested_energy += harvested;

  return harvested;
}

void TomaschOscillator::process_thermal_waves(float temperature) {
  // Thermal energy E = k_B * T (normalized)
  float thermal_energy = temperature / 300.0f; // Normalize to room temp

  // Convert thermal fluctuations to harvested energy
  float noise = thermal_energy * std::sin(temperature * 0.1f);
  harvest_noise_energy(std::abs(noise));
}

float TomaschOscillator::apply_filter(float input) {
  // Simple low-pass approximation
  static float prev_output = 0.0f;
  float alpha = 0.1f; // Filter coefficient
  float output = alpha * input + (1.0f - alpha) * prev_output;
  prev_output = output;
  return output;
}

float TomaschOscillator::rectify_signal(float signal) {
  return std::abs(signal);
}

void TomaschOscillator::reset_accumulator() {
  harvested_energy = 0.0f;
  accumulated_signal = 0.0f;
}

void TomaschOscillator::update_config(const TomaschConfig &new_config) {
  config = new_config;
}

// ================================================================
// Phase Space Dynamics Implementation
// ================================================================

PhaseSpaceDynamics::PhaseSpaceDynamics(NanoBrainKernel *kernel,
                                       const PhaseSpaceConfig &config)
    : kernel(kernel), config(config), global_phase(0.0f),
      tc_coupling_strength(0.0f), current_time(0.0f) {
  initialize_holes();
}

PhaseSpaceDynamics::~PhaseSpaceDynamics() {}

void PhaseSpaceDynamics::initialize_holes() {
  // Assign prime-based frequencies to each hole
  const std::array<int, 12> primes = {2,  3,  5,  7,  11, 13,
                                      17, 19, 23, 29, 31, 37};

  for (int i = 0; i < PHASE_SPACE_HOLES; i++) {
    holes[i].index = i;
    holes[i].state = (i % 2 == 0) ? HoleState::Active : HoleState::Inactive;
    holes[i].phase = 2.0f * M_PI * i / PHASE_SPACE_HOLES;
    holes[i].blink_frequency = static_cast<float>(primes[i]);
    holes[i].amplitude = 1.0f;
    holes[i].last_transition_time = 0.0f;
  }
}

void PhaseSpaceDynamics::update_hole_states(float delta_time) {
  current_time += delta_time;

  // Update each hole independently
  for (int i = 0; i < PHASE_SPACE_HOLES; i++) {
    update_single_hole(i, delta_time);
  }

  // Apply inter-hole coupling
  apply_coupling(delta_time);

  // Update global phase
  global_phase = std::fmod(global_phase + delta_time, 2.0f * M_PI);
}

void PhaseSpaceDynamics::update_single_hole(int index, float delta_time) {
  auto &hole = holes[index];

  // Update phase based on frequency
  hole.phase += 2.0f * M_PI * hole.blink_frequency * delta_time;
  hole.phase = std::fmod(hole.phase, 2.0f * M_PI);

  // Determine state based on phase (blink pattern)
  float threshold = std::sin(hole.phase);

  HoleState new_state;
  if (threshold > 0.3f) {
    new_state = HoleState::Active;
  } else if (threshold < -0.3f) {
    new_state = HoleState::Inactive;
  } else {
    new_state = HoleState::Transition;
  }

  if (new_state != hole.state) {
    hole.last_transition_time = current_time;
    hole.state = new_state;
  }

  // Damping
  hole.amplitude *= (1.0f - config.damping * delta_time);
  hole.amplitude = std::max(0.1f, hole.amplitude);
}

void PhaseSpaceDynamics::apply_coupling(float delta_time) {
  // Couple adjacent holes
  for (int i = 0; i < PHASE_SPACE_HOLES; i++) {
    int prev = (i + PHASE_SPACE_HOLES - 1) % PHASE_SPACE_HOLES;
    int next = (i + 1) % PHASE_SPACE_HOLES;

    // Phase synchronization tendency
    float phase_diff_prev = holes[prev].phase - holes[i].phase;
    float phase_diff_next = holes[next].phase - holes[i].phase;

    holes[i].phase += config.coupling_strength * delta_time *
                      (std::sin(phase_diff_prev) + std::sin(phase_diff_next));
  }
}

float PhaseSpaceDynamics::get_oscillation_phase() const {
  // Compute collective phase from all holes
  float sin_sum = 0.0f;
  float cos_sum = 0.0f;

  for (const auto &hole : holes) {
    if (hole.state == HoleState::Active) {
      sin_sum += std::sin(hole.phase);
      cos_sum += std::cos(hole.phase);
    }
  }

  return std::atan2(sin_sum, cos_sum);
}

HoleState PhaseSpaceDynamics::get_hole_state(int index) const {
  if (index >= 0 && index < PHASE_SPACE_HOLES) {
    return holes[index].state;
  }
  return HoleState::Inactive;
}

int PhaseSpaceDynamics::count_active_holes() const {
  int count = 0;
  for (const auto &hole : holes) {
    if (hole.state == HoleState::Active) {
      count++;
    }
  }
  return count;
}

std::vector<int> PhaseSpaceDynamics::get_active_pattern() const {
  std::vector<int> pattern;
  for (int i = 0; i < PHASE_SPACE_HOLES; i++) {
    if (holes[i].state == HoleState::Active) {
      pattern.push_back(i);
    }
  }
  return pattern;
}

float PhaseSpaceDynamics::compute_pattern_coherence() const {
  // Coherence based on uniformity of active hole spacing
  auto pattern = get_active_pattern();
  if (pattern.size() < 2)
    return 0.0f;

  std::vector<int> gaps;
  for (size_t i = 0; i < pattern.size(); i++) {
    int next = (i + 1) % pattern.size();
    int gap =
        (pattern[next] - pattern[i] + PHASE_SPACE_HOLES) % PHASE_SPACE_HOLES;
    gaps.push_back(gap);
  }

  // Compute variance of gaps
  float mean_gap = static_cast<float>(PHASE_SPACE_HOLES) / pattern.size();
  float variance = 0.0f;
  for (int gap : gaps) {
    float diff = gap - mean_gap;
    variance += diff * diff;
  }
  variance /= gaps.size();

  // Convert to coherence (low variance = high coherence)
  return 1.0f / (1.0f + variance);
}

bool PhaseSpaceDynamics::is_resonant() const {
  // Check if active holes form resonant pattern
  int active = count_active_holes();

  // Resonance at prime counts: 2, 3, 5, 7, 11
  return active == 2 || active == 3 || active == 5 || active == 7 ||
         active == 11;
}

void PhaseSpaceDynamics::couple_to_time_crystal(NanoBrainTensor *tc_state) {
  if (!tc_state || !tc_state->ggml_tensor) {
    tc_coupling_strength = 0.0f;
    return;
  }

  kernel->compute(tc_state);

  // Extract coherence from time crystal state
  float *data = (float *)tc_state->ggml_tensor->data;
  int64_t size = std::min(int64_t(11), ggml_nelements(tc_state->ggml_tensor));

  float sum = 0.0f;
  for (int64_t i = 0; i < size; i++) {
    sum += std::abs(data[i]);
  }

  tc_coupling_strength = sum / size;
}

std::string PhaseSpaceDynamics::visualize_state() const {
  std::ostringstream oss;
  oss << "[ ";
  for (int i = 0; i < PHASE_SPACE_HOLES; i++) {
    switch (holes[i].state) {
    case HoleState::Active:
      oss << "●";
      break;
    case HoleState::Inactive:
      oss << "○";
      break;
    case HoleState::Transition:
      oss << "◐";
      break;
    }
    if (i < PHASE_SPACE_HOLES - 1)
      oss << " ";
  }
  oss << " ]";
  return oss.str();
}

// ================================================================
// Magnonic Bridge Implementation
// ================================================================

MagnonicBridge::MagnonicBridge(NanoBrainKernel *kernel,
                               const MagnonicBridgeConfig &config)
    : kernel(kernel), config(config), wireless_active(false),
      realized_efficiency(0.0f), energy_loss(0.0f), transition_count(0) {}

MagnonicBridge::~MagnonicBridge() {}

MagnonPacket
MagnonicBridge::transition_electron_to_magnon(float electron_energy,
                                              float spin_polarization) {
  MagnonPacket magnon;

  // Calculate transition parameters
  float loss = compute_transition_loss(electron_energy);
  float transferred_energy = electron_energy - loss;

  // Create magnon
  magnon.type =
      (transferred_energy > 1.0f) ? MagnonType::Optical : MagnonType::Acoustic;
  magnon.frequency = transferred_energy * 1.0e12f; // Energy to THz
  magnon.wavelength = 3.0e8f / magnon.frequency;   // c/f approximation
  magnon.amplitude = std::sqrt(transferred_energy);
  magnon.phase = spin_polarization * M_PI;
  magnon.group_velocity = 1.0e3f; // ~km/s for magnons
  magnon.propagation_direction = {1.0f, 0.0f, 0.0f};
  magnon.spin_angular_momentum = spin_polarization;

  // Update statistics
  energy_loss += loss;
  realized_efficiency = transferred_energy / (electron_energy + 1e-10f);
  transition_count++;

  return magnon;
}

float MagnonicBridge::transition_magnon_to_electron(
    const MagnonPacket &magnon) {
  // Reverse transition
  float magnon_energy = magnon.amplitude * magnon.amplitude;
  float loss = compute_transition_loss(magnon_energy);

  energy_loss += loss;
  transition_count++;

  return magnon_energy - loss;
}

float MagnonicBridge::compute_transition_loss(float energy) {
  // Loss proportional to energy, modified by efficiency
  return energy * (1.0f - config.transition_efficiency);
}

void MagnonicBridge::setup_wireless_h_device() {
  wireless_active = true;
  transmit_buffer.clear();
  receive_buffer.clear();

  std::cout << "[MagnonicBridge] Wireless H-device activated at "
            << config.carrier_frequency / 1.0e9f << " GHz" << std::endl;
}

void MagnonicBridge::transmit_magnon(const MagnonPacket &magnon) {
  if (!wireless_active) {
    std::cerr << "[MagnonicBridge] Wireless not active" << std::endl;
    return;
  }

  transmit_buffer.push_back(magnon);
}

MagnonPacket MagnonicBridge::receive_magnon() {
  if (receive_buffer.empty()) {
    // Return empty packet
    MagnonPacket empty;
    empty.amplitude = 0.0f;
    return empty;
  }

  MagnonPacket received = receive_buffer.front();
  receive_buffer.erase(receive_buffer.begin());
  return received;
}

MagnonPacket MagnonicBridge::create_magnon(MagnonType type, float frequency,
                                           float amplitude) {
  MagnonPacket magnon;
  magnon.type = type;
  magnon.frequency = frequency;
  magnon.wavelength = 3.0e8f / frequency;
  magnon.amplitude = amplitude;
  magnon.phase = 0.0f;
  magnon.group_velocity = 1.0e3f;
  magnon.propagation_direction = {1.0f, 0.0f, 0.0f};
  magnon.spin_angular_momentum = 1.0f;

  return magnon;
}

std::vector<MagnonPacket>
MagnonicBridge::split_magnon(const MagnonPacket &magnon, int count) {
  std::vector<MagnonPacket> result;
  if (count <= 0)
    return result;

  float amp_per = magnon.amplitude / std::sqrt(static_cast<float>(count));

  for (int i = 0; i < count; i++) {
    MagnonPacket split = magnon;
    split.amplitude = amp_per;
    split.phase += 2.0f * M_PI * i / count;
    result.push_back(split);
  }

  return result;
}

MagnonPacket
MagnonicBridge::combine_magnons(const std::vector<MagnonPacket> &magnons) {
  if (magnons.empty()) {
    return create_magnon(MagnonType::Acoustic, 1.0e9f, 0.0f);
  }

  // Coherent combination (assuming same frequency)
  float total_amp_sq = 0.0f;
  float freq_sum = 0.0f;

  for (const auto &m : magnons) {
    total_amp_sq += m.amplitude * m.amplitude;
    freq_sum += m.frequency;
  }

  MagnonPacket combined = magnons[0];
  combined.amplitude = std::sqrt(total_amp_sq);
  combined.frequency = freq_sum / magnons.size();

  return combined;
}
