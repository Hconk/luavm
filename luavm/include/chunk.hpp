#ifndef LUAVM_CHUNK_HPP
#define LUAVM_CHUNK_HPP
#include "versioh.hpp"
#include "raw.hpp"
#include <string>
#include <vector>

namespace luavm
{


	class LuaHeader
	{
	public:
		template<typename  Stream>
		friend Stream& operator<<(Stream& out, LuaHeader&head);
	private:
		uint8_t signature[4]{ 0x1B,0x4C,0x75,0x61 };					// [ ESC, L, u, a ] ASCII码
		uint8_t version = MAJOR_VERSION * 16 + MINOR_VERSION;			//Lua 版本号 
		uint8_t format = 0;												//二进制chunk格式 ，官方实现默认版本号0
		uint8_t luacData[6]{ 0x19,0x93,0x0D,0x0A,0x1A,0x0A};			//LUAC_DATA : 前两个字节0x1993 lua1.0发布年份， 后四个字节依次是 回车符号(0x0D), 换行符(0x0A), 替换符(0x1A) ,另一个换行符(0x0A)
		uint8_t cintSize = 4;
		uint8_t sizetSize = 8;
		uint8_t instructionSize = 4;
		uint8_t luaIntegerSize = 8;
		uint8_t luaNumberSize = 8;
		uint64_t luaInt = 0x5678;
		double luacNum = 370.5;
	};

	template<typename Stream>
	Stream& operator<<(Stream& out, LuaHeader& head)
	{
		pack(out, head.signature, head.version, head.format, head.luacData, head.cintSize, head.sizetSize, head.instructionSize, head.luaIntegerSize, head.luaNumberSize, head.luaInt, head.luacNum);
		return out;
	}




	class  Upvalue
	{
	private:
		uint8_t Instack;
		uint8_t Idx;
	};

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
	private:
		tag m_tag;

	};

	class  LocVar
	{
	private:
		std::string					VarName;
		uint32_t					StartPC;
		uint32_t					EndPC;
	};
	class  Prototype
	{
	private:
		std::string					Source;
		uint32_t					LineDefined;
		uint32_t					LastLineDefined;
		uint8_t						NumParams;
		uint8_t						IsVararg;
		uint8_t						MaxStackSize;
		std::vector<uint32_t>		Code;
		std::vector<Constant>		Constants;
		std::vector<Upvalue>		Upvalues;
		std::vector<Prototype>		Protos;
		std::vector<uint32_t>		LineInfol;
		std::vector<LocVar>			LocVars;
		std::vector<std::string>	UpvalueNames;
	};

	class BinaryChunk
	{
	private:
		LuaHeader					header;
		uint8_t						byteUpvalues;
		Prototype*					mainfunc;
	};

}

#endif
