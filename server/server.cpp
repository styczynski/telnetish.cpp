#include <telnetish/tcp-server.h>
#include <iostream>

int main(void) {
  
  TCPServer server;
  
  server.setPort(80);
  
  server.onMessage(server.defaultPrintStdoutMessageListener());
  
  server.onClientConnected([](TCPServerEvent event){
    std::cout << "New client connected!\n";
  });
  
  server.start();
  
  return 0;
};