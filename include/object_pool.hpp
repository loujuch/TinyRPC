#ifndef _OBJECT_POOL_HPP__
#define _OBJECT_POOL_HPP__

#include <vector>
#include <mutex>

namespace rpc {

// 对象池，要求线程安全
template <typename T>
class ObjectPool {
	std::mutex object_pool_mutex_;
	std::vector<T *> object_pool_;
public:
	~ObjectPool() {
		for(auto &p : object_pool_) {
			delete p;
		}
		object_pool_.clear();
	}

	bool set_object_capacity(int capacity) {
		std::unique_lock<std::mutex> locker(object_pool_mutex_);
		if(!object_pool_.empty() || capacity <= 0) {
			return false;
		}
		for(int i = 0;i < capacity;++i) {
			object_pool_.emplace_back(new T);
		}
		return true;
	}

	T *allocate() {
		std::unique_lock<std::mutex> locker(object_pool_mutex_);
		if(object_pool_.empty()) {
			return nullptr;
		}
		auto *p = object_pool_.back();
		object_pool_.pop_back();
		return p;
	}

	void release(T *p) {
		if(p == nullptr) {
			return;
		}
		std::unique_lock<std::mutex> locker(object_pool_mutex_);
		object_pool_.push_back(p);
	}
}; // class ObjectPool

} // namespace rpc

#endif // _OBJECT_POOL_HPP__