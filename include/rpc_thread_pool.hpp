#ifndef _RPC_THREAD_POOL_HPP__
#define _RPC_THREAD_POOL_HPP__

#include <set>
#include <mutex>

#include "rpc_thread.hpp"

namespace rpc {

class RPCThreadPool {
public:
	friend class RPCThread;
private:
	std::set<RPCThread *> busy_thread_;
	std::set<RPCThread *> idle_thread_;

	std::mutex pool_mutex_;

	bool close_;

	void release(RPCThread *rpc_thread);
public:
	RPCThreadPool(int thread_num);
	~RPCThreadPool();

	template <typename Callback_, typename ... Args_>
	bool emplace(Callback_ callback, Args_ ... args) {
		if(close_) {
			return false;
		}
		std::unique_lock<std::mutex> idle_locker(pool_mutex_);
		if(idle_thread_.empty() || close_) {
			return false;
		}
		auto tmp = *idle_thread_.begin();
		idle_thread_.erase(tmp);
		auto func = std::bind(std::forward<Callback_>(callback), std::forward<Args_>(args)...);
		busy_thread_.emplace(tmp);
		tmp->exec(func);
		return true;
	}

	void close();
};

} // namespace rpc

#endif // _RPC_THREAD_POOL_HPP__