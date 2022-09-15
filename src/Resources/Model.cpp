
#include <Model.hpp>

#include <stdexcept>

#include <Logger.hpp>

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

  logging::Logger::LogDebug("processing root node");
  ProcessNode(scene->mRootNode, scene);
}

void Model::Draw(ShaderProgram& shader) const {
  // LogPainful
  //  std::cout << "Rendering model " << this->path << std::endl;
  for (unsigned int i = 0; i < meshes.size(); i++) {
    meshes[i].Draw(shader);
  }
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
  for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
    logging::Logger::LogDebug("loading mesh " + i);
    Mesh mesh;
    mesh.Load(scene, scene->mMeshes[i], this->path);
    this->meshes.push_back(mesh);
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    logging::Logger::LogDebug("processing child node " + i);
    ProcessNode(node->mChildren[i], scene);
  }
}