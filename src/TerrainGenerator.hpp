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

  void mouseMoved(GLFWwindow*, double, double);
  
  float fov = 45.0;

 protected:
  virtual void loop();

 private:
  const int size = 1024;

  // shader
  Shader vertexShader;
  Shader fragmentShader;
  ShaderProgram shaderProgram;

  // shader matrix uniform
  glm::mat4 projection = glm::mat4(1.0);
  glm::mat4 view = glm::mat4(1.0);

  // VBO/VAO/ibo
  GLuint vao, vbo, ibo;

  void registerKeypressCallbacks();
  void mouse_callback(GLFWwindow* window, double xpos, double ypos);
  float mouseSpeed = 0.01f;
};

#endif  // OPENGL_CMAKE_TERRAINGENERATOR
