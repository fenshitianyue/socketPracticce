#pragma once


#include <functional>
#include <pthread.h>
#include "tcp_socket.hpp"

using Handler = std::function<void (const std::string&, std::string*)>;


class TcpThreadServer{
public:
private:
};
