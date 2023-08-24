#include <iostream>
#include <string>

#include "server.hpp"

int test() {
	return 0;
}

int main() {
	int n = 512;
	rpc::Server server;
	server.listen("", 8080);
	server.set_object_size(n);
	server.set_thread_pool_size(n);
	server.bind("test", test);
	server.start();
	return 0;
}