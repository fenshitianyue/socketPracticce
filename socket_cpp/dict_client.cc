#include "tcp_client.hpp"
#include <iostream>


int main(int argc, char* argv[]) {
  if(argc != 3){
    std::cout << "Usage: ./dict_client [ip] [port]" << std::endl;
    return 1;
  } 
  TcpClient client(argv[1], atoi(argv[2]));
  if(client.Connect()){
    std::cout << "服务器连接失败！" << std::endl;
    return 1;
  }
  std::string word;
  std::string result;
  while(true){
    std::cout << "输入要查询的单词：" << std::endl;
    std::cin >> word;
    //if(!std::cin) 
    //  break;
    client.Send(word);
    client.Recv(&result);
    std::cout << result << std::endl;
    std::string().swap(word);
    std::string().swap(result);
  }
  return 0;
}

