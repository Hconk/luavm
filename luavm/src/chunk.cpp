#include "chunk.hpp"
#include <iostream>

#define STR(x) #x
#define PRINT_NAME_HEX(name) do {	\
	std::cout << STR(name)": \t" ;	\
	phex(name, sizeof(name));		\
	std::cout << std::endl;			\
} while(0);
#define PRINT_HEX(name) do {	\
	phex(name, sizeof(name));	\
} while(0);
namespace luavm
{
	/*
			默认值
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

	*/

	template<typename T>
	void phex(T data, int len)
	{
		uint8_t *p = reinterpret_cast<uint8_t*>(&data);
		for(int i = 0;i < len;i++)
		{
			std::cout << std::setiosflags(std::ios::uppercase) << std::hex << std::setw(2) << std::setfill('0') << p[i] + 0;
		}
	}
	void LuaHeader::print()
	{
		std::cout.fill(0);
		PRINT_NAME_HEX(signature);
		PRINT_NAME_HEX(version);
		PRINT_NAME_HEX(format);
		PRINT_NAME_HEX(luacData);
		PRINT_NAME_HEX(cintSize);
		PRINT_NAME_HEX(sizetSize);
		PRINT_NAME_HEX(instructionSize);
		PRINT_NAME_HEX(luaIntegerSize);
		PRINT_NAME_HEX(luaNumberSize);
		std::cout << "luaInt: \t" << luaInt << '\n';
		std::cout << "luacNum: \t" << luacNum << '\n';
		
	}


	void Prototype::print()
	{
		std::cout << "Source: \t" << Source << std::endl;
		PRINT_NAME_HEX(LineDefined);
		PRINT_NAME_HEX(LastLineDefined);
		PRINT_NAME_HEX(NumParams);
		PRINT_NAME_HEX(IsVararg);
		PRINT_NAME_HEX(MaxStackSize);
		int count = 0;
		std::cout << "Code : " << std::endl;
		for(auto code: Code)
		{
			std::cout << "[" << ++count << "]";
			PRINT_HEX(code);
			std::cout << std::endl;
		}

		std::cout << "constant (" << Constants.size() << "):" << std::endl;
		count = 0;
		for (auto constant : Constants)
		{
			std::cout << "[" << ++count << "]";
			constant.print();
			std::cout << std::endl;
		}

		std::cout << "Upvalues (" << Upvalues.size() << "):" << std::endl;
		count = 0;
		for (auto upvalue : Upvalues)
		{
			std::cout << "[" << ++count << "]";
			upvalue.print();
			std::cout << std::endl;
		}

		std::cout << "Protos (" << Protos.size() << "):" << std::endl;
		count = 0;
		for (auto proto : Protos)
		{
			std::cout << "[" << ++count << "]";
			proto.print();
			std::cout << std::endl;
		}

		count = 0;
		std::cout << "LineInfol() : " << std::endl;
		for (auto line_info : LineInfo)
		{
			std::cout << "[" << ++count << "]";
			PRINT_HEX(line_info);
			std::cout << std::endl;
		}

		count = 0;
		std::cout << "LocVars() : " << std::endl;
		for (auto locvar : LocVars)
		{
			std::cout << "[" << ++count << "]";
			locvar.print();
			std::cout << std::endl;
		}

		count = 0;
		std::cout << "UpvalueName() : " << std::endl;
		for (auto upv_name : UpvalueNames)
		{
			std::cout << "[" << ++count << "]";
			std::cout << upv_name << std::endl;
		}

	}

	void Constant::print()
	{
		std::visit([](auto&& arg) {std::cout << arg; }, data);
	}

	void Upvalue::print()
	{
		std::cout << "Instack: \t";
		PRINT_HEX(Instack);
		std::cout << "\tIdx: \t";
		PRINT_HEX(Idx);
	}

	void LocVar::print()
	{
		std::cout << "VarName: " << VarName;
		std::cout << " StartPc: ";
		pack(std::cout, StartPC);
		std::cout << "\tEndPc: ";
		pack(std::cout, EndPC);
	}

}
