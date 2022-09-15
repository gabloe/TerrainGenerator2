
#include <map>
#include <string>
namespace logging {

enum LogLevel {
  DBG,
  INF,
  WRN,
  ERR,
};

class Logger {
  static Logger globalInstance;

 private:
  std::map<LogLevel, bool> enabled = {
      {DBG, true},
      {INF, true},
      {WRN, true},
      {ERR, true},
  };

  std::map<LogLevel, std::string> stringMapping = {
      {DBG, "DEBUG"},
      {INF, "INFO"},
      {WRN, "WARN"},
      {ERR, "ERROR"},
  };

  Logger() {}

 public:
  static Logger& GetInstance() { return globalInstance; }

  void Log(LogLevel level, std::string msg) const;

  static void LogDebug(std::string msg);
  static void LogInfo(std::string msg);
  static void LogWarn(std::string msg);
  static void LogError(std::string msg);

  void SetEnabled(LogLevel level, bool value);
  bool GetEnabled(LogLevel level) const;
};
}  // namespace logging