#ifndef _SERVER_HPP__
#define _SERVER_HPP__

#include "singleton.hpp"
#include "rpc_socket.hpp"
#include "function_handler.hpp"

#include <string>

namespace rpc {

class Server :public Singleton<Server> {
	std::string ip_;
	uint16_t port_;
	RPCSocket rpc_socket_;
	FunctionHandler function_handler_;

	void main_func(RPCSocket &&sock);
public:
	template <typename F>
	inline bool bind(const std::string &s, F func) {
		return function_handler_.bind(s, func);
	}

	inline void listen(const std::string &ip, int port) {
		ip_ = ip;
		port_ = port;
	}

	void start();
}; // class Server

} // namespace rpc

#endif // _SERVER_HPP__