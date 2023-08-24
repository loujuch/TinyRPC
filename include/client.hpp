#ifndef _CLIENT_HPP__
#define _CLIENT_HPP__

#include <string>

#include "rpc_data.hpp"
#include "rpc_instance_socket.hpp"
#include "rpc_exception.hpp"

namespace rpc {

class Client {
	RPCInstanceSocket rpc_socket_;
public:
	Client() = default;
	~Client() = default;

	inline bool connect(const std::string &ip, uint16_t port) {
		return rpc_socket_.connect(ip, port);
	}

	template <typename RES, typename ... Args>
	RES call(const std::string &name, const Args ... args) {
		rpc::RPCData rpc_in;
		rpc_in.write(name);
		rpc_in.write_args(args...);

		rpc_socket_.send(rpc_in);

		rpc::RPCData rpc_out;
		bool ok = rpc_socket_.recv(rpc_out);

		if(!ok) {
			throw RPCException{ "server return error!" };
		}

		RES res;
		rpc_out.read(res);
		return std::move(res);
	}
};

} // namespace rpc

#endif // _CLIENT_HPP__