#ifndef _SERIALIZABLE_HPP__
#define _SERIALIZABLE_HPP__

namespace rpc {

class RPCData;

class Serializable {
public:
	virtual void serialize(RPCData &stream) const = 0;
	virtual bool unserialize(RPCData &stream) = 0;
};

} // namespace rpc

#endif // _SERIALIZABLE_HPP__