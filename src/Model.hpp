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
  void Load(std::string fileName) {
    auto constexpr flags = aiPostProcessSteps::aiProcess_ValidateDataStructure |
                           aiProcess_GenNormals;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(fileName, flags);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
      throw "Could not read the model file";
    }

    for (auto i = 0; i < scene->mNumMeshes; i++) {
      Mesh mesh;
      mesh.Load(scene, scene->mMeshes[0]);
      this->meshes.push_back(mesh);
    }
  }
};
}  // namespace models