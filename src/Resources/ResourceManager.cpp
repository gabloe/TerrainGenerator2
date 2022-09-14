
#include <ResourceManager.hpp>

using namespace resources;

static ResourceManager manager;

#include <filesystem>
#include <iostream>
#include <optional>

ResourceManager& ResourceManager::GetManager() {
  return manager;
}

std::vector<std::shared_ptr<models::Texture>> ResourceManager::LoadTextures(
    aiMaterial* mat,
    aiTextureType type,
    std::string typeName,
    std::optional<std::string> relativePath) {
  std::vector<std::shared_ptr<models::Texture>> result;

  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
    bool found = false;
    aiString str;
    mat->GetTexture(type, i, &str);

    std::string path{str.C_Str()};
    if (relativePath.has_value()) {
      std::filesystem::path rPath{relativePath.value()};

      path = rPath.parent_path().append(path).string();
    }

    auto ptr = this->textures_loaded.find(path);
    if (ptr != this->textures_loaded.end()) {
      result.push_back(ptr->second);
    } else {
      std::shared_ptr<models::Texture> texture =
          std::make_shared<models::Texture>(path, typeName);

      this->textures_loaded[path] = texture;
      result.push_back(texture);
    }
  }

  return result;
}

std::shared_ptr<models::Model> ResourceManager::LoadModel(std::string path) {
  std::cout << "Loading model from " << path << std::endl;
  auto ptr = this->models_loaded.find(path);
  if (ptr != this->models_loaded.end()) {
    std::cout << "\tModel already loaded, returning cached value." << std::endl;
    return ptr->second;
  }

  std::cout << "\tModel not loaded, loading." << std::endl;
  std::shared_ptr<models::Model> result = std::make_shared<models::Model>();
  result->Load(path);
  this->models_loaded[path] = result;

  return result;
}
