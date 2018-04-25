#include <telnetish/telnet-server.h>
#include <string>

const std::vector<std::pair<std::string, int>> TelnetMessage::TELNET_COMMAND_TABLE = {
  TNDEF(   NOOP           )
  TNDEF(   WILL           )
  TNDEF(   WONT           )
  TNDEF(   DO             )
  TNDEF(   DONT           )
  TNDEF(   IAC            )
  TNDEF(   BINARY         )
  TNDEF(   ECHO           )
  TNDEF(   REQUEST_ACK    )
  TNDEF(   LINEMODE       )
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
