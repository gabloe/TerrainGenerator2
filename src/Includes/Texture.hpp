#pragma once

#include <string>

namespace models {
class Texture {
 private:
  std::string type;
  unsigned int id;
  std::string path;

 public:
  std::string Type() const { return this->type; }
  unsigned int Id() const { return this->id; }
  std::string Path() const { return this->path; }

  Texture(std::string path, std::string typeName);
};
}  // namespace models