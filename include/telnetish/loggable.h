/** @file
*
*  C++ Logging utilities.
*
*  @author Piotr Styczyński <piotrsty1@gmail.com>
*  @copyright MIT
*  @date 2018-04-29
*/
#ifndef __LOGGABLE_H__
#define __LOGGABLE_H__

#include <string>
#include <vector>
#include <functional>

/**
 * Defines available logging levels.
 */
enum LoggableMessageLevel {
  LOG_MESSAGE_ANY = 0,
  LOG_MESSAGE_ERROR = 1,
  LOG_MESSAGE_WARN = 2,
  LOG_MESSAGE_LOG = 100
};

/**
 * Message that can be logged.
 */
class LoggableMessage {
private:
  std::string message;
  LoggableMessageLevel level;

public:

  /**
   * Create message with givenstring content and logging level.
   */
  LoggableMessage(const std::string message, LoggableMessageLevel level=LOG_MESSAGE_LOG) {
    this->message = message;
    this->level = level;
  }

  /**
   * Get message logging level.
   *
   * @return message logging level
   */
  LoggableMessageLevel getLevel() {
    return this->level;
  }

  /**
   * Convert message to string.
   *
   * @return message formatted as string
   */
  std::string toString() {
    return std::string("[") + LoggableMessage::getMessageLevelString(this->level) + "] " + this->message;
  }

  /**
   * Get message logging level description.
   *
   * @param[in] level : logging level
   * @return human readable description of message logging level
   */
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

/**
 * Class representing entity that can print log messages.
 */
class Loggable {
public:
  using logListener_t = std::function<void(LoggableMessage)>;

private:
  std::vector<LoggableMessage> messages;
  logListener_t messagesListenerFn;

public:

  Loggable();
  
  /**
   * Clear entire log.
   */
  void clear();
  
  /**
   * Report new error.
   *
   * @param[in] error : Error message
   */
  void reportError(std::string error);
  
  /**
   * Report new info-level message.
   *
   * @param[in] message : message to be reported
   */
  void log(std::string message);
  
  /**
   * Report new message.
   *
   * @param[in] message : message to be reported
   */
  void sendMessage(LoggableMessage message);
  
  /**
   * Register new message listener for the desired minimum logging level.
   * By default all messages are reported.
   *
   * @param[in] handler             : logging listener
   * @param[in] minimumMessageLevel : minimum message level to trigger the listener
   */
  void onMessage(logListener_t handler, LoggableMessageLevel minimumMessageLevel=LOG_MESSAGE_ANY);
  
  /**
   * Register new message listener that will forward messages to other loggable enitity.
   *
   * @param[in] loggable            : entity to send logs to
   * @param[in] minimumMessageLevel : minimum message level to be forwarded
   */
  void redirectLogTo(Loggable& loggable, LoggableMessageLevel minimumMessageLevel=LOG_MESSAGE_ANY);

  /**
   * Return default logger listener that prints all messages to stdout.
   *
   * @return message listener that prints messages to stdout
   */
  static logListener_t defaultPrintStdoutMessageListener();

};

#endif /* __LOGGABLE_H__ */
