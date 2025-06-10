#pragma once

#include <string>


class OutputLogger {
public:
  OutputLogger() = default;
  virtual ~OutputLogger() = default;

  virtual void log(const std::string &message) = 0;
  virtual void logError(const std::string &message) = 0;
  virtual void logWarning(const std::string &message) = 0;
  virtual void logInfo(const std::string &message) = 0;
};