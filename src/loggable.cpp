#include <telnetish/loggable.h>
#include <iostream>

Loggable::Loggable() {
  this->messagesListenerFn = [](std::string message)->void {};
}

void Loggable::reportError(std::string error) {
  this->messages.push_back(error);
}

void Loggable::log(std::string message) {
  this->messagesListenerFn(message);
  this->messages.push_back(message);
}

void Loggable::onMessage(logListener_t handler) {
  const logListener_t next = this->messagesListenerFn;
  this->messagesListenerFn = [next, handler](std::string message)->void {
    handler(message);
    next(message);
  };
}

Loggable::logListener_t Loggable::defaultPrintStdoutMessageListener() {
  return [](std::string message)->void {
    std::cout << ("[LOG] "+message+"\n");
  };
}