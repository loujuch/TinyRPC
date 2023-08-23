#include <iostream>

#include "server.hpp"
#include "rpc_data.hpp"
#include "serializable.hpp"

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
		stream.read(s);
		return true;
	}
};

test1 change(const test0 &t) {
	return test1(t);
}

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
	auto &server = rpc::Server::getInstance();
	server.listen("", 8080);
	server.bind("change", change);
	server.bind("sum", sum);
	server.bind("sumv", sumv);
	server.start();
	return 0;
}