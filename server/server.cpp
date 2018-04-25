#include <telnetish/telnet-server.h>
#include <iostream>

int main(void) {

  TelnetServer server;

  server.setPort(3005);
  server.onMessage(server.defaultPrintStdoutMessageListener());

  server.onClientConnected([](TelnetServer::TelnetServerEvent event){
    std::cout << "New client connected!\n";
    event.getConnection() << "Hello!";
    
    Message message;
    while(true) {
      event.getConnection() >> message;
      std::cout << "[RCV] " << message.toString() << "\n";
    }
    
  });

  server.start();

  return 0;
};
