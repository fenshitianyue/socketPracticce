#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>

int main() {
  fd_set read_fds;
  FD_ZERO(&read_fds); //初始化fd_set结构
  FD_SET(STDIN_FILENO, &read_fds); //监听标准输入
  while(1){
    printf("> ");
    fflush(stdout);
    int ret = select(STDIN_FILENO+1, &read_fds, NULL, NULL, NULL);
    if(ret < 0){
      perror("select");
      continue;
    }
    if(FD_ISSET(STDIN_FILENO, &read_fds)){
      char buf[1024] = {0}; 
      read(STDIN_FILENO, buf, sizeof(buf) - 1); //读取键盘输入
      printf("Echo: %s\n", buf);
    } else {
      printf("error! invalid fd\n");
      continue;
    }
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
  }
  return 0;
}
