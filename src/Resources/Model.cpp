
#include <Model.hpp>

#include <iostream>
#include <glError.hpp>

using namespace models;

void Model::Load(std::string fileName) {
  auto constexpr flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                         aiProcess_FlipUVs | aiProcess_CalcTangentSpace;

  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(fileName, flags);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    throw std::runtime_error{"Could not read the model file: " + path};
  }

  this->path = fileName;

  std::cout << "processing root node" << std::endl;
  ProcessNode(scene->mRootNode, scene);
}

void Model::Draw(ShaderProgram& shader) const {
//  std::cout << "Rendering model " << this->path << std::endl;
  for (unsigned int i = 0; i < meshes.size(); i++) {
    meshes[i].Draw(shader);
  }
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
  for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
    std::cout << "loading mesh " << i << std::endl;
    Mesh mesh;
    mesh.Load(scene, scene->mMeshes[i], this->path);
    this->meshes.push_back(mesh);
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    std::cout << "processing child node " << i << std::endl;
    ProcessNode(node->mChildren[i], scene);
  }
}