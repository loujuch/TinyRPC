#include "server.hpp"

#include <thread>

#include <assert.h>

void rpc::Server::main_func(RPCSocket *p) {
	RPCSocket &sock = *p;
	while(true) {
		RPCData in, out;
		if(!sock.recv(in)) {
			break;
		}
		std::string func;
		in.read(func);
		// 若出错则将out设为空
		function_handler_.call(func, in, out);
		if(!sock.send(out)) {
			break;
		}
	}
	sock.close();
	sock_pool_.release(p);
}

void rpc::Server::start() {
	bool ok;
	ok = rpc_socket_.bind(ip_, port_);
	assert(ok);
	ok = rpc_socket_.listen(5);
	assert(ok);

	sock_pool_.set_object_capacity(1024);

	while(true) {
		int sock = rpc_socket_.accept();
		auto p = sock_pool_.allocate();
		if(p == nullptr) {
			RPCSocket(sock).send_error();
			continue;
		}
		p->set_rpc_socket(sock);
		std::thread tmp(&Server::main_func, this, p);
		tmp.detach();
	}
}