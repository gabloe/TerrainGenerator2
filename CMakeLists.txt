# Projects Settings
cmake_minimum_required (VERSION 3.1)
project (terrain-generator)

file(GLOB terrain-generator-code
     "src/*.hpp"
     "src/*.cpp"
)

# The main executable
add_executable(terrain-generator
  ${terrain-generator-code}
)

set_property(TARGET terrain-generator PROPERTY CXX_STANDARD 11)
target_compile_options(terrain-generator PRIVATE -Wall)

add_definitions(-DGLEW_STATIC)
add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
add_subdirectory(lib/glew EXCLUDE_FROM_ALL)
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)
add_subdirectory(lib/assimp EXCLUDE_FROM_ALL)
add_subdirectory(lib/glad EXCLUDE_FROM_ALL)

target_link_libraries(terrain-generator
  PRIVATE glfw
  PRIVATE libglew_static
  PRIVATE glm
  PRIVATE assimp
  PRIVATE glad
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/src/asset.hpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/src/asset.hpp
)
target_include_directories(terrain-generator
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src
)
