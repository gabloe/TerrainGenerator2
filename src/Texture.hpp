#pragma once

#include <glad/glad.h>

#include <stb/stb_image.h>

#include <string>

namespace models {
class Texture {
 private:
  static unsigned int Load(std::string path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data =
        stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
      GLenum format;
      if (nrComponents == 1)
        format = GL_RED;
      else if (nrComponents == 3)
        format = GL_RGB;
      else if (nrComponents == 4)
        format = GL_RGBA;

      glBindTexture(GL_TEXTURE_2D, textureID);
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                   GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      stbi_image_free(data);
    } else {
      stbi_image_free(data);
      throw std::string{"Texture failed to load at path: "} + path;
    }

    return textureID;
  }

 public:
  const std::string type;
  const unsigned int id;
  const std::string path;

  Texture(std::string path, std::string type)
      : path{path}, id{Load(path)}, type{type} {}
};
}  // namespace models