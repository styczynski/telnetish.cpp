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

    if(true) {
  
    int cycle = 0, pid, p[2], r[2], row, col;

    if(pipe(p) == -1) {
        perror("pipe call error");
        return(1);
    }
    
    if(pipe(r) == -1) {
        perror("pipe call error");
        return(1);
    }

    char c = ' ';
    switch (pid=fork()) {
      case -1: perror("error: fork call");
        return(2);

      case 0: /* child */
        if(dup2(p[1], 1) == -1 ) {
          perror( "dup2 failed" );
          return(1);
        }
        
        if(dup2(r[0], 0) == -1 ) {
          perror( "dup2 failed" );
          return(1);
        }
        
        setvbuf(stdout, NULL, _IOLBF, 1000);
        initscr();
        
        if(has_colors() == FALSE) {
          endwin();
          printf("Your terminal does not support color\n");
          exit(1);
        }
        
        start_color();
        //init_pair(1, COLOR_BLACK, COLOR_WHITE);
        
        
        clear();
        
        while(true) {
          c = getch();
          
          clear();
          for(int y=10;y<=20;++y) {
            for(int x=10;x<=20;++x) {
              move(x, y);
              if(x==10 || x==20 || y==10 || y==20) {
                printw("#");
              }
            }
          }
          
          move(15, 15);
          addch(c);
          
          refresh();          
          printf("\n");
          fflush(stdout);
          
          cycle++;
          
        }
        break;

        endwin();
      
      default:
        std::string mystr;
        
        std::stringstream ss;
        Message m;

        int flags = fcntl(p[0], F_GETFL, 0);
        fcntl(p[0], F_SETFL, flags | O_NONBLOCK);
        
        while(true) {
            
            char buf[4096];
            int len = 0;
            
            event.getConnection() >> m;
            
            if(TelnetMessage::isCommand(m)) {
              std::cout << "[COMMAND] " << TelnetMessage::commandDescription(m) << "\n";
            } else {

              write(r[1], m.getContents(), m.getSize());
              std::cout << "[TEXT]    " << m.toString() << "\n";
              
              usleep(5000);
              
              int refcount = 0;
              while(len = read(p[0], buf, sizeof(buf))) {
                event.getConnection().writeData(buf, len);
                ++refcount;
                if(refcount > 10) break;
              }
              
            }
        }

        break;
    }
  
    }
  
    Message message;
    while(true) {
      event.getConnection() >> message;
  
      if(TelnetMessage::isCommand(message)) {
        std::cout << "[COMMAND] " << TelnetMessage::commandDescription(message) << "\n";
      } else {
        std::cout << "[TEXT]    " << message.toString() << "\n";
      }
    }
    
  });

  server.start();

  return 0;
  
}