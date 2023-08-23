#include <iostream>

#include "serializable.hpp"
#include "rpc_data.hpp"

#include <assert.h>

class test0 :public rpc::Serializable {
	bool b1;
	char c1;
	int16_t i161;
	int32_t i321;
	int64_t i641;
	float f1;
	double d1;
	std::string s1;
	std::vector<std::string> vs1;
	std::vector<double> vd1;
public:
	test0() :b1(false), c1(0), i161(0), i321(0), i641(0),
		f1(0), d1(0), s1("none"), vs1{ "none1", "none2" }, vd1{ -1, -2, -3 } {
	}

	void change() {
		b1 = true;
		c1 = ~c1;
		i161 = ~i161;
		i321 = ~i321;
		i641 = ~i641;
		f1 = 1029.122;
		d1 = 2918.102918;
		s1 = "change";
		vs1 = std::move(std::vector<std::string>{ "vs1", "vs2", "vs3", "vs4" });
		vd1 = std::move(std::vector<double>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 });
	}

	void cmp(const test0 &t) {
		assert(b1 == t.b1);
		assert(c1 == t.c1);
		assert(i161 == t.i161);
		assert(i321 == t.i321);
		assert(i641 == t.i641);
		assert(f1 == t.f1);
		assert(d1 == t.d1);
		assert(s1 == t.s1);
		assert(vs1.size() != t.vs1.size() || vd1.size() != t.vd1.size());
		for(int i = 0;i < vs1.size();++i) {
			assert(vs1[i] == t.vs1[i]);
		}
		for(int i = 0;i < vd1.size();++i) {
			assert(vd1[i] == t.vd1[i]);
		}
	}

	bool operator==(const test0 &t) {
		bool out = true;
		out = out && (b1 == t.b1);
		out = out && (c1 == t.c1);
		out = out && (i161 == t.i161);
		out = out && (i321 == t.i321);
		out = out && (i641 == t.i641);
		out = out && (f1 == t.f1);
		out = out && (d1 == t.d1);
		out = out && (s1 == t.s1);
		if(vs1.size() != t.vs1.size() || vd1.size() != t.vd1.size()) {
			return false;
		}
		for(int i = 0;i < vs1.size();++i) {
			out = out && (vs1[i] == t.vs1[i]);
		}
		for(int i = 0;i < vd1.size();++i) {
			out = out && (vd1[i] == t.vd1[i]);
		}
		return out;
	}

	void serialize(rpc::RPCData &stream) const {
		stream.write(b1);
		stream.write(c1);
		stream.write(i161);
		stream.write(i321);
		stream.write(i641);
		stream.write(f1);
		stream.write(d1);
		stream.write(s1);
		stream.write(vs1);
		stream.write(vd1);
	}

	bool unserialize(rpc::RPCData &stream) {
		stream.read(b1);
		stream.read(c1);
		stream.read(i161);
		stream.read(i321);
		stream.read(i641);
		stream.read(f1);
		stream.read(d1);
		stream.read(s1);
		stream.read(vs1);
		stream.read(vd1);
		return true;
	}
};

class test :public rpc::Serializable {
	bool b1;
	char c1;
	int16_t i161;
	int32_t i321;
	int64_t i641;
	float f1;
	double d1;
	std::string s1;
	std::vector<std::string> vs1;
	std::vector<double> vd1;
	test0 t1;
public:
	test() :b1(true), c1(192), i161(19731), i321(182731), i641(192836),
		f1(12093.11), d1(1827361.01273), s1("n"), vs1{ "n", "n1", "n2" }, vd1{ -1, 2, -3 } {
	}

	void cmp(const test &t) {
		assert(b1 == t.b1);
		assert(c1 == t.c1);
		assert(i161 == t.i161);
		assert(i321 == t.i321);
		assert(i641 == t.i641);
		assert(f1 == t.f1);
		assert(d1 == t.d1);
		assert(s1 == t.s1);
		assert(vs1.size() == t.vs1.size() && vd1.size() == t.vd1.size());
		for(int i = 0;i < vs1.size();++i) {
			assert(vs1[i] == t.vs1[i]);
		}
		for(int i = 0;i < vd1.size();++i) {
			assert(vd1[i] == t.vd1[i]);
		}
		assert(t1 == t.t1);
	}

	void change() {
		b1 = false;
		c1 = ~c1;
		i161 = ~i161;
		i321 = ~i321;
		i641 = ~i641;
		f1 = 12.10281;
		d1 = 11111.21928;
		s1 = "changewww";
		vs1 = std::move(std::vector<std::string>{ "vs1tt", "tvs2", "vts3", "vts4" });
		vd1 = std::move(std::vector<double>{  7, 8, 9, 1, 2, 3, 4, 5, 6, 0 });
		t1.change();
	}

	bool operator==(const test &t) {
		bool out = true;
		out = out && (b1 == t.b1);
		out = out && (c1 == t.c1);
		out = out && (i161 == t.i161);
		out = out && (i321 == t.i321);
		out = out && (i641 == t.i641);
		out = out && (f1 == t.f1);
		out = out && (d1 == t.d1);
		out = out && (s1 == t.s1);
		if(vs1.size() != t.vs1.size() || vd1.size() != t.vd1.size()) {
			return false;
		}
		for(int i = 0;i < vs1.size();++i) {
			out = out && (vs1[i] == t.vs1[i]);
		}
		for(int i = 0;i < vd1.size();++i) {
			out = out && (vd1[i] == t.vd1[i]);
		}
		return out && (t1 == t.t1);
	}

	void serialize(rpc::RPCData &stream) const {
		stream.write(b1);
		stream.write(c1);
		stream.write(i161);
		stream.write(i321);
		stream.write(i641);
		stream.write(f1);
		stream.write(d1);
		stream.write(s1);
		stream.write(vs1);
		stream.write(vd1);
		stream.write(t1);
	}

	bool unserialize(rpc::RPCData &stream) {
		stream.read(b1);
		stream.read(c1);
		stream.read(i161);
		stream.read(i321);
		stream.read(i641);
		stream.read(f1);
		stream.read(d1);
		stream.read(s1);
		stream.read(vs1);
		stream.read(vd1);
		stream.read(t1);
		return true;
	}
};

int main() {
	test t1, t2;
	t1.change();
	rpc::RPCData data;
	t1.serialize(data);
	if(t1 == t2) {
		std::cout << "YES" << std::endl;
	} else {
		std::cout << "NO" << std::endl;
	}
	t2.unserialize(data);
	t1.cmp(t2);
	if(t1 == t2) {
		std::cout << "YES" << std::endl;
	} else {
		std::cout << "NO" << std::endl;
	}
	test t3;
	if(t1 == t3) {
		std::cout << "YES" << std::endl;
	} else {
		std::cout << "NO" << std::endl;
	}
	data.reset();
	t3.unserialize(data);
	if(t2 == t3) {
		std::cout << "YES" << std::endl;
	} else {
		std::cout << "NO" << std::endl;
	}
	return 0;
}