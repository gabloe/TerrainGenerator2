#pragma once

/**
 * TerrainGenerator.hpp
 * Contributors:
 *      * Gabriel Loewen
 * Licence:
 *      * MIT
 */

#ifndef OPENGL_CMAKE_TERRAINGENERATOR
#define OPENGL_CMAKE_TERRAINGENERATOR

#include "OGLApplication.hpp"
#include "Shader.hpp"

class TerrainGenerator : public OGLApplication {
 public:
  TerrainGenerator();
  glm::vec3 cameraPos;
  glm::vec3 cameraFront;
  glm::vec3 cameraUp;
  glm::vec3 cameraTarget;
  glm::vec3 cameraDirection;
  bool firstMouse = true;
  float lastX = getWidth() / 2.0f;
  float lastY = getHeight() / 2.0f;
  float yaw = 90.0f;
  float pitch = 0.0f;
  float fov = 45.0;
  float znear = 0.01f;
  float zfar = 150.0f;

 protected:
  virtual void loop();
  virtual void mouseMoved(GLFWwindow*, double, double);
  virtual void handleKeyboardEvent(GLFWwindow*, int, int, int, int);

 private:
  const int size = 1024;
  int num_vertices;
  int num_indexes;

  // Movement speed
  const float running_speed = 0.5f;
  const float walking_speed = 0.01f;
  float speed = walking_speed;

  // shader
  Shader vertexShader;
  Shader fragmentShader;
  ShaderProgram shaderProgram;

  // shader matrix uniforms, start with identity
  glm::mat4 model = glm::mat4(1.0);
  glm::mat4 projection = glm::mat4(1.0);
  glm::mat4 view = glm::mat4(1.0);

  // VBO/VAO/ibo
  GLuint vao, vbo, ibo;

  // polygon representation
  GLenum polygonModes[2] = {GL_FILL, GL_LINE};
  int polygonMode = 0;

  // input handling
  void processInput(GLFWwindow*);

  void Generate();
};

#endif  // OPENGL_CMAKE_TERRAINGENERATOR
