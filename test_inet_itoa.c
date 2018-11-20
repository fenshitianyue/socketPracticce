#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

void* func1(void* p){
  sockaddr_in *addr = (sockaddr_in*)p;
  while(1){
    char* ptr = inet_ntoa(addr->sin_addr);
    printf("addr1 : %s\n", ptr);
  }
  return NULL;
}

void* func2(void* p){
  sockaddr_in *addr = (sockaddr_in*)p;
  while(1){
    char* ptr = inet_ntoa(addr->sin_addr);
    printf("addr2 : %s\n", ptr);
  }
  return NULL;
}

int main() {
  pthread_t tid1 = 0;
  sockaddr_in addr1;
  sockaddr_in addr2;
  addr1.sin_addr.s_addr = 0;
  addr2.sin_addr.s_addr = 0xfffffff;
  pthread_create(&tid1, NULL, func1, &addr1);
  pthread_t tid2 = 0;
  pthread_create(&tid2, NULL, func2, &addr2);
  pthread_join(tid1, NULL);     
  pthread_join(tid2, NULL);     
  return 0;
}
