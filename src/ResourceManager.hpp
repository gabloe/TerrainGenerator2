#pragma once

#include <Texture.hpp>

#include <vector>

namespace resources {
class ResourceManager {
 private:
  std::vector<models::Texture> textures_loaded;

  static ResourceManager manager;

 public:
  static ResourceManager& GetManager() { return manager; }

  models::Texture LoadTexture(std::string path,
                              aiTextureType type,
                              std::string type) {
    for (auto i = 0; i < textures_loaded.size(); i++) {
      auto texture = textures_loaded[i];
      if (texture.path == path) {
        return texture;
      }
    }

    auto texture = models::Texture{path, type};
    return texture;
  }
};
}  // namespace Resources