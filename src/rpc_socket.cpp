#include "rpc_socket.hpp"

#include "rpc_data.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <endian.h>
#include <assert.h>

rpc::RPCSocket::RPCSocket() :
	sock_fd_(INADDR_NONE) {
}

rpc::RPCSocket::RPCSocket(int socket_fd) :
	sock_fd_(socket_fd) {
}

rpc::RPCSocket::RPCSocket(RPCSocket &&rpc_socket) :
	sock_fd_(rpc_socket.sock_fd_) {
	rpc_socket.sock_fd_ = INADDR_NONE;
}

rpc::RPCSocket::~RPCSocket() {
	close();
}

rpc::RPCSocket &rpc::RPCSocket::operator=(RPCSocket &&rpc_socket) {
	if(this == &rpc_socket) {
		return *this;
	}
	if(sock_fd_ != INADDR_NONE) {
		::close(sock_fd_);
	}
	sock_fd_ = rpc_socket.sock_fd_;
	rpc_socket.sock_fd_ = INADDR_NONE;
	return *this;
}

bool rpc::RPCSocket::connect(const std::string &ip, uint16_t port) {
	struct sockaddr_in addr;
	::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	addr.sin_port = htons(port);
	int flag = ::connect(sock_fd_, (struct sockaddr *)&addr, sizeof(sockaddr));
	return flag == 0;
}

bool rpc::RPCSocket::bind(const std::string &ip, uint16_t port) {
	struct sockaddr_in addr;
	::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	if(ip != "") {
		addr.sin_addr.s_addr = inet_addr(ip.c_str());
	} else {
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	addr.sin_port = htons(port);

	int flag = ::bind(sock_fd_, (struct sockaddr *)&addr, sizeof(addr));
	return flag == 0;
}

bool rpc::RPCSocket::listen(int backlog) {
	return ::listen(sock_fd_, backlog) == 0;
}

int rpc::RPCSocket::accept() {
	struct sockaddr addr;
	socklen_t length = sizeof(addr);
	int new_socket = ::accept(sock_fd_, &addr, &length);
	assert(new_socket >= 0);
	return new_socket;
}

void rpc::RPCSocket::close() {
	if(sock_fd_ != INADDR_NONE) {
		::close(sock_fd_);
		sock_fd_ = INADDR_NONE;
	}
}

void rpc::RPCSocket::send_error() {
	int64_t tmp = 0;
	send((char *)&tmp, sizeof(tmp));
}

int64_t rpc::RPCSocket::send(const char *buffer, int64_t length) {
	if(length == 0) {
		return 0;
	}
	int now = 0;
	while(now != length) {
		int p = ::send(sock_fd_, buffer + now, length - now, 0);
		if(p < 0) {
			if(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) {
				continue;
			}
			return -1;
		} else if(p == 0) {
			return -1;
		}
		now += p;
	}
	return now;
}

int64_t rpc::RPCSocket::recv(char *buffer, int64_t length) {
	if(length == 0) {
		return 0;
	}
	int now = 0;
	while(now != length) {
		int p = ::recv(sock_fd_, buffer + now, length - now, 0);
		if(p < 0) {
			if(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) {
				continue;
			}
			return -1;
		} else if(p == 0) {
			return -1;
		}
		now += p;
	}
	return now;
}

bool rpc::RPCSocket::send(const RPCData &rpc_data) {
	static_assert(sizeof(decltype(RPCData().size())) == sizeof(int64_t));
	int64_t size = rpc_data.size();
	int64_t out = htobe64(size);
	int len = send((const char *)&out, sizeof(out));
	if(len != sizeof(out)) {
		return false;
	}
	if(size == 0) {
		return true;
	}
	len = send(rpc_data.str(), size);
	return len == size;
}

bool rpc::RPCSocket::recv(RPCData &rpc_data) {
	static_assert(sizeof(decltype(RPCData().size())) == sizeof(int64_t));
	int64_t size;
	int len = recv((char *)&size, sizeof(size));
	if(len != sizeof(size) || size == 0) {
		return false;
	}
	size = be64toh(size);
	rpc_data.assign(size);
	len = recv(rpc_data.str(), size);
	return len == size;
}