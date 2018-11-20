#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void Usage(){
  printf("Usage: ./echo_server_thread [ip] [port]\n");
  exit(1);
}

void ProcessRequest(int client_fd, struct sockaddr_in* client_addr){
  char buf[1024] = {0};
  while(1){
    ssize_t read_size = read(client_fd, buf, sizeof(buf) - 1);
    if(read_size < 0){
      perror("read");
      continue;
    } else if(0 == read_size) {
      char tmp_buf[1024] = {0};
      printf("client:%s exit!\n", inet_ntop(AF_INET, &client_addr->sin_addr, tmp_buf, sizeof(tmp_buf)));
      close(client_fd);
      break;
    } else {
      char tmp_buf[1024] = {0};
      buf[read_size] = 0;
      printf("client: %s said: %s\n", inet_ntop(AF_INET, &client_addr->sin_addr, tmp_buf, sizeof(tmp_buf)), buf);
      write(client_fd, buf, strlen(buf));
    }
  }
}

void CheckError(const int n, const char* s){
  if(n < 0){
    perror(s);
    exit(1);
  }
}

typedef struct Arg{
  int fd;
  struct sockaddr_in addr;
} Arg;

void* CreatWorker(void* ptr){
  Arg* arg = (Arg*)ptr;
  ProcessRequest(arg->fd, &arg->addr);
  free(arg);
  return NULL;
}

int main(int argc, char* argv[]) {
  if(argc != 3){
    Usage();
  } 
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(argv[1]);
  addr.sin_port = htons(atoi(argv[2]));
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  CheckError(fd, "socket"); 
  int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
  CheckError(ret, "bind");
  ret = listen(fd, 10);
  CheckError(ret, "listen");
  while(1){
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    int client_fd = accept(fd, (struct sockaddr*)&client_fd, &len);
    if(client_fd < 0){
      perror("accept");
      continue;
    }
    pthread_t tid = 0;
    Arg* arg = (Arg*)malloc(sizeof(Arg));
    arg->fd = client_fd;
    arg->addr = client_addr;
    pthread_create(&tid, NULL, CreatWorker, (void*)arg);
    pthread_detach(tid);
  }
  return 0;
}

