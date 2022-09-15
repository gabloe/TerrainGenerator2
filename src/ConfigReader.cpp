
#include <ConfigReader.hpp>

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

using namespace config;

void CheckKey(json& data, std::string key) {
  if (!data.contains(key)) {
    throw new std::runtime_error{"The key does not exist: " + key};
  }
}

ConfigReader::ConfigReader(std::filesystem::path path) : _path{path} {}

std::string ConfigReader::ReadString(std::string key) {
  std::ifstream f(this->_path.string());
  json data = json::parse(f);
  CheckKey(data, key);
  return data[key];
}

int ConfigReader::ReadInt(std::string key) {
  std::ifstream f(this->_path.string());
  json data = json::parse(f);
  CheckKey(data, key);
  return data[key];
}

bool ConfigReader::ReadBool(std::string key) {
  std::ifstream f(this->_path.string());
  json data = json::parse(f);
  CheckKey(data, key);
  return data[key];
}

double ConfigReader::ReadReal(std::string key) {
  std::ifstream f(this->_path.string());
  json data = json::parse(f);
  CheckKey(data, key);
  return data[key];
}

bool ConfigReader::ContainsKey(std::string key) {
  std::ifstream f(this->_path.string());
  json data = json::parse(f);
  return data.contains(key);
}
