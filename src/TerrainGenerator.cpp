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
#include <iostream>
#include <vector>

#include <asset.hpp>
#include <glError.hpp>

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

float constexpr Pi = 3.14159f;

double heightMap(const glm::vec2 position) {
  auto radius = sqrt(position.x * position.x + position.y * position.y);
  if (radius > (2.0 * Pi)) {
    radius -= 2.0 * Pi;
  }

  return 2.0 * cos(radius) * cos(radius);
}

VertexType getHeightMap(const glm::vec2 position) {
  constexpr double delta = 0.0001;
  constexpr double delta_inverse = 1.0 / delta;
  const glm::vec2 dx(delta, 0.0);
  const glm::vec2 dy(0.0, delta);

  VertexType v;
  double h = heightMap(position);
  // hx= (d/dx heightMap)(position)
  double hx = delta_inverse * (heightMap(position + dx) - h);

  // hy= (d/dy heightMap)(position)
  double hy = delta_inverse * (heightMap(position + dy) - h);

  v.position = glm::vec3(position, h);
  v.normal = glm::normalize(glm::vec3(-hx, -hy, 1.0));

  double c = sin(h * 5.f) * 0.5 + 0.5;
  v.color = glm::vec4(c, 1.0 - c, 1.0, 1.0);
  return v;
}

void TerrainGenerator::Generate() {
  std::vector<VertexType> vertices;
  std::vector<GLuint> index;

  for (int y = 0; y <= size; ++y)
    for (int x = 0; x <= size; ++x) {
      float xx = (x - size / 2) * 0.1f;
      float yy = (y - size / 2) * 0.1f;
      vertices.push_back(getHeightMap({xx, yy}));
    }

  for (int y = 0; y < size; ++y)
    for (int x = 0; x < size; ++x) {
      index.push_back((x + 0) + (size + 1) * (y + 0));
      index.push_back((x + 1) + (size + 1) * (y + 0));
      index.push_back((x + 1) + (size + 1) * (y + 1));

      index.push_back((x + 1) + (size + 1) * (y + 1));
      index.push_back((x + 0) + (size + 1) * (y + 1));
      index.push_back((x + 0) + (size + 1) * (y + 0));
    }

  std::cout << "vertices=" << vertices.size() << std::endl;
  std::cout << "index=" << index.size() << std::endl;
}

TerrainGenerator::TerrainGenerator(std::string configPath)
    : OGLApplication(),
      modelPath{MODELS_DIR "/tree.DAE"},
      vertexShaderPath(SHADERS_DIR "/shader.vert"),
      fragmentShaderPath(SHADERS_DIR "/shader.frag") {
  config::ConfigReader reader{configPath};

  if (reader.ContainsKey("model")) {
    modelPath = reader.ReadString("model");
  }

  if (reader.ContainsKey("vertexShader")) {
    vertexShaderPath = reader.ReadString("vertexShader");
  }

  if (reader.ContainsKey("fragmentShader")) {
    fragmentShaderPath = reader.ReadString("fragmentShader");
  }

  if (reader.ContainsKey("debugEnabled")) {
    logging::Logger::GetInstance().SetEnabled(logging::DBG,
                                              reader.ReadBool("debugEnabled"));
  }
  if (reader.ContainsKey("infoEnabled")) {
    logging::Logger::GetInstance().SetEnabled(logging::INF,
                                              reader.ReadBool("infoEnabled"));
  }

  Init();
}

TerrainGenerator::TerrainGenerator()
    : OGLApplication(),
      modelPath{MODELS_DIR "/tree.DAE"},
      vertexShaderPath(SHADERS_DIR "/shader.vert"),
      fragmentShaderPath(SHADERS_DIR "/shader.frag") {
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

void TerrainGenerator::loop() {
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