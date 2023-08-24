#include "rpc_handler.hpp"

#include <thread>

#include "function_handler.hpp"

void rpc::RPCHandler::main_func(RPCSocket *p,
	FunctionHandler &fh, ObjectPool<RPCSocket> *opool) {
	RPCSocket &sock = *p;
	while(true) {
		RPCData in, out;
		if(!sock.recv(in)) {
			break;
		}
		std::string func;
		in.read(func);
		// 若出错则将out设为空
		fh.call(func, in, out);
		if(!sock.send(out)) {
			break;
		}
	}
	sock.close();
	opool->release(p);

}

void rpc::RPCHandler::start(RPCInstanceSocket &m_sock, FunctionHandler &fh, int timeout) {
	ObjectPool<RPCSocket> sock_pool;

	sock_pool.set_object_capacity(socket_pool_size_);

	while(true) {
		int sock = m_sock.accept();
		auto p = sock_pool.allocate();
		if(p == nullptr) {
			RPCSocket(sock).send_error();
			continue;
		}
		p->set_rpc_socket(sock);
		std::thread tmp(&RPCHandler::main_func, p, std::ref(fh), &sock_pool);
		tmp.detach();
	}
}