#include "rpc_thread.hpp"

#include "rpc_thread_pool.hpp"

void rpc::RPCThread::main_func() {
	while(!close_) {
		std::unique_lock<std::mutex> locker(thread_mutex_);
		if(!wake_up_) {
			thread_cond_.wait(locker);
		}
		wake_up_ = false;
		status_ = Status::RUNNING;
		if(func_) {
			func_();
		}
		if(close_) {
			break;
		}
		func_ = std::function<void()>();
		status_ = Status::SUSPEND;
		own_pool_->release(this);
	}
}

rpc::RPCThread::RPCThread(bool &c, RPCThreadPool *pool) :
	close_(c),
	wake_up_(false),
	own_pool_(pool),
	status_(Status::SUSPEND),
	rpc_thread_(&RPCThread::main_func, this) {
}

void rpc::RPCThread::exec(std::function<void()> func) {
	func_ = func;
	wake_up_ = true;
	thread_cond_.notify_one();
}