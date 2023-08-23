#include "rpc_data.hpp"

#include <endian.h>
#include <string.h>

#include <iostream>

rpc::RPCData::RPCData() :
	visitor(0) {
}

void rpc::RPCData::write(bool v) {
	char type = BOOL;
	rpc_data_.push_back(type);
	rpc_data_.push_back((char)v);
}

void rpc::RPCData::write(char v) {
	char type = CHAR;
	rpc_data_.push_back(type);
	rpc_data_.push_back(v);
}

void rpc::RPCData::write(int16_t v) {
	char type = INT16;
	rpc_data_.push_back(type);
	v = htobe16(v);
	write((char *)&v, sizeof(v));
}

void rpc::RPCData::write(int32_t v) {
	char type = INT32;
	rpc_data_.push_back(type);
	v = htobe32(v);
	write((char *)&v, sizeof(v));
}

void rpc::RPCData::write(int64_t v) {
	char type = INT64;
	rpc_data_.push_back(type);
	v = htobe64(v);
	write((char *)&v, sizeof(v));
}

void rpc::RPCData::write(float v) {
	static_assert(sizeof(v) == sizeof(int32_t));
	char type = FLOAT;
	rpc_data_.push_back(type);
	int32_t tmp = *((int32_t *)&v);
	tmp = htobe32(tmp);
	write((char *)&tmp, sizeof(tmp));
}

void rpc::RPCData::write(double v) {
	static_assert(sizeof(v) == sizeof(int64_t));
	char type = DOUBLE;
	rpc_data_.push_back(type);
	int64_t tmp = *((int64_t *)&v);
	tmp = htobe64(tmp);
	write((char *)&tmp, sizeof(tmp));
}

void rpc::RPCData::write(const char *v, uint32_t len) {
	for(int i = 0;i < len;++i) {
		rpc_data_.push_back(v[i]);
	}
}

void rpc::RPCData::write(const std::string &v) {
	static_assert(sizeof(v.size()) == sizeof(int64_t));
	char type = STRING;
	rpc_data_.push_back(type);
	write((int64_t)v.size());
	write(v.c_str(), v.size());
}

void rpc::RPCData::write(const Serializable &v) {
	char type = SERIALIZABLE;
	rpc_data_.push_back(type);
	v.serialize(*this);
}

bool rpc::RPCData::read(bool &v) {
	char type = rpc_data_[visitor++];
	if(type != BOOL) {
		return false;
	}
	v = rpc_data_[visitor++] != 0;
	return true;
}

bool rpc::RPCData::read(char &v) {
	char type = rpc_data_[visitor++];
	if(type != CHAR) {
		return false;
	}
	v = rpc_data_[visitor++];
	return true;
}

bool rpc::RPCData::read(int16_t &v) {
	char type = rpc_data_[visitor++];
	if(type != INT16) {
		return false;
	}
	read((char *)&v, sizeof(v));
	v = be16toh(v);
	return true;
}

bool rpc::RPCData::read(int32_t &v) {
	char type = rpc_data_[visitor++];
	if(type != INT32) {
		return false;
	}
	read((char *)&v, sizeof(v));
	v = be32toh(v);
	return true;
}

bool rpc::RPCData::read(int64_t &v) {
	char type = rpc_data_[visitor++];
	if(type != INT64) {
		return false;
	}
	read((char *)&v, sizeof(v));
	v = be64toh(v);
	return true;
}

bool rpc::RPCData::read(float &v) {
	static_assert(sizeof(v) == sizeof(int32_t));
	char type = rpc_data_[visitor++];
	if(type != FLOAT) {
		return false;
	}
	int32_t tmp;
	read((char *)&tmp, sizeof(tmp));
	tmp = be32toh(tmp);
	v = *((float *)&tmp);
	return true;
}

bool rpc::RPCData::read(double &v) {
	static_assert(sizeof(v) == sizeof(int64_t));
	char type = rpc_data_[visitor++];
	if(type != DOUBLE) {
		return false;
	}
	int64_t tmp;
	read((char *)&tmp, sizeof(tmp));
	tmp = be64toh(tmp);
	v = *((double *)&tmp);
	return true;
}

bool rpc::RPCData::read(char *v, uint32_t len) {
	::memcpy(v, (char *)&rpc_data_[visitor], len);
	visitor += len;
	return true;
}

bool rpc::RPCData::read(std::string &v) {
	static_assert(sizeof(v.size()) == sizeof(int64_t));
	v.clear();
	char type = rpc_data_[visitor++];
	if(type != STRING) {
		return false;
	}
	int64_t size;
	read(size);
	v = std::string(rpc_data_.begin() + visitor, rpc_data_.begin() + visitor + size);
	visitor += size;
	return true;
}

bool rpc::RPCData::read(Serializable &v) {
	char type = rpc_data_[visitor++];
	if(type != SERIALIZABLE) {
		return false;
	}
	return v.unserialize(*this);
}