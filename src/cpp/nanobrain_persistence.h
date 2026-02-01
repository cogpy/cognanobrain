#ifndef NANOBRAIN_PERSISTENCE_H
#define NANOBRAIN_PERSISTENCE_H

#include "nanobrain_time_crystal.h"
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

/**
 * Persistence format version
 */
constexpr uint32_t NANOBRAIN_PERSISTENCE_VERSION = 1;

/**
 * Persistence configuration
 */
struct PersistenceConfig {
  bool include_quantum_states = true;
  bool include_tensor_data = false; // Large - optional
  bool compress_output = false;     // Future: compression support
  bool human_readable = false;      // JSON mode (slower but readable)
};

/**
 * Serialization result
 */
struct SerializationResult {
  bool success;
  std::string error_message;
  size_t bytes_written;
  size_t atoms_serialized;
  size_t inferences_serialized;
};

/**
 * Deserialization result
 */
struct DeserializationResult {
  bool success;
  std::string error_message;
  size_t bytes_read;
  size_t atoms_loaded;
  size_t inferences_loaded;
  uint32_t file_version;
};

/**
 * NanoBrain AtomSpace Persistence
 *
 * Provides save/load functionality for the complete AtomSpace state,
 * including Time Crystal quantum states, truth/attention values,
 * and inference history.
 *
 * Binary format for efficiency, with optional JSON export.
 */
class AtomSpacePersistence {
public:
  AtomSpacePersistence(const PersistenceConfig &config = PersistenceConfig{});
  ~AtomSpacePersistence() = default;

  // ================================================================
  // Save Operations
  // ================================================================

  // Save AtomSpace to binary file
  SerializationResult save_to_file(const TimeCrystalKernel *kernel,
                                   const std::string &filepath);

  // Save AtomSpace to memory buffer
  SerializationResult save_to_buffer(const TimeCrystalKernel *kernel,
                                     std::vector<uint8_t> &buffer);

  // Export to JSON format
  SerializationResult export_to_json(const TimeCrystalKernel *kernel,
                                     const std::string &filepath);

  // ================================================================
  // Load Operations
  // ================================================================

  // Load AtomSpace from binary file
  DeserializationResult load_from_file(TimeCrystalKernel *kernel,
                                       const std::string &filepath);

  // Load AtomSpace from memory buffer
  DeserializationResult load_from_buffer(TimeCrystalKernel *kernel,
                                         const std::vector<uint8_t> &buffer);

  // Import from JSON format
  DeserializationResult import_from_json(TimeCrystalKernel *kernel,
                                         const std::string &filepath);

  // ================================================================
  // Utilities
  // ================================================================

  // Check if file is valid NanoBrain AtomSpace
  bool validate_file(const std::string &filepath);

  // Get file info without full load
  struct FileInfo {
    uint32_t version;
    size_t atom_count;
    size_t inference_count;
    int64_t creation_time;
    std::string description;
  };
  FileInfo get_file_info(const std::string &filepath);

  // ================================================================
  // Configuration
  // ================================================================

  void set_config(const PersistenceConfig &config) { this->config = config; }
  const PersistenceConfig &get_config() const { return config; }

private:
  PersistenceConfig config;

  // Binary serialization helpers
  void write_header(std::ostream &out, size_t atom_count,
                    size_t inference_count);
  bool read_header(std::istream &in, uint32_t &version, size_t &atom_count,
                   size_t &inference_count);

  void write_atom(std::ostream &out, const TimeCrystalAtom &atom);
  TimeCrystalAtom read_atom(std::istream &in);

  void write_inference(std::ostream &out,
                       const TimeCrystalInference &inference);
  TimeCrystalInference read_inference(std::istream &in);

  void write_quantum_state(std::ostream &out,
                           const TimeCrystalQuantumState &state);
  TimeCrystalQuantumState read_quantum_state(std::istream &in);

  void write_string(std::ostream &out, const std::string &str);
  std::string read_string(std::istream &in);

  void write_float_vector(std::ostream &out, const std::vector<float> &vec);
  std::vector<float> read_float_vector(std::istream &in);

  void write_int_vector(std::ostream &out, const std::vector<int> &vec);
  std::vector<int> read_int_vector(std::istream &in);

  // JSON helpers
  std::string atom_to_json(const TimeCrystalAtom &atom);
  std::string inference_to_json(const TimeCrystalInference &inference);
  TimeCrystalAtom json_to_atom(const std::string &json);
};

/**
 * Checkpoint manager for automatic saving
 */
class CheckpointManager {
public:
  CheckpointManager(const std::string &checkpoint_dir,
                    const PersistenceConfig &config = PersistenceConfig{});
  ~CheckpointManager() = default;

  // Save checkpoint with automatic naming
  SerializationResult save_checkpoint(const TimeCrystalKernel *kernel,
                                      const std::string &tag = "");

  // Load latest checkpoint
  DeserializationResult load_latest_checkpoint(TimeCrystalKernel *kernel);

  // Load specific checkpoint by name
  DeserializationResult load_checkpoint(TimeCrystalKernel *kernel,
                                        const std::string &checkpoint_name);

  // List available checkpoints
  std::vector<std::string> list_checkpoints() const;

  // Get checkpoint directory
  const std::string &get_directory() const { return checkpoint_dir; }

  // Set max checkpoints to keep (older ones deleted)
  void set_max_checkpoints(size_t max) { max_checkpoints = max; }

  // Clean old checkpoints
  void cleanup_old_checkpoints();

private:
  std::string checkpoint_dir;
  AtomSpacePersistence persistence;
  size_t max_checkpoints = 10;

  std::string generate_checkpoint_name(const std::string &tag);
  int64_t get_checkpoint_timestamp(const std::string &name);
};

#endif // NANOBRAIN_PERSISTENCE_H
