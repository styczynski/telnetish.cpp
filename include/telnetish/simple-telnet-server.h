#ifndef __SIMPLE_TELNET_SERVER_H__
#define __SIMPLE_TELNET_SERVER_H__

class SimpleTelnetServer;

#include <telnetish/telnet-server.h>
#include <telnetish/term-program.h>

#define MAX_AYT_TIMEOUT 1000

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
      if(event.getPayload().clientTerminalType != "UNKNOWN") {
        program.setTerminalType(event.getPayload().clientTerminalType);
      }
      
      int aytProgramCounter = 0;
      program.onMessageReceived([&event, &aytProgramCounter](TermProgram& program, Message& m){
        event.getConnection() << m;
        aytProgramCounter = 0;
      });
      program.start();
      
      int aytCounter = 0;
      program.wait([&event, &aytCounter, &aytProgramCounter](TermProgram& program)->bool {
        Message m;
        event.getConnection() >> m;
        ++aytProgramCounter;
        if(m.getSize() > 0) {
          if(!TelnetMessage::isCommand(m)) {
            //std::cout << "[TEXT] " << m.bytesDumpString() << "\n";
            //std::cout.flush();
            program.send(m);
            //usleep(50000);
            //program.send(Message("?"));
            usleep(100000);
          } else {
            //Message com = Message(TelnetMessage::commandDescription(m));
            //std::cout << "[COM] " << com.toString() << "\n";
            //std::cout.flush();
          }
          aytCounter = 0;
        } else {
          ++aytCounter;
          if(aytCounter > MAX_AYT_TIMEOUT) {
            //std::cout << "[TIMEOUT]\n";
            //std::cout.flush();
            return false;
          }
        }
        if(aytCounter % 50 == 0) {
          event.getConnection() << TelnetMessage::commandFrom("IAC DO STATUS");
        }
        if(aytProgramCounter > 200) {
          program.send(Message("?"));
          aytProgramCounter = 0;
        }
        usleep(5000);
        return true;
      });
      
      program.end();
    });
  }
  
  void setProgram(TermProgram program) {
    this->prog = program;
  }

};

#endif /* __SIMPLE_TELNET_SERVER_H__ */
