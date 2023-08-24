#include "epoller.hpp"

#include <assert.h>
#include <unistd.h>

rpc::Epoller::Epoller() :
	epoll_fd_(epoll_create1(EPOLL_CLOEXEC)),
	events(new epoll_event[1024]) {
	assert(epoll_fd_ >= 0);
}

rpc::Epoller::~Epoller() {
	if(epoll_fd_ >= 0) {
		::close(epoll_fd_);
		epoll_fd_ = -1;
	}
	if(events != nullptr) {
		delete[]events;
		events = nullptr;
	}
}

void rpc::Epoller::add(int fd, void *ptr, __uint32_t events) {
	ctr(fd, ptr, events, EPOLL_CTL_ADD);
}

void rpc::Epoller::del(int fd, void *ptr, __uint32_t events) {
	ctr(fd, ptr, events, EPOLL_CTL_DEL);
}

void rpc::Epoller::mod(int fd, void *ptr, __uint32_t events) {
	ctr(fd, ptr, events, EPOLL_CTL_MOD);
}

int rpc::Epoller::wait(int timeout) {
	return epoll_wait(epoll_fd_, events, 1024, timeout);
}

void rpc::Epoller::ctr(int fd, void *ptr, __uint32_t events, int op) {
	assert(fd >= 0);
	struct epoll_event ev;
	ev.data.ptr = ptr;
	epoll_ctl(epoll_fd_, op, fd, &ev);
}