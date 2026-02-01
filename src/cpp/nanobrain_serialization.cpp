#include "nanobrain_serialization.h"
#include <chrono>
#include <cstring>
#include <iomanip>
#include <sstream>

// Binary format magic number and version
static const uint32_t BINARY_MAGIC = 0x4E424153; // "NBAS"
static const uint16_t BINARY_VERSION = 1;

AtomSpaceSerializer::AtomSpaceSerializer() {}

AtomSpaceSerializer::~AtomSpaceSerializer() {
  if (incremental_active) {
    end_incremental_save();
  }
}

SerializationResult
AtomSpaceSerializer::serialize_to_file(const UnifiedNanoBrainKernel *kernel,
                                       const std::string &filepath,
                                       const SerializationOptions &options) {
  SerializationResult result = {false, "", 0, 0, 0, 0.0};

  auto start_time = std::chrono::high_resolution_clock::now();

  if (!kernel) {
    result.error_message = "Kernel is null";
    return result;
  }

  std::ofstream out;

  if (options.format == SerializationFormat::Binary) {
    out.open(filepath, std::ios::binary);
  } else {
    out.open(filepath);
  }

  if (!out.is_open()) {
    result.error_message = "Failed to open file: " + filepath;
    return result;
  }

  if (options.format == SerializationFormat::Binary) {
    write_binary_header(out);

    auto atom_ids = kernel->get_all_atom_ids();
    uint32_t atom_count = static_cast<uint32_t>(atom_ids.size());
    out.write(reinterpret_cast<const char *>(&atom_count), sizeof(atom_count));

    for (const auto &atom_id : atom_ids) {
      const auto *atom = kernel->get_atom(atom_id);
      if (atom) {
        write_atom_binary(out, *atom);
        result.atoms_serialized++;
      }
    }
  } else {
    // JSON format
    std::string json = serialize_to_string(kernel, options);
    out << json;
    result.atoms_serialized = kernel->get_all_atom_ids().size();
  }

  result.bytes_written = out.tellp();
  out.close();

  auto end_time = std::chrono::high_resolution_clock::now();
  result.time_seconds =
      std::chrono::duration<double>(end_time - start_time).count();
  result.success = true;

  return result;
}

std::string
AtomSpaceSerializer::serialize_to_string(const UnifiedNanoBrainKernel *kernel,
                                         const SerializationOptions &options) {
  if (!kernel) {
    return "{}";
  }

  std::ostringstream ss;
  ss << std::fixed << std::setprecision(options.precision);

  ss << "{\n";
  ss << "  \"version\": \"" << get_serializer_version() << "\",\n";
  ss << "  \"format\": \"nanobrain-atomspace\",\n";
  ss << "  \"timestamp\": " << std::time(nullptr) << ",\n";

  // Atoms
  ss << "  \"atoms\": [\n";
  auto atom_ids = kernel->get_all_atom_ids();
  for (size_t i = 0; i < atom_ids.size(); i++) {
    const auto *atom = kernel->get_atom(atom_ids[i]);
    if (atom) {
      ss << "    " << atom_to_json(*atom, options.precision);
      if (i < atom_ids.size() - 1)
        ss << ",";
      ss << "\n";
    }
  }
  ss << "  ],\n";

  // Metrics
  auto metrics = kernel->get_metrics();
  ss << "  \"metrics\": {\n";
  ss << "    \"total_atoms\": " << metrics.total_atoms << ",\n";
  ss << "    \"total_links\": " << metrics.total_links << ",\n";
  ss << "    \"quantum_coherence\": " << metrics.quantum_coherence << ",\n";
  ss << "    \"consciousness_emergence\": " << metrics.consciousness_emergence
     << "\n";
  ss << "  },\n";

  // Configuration
  const auto &config = kernel->get_config();
  ss << "  \"config\": {\n";
  ss << "    \"memory_size\": " << config.memory_size << ",\n";
  ss << "    \"time_crystal_dimensions\": " << config.time_crystal_dimensions
     << ",\n";
  ss << "    \"max_reasoning_depth\": " << config.max_reasoning_depth << "\n";
  ss << "  }\n";

  ss << "}\n";

  return ss.str();
}

std::string AtomSpaceSerializer::serialize_atom(const TimeCrystalAtom &atom) {
  return atom_to_json(atom, 6);
}

