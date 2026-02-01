#include "nanobrain_persistence.h"
#include <algorithm>
#include <chrono>
#include <cstring>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace fs = std::filesystem;

// ================================================================
// Magic bytes for file identification
// ================================================================

constexpr char NANOBRAIN_MAGIC[4] = {'N', 'B', 'A', 'S'}; // NanoBrain AtomSpace

// ================================================================
// AtomSpacePersistence Implementation
// ================================================================

AtomSpacePersistence::AtomSpacePersistence(const PersistenceConfig &config)
    : config(config) {}

// ================================================================
// Save Operations
// ================================================================

SerializationResult
AtomSpacePersistence::save_to_file(const TimeCrystalKernel *kernel,
                                   const std::string &filepath) {
  SerializationResult result;
  result.success = false;
  result.bytes_written = 0;
  result.atoms_serialized = 0;
  result.inferences_serialized = 0;

  if (!kernel) {
    result.error_message = "Null kernel pointer";
    return result;
  }

  std::ofstream file(filepath, std::ios::binary);
  if (!file.is_open()) {
    result.error_message = "Failed to open file for writing: " + filepath;
    return result;
  }

  try {
    auto atom_ids = kernel->get_all_atom_ids();
    auto inference_ids = kernel->get_all_inference_ids();

    // Write header
    write_header(file, atom_ids.size(), inference_ids.size());

    // Write atoms
    for (const auto &id : atom_ids) {
      const auto *atom = kernel->get_atom(id);
      if (atom) {
        write_atom(file, *atom);
        result.atoms_serialized++;
      }
    }

    // Write inferences
    for (const auto &id : inference_ids) {
      const auto *inference = kernel->get_inference(id);
      if (inference) {
        write_inference(file, *inference);
        result.inferences_serialized++;
      }
    }

    result.bytes_written = file.tellp();
    result.success = true;

  } catch (const std::exception &e) {
    result.error_message = std::string("Serialization error: ") + e.what();
  }

  file.close();
  return result;
}

SerializationResult
AtomSpacePersistence::save_to_buffer(const TimeCrystalKernel *kernel,
                                     std::vector<uint8_t> &buffer) {
  SerializationResult result;
  result.success = false;

  if (!kernel) {
    result.error_message = "Null kernel pointer";
    return result;
  }

  std::ostringstream oss(std::ios::binary);

  try {
    auto atom_ids = kernel->get_all_atom_ids();
    auto inference_ids = kernel->get_all_inference_ids();

    write_header(oss, atom_ids.size(), inference_ids.size());

    for (const auto &id : atom_ids) {
      const auto *atom = kernel->get_atom(id);
      if (atom) {
        write_atom(oss, *atom);
        result.atoms_serialized++;
      }
    }

    for (const auto &id : inference_ids) {
      const auto *inference = kernel->get_inference(id);
      if (inference) {
        write_inference(oss, *inference);
        result.inferences_serialized++;
      }
    }

    std::string str = oss.str();
    buffer.assign(str.begin(), str.end());
    result.bytes_written = buffer.size();
    result.success = true;

  } catch (const std::exception &e) {
    result.error_message = std::string("Serialization error: ") + e.what();
  }

  return result;
}

SerializationResult
AtomSpacePersistence::export_to_json(const TimeCrystalKernel *kernel,
                                     const std::string &filepath) {
  SerializationResult result;
  result.success = false;

  if (!kernel) {
    result.error_message = "Null kernel pointer";
    return result;
  }

  std::ofstream file(filepath);
  if (!file.is_open()) {
    result.error_message = "Failed to open file: " + filepath;
    return result;
  }

  try {
    auto atom_ids = kernel->get_all_atom_ids();
    auto inference_ids = kernel->get_all_inference_ids();

    file << "{\n";
    file << "  \"version\": " << NANOBRAIN_PERSISTENCE_VERSION << ",\n";
    file << "  \"timestamp\": "
         << std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch())
                .count()
         << ",\n";

    // Write atoms
    file << "  \"atoms\": [\n";
    bool first = true;
    for (const auto &id : atom_ids) {
      const auto *atom = kernel->get_atom(id);
      if (atom) {
        if (!first)
          file << ",\n";
        file << "    " << atom_to_json(*atom);
        first = false;
        result.atoms_serialized++;
      }
    }
    file << "\n  ],\n";

    // Write inferences
    file << "  \"inferences\": [\n";
    first = true;
    for (const auto &id : inference_ids) {
      const auto *inference = kernel->get_inference(id);
      if (inference) {
        if (!first)
          file << ",\n";
        file << "    " << inference_to_json(*inference);
        first = false;
        result.inferences_serialized++;
      }
    }
    file << "\n  ]\n";
    file << "}\n";

    result.bytes_written = file.tellp();
    result.success = true;

  } catch (const std::exception &e) {
    result.error_message = std::string("JSON export error: ") + e.what();
  }

  file.close();
  return result;
}

