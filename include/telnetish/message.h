/** @file
*
*  C++ messages abstractions.
*
*  @author Piotr Styczyński <piotrsty1@gmail.com>
*  @copyright MIT
*  @date 2018-04-29
*/
#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>

/**
 * Class reprenseting universal byte message that can be sent via data sources
 * and streams.
 */
class Message {
private:
  char* content;
  int length;

  /**
   * Copy the byte data into the message.
   * If @p length is not specified then bytes are supposed to be 
   * correctly formatted cstring (null-terminated).
   *
   * If the @p bytes is nullptr then message is set to be empty.
   *
   * @param[in] bytes  : byte data sources
   * @param[in] length : bytes count to be copied
   */
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

  /**
   * Creates empty message.
   */
  Message() {
    this->content = nullptr;
    this->length = 0;
  }

  /**
   * Copies existing message.
   *
   * @param[in] message : existing Message object
   */
  Message(const Message& message) {
    this->importData(message.content, message.length);
  }

  /**
   * Creates message from null-terminated cstring.
   *
   * @param[in] message : null-terminated C string
   */
  Message(const char* message) {
    this->importData(message);
  }
  
  /**
   * Creates message from byte stream of the specified length.
   * Note that the bytes doesn't have to be null-terminated
   * like in the case of Message(const char*) constructor.
   *
   * @param[in] message : any byte array
   * @param[in] length  : length of the input array
   */
  Message(const char* message, const int length) {
    this->importData(message, length);
  }

  /**
   * Creates message from string.
   *
   * @param[in] message : string
   */
  Message(std::string message) {
    this->importData(message.c_str());
  }

  /**
   * Creates message from vector of ints.
   * Each int is converted to a single byte.
   *
   * @param[in] message : vector of ints
   */
  Message(std::vector<int> message) {
    const int size = message.size();
    char buffer[size+7];
    int i = 0;
    for(auto c : message ) {
      buffer[i++] = (char) c;
    }
    this->importData(buffer, size);
  }

  /**
   * Default assignment operator.
   *
   * @param[in] message : Message to be copied
   */
  void operator=(const Message& message) {
    if(this->content != nullptr) {
      free(this->content);
      this->content = nullptr;
    }
    this->importData(message.content, message.length);
  }

  /**
   * Default destructor that frees all allocated memory.
   */
  ~Message() {
    free(this->content);
    this->content = nullptr;
    this->length = 0;
  }

  /**
   * Obtain contents of the message.
   *
   * Note that the contents is not a null-terminated string.
   * The length of the contents is only determined by getSize() method.
   *
   * @return pointer to the array with internal byte data
   */
  const char* getContents() const {
    return this->content;
  }

  /**
   * Obtain the size of the message.
   *
   * @return number of bytes that message contains
   */
  int getSize() const {
    return this->length;
  }
  
  /**
   * Converts message to string.
   * 
   * @return string representation of message
   */
  std::string toString() const {
    if(this->content == nullptr) return "";
    return std::string(this->content, (size_t) this->length);
  }

  /**
   * Converts message to string containing values of each byte.
   * Useful for debugging. 
   *
   * @return string representation of message contents
   */
  std::string bytesDumpString() const {
    std::string result = "{ ";
    for(int i=0;i<this->length;++i) {
      result += std::to_string((((int)(this->content[i]))+256)%256) + " ";
    }
    result += "}";
    return result;
  }
  
  /**
   * Converts message to ints vector.
   * Each byte is converted to the 0-255 range int. 
   *
   * @return vector with bytes values
   */
  std::vector<int> toBytes() const {
    std::vector<int> result;
    for(int i=0;i<this->length;++i) {
      result.push_back(((int)(this->content[i])+256)%256);
    }
    return result;
  }

  /**
   * Set value of any byte in the message.
   * The input @p value is converted to the single byte value.
   * If the @p index is not correct (outside bounds of message)
   * then this function is NOP.   
   * 
   * @param[in] index : index of byte to be set
   * @param[in] value : new byte value
   */
  void setByte(const int index, const int value) {
    if(index < 0 || index >= this->length) return;
    this->content[index] = (char) value;
  }
  
  /**
   * Obtains the value of the specified byte in the message.
   * The byte value is converted to 0-255 range integer value.
   * If the @p index is not correct (outside bounds of message)
   * then this function returns 0. 
   * 
   * @param[in] index : index of byte
   * @return integer value of the specified byte
   */
  int operator[](const int index) const {
    if(index < 0 || index >= this->length) return 0;
    return (((int)(this->content[index]))+256)%256;
  }

};

#endif /* __MESSAGE_H__ */
