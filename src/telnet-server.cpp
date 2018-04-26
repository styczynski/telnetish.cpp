#include <telnetish/telnet-server.h>
#include <string>

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

    if(this->optionEcho) {
      con << TelnetMessage::commandFrom("IAC WONT ECHO");
    } else {
      con << TelnetMessage::commandFrom("IAC WILL ECHO");
    }

    if(this->optionLinemode) {
      con << TelnetMessage::commandFrom("IAC DO LINEMODE");
    } else {
      con << TelnetMessage::commandFrom("IAC DONT LINEMODE");
    }
    //con << TelnetMessage::commandFrom("IAC REQUEST_ACK");

    TelnetServerEvent event(this, tcpEvent.getConnectionSource());
    this->clientConnected(event);
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
