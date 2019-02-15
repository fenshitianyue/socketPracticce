#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>


int main() {
  fd_set read_fd; 
  FD_ZERO(&read_fd);
  FD_SET(0, &read_fd);
  char buf[1024] = {0};
  while(1){
    printf(">");
    fflush(stdout);
    int ret = select(1, &read_fd, NULL, NULL, NULL);
    if(ret < 0){
      perror("select");
      continue;
    }
    if(FD_ISSET(0, &read_fd)){
      read(0, buf, sizeof(buf) - 1);
      printf("input: %s\n", buf);
    }else{
      printf("error! invaild fd\n");
      continue;
    }
    FD_ZERO(&read_fd);
    FD_SET(0, &read_fd);
  }
  return 0;
}
