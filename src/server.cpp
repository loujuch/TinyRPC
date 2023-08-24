#include "server.hpp"

#include <thread>

#include "rpc_handler.hpp"
#include "object_pool.hpp"

#include <assert.h>

rpc::Server::Server() :
	object_size_(1024),
	thread_pool_size_(1024),
	timeout_(3000) {
}

void rpc::Server::start() {
	bool ok;
	ok = rpc_socket_.bind(ip_, port_);
	assert(ok);
	ok = rpc_socket_.listen(5);
	assert(ok);

	RPCHandler handler;
	handler.set_socket_pool_size(object_size_);
	handler.set_thread_pool_size(thread_pool_size_);
	handler.start(rpc_socket_, function_handler_, timeout_);
}