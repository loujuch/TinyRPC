#include <iostream>

#include "rpc_socket.hpp"

int main() {
	rpc::RPCSocket client;
	client.connect("127.0.0.1", 8080);
	client.send("test!", 5);
	std::cout << "send: " << "test!" << std::endl;
	char buffer[32];
	client.recv(buffer, 5);
	buffer[5] = '\0';
	std::cout << "recv: " << buffer << std::endl;
	return 0;
}