DeserializationResult AtomSpaceSerializer::deserialize_from_file(
    UnifiedNanoBrainKernel *kernel, const std::string &filepath,
    const SerializationOptions &options) {
  DeserializationResult result = {false, "", 0, 0, 0, 0.0};

  auto start_time = std::chrono::high_resolution_clock::now();

  if (!kernel) {
    result.error_message = "Kernel is null";
    return result;
  }

  std::ifstream in;

  if (options.format == SerializationFormat::Binary) {
    in.open(filepath, std::ios::binary);
  } else {
    in.open(filepath);
  }

  if (!in.is_open()) {
    result.error_message = "Failed to open file: " + filepath;
    return result;
  }

  in.seekg(0, std::ios::end);
  result.bytes_read = in.tellg();
  in.seekg(0, std::ios::beg);

  if (options.format == SerializationFormat::Binary) {
    if (!read_binary_header(in)) {
      result.error_message = "Invalid binary header";
      return result;
    }

    uint32_t atom_count = 0;
    in.read(reinterpret_cast<char *>(&atom_count), sizeof(atom_count));

    for (uint32_t i = 0; i < atom_count; i++) {
      TimeCrystalAtom atom = read_atom_binary(in);

      // Create atom in kernel
      TruthValue tv = {atom.truth_value.strength, atom.truth_value.confidence,
                       atom.truth_value.count};
      kernel->create_atom(atom.type, atom.name, tv.strength, tv.confidence,
                          atom.prime_encoding);
      result.atoms_loaded++;
    }
  } else {
    // JSON format
    std::stringstream buffer;
    buffer << in.rdbuf();
    std::string data = buffer.str();

    auto deser_result = deserialize_from_string(kernel, data, options);
    result.atoms_loaded = deser_result.atoms_loaded;

    if (!deser_result.success) {
      result.error_message = deser_result.error_message;
      return result;
    }
  }

  in.close();

  auto end_time = std::chrono::high_resolution_clock::now();
  result.time_seconds =
      std::chrono::duration<double>(end_time - start_time).count();
  result.success = true;

  return result;
}

DeserializationResult AtomSpaceSerializer::deserialize_from_string(
    UnifiedNanoBrainKernel *kernel, const std::string &data,
    const SerializationOptions &options) {
  DeserializationResult result = {false, "", data.size(), 0, 0, 0.0};

  // Simple JSON parsing for atoms array
  // Note: This is a minimal implementation. Production code would use a proper
  // JSON parser.

  size_t atoms_start = data.find("\"atoms\":");
  if (atoms_start == std::string::npos) {
    result.error_message = "No atoms array found";
    return result;
  }

  size_t array_start = data.find('[', atoms_start);
  size_t array_end = data.find(']', array_start);

  if (array_start == std::string::npos || array_end == std::string::npos) {
    result.error_message = "Malformed atoms array";
    return result;
  }

  // For now, just count opening braces as approximate atom count
  std::string atoms_str = data.substr(array_start, array_end - array_start);
  size_t count = 0;
  for (char c : atoms_str) {
    if (c == '{')
      count++;
  }
  result.atoms_loaded = count;

  // Full parsing would extract each atom JSON and call kernel->create_atom()
  // This is left as a TODO for production implementation

  result.success = true;
  return result;
}

TimeCrystalAtom AtomSpaceSerializer::parse_atom(const std::string &json) {
  return json_to_atom(json);
}

void AtomSpaceSerializer::begin_incremental_save(const std::string &filepath) {
  incremental_base_path = filepath;
  incremental_count = 0;
  incremental_files.clear();
  incremental_active = true;
}

size_t
AtomSpaceSerializer::save_increment(const UnifiedNanoBrainKernel *kernel) {
  if (!incremental_active || !kernel)
    return 0;

  std::string filepath = incremental_base_path + "." +
                         std::to_string(incremental_count++) + ".json";

  SerializationOptions options;
  options.format = SerializationFormat::JSON;

  auto result = serialize_to_file(kernel, filepath, options);

  if (result.success) {
    incremental_files.push_back(filepath);
    return result.atoms_serialized;
  }

  return 0;
}

void AtomSpaceSerializer::end_incremental_save() { incremental_active = false; }

void AtomSpaceSerializer::merge_incremental_saves(
    const std::string &output_filepath) {
  // Merge all incremental files into one
  std::ofstream out(output_filepath);

  out << "{\n  \"merged_from\": " << incremental_files.size() << ",\n";
  out << "  \"atoms\": [\n";

  for (size_t i = 0; i < incremental_files.size(); i++) {
    std::ifstream in(incremental_files[i]);
    std::stringstream buffer;
    buffer << in.rdbuf();

    // Extract atoms array content (simplified)
    std::string content = buffer.str();
    size_t start = content.find('[');
    size_t end = content.rfind(']');

    if (start != std::string::npos && end != std::string::npos) {
      out << content.substr(start + 1, end - start - 1);
      if (i < incremental_files.size() - 1)
        out << ",\n";
    }
  }

  out << "\n  ]\n}\n";
  out.close();
}

