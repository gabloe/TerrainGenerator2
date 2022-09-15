/**
 * OGLApplication.cpp
 * Contributors:
 *      * Gabriel Loewen
 * Licence:
 *      * MIT
 */

#include <OGLApplication.hpp>

#include <stdexcept>

#include <Logger.hpp>

using namespace std;

int r_width = 1600;
int r_height = 900;

void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            GLsizei length,
                            const char* message,
                            const void* userParam);

OGLApplication* currentApplication = NULL;

OGLApplication& OGLApplication::getInstance() {
  if (currentApplication)
    return *currentApplication;
  else
    throw std::runtime_error("There is no current Application");
}

OGLApplication::OGLApplication()
    : _state(stateReady),
      _windowSize({r_width, r_height}),
      title("Terrain Generator") {
  currentApplication = this;

  logging::Logger::LogDebug("[Info] GLFW initialisation");

  // initialize the GLFW library
  if (!glfwInit()) {
    throw std::runtime_error("Couldn't init GLFW");
  }

// setting the required opengl hints
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  for (const auto& version : opengl_versions) {
    // test opengl versions from highest to lowest compatible to find one that
    // is supported on this platform
    logging::Logger::LogDebug("Testing OpenGL " +
                              std::to_string(version.major) + "." +
                              std::to_string(version.minor));
    ;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version.major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version.minor);

    // create the window
    _window = glfwCreateWindow(_windowSize[0], _windowSize[1], title.c_str(),
                               NULL, NULL);
    if (_window) {
      break;
    }
  }
  if (!_window) {
    glfwTerminate();
    throw std::runtime_error("Couldn't create a window");
  }

  glfwMakeContextCurrent(_window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    glfwTerminate();
    throw std::runtime_error(string("Could initialize GLAD"));
  }

  int flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);  // makes sure errors are displayed
                                            // synchronously
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr,
                          GL_TRUE);
  }

  // get version info
  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* version = glGetString(GL_VERSION);
  logging::Logger::LogInfo(
      "Renderer: " + std::string{reinterpret_cast<const char*>(renderer)});
  logging::Logger::LogInfo(
      "Renderer: " + std::string{reinterpret_cast<const char*>(renderer)});

  // opengl configuration
  glEnable(GL_DEPTH_TEST);  // enable depth-testing
  glDepthFunc(GL_LESS);  // depth-testing interprets a smaller value as "closer"
  glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // set the window user pointer to this instance so that we can reference
  // members in OGL callbacks
  glfwSetWindowUserPointer(_window, this);

  // setup mouse event listeners
  glfwSetCursorPosCallback(
      _window, +[](GLFWwindow* win, double x, double y) {
        static_cast<OGLApplication*>(glfwGetWindowUserPointer(win))
            ->mouseMoved(win, x, y);
      });
  glfwSetWindowSizeCallback(
      _window, +[](GLFWwindow* win, int cx, int cy) {
        static_cast<OGLApplication*>(glfwGetWindowUserPointer(win))
            ->windowSizeChanged(win, cx, cy);
      });

  // setup keyboard event listener
  glfwSetKeyCallback(
      _window,
      +[](GLFWwindow* win, int key, int scancode, int action, int mods) {
        static_cast<OGLApplication*>(glfwGetWindowUserPointer(win))
            ->handleKeyboardEvent(win, key, scancode, action, mods);
      });

  // setup monitor
  _primaryMonitor = glfwGetPrimaryMonitor();

  // Find all possible video modes and store them for later use
  _enumerate_video_modes(_primaryMonitor);

  glfwGetWindowSize(_window, &_windowSize[0], &_windowSize[1]);
  glfwGetWindowPos(_window, &_windowPosition[0], &_windowPosition[1]);

  _updateViewport = true;
}

