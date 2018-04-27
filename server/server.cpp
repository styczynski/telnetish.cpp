#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <sstream>
#define MAX_LEN 99

#include <telnetish/simple-telnet-server.h>
#include <telnetish/term-program.h>
#include <telnetish/message.h>
#include <iostream>
#include <unistd.h>
#include <ncurses.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>


void renderer() {
  if(has_colors() == FALSE) {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }
  
  start_color();
  //init_pair(1, COLOR_BLACK, COLOR_WHITE);
  clear();
  char c = ' ';
  
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
  }
}

int main(int argc, char** argv) {

  if(argc<=1) {
    std::cout << "Usage: server [port]\n";
    return 1;
  }

  TermProgram program(renderer);
  program.start();
  return 0;
  
  SimpleTelnetServer server(program);
  server.setPort(argv[1]);
  server.onMessage(server.defaultPrintStdoutMessageListener());
  server.start();

  return 0;
  
}