#ifndef _RPC_HANDLER_HPP__
#define _RPC_HANDLER_HPP__

#include "rpc_instance_socket.hpp"
#include "object_pool.hpp"

namespace rpc {

class FunctionHandler;

class RPCHandler {
	int socket_pool_size_;
	int thread_pool_size_;
public:
	void set_socket_pool_size(int size) {
		socket_pool_size_ = size;
	}

	void set_thread_pool_size(int size) {
		thread_pool_size_ = size;
	}

	void start(RPCInstanceSocket &m_sock, FunctionHandler &fh, int timeout);
}; // class RPCHandler

} // namespace rpc

#endif // _RPC_HANDLER_HPP__