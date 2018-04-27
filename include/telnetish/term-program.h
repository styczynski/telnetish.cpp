#ifndef __TERM_PROGRAM_H__
#define __TERM_PROGRAM_H__

#include <telnetish/message.h>
#include <iostream>
#include <unistd.h>
#include <ncurses.h>
#include <cstdlib>
#include <cstdio>
#include <functional>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>


#define REMOTE_TERMINAL_DEFAULT_BUFFER_SIZE  5064

#define RCV_MAX_LOOP_COUNT 10

class TermProgram {
public:
  using RendererFn = std::function<void(void)>;
  using MessageSentCallbackFn = std::function<void(TermProgram&, Message&)>;
  using MessageReceivedCallbackFn = std::function<void(TermProgram&, Message&)>;
  using WaitingFn = std::function<void(TermProgram&)>;
  
  
private:
  int pid;
  int p[2];
  int r[2];
  bool inited;
  RendererFn body;
  MessageSentCallbackFn sentCallback;
  MessageReceivedCallbackFn receivedCallback;
  
public:

  TermProgram(RendererFn bodyFn) {
    inited = false;
    body = bodyFn;
    sentCallback = [](TermProgram& rp, Message& m)->void {};
    receivedCallback = [](TermProgram& rp, Message& m)->void {};
  }
  
  TermProgram() {
    TermProgram([]()->void {});
  }
  
  void setBody(RendererFn fn) {
    body = fn;
  }
  
  Message receive() {
    std::string ret;
    
    char buf[4096];
    int loopCount = 0;
    int len = 0;
    
    while(len = read(p[0], buf, sizeof(buf))) {
      if(len < 0) {
        break;
      }
      
      ret += std::string(buf, len);
      ++loopCount;
      if(loopCount > RCV_MAX_LOOP_COUNT) {
        break;
      }
    }
    
    Message retMessage(ret);
    
    if(retMessage.getSize() > 0) {
      receivedCallback(*this, retMessage);
    }
    
    return retMessage;
  }
  
  void send(Message message) {
    write(r[1], message.getContents(), message.getSize());
    sentCallback(*this, message);
  }
  
  TermProgram& operator>>(Message& output) {
    output = receive();
    return *this;
  }
  
  TermProgram& operator<<(Message input) {
    send(input);
    return *this;
  }
  
  void onMessageReceived(MessageReceivedCallbackFn fn) {
    const auto next = receivedCallback;
    receivedCallback = [fn, next](TermProgram& rp, Message& m)->void {
      fn(rp, m);
      next(rp, m);
    };
  }
  
  void onMessageSent(MessageSentCallbackFn fn) {
    const auto next = sentCallback;
    sentCallback = [fn, next](TermProgram& rp, Message& m)->void {
      fn(rp, m);
      next(rp, m);
    };
  }
  
  void wait(WaitingFn waitFn) {
    int flags = fcntl(p[0], F_GETFL, 0);
    fcntl(p[0], F_SETFL, flags | O_NONBLOCK);
    while(true) {
      waitFn(*this);
      receive();
    }
  }
  
  int start() {
    
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

      case 0:
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
        body();
        break;

      default:
        int flags = fcntl(p[0], F_GETFL, 0);
        fcntl(p[0], F_SETFL, flags | O_NONBLOCK);
        break;
    }

    
  }
  
  void end() {
    
    if(inited) {
      inited = false;
    } else {
      return;
    }
    
    
  }
  
  ~TermProgram() {
    //this->end();
  }
};


#endif /* __TERM_PROGRAM_H__ */
