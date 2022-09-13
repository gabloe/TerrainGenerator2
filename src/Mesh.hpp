
#pragma once

#include <glm/vec3.hpp>
#include <vector>

#include <Shader.hpp>
#include <Texture.hpp>

namespace models {

struct VertexType {
  // Vertice
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec4 color;

  // Texture
  glm::vec2 TexCoords;
  glm::vec3 Bitangent;
  glm::vec3 Tangent;
};

class Mesh {
 private:
  std::vector<VertexType> vertices;
  std::vector<unsigned int> indices;
  std::vector<models::Texture> textures;

  unsigned int VAO;

 public:
  /// <summary>
  /// Load the Mesh from a file.
  /// </summary>
  void Mesh::Load(const aiScene* scene, const aiMesh* mesh);
  
  /// <summary>
  /// Draw the mesh.
  /// </summary>
  /// <param name="shader"></param>
  void Draw(Shader& shader);
};
}  // namespace models