#ifndef NANOBRAIN_SERIALIZATION_H
#define NANOBRAIN_SERIALIZATION_H

/**
 * NanoBrain Serialization Module
 *
 * Provides persistence for AtomSpace and Time Crystal states,
 * enabling save/load of cognitive state between sessions.
 *
 * Supports:
 * - JSON format for human-readable exports
 * - Binary format for efficient storage
 * - Incremental saves for large AtomSpaces
 */

#include "nanobrain_time_crystal.h"
#include "nanobrain_unified.h"
#include <fstream>
#include <string>
#include <vector>

/**
 * Serialization format options
 */
enum class SerializationFormat {
  JSON,   // Human-readable JSON
  Binary, // Compact binary format
  SCXML,  // OpenCog SCXML format (compatibility)
};

/**
 * Serialization options
 */
struct SerializationOptions {
  SerializationFormat format = SerializationFormat::JSON;
  bool include_tensors = false;     // Include raw tensor data
  bool include_time_crystal = true; // Include TC quantum states
  bool include_inferences = true;   // Include inference links
  bool compress = false;            // Use compression
  int precision = 6;                // Float precision for JSON
  std::string encoding = "utf-8";   // Text encoding
};

/**
 * Serialization result
 */
struct SerializationResult {
  bool success;
  std::string error_message;
  size_t bytes_written;
  size_t atoms_serialized;
  size_t links_serialized;
  double time_seconds;
};

/**
 * Deserialization result
 */
struct DeserializationResult {
  bool success;
  std::string error_message;
  size_t bytes_read;
  size_t atoms_loaded;
  size_t links_loaded;
  double time_seconds;
};

/**
 * AtomSpace Serializer
 *
 * Handles serialization and deserialization of NanoBrain state.
 */
class AtomSpaceSerializer {
public:
  AtomSpaceSerializer();
  ~AtomSpaceSerializer();

  // ================================================================
  // Serialization
  // ================================================================

  /**
   * Serialize AtomSpace to file
   * @param kernel UnifiedNanoBrainKernel to serialize
   * @param filepath Output file path
   * @param options Serialization options
   * @return Serialization result
   */
  SerializationResult serialize_to_file(const UnifiedNanoBrainKernel *kernel,
                                        const std::string &filepath,
                                        const SerializationOptions &options);

  /**
   * Serialize AtomSpace to string
   * @param kernel UnifiedNanoBrainKernel to serialize
   * @param options Serialization options
   * @return JSON/SCXML string
   */
  std::string serialize_to_string(const UnifiedNanoBrainKernel *kernel,
                                  const SerializationOptions &options);

  /**
   * Serialize single atom to JSON
   * @param atom TimeCrystalAtom to serialize
   * @return JSON string
   */
  std::string serialize_atom(const TimeCrystalAtom &atom);

  // ================================================================
  // Deserialization
  // ================================================================

  /**
   * Deserialize AtomSpace from file
   * @param kernel UnifiedNanoBrainKernel to populate
   * @param filepath Input file path
   * @param options Serialization options (format detection)
   * @return Deserialization result
   */
  DeserializationResult
  deserialize_from_file(UnifiedNanoBrainKernel *kernel,
                        const std::string &filepath,
                        const SerializationOptions &options);

  /**
   * Deserialize AtomSpace from string
   * @param kernel UnifiedNanoBrainKernel to populate
   * @param data JSON/SCXML string
   * @param options Serialization options
   * @return Deserialization result
   */
  DeserializationResult
  deserialize_from_string(UnifiedNanoBrainKernel *kernel,
                          const std::string &data,
                          const SerializationOptions &options);

  /**
   * Parse single atom from JSON
   * @param json JSON string
   * @return Parsed TimeCrystalAtom
   */
  TimeCrystalAtom parse_atom(const std::string &json);

  // ================================================================
  // Incremental Operations
  // ================================================================

  /**
   * Begin incremental save session
   * @param filepath Base file path for saves
   */
  void begin_incremental_save(const std::string &filepath);

  /**
   * Save incremental changes
   * @param kernel Kernel to save changes from
   * @return Number of atoms saved in this increment
   */
  size_t save_increment(const UnifiedNanoBrainKernel *kernel);

  /**
   * End incremental save session
   */
  void end_incremental_save();

  /**
   * Merge incremental saves into single file
   * @param output_filepath Output merged file
   */
  void merge_incremental_saves(const std::string &output_filepath);

  // ================================================================
  // Utilities
  // ================================================================

  /**
   * Detect format from file
   */
  SerializationFormat detect_format(const std::string &filepath);

  /**
   * Validate serialized data
   */
  bool validate(const std::string &data, SerializationFormat format);

  /**
   * Get version information
   */
  std::string get_serializer_version() const { return "1.0.0"; }

private:
  // Incremental save state
  bool incremental_active = false;
  std::string incremental_base_path;
  int incremental_count = 0;
  std::vector<std::string> incremental_files;

  // Internal helpers
  std::string atom_to_json(const TimeCrystalAtom &atom, int precision);
  std::string inference_to_json(const TimeCrystalInference &inference,
                                int precision);
  std::string quantum_state_to_json(const TimeCrystalQuantumState &state,
                                    int precision);
  std::string geometry_to_json(const GeometricPattern &geometry, int precision);

  TimeCrystalAtom json_to_atom(const std::string &json);
  TimeCrystalQuantumState json_to_quantum_state(const std::string &json);
  GeometricPattern json_to_geometry(const std::string &json);

  // Binary helpers
  void write_binary_header(std::ofstream &out);
  void write_atom_binary(std::ofstream &out, const TimeCrystalAtom &atom);
  bool read_binary_header(std::ifstream &in);
  TimeCrystalAtom read_atom_binary(std::ifstream &in);
};

// ================================================================
// JSON Helper Functions
// ================================================================

/**
 * Escape string for JSON
 */
inline std::string json_escape(const std::string &str) {
  std::string result;
  for (char c : str) {
    switch (c) {
    case '"':
      result += "\\\"";
      break;
    case '\\':
      result += "\\\\";
      break;
    case '\n':
      result += "\\n";
      break;
    case '\r':
      result += "\\r";
      break;
    case '\t':
      result += "\\t";
      break;
    default:
      result += c;
    }
  }
  return result;
}

/**
 * Format float to string with precision
 */
inline std::string float_to_string(float value, int precision = 6) {
  char buffer[64];
  snprintf(buffer, sizeof(buffer), "%.*f", precision, value);
  return std::string(buffer);
}

#endif // NANOBRAIN_SERIALIZATION_H
