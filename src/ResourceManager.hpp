#pragma once

#include <Texture.hpp>

#include <vector>

namespace Resources {
class ResourceManager {
 private:
  std::vector<models::Texture> textures_loaded;

 public:
  Texture LoadTexture(std::string fileName) {
    if (textures_loaded.contains(fileName)) {
      return;
    }
  }
};
}  // namespace Resources