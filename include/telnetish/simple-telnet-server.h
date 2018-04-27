#ifndef __SIMPLE_TELNET_SERVER_H__
#define __SIMPLE_TELNET_SERVER_H__

class SimpleTelnetServer;

#include <telnetish/telnet-server.h>
#include <telnetish/term-program.h>

class SimpleTelnetServer : public TelnetServer {
public:
  
protected:
  TermProgram prog;

public:
  SimpleTelnetServer(TermProgram program) : TelnetServer() {
    setOptionEcho(false);
    setOptionLinemode(true);
    this->prog = program;
    
    onClientConnected([this](TelnetServer::TelnetServerEvent event){
      TermProgram program = this->prog;
      program.setControlled(true);
      program.onMessageReceived([&event](TermProgram& program, Message& m){
        event.getConnection() << m;
      });
      program.start();
      program.wait([&event](TermProgram& program){
        Message m;
        event.getConnection() >> m;
        if(TelnetMessage::isCommand(m)) {
          std::cout << "[COMMAND] " << TelnetMessage::commandDescription(m) << "\n";
        } else {
          program.send(m);
        }
        usleep(5000);
      });
      
    });
  }
  
  void setProgram(TermProgram program) {
    this->prog = program;
  }

};

#endif /* __SIMPLE_TELNET_SERVER_H__ */
