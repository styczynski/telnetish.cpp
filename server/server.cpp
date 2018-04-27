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


int menu1();
int menu2();


int menu1() {
    
    if(has_colors() == FALSE) {
      endwin();
      printf("Your terminal does not support color\n");
      exit(1);
    }
    
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    
    std::string list[] = {
      "Opcja A",
      "Opcja B",
      "Koniec"
    };
    
    std::string descriptions[] = {
      "Wybierz opcje A\n  Opcja A umozliwia duzo rzeczy :)",
      "Opcja B jest nawet lepsza niz opcja A\n  Czemu niby masz zawsze wybierac opcje A?\n  Nie dyskryminuj literek prosze :>",
      "E tam... Ide stad!"
    };
    
    char item[20];
    int ch;
    int i=0;
 
    WINDOW *menuWindow;
    WINDOW *descriptionWindow;
    
    menuWindow = newwin(8, 20, 15, 15);
    descriptionWindow = newwin(14, 50, 15, 36);
    
    box(menuWindow, 0, 0); 
    box(descriptionWindow, 0, 0);
    
    for(int i=0; i<3; i++ ) {
      if(i==0) {
          wattron(menuWindow, A_STANDOUT);
      } else {
          wattroff(menuWindow, A_STANDOUT);
      }
      sprintf(item, "%-16s",  list[i].c_str());
      mvwprintw(menuWindow, i+2, 2, "%s", item);
    }
 
    wrefresh(menuWindow);
    wrefresh(descriptionWindow);
 
    i=0;
    noecho();
    keypad(menuWindow, TRUE);
    curs_set(0);
     
    while((ch = wgetch(menuWindow)) != 10){ 
      
      sprintf(item, "%-16s", list[i].c_str()); 
      mvwprintw(menuWindow, i+2, 2, "%s", item); 

      switch(ch) {
        case KEY_UP:
          i--;
          i = (i<0)?2:i;
          break;
        case KEY_DOWN:
          i++;
          i = (i>2)?0:i;
          break;
      }
      wattron(menuWindow, A_STANDOUT);
       
      sprintf(item, "%-16s",  list[i].c_str());
      mvwprintw(menuWindow, i+2, 2, "%s", item);
      wattroff(menuWindow, A_STANDOUT);
      
      wclear(descriptionWindow);
      box(descriptionWindow, 0, 0);
      
      wattron(descriptionWindow, COLOR_PAIR(1));
      mvwprintw(descriptionWindow, 3, 2, "[ %-10s ]", item);
      wattroff(descriptionWindow, COLOR_PAIR(1));
      
      mvwprintw(descriptionWindow, 5, 2, "%s", descriptions[i].c_str());
      box(descriptionWindow, 0, 0);
      wrefresh(descriptionWindow);
    }
 
    delwin(descriptionWindow);
    delwin(menuWindow);
    
    if(i == 2) {
      endwin();
      return 0;
    }
    
    return menu2();
}


int menu2() {
    
    if(has_colors() == FALSE) {
      endwin();
      printf("Your terminal does not support color\n");
      exit(1);
    }
    
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    
    std::string list[] = {
      "Opcja B1",
      "Opcja B2",
      "Wstecz"
    };
    
    std::string descriptions[] = {
      "Wybierz opcje A\n  Opcja A umozliwia duzo rzeczy :)",
      "Opcja B jest nawet lepsza niz opcja A\n  Czemu niby masz zawsze wybierac opcje A?\n  Nie dyskryminuj literek prosze :>",
      "E tam... Ide stad!"
    };
    
    char item[20];
    int ch;
    int i=0;
 
    WINDOW *menuWindow;
    WINDOW *descriptionWindow;
    
    menuWindow = newwin(8, 20, 15, 15);
    descriptionWindow = newwin(14, 50, 15, 36);
    
    box(menuWindow, 0, 0); 
    box(descriptionWindow, 0, 0);
    
    for(int i=0; i<3; i++ ) {
      if(i==0) {
          wattron(menuWindow, A_STANDOUT);
      } else {
          wattroff(menuWindow, A_STANDOUT);
      }
      sprintf(item, "%-16s",  list[i].c_str());
      mvwprintw(menuWindow, i+2, 2, "%s", item);
    }
 
    wrefresh(menuWindow);
    wrefresh(descriptionWindow);
 
    i=0;
    noecho();
    keypad(menuWindow, TRUE);
    curs_set(0);
     
    while((ch = wgetch(menuWindow)) != 10){ 
      
      sprintf(item, "%-16s", list[i].c_str()); 
      mvwprintw(menuWindow, i+2, 2, "%s", item); 

      switch(ch) {
        case KEY_UP:
          i--;
          i = (i<0)?2:i;
          break;
        case KEY_DOWN:
          i++;
          i = (i>2)?0:i;
          break;
      }
      wattron(menuWindow, A_STANDOUT);
       
      sprintf(item, "%-16s",  list[i].c_str());
      mvwprintw(menuWindow, i+2, 2, "%s", item);
      wattroff(menuWindow, A_STANDOUT);
      
      wclear(descriptionWindow);
      box(descriptionWindow, 0, 0);
      
      wattron(descriptionWindow, COLOR_PAIR(1));
      mvwprintw(descriptionWindow, 3, 2, "[ %-10s ]", item);
      wattroff(descriptionWindow, COLOR_PAIR(1));
      
      mvwprintw(descriptionWindow, 5, 2, "%s", descriptions[i].c_str());
      box(descriptionWindow, 0, 0);
      wrefresh(descriptionWindow);
    }
 
    delwin(descriptionWindow);
    delwin(menuWindow);
    
    if(i == 2) {
      return menu1();
    }
    
    return i;
}

void renderer() {
  menu1();
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