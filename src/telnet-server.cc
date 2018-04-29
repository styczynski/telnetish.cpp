#include <telnetish/telnet-server.h>
#include <string>

#define TELNET_INITIALIZATION_MAX_TIMEOUT_MS 250

const std::vector<std::pair<std::string, int>> TelnetMessage::TELNET_COMMAND_TABLE = {
  
  TNDEF(   BINARY         )
  TNDEF(   ECHO           )
  TNDEF(   SUPRESS_GA     )
  TNDEF(   STATUS         )
  TNDEF(   TIMING_MARK    )
  TNDEF(   RCXMTECHO      )
  TNDEF(   LINE_WIDTH     )
  TNDEF(   PAGE_LENGTH    )
  TNDEF(   CR_USE         )
  TNDEF(   HORIZ_TABS     )
  TNDEF(   HORIZ_TAB_USE  )
  TNDEF(   FF_USE         )
  TNDEF(   VERT_TABS      )
  TNDEF(   VERT_TABS_USE  )
  TNDEF(   LF_USE         )
  TNDEF(   EXT_ASCII      )
  TNDEF(   LOGOUT         )
  TNDEF(   BYTE_MACRO     )
  TNDEF(   DATA_TERM      )
  TNDEF(   SUPDUP         )
  TNDEF(   SUPDUP_OUT     )
  TNDEF(   SEND_LOCATE    )
  TNDEF(   TERM_TYPE      )
  TNDEF(   END_RECORD     )
  TNDEF(   TACACS_ID      )
  TNDEF(   OUT_MARK       )
  TNDEF(   TERM_LOC       )
  TNDEF(   3270_REGIME    )
  TNDEF(   X3_PAD         )
  TNDEF(   WINDOW_SIZE    )
  TNDEF(   TERM_SPEED     )
  TNDEF(   REMOTE_FLOW    )
  TNDEF(   LINEMODE       )
  TNDEF(   SE             )
  TNDEF(   NOOP           )
  TNDEF(   DATA_MARK      )
  TNDEF(   BREAK          )
  TNDEF(   IP             )
  TNDEF(   AO             )
  TNDEF(   AYT            )
  TNDEF(   EC             )
  TNDEF(   EL             )
  TNDEF(   GA             )
  TNDEF(   SB             )
  TNDEF(   WILL           )
  TNDEF(   WONT           )
  TNDEF(   DO             )
  TNDEF(   DONT           )
  TNDEF(   IAC            )
};

const std::vector<std::pair<std::string, int>> TelnetMessage::TELNET_NEGOTIATION_COMMAND_TABLE = {
  TNDEF(   IS             )
  TNDEF(   SEND           )
  TNDEF(   INFO           )
};

TelnetServerEventData TelnetServer::basicInitializationRoutine(Connection& con) {
  
  if(this->isVerboseMode) {
    this->log("Starting telnet negotiations with client");
  }
  
  TelnetServerEventData clientData;
  
  clientData.clientTerminalType = "UNKNOWN";
  
  char buffer[100];
  buffer[0] = 0;
  
  if(this->isVerboseMode) {
    this->log("Configuring ECHO option");
  }
  
  if(this->optionEcho) {
    con << TelnetMessage::commandFrom("IAC WONT ECHO");
  } else {
    con << TelnetMessage::commandFrom("IAC WILL ECHO");
  }
  
  if(this->isVerboseMode) {
    this->log("Configuring telnet SGA/LINEMODE options");
  }
  
  if(this->optionLinemode) {
    con << TelnetMessage::commandFrom("IAC DO LINEMODE");
    con << TelnetMessage::commandFrom("IAC WILL SUPRESS_GA");
  } else {
    con << TelnetMessage::commandFrom("IAC DONT LINEMODE");
    con << TelnetMessage::commandFrom("IAC DONT SUPRESS_GA");
  }
  
  if(this->isVerboseMode) {
    this->log("Reading client answers");
  }
  
  int cycle = 0;
  while(true) {
    Message message;
    con >> message;
    usleep(1000);
    ++cycle;
    if(cycle > 250) {
      break;
    }
  }
  
  if(this->isVerboseMode) {
    this->log("Pingping client terminal type");
  }
  
  con << TelnetMessage::commandFrom("IAC DO TERM_TYPE");
  cycle = 0;
  
  if(this->isVerboseMode) {
    this->log("Awaiting response");
  }
  
  while(true) {
    Message message;
    con >> message;
    if(message.getSize() > 0) {
      if(TelnetMessage::isCommand(message)) {
        std::string com = TelnetMessage::commandDescription(message);
        if(sscanf(com.c_str(), "IAC WILL %s", buffer)) {
          std::string willCommandCat(buffer);
          if(willCommandCat == "TERM_TYPE") {
            con << TelnetMessage::commandFrom("IAC SB TERM_TYPE SEND IAC SE");
          }
        } else if(sscanf(com.c_str(), "IAC SB TERM_TYPE IS %s IAC SE", buffer)) {
          clientData.clientTerminalType = std::string(buffer);
          if(this->isVerboseMode) {
            this->log(std::string("Got client terminal type: \"")+std::string(buffer)+"\"");
          }
          break;
        }
      }
    }
    usleep(1000);
    ++cycle;
    if(cycle > TELNET_INITIALIZATION_MAX_TIMEOUT_MS) {
      if(this->isVerboseMode) {
        this->log("Response timed out");
      }
      break;
    }
  }
  
  if(this->isVerboseMode) {
    this->log("Ended telnet negotiations");
  }
  
  return clientData;
}


bool TelnetServer::init() {
  if(this->inited) {
    return false;
  }

  this->server = TCPServer();
  this->inited = true;

  this->server.redirectLogTo(*this);

  this->server.setPort(this->port);

  this->server.onClientConnected([this](TCPServer::TCPServerEvent tcpEvent){
    this->log("New client connected to the TELNET");

    Connection& con = tcpEvent.getConnection();
    
    TelnetServerEventData clientData = basicInitializationRoutine(con);
    
    TelnetServerEvent event(this, tcpEvent.getConnectionSource(), clientData);
    this->clientConnected(event);
    
    if(this->isVerboseMode) {
      this->log("Terminating client telnet session");
    }
  });

  return true;
}

void TelnetServer::shutdown() {
  this->server.shutdown();
}

bool TelnetServer::start() {
  this->init();
  this->server.start();
}
