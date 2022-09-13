#pragma once

#include <assimp/material.h>
#include <Texture.hpp>

#include <vector>

namespace resources {
class ResourceManager {
 private:
  std::vector<models::Texture> textures_loaded;

 public:
  static ResourceManager& GetManager();

  std::vector<models::Texture> LoadTextures(aiMaterial* mat,
                                            aiTextureType type,
                                            std::string typeName);
};
}  // namespace resources