#ifndef __REMOTE_TERM_H__
#define __REMOTE_TERM_H__

#include <iostream>
#include <unistd.h>
#include <ncurses.h>
#include <cstdlib>
#include <cstdio>

#define REMOTE_TERMINAL_DEFAULT_BUFFER_SIZE  5064

class RemoteTerminal {
private:
  int    ncurses_pipe[2];
  FILE*  ncurses_in;
  FILE*  ncurses_out;
  bool   ncurses_on;
  bool   inited;
  char*  buffer;
  int    buffer_length;
  
public:
  RemoteTerminal() {
    this->inited = true;
    
    if(pipe(this->ncurses_pipe) == -1) {
      throw "Failed to create ncurses pipe";
    }
    
    this->ncurses_in  = fdopen(ncurses_pipe[1], "r");
    this->ncurses_out = fdopen(ncurses_pipe[0], "w");
    
    if(this->ncurses_in == NULL || this->ncurses_out == NULL) {
      throw "Failed to set up ncurses file streams";
    }
    
    this->buffer_length = REMOTE_TERMINAL_DEFAULT_BUFFER_SIZE;
    this->buffer = (char*) malloc(sizeof(char) * (this->buffer_length + 1));
    
    this->ncurses_on = false;
  }
  
  void writeString(std::string str) {
    write(this->ncurses_pipe[1], str.c_str(), str.size());
  }
  
  std::string readString() {
    const int len = read(this->ncurses_pipe[0], this->buffer, this->buffer_length);
    return std::string(this->buffer, len);
  }
  
  RemoteTerminal& operator<<(std::string str) {
    this->writeString(str);
    return *this;
  }
  
  RemoteTerminal& operator>>(std::string& str) {
    str = this->readString();
    return *this;
  }
  
  RemoteTerminal& operator>>(std::ostream& out) {
    std::string outStr;
    operator>>(outStr);
    out << outStr;
    return *this;
  }
  
  RemoteTerminal& operator<<(std::istream& in) {
    std::string inStr;
    in >> inStr;
    operator<<(inStr);
    return *this;
  }
  
  void start() {
    if(this->ncurses_on) return;
    if(!this->inited) return;
    
    newterm(NULL, this->ncurses_out, this->ncurses_in);
    
    this->ncurses_on = true;
  }
  
  void end() {
    
    if(this->inited) {
      this->inited = false;
    } else {
      return;
    }
    
    if(this->ncurses_on) {
      endwin();
      this->ncurses_on = false;
    }
    
    fclose(this->ncurses_in);
    fclose(this->ncurses_out);
    close(this->ncurses_pipe[0]);
    close(this->ncurses_pipe[1]);
    
    free(this->buffer);
    this->buffer = nullptr;
    this->buffer_length = 0;
  }
  
  ~RemoteTerminal() {
    this->end();
  }
};


#endif /* __REMOTE_TERM_H__ */
