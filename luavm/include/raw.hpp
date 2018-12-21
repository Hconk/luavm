#ifndef LUAVM_RAW_HPP
#define LUAVM_RAW_HPP
#include <iterator>
#include <vector>
#include <iomanip>

namespace luavm
{
	template<typename T>
	struct FixHex
	{
		FixHex(T v_) : v(v_) {}
		T v;
	};
	template<typename T>
	std::ostream& operator<<(std::ostream& out, const FixHex<T> &fw)
	{
		return out << std::setiosflags(std::ios::uppercase) << std::hex << std::setw(2) << std::setfill('0') << fw.v + 0;
	}

	template<typename Stream>
	Stream& pack(Stream& out, double d) {
		auto p = reinterpret_cast<uint64_t*>(&d);
		for (int i = 0; i < 8; i++)
		{
			out << FixHex(static_cast<uint8_t>((*p >> (8 * i)) & 0xff));
		}
		return out;
	}

	template<typename Stream, typename T, int size>
	Stream& pack(Stream& out, T(&p)[size]) {
		std::copy(p, p + size, std::ostream_iterator<FixHex<T>>(out));
		return out;
	}
	template<typename Stream, typename T>
	Stream& pack(Stream& out, T c)
	{
		int size = sizeof(c);
		std::vector<uint8_t> v_int8;
		for (int i = 0; i < size; i++)
		{
			out << FixHex(static_cast<uint8_t>((c >> (8 * i) & 0xff)));
		}
		return out;
	}

	template<typename Stream, typename T>
	Stream& unpack(Stream& in, T& c)
	{
		int size = sizeof(c);
		std::vector<uint8_t> v_uint8;
		char temp;
		for(int i = 0;i < size; i++)
		{
			in.read(&temp,1);
			v_uint8.push_back(temp);
			c = ((c << (i * 8) )| temp);
		}
		return in;
	}

	template<typename Stream, typename T, typename... Args>
	Stream& pack(Stream& out, T &d, Args&... args) {
		pack(out, d);
		pack(out, args...);
		return out;
	}

	template<typename Stream, typename T, typename... Args>
	Stream& unpack(Stream& in, T &d, Args&... args)
	{
		unpack(in, d);
		unpack(in, args...);
	}

}


#endif
