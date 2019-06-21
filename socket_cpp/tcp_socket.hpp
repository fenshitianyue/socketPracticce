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
    _fd = socket(AF_INET, SOCK_STREAM, 0); 
    if(_fd < 0){
      perror("socket");
      return false;
    }
    return true;
  }

  bool Close() const {
    close(_fd);
    return true;
  }

  bool Bind(const std::string& ip, uint16_t port) const {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.data());
    if(bind(_fd, (sockaddr*)&addr, sizeof(addr)) < 0){
      perror("bind");
      return false;
    }
    return true;
  }

  bool Listen(int num) const {
    if(listen(_fd, num) < 0){
      perror("listen");
      return false;
    } 
    return 0;
  }

  bool Accept(TcpSocket* peer, std::string* ip = NULL, uint16_t* port = NULL) const {
    sockaddr_in peer_addr;
    socklen_t len = sizeof(peer_addr);
    int new_sock;
    if((new_sock = accept(_fd, (sockaddr*)&peer_addr, &len)) < 0){
      perror("accept");
      return false;
    }
    peer->_fd = new_sock;
    if(ip != NULL)
      *ip = inet_ntoa(peer_addr.sin_addr);
    if(port != NULL)
      *port = ntohs(peer_addr.sin_port);
    return true;
  }

  bool Recv(std::string* buf) const {
    buf->clear();
    char tmp[4096] = {0};
    ssize_t read_size = recv(_fd, tmp, sizeof(tmp), 0);
    if(read_size < 0){ //recv 失败
      perror("recv");
      return false;
    }else if(0 == read_size){  //未读取到数据
      return false;
    }
    buf->assign(tmp, read_size); //填充读取缓冲区数据
    return true; 
  }
  
  bool Send(const std::string& buf) const {
    if(send(_fd, buf.data(), buf.size(), 0) < 0){
      perror("send");
      return false;
    }
    return true;
  }

  bool Connect(const std::string& ip, uint16_t port) const {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.data());
    if(connect(_fd, (sockaddr*)&addr, sizeof(addr)) < 0){
      perror("connect");
      return false;
    }
    return true;
  }

  int GetFd() const {
    return _fd;
  }
private:
  int _fd;
};

