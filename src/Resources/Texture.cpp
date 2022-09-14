// This should expose stbi_load and stbi_image_free
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <stb_image.h>

#include <ResourceManager.hpp>
#include <Texture.hpp>

#include <glad/glad.h>
#include <string>

#include <exception>
#include <sstream>

using namespace models;

unsigned int Load(std::string path) {
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
    else {
      std::stringstream ss;
      ss << "Unsupported format: " << nrComponents;
      throw std::exception{ss.str().c_str()};
    }

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

Texture::Texture(std::string path, std::string type)
    : path{path}, id{Load(path)}, type{type} {}
