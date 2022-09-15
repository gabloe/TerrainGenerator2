/**
 * Main.cpp skeleton
 * Contributors:
 *      * Gabriel Loewen
 * Licence:
 *      * MIT
 */

#include <TerrainGenerator.hpp>

#include <memory>

int main(int argc, const char* argv[]) {
  std::unique_ptr<TerrainGenerator> app;
  if (argc == 2) {
    app = std::make_unique<TerrainGenerator>(std::string{argv[1]});
  } else {
    app = std::make_unique<TerrainGenerator>();
  }
  app->run();
  return 0;
}
