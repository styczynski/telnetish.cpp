#ifndef __LOGGABLE_H__
#define __LOGGABLE_H__

#include <string>
#include <vector>
#include <functional>

enum LoggableMessageLevel {
  LOG_MESSAGE_ANY = 0,
  LOG_MESSAGE_ERROR = 1,
  LOG_MESSAGE_WARN = 2,
  LOG_MESSAGE_LOG = 100
};

class LoggableMessage {
private:
  std::string message;
  LoggableMessageLevel level;

public:
  LoggableMessage(const std::string message, LoggableMessageLevel level=LOG_MESSAGE_LOG) {
    this->message = message;
    this->level = level;
  }

  LoggableMessageLevel getLevel() {
    return this->level;
  }

  std::string toString() {
    return std::string("[") + LoggableMessage::getMessageLevelString(this->level) + "] " + this->message;
  }

  static std::string getMessageLevelString(const LoggableMessageLevel level) {
    switch(level) {
      case LOG_MESSAGE_ERROR:
        return "ERROR";
      case LOG_MESSAGE_WARN:
        return "WARN";
      case LOG_MESSAGE_LOG:
        return "LOG";
      default:
        return "LOG";
    }
  }
};

class Loggable {
public:
  using logListener_t = std::function<void(LoggableMessage)>;

private:
  std::vector<LoggableMessage> messages;
  logListener_t messagesListenerFn;

public:

  Loggable();
  void clear();
  void reportError(std::string error);
  void log(std::string message);
  void sendMessage(LoggableMessage message);
  void onMessage(logListener_t handler, LoggableMessageLevel minimumMessageLevel=LOG_MESSAGE_ANY);
  void redirectLogTo(Loggable& loggable, LoggableMessageLevel minimumMessageLevel=LOG_MESSAGE_ANY);

  static logListener_t defaultPrintStdoutMessageListener();

};

#endif /* __LOGGABLE_H__ */
