#ifndef __LOGGABLE_H__
#define __LOGGABLE_H__

#include <string>
#include <vector>
#include <functional>

class Loggable {
public:
  using logListener_t = std::function<void(std::string)>;

private:
  std::vector<std::string> messages;
  logListener_t messagesListenerFn;
  
public:
  
  Loggable();
  void reportError(std::string error);
  void log(std::string message);
  void onMessage(logListener_t handler);
  
  static logListener_t defaultPrintStdoutMessageListener();
  
};

#endif /* __LOGGABLE_H__ */