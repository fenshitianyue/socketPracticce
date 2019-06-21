#pragma once

#include <iostream>
#include <functional>
#include <signal.h>
#include "tcp_socket.hpp"

using Handler = std::function<void (const std::string& req, std::string* resp)>;

//多进程版本的TCP服务器
class TcpProcessServer{
public:
  TcpProcessServer(const std::string& ip, uint16_t port) : _ip(ip), _port(port) {
    signal(SIGCHLD, SIG_IGN);
  }

  void ProcessConnect(const TcpSocket& new_sock, const std::string& ip, uint16_t port, Handler handler){
    int ret = fork();
    if(ret > 0){ //father process
      new_sock.Close();
      return;
    }else if(0 == ret){ //child process
      while(true){
        std::string req;
        bool ret = new_sock.Recv(&req);
        if(!ret){
          std::cout << "[client " << ip.data() << ":" << port << "] disconnected!" << std::endl;
          exit(0);
        }
        std::string resp;
        handler(req, &resp);
        if(!new_sock.Send(resp)){
          perror("send");
          exit(1);
        }
      }
    }else{
      perror("fork");
    }
  }

  bool Start(Handler handler){
    CHECK_RET(_listen_sock.Socket());
    CHECK_RET(_listen_sock.Bind(_ip, _port)); //绑定服务器 IP 和 port
    CHECK_RET(_listen_sock.Listen(10));
    while(true){
      TcpSocket new_sock;
      std::string ip;
      uint16_t port = 0;
      if(!_listen_sock.Accept(&new_sock, &ip, &port)){
        continue;
      }
      std::cout << "[client " << ip.data() << ":" << port << "] connect!" << std::endl;     
      ProcessConnect(new_sock, ip, port, handler);
    }
    return true;
  }
private:
  TcpSocket _listen_sock;
  std::string _ip;
  uint16_t _port;
};
