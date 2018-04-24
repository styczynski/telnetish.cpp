#include <telnetish/loggable.h>
#include <iostream>

Loggable::Loggable() {
  this->messagesListenerFn = [](LoggableMessage message)->void {};
}

void Loggable::reportError(std::string error) {
  LoggableMessage message(error, LOG_MESSAGE_ERROR);
  this->messagesListenerFn(message);
  this->messages.push_back(message);
}

void Loggable::log(std::string info) {
  LoggableMessage message(info, LOG_MESSAGE_LOG);
  this->messagesListenerFn(message);
  this->messages.push_back(message);
}

void Loggable::onMessage(logListener_t handler, LoggableMessageLevel minimumMessageLevel) {
  const logListener_t next = this->messagesListenerFn;
  this->messagesListenerFn = [next, handler, minimumMessageLevel](LoggableMessage message)->void {
    if(message.getLevel() >= minimumMessageLevel) {
      handler(message);
    }
    next(message);
  };
}

Loggable::logListener_t Loggable::defaultPrintStdoutMessageListener() {
  return [](LoggableMessage message)->void {
    std::cout << (message.toString()+"\n");
  };
}
