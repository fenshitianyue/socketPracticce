///////////////////////////////////////////////////////
// 这是一个TCP通用服务器框架
///////////////////////////////////////////////////////
#pragma once

#include <iostream>
#include <functional>
#include "tcp_socket.hpp"

using Handler = std::function<void(const std::string& req, std::string* resp)>;

class TcpServer{
public:
  TcpServer(const std::string& ip, uint16_t port) : _ip(ip),_port(port){}
  bool Start(Handler handler){
    //创建 socket
    CHECK_RET(_listen_sock.Socket());
    //绑定端口号
    CHECK_RET(_listen_sock.Bind(_ip, _port));
    //进行监听
    CHECK_RET(_listen_sock.Listen(5));
    //进入事件循环
    while(true){
      //进行accept
      TcpSocket new_sock; 
      std::string ip;
      uint16_t port = 0;
      if(!_listen_sock.Accept(&new_sock, &ip, &port)){
        continue;
      }
      std::cout << "[client " << ip.data() << ":" << port << "] connect!" << std::endl;
      //循环读写
      while(true){
        std::string req;
        if(!new_sock.Recv(&req)){
          std::cout << "[client " << ip.data() << ":" << port << "] disconnect!" << std::endl;
          new_sock.Close(); 
          break;
        }
        //计算响应
        std::string resp;
        handler(req, &resp);
        //发送响应
        if(!new_sock.Send(resp)){
          return false;
        }
      }
    }
    return true;
  }
private:
  TcpSocket _listen_sock;
  std::string _ip;
  uint64_t _port;
};

