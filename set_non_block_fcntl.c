#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void set_no_block(int fd){
  int option = fcntl(fd, F_GETFL);
  if(option < 0){
    perror("fcntl_get");
    exit(EXIT_FAILURE);
  }
  option = fcntl(fd, F_SETFL, option | O_NONBLOCK);
  if(option < 0){
    perror("fcntl_set");
    exit(EXIT_FAILURE);
  }
}

int main() {
  set_no_block(0); 
  char buf[1024] = {0};
  while(1){
    ssize_t size = read(0, buf, sizeof(buf) - 1);
    if(size < 0){
      perror("read");
      sleep(1);
      continue;
    }
    buf[size] = 0;
    printf("output:%s\n", buf);
  }
  return 0;
}
