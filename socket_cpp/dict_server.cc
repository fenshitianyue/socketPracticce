#include <iostream>
#include <unordered_map>
#include "tcp_server.hpp"

std::unordered_map<std::string, std::string> g_dict; //g -> global


void Translate(const std::string& req, std::string* resp){
  auto it = g_dict.find(req);
  if(it != g_dict.end()){
    *resp = it->second;
  }else{
    *resp = "未找到";
  }
}

int main(int argc, char* argv[]) {
  if(argc != 3){
    std::cout << "Usage: ./dict_server [ip] [port]" << std::endl;
    return 1;
  } 
  //初始化词典
  g_dict.insert({"hello", "你好"}); 
  g_dict.insert({"world", "世界"});
  //启动服务器
  TcpServer server(argv[1], atoi(argv[2]));
  server.Start(Translate);
  return 0;
}


