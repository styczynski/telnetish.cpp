#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <sstream>
#define MAX_LEN 99


#include <telnetish/telnet-server.h>
#include <iostream>
#include <unistd.h>
#include <ncurses.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>


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

        
    int cycle = 0, pid, p[2], row, col;

    if(pipe(p) == -1) {
        perror("pipe call error");
        return(1);
    }

    switch (pid=fork()) {
      case -1: perror("error: fork call");
        return(2);

      case 0: /* child */
        if(dup2(p[1], 1) == -1 ) {
          perror( "dup2 failed" );
          return(1);
        }
        setvbuf(stdout, NULL, _IOLBF, 1000);
        initscr();
        init_pair(1, COLOR_YELLOW, COLOR_GREEN);
        
        while(true) {
          attron(COLOR_PAIR(1));
          addch('-');
          attroff(COLOR_PAIR(1));
          
          refresh();
          
          cycle++;
          usleep(1000000);
        }
        break;

        endwin();
      
      default:
        std::string mystr;
        if(dup2(p[0], 0 ) == -1 ) {
          perror( "dup2 failed" );
          return(1);
        }
        
        std::stringstream ss;
        while(true) {
            
            char inp = 0;
            if(fscanf(stdin, "%c", &inp) > 0) {
              printf("%d ", (int)inp);
              if(inp == _IAC) {
                event.getConnection() << std::vector<int>({ _IAC, _IAC });
              } else {
                event.getConnection() << std::vector<int>({ (int)inp });
              }
              fflush(stdout);
            }
            
            usleep(100000);
        }

        break;
    }
  
    /*Message message;
    while(true) {
      event.getConnection() >> message;
      if(TelnetMessage::isCommand(message)) {

      } else {
        std::cout << "[TEXT] " << message.bytesDumpString() << "\n";
      }
    }*/
    
  });

  server.start();

  return 0;
  
}