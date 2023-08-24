#ifndef _SOCKET_HPP__
#define _SOCKET_HPP__

#include <string>

#include <stdint.h>

namespace rpc {

class RPCData;

class RPCSocket {
private:
	int sock_fd_;
public:
	RPCSocket();
	explicit RPCSocket(int socket_fd);
	RPCSocket(RPCSocket &&rpc_socket);
	virtual ~RPCSocket();

	RPCSocket &operator=(RPCSocket &&rpc_socket);

	inline void set_rpc_socket(int sock) {
		sock_fd_ = sock;
	}

	bool connect(const std::string &ip, uint16_t port);

	bool bind(const std::string &ip, uint16_t port);
	bool listen(int backlog);

	void close();

	int accept();

	void send_error();

	int64_t send(const char *buffer, int64_t length);
	int64_t recv(char *buffer, int64_t length);

	bool send(const RPCData &rpc_data);
	bool recv(RPCData &rpc_data);

	RPCSocket(const RPCSocket &) = delete;
	RPCSocket &operator=(const RPCSocket &) = delete;
};

} // namespace rpc

#endif // _SOCKET_HPP__