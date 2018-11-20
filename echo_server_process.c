#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define TRUE 1
#define FALSE 0

void Usage(){
  printf("usage: ./server [ip] [port]\n");
}

void CheckErrorQuit(const int n, const char* s){
  if(n < 0){
    perror(s);
    exit(1);
  }
}

void ProcessRequest(int client_fd, struct sockaddr_in* client_addr){
  char buf[1024] = {0};
  while(TRUE){
    ssize_t read_size = read(client_fd, buf, sizeof(buf));
    if(read_size< 0){
      perror("read");
      continue;
    } else if(0 == read_size) {
      printf("client:%s exit!\n", inet_ntoa(client_addr->sin_addr));
      close(client_fd);
      break;
    }
    buf[read_size] = '\0';
    printf("client %s said: %s\n", inet_ntoa(client_addr->sin_addr), buf);
    write(client_fd, buf, strlen(buf));
  }
}

void CreatWorker(int client_fd, struct sockaddr_in* client_addr){
  pid_t pid = fork();
  if(pid < 0) {
    perror("fork");
    return;
  } else if(0 == pid) {
    if(fork() == 0){
      ProcessRequest(client_fd, client_addr);
    } 
  } else {
    close(client_fd);
    waitpid(pid, NULL, 0);
  }
}

int main(int argc, char* argv[]) {
  if(argc != 3){
    Usage();
  } 
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(atoi(argv[2]));
  addr.sin_addr.s_addr = inet_addr(argv[1]);
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  CheckErrorQuit(fd, "socket");
  int ret = bind(fd, (sockaddr*)&addr, sizeof(addr));
  CheckErrorQuit(ret, "bind");
  ret = listen(fd, 10);
  CheckErrorQuit(ret, "listen");
  while(TRUE){
    struct sockaddr_in client;
    socklen_t len = sizeof(client); 
    int client_fd = accept(fd, (struct sockaddr*)&client, &len);
    if(client_fd < 0){
      perror("client");
      continue;
    }
    CreatWorker(client_fd, &client);
  }
  return 0;
}
