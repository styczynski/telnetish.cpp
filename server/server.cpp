#include <telnetish/tcp-server.h>
#include <iostream>

int main(void) {

  TCPServer server;

  server.setPort(3000);

  server.onMessage(server.defaultPrintStdoutMessageListener());

  server.onClientConnected([](TCPServerEvent event){
    std::cout << "New client connected!\n";
    event.getConnection() << "Hello!";
  });

  server.start();

  return 0;
};
