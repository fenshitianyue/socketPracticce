#include <iostream>
#include <string>

#define LOG 

class TcpServer{
public:
  TcpServer(const std::string& ip = "127.0.0.1" , uint16_t port = 8080) : _ip(ip), _port(port){
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
  TcpServer *pserver;
  if(argc != 1) {
    pserver = new TcpServer(argv[1], atoi(argv[2]));
  }else{
    pserver = new TcpServer;
  }
  
  return 0;
}

