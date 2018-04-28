#include <telnetish/simple-telnet-server.h>
#include <telnetish/term-program.h>
#include <telnetish/message.h>

#include "renderer.h"

struct ServerOptions {
  bool selfMode = false;
  bool verboseMode = false;
} serverOptions;

void printUsage() {
  std::cout << "Usage: server <port> [--local | -l] [--nosplash]\n";
  std::cout << "\n";
  std::cout << "  --local [ -l ]       Run renderer without server (used to test ncurses output)\n";
  std::cout << "  --nosplash           Disable animated splash before displaying menu\n";
  std::cout << "  --nocolors [ -nc ]   Disable any text colouring\n";
  std::cout << "  --verbose  [ -v ]    Verbose mode\n";
  std::cout << "\n";
  exit(-1);
}

int main(int argc, char** argv) {

  signal(SIGPIPE, SIG_IGN);
  
  if(argc<=1) {
    printUsage();
  }
  
  for(int i=2;i<argc;++i) {
    std::string opt(argv[i]);
    if(opt == "--local" || opt == "-l") {
      serverOptions.selfMode = true;
    } else if(opt == "--verbose" || opt == "-v") {
      serverOptions.verboseMode = true;
    } else if(opt == "--nosplash") {
      rendererOptions.noSplash = true;
    } else if(opt == "--nocolors" || opt == "-nc") {
      rendererOptions.noColors = true;
    } else {
      printUsage();
    }
  }
  
  try {
    std::stoi(std::string(argv[1]));
  } catch(...) {
    printUsage();
  }
  
  TermProgram program(renderer);
  if(serverOptions.selfMode) {
    program.start();
    return 0;
  }
  
  SimpleTelnetServer server(program);
  server.setPort(argv[1]);
  server.setVerbose(serverOptions.verboseMode);
  server.onMessage(server.defaultPrintStdoutMessageListener());
  server.onMessage([](LoggableMessage message){
    if(message.getLevel() <= LOG_MESSAGE_ERROR) {
      std::cout << message.toString() << "\n";
      std::cout << "  Terminatting due to the error...\n";
      std::cout.flush();
      exit(-1);
    }
  });
  server.start();

  return 0;
  
}