#include <telnetish/tcp-server.h>

bool TCPServer::init() {

  if(this->inited) {
    return false;
  }

  this->log("Initializing server...");

  this->in_sock = socket(PF_INET, SOCK_STREAM, 0);
  if(this->in_sock < 0) {
    this->reportError("Could not init socket.");
    return false;
  }

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(this->port);

  this->log(std::string("Starting TCP server at port ")+std::to_string(this->port));
  
  if (bind(this->in_sock, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
    this->reportError("Could not bind socket to the specified port.");
    return false;
  }

  this->next_client_id = -1;

  this->log("Server initialized (ready).");

  return true;
}

void TCPServer::shutdown() {
  this->on = false;
}

bool TCPServer::start() {

  const int input_queue_length = DEFAULT_TCP_LISTEN_QUEUE_LENGTH;

  this->init();

  this->log("Switch to listening mode.");

  if(listen(this->in_sock, input_queue_length) < 0) {
    this->reportError("Could not switch server socket to the listening mode.");
    return false;
  }

  struct sockaddr_in client_address;

  this->log("Enter default client handler route.");
  this->on = true;

  while(this->on) {
    ++(this->next_client_id);

    socklen_t client_address_len = sizeof(client_address);

    this->log("Wait for incoming connection...");

    this->out_sock = accept(this->in_sock, (struct sockaddr *) &client_address, &client_address_len);
    if (this->out_sock < 0) {
      this->reportError("Could not accept client connection.");
      return false;
    }

    this->log("Connected to a new client. Setup connection and run handlers.");

    TCPConnection clientConnection(this->out_sock, this->next_client_id);
    this->clientConnected(TCPServerEvent(this, &clientConnection));

    this->log("Ending client connection.");

    if (!clientConnection.end()) {
      this->reportError("Could not close output socket.");
      return false;
    }

    this->clientDisconnected(TCPServerEvent(this, &clientConnection));

    this->log("Client connection dumped.");
  }

  this->log("Terminating server.");
  this->on = false;
}
