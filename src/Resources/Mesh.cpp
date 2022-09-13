
#include <Mesh.hpp>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <ResourceManager.hpp>

using namespace models;

void Mesh::Load(const aiScene* scene, const aiMesh* mesh) {
  float scale = 2.0;

  for (auto i = 0; i < mesh->mNumVertices; i++) {
    VertexType vert;
    vert.position.x = mesh->mVertices[i].x / scale;
    vert.position.y = mesh->mVertices[i].y / scale;
    vert.position.z = mesh->mVertices[i].z / scale;

    if (mesh->HasNormals()) {
      vert.normal.x = mesh->mNormals[i].x;
      vert.normal.y = mesh->mNormals[i].y;
      vert.normal.z = mesh->mNormals[i].z;
    }

    if (mesh->mTextureCoords[0]) {
      // a vertex can contain up to 8 different texture coordinates. We thus
      // make the assumption that we won't use models where a vertex can have
      // multiple texture coordinates so we always take the first set (0).
      vert.TexCoords.x = mesh->mTextureCoords[0][i].x;
      vert.TexCoords.y = mesh->mTextureCoords[0][i].y;

      // tangent
      vert.Tangent.x = mesh->mTangents[i].x;
      vert.Tangent.y = mesh->mTangents[i].y;
      vert.Tangent.z = mesh->mTangents[i].z;

      // bitangent
      vert.Bitangent.x = mesh->mBitangents[i].x;
      vert.Bitangent.y = mesh->mBitangents[i].y;
      vert.Bitangent.z = mesh->mBitangents[i].z;
    } else {
      vert.TexCoords.x = 0.0;
      vert.TexCoords.y = 0.0;
    }

    vert.color.x = 1.0;

    vertices.push_back(vert);
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    // retrieve all indices of the face and store them in the indices vector
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }

  if (scene->HasMaterials()) {
    // TODO: Handle n > 1
    auto material = scene->mMaterials[0];

    auto manager = resources::ResourceManager::GetManager();

    std::vector<Texture> diffuseMaps = manager.LoadTextures(
        material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    // 2. specular maps
    std::vector<models::Texture> specularMaps = manager.LoadTextures(
        material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    // 3. normal maps
    std::vector<Texture> normalMaps =
        manager.LoadTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    // 4. height maps
    std::vector<Texture> heightMaps =
        manager.LoadTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
  }
}

void Mesh::Draw(Shader& shader) {
  // bind appropriate textures
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  unsigned int normalNr = 1;
  unsigned int heightNr = 1;

  for (unsigned int i = 0; i < textures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 +
                    i);  // active proper texture unit before binding
    // retrieve texture number (the N in diffuse_textureN)
    std::string number;
    std::string name = textures[i].Type();

    if (name == "texture_diffuse")
      number = std::to_string(diffuseNr++);
    else if (name == "texture_specular")
      number = std::to_string(specularNr++);  // transfer unsigned int to stream
    else if (name == "texture_normal")
      number = std::to_string(normalNr++);  // transfer unsigned int to stream
    else if (name == "texture_height")
      number = std::to_string(heightNr++);  // transfer unsigned int to stream

    // now set the sampler to the correct texture unit
    glUniform1i(
        glGetUniformLocation(shader.getHandle(), (name + number).c_str()), i);
    // and finally bind the texture
    glBindTexture(GL_TEXTURE_2D, textures[i].Id());
  }

  // draw mesh
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  // always good practice to set everything back to defaults once configured.
  glActiveTexture(GL_TEXTURE0);
}