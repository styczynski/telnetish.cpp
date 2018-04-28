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
    if(bytes == nullptr) {
      this->content = nullptr;
      this->length = 0;
      return;
    }
    
    const int len = (length<0) ? strlen(bytes) : length;
    if(len <= 0) {
      this->content = nullptr;
      this->length = 0;
      return;
    }
    
    this->content = (char*) malloc(sizeof(char) * (len+7));
    for(int i=0;i<len;++i) {
      this->content[i] = bytes[i];
    }
    this->length = len;
  }

public:

  Message() {
    this->content = nullptr;
    this->length = 0;
  }

  Message(const Message& message) {
    this->importData(message.content, message.length);
  }

  Message(const char* message) {
    this->importData(message);
  }
  
  Message(const char* message, const int length) {
    this->importData(message, length);
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

  void operator=(const Message& message) {
    if(this->content != nullptr) {
      free(this->content);
      this->content = nullptr;
    }
    this->importData(message.content, message.length);
  }

  ~Message() {
    free(this->content);
    this->content = nullptr;
    this->length = 0;
  }

  const char* getContents() const {
    return this->content;
  }

  int getSize() const {
    return this->length;
  }
  
  std::string toString() const {
    if(this->content == nullptr) return "";
    return std::string(this->content, (size_t) this->length);
  }

  std::string bytesDumpString() const {
    std::string result = "{ ";
    for(int i=0;i<this->length;++i) {
      result += std::to_string(this->content[i]) + " ";
    }
    result += "}";
    return result;
  }
  
  std::vector<int> toBytes() const {
    std::vector<int> result;
    for(int i=0;i<this->length;++i) {
      result.push_back(((int)(this->content[i])+256)%256);
    }
    return result;
  }

  void setByte(const int index, const int value) {
    if(index < 0 || index >= this->length) return;;
    this->content[index] = (char) value;
  }
  
  int operator[](const int index) const {
    if(index < 0 || index >= this->length) return 0;
    return (int)(this->content[index]);
  }

};

#endif /* __MESSAGE_H__ */
