#include "rpc_thread_pool.hpp"

rpc::RPCThreadPool::RPCThreadPool(int num) :close_(false) {
	for(int i = 0;i < num;++i) {
		idle_thread_.emplace(new RPCThread(close_, this));
	}
}

rpc::RPCThreadPool::~RPCThreadPool() {
	if(!close_) {
		close();
	}
	for(auto &p : idle_thread_) {
		delete p;
	}
	for(auto &p : busy_thread_) {
		delete p;
	}
}

void rpc::RPCThreadPool::release(RPCThread *rpc_thread) {
	if(close_) {
		return;
	}
	std::unique_lock<std::mutex> idle_locker(pool_mutex_);
	busy_thread_.erase(rpc_thread);
	idle_thread_.emplace(rpc_thread);
}

void rpc::RPCThreadPool::close() {
	if(close_) {
		return;
	}
	std::unique_lock<std::mutex> idle_locker(pool_mutex_);
	close_ = true;
	for(auto &p : idle_thread_) {
		p->exec(std::function<void()>());
	}
	for(auto &p : idle_thread_) {
		p->join();
	}
	for(auto &p : busy_thread_) {
		p->join();
	}
}