// ================================================================
// Load Operations
// ================================================================

DeserializationResult
AtomSpacePersistence::load_from_file(TimeCrystalKernel *kernel,
                                     const std::string &filepath) {
  DeserializationResult result;
  result.success = false;
  result.bytes_read = 0;
  result.atoms_loaded = 0;
  result.inferences_loaded = 0;

  if (!kernel) {
    result.error_message = "Null kernel pointer";
    return result;
  }

  std::ifstream file(filepath, std::ios::binary);
  if (!file.is_open()) {
    result.error_message = "Failed to open file: " + filepath;
    return result;
  }

  try {
    uint32_t version;
    size_t atom_count, inference_count;

    if (!read_header(file, version, atom_count, inference_count)) {
      result.error_message = "Invalid file header";
      return result;
    }

    result.file_version = version;

    if (version > NANOBRAIN_PERSISTENCE_VERSION) {
      result.error_message = "File version newer than supported";
      return result;
    }

    // Read atoms
    for (size_t i = 0; i < atom_count; i++) {
      TimeCrystalAtom atom = read_atom(file);

      // Create atom in kernel
      kernel->create_atom(atom.type, atom.name, atom.truth_value,
                          atom.attention_value, atom.prime_encoding,
                          atom.fractal_geometry);
      result.atoms_loaded++;
    }

    // Read inferences
    for (size_t i = 0; i < inference_count; i++) {
      TimeCrystalInference inference = read_inference(file);
      // Note: Inferences are typically regenerated by PLN reasoning
      // but we track how many were in the file
      result.inferences_loaded++;
    }

    result.bytes_read = file.tellg();
    result.success = true;

  } catch (const std::exception &e) {
    result.error_message = std::string("Deserialization error: ") + e.what();
  }

  file.close();
  return result;
}

DeserializationResult
AtomSpacePersistence::load_from_buffer(TimeCrystalKernel *kernel,
                                       const std::vector<uint8_t> &buffer) {
  DeserializationResult result;
  result.success = false;

  if (!kernel) {
    result.error_message = "Null kernel pointer";
    return result;
  }

  if (buffer.empty()) {
    result.error_message = "Empty buffer";
    return result;
  }

  std::string str(buffer.begin(), buffer.end());
  std::istringstream iss(str, std::ios::binary);

  try {
    uint32_t version;
    size_t atom_count, inference_count;

    if (!read_header(iss, version, atom_count, inference_count)) {
      result.error_message = "Invalid buffer header";
      return result;
    }

    result.file_version = version;

    for (size_t i = 0; i < atom_count; i++) {
      TimeCrystalAtom atom = read_atom(iss);
      kernel->create_atom(atom.type, atom.name, atom.truth_value,
                          atom.attention_value, atom.prime_encoding,
                          atom.fractal_geometry);
      result.atoms_loaded++;
    }

    for (size_t i = 0; i < inference_count; i++) {
      read_inference(iss);
      result.inferences_loaded++;
    }

    result.bytes_read = buffer.size();
    result.success = true;

  } catch (const std::exception &e) {
    result.error_message = std::string("Buffer load error: ") + e.what();
  }

  return result;
}

DeserializationResult
AtomSpacePersistence::import_from_json(TimeCrystalKernel *kernel,
                                       const std::string &filepath) {
  DeserializationResult result;
  result.success = false;
  result.error_message = "JSON import not yet implemented";
  // TODO: Implement JSON parser
  return result;
}

// ================================================================
// Utilities
// ================================================================

bool AtomSpacePersistence::validate_file(const std::string &filepath) {
  std::ifstream file(filepath, std::ios::binary);
  if (!file.is_open())
    return false;

  char magic[4];
  file.read(magic, 4);
  return std::memcmp(magic, NANOBRAIN_MAGIC, 4) == 0;
}

