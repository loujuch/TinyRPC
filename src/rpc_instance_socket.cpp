#include "rpc_instance_socket.hpp"

#include <netinet/in.h>
#include <assert.h>

rpc::RPCInstanceSocket::RPCInstanceSocket() :
	RPCSocket(::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) {
}