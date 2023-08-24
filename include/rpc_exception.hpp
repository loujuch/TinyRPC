#ifndef _RPC_EXCEPTION_HPP__
#define _RPC_EXCEPTION_HPP__

#include <exception>

namespace rpc {

class RPCException :public std::exception {
	const char *what_str_;
public:
	RPCException(const char *what_str) :what_str_(what_str) {
	}

	inline const char *what() const throw() {
		return what_str_;
	}
}; // class RPCException

} // namespace rpc

#endif // _RPC_EXCEPTION_HPP__