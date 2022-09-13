#pragma once

#include <ResourceManager.hpp>

using namespace resources;

static ResourceManager manager;

ResourceManager& ResourceManager::GetManager() {
  return manager;
}

std::vector<models::Texture> ResourceManager::LoadTextures(
    aiMaterial* mat,
    aiTextureType type,
    std::string typeName) {
  std::vector<models::Texture> result;

  for (auto i = 0; i < mat->GetTextureCount(type); i++) {
    bool found = false;
    aiString str;
    mat->GetTexture(type, i, &str);

    for (auto i = 0; i < textures_loaded.size(); i++) {
      auto texture = textures_loaded[i];
      if (std::strcmp(texture.path.c_str(), str.C_Str()) == 0) {
        result.push_back(texture);
        found = true;
        break;
      }
    }

    if (!found) {
      auto texture = models::Texture{typeName, type};
      this->textures_loaded.push_back(texture);
      result.push_back(texture);
    }
  }

  return result;
}