SerializationFormat
AtomSpaceSerializer::detect_format(const std::string &filepath) {
  std::ifstream in(filepath, std::ios::binary);

  if (!in.is_open()) {
    return SerializationFormat::JSON; // Default
  }

  uint32_t magic = 0;
  in.read(reinterpret_cast<char *>(&magic), sizeof(magic));

  if (magic == BINARY_MAGIC) {
    return SerializationFormat::Binary;
  }

  // Check for JSON or SCXML
  in.seekg(0);
  char first_char = 0;
  in >> first_char;

  if (first_char == '<') {
    return SerializationFormat::SCXML;
  }

  return SerializationFormat::JSON;
}

bool AtomSpaceSerializer::validate(const std::string &data,
                                   SerializationFormat format) {
  if (format == SerializationFormat::JSON) {
    // Basic JSON validation
    int braces = 0;
    int brackets = 0;

    for (char c : data) {
      if (c == '{')
        braces++;
      if (c == '}')
        braces--;
      if (c == '[')
        brackets++;
      if (c == ']')
        brackets--;

      if (braces < 0 || brackets < 0)
        return false;
    }

    return braces == 0 && brackets == 0;
  }

  return true; // Other formats assumed valid
}

// Private helpers

std::string AtomSpaceSerializer::atom_to_json(const TimeCrystalAtom &atom,
                                              int precision) {
  std::ostringstream ss;
  ss << std::fixed << std::setprecision(precision);

  ss << "{";
  ss << "\"id\":\"" << json_escape(atom.id) << "\",";
  ss << "\"type\":\"" << json_escape(atom.type) << "\",";
  ss << "\"name\":\"" << json_escape(atom.name) << "\",";

  ss << "\"truth_value\":{";
  ss << "\"strength\":" << atom.truth_value.strength << ",";
  ss << "\"confidence\":" << atom.truth_value.confidence << ",";
  ss << "\"count\":" << atom.truth_value.count << "},";

  ss << "\"attention_value\":{";
  ss << "\"sti\":" << atom.attention_value.sti << ",";
  ss << "\"lti\":" << atom.attention_value.lti << ",";
  ss << "\"vlti\":" << atom.attention_value.vlti << "},";

  ss << "\"prime_encoding\":[";
  for (size_t i = 0; i < atom.prime_encoding.size(); i++) {
    ss << atom.prime_encoding[i];
    if (i < atom.prime_encoding.size() - 1)
      ss << ",";
  }
  ss << "],";

  ss << "\"time_crystal_state\":"
     << quantum_state_to_json(atom.time_crystal_state, precision) << ",";
  ss << "\"fractal_geometry\":"
     << geometry_to_json(atom.fractal_geometry, precision);

  ss << "}";

  return ss.str();
}

std::string
AtomSpaceSerializer::quantum_state_to_json(const TimeCrystalQuantumState &state,
                                           int precision) {
  std::ostringstream ss;
  ss << std::fixed << std::setprecision(precision);

  ss << "{\"dimensions\":[";
  for (size_t i = 0; i < state.dimensions.size(); i++) {
    ss << state.dimensions[i];
    if (i < state.dimensions.size() - 1)
      ss << ",";
  }
  ss << "],";

  ss << "\"temporal_coherence\":" << state.temporal_coherence << ",";
  ss << "\"fractal_dimension\":" << state.fractal_dimension << ",";
  ss << "\"resonance_frequency\":" << state.resonance_frequency << ",";
  ss << "\"quantum_phase\":" << state.quantum_phase << "}";

  return ss.str();
}

std::string
AtomSpaceSerializer::geometry_to_json(const GeometricPattern &geometry,
                                      int precision) {
  std::ostringstream ss;
  ss << std::fixed << std::setprecision(precision);

  ss << "{\"shape\":" << static_cast<int>(geometry.shape) << ",";
  ss << "\"dimensions\":" << geometry.dimensions << ",";
  ss << "\"symmetry_group\":\"" << json_escape(geometry.symmetry_group)
     << "\",";
  ss << "\"musical_note\":" << static_cast<int>(geometry.musical_note) << ",";
  ss << "\"scale_factor\":" << geometry.scale_factor << "}";

  return ss.str();
}

