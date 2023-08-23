#ifndef _RPC_DATA_HPP__
#define _RPC_DATA_HPP__

#include <vector>
#include <string>

#include "serializable.hpp"

#include <stdint.h>

namespace rpc {

// 永远使用网络字节序
class RPCData {
private:
	enum DataType :uint8_t {
		BOOL = 0,
		CHAR,
		INT16,
		INT32,
		INT64,
		FLOAT,
		DOUBLE,
		STRING,
		VECTOR,
		SERIALIZABLE
	}; // enum DataType

	std::size_t visitor;
	std::vector<char> rpc_data_;
public:
	RPCData();

	inline const char *str() const {
		return &rpc_data_[0];
	}

	inline char *str() {
		return &rpc_data_[0];
	}

	inline void assign(int64_t size) {
		rpc_data_.resize(size);
	}

	inline void reset() {
		visitor = 0;
	}

	inline void clear() {
		visitor = 0;
		rpc_data_.clear();
	}

	inline decltype(std::vector<char>().size()) size() const {
		return rpc_data_.size();
	}

	void write(bool v);
	void write(char v);
	void write(int16_t v);
	void write(int32_t v);
	void write(int64_t v);
	void write(float v);
	void write(double v);
	void write(const char *v, uint32_t len);
	void write(const std::string &v);
	void write(const Serializable &v);

	template <typename V>
	void write(const std::vector<V> &v) {
		static_assert(sizeof(v.size()) == sizeof(int64_t));
		char type = DataType::VECTOR;
		rpc_data_.push_back(type);
		write((int64_t)v.size());
		for(auto &i : v) {
			write(i);
		}
	}

	template <typename T, typename ...Args>
	void write_args(const T &head, const Args&... args) {
		write(head);
		write_args(args...);
	}

	inline void write_args() {
	}

	bool read(bool &v);
	bool read(char &v);
	bool read(int16_t &v);
	bool read(int32_t &v);
	bool read(int64_t &v);
	bool read(float &v);
	bool read(double &v);
	bool read(char *v, uint32_t len);
	bool read(std::string &v);
	bool read(Serializable &v);

	template <typename V>
	bool read(std::vector<V> &v) {
		static_assert(sizeof(v.size()) == sizeof(int64_t));
		v.clear();
		char type = rpc_data_[visitor++];
		if(type != VECTOR) {
			return false;
		}
		int64_t size;
		if(!read(size)) {
			return false;
		}
		v.resize(size);
		for(auto &i : v) {
			V tmp;
			read(tmp);
			i = std::move(tmp);
		}
		return true;
	}

	template <typename T, typename ...Args>
	bool read_args(T &head, Args&... args) {
		return read(head) && read_args(args...);
	}

	inline constexpr bool read_args() {
		return true;
	}
}; // class RPCData

}

#endif // _RPC_DATA_HPP__