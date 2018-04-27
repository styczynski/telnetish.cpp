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


int menu1(int);
int menu2(int);

void menu_end() {
  time_t t;
  srand((unsigned) time(&t));
  
  int len = 0;
  for(int t=0;t<1000;++t) {
    move(rand() % 25 + 10, rand() % 80 + 10);
    addch(' ');
    usleep(100);
    refresh();
  }
  
  for(int y=10;y<=35;++y) {
    for(int x=10;x<=90;++x) {
      move(y, x);
      addch(' ');
    }
  }
  
  refresh();

  usleep(100000);
  mvprintw(15, 15, "GOODBYE :)");
  mvprintw(17, 10, "Please feel free to leave a feedback.");
  mvprintw(18, 10, "I would like to know about your telnet experiences.");
  mvprintw(19, 10, "Have a nice day!");
  mvprintw(21, 15, "Piotr Stczynski");
  refresh();
  
  usleep(3500000);
}

void menu_start() {
  
  time_t t;
  srand((unsigned) time(&t));
  
  clear();
  int len = 0;
  for(int t=0;t<1000;++t) {
    move(rand() % 25 + 10, rand() % 80 + 10);
    addch('#');
    usleep(100);
    refresh();
  }
  
  for(int y=10;y<=35;++y) {
    for(int x=10;x<=90;++x) {
      move(y, x);
      addch('#');
    }
  }
  refresh();
  
  menu1(0);
}

int menu1(int first_opt) {
    
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
    int i=first_opt;
 
    WINDOW *menuWindow;
    WINDOW *descriptionWindow;
    
    menuWindow = newwin(8, 20, 15, 15);
    descriptionWindow = newwin(14, 50, 15, 36);
    
    box(menuWindow, 0, 0); 
    box(descriptionWindow, 0, 0);
    
    for(int i=0; i<3; i++ ) {
      if(i==first_opt) {
          wattron(menuWindow, A_STANDOUT);
      } else {
          wattroff(menuWindow, A_STANDOUT);
      }
      sprintf(item, "%-16s",  list[i].c_str());
      mvwprintw(menuWindow, i+2, 2, "%s", item);
    }
 
    wrefresh(menuWindow);
    wrefresh(descriptionWindow);
 
    keypad(menuWindow, TRUE);
     
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
      menu_end();
      return -1;
    }
    
    if(i == 0) {
      attron(COLOR_PAIR(2));
      mvprintw(25, 20, "@ Selected A ");
      attroff(COLOR_PAIR(2));
      refresh();
      return menu1(i);
    }
    
    return menu2(0);
}


int menu2(int first_opt) {
    
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
    int i = first_opt;
 
    WINDOW *menuWindow;
    WINDOW *descriptionWindow;
    
    menuWindow = newwin(8, 20, 15, 15);
    descriptionWindow = newwin(14, 50, 15, 36);
    
    box(menuWindow, 0, 0); 
    box(descriptionWindow, 0, 0);
    
    for(int i=0; i<3; i++ ) {
      if(i==first_opt) {
          wattron(menuWindow, A_STANDOUT);
      } else {
          wattroff(menuWindow, A_STANDOUT);
      }
      sprintf(item, "%-16s",  list[i].c_str());
      mvwprintw(menuWindow, i+2, 2, "%s", item);
    }
 
    wrefresh(menuWindow);
    wrefresh(descriptionWindow);
 
    keypad(menuWindow, TRUE);
     
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
      return menu1(0);
    }
    
    attron(COLOR_PAIR(2));
    if(i == 0) {
      mvprintw(25, 20, "@ Selected B1");
    } else {
      mvprintw(25, 20, "@ Selected B2");
    }
    attroff(COLOR_PAIR(2));
    refresh();
    
    return menu2(i);
}

void renderer() {
  
  if(has_colors() == FALSE) {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }
  
  noecho();
  curs_set(0);
    
  start_color();
  init_pair(1, COLOR_BLACK, COLOR_WHITE);
  init_pair(2, COLOR_WHITE, COLOR_BLUE);
    
  menu_start();

  echo();
  curs_set(1);
  refresh();
  endwin();
}

int main(int argc, char** argv) {

  if(argc<=1) {
    std::cout << "Usage: server [port]\n";
    return 1;
  }

  TermProgram program(renderer);
  //program.start();
  //return 0;
  
  SimpleTelnetServer server(program);
  server.setPort(argv[1]);
  server.onMessage(server.defaultPrintStdoutMessageListener());
  server.start();

  return 0;
  
}