
#pragma once

#include <glm/vec3.hpp>
#include <vector>

#include <Shader.hpp>
#include <Texture.hpp>

namespace models {
	class Mesh {
 private:
	 std::vector<glm::vec3> vertices;
	 std::vector<unsigned int> indices;
	 std::vector<models::Texture> texture;

public:
	void Draw(Shader &shader);
	};
}