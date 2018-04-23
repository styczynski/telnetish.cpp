#ifndef __TCP_CONNECTION_H__
#define __TCP_CONNECTION_H__

#define DEFAULT_CONNECTION_ID  -1
#define CONNECTION_NO_SOCKET   -1

class TCPConnection {
private:
  int socket;
  int id;
  bool opened;
  
public:
  
  TCPConnection(const int socket=CONNECTION_NO_SOCKET, const int id=DEFAULT_CONNECTION_ID);
  int getSocket() const;
  int getID() const;
  bool end();
  bool isOpened() const;
};


#endif /* __TCP_CONNECTION_H__ */