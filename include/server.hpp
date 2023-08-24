#ifndef _SERVER_HPP__
#define _SERVER_HPP__

#include "singleton.hpp"
#include "rpc_instance_socket.hpp"
#include "function_handler.hpp"

#include <string>

namespace rpc {

class Server :public Singleton<Server> {
	int object_size_;
	int thread_pool_size_;
	int timeout_;
	std::string ip_;
	uint16_t port_;
	RPCInstanceSocket rpc_socket_;
	FunctionHandler function_handler_;
public:
	Server();

	template <typename F>
	inline bool bind(const std::string &s, F func) {
		return function_handler_.bind(s, func);
	}

	inline void listen(const std::string &ip, int port) {
		ip_ = ip;
		port_ = port;
	}

	inline void set_object_size(int object_size) {
		object_size_ = object_size;
	}

	inline void set_thread_pool_size(int size) {
		thread_pool_size_ = size;
	}

	inline void set_timeout(int timeout) {
		timeout_ = timeout;
	}

	void start();
}; // class Server

} // namespace rpc

#endif // _SERVER_HPP__