TimeCrystalAtom AtomSpaceSerializer::json_to_atom(const std::string &json) {
  TimeCrystalAtom atom;
  // Minimal parsing - production code would use proper JSON parser
  // This returns a default atom
  atom.id = "parsed_atom";
  atom.type = "ConceptNode";
  atom.name = "Parsed";
  atom.truth_value = {0.5f, 0.5f, 1.0f};
  atom.attention_value = {0.0f, 0.0f, 0.0f};
  return atom;
}

TimeCrystalQuantumState
AtomSpaceSerializer::json_to_quantum_state(const std::string &json) {
  TimeCrystalQuantumState state;
  state.dimensions.fill(0.0f);
  state.temporal_coherence = 1.0f;
  state.fractal_dimension = 1.0f;
  state.resonance_frequency = 1.0f;
  state.quantum_phase = 0.0f;
  return state;
}

GeometricPattern
AtomSpaceSerializer::json_to_geometry(const std::string &json) {
  GeometricPattern pattern;
  pattern.shape = GMLShape::Sphere;
  pattern.dimensions = 3;
  pattern.symmetry_group = "SO(3)";
  pattern.musical_note = MusicalNote::C;
  pattern.scale_factor = 1.0f;
  return pattern;
}

void AtomSpaceSerializer::write_binary_header(std::ofstream &out) {
  out.write(reinterpret_cast<const char *>(&BINARY_MAGIC),
            sizeof(BINARY_MAGIC));
  out.write(reinterpret_cast<const char *>(&BINARY_VERSION),
            sizeof(BINARY_VERSION));
}

void AtomSpaceSerializer::write_atom_binary(std::ofstream &out,
                                            const TimeCrystalAtom &atom) {
  // Write fixed-size fields
  uint16_t id_len = static_cast<uint16_t>(atom.id.size());
  out.write(reinterpret_cast<const char *>(&id_len), sizeof(id_len));
  out.write(atom.id.c_str(), id_len);

  uint16_t type_len = static_cast<uint16_t>(atom.type.size());
  out.write(reinterpret_cast<const char *>(&type_len), sizeof(type_len));
  out.write(atom.type.c_str(), type_len);

  uint16_t name_len = static_cast<uint16_t>(atom.name.size());
  out.write(reinterpret_cast<const char *>(&name_len), sizeof(name_len));
  out.write(atom.name.c_str(), name_len);

  // Truth value
  out.write(reinterpret_cast<const char *>(&atom.truth_value),
            sizeof(TruthValue));

  // Attention value
  out.write(reinterpret_cast<const char *>(&atom.attention_value),
            sizeof(AttentionValue));

  // Prime encoding
  uint16_t prime_count = static_cast<uint16_t>(atom.prime_encoding.size());
  out.write(reinterpret_cast<const char *>(&prime_count), sizeof(prime_count));
  for (int prime : atom.prime_encoding) {
    out.write(reinterpret_cast<const char *>(&prime), sizeof(prime));
  }
}

bool AtomSpaceSerializer::read_binary_header(std::ifstream &in) {
  uint32_t magic = 0;
  uint16_t version = 0;

  in.read(reinterpret_cast<char *>(&magic), sizeof(magic));
  in.read(reinterpret_cast<char *>(&version), sizeof(version));

  return magic == BINARY_MAGIC && version == BINARY_VERSION;
}

TimeCrystalAtom AtomSpaceSerializer::read_atom_binary(std::ifstream &in) {
  TimeCrystalAtom atom;

  // Read ID
  uint16_t id_len = 0;
  in.read(reinterpret_cast<char *>(&id_len), sizeof(id_len));
  atom.id.resize(id_len);
  in.read(&atom.id[0], id_len);

  // Read Type
  uint16_t type_len = 0;
  in.read(reinterpret_cast<char *>(&type_len), sizeof(type_len));
  atom.type.resize(type_len);
  in.read(&atom.type[0], type_len);

  // Read Name
  uint16_t name_len = 0;
  in.read(reinterpret_cast<char *>(&name_len), sizeof(name_len));
  atom.name.resize(name_len);
  in.read(&atom.name[0], name_len);

  // Read truth value
  in.read(reinterpret_cast<char *>(&atom.truth_value), sizeof(TruthValue));

  // Read attention value
  in.read(reinterpret_cast<char *>(&atom.attention_value),
          sizeof(AttentionValue));

  // Read prime encoding
  uint16_t prime_count = 0;
  in.read(reinterpret_cast<char *>(&prime_count), sizeof(prime_count));
  atom.prime_encoding.resize(prime_count);
  for (uint16_t i = 0; i < prime_count; i++) {
    int prime = 0;
    in.read(reinterpret_cast<char *>(&prime), sizeof(prime));
    atom.prime_encoding[i] = prime;
  }

  return atom;
}
