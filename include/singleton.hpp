#ifndef _SINGLETON_HPP__
#define _SINGLETON_HPP__

namespace rpc {

template <typename T>
class Singleton {
protected:
	Singleton() = default;
	virtual ~Singleton() = default;
public:
	static T &getInstance() {
		static T tmp;
		return tmp;
	}

	Singleton(const Singleton &) = delete;
	Singleton(Singleton &&) = delete;

	Singleton &operator=(const Singleton &) = delete;
	Singleton &operator=(Singleton &&) = delete;
}; // class Singleton

} // namespace rpc

#endif // _SINGLETON_HPP__