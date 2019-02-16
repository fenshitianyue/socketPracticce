#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct pollfd pollfd;

void Init(pollfd* fd_list, int size){
  for(int i = 0; i < size; ++i){
    fd_list[i].fd = -1;
    fd_list[i].events = 0;
    fd_list[i].revents = 0;
  }
}

void Add(int fd, pollfd* fd_list, int size){
  for(int i = 0; i < size; ++i){
    if(fd_list[i].fd == -1){
      fd_list[i].fd = fd;
      fd_list[i].events = POLLIN;
      break;
    }
  }
}

void CorrectUsage(){
  printf("usage: ./poll_server [ip] [port]\n");
  exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
  if(argc != 3){
    CorrectUsage();
  } 
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(argv[1]);
  addr.sin_port = htons(atoi(argv[2]));
  int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(listen_fd < 0){
    perror("socket");
    exit(EXIT_FAILURE);
  }
  int ret = bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr));
  if(ret < 0){
    perror("bind");
    exit(EXIT_FAILURE);
  }
  ret = listen(listen_fd, 10);
  if(ret < 0){
    perror("listen");
    exit(EXIT_FAILURE);
  }
  pollfd fd_list[1024];
  Init(fd_list, sizeof(fd_list) / sizeof(fd_list[0]));
  Add(listen_fd, fd_list, sizeof(fd_list) / sizeof(fd_list[0]));
  while(1){
    int ret = poll(fd_list, sizeof(fd_list) / sizeof(fd_list[0]), 1000);
    if(ret < 0){
      perror("poll");
      exit(EXIT_FAILURE);
    }
    if(0 == ret){
      printf("poll timeout\n");
      continue;
    }
    for(size_t i = 0; i < sizeof(fd_list) / sizeof(fd_list[0]); ++i){
      if(fd_list[i].fd == -1){
        continue;
      }
      if(!(fd_list[i].revents & POLLIN)){
        continue;
      }
      if(fd_list[i].fd == listen_fd){
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int connect_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &len);
        if(connect_fd < 0){
          perror("accept");
          continue;
        }
        Add(connect_fd, fd_list, sizeof(fd_list) / sizeof(fd_list[0]));
      }else{
        char buf[1024] = {0}; 
        ssize_t read_size = read(fd_list[i].fd, buf, sizeof(buf ) - 1);
        if(read_size < 0){
          perror("read");
          continue;
        }else if(0 == read_size){
          printf("client say: goodbye\n");
          close(fd_list[i].fd);
          fd_list[i].fd = -1;
        }
        printf("client say: %s\n", buf);
        write(fd_list[i].fd, buf, strlen(buf));
      }
    }
  }
  return 0;
}
