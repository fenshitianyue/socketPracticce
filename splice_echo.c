#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <libgen.h> //basename

int main(int argc, char* argv[]) {
  if(argc <= 2){
    printf("usage: %s ip_address port_number file_name\n", basename(argv[0]));
  } 
  const char* ip = argv[1];
  int port = atoi(argv[2]);
  struct sockaddr_in address;
  bzero(&address, sizeof(address));
  address.sin_family = AF_INET;
  inet_pton(AF_INET, ip, &address.sin_addr);
  address.sin_port = htons(port);

  int sock = socket(PF_INET, SOCK_STREAM, 0);
  assert(sock >= 0);
  int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
  assert(ret != -1);
  ret = listen(sock, 5);
  assert(ret != -1);

  struct sockaddr_in client;
  socklen_t client_addrlength = sizeof(client);
  int connfd = accept(sock, (struct sockaddr*)&address, &client_addrlength);
  if(connfd < 0){
    perror("accept");
  }else{
    int pipefd[2];
    ret = pipe(pipefd);
    //将 connfd 上流入的客户端数据定向到管道中
    ret = splice(connfd, NULL, pipefd[1], NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
    assert(ret != -1);
    //将管道的数据输出到 connfd 客户端连接文件描述符
    ret = splice(pipefd[0], NULL, connfd, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
    assert(ret != -1);
    close(connfd);
  }
  return 0;
}

