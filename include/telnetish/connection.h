/** @file
*
*  C++11 Connection abstraction.
*  This file provides abstraction for socket-based connections.
*
*
*  @author Piotr Styczyński <piotrsty1@gmail.com>
*  @copyright MIT
*  @date 2018-04-29
*/
#ifndef __CONNECTION_H__
#define __CONNECTION_H__

/**
 * @def DEFAULT_CONNECTION_ID
 * 
 * Default ID set when the connection ID is not specified explicite.
 */
#define DEFAULT_CONNECTION_ID  -1

/**
 * @def CONNECTION_NO_SOCKET
 *
 * Default socket descriptor set when connection is initialized without specifying
 * socket descriptor explicite.
 */
#define CONNECTION_NO_SOCKET   -1

#include <telnetish/message.h>
#include <telnetish/data-source.h>
#include <vector>
#include <string>

/**
 * Abstract class used for implementing connections via inheritance.
 */
class Connection : public DataSource {
protected:
  int socket;
  int id;
  bool opened;
  
public:

  Connection(const int socket=CONNECTION_NO_SOCKET, const int id=DEFAULT_CONNECTION_ID) {
    this->socket = socket;
    this->id = id;
    this->opened = false;
  }

  virtual ~Connection() {
    
  }

  int getSocket() const {
    return this->socket;
  }

  int getID() const {
    return this->id;
  }

  virtual bool end() = 0;

  bool isOpened() const {
    return this->opened;
  }
  
};


#endif /* __CONNECTION_H__ */
