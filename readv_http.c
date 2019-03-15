#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>

#define BUFFER_SIZE 1024

static const char* status_line[2] = {"200 OK", "500 Internal server error"};

int main(int argc, char* argv[]) {
  if(argc <= 3){
    printf("usage: %s ip_address port_number filename\n", basename(argv[0]));
    return 1;
  } 
  const char* ip = argv[1];
  int port = atoi(argv[2]);

  return 0;
}

