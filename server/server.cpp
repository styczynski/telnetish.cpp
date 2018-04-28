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


int menu1(int, TermProgram&);
int menu2(int, TermProgram&);

std::string menu_message = "                         ";

void menu_end(TermProgram& program) {
  time_t t;
  srand((unsigned) time(&t));
  
  clear();
  refresh();
  
  int len = 0;
  for(int t=0;t<1000;++t) {
    move(rand() % 25 + 1, rand() % 75 + 5);
    addch(' ');
    usleep(100);
    refresh();
  }
  
  for(int y=1;y<=25;++y) {
    for(int x=5;x<=80;++x) {
      move(y, x);
      addch(' ');
    }
  }
  
  refresh();

  usleep(100000);
  mvprintw(5, 15, "GOODBYE :)");
  mvprintw(7, 10, "Please feel free to leave a feedback.");
  mvprintw(8, 10, "I would like to know about your telnet experiences.");
  mvprintw(9, 10, "Have a nice day!");
  mvprintw(11, 15, "Piotr Styczynski");
  refresh();
  
  usleep(3500000);
}

void menu_start(TermProgram& program) {
  
  time_t t;
  srand((unsigned) time(&t));
  
  clear();
  int len = 0;
  for(int t=0;t<1000;++t) {
    move(rand() % 25 + 1, rand() % 75 + 5);
    addch('#');
    usleep(100);
    refresh();
  }
  
  for(int y=1;y<=25;++y) {
    for(int x=5;x<=80;++x) {
      move(y, x);
      addch('#');
    }
  }
  refresh();
  
  menu1(0, program);
}

int menu1(int first_opt, TermProgram& program) {
    
    //clear();
    for(int y=1;y<=25;++y) {
      for(int x=5;x<=80;++x) {
        move(y, x);
        addch('#');
      }
    }
    refresh();
    
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
    int ch = '?';
    int i=first_opt;
 
    WINDOW *menuWindow;
    WINDOW *descriptionWindow;
    
    menuWindow = newwin(8, 20, 5, 10);
    descriptionWindow = newwin(14, 50, 5, 31);
    
    keypad(menuWindow, TRUE);
     
    attron(COLOR_PAIR(2));
    mvprintw(26, 20, menu_message.c_str());
    attroff(COLOR_PAIR(2));
    refresh();
     
    do { 
    
      //clear();
      wclear(menuWindow);
      wclear(descriptionWindow);
      refresh();
    
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
    
      mvprintw(2, 12, "[ Terminal type: %s ]", program.getTerminalType().c_str());
    
      attron(COLOR_PAIR(2));
      mvprintw(26, 20, menu_message.c_str());
      attroff(COLOR_PAIR(2));
    
      for(int j=0; j<3; j++) {
        if(j==i) {
            wattron(menuWindow, A_STANDOUT);
        } else {
            wattroff(menuWindow, A_STANDOUT);
        }
        sprintf(item, "%-16s",  list[j].c_str());
        mvwprintw(menuWindow, j+2, 2, "%s", item);
      }
      
      sprintf(item, "%-16s",  list[i].c_str());
      
      box(menuWindow, 0, 0); 
      box(descriptionWindow, 0, 0);
      
      keypad(menuWindow, TRUE);
      wclear(descriptionWindow);
      box(descriptionWindow, 0, 0);
      
      wattron(descriptionWindow, COLOR_PAIR(1));
      mvwprintw(descriptionWindow, 3, 2, "[ %-10s ]", item);
      wattroff(descriptionWindow, COLOR_PAIR(1));
      
      mvwprintw(descriptionWindow, 5, 2, "%s", descriptions[i].c_str());
      box(descriptionWindow, 0, 0);
      wrefresh(descriptionWindow);
      
      wrefresh(menuWindow);
      wrefresh(descriptionWindow);
      refresh();
      
      move(0,0);
      addch(' ');
    } while((ch = wgetch(menuWindow)) != 10);
 
    delwin(descriptionWindow);
    delwin(menuWindow);
    
    if(i == 2) {
      menu_end(program);
      return -1;
    }
    
    if(i == 0) {
      menu_message = "      @ Selected A       ";
      return menu1(i, program);
    }
    
    return menu2(0, program);
}


int menu2(int first_opt, TermProgram& program) {
    
    //clear();
    for(int y=1;y<=25;++y) {
      for(int x=5;x<=80;++x) {
        move(y, x);
        addch('#');
      }
    }
    refresh();
    
    std::string list[] = {
      "Opcja B1",
      "Opcja B2",
      "Wstecz"
    };
    
    std::string descriptions[] = {
      "Wybierz opcje B1\n  Opcja B1 umozliwia duzo rzeczy :)",
      "Opcja B2 jest nawet lepsza niz opcja B1\n  Czemu niby masz zawsze wybierac opcje B1?\n  Nie dyskryminuj cyferek prosze :>",
      "Moge wrocic spowrotem?"
    };
    
    char item[20];
    int ch = '?';
    int i = first_opt;
 
    WINDOW *menuWindow;
    WINDOW *descriptionWindow;
    
    menuWindow = newwin(8, 20, 5, 10);
    descriptionWindow = newwin(14, 50, 5, 31);
    
    keypad(menuWindow, TRUE);
     
    attron(COLOR_PAIR(2));
    mvprintw(26, 20, menu_message.c_str());
    attroff(COLOR_PAIR(2));
    refresh();
     
    do { 
    
      //clear();
      wclear(menuWindow);
      wclear(descriptionWindow);
      refresh();
    
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
    
      mvprintw(2, 12, "[ Terminal type: %s ]", program.getTerminalType().c_str());
    
      attron(COLOR_PAIR(2));
      mvprintw(26, 20, menu_message.c_str());
      attroff(COLOR_PAIR(2));
    
      for(int j=0; j<3; j++) {
        if(j==i) {
            wattron(menuWindow, A_STANDOUT);
        } else {
            wattroff(menuWindow, A_STANDOUT);
        }
        sprintf(item, "%-16s",  list[j].c_str());
        mvwprintw(menuWindow, j+2, 2, "%s", item);
      }
      
      sprintf(item, "%-16s",  list[i].c_str());
      
      box(menuWindow, 0, 0); 
      box(descriptionWindow, 0, 0);
      
      keypad(menuWindow, TRUE);
      wclear(descriptionWindow);
      box(descriptionWindow, 0, 0);
      
      wattron(descriptionWindow, COLOR_PAIR(1));
      mvwprintw(descriptionWindow, 3, 2, "[ %-10s ]", item);
      wattroff(descriptionWindow, COLOR_PAIR(1));
      
      mvwprintw(descriptionWindow, 5, 2, "%s", descriptions[i].c_str());
      box(descriptionWindow, 0, 0);
      wrefresh(descriptionWindow);
      
      wrefresh(menuWindow);
      wrefresh(descriptionWindow);
      refresh();
      
      move(0,0);
      addch(' ');
    } while((ch = wgetch(menuWindow)) != 10);
 
    delwin(descriptionWindow);
    delwin(menuWindow);
    
    if(i == 2) {
      return menu1(0, program);
    }
    
    if(i == 0) {
      menu_message = "      @ Selected B1      ";
    } else {
      menu_message = "      @ Selected B2      ";
    }
    
    return menu2(i, program);
}

void renderer(TermProgram& program) {
  
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
    
  menu_start(program);

  echo();
  curs_set(1);
  refresh();
  endwin();
}

int main(int argc, char** argv) {

  signal(SIGPIPE, SIG_IGN);

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