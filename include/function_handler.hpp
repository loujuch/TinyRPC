#ifndef _FUNCTION_HANDLER_HPP__
#define _FUNCTION_HANDLER_HPP__

#include <string>
#include <unordered_map>
#include <functional>

#include "rpc_data.hpp"

namespace rpc {

class FunctionHandler {
	std::unordered_map<std::string,
		std::function<void(RPCData &, RPCData &)>>handler_;
public:
	template <typename F>
	bool bind(const std::string &func_name, F func) {
		auto p = handler_.find(func_name);
		if(p != handler_.end()) {
			return false;
		}
		handler_.emplace(func_name, std::bind(&FunctionHandler::wrap<F>, this,
			func, std::placeholders::_1, std::placeholders::_2));
		return true;
	}

	bool call(const std::string &func_name, RPCData &in, RPCData &out) {
		auto p = handler_.find(func_name);
		if(p == handler_.end()) {
			return false;
		}
		p->second(in, out);
		return true;
	}
private:
	template <typename F>
	void wrap(F func, RPCData &in, RPCData &out) {
		wrap_impl(func, in, out);
	}

	template <typename R, typename ...Args>
	void wrap_impl(R(*func)(Args...), RPCData &in, RPCData &out) {
		wrap_impl(std::function<R(Args...)>(func), in, out);
	}

	template <typename R, typename ...Args>
	void wrap_impl(std::function<R(Args...)> func, RPCData &in, RPCData &out) {
		using args_type = std::tuple<typename std::decay<Args>::type...>;

		constexpr auto size = std::tuple_size<typename std::decay<args_type>::type>::value;
		args_type args = get_args<args_type>(in, std::make_index_sequence<size>{});
		auto ret = call_impl<R>(func, args);
		out.write(ret);
	}

	template<typename Tuple, std::size_t... I>
	Tuple get_args(RPCData &in, std::index_sequence<I...>) {
		Tuple t;
		std::initializer_list<int>{((get_arg<Tuple, I>(in, t)), 0)...};
		return t;
	}

	template<typename Tuple, std::size_t Id>
	void get_arg(RPCData &in, Tuple &t) {
		in.read(std::get<Id>(t));
	}

	template<typename R, typename F, typename Tuple>
	typename std::enable_if<std::is_same<R, void>::value, int32_t>::type
		call_impl(F func, Tuple args) {
		invoke<R>(func, args);
		return 0;
	}

	template<typename R, typename F, typename Tuple>
	typename std::enable_if<!std::is_same<R, void>::value, R>::type
		call_impl(F func, Tuple args) {
		return invoke<R>(func, args);
	}

	template<typename R, typename F, typename Tuple>
	auto invoke(F &&func, Tuple &&t) {
		constexpr auto size = std::tuple_size<typename std::decay<Tuple>::type>::value;

		return invoke_impl<R>(std::forward<F>(func), std::forward<Tuple>(t),
			std::make_index_sequence<size>{});
	}

	template<typename R, typename F, typename Tuple, std::size_t... Index>
	auto invoke_impl(F &&func, Tuple &&t, std::index_sequence<Index...>) {
		return func(std::get<Index>(std::forward<Tuple>(t))...);
	}
}; // class FunctionHandler

} // namespace rpc

#endif // _FUNCTION_HANDLER_HPP__