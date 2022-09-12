/**
 * OGLApplication.cpp
 * Contributors:
 *      * Gabriel Loewen
 * Licence:
 *      * MIT
 */

#ifndef OPENGL_CMAKE_SKELETON_OGLAPPLICATION_HPP
#define OPENGL_CMAKE_SKELETON_OGLAPPLICATION_HPP

#include <string>
#include <map>
#include <functional>

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

  // OGLApplication informations
  int getWidth();
  int getHeight();
  float getWindowRatio();
  bool windowDimensionChanged();
  void registerKeypressCallback(int, std::function<void()>);
  std::map<int, std::function<void()>> keypressCallbacks;

  // Movement speed
  float speed = 0.1f;

 private:
  enum State { stateReady, stateRun, stateExit };

  State state;

  OGLApplication& operator=(const OGLApplication&) { return *this; }

  GLFWwindow* window;

  // Time:
  float time;
  float deltaTime;

  // Dimensions:
  int width;
  int height;
  bool dimensionChanged;
  void detectWindowDimensionChange();

 protected:
  OGLApplication(const OGLApplication&){};

  std::string title;

  virtual void loop();
  virtual void mouseMoved(GLFWwindow*, double, double);

};

#endif /* end of include guard: OPENGL_CMAKE_SKELETON_OGLApplication_HPP */
