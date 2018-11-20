#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void Usage(){
  printf("Usage: ./udp_client [ip] [port]\n"); 
  exit(1);
}


int main(int argc, char* argv[]) {
  if(argc != 3){
    Usage();
  }
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock < 0){
    perror("socket");
    return 2;
  }
  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons(atoi(argv[2]));
  char buf[1024] = {0};
  struct sockaddr_in peer;
  while(1){
    socklen_t len = sizeof(peer);
    printf("Please Enter# ");
    fflush(stdout);
    ssize_t s = read(0, buf, sizeof(buf) - 1);
    if(s > 0){
      buf[s] = '\0';
      sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&peer, sizeof(peer));
      ssize_t _s = recvfrom(sock, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&peer, &len);
      if(_s > 0){
        buf[_s] = 0;
        printf("server echo# %s\n", buf);
      }
    }
  }
  return 0;
}

