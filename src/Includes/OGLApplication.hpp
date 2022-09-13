
#pragma once

/**
 * OGLApplication.cpp
 * Contributors:
 *      * Gabriel Loewen
 * Licence:
 *      * MIT
 */

#ifndef OPENGL_CMAKE_SKELETON_OGLAPPLICATION_HPP
#define OPENGL_CMAKE_SKELETON_OGLAPPLICATION_HPP

#include <array>
#include <functional>
#include <map>
#include <string>

// OGL imports
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct GLFWwindow;

/// OGLApplication class:
/// * init OpenGL
/// * provide:
///   * getWidth()
///   * getHeight()
///   * getFrameDeltaTime()
///   * getWindowRatio()
///   * windowDimensionChanged()
/// * let the user define the "loop" function.
class OGLApplication {
 public:
  OGLApplication();

  static OGLApplication& getInstance();

  // get the window id
  GLFWwindow* getWindow() const;

  // window control
  void exit();

  // delta time between frame and time from beginning
  float getFrameDeltaTime() const;
  float getTime() const;

  // OGLApplication run
  void run();

  // OGLApplication information
  int getWidth();
  int getHeight();
  float getWindowRatio();
  bool isFullScreen();
  void setFullScreen(bool);
  void windowSizeChanged(GLFWwindow*, int, int);

 private:
  enum State { stateReady, stateRun, stateExit };

  State _state;

  OGLApplication& operator=(const OGLApplication&) { return *this; }

  GLFWwindow* _window;

  // Time:
  float _time;
  float _deltaTime;

  // Dimensions and positioning of window:
  std::array<int, 2> _windowPosition{0, 0};
  std::array<int, 2> _windowSize{0, 0};
  std::array<int, 2> _viewportSize{0, 0};

  // Screen
  GLFWmonitor* _primaryMonitor;
  void _resize(int, int);
  bool _updateViewport = true;
  const GLFWvidmode* _video_modes;
  int _video_mode_count = 0;
  void _enumerate_video_modes(GLFWmonitor*);

 protected:
  OGLApplication(const OGLApplication&){};

  std::string title;

  virtual void loop();
  virtual void mouseMoved(GLFWwindow*, double, double);
  virtual void handleKeyboardEvent(GLFWwindow*, int, int, int, int);
};

#endif /* end of include guard: OPENGL_CMAKE_SKELETON_OGLApplication_HPP */