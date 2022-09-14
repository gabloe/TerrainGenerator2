#pragma once

#include <assimp/material.h>

// shared_ptr
#include <memory>

#include <Model.hpp>
#include <Texture.hpp>

#include <map>
#include <optional>
#include <vector>

namespace resources {
class ResourceManager {
 private:
  std::map<std::string, std::shared_ptr<models::Texture>> textures_loaded;
  std::map<std::string, std::shared_ptr<models::Model>> models_loaded;

 public:
  static ResourceManager& GetManager();

  std::vector<std::shared_ptr<models::Texture>> LoadTextures(
      aiMaterial* mat,
      aiTextureType type,
      std::string typeName,
      std::optional<std::string> relativePath = std::nullopt);

  std::shared_ptr<models::Model> LoadModel(std::string path);
};
}  // namespace resources