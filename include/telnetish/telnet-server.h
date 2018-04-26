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


#define     _BINARY          0
#define     _ECHO            1
#define     _SUPRESS_GA      3
#define     _STATUS          5
#define     _TIMING_MARK     6
#define     _RCXMTECHO       7
#define     _LINE_WIDTH      8
#define     _PAGE_LENGTH     9
#define     _CR_USE          10
#define     _HORIZ_TABS      11
#define     _HORIZ_TAB_USE   12
#define     _FF_USE          13
#define     _VERT_TABS       14
#define     _VERT_TABS_USE   15
#define     _LF_USE          16
#define     _EXT_ASCII       17
#define     _LOGOUT          18
#define     _BYTE_MACRO      19
#define     _DATA_TERM       20
#define     _SUPDUP          21
#define     _SUPDUP_OUT      22
#define     _SEND_LOCATE     23
#define     _TERM_TYPE       24
#define     _END_RECORD      25
#define     _TACACS_ID       26
#define     _OUT_MARK        27
#define     _TERM_LOC        28
#define     _3270_REGIME     29
#define     _X3_PAD          30
#define     _WINDOW_SIZE     31
#define     _TERM_SPEED      32
#define     _REMOTE_FLOW     33
#define     _LINEMODE        34
#define     _SE              240
#define     _NOOP            241
#define     _DATA_MARK       242
#define     _BREAK           243
#define     _IP              244
#define     _AO              245
#define     _AYT             246
#define     _EC              247
#define     _EL              248
#define     _GA              249
#define     _SB              250
#define     _WILL            251
#define     _WONT            252
#define     _DO              253
#define     _DONT            254
#define     _IAC             255



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

    return std::string("<") + std::to_string(id) + ">";
    //throw "Invalid Telnet command: unknown command was specified!";
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

  static std::string commandDescription(const Message& message) {
    std::vector<std::string> strs = decodeTelnetCommand(message.toBytes());
    std::string out = "";
    const int len = strs.size();
    for(int i=0;i<len;++i) {
      out += strs[i] + " ";
    }
    return out;
  }
  
  static bool isCommand(const Message& message) {
    if(message.getSize() >= 2) {
      return ((((int)message[0])+256)%256) == _IAC && ((((int)message[1])+256)%256) != _IAC;
    }
    return ((((int)message[0])+256)%256) == _IAC;
  }

  static bool isText(const Message& message) {
    return !isCommand(message);
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
  bool optionEcho;
  bool optionLinemode;

public:

  TelnetServer(const int port=DEFAULT_SERVER_PORT) : Server<TelnetServerEventData>(port) {
    this->inited = false;
    this->optionEcho = true;
    this->optionLinemode = false;
  }

  ~TelnetServer() {

  }

  void setOptionEcho(bool optionValue) {
    this->optionEcho = optionValue;
  }

  void setOptionLinemode(bool optionValue) {
    this->optionLinemode = optionValue;
  }

  bool init() override;
  bool start() override;
  void shutdown() override;

};

#endif /* __TCP_SERVER_H__ */
