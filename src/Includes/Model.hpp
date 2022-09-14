#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <Mesh.hpp>
#include <Shader.hpp>
#include <vector>

namespace models {

class Model {
 private:
  std::string path;

  std::vector<models::Mesh> meshes;
  std::vector<Texture>
      textures_loaded;  // Unsure a texture is only loaded once.

  void ProcessNode(aiNode* node, const aiScene* scene);

 public:
  /// @brief Load the model from the provided file.
  /// @param fileName The path to the file.
  void Load(std::string fileName);

  /// @brief Draw using a shader.
  /// @param shader The shader to bind to the model.
  void Draw(ShaderProgram& shader) const;
};
}  // namespace models