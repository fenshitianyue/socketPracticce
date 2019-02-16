#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void ProcessConnect(int listen_fd, int epoll_fd){
  struct sockaddr_in client_addr;
  socklen_t len = sizeof(client_addr);
  int connect_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &len);
  if(connect_fd < 0){
    perror("accept");
    exit(EXIT_FAILURE);
  }
  printf("client %s:%d connect\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
  struct epoll_event ev;
  ev.data.fd = connect_fd;
  ev.events = EPOLLIN;
  int ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connect_fd, &ev);
  if(ret < 0){
    perror("epoll_ctl");
    exit(EXIT_FAILURE);
  }
}

void CheckError(const char str[], int n){
  if(n < 0){
    perror(str);
    exit(EXIT_FAILURE);
  }
}

void ProcessRequest(int connect_fd, int epoll_fd){
  char buf[1024] = {0};
  ssize_t read_size = read(connect_fd, buf, sizeof(buf) - 1);
  if(read_size < 0){
    perror("read");
    return;
  }
  if(0 == read_size){
    close(connect_fd);
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, connect_fd, NULL);
    printf("client say: goodbye\n");
    return;
  }
  printf("client say: %s\n", buf);
  write(connect_fd, buf, strlen(buf));
}

void CorrectUsage(){
  printf("Usage: ./epoll_server [ip] [port]\n");
}

int main(int argc, char* argv[]) {
  if(argc != 3){
    CorrectUsage();
    exit(EXIT_FAILURE);
  }   
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(atoi(argv[2]));
  addr.sin_addr.s_addr = inet_addr(argv[1]);
  int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  CheckError("socket", listen_fd);
  int ret = bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr));  
  CheckError("bind", ret);
  ret = listen(listen_fd, 10);
  CheckError("listen", ret);
  int epoll_fd = epoll_create(0);
  CheckError("epoll_create", epoll_fd);
  struct epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = listen_fd;
  ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &event);
  CheckError("epoll_ctl", ret);
  while(1){
    struct epoll_event events[10];
    int size = epoll_wait(epoll_fd, events, sizeof(events) / sizeof(events[0]), -1);
    if(size < 0){
      perror("epoll_wait");
      continue;
    }
    if(0 == size){
      printf("epoll timeout\n");
      continue;
    }
    for(int i = 0; i < size; ++i){
      if(!(events[i].events & EPOLLIN)){
        continue;
      }
      if(events[i].data.fd == listen_fd){
        ProcessConnect(listen_fd, epoll_fd);
      }else{
        ProcessRequest(events[i].data.fd, epoll_fd);
      }
    }
  }
  return 0;
}

