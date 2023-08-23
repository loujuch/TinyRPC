#include <iostream>
#include "rpc_socket.hpp"

int main() {
	rpc::RPCSocket server;
	server.bind("", 8080);
	server.listen(10);
	rpc::RPCSocket c1 = std::move(server.accept());
	char buffer[32];
	c1.recv(buffer, 5);
	std::cout << "recv: " << buffer << std::endl;
	c1.send(buffer, 5);
	std::cout << "send: " << buffer << std::endl;
	return 0;
}