AtomSpacePersistence::FileInfo
AtomSpacePersistence::get_file_info(const std::string &filepath) {
  FileInfo info;
  info.version = 0;
  info.atom_count = 0;
  info.inference_count = 0;
  info.creation_time = 0;

  std::ifstream file(filepath, std::ios::binary);
  if (!file.is_open())
    return info;

  uint32_t version;
  size_t atom_count, inference_count;

  if (read_header(file, version, atom_count, inference_count)) {
    info.version = version;
    info.atom_count = atom_count;
    info.inference_count = inference_count;
    file.read(reinterpret_cast<char *>(&info.creation_time),
              sizeof(info.creation_time));
    info.description = read_string(file);
  }

  return info;
}

// ================================================================
// Binary Serialization Helpers
// ================================================================

void AtomSpacePersistence::write_header(std::ostream &out, size_t atom_count,
                                        size_t inference_count) {
  // Magic bytes
  out.write(NANOBRAIN_MAGIC, 4);

  // Version
  uint32_t version = NANOBRAIN_PERSISTENCE_VERSION;
  out.write(reinterpret_cast<const char *>(&version), sizeof(version));

  // Counts
  out.write(reinterpret_cast<const char *>(&atom_count), sizeof(atom_count));
  out.write(reinterpret_cast<const char *>(&inference_count),
            sizeof(inference_count));

  // Timestamp
  int64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                          std::chrono::system_clock::now().time_since_epoch())
                          .count();
  out.write(reinterpret_cast<const char *>(&timestamp), sizeof(timestamp));

  // Description (empty for now)
  write_string(out, "");
}

bool AtomSpacePersistence::read_header(std::istream &in, uint32_t &version,
                                       size_t &atom_count,
                                       size_t &inference_count) {
  char magic[4];
  in.read(magic, 4);
  if (std::memcmp(magic, NANOBRAIN_MAGIC, 4) != 0)
    return false;

  in.read(reinterpret_cast<char *>(&version), sizeof(version));
  in.read(reinterpret_cast<char *>(&atom_count), sizeof(atom_count));
  in.read(reinterpret_cast<char *>(&inference_count), sizeof(inference_count));

  return true;
}

void AtomSpacePersistence::write_atom(std::ostream &out,
                                      const TimeCrystalAtom &atom) {
  write_string(out, atom.id);
  write_string(out, atom.type);
  write_string(out, atom.name);

  // Truth value
  out.write(reinterpret_cast<const char *>(&atom.truth_value),
            sizeof(TruthValue));

  // Attention value
  out.write(reinterpret_cast<const char *>(&atom.attention_value),
            sizeof(AttentionValue));

  // Prime encoding
  write_int_vector(out, atom.prime_encoding);

  // Quantum state (if enabled)
  if (config.include_quantum_states) {
    write_quantum_state(out, atom.time_crystal_state);
  }

  // Fractal geometry
  int shape = static_cast<int>(atom.fractal_geometry.shape);
  out.write(reinterpret_cast<const char *>(&shape), sizeof(shape));
  out.write(reinterpret_cast<const char *>(&atom.fractal_geometry.dimensions),
            sizeof(int));
  write_string(out, atom.fractal_geometry.symmetry_group);
  int note = static_cast<int>(atom.fractal_geometry.musical_note);
  out.write(reinterpret_cast<const char *>(&note), sizeof(note));
  write_int_vector(out, atom.fractal_geometry.prime_resonance);
  out.write(reinterpret_cast<const char *>(&atom.fractal_geometry.scale_factor),
            sizeof(float));
}

TimeCrystalAtom AtomSpacePersistence::read_atom(std::istream &in) {
  TimeCrystalAtom atom;

  atom.id = read_string(in);
  atom.type = read_string(in);
  atom.name = read_string(in);

  in.read(reinterpret_cast<char *>(&atom.truth_value), sizeof(TruthValue));
  in.read(reinterpret_cast<char *>(&atom.attention_value),
          sizeof(AttentionValue));

  atom.prime_encoding = read_int_vector(in);

  if (config.include_quantum_states) {
    atom.time_crystal_state = read_quantum_state(in);
  }

  int shape;
  in.read(reinterpret_cast<char *>(&shape), sizeof(shape));
  atom.fractal_geometry.shape = static_cast<GMLShape>(shape);
  in.read(reinterpret_cast<char *>(&atom.fractal_geometry.dimensions),
          sizeof(int));
  atom.fractal_geometry.symmetry_group = read_string(in);
  int note;
  in.read(reinterpret_cast<char *>(&note), sizeof(note));
  atom.fractal_geometry.musical_note = static_cast<MusicalNote>(note);
  atom.fractal_geometry.prime_resonance = read_int_vector(in);
  in.read(reinterpret_cast<char *>(&atom.fractal_geometry.scale_factor),
          sizeof(float));

  return atom;
}

