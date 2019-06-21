///////////////////////////////////////////////////////
// 这是一个TCP通用客户端框架
///////////////////////////////////////////////////////
#pragma once

#include "tcp_socket.hpp"

class TcpClient{
public:
  TcpClient(const std::string& ip, uint16_t port) :_ip(ip),_port(port){
    _sock.Socket();
  }

  ~TcpClient(){
    _sock.Close();
  }

  bool Connect(){
    return _sock.Connect(_ip, _port);
  }

  bool Recv(std::string* buf){
    return _sock.Recv(buf);
  }

  bool Send(const std::string& buf){
    return _sock.Send(buf);
  }
private:
  TcpSocket _sock;
  std::string _ip;
  uint16_t _port;
};

