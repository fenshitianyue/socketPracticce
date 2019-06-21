#pragma once

#include <cassert>
#include <unistd.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define CHECK_RET(exp) if(!(exp)){ return false; }

class TcpSocket{
public:
  TcpSocket() : _fd(-1){}
  TcpSocket(int fd) : _fd(fd) {}

  bool Socket(){

  }

  bool Close() const {
    return true;
  }

  bool Bind() const {
    return true;
  }

  bool Listen(int num) const {
    
    return 0;
  }

  bool Accept() const {

  }

  bool Recv() const {

  }
  
  bool Send() const {

  }

  bool Connect() const {

  }

  int GetFd() const {
    return _fd;
  }
private:
  int _fd;
};

