/** @file
*
*  Implementation of Telnet server.
*
*  @author Piotr Styczyński <piotrsty1@gmail.com>
*  @copyright MIT
*  @date 2018-04-29
*/
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

/**
 * Definitions of all telnet opcodes
 */
#define     _BINARY          0
#define     _IS              0
#define     _ECHO            1
#define     _SEND            1
#define     _INFO            2
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


/**
 * Macro that is used in telnet-server.cpp
 * to create command <-> byte code mapping.
 */
#define TNDEF(NAME) { #NAME, _ ## NAME },

/**
 * Helper to provide utilities to work with telnet communication.
 */
class TelnetMessage {
public:

  /**
   * Mapping between telnet commands and their byte codes
   */
  static const std::vector<std::pair<std::string, int>> TELNET_COMMAND_TABLE;
  
  /**
   * Mapping between telnet subnegotiation commands and their byte codes
   */
  static const std::vector<std::pair<std::string, int>> TELNET_NEGOTIATION_COMMAND_TABLE;
  
  /**
   * Search telnet command mapping for the given command name.
   *
   * @param[in] name         : command name to be found
   * @param[in] SBSearchMode : should also search TELNET_NEGOTIATION_COMMAND_TABLE?
   *
   * @return byte code of the command
   */
  static int findTelnetCommandByName(std::string name, const bool SBSearchMode=false) {

    if(SBSearchMode) {
      const int len = TELNET_NEGOTIATION_COMMAND_TABLE.size();
      for(int i=0;i<len;++i) {
        if(TELNET_NEGOTIATION_COMMAND_TABLE[i].first == name) {
          return TELNET_NEGOTIATION_COMMAND_TABLE[i].second;
        }
      }
    }
  
    const int len = TELNET_COMMAND_TABLE.size();
    for(int i=0;i<len;++i) {
      if(TELNET_COMMAND_TABLE[i].first == name) {
        return TELNET_COMMAND_TABLE[i].second;
      }
    }

    throw "Invalid Telnet command: unknown command was specified!";
  }

  /**
   * Search telnet command mapping for the given command byte code.
   *
   * @param[in] id           : command code to be found
   * @param[in] SBSearchMode : should also search TELNET_NEGOTIATION_COMMAND_TABLE?
   *
   * @return name of the command
   */
  static std::string findTelnetCommandByID(int id, const bool SBSearchMode=false) {
  
    if(SBSearchMode) {
      const int len = TELNET_NEGOTIATION_COMMAND_TABLE.size();
      for(int i=0;i<len;++i) {
        if(TELNET_NEGOTIATION_COMMAND_TABLE[i].second == id) {
          return TELNET_NEGOTIATION_COMMAND_TABLE[i].first;
        }
      }
    }
    
    const int len = TELNET_COMMAND_TABLE.size();
    for(int i=0;i<len;++i) {
      if(TELNET_COMMAND_TABLE[i].second == id) {
        return TELNET_COMMAND_TABLE[i].first;
      }
    }

    return std::string("<") + std::to_string(id) + ">";
  }

  /**
   * Decodes telnet command bytes to string representation using findTelnetCommandByID.
   *
   * @param[in] command : bytes sequence to be decoded
   * @return human readable command translation
   */
  static std::vector<std::string> decodeTelnetCommand(std::vector<int> command) {
    const int size = command.size();
    bool SBSearchMode = false;
    bool textMode = false;
    std::vector<std::string> result;

    for(int i=0;i<size;++i) {
      
      if(command[i] == _SB && !textMode) {
          SBSearchMode = true;
      }
      if(command[i] == _SE && !textMode) {
        SBSearchMode = false;
      }
      if(command[i] == _IAC && textMode) {
        textMode = false;
      }
      
      std::string commandCode = "";
      
      if(!textMode) {
        commandCode = findTelnetCommandByID(command[i], SBSearchMode);
        result.push_back(commandCode);
      } else {
        
        char textBuffer[1000];
        int textBufferLength = 0;
        while(i<size && command[i] != _IAC) {
          textBuffer[textBufferLength] = command[i];
          ++i;
          ++textBufferLength;
        }
        textBuffer[textBufferLength] = 0;
        result.push_back(std::string(textBuffer, textBufferLength));
        
        --i;
        continue;
      }
      
      if(command[i] == _IS && !textMode) {
        textMode = true;
      }
      
    }

    return result;
  }

