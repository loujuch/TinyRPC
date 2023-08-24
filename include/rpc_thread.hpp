#ifndef _RPC_THREAD_HPP__
#define _RPC_THREAD_HPP__

#include <thread>
#include <functional>
#include <atomic>
#include <condition_variable>

namespace rpc {

class RPCThreadPool;

class RPCThread {
	enum class Status :uint8_t {
		RUNNING = 0,
		SUSPEND
	};

	bool &close_;
	RPCThreadPool *own_pool_;

	bool wake_up_;
	std::atomic<Status> status_;
	std::function<void()> func_;
	std::thread rpc_thread_;

	std::mutex thread_mutex_;
	std::condition_variable thread_cond_;

	void main_func();
public:
	RPCThread(bool &c, RPCThreadPool *pool);

	void exec(std::function<void()> func_);

	inline void join() {
		rpc_thread_.join();
	}
}; // class RPCThread

} // namespace rpc

#endif // _RPC_THREAD_HPP__