void AtomSpacePersistence::write_inference(std::ostream &out,
                                           const TimeCrystalInference &inf) {
  write_string(out, inf.id);

  // Premise IDs
  size_t count = inf.premise_ids.size();
  out.write(reinterpret_cast<const char *>(&count), sizeof(count));
  for (const auto &id : inf.premise_ids) {
    write_string(out, id);
  }

  write_string(out, inf.conclusion_id);
  write_string(out, inf.rule);

  out.write(reinterpret_cast<const char *>(&inf.temporal_flow), sizeof(float));
  out.write(reinterpret_cast<const char *>(&inf.quantum_coherence),
            sizeof(float));
  out.write(reinterpret_cast<const char *>(&inf.prime_consistency),
            sizeof(float));
  out.write(reinterpret_cast<const char *>(&inf.fractal_convergence),
            sizeof(float));
}

TimeCrystalInference AtomSpacePersistence::read_inference(std::istream &in) {
  TimeCrystalInference inf;

  inf.id = read_string(in);

  size_t count;
  in.read(reinterpret_cast<char *>(&count), sizeof(count));
  for (size_t i = 0; i < count; i++) {
    inf.premise_ids.push_back(read_string(in));
  }

  inf.conclusion_id = read_string(in);
  inf.rule = read_string(in);

  in.read(reinterpret_cast<char *>(&inf.temporal_flow), sizeof(float));
  in.read(reinterpret_cast<char *>(&inf.quantum_coherence), sizeof(float));
  in.read(reinterpret_cast<char *>(&inf.prime_consistency), sizeof(float));
  in.read(reinterpret_cast<char *>(&inf.fractal_convergence), sizeof(float));

  return inf;
}

void AtomSpacePersistence::write_quantum_state(
    std::ostream &out, const TimeCrystalQuantumState &state) {
  write_float_vector(out, state.dimensions);
  write_float_vector(out, state.prime_signature);
  out.write(reinterpret_cast<const char *>(&state.temporal_coherence),
            sizeof(float));
  out.write(reinterpret_cast<const char *>(&state.fractal_dimension),
            sizeof(float));
  out.write(reinterpret_cast<const char *>(&state.resonance_frequency),
            sizeof(float));
  out.write(reinterpret_cast<const char *>(&state.quantum_phase),
            sizeof(float));
}

TimeCrystalQuantumState
AtomSpacePersistence::read_quantum_state(std::istream &in) {
  TimeCrystalQuantumState state;
  state.dimensions = read_float_vector(in);
  state.prime_signature = read_float_vector(in);
  in.read(reinterpret_cast<char *>(&state.temporal_coherence), sizeof(float));
  in.read(reinterpret_cast<char *>(&state.fractal_dimension), sizeof(float));
  in.read(reinterpret_cast<char *>(&state.resonance_frequency), sizeof(float));
  in.read(reinterpret_cast<char *>(&state.quantum_phase), sizeof(float));
  return state;
}

void AtomSpacePersistence::write_string(std::ostream &out,
                                        const std::string &str) {
  size_t len = str.size();
  out.write(reinterpret_cast<const char *>(&len), sizeof(len));
  out.write(str.data(), len);
}

std::string AtomSpacePersistence::read_string(std::istream &in) {
  size_t len;
  in.read(reinterpret_cast<char *>(&len), sizeof(len));
  std::string str(len, '\0');
  in.read(&str[0], len);
  return str;
}

void AtomSpacePersistence::write_float_vector(std::ostream &out,
                                              const std::vector<float> &vec) {
  size_t len = vec.size();
  out.write(reinterpret_cast<const char *>(&len), sizeof(len));
  out.write(reinterpret_cast<const char *>(vec.data()), len * sizeof(float));
}

std::vector<float> AtomSpacePersistence::read_float_vector(std::istream &in) {
  size_t len;
  in.read(reinterpret_cast<char *>(&len), sizeof(len));
  std::vector<float> vec(len);
  in.read(reinterpret_cast<char *>(vec.data()), len * sizeof(float));
  return vec;
}

