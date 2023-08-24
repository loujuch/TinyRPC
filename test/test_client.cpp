#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

#include "client.hpp"
#include "rpc_data.hpp"
#include "serializable.hpp"
#include "rpc_exception.hpp"

#include <assert.h>

class test0 :public rpc::Serializable {
public:
	friend class test1;
private:
	std::string s;
public:
	test0(const std::string &ss = "") :s(ss) {
	}

	void print() {
		std::cout << s << std::endl;
	}

	void serialize(rpc::RPCData &stream) const {
		stream.write(s);
	}

	bool unserialize(rpc::RPCData &stream) {
		stream.read(s);
		return true;
	}
};

class test1 :public rpc::Serializable {
	std::string s;
public:
	test1(const test0 &t = test0()) :s(t.s) {
		for(char &c : s) {
			c = toupper(c);
		}
	}

	void print() {
		std::cout << s << std::endl;
	}

	void serialize(rpc::RPCData &stream) const {
		stream.write(s);
	}

	bool unserialize(rpc::RPCData &stream) {
		assert(stream.read(s));
		return true;
	}
};

test1 change(const test0 &t) {
	return test1(t);
}

// int num = 0;
// std::mutex io_mutex;

// void main_func(int i) {
// 	rpc::Client client;
// 	assert(client.connect("127.0.0.1", 8080));
// 	// std::cout << "connect" << std::endl;
// 	auto tmp = client.call<test1>("change", test0("loujuch"));
// 	// std::cout << "call" << std::endl;
// 	std::unique_lock<std::mutex> locker(io_mutex);
// 	std::cout << i << ": ";
// 	tmp.print();
// 	++num;
// }

int sum(int a, int b) {
	return a + b;
}

double sumv(const std::vector<double> &v) {
	double out = 0;
	for(double d : v) {
		out += d;
	}
	return out;
}

int main() {
	// int n = 32;
	// std::vector<std::thread> client;
	// for(int i = 0;i < n;++i) {
	// 	client.emplace_back(main_func, i);
	// }
	// for(auto &p : client) {
	// 	p.join();
	// }
	// std::cout << num << std::endl;
	rpc::Client client;
	assert(client.connect("127.0.0.1", 8080));
	try {
		auto tmp = client.call<test1>("change", test0("loujuch"));
		tmp.print();
		auto tmp1 = client.call<int>("sum", 2, 9);
		std::cout << tmp1 << std::endl;
		auto tmp2 = client.call<double>("sumv", std::vector<double>({ 1, 22,3,1,0.23 }));
		std::cout << tmp2 << std::endl;
	} catch(rpc::RPCException e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}