  /**
   * Encodes telnet command string to corresponding bytes.
   *
   * @param[in] command : string command to be encoded
   * @return sequence of bytes corresponding to the command string
   */
  static std::vector<int> encodeTelnetCommand(std::string command) {
    const int size = command.size();
    int lastBegin = 0;
    bool firstCommand = true;
    bool SBSearchMode = false;
    bool textMode = false;
    
    std::vector<int> result;

    for(int i=0;i<size;++i) {
      if(command[i]==' ') {
        
        std::string token = command.substr(lastBegin, i-lastBegin);
        int code = findTelnetCommandByName(token, SBSearchMode);
        
        if(code == _SB && !textMode) {
          SBSearchMode = true;
        }
        if(code == _SE && !textMode) {
          SBSearchMode = false;
        }
        if(code == _IAC && textMode) {
          textMode = false;
        }
        
        if(!textMode) {
          code = findTelnetCommandByName(token, SBSearchMode);
          result.push_back(code);
        } else {
          const int tokenLen = token.size();
          for(int j=0;j<tokenLen;++j) {
            result.push_back(((((int)token[j])+256)%256));
          }
        }

        if(code == _IS && !textMode) {
          textMode = true;
        }
        
        lastBegin = i+1;
        firstCommand = false;
      }
    }

    std::string token = command.substr(lastBegin, size-lastBegin);
    int code = findTelnetCommandByName(token, SBSearchMode);
    result.push_back(code);
    
    return result;
  }

  /**
   * Translates telnet command string to the Message object.
   *
   * @param[in] command : string command to be encoded
   * @return sequence of bytes corresponding to the command string
   */
  static Message commandFrom(std::string command) {
    return Message(encodeTelnetCommand(command));
  }

  /**
   * Translates telnet message to the human readable description.
   *
   * @param[in] command : message with telnet command
   * @return sequence of bytes corresponding to the command string
   */
  static std::string commandDescription(const Message& message) {
    std::vector<std::string> strs = decodeTelnetCommand(message.toBytes());
    std::string out = "";
    const int len = strs.size();
    for(int i=0;i<len;++i) {
      out += strs[i];
      if(i!=len-1) {
        out += " ";
      }
    }
    return out;
  }
  
  /** 
   * Checks if message has IAC telnet starting byte.
   *
   * @return if message has IAC header?
   */
  static bool isCommand(const Message& message) {
    if(message.getSize() >= 2) {
      return ((((int)message[0])+256)%256) == _IAC && ((((int)message[1])+256)%256) != _IAC;
    }
    return ((((int)message[0])+256)%256) == _IAC;
  }

  /** 
   * Checks if message has not IAC telnet starting byte.
   *
   * @return if message has not IAC header?
   */
  static bool isText(const Message& message) {
    return !isCommand(message);
  }

};

/**
 * Data attached to telnet server events.
 */
class TelnetServerEventData {
public:
  std::string clientTerminalType; //< Name of the type of the client terminal
};

/**
 * Class representing configurable telnet TCP server.
 */
class TelnetServer : public Server<TelnetServerEventData> {
public:
  using TelnetServerEvent = ServerEvent<TelnetServerEventData>;
  
private:

  TelnetServerEventData basicInitializationRoutine(Connection& con);
 
protected:
  TCPServer server;
  bool inited;
  bool optionEcho;
  bool optionLinemode;
  bool isVerboseMode;

public:

  TelnetServer(const int port=DEFAULT_SERVER_PORT) : Server<TelnetServerEventData>(port) {
    this->inited = false;
    this->optionEcho = true;
    this->optionLinemode = false;
    this->isVerboseMode = false;
  }

  ~TelnetServer() {

  }

  /**
   * Server logs more information in verbosive mode.
   *
   * @param[in] mode : set verbosive mode?
   */
  void setVerbose(bool mode = true) {
    this->isVerboseMode = mode;
  }
  
  /**
   * Enables telnet client echo mode.
   *
   * @param[in] mode : set client echo mode?
   */
  void setOptionEcho(bool optionValue) {
    this->optionEcho = optionValue;
  }

  /**
   * Enables telnet linemode.
   *
   * @param[in] mode : set client linemode?
   */
  void setOptionLinemode(bool optionValue) {
    this->optionLinemode = optionValue;
  }

  bool init() override;
  bool start() override;
  void shutdown() override;

};

#endif /* __TELNET_SERVER_H__ */
