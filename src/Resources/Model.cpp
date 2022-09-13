
#include <Model.hpp>

using namespace models;

void Model::Load(std::string fileName) {
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