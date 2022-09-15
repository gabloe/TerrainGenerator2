
#include <Logger.hpp>

#include <iostream>

using namespace logging;

Logger Logger::globalInstance;

void Logger::Log(LogLevel level, std::string msg) const {
  if (!enabled.find(level)->second) {
    return;
  }

  std::cout << "[" << stringMapping.find(level)->second << "] : " << msg
            << std::endl;
}

void Logger::LogDebug(std::string msg) {
  GetInstance().Log(LogLevel::DBG, msg);
}
void Logger::LogInfo(std::string msg) {
  GetInstance().Log(LogLevel::INF, msg);
}
void Logger::LogWarn(std::string msg) {
  GetInstance().Log(LogLevel::WRN, msg);
}
void Logger::LogError(std::string msg) {
  GetInstance().Log(LogLevel::ERR, msg);
}

void Logger::SetEnabled(LogLevel level, bool value) {
  enabled[level] = value;
}

bool Logger::GetEnabled(LogLevel level) const {
  return enabled.find(level)->second;
}
