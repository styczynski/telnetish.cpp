#ifndef __TELNET_SERVER_H__
#define __TELNET_SERVER_H__

class TelnetServer;

#include <telnetish/server.h>
#include <telnetish/loggable.h>
#include <telnetish/tcp-server.h>
#include <telnetish/server-event.h>
#include <telnetish/server-handler.h>
#include <telnetish/message.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <functional>
#include <vector>
#include <string>
#include <utility>

#define     _WILL            251
#define     _WONT            252
#define     _DO              253
#define     _DONT            254
#define     _IAC             255
#define     _BINARY          0
#define     _ECHO            1
#define     _REQUEST_ACK     246
#define     _NOOP            241

#define TNDEF(NAME) { #NAME, _ ## NAME },

class TelnetMessage {
public:

  static const std::vector<std::pair<std::string, int>> TELNET_COMMAND_TABLE;
  
  static int findTelnetCommandByName(std::string name) {
    
    const int len = TELNET_COMMAND_TABLE.size();
    for(int i=0;i<len;++i) {
      if(TELNET_COMMAND_TABLE[i].first == name) {
        return TELNET_COMMAND_TABLE[i].second;
      }
    }
    
    throw "Invalid Telnet command: unknown command was specified!";
  }
  
  static std::string findTelnetCommandByID(int id) {
    
    const int len = TELNET_COMMAND_TABLE.size();
    for(int i=0;i<len;++i) {
      if(TELNET_COMMAND_TABLE[i].second == id) {
        return TELNET_COMMAND_TABLE[i].first;
      }
    }
    
    throw "Invalid Telnet command: unknown command was specified!";
  }

  static std::vector<std::string> decodeTelnetCommand(std::vector<int> command) {
    const int size = command.size();
    std::vector<std::string> result;
    
    for(int i=0;i<size;++i) {
      result.push_back(findTelnetCommandByID(command[i]));
    }
    
    return result;
  }  
  
  static std::vector<int> encodeTelnetCommand(std::string command) {
    const int size = command.size();
    int lastBegin = 0;
    bool firstCommand = true;
    
    std::vector<int> result;
    
    for(int i=0;i<size;++i) {
      if(command[i]==' ') {
        std::string token = command.substr(lastBegin, i-lastBegin);
        int code = findTelnetCommandByName(token);
        if(code == _IAC && !firstCommand) {
          throw "Invalid Telnet command: IAC is not first command!";
        }
        if(code != _IAC && firstCommand) {
          throw "Invalid Telnet command: Missing IAC header!";
        }
        result.push_back(code);
        lastBegin = i+1;
        firstCommand = false;
      }
    }
    
    if(firstCommand == true) {
      throw "Invalid Telnet command: Single command (should be min. two)!";
    }
    
    std::string token = command.substr(lastBegin, size-lastBegin);
    int code = findTelnetCommandByName(token);
    result.push_back(code);
    
    return result;
  }

  static Message commandFrom(std::string command) {
    return Message(encodeTelnetCommand(command));
  }
  
};

class TelnetServerEventData {

};

class TelnetServer : public Server<TelnetServerEventData> {
public:
  using TelnetServerEvent = ServerEvent<TelnetServerEventData>;

protected:
  TCPServer server;
  bool inited;

public:

  TelnetServer(const int port=DEFAULT_SERVER_PORT) : Server<TelnetServerEventData>(port) {
    this->inited = false;
  }

  ~TelnetServer() {

  }

  bool init() override;
  bool start() override;
  void shutdown() override;

};

#endif /* __TCP_SERVER_H__ */
