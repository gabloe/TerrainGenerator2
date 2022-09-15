#pragma once

#include <filesystem>

namespace config {
class ConfigReader {
    private:
    std::filesystem::path _path;
 public:
  /// @brief Loads the config from the file.
  ConfigReader(std::filesystem::path path);

  /// @brief Read a string value from config.
  /// @param key The configuration key.
  /// @return The configuration value if exists, exception otherwise.
  std::string ReadString(std::string key);

  /// @brief Read an integer value from config.
  /// @param key The configuration key.
  /// @return The configuration value if exists, exception otherwise.
  int ReadInt(std::string key);

  /// @brief Read an boolean value from config.
  /// @param key The configuration key.
  /// @return The configuration value if exists, exception otherwise.
  bool ReadBool(std::string key);

  /// @brief Read a real value from config.
  /// @param key The configuration key.
  /// @return The configuration value if exists, exception otherwise.
  double ReadReal(std::string key);

  /// @brief Determines if a key exists in the configuration file.
  /// @param key The configuration key.
  /// @return True if exists, false otherwise.
  bool ContainsKey(std::string key);
};
}  // namespace config