#ifndef __DATA_SOURCE_H__
#define __DATA_SOURCE_H__

#define DEFAULT_MESSAGE_BUFFER_LENGTH 100000

#include <telnetish/message.h>
#include <vector>
#include <string>
#include <functional>

class DataSource {
protected:

  virtual int readData(char* output, const int length) = 0;
  
  virtual int writeData(const char* input, const int length) = 0;
  
  virtual void whenMessageWasReceived(Message& message) {}
  
public:

  DataSource() {
    
  }

  virtual ~DataSource() {
    
  }

  virtual DataSource& operator>>(Message& output) {
    static char buffer[DEFAULT_MESSAGE_BUFFER_LENGTH];
    buffer[0] = '\0';
    const int len = readData(buffer, DEFAULT_MESSAGE_BUFFER_LENGTH);
    if(len > 0) {
      output = Message(buffer, len);
    } else {
      output = Message();
    }
    whenMessageWasReceived(output);
    return *this;
  }

  virtual DataSource& operator<<(Message input) {
    writeData(input.getContents(), input.getSize());
    return *this;
  }

  virtual DataSource& send(std::vector<int> input) {
    const int size = input.size();
    char buffer[size+7];
    int i = 0;
    for(auto c : input ) {
      buffer[i++] = (char) c;
    }
    writeData(buffer, size);
    return *this;
  }
  
};

#endif /* __DATA_SOURCE_H__ */
