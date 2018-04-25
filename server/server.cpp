#include <telnetish/telnet-server.h>
#include <iostream>

int main(int argc, char** argv) {

  TelnetServer server;

  if(argc<=1) {
    std::cout << "Usage: server [port]\n";
    return 1;
  }

  server.setPort(argv[1]);
  server.setOptionEcho(false);
  server.setOptionLinemode(true);
  server.onMessage(server.defaultPrintStdoutMessageListener());

  server.onClientConnected([](TelnetServer::TelnetServerEvent event){

    Message message;
    while(true) {
      event.getConnection() >> message;
      if(TelnetMessage::isCommand(message)) {

      } else {
        std::cout << "[TEXT] " << message.bytesDumpString() << "\n";
      }
    }

  });

  server.start();

  return 0;
};