void AtomSpacePersistence::write_int_vector(std::ostream &out,
                                            const std::vector<int> &vec) {
  size_t len = vec.size();
  out.write(reinterpret_cast<const char *>(&len), sizeof(len));
  out.write(reinterpret_cast<const char *>(vec.data()), len * sizeof(int));
}

std::vector<int> AtomSpacePersistence::read_int_vector(std::istream &in) {
  size_t len;
  in.read(reinterpret_cast<char *>(&len), sizeof(len));
  std::vector<int> vec(len);
  in.read(reinterpret_cast<char *>(vec.data()), len * sizeof(int));
  return vec;
}

// ================================================================
// JSON Helpers
// ================================================================

std::string AtomSpacePersistence::atom_to_json(const TimeCrystalAtom &atom) {
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(6);

  oss << "{";
  oss << "\"id\":\"" << atom.id << "\",";
  oss << "\"type\":\"" << atom.type << "\",";
  oss << "\"name\":\"" << atom.name << "\",";
  oss << "\"truth_value\":{";
  oss << "\"strength\":" << atom.truth_value.strength << ",";
  oss << "\"confidence\":" << atom.truth_value.confidence << ",";
  oss << "\"count\":" << atom.truth_value.count << "},";
  oss << "\"attention_value\":{";
  oss << "\"sti\":" << atom.attention_value.sti << ",";
  oss << "\"lti\":" << atom.attention_value.lti << ",";
  oss << "\"vlti\":" << atom.attention_value.vlti << "},";
  oss << "\"prime_encoding\":[";
  for (size_t i = 0; i < atom.prime_encoding.size(); i++) {
    oss << atom.prime_encoding[i];
    if (i < atom.prime_encoding.size() - 1)
      oss << ",";
  }
  oss << "],";

  if (config.include_quantum_states) {
    oss << "\"time_crystal_state\":{";
    oss << "\"temporal_coherence\":"
        << atom.time_crystal_state.temporal_coherence << ",";
    oss << "\"fractal_dimension\":" << atom.time_crystal_state.fractal_dimension
        << ",";
    oss << "\"resonance_frequency\":"
        << atom.time_crystal_state.resonance_frequency << ",";
    oss << "\"quantum_phase\":" << atom.time_crystal_state.quantum_phase << ",";
    oss << "\"dimensions\":[";
    for (size_t i = 0; i < atom.time_crystal_state.dimensions.size(); i++) {
      oss << atom.time_crystal_state.dimensions[i];
      if (i < atom.time_crystal_state.dimensions.size() - 1)
        oss << ",";
    }
    oss << "]},";
  }

  oss << "\"fractal_geometry\":{";
  oss << "\"shape\":" << static_cast<int>(atom.fractal_geometry.shape) << ",";
  oss << "\"dimensions\":" << atom.fractal_geometry.dimensions << ",";
  oss << "\"symmetry_group\":\"" << atom.fractal_geometry.symmetry_group
      << "\",";
  oss << "\"musical_note\":"
      << static_cast<int>(atom.fractal_geometry.musical_note) << ",";
  oss << "\"scale_factor\":" << atom.fractal_geometry.scale_factor << "}";
  oss << "}";

  return oss.str();
}

std::string
AtomSpacePersistence::inference_to_json(const TimeCrystalInference &inf) {
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(6);

  oss << "{";
  oss << "\"id\":\"" << inf.id << "\",";
  oss << "\"premise_ids\":[";
  for (size_t i = 0; i < inf.premise_ids.size(); i++) {
    oss << "\"" << inf.premise_ids[i] << "\"";
    if (i < inf.premise_ids.size() - 1)
      oss << ",";
  }
  oss << "],";
  oss << "\"conclusion_id\":\"" << inf.conclusion_id << "\",";
  oss << "\"rule\":\"" << inf.rule << "\",";
  oss << "\"temporal_flow\":" << inf.temporal_flow << ",";
  oss << "\"quantum_coherence\":" << inf.quantum_coherence << ",";
  oss << "\"prime_consistency\":" << inf.prime_consistency << ",";
  oss << "\"fractal_convergence\":" << inf.fractal_convergence;
  oss << "}";

  return oss.str();
}

TimeCrystalAtom AtomSpacePersistence::json_to_atom(const std::string &json) {
  // TODO: Implement proper JSON parsing
  return TimeCrystalAtom{};
}

// ================================================================
// CheckpointManager Implementation
// ================================================================

