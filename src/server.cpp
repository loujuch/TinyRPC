#include "server.hpp"

#include <thread>

#include <assert.h>

void rpc::Server::main_func(RPCSocket &&sock) {
	while(true) {
		RPCData in, out;
		if(!sock.recv(in)) {
			break;
		}
		std::string func;
		in.read(func);
		function_handler_.call(func, in, out);
		if(!sock.send(out)) {
			break;
		}
	}
}

void rpc::Server::start() {
	bool ok;
	ok = rpc_socket_.bind(ip_, port_);
	assert(ok);
	ok = rpc_socket_.listen(5);
	assert(ok);

	while(true) {
		std::thread tmp(&Server::main_func, this, std::move(rpc_socket_.accept()));
		tmp.detach();
	}
}