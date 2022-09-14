
#include <Mesh.hpp>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <ResourceManager.hpp>

#include <glError.hpp>
#include <iostream>

using namespace models;

void Mesh::Load(const aiScene* scene,
                const aiMesh* mesh,
                std::optional<std::string> relativePath) {
  float scale = 2.0;

  std::cout << "Mesh has " << mesh->mNumVertices << " vertices." << std::endl;
  std::cout << "Mesh has normals: " << mesh->HasNormals() << std::endl;

  int print = 0;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    double per_done = (100 * i) / mesh->mNumVertices;

    if (per_done == print) {
      std::cout << per_done << "%" << std::endl;
      print += 5;
    }

    VertexType vert;
    vert.Position.x = mesh->mVertices[i].x / scale;
    vert.Position.y = mesh->mVertices[i].y / scale;
    vert.Position.z = mesh->mVertices[i].z / scale;

    if (mesh->HasNormals()) {
      vert.Normal.x = mesh->mNormals[i].x;
      vert.Normal.y = mesh->mNormals[i].y;
      vert.Normal.z = mesh->mNormals[i].z;
    }

    if (mesh->HasTextureCoords(0) && mesh->mTextureCoords[0]) {
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

    // Everyone is red, I don't care!
    vert.Color.x = 1.0;

    vertices.push_back(vert);
  }

  std::cout << "Mesh has " << mesh->mNumFaces << " faces." << std::endl;

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    // retrieve all indices of the face and store them in the indices vector
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }
  std::cout << "Mesh has: " << indices.size() << " indices." << std::endl;

  std::cout << "Scene HasMaterials: " << scene->HasMaterials() << std::endl;
  if (scene->HasMaterials()) {
    // TODO: Handle n > 1
    auto material = scene->mMaterials[0];

    auto manager = resources::ResourceManager::GetManager();

    std::vector<std::shared_ptr<Texture>> diffuseMaps = manager.LoadTextures(
        material, aiTextureType_DIFFUSE, "texture_diffuse", relativePath);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    // 2. specular maps
    std::vector<std::shared_ptr<Texture>> specularMaps = manager.LoadTextures(
        material, aiTextureType_SPECULAR, "texture_specular", relativePath);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    // 3. Normal maps
    std::vector<std::shared_ptr<Texture>> normalMaps = manager.LoadTextures(
        material, aiTextureType_HEIGHT, "texture_normal", relativePath);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    // 4. height maps
    std::vector<std::shared_ptr<Texture>> heightMaps = manager.LoadTextures(
        material, aiTextureType_AMBIENT, "texture_height", relativePath);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
  }
}

void Mesh::Setup() {
  // create buffers/arrays
  CHECKED(glGenVertexArrays(1, &VAO));
  CHECKED(glGenBuffers(1, &VBO));
  CHECKED(glGenBuffers(1, &EBO));

  CHECKED(glBindVertexArray(VAO));
  CHECKED(glCheckError(__FILE__, __LINE__));

  // load data into vertex buffers
  CHECKED(glBindBuffer(GL_ARRAY_BUFFER, VBO));

  // A great thing about structs is that their memory layout is sequential for
  // all its items. The effect is that we can simply pass a pointer to the
  // struct and it translates perfectly to a glm::vec3/2 array which again
  // translates to 3/2 floats which translates to a byte array.
  CHECKED(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexType),
                       &vertices[0], GL_STATIC_DRAW));

  CHECKED(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));

  CHECKED(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                       indices.size() * sizeof(unsigned int), &indices[0],
                       GL_STATIC_DRAW));

  int idx = 0;

  // vertex Positions
  CHECKED(glEnableVertexAttribArray(0));
  CHECKED(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexType),
                                (void*)0));

  // vertex normals
  CHECKED(glEnableVertexAttribArray(1));
  CHECKED(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexType),
                                (void*)offsetof(VertexType, Normal)));

  // vertex color
  CHECKED(glEnableVertexAttribArray(2));
  CHECKED(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexType),
                                (void*)offsetof(VertexType, Color)));

  // vertex texture coords
  glEnableVertexAttribArray(3);
  glCheckError(__FILE__, __LINE__);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexType),
                        (void*)offsetof(VertexType, TexCoords));
  glCheckError(__FILE__, __LINE__);

  // vertex tangent
  glEnableVertexAttribArray(4);
  glCheckError(__FILE__, __LINE__);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexType),
                        (void*)offsetof(VertexType, Tangent));
  glCheckError(__FILE__, __LINE__);

  // vertex bitangent
  glEnableVertexAttribArray(5);
  glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(VertexType),
                        (void*)offsetof(VertexType, Bitangent));
  glCheckError(__FILE__, __LINE__);

  glBindVertexArray(0);
  glCheckError(__FILE__, __LINE__);
}

void Mesh::Draw(ShaderProgram& shader) const {
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
    std::string name = textures[i]->Type();

    if (name == "texture_diffuse")
      number = std::to_string(diffuseNr++);
    else if (name == "texture_specular")
      number = std::to_string(specularNr++);  // transfer unsigned int to stream
    else if (name == "texture_normal")
      number = std::to_string(normalNr++);  // transfer unsigned int to stream
    else if (name == "texture_height")
      number = std::to_string(heightNr++);  // transfer unsigned int to stream

    // now set the sampler to the correct texture unit
    auto location =
        glGetUniformLocation(shader.getHandle(), (name + number).c_str());
    glCheckError(__FILE__, __LINE__);

    glUniform1i(location, i);
    glCheckError(__FILE__, __LINE__);
    // and finally bind the texture
    glBindTexture(GL_TEXTURE_2D, textures[i]->Id());
    glCheckError(__FILE__, __LINE__);
  }

  // draw mesh
  glBindVertexArray(VAO);
  glCheckError(__FILE__, __LINE__);
  glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
  glCheckError(__FILE__, __LINE__);
  glBindVertexArray(0);
  glCheckError(__FILE__, __LINE__);

  // always good practice to set everything back to defaults once configured.
  glActiveTexture(GL_TEXTURE0);
  glCheckError(__FILE__, __LINE__);
}