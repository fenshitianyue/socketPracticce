#pragma once

#include <iostream>
#include <functional>
#include <pthread.h>
#include "tcp_socket.hpp"

using Handler = std::function<void (const std::string&, std::string*)>;

struct ThreadArg{
  TcpSocket new_sock;
  std::string ip;
  uint16_t port;
  Handler handler;
};

class TcpThreadServer{
public:
  TcpThreadServer(const std::string& ip, uint16_t port) : _ip(ip), _port(port){}
  bool Start(Handler handler){
    CHECK_RET(_listen_sock.Socket());
    CHECK_RET(_listen_sock.Bind(_ip, _port));
    CHECK_RET(_listen_sock.Listen(10));
    while(true){
      ThreadArg* arg = new ThreadArg();
      arg->handler = handler;
      if(!_listen_sock.Accept(&arg->new_sock, &arg->ip, &arg->port))
        continue;
      std::cout << "[client " <<  arg->ip.data() << ":" << arg->port << "] connect!" << std::endl;
      //创建线程完成具体的操作
      pthread_t tid;
      pthread_create(&tid, NULL, ThreadEntry, arg);
      pthread_detach(tid);
    }
    return true;
  }

  static void* ThreadEntry(void* arg){
    ThreadArg* p = reinterpret_cast<ThreadArg*>(arg);
    //处理单次请求
    ProcessConnect(p);
    //关闭文件描述符并释放内存
    p->new_sock.Close();
    delete p;
    return NULL;
  }

  static void ProcessConnect(ThreadArg* arg){
    while(true){
      std::string req;
      if(!(arg->new_sock.Recv(&req))){
        std::cout << "[client " <<  arg->ip.data() << ":" << arg->port << "] disconnect!" << std::endl;
        break;
      }
      std::string resp;
      arg->handler(req, &resp);
      arg->new_sock.Send(resp);
      //打印日志
    }
  }
private:
  TcpSocket _listen_sock;
  std::string _ip;
  uint16_t _port;
};
