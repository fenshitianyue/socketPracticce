#include <iostream>
#include <string>
#include <gflags/gflags.h>
//else header file

#define LOG

DEFINE_string(ip, "127.0.0.1", "IP address");
DEFINE_int32(port, 8080, "port");

class Server{
public:
  Server(const std::string& ip, uint16_t port) : _ip(ip), _port(port) {
    std::cout << "Init Server..." << std::endl;
#ifdef LOG 
    std::cout << "ip  : " << _ip << std::endl;
    std::cout << "port: " << _port << std::endl;
#endif
    std::cout << "Init OK!" << std::endl;
  }
  //else code
private:
  std::string _ip;
  uint16_t _port;
  //else code
};


int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);  
  Server* pserver = new Server(FLAGS_ip, FLAGS_port);

  return 0;
}

