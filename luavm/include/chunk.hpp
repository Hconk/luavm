#ifndef LUAVM_CHUNK_HPP
#define LUAVM_CHUNK_HPP
#include "versioh.hpp"
#include "raw.hpp"
#include <string>
#include <vector>
#include <variant>

namespace luavm
{


	class LuaHeader
	{
	public:
		template<typename  Stream>
		friend Stream& operator<<(Stream& out, LuaHeader&head);

		template<typename  Stream>
		friend Stream& operator>>(Stream& in, LuaHeader&head);

		void print();
	private:
		uint8_t signature[4]{};			// [ ESC, L, u, a ] ASCII码
		uint8_t version = 0;			//Lua 版本号 
		uint8_t format = 0;				//二进制chunk格式 ，官方实现默认版本号0
		uint8_t luacData[6]{};			//LUAC_DATA : 前两个字节0x1993 lua1.0发布年份， 后四个字节依次是 回车符号(0x0D), 换行符(0x0A), 替换符(0x1A) ,另一个换行符(0x0A)
		uint8_t cintSize = 0;
		uint8_t sizetSize = 0;
		uint8_t instructionSize = 0;
		uint8_t luaIntegerSize = 0;
		uint8_t luaNumberSize = 0;
		uint64_t luaInt = 0;
		double luacNum = 0;
	};

	template<typename Stream>
	Stream& operator<<(Stream& out, LuaHeader& head)
	{
		pack(out, head.signature, head.version, head.format, head.luacData, head.cintSize, head.sizetSize, \
				  head.instructionSize, head.luaIntegerSize, head.luaNumberSize, head.luaInt, head.luacNum);
		return out;
	}

	template<typename Stream>
	Stream& operator>>(Stream& in, LuaHeader& head)
	{
		unpack(in, head.signature, head.version, head.format, head.luacData, head.cintSize, head.sizetSize, \
					head.instructionSize, head.luaIntegerSize, head.luaNumberSize, head.luaInt, head.luacNum);
		return in;
	}



	class  Upvalue
	{
	public:
		template<typename Stream>
		friend Stream& operator>>(Stream& in, Upvalue& upv);
		void print();
	private:
		uint8_t Instack;
		uint8_t Idx;
	};

	template<typename Stream>
	Stream& operator>>(Stream& in, Upvalue& upv)
	{
		pack(in, upv.Instack, upv.Idx);
		return in;
	}
	class  Constant
	{
		enum tag
		{
			TAG_NIL = 0x00,
			TAG_BOOLAN = 0x01,
			TAG_NUMBER = 0x03,
			TAG_INTEGER = 0x13,
			TAG_SHORT_STR = 0x04,
			TAG_LONG_STR = 0x14
		};
	public:
		template<typename Stream>
		friend Stream& operator>>(Stream& in, Constant& con);

		void print();
	private:
		tag m_tag;
		std::variant<void*, bool,double,uint8_t, uint64_t, std::string> data;

	};

	template<typename Stream>
	Stream& operator>>(Stream& in, Constant& con)
	{
		uint8_t type;
		unpack(in, type);
		std::string str = "";
		switch (type)
		{
		case con.TAG_NIL:
			con.m_tag = Constant::TAG_NIL;
			con.data = nullptr;
			break;
		case con.TAG_BOOLAN:
			con.m_tag = Constant::TAG_BOOLAN;
			uint8_t t;
			unpack(in, t);
			con.data = t;
			break;
		case con.TAG_NUMBER:
			con.m_tag = Constant::TAG_NUMBER;
			double d;
			unpack(in, d);
			con.data = d;
			break;
		case con.TAG_INTEGER:
			con.m_tag = Constant::TAG_INTEGER;
			uint64_t u64;
			unpack(in, u64);
			con.data = u64;
			break;
		case con.TAG_SHORT_STR:
			con.m_tag = Constant::TAG_SHORT_STR;
			unpack(in, str);
			con.data = str;
			break;
		case con.TAG_LONG_STR:
			con.m_tag = Constant::TAG_LONG_STR;
			unpack(in, str);
			con.data = str;
		default:
			throw("Read Constant error!");
		}
		return in;
	}

	class  LocVar
	{
	public:
		template<typename Stream>
		friend Stream& operator>>(Stream& in, LocVar& lov);

		void print();
	private:
		std::string VarName;
		uint32_t StartPC;
		uint32_t EndPC;
	};
	template<typename Stream>
	Stream& operator>>(Stream& in, LocVar& lov)
	{
		unpack(in, lov.VarName, lov.StartPC, lov.EndPC);
		return in;
	}


	class  Prototype
	{
	public:
		template<typename Stream>
		friend Stream& operator>>(Stream& in, Prototype& proto);
		void print();

	private:
		std::string Source;
		uint32_t LineDefined;
		uint32_t LastLineDefined;
		uint8_t NumParams;
		uint8_t IsVararg;
		uint8_t MaxStackSize;
		std::vector<uint32_t> Code;
		std::vector<Constant> Constants;
		std::vector<Upvalue> Upvalues;
		std::vector<Prototype> Protos;
		std::vector<uint32_t> LineInfo;
		std::vector<LocVar> LocVars;
		std::vector<std::string> UpvalueNames;
	};

	template<typename Stream>
	Stream& operator>>(Stream& in, Prototype& proto)
	{
		//unknown
		uint8_t unknow = 0;
		unpack(in, unknow);
		unpack(in, proto.Source, proto.LineDefined, proto.LastLineDefined);
		unpack(in,proto.NumParams, proto.IsVararg, proto.MaxStackSize );
		unpack(in, proto.Code);
		uint32_t constant_size;

		//读取Constant
		unpack(in, constant_size);
		for(int i = 0;i < constant_size;i++)
		{
			Constant temp;
			in >> temp;
			proto.Constants.push_back(temp);
		}

		//读取up value
		uint32_t upv_size;
		unpack(in, upv_size);
		for(int i = 0;i < upv_size;i++)
		{
			Upvalue upv;
			in >> upv;
			proto.Upvalues.push_back(upv);
		}

		//读取Proto函数原型
		uint32_t pro_size;
		unpack(in, pro_size);
		for(int i = 0;i < pro_size;i++)
		{
			Prototype pro;
			in >> pro;
			proto.Protos.push_back(pro);
		}
		//读取行号表
		uint32_t lineInfo_size;
		unpack(in, lineInfo_size);
		for(int i = 0;i < lineInfo_size; i++)
		{
			uint32_t line_info;
			unpack(in, line_info);
			proto.LineInfo.push_back(line_info);
		}

		//读取局部变量
		uint32_t lov_size;
		unpack(in, lov_size);
		for(int i = 0;i < lov_size;i++)
		{
			LocVar lov;
			in >> lov;
			proto.LocVars.push_back(lov);
		}

		//读取Up value名字
		uint32_t upv_name_size;
		unpack(in, upv_name_size);
		for(int i = 0;i < upv_name_size;i++)
		{
			std::string name;
			unpack(in, name);
			proto.UpvalueNames.push_back(name);
		}
		return in;
	}

	class BinaryChunk
	{
	private:
		LuaHeader header;
		uint8_t byteUpvalues;
		Prototype* mainfunc;
	};

}

#endif
