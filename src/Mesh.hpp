
#pragma once

#include <glm/vec3.hpp>
#include <vector>

#include <assimp/scene.h>

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
  /// @brief Loads the mesh data from the scene and assimp mesh object.
  /// @param scene The assimp scene object.
  /// @param mesh The assimp mesh object.
  void Load(const aiScene* scene, const aiMesh* mesh);

  /// @brief Draw the mesh to the screen.
  /// @param shader The shader we want to use when drawing.
  void Draw(Shader& shader);
};
}  // namespace models