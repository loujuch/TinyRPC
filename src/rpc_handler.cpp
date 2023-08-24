#include "rpc_handler.hpp"

#include <thread>

#include "epoller.hpp"
#include "function_handler.hpp"

#include <assert.h>

static void main_func(rpc::RPCSocket *p,
	rpc::FunctionHandler &fh,
	rpc::ObjectPool<rpc::RPCSocket> *opool,
	rpc::Epoller *epoller) {

	rpc::RPCSocket &sock = *p;
	rpc::RPCData in, out;
	if(!sock.recv(in)) {
		sock.close();
		opool->release(p);
		return;
	}
	std::string func;
	in.read(func);
	// 若出错则将out设为空
	fh.call(func, in, out);
	if(!sock.send(out)) {
		sock.close();
		opool->release(p);
		return;
	}
	epoller->add(sock.socket(), p, EPOLLIN | EPOLLHUP | EPOLLERR | EPOLLONESHOT);
}

void rpc::RPCHandler::start(RPCInstanceSocket &m_sock, FunctionHandler &fh, int timeout) {
	ObjectPool<RPCSocket> sock_pool;
	Epoller epoller;

	m_sock.set_no_block();
	epoller.add(m_sock.socket(), &m_sock, (EPOLLIN | EPOLLHUP | EPOLLERR));
	sock_pool.set_object_capacity(socket_pool_size_);

	while(true) {
		int nfds = epoller.wait(timeout);
		for(int i = 0;i < nfds;++i) {
			if(epoller.events[i].data.ptr == &m_sock) {
				if(epoller.events[i].events & (EPOLLHUP | EPOLLERR)) {
					perror("listen socket fd error");
					assert(false);
				} else {
					int sock = m_sock.accept();
					auto p = sock_pool.allocate();
					if(p == nullptr) {
						RPCSocket(sock).send_error();
						continue;
					}
					p->set_rpc_socket(sock);
					p->set_no_block();
					epoller.add(sock, p, (EPOLLIN | EPOLLHUP | EPOLLERR | EPOLLONESHOT));
				}
			} else {
				RPCSocket *p = (RPCSocket *)epoller.events[i].data.ptr;
				epoller.del(p->socket(), p, EPOLLIN | EPOLLHUP | EPOLLERR | EPOLLONESHOT);
				if(epoller.events[i].events & EPOLLHUP) {
					p->close();
					sock_pool.release(p);
				} else if(epoller.events[i].events & EPOLLERR) {
					p->close();
					sock_pool.release(p);
				} else {
					std::thread tmp(main_func, p, std::ref(fh), &sock_pool, &epoller);
					tmp.detach();
				}
			}
		}
	}
}