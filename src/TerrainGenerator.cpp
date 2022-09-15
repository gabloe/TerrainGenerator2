/**
 * TerrainGenerator.cpp
 * Contributors:
 *      * Gabriel Loewen
 * Licence:
 *      * MIT
 */

#include <TerrainGenerator.hpp>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <vector>

#include <asset.hpp>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <ResourceManager.hpp>

#include <ConfigReader.hpp>

#include <Logger.hpp>

struct VertexType {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec4 color;
};

TerrainGenerator::TerrainGenerator(config::ConfigReader& configReader)
    : OGLApplication(),
      modelPath{MODELS_DIR "/tree.DAE"},
      vertexShaderPath(SHADERS_DIR "/shader.vert"),
      fragmentShaderPath(SHADERS_DIR "/shader.frag") {
  if (configReader.ContainsKey("model")) {
    std::string modelName = configReader.ReadString("model");
    modelPath = std::string(MODELS_DIR) + "/" + modelName;
    logging::Logger::LogInfo("Overriding default model vale: " + modelPath);
  }

  if (configReader.ContainsKey("vertexShader")) {
    std::string vertexShaderName = configReader.ReadString("vertexShader");
    vertexShaderPath = std::string(SHADERS_DIR) + "/" + vertexShaderName;
    logging::Logger::LogInfo("Overriding default vertex value: " +
                             vertexShaderPath);
  }

  if (configReader.ContainsKey("fragmentShader")) {
    std::string fragmentShaderName = configReader.ReadString("fragmentShader");
    fragmentShaderPath = std::string(SHADERS_DIR) + "/" + fragmentShaderName;
    logging::Logger::LogInfo("Overriding default fragment value: " +
                             fragmentShaderPath);
  }

  Init();
}

void TerrainGenerator::Init() {
  auto manager = resources::ResourceManager::GetManager();

  // Create shaders
  auto vertexShader = Shader(vertexShaderPath, GL_VERTEX_SHADER);
  auto fragmentShader = Shader(fragmentShaderPath, GL_FRAGMENT_SHADER);
  shaderProgram = std::make_unique<ShaderProgram>(
      std::initializer_list<Shader>{vertexShader, fragmentShader});

  auto model = manager.LoadModel(modelPath);

  models.push_back(model);

  // setup the camera
  cameraPos = glm::vec3(0.0, 0.0, 50.0);
  cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
  cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
  cameraDirection = glm::normalize(cameraPos - cameraTarget);
}

void TerrainGenerator::render() {
  // exit on window close button pressed
  if (glfwWindowShouldClose(getWindow()))
    exit();

  processInput(getWindow());

  // set matrix : projection + view
  projection =
      glm::perspective(glm::radians(fov), getWindowRatio(), znear, zfar);

  // glm::lookAt(eye, center, up)
  view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

  // clear
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shaderProgram->use();

  // send uniforms
  shaderProgram->setUniform("camera", cameraPos);
  shaderProgram->setUniform("model", model);
  shaderProgram->setUniform("projection", projection);
  shaderProgram->setUniform("view", view);

  for (size_t i = 0; i < this->models.size(); i++) {
    this->models[i]->Draw(*shaderProgram);
  }
}

void TerrainGenerator::mouseMoved(GLFWwindow* window, double x, double y) {
  float xpos = static_cast<float>(x);
  float ypos = static_cast<float>(y);
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  cameraFront = glm::normalize(direction);
}

void TerrainGenerator::handleKeyboardEvent(GLFWwindow* window,
                                           int key,
                                           int scancode,
                                           int action,
                                           int mods) {
  if (mods == GLFW_MOD_SHIFT) {
    speed = running_speed;
  } else {
    speed = walking_speed;
  }
  if (key == GLFW_KEY_F && action == GLFW_PRESS) {
    setFullScreen(!isFullScreen());
  }
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    exit();
  }
  if (key == GLFW_KEY_P && action == GLFW_PRESS) {
    polygonMode = (polygonMode + 1) % 2;
    glPolygonMode(GL_FRONT_AND_BACK, polygonModes[polygonMode]);
  }
}

void TerrainGenerator::processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    cameraPos += cameraFront * speed;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    cameraPos -= cameraFront * speed;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
  }
}