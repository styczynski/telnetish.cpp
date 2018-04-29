/** @file
*
*  C++11 Data source abstraction.
*  This file provides abstraction for data sources.
*  Data sources are entities representing bi-directional streams.
*
*
*  @author Piotr Styczyński <piotrsty1@gmail.com>
*  @copyright MIT
*  @date 2018-04-29
*/
#ifndef __DATA_SOURCE_H__
#define __DATA_SOURCE_H__

/**
 * @def DEFAULT_MESSAGE_BUFFER_LENGTH
 *
 * Specifies maximum single-read data length in bytes.
 */
#define DEFAULT_MESSAGE_BUFFER_LENGTH 100000

#include <telnetish/message.h>
#include <vector>
#include <string>
#include <functional>

/**
 * Data source is bidirectional byte stream.
 */
class DataSource {
protected:

  /**
   * Reads bytes from the source.
   * Tries to extract at most @p length bytes.
   * Sometimes less than @p length bytes can be available.
   * In that scenario the function extracts all available bytes.
   *
   * Function returns number of bytes extracted from source.
   * The value can be non-positive (then source can contain no data or
   * other error could occur).
   *
   * @param[out] output : output array
   * @param[in]  length : maximum number of bytes to be extracted
   * @return number of bytes read from the source
   */
  virtual int readData(char* output, const int length) = 0;
  
  /**
   * Writes bytes to the source.
   * Tries to write @p length bytes into the source.
   * Returns number of bytes written.
   * If this number is less than @p length then source can be full or
   * othre error could occur.
   *
   * @param[in] input   : input array
   * @param[in] length  : number of bytes to be written
   * @return number of bytes written to the source
   */
  virtual int writeData(const char* input, const int length) = 0;
  
  /**
   * Callback fired when new message is written into the source.
   *
   * @param[in] input : data written into the source
   */
  virtual void whenMessageWasReceived(Message& message) {}
  
public:

  DataSource() {
    
  }

  virtual ~DataSource() {
    
  }

  /**
   * Read message from the source.
   *
   * @param[out] Message : message object to write the data to
   * @return self
   */
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

  
  /**
   * Write message from the source.
   *
   * @param[out] Message : message object to be written
   * @return self
   */
  virtual DataSource& operator<<(Message input) {
    writeData(input.getContents(), input.getSize());
    return *this;
  }

  /**
   * Write byte vector to the source.
   *
   * @param[out] input : bytes vector to be written
   * @return self
   */
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
