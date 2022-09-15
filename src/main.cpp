/**
 * Main.cpp skeleton
 * Contributors:
 *      * Gabriel Loewen
 * Licence:
 *      * MIT
 */

#include <TerrainGenerator.hpp>

#include <asset.hpp>
#include <memory>

int main(int argc, const char* argv[]) {
  std::string configPath = CONFIG_PATH;
  if (argc == 2) {
    configPath = argv[1];
  }

  config::ConfigReader configReader{configPath};
  TerrainGenerator app{configReader};
  app.run();

  return 0;
}
