#pragma once

#include <string>

namespace models {
class Texture {
 private:
  static unsigned int Load(std::string path);

 public:
  const std::string type;
  const unsigned int id;
  const std::string path;

  Texture(std::string path, std::string type);
};
}  // namespace models