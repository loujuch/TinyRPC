#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <mutex>

#include "client.hpp"

int n = 0, num = 0;
std::mutex m;

void test(int i) {
	rpc::Client client;
	client.connect("127.0.0.1", 8080);
	int p = -1;
	try {
		p = client.call<int>("test");
	} catch(rpc::RPCException &e) {
		std::unique_lock<std::mutex> locker(m);
		std::cout << e.what() << std::endl;
		return;
	}
	std::unique_lock<std::mutex> locker(m);
	std::cout << num << std::endl;
	n += (p == 0);
	++num;
}

int main() {
	int num = 256;
	std::vector<std::thread> v;
	for(int i = 0;i < num;++i) {
		v.emplace_back(test, i);
	}
	for(auto &p : v) {
		p.join();
	}
	std::cout << n << std::endl;
	return 0;
}