#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>

void check_report(const int num, const char* message){
  if(num < 0){
    perror(message);
    exit(EXIT_FAILURE);
  }
}

// ./server 0.0.0.0 9090
int main(int argc, char* argv[]) {
  if(argc != 3){
    printf("server boot failure!\n");
    exit(EXIT_FAILURE);
  } 
  printf("server start...\n"); 
  const char* ip = argv[1];
  const int port = 9090;
  int sock = socket(PF_INET, SOCK_STREAM, 0);
  check_report(sock, "socket");
  struct sockaddr_in serveraddr;
  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = PF_INET; 
  serveraddr.sin_port = htons(port);
  inet_pton(AF_INET, ip, &serveraddr.sin_addr);
  int ret = bind(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
  check_report(ret, "bind");
  ret = listen(sock, 5);
  check_report(ret, "listen");

  while(1){
    struct sockaddr_in clientaddr;
    bzero(&clientaddr, sizeof(clientaddr)); 
    socklen_t len = sizeof(clientaddr);
    int client_fd = accept(sock, (struct sockaddr*)&clientaddr, &len);
    if(client_fd < 0){
      perror("accept");
      continue;
    }
    char input_buf[1024 * 10] = {0}; 
    ssize_t read_size = read(client_fd, input_buf, sizeof(input_buf) - 1);
    if(read_size < 0){
      exit(EXIT_FAILURE);
    }else if(0 == read_size){
      printf("client exit!\n"); 
      break;
    }else{
      printf("[Request] %s", input_buf);
      char buf[1024] = {0};
      const char* doc = "<h1>hello world</h1>";
      sprintf(buf, "HTTP/1.0 200 OK\nContent-Length:%lu\n\n%s", strlen(doc), doc);
      write(client_fd, buf, strlen(buf));
    }
  }

  return 0;
}

