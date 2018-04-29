/** @file
*
*  Utilities to run subprograms.
*
*  @author Piotr Styczyński <piotrsty1@gmail.com>
*  @copyright MIT
*  @date 2018-04-29
*/
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

/**
 * @def RCV_MAX_LOOP_COUNT
 *
 * Maximum number of loops done before TermProgram receive function terminates.
 */
#define RCV_MAX_LOOP_COUNT 10

/**
 * Class representing runnable subprogram.
 *
 *  By default each subprogram runs in the default routine of the application.
 * In the controlled mode the application is forked and made running the subprogram.
 * In this controlled mode all input/output is provided by the host.
 *
 * In non controlled mode subprogram starts in the same routine and there is no way
 * to provide output/input for it programatically (input is extracted from stdin and
 * output from stdout).
 *
 * So in:
 *  - uncontrolled mode:
 *    start() is blocking and send/receive wont work as desired
 *  - controlled mode:
 *    start() is non-blocking and send/receive works as pipe interface
 */
class TermProgram {
public:
  using RendererFn = std::function<void(TermProgram&)>;
  using MessageSentCallbackFn = std::function<void(TermProgram&, Message&)>;
  using MessageReceivedCallbackFn = std::function<void(TermProgram&, Message&)>;
  using WaitingFn = std::function<bool(TermProgram&)>;
  
  
private:
  int pid;
  int p[2];
  int r[2];
  std::string terminalTypeName;
  bool inited;
  bool controlled;
  RendererFn body;
  MessageSentCallbackFn sentCallback;
  MessageReceivedCallbackFn receivedCallback;
  
public:

  /**
   * Create new program from function.
   */
  TermProgram(RendererFn bodyFn) {
    inited = false;
    controlled = false;
    body = bodyFn;
    sentCallback = [](TermProgram& rp, Message& m)->void {};
    receivedCallback = [](TermProgram& rp, Message& m)->void {};
    terminalTypeName = "";
  }
  
  /**
   * Create new empty program.
   */
  TermProgram() {
    TermProgram([](TermProgram& program)->void {});
  }
  
  /**
   * Set terminal type for the program.
   *
   * @param[in] type : terminal type name
   */
  void setTerminalType(std::string type) {
    terminalTypeName = type;
  }
  
  /**
   * Get terminal type for the program.
   * If the terminal type was not specified or it's empty string
   * then "UNKNOWN" is returned.
   *
   * @return terminal type name
   */
  std::string getTerminalType() {
    if(terminalTypeName == "") {
      return "UNKNOWN";
    }
    return terminalTypeName;
  }
  
  /**
   * Set if program should be controlled.
   * This have effect only before start() call.
   *
   * @param[in] value : Set controlled mode?
   */
  void setControlled(bool value = true) {
    controlled = value;
  }
  
  /**
   * Set program function body.
   *
   * @param[in] fn : set subprogram main function
   */
  void setBody(RendererFn fn) {
    body = fn;
  }
  
  /**
   * Receive new message from the subprogram (non-blocking).
   *
   * @return Received message
   */
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
  
  /**
   * Send new message to the subprogram (non-blocking).
   *
   * @param[in] message : Message to be sent
   */
  void send(Message message) {
    for(int i=1;i<message.getSize();++i) {
      if(message[i-1] == 13 && message[i] == 10) {
        message.setByte(i-1, '?');
      }
    }
    write(r[1], message.getContents(), message.getSize());
    sentCallback(*this, message);
  }
  
  /**
   * Receive new message from the subprogram (non-blocking).
   *
   * @param[in] output : Message that received content will be saved to
   * @return self
   */
  TermProgram& operator>>(Message& output) {
    output = receive();
    return *this;
  }
  
   /**
   * Send new message to the subprogram (non-blocking).
   *
   * @param[in] input : Message to be sent
   * @return self
   */
  TermProgram& operator<<(Message input) {
    send(input);
    return *this;
  }
  
  /**
   * Add message received listener.
   *
   * @param[in] fn : listener listening to the received messages
   */
  void onMessageReceived(MessageReceivedCallbackFn fn) {
    const auto next = receivedCallback;
    receivedCallback = [fn, next](TermProgram& rp, Message& m)->void {
      fn(rp, m);
      next(rp, m);
    };
  }
  
  /**
   * Add message sent listener.
   *
   * @param[in] fn : listener listening to the sent messages
   */
  void onMessageSent(MessageSentCallbackFn fn) {
    const auto next = sentCallback;
    sentCallback = [fn, next](TermProgram& rp, Message& m)->void {
      fn(rp, m);
      next(rp, m);
    };
  }
  
  /**
   * Wait for subprogram termination receiving all messages and triggering listeners.
   * This function is blocking.
   * It will terminate only when program is non-controlled or has been terminated.
   * The function accepts waiting function triggered in a infinite loop util termination
   * has occurred.
   * 
   * @param[in] waitFn : waiting function executed in a loop 
   */
  void wait(WaitingFn waitFn) {
    if(!controlled) return;
    
    int flags = fcntl(p[0], F_GETFL, 0);
    fcntl(p[0], F_SETFL, flags | O_NONBLOCK);
    
    flags = fcntl(r[1], F_GETFL, 0);
    fcntl(r[1], F_SETFL, flags | O_NONBLOCK);
    
    while(true) {
      if(!waitFn(*this)) {
        return;
      }
      receive();
      
      int status = 0;
      int ret = waitpid(pid, &status, WNOHANG);
      if(ret != 0) {
        return;
      }
    
    }
  }
  
  /**
   * Starts the subprogram
   */
  int start() {
    
    inited = true;
    
    if(!controlled) {
      int flags = fcntl(0, F_GETFL, 0);
      fcntl(0, F_SETFL, flags & (~O_NONBLOCK));
      if(terminalTypeName.size() > 0) {
        newterm(terminalTypeName.c_str(), stdout, stdin);
      } else {
        initscr();
      }
      body(*this);
      return 0;
    }
    
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
        if(terminalTypeName.size() > 0) {
          newterm(terminalTypeName.c_str(), stdout, stdin);
        } else {
          initscr();
        }
        body(*this);
        exit(0);
        break;

      default:
        int flags = fcntl(p[0], F_GETFL, 0);
        fcntl(p[0], F_SETFL, flags | O_NONBLOCK);
        
        flags = fcntl(r[1], F_GETFL, 0);
        fcntl(r[1], F_SETFL, flags | O_NONBLOCK);
        break;
    }
  }
  
  /**
   * Forcefully kills the program.
   * (does not work in un-controlled mode)
   */
  void end() {
    
    if(inited) {
      inited = false;
      kill(pid, SIGKILL);
    } else {
      return;
    }
    
    
  }
  
  ~TermProgram() {
    //this->end();
  }
};


#endif /* __TERM_PROGRAM_H__ */
