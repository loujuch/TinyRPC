#include <iostream>

#include "rpc_thread_pool.hpp"
#include <unistd.h>

void test() {
	std::cout << "test" << std::endl;
}

int main() {
	rpc::RPCThreadPool pool(4);
	pool.emplace(test);
	pool.emplace(test);
	pool.emplace(test);
	sleep(1);
	pool.close();
	return 0;
}