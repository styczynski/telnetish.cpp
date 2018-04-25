#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>

class Message {
private:
  char* content;
  int length;

  void importData(const char* bytes, const int length=-1) {
    const int len = (length<0) ? strlen(bytes) : length;
    this->content = (char*) malloc(sizeof(char) * len);
    strcpy(this->content, bytes);
    this->length = len;
  }
  
public:
  
  Message() {
    this->content = nullptr;
    this->length = 0;
  }
  
  Message(const char* message) {
    this->importData(message);
  }
  
  Message(std::string message) {
    this->importData(message.c_str());
  }
  
  Message(std::vector<int> message) {
    const int size = message.size();
    char buffer[size+7];
    int i = 0;
    for(auto c : message ) {
      buffer[i++] = (char) c;
    }
    this->importData(buffer, size);
  }
  
  ~Message() {
    free(this->content);
    this->content = nullptr;
  }
  
  const char* getContents() const {
    return this->content;
  }
  
  int getSize() const {
    return this->length;
  }
  
  std::string toString() const {
    return std::string(this->content, (size_t) this->length);
  }
  
};

#endif /* __MESSAGE_H__ */
