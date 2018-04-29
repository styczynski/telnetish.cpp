/** @file
*
*  Implmentation of telnet server application.
*
*  @author Piotr Styczyński <piotrsty1@gmail.com>
*  @copyright MIT
*  @date 2018-04-29
*/
#include <signal.h>
#include <telnetish/simple-telnet-server.h>
#include <telnetish/term-program.h>
#include <telnetish/message.h>

#include "renderer.h"

/**
 * Configuration options for the server.
 */
struct ServerOptions {
  bool selfMode = false;
  bool verboseMode = false;
} serverOptions;

/**
 * Print application usage information
 */
void printUsage() {
  std::cout << "Usage: server <port> [--local | -l] [--nosplash]\n";
  std::cout << "              [--nocolors | -nc] [--verbose | -v]\n";
  std::cout << "\n";
  std::cout << "  --local    [ -l ]    Run renderer without server (used to test ncurses output)\n";
  std::cout << "  --nosplash           Disable animated splash before displaying menu\n";
  std::cout << "  --nocolors [ -nc ]   Disable any text colouring\n";
  std::cout << "  --verbose  [ -v ]    Verbose mode\n";
  std::cout << "\n";
  exit(-1);
}

int main(int argc, char** argv) {

  // Ignore sigpipe errors
  signal(SIGPIPE, SIG_IGN);
  
  // Invalid number of arguments
  if(argc<=1) {
    printUsage();
  }
  
  // Parse cli flags
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
  
  // Check if port is valid number
  try {
    std::stoi(std::string(argv[1]));
  } catch(...) {
    printUsage();
  }
  
  // Setup subprogram
  TermProgram program(renderer);
  if(serverOptions.selfMode) {
    // Run program in un-controlled mode if self mode setting was specified
    program.start();
    return 0;
  }
  
  // Setup the telent server
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
  
  // Start server
  server.start();

  return 0;
  
}