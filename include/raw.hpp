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
		//return out << std::setiosflags(std::ios::uppercase) << std::hex << std::setw(2) << std::setfill('0') << fw.v + 0;
		return out << fw.v;
	}
	//针对对单个数据打包及解包函数模板的 浮点型变量 特化版本
	template<typename Stream>
	Stream& pack(Stream& out, double d) {
		auto p = reinterpret_cast<uint64_t*>(&d);
		for (int i = 0; i < 8; i++)
		{
			out << FixHex(static_cast<uint8_t>((*p >> (8 * i)) & 0xff));
		}
		return out;
	}

	template<typename Stream>
	Stream& unpack(Stream& in, double& d)
	{
		auto p = reinterpret_cast<uint64_t*>(&d);
		char t;
		for(int i = 0;i < 8;i++)
		{
			in.read(&t, 1);
			uint64_t t1 = t;
			*p = (t1 << (i * 8)) | *p;
		}
		return in;
	}

	//对单个变量进行打包，解包， 支持不同字节大小数据
	template<typename Stream, typename T>
	Stream& pack(Stream& out, T c)
	{
		int size = sizeof(c);
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
		char temp = 0;
		c = 0;
		for (int i = 0; i < size; i++)
		{
			in.read(&temp, 1);
			T t1 = temp;
			c = ((t1 << (i * 8)) | c);
		}
		return in;
	}


	//对多种类型数组数据进行打包及解包的模板函数
	template<typename Stream, typename T, int size>
	Stream& pack(Stream& out, T(&p)[size]) {
		std::copy(p, p + size, std::ostream_iterator<FixHex<T>>(out));
		return out;
	}
	
	template<typename Stream, typename T, int size>
	Stream& unpack(Stream& in, T(&p)[size])
	{
		T t = 0;
		for(int i = 0;i < size;i++)
		{
			unpack(in, t);
			p[i] = t;
			t = 0;
		}
		return in;
	}


	//对字符串数据进行打包及解包
	template<typename Stream>
	Stream& pack(Stream& out, std::string &str)
	{
		uint64_t str_len = str.size();
		if (str_len < 0xff)
		{
			uint8_t size = str_len;
			if(str_len != 0)
				pack(out, size, str.data());
		} else
		{
			pack(out, uint8_t(0xff), str_len, str.data());
		}
	}

	template<typename Stream>
	Stream& unpack(Stream& in, std::string& str)
	{
		uint8_t str_len = 0;
		uint64_t size = 0;
		unpack(in, str_len);
		if (str_len == 0)
			str = "";
		if(str_len == 0xff)
		{
			unpack(in, size);
		} else
		{
			size = str_len;
		}
		char temp = 0;
		for(int i = 0;i < size-1;i++)
		{
			unpack(in, temp);
			str.push_back(temp);
		}
		return in;
	}

	template<typename Stream, typename T>
	Stream& unpack(Stream& in, std::vector<T>& vec)
	{
		uint32_t size;
		unpack(in, size);
		for (int i = 0; i < size; i++)
		{
			T data;
			unpack(in, data);
			vec.push_back(data);
		}
		return in;
	}


	


	//打包方法的变参模板函数，支持多种类型，不限参数的对数据进行打包
	template<typename Stream, typename T, typename... Args>
	Stream& pack(Stream& out, T &d, Args&... args) {
		pack(out, d);
		pack(out, args...);
		return out;
	}
	//解包方法的变参模板函数，支持多种类型，不限参数的对数据进行解包
	template<typename Stream, typename T, typename... Args>
	Stream& unpack(Stream& in, T &d, Args&... args)
	{
		unpack(in, d);
		unpack(in, args...);
		return in;
	}

}


#endif
