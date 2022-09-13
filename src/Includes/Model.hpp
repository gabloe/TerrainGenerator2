#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <Mesh.hpp>
#include <vector>

namespace models {

class Model {
 private:
  std::vector<models::Mesh> meshes;
  std::vector<Texture>
      textures_loaded;  // Unsure a texture is only loaded once.

 public:
  /// @brief Load the model from the provided file.
  /// @param fileName The path to the file.
  void Load(std::string fileName);
};
}  // namespace models