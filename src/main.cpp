/**
 * Main.cpp skeleton
 * Contributors:
 *      * Gabriel Loewen
 * Licence:
 *      * MIT
 */

#include <TerrainGenerator.hpp>

#include <Logger.hpp>
#include <Asset.hpp>

int main(int argc, const char* argv[]) {
  std::string configPath = asset::Asset::CONFIG_PATH;
  if (argc == 2) {
    configPath = argv[1];
  }

  config::ConfigReader configReader{configPath};

  // Check this setting first before logging configuration being loadded.
  if (configReader.ContainsKey("infoLoggingEnabled")) {
    logging::Logger::GetInstance().SetEnabled(
        logging::INF, configReader.ReadBool("infoLoggingEnabled"));
  }

  logging::Logger::LogInfo("Config loaded from " + configPath);

  if (configReader.ContainsKey("debugLoggingEnabled")) {
    auto value = configReader.ReadBool("debugLoggingEnabled");
    logging::Logger::GetInstance().SetEnabled(
        logging::DBG, configReader.ReadBool("debugLoggingEnabled"));
    logging::Logger::LogInfo(
        "Overriding default debug logging enabled value: " +
        std::to_string(value));
  }

  TerrainGenerator app{configReader};
  app.run();

  return 0;
}
