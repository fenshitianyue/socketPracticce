#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define TRUE 1
#define FALSE 0
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

void Usage(){
  printf("usage: ./server [ip] [port]\n");
  exit(1);
}

void CheckErrorQuit(const int n, const char* s){
  if(n < 0){
    perror(s);
    exit(1);
  }
}

int main(int argc, char* argv[]) {
  if(argc != 3){
    Usage();
  }
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  CheckErrorQuit(sock, "socket");
  sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);

  while(TRUE){
    char buf[1024] = {0};
    ssize_t read_size = read(0, buf, sizeof(buf) - 1);
    CheckErrorQuit(read_size, "read");
    if(0 == read_size){
      printf("read done!\n");
      close(sock);
      break;
    }
    buf[read_size] = '\0';
    sendto(sock, buf, strlen(buf), (sockaddr*)&server_addr, sizeof(server_addr));
    char buf_output[1024] = {0};
    read_size = recvfrom(sock, buf, sizeof(buf_output) - 1, 0, NULL, NULL);
    printf("server response:%s\n", buf_output);
  }
  close(sock);
  return 0;
}

