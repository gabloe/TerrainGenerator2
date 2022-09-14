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

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

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

TerrainGenerator::TerrainGenerator()
    : OGLApplication(),
      vertexShader(SHADER_DIR "/shader.vert", GL_VERTEX_SHADER),
      fragmentShader(SHADER_DIR "/shader.frag", GL_FRAGMENT_SHADER),
      shaderProgram({vertexShader, fragmentShader}) {
  glCheckError(__FILE__, __LINE__);

  // creation of the mesh ------------------------------------------------------
  std::vector<VertexType> vertices;
  std::vector<GLuint> index;

  Assimp::Importer importer;
  const aiScene* scene =
      importer.ReadFile("resources/models/teapots.DAE",
                        aiPostProcessSteps::aiProcess_ValidateDataStructure |
                            aiProcess_GenNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode)  {
        throw "Could not read the model file";
    }
    auto mesh = scene->mMeshes[0];

    std::cout << "Mesh count: " << scene->mNumMeshes << std::endl;
    std::cout << "Mesh[0].HasNormals() = " << mesh->HasNormals() << std::endl;

    float scale = 2.0;
    
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
      VertexType vert;
      vert.position.x = mesh->mVertices[i].x / scale;
      vert.position.y = mesh->mVertices[i].y / scale;
      vert.position.z = mesh->mVertices[i].z / scale;

      if (mesh->HasNormals()) {
        vert.normal.x = mesh->mNormals[i].x;
        vert.normal.y = mesh->mNormals[i].y;
        vert.normal.z = mesh->mNormals[i].z;
      }

      vert.color.x = 1.0;

      vertices.push_back(vert);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      // retrieve all indices of the face and store them in the indices vector
      for (unsigned int j = 0; j < face.mNumIndices; j++)
        index.push_back(face.mIndices[j]);
    }

    std::cout << "vertices=" << vertices.size() << std::endl;
    std::cout << "index=" << index.size() << std::endl;

  // creation of the vertex array buffer----------------------------------------

  // vbo
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexType),
               vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // ibo
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint),
               index.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // vao
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // bind vbo
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // map vbo to shader attributes
  shaderProgram.setAttribute("position", 3, sizeof(VertexType),
                             offsetof(VertexType, position));
  shaderProgram.setAttribute("normal", 3, sizeof(VertexType),
                             offsetof(VertexType, normal));
  shaderProgram.setAttribute("color", 4, sizeof(VertexType),
                             offsetof(VertexType, color));

  num_vertices = vertices.size();
  num_indexes = index.size();

  // bind the ibo
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

  // vao end
  glBindVertexArray(0);

  // setup the camera
  cameraPos       = glm::vec3(0.0, 0.0, 10.0);
  cameraFront     = glm::vec3(0.0f, 1.0f, 0.0f);
  cameraUp        = glm::vec3(0.0f, 0.0f,  1.0f);
  cameraTarget    = glm::vec3(0.0f, 0.0f, 0.0f);
  cameraDirection = glm::normalize(cameraPos - cameraTarget);

  std::cout << "OpenGL configured" << std::endl;
}

void TerrainGenerator::loop() {
  // exit on window close button pressed
  if (glfwWindowShouldClose(getWindow()))
    exit();

  processInput(getWindow());

  // set matrix : projection + view
  projection = glm::perspective(glm::radians(fov), getWindowRatio(), znear, zfar);  

  // glm::lookAt(eye, center, up)
  view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

  // clear
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shaderProgram.use();

  // send uniforms
  shaderProgram.setUniform("camera", cameraPos);
  glCheckError(__FILE__, __LINE__);

  shaderProgram.setUniform("model", model);
  glCheckError(__FILE__, __LINE__);

  shaderProgram.setUniform("projection", projection);
  glCheckError(__FILE__, __LINE__);

  shaderProgram.setUniform("view", view);
  glCheckError(__FILE__, __LINE__);


  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

  glCheckError(__FILE__, __LINE__);
  glDrawElements(GL_TRIANGLES, (GLsizei)num_vertices, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);

  shaderProgram.unuse();
}

void TerrainGenerator::mouseMoved(GLFWwindow * window, double x, double y) {
  float xpos = static_cast<float>(x);
  float ypos = static_cast<float>(y);
  if (firstMouse)
  {
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

  yaw   += xoffset;
  pitch += yoffset;

  if(pitch > 89.0f)
      pitch = 89.0f;
  if(pitch < -89.0f)
      pitch = -89.0f;

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * -cos(glm::radians(pitch));
  direction.z = sin(glm::radians(pitch));
  direction.y = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  cameraFront = glm::normalize(direction);
}

void TerrainGenerator::handleKeyboardEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (mods == GLFW_MOD_SHIFT)
  {
    speed = running_speed;
  }
  else
  {
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