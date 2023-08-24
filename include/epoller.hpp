#ifndef _EPOLLER_HPP__
#define _EPOLLER_HPP__

#include "rpc_handler.hpp"

#include <sys/epoll.h>

namespace rpc {

class Epoller {
public:
	friend void RPCHandler::start(RPCInstanceSocket &m_sock, FunctionHandler &fh, int timeout);
private:
	int epoll_fd_;
	struct epoll_event *events;

	void ctr(int fd, void *ptr, __uint32_t events, int op);
public:
	Epoller();
	~Epoller();

	void add(int fd, void *ptr, __uint32_t events);
	void del(int fd, void *ptr, __uint32_t events);
	void mod(int fd, void *ptr, __uint32_t events);

	int wait(int millsecond);
}; // class Epoller

} // namespace rpc

#endif // _EPOLLER_HPP__