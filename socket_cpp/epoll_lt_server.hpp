#pragma once

#include <vector>
#include <functional>
#include <sys/epoll.h>
#include "tcp_socket.hpp"

using Handler = std::function<void (const std::string&, std::string*)>;

class Epoll{
public: 
  Epoll(){
    _epoll_fd = epoll_create(10);
  }

  ~Epoll(){
    close(_epoll_fd);
  }
  bool Add(const TcpSocket& sock) const {
    int fd = sock.GetFd();
    epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    if(epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fd, &ev)){
      perror("epoll_ctl ADD");
      return false;
    }
    return true;
  }
  
  bool Del(const TcpSocket& sock) const {
    int fd = sock.GetFd();
    if(epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, NULL)){
      perror("epoll_ctl DEL");
      return false;
    }
    return true;
  }

  bool Wait(std::vector<TcpSocket>* output) const {
    output->clear(); 
    epoll_event events[1024];
    int nfds = epoll_wait(_epoll_fd, events, sizeof(events) / sizeof(events[0]), -1);
    if(nfds < 0){
      perror("epoll_wait"); 
      return false;
    }
    //将就绪的socket句柄添加到output中
    for(int i = 0; i != nfds; ++i){
      TcpSocket sock(events[i].data.fd);
      output->emplace_back(std::move(sock));
    }
    return true;
  }
private:
  int _epoll_fd;
};

class TcpEpollServer{
public:
  TcpEpollServer(const std::string& ip, uint16_t port) : _ip(ip), _port(port){}
  bool Start(Handler handler){
    //创建socket 
    TcpSocket listen_sock;
    CHECK_RET(listen_sock.Socket());
    //绑定
    CHECK_RET(listen_sock.Bind(_ip, _port));
    //监听
    CHECK_RET(listen_sock.Listen(10));
    //创建Epoll对象，并将listen_sock添加到Epoll对象中
    Epoll epoll;
    epoll.Add(listen_sock);
    //进入事件循环
    while(true){
      //epoll_wait
      std::vector<TcpSocket> output;
      if(!epoll.Wait(&output))
        continue;
      //根据就绪文件描述符的种类进行处理
      for(size_t i = 0; i != output.size(); ++i){
        //如果是listen_sock, 就调用accept, 并将new_sock添加到Epoll对象中
        if(output[i].GetFd() == listen_sock.GetFd()){
          TcpSocket new_sock;
          listen_sock.Accept(&new_sock);
          epoll.Add(new_sock);
        }else{//如果是new_sock, 就进行一次读写
          std::string req, resp;
          bool ret = output[i].Recv(&req);
          if(!ret){
            epoll.Del(output[i]);
            output[i].Close();
            continue;
          }
          handler(req, &resp);
          output[i].Send(resp);
        }
      }//end for(size_t ...)
    }//end while(true)
    return true;
  }
private:
  std::string _ip;  
  uint16_t _port;
};

