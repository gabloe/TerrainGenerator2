
#include <ConfigReader.hpp>

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

using namespace config;

ConfigReader::ConfigReader(std::filesystem::path path) : _path{path} {}

std::string ConfigReader::ReadString(std::string key) {
  std::ifstream f("example.json");
  json data = json::parse(f);
  if (data.contains(key)) {
    return data[key];
  }
  return std::string{};
}

int ConfigReader::ReadInt(std::string key) {
  return 0;
}

bool ConfigReader::ReadBool(std::string key) {
  return false;
}

double ConfigReader::ReadReal(std::string key) {
  return 0.0;
}

bool ConfigReader::ContainsKey(std::string key) {
  return true;
}
