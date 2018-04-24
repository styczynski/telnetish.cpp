#include <telnetish/telnet-server.h>
#include <iostream>

int main(void) {

  TelnetServer server;

  server.setPort(3000);
  server.onMessage(server.defaultPrintStdoutMessageListener());

  server.onClientConnected([](TelnetServer::TelnetServerEvent event){
    std::cout << "New client connected!\n";
    event.getConnection() << "Hello!";
  });

  server.start();

  return 0;
};
