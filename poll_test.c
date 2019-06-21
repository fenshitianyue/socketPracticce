#include <sys/poll.h>
#include <unistd.h>
#include <stdio.h>


int main() {
  struct pollfd poll_fd;
  poll_fd.fd = STDIN_FILENO;
  poll_fd.events = POLLIN;
  while(1){
    printf("> ");
    fflush(stdout);
    int ret = poll(&poll_fd, 1, -1);
    if(0 == ret){
      printf("poll timeout.\n");
      continue;
    }else if(ret < 0){
      perror("poll");
      continue;
    }
    if(POLLIN == poll_fd.revents){
      char buf[1024] = {0};
      read(STDIN_FILENO, buf, sizeof(buf) - 1);
      printf("echo: %s", buf);
    }
  }
  return 0;
}
