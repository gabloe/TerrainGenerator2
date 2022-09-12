/**
 * OGLApplication.cpp
 * Contributors:
 *      * Gabriel Loewen
 * Licence:
 *      * MIT
 */

#include "OGLApplication.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>

using namespace std;

const int initial_width=1024;
const int initial_height=768;

OGLApplication* currentApplication = NULL;

OGLApplication& OGLApplication::getInstance() {
  if (currentApplication)
    return *currentApplication;
  else
    throw std::runtime_error("There is no current Application");
}


OGLApplication::OGLApplication()
    : state(stateReady), width(initial_width), height(initial_height), title("Terrain Generator") {
  currentApplication = this;

  cout << "[Info] GLFW initialisation" << endl;

  // initialize the GLFW library
  if (!glfwInit()) {
    throw std::runtime_error("Couldn't init GLFW");
  }

  // setting the opengl version
  int major = 3;
  int minor = 2;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create the window
  window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  if (!window) {
    glfwTerminate();
    throw std::runtime_error("Couldn't create a window");
  }

  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  GLenum err = glewInit();

  if (err != GLEW_OK) {
    glfwTerminate();
    throw std::runtime_error(string("Could initialize GLEW, error = ") +
                             (const char*)glewGetErrorString(err));
  }

  // get version info
  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* version = glGetString(GL_VERSION);
  cout << "Renderer: " << renderer << endl;
  cout << "OpenGL version supported " << version << endl;

  // opengl configuration
  glEnable(GL_DEPTH_TEST);  // enable depth-testing
  glDepthFunc(GL_LESS);  // depth-testing interprets a smaller value as "closer"
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  
  // setup mouse event listeners
  glfwSetWindowUserPointer(window, this);
  glfwSetCursorPosCallback(window, +[](GLFWwindow* win, double x, double y){
    static_cast<OGLApplication*>(glfwGetWindowUserPointer(win))->mouseMoved(win, x, y);
  });

  // vsync
  // glfwSwapInterval(false);
}

GLFWwindow* OGLApplication::getWindow() const {
  return window;
}

void OGLApplication::exit() {
  state = stateExit;
}

float OGLApplication::getFrameDeltaTime() const {
  return deltaTime;
}

float OGLApplication::getTime() const {
  return time;
}

void OGLApplication::run() {
  state = stateRun;

  // Make the window's context current
  glfwMakeContextCurrent(window);

  time = glfwGetTime();

  while (state == stateRun) {
    // compute new time and delta time
    float t = glfwGetTime();
    deltaTime = t - time;
    time = t;

    // detech window related changes
    detectWindowDimensionChange();

    // execute the frame code
    loop();

    // Swap Front and Back buffers (double buffering)
    glfwSwapBuffers(window);

    // Pool and process events
    glfwPollEvents();
  }

  glfwTerminate();
}

void OGLApplication::detectWindowDimensionChange() {
  int w, h;
  glfwGetWindowSize(getWindow(), &w, &h);
  dimensionChanged = (w != width || h != height);
  if (dimensionChanged) {
    width = w;
    height = h;
    glViewport(0, 0, 2*w, 2*h);
  }
}

void OGLApplication::loop() {
  cout << "[INFO] : loop" << endl;
}

void OGLApplication::registerKeypressCallback(int key, std::function<void()> callback) {
  keypressCallbacks.insert(std::make_pair(key, callback));
}

int OGLApplication::getWidth() {
  return width;
}

int OGLApplication::getHeight() {
  return height;
}

float OGLApplication::getWindowRatio() {
  return float(width) / float(height);
}

bool OGLApplication::windowDimensionChanged() {
  return dimensionChanged;
}

void OGLApplication::mouseMoved(GLFWwindow * window, double x, double y) {
    cout << "[INFO] : mouseMoved to <" << x << "," << y << ">" << endl;
}