CheckpointManager::CheckpointManager(const std::string &checkpoint_dir,
                                     const PersistenceConfig &config)
    : checkpoint_dir(checkpoint_dir), persistence(config) {

  // Create directory if it doesn't exist
  if (!fs::exists(checkpoint_dir)) {
    fs::create_directories(checkpoint_dir);
  }
}

SerializationResult
CheckpointManager::save_checkpoint(const TimeCrystalKernel *kernel,
                                   const std::string &tag) {
  std::string name = generate_checkpoint_name(tag);
  std::string filepath = checkpoint_dir + "/" + name;

  auto result = persistence.save_to_file(kernel, filepath);

  if (result.success) {
    std::cout << "[Checkpoint] Saved: " << name << std::endl;
    cleanup_old_checkpoints();
  }

  return result;
}

DeserializationResult
CheckpointManager::load_latest_checkpoint(TimeCrystalKernel *kernel) {
  auto checkpoints = list_checkpoints();
  if (checkpoints.empty()) {
    DeserializationResult result;
    result.success = false;
    result.error_message = "No checkpoints found";
    return result;
  }

  // Sort by timestamp (newest first)
  std::sort(checkpoints.begin(), checkpoints.end(),
            [this](const std::string &a, const std::string &b) {
              return get_checkpoint_timestamp(a) > get_checkpoint_timestamp(b);
            });

  return load_checkpoint(kernel, checkpoints[0]);
}

DeserializationResult
CheckpointManager::load_checkpoint(TimeCrystalKernel *kernel,
                                   const std::string &checkpoint_name) {
  std::string filepath = checkpoint_dir + "/" + checkpoint_name;
  auto result = persistence.load_from_file(kernel, filepath);

  if (result.success) {
    std::cout << "[Checkpoint] Loaded: " << checkpoint_name << std::endl;
  }

  return result;
}

std::vector<std::string> CheckpointManager::list_checkpoints() const {
  std::vector<std::string> checkpoints;

  if (!fs::exists(checkpoint_dir)) {
    return checkpoints;
  }

  for (const auto &entry : fs::directory_iterator(checkpoint_dir)) {
    if (entry.path().extension() == ".nbas") {
      checkpoints.push_back(entry.path().filename().string());
    }
  }

  return checkpoints;
}

void CheckpointManager::cleanup_old_checkpoints() {
  auto checkpoints = list_checkpoints();

  if (checkpoints.size() <= max_checkpoints) {
    return;
  }

  // Sort by timestamp (oldest first)
  std::sort(checkpoints.begin(), checkpoints.end(),
            [this](const std::string &a, const std::string &b) {
              return get_checkpoint_timestamp(a) < get_checkpoint_timestamp(b);
            });

  // Remove oldest checkpoints
  size_t to_remove = checkpoints.size() - max_checkpoints;
  for (size_t i = 0; i < to_remove; i++) {
    std::string filepath = checkpoint_dir + "/" + checkpoints[i];
    fs::remove(filepath);
    std::cout << "[Checkpoint] Removed old checkpoint: " << checkpoints[i]
              << std::endl;
  }
}

std::string
CheckpointManager::generate_checkpoint_name(const std::string &tag) {
  auto now = std::chrono::system_clock::now();
  auto time_t = std::chrono::system_clock::to_time_t(now);
  auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(
                    now.time_since_epoch())
                    .count() %
                1000;

  std::ostringstream oss;
  oss << "checkpoint_"
      << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S") << "_"
      << std::setfill('0') << std::setw(3) << millis;

  if (!tag.empty()) {
    oss << "_" << tag;
  }

  oss << ".nbas";
  return oss.str();
}

int64_t CheckpointManager::get_checkpoint_timestamp(const std::string &name) {
  // Extract timestamp from checkpoint name format:
  // checkpoint_YYYYMMDD_HHMMSS_mmm[_tag].nbas
  if (name.size() < 28)
    return 0;

  try {
    std::string datetime = name.substr(11, 15); // YYYYMMDD_HHMMSS
    std::string millis_str = name.substr(27, 3);

    std::tm tm = {};
    std::istringstream ss(datetime);
    ss >> std::get_time(&tm, "%Y%m%d_%H%M%S");

    auto time = std::mktime(&tm);
    int64_t millis = std::stoi(millis_str);

    return static_cast<int64_t>(time) * 1000 + millis;
  } catch (...) {
    return 0;
  }
}
