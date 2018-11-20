#pragma once

///////////////////////////////////////////////////////
// 打算使用面向对象封装一个SOCKET编程的接口
///////////////////////////////////////////////////////
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <iostream>


class SocketTcpCpp{
public:
  SocketTcpCpp(){
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = inet_addr(_ip.c_str());
    _addr.sin_port = htons(_port);
    _fd = socket(AF_INET, SOCK_STREAM, 0);
  }
  SocketTcpCpp(const SocketTcpCpp&);
  SocketTcpCpp& operator=(const SocketTcpCpp&);
  ~SocketTcpCpp(){

  }

private:
  int _fd; //socket 套接字
  std::string _ip;  //server ip地址
  int _port; //server 端口号
  struct sockaddr_in _addr; //server 接口
};

