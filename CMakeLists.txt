# Projects Settings
cmake_minimum_required (VERSION 3.1)
project (terrain-generator)


#
# OpenGL is required.
#
find_package (OpenGL REQUIRED)

if (CMAKE_VERSION VERSION_LESS 3.10)
  find_library(OPENGL_egl_LIBRARY NAMES EGL)
  if (OPENGL_egl_LIBRARY)
    set (OpenGL_EGL_FOUND TRUE)
  endif ()
endif ()

if (OPENGL_opengl_LIBRARY)
  set (OUR_LIBRARIES ${OPENGL_opengl_LIBRARY})
else ()
  set (OUR_LIBRARIES ${OPENGL_gl_LIBRARY})
endif ()

if (GLEW_X11 AND NOT WIN32 AND NOT APPLE)
  find_package (X11)
  list (APPEND OUR_LIBRARIES ${OPENGL_glx_LIBRARY} ${X11_LIBRARIES})
endif()

if (GLEW_EGL AND UNIX)
  add_definitions (-DGLEW_EGL)
  if (NOT OpenGL_EGL_FOUND)
    message (FATAL_ERROR "EGL library set but not found.")
  endif ()
  list (APPEND OUR_LIBRARIES ${OPENGL_egl_LIBRARY})
endif ()


# Disable all exporters
set(ASSIMP_NO_EXPORT TRUE)

# Limit importers
set(ASSIMP_IMPORTER_ENABLED TRUE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT FALSE)
set(ASSIMP_BUILD_COLLADA_IMPORTER TRUE)
include_directories(src/includes lib/stb)

file(GLOB terrain-generator-code
     "src/**/*.hpp"
     "src/*.cpp"
     "src/**/*.cpp"
)

# The main executable
add_executable(terrain-generator
  ${terrain-generator-code}
)

set_property(TARGET terrain-generator PROPERTY CXX_STANDARD 11)
target_compile_options(terrain-generator PRIVATE -Wall)

add_definitions(-DGLEW_STATIC)
add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)
add_subdirectory(lib/assimp EXCLUDE_FROM_ALL)
add_subdirectory(lib/glad EXCLUDE_FROM_ALL)

target_link_libraries(terrain-generator
  PRIVATE glfw
  PRIVATE glm
  PRIVATE assimp
  PRIVATE glad
  ${OUR_LIBRARIES}
)


configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/src/asset.hpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/src/asset.hpp
)
target_include_directories(terrain-generator
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src
)