void OGLApplication::_enumerate_video_modes(GLFWmonitor* monitor) {
  // get resolution of monitor
  _video_modes = glfwGetVideoModes(monitor, &_video_mode_count);

  logging::Logger::LogInfo("Start enumerating video modes");
  for (int i = _video_mode_count - 1; i >= 0; --i) {
    logging::Logger::LogInfo(
        std::to_string(_video_modes[i].width) + " x " +
        std::to_string(_video_modes[i].height) + ", " +
        std::to_string(_video_modes[i].redBits + _video_modes[i].blueBits +
                       _video_modes[i].greenBits) +
        " bit color, " + std::to_string(_video_modes[i].refreshRate) + " hz");
  }
  logging::Logger::LogInfo("End enumerating video modes");
}

GLFWwindow* OGLApplication::getWindow() const {
  return _window;
}

void OGLApplication::exit() {
  _state = stateExit;
}

float OGLApplication::getFrameDeltaTime() const {
  return _deltaTime;
}

float OGLApplication::getTime() const {
  return _time;
}

void OGLApplication::run() {
  _state = stateRun;

  // Make the window's context current
  glfwMakeContextCurrent(_window);

  _time = (float)glfwGetTime();

  while (_state == stateRun) {
    // compute new time and delta time
    float t = (float)glfwGetTime();
    _deltaTime = t - _time;
    _time = t;

    if (_updateViewport) {
      glfwGetFramebufferSize(_window, &_viewportSize[0], &_viewportSize[1]);
      glViewport(0, 0, _viewportSize[0], _viewportSize[1]);
      _updateViewport = false;
    }

    // execute the frame code
    loop();

    // Swap Front and Back buffers (double buffering)
    glfwSwapBuffers(_window);

    // Pool and process events
    glfwPollEvents();
  }

  glfwTerminate();
}

void OGLApplication::windowSizeChanged(GLFWwindow* window, int cx, int cy) {
  void* ptr = glfwGetWindowUserPointer(window);
  if (OGLApplication* wndPtr = static_cast<OGLApplication*>(ptr)) {
    wndPtr->_resize(cx, cy);
  }
}

void OGLApplication::_resize(int cx, int cy) {
  _updateViewport = true;
}

bool OGLApplication::isFullScreen() {
  return glfwGetWindowMonitor(_window) != NULL;
}

void OGLApplication::setFullScreen(bool fullscreen) {
  if (isFullScreen() == fullscreen) {
    return;
  }

  if (fullscreen) {
    // backup window position and window size
    glfwGetWindowPos(_window, &_windowPosition[0], &_windowPosition[1]);
    glfwGetWindowSize(_window, &_windowSize[0], &_windowSize[1]);

    // Use the highest resolution possible
    const GLFWvidmode mode = _video_modes[_video_mode_count - 1];

    // switch to full screen
    glfwSetWindowMonitor(_window, _primaryMonitor, 0, 0, mode.width,
                         mode.height, 0);
  } else {
    // restore last window size and position
    glfwSetWindowMonitor(_window, NULL, _windowPosition[0], _windowPosition[1],
                         _windowSize[0], _windowSize[1], 0);
  }

  _updateViewport = true;
}

void OGLApplication::loop() {
  logging::Logger::LogInfo("Loop");
}

int OGLApplication::getWidth() {
  return _windowSize[0];
}

int OGLApplication::getHeight() {
  return _windowSize[1];
}

float OGLApplication::getWindowRatio() {
  return float(_windowSize[0]) / float(_windowSize[1]);
}

void OGLApplication::mouseMoved(GLFWwindow* window, double x, double y) {
  logging::Logger::LogInfo("Mouse moved to <" + std::to_string(x) + "," +
                           std::to_string(y) + ">");
}

void OGLApplication::handleKeyboardEvent(GLFWwindow* window,
                                         int key,
                                         int scancode,
                                         int action,
                                         int mods) {
  logging::Logger::LogInfo("Keyboard event, key = " + std::to_string(key) +
                           ", scancode = " + std::to_string(scancode) +
                           ", action = " + std::to_string(action) +
                           ", mods = " + std::to_string(mods));
}