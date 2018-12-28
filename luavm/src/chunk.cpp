#include "chunk.hpp"
#include <iostream>

namespace luavm
{
	/*
			Ĭ��ֵ
	uint8_t signature[4]{ 0x1B,0x4C,0x75,0x61 };					// [ ESC, L, u, a ] ASCII��
	uint8_t version = MAJOR_VERSION * 16 + MINOR_VERSION;			//Lua �汾��
	uint8_t format = 0;												//������chunk��ʽ ���ٷ�ʵ��Ĭ�ϰ汾��0
	uint8_t luacData[6]{ 0x19,0x93,0x0D,0x0A,0x1A,0x0A};			//LUAC_DATA : ǰ�����ֽ�0x1993 lua1.0������ݣ� ���ĸ��ֽ������� �س�����(0x0D), ���з�(0x0A), �滻��(0x1A) ,��һ�����з�(0x0A)
	uint8_t cintSize = 4;
	uint8_t sizetSize = 8;
	uint8_t instructionSize = 4;
	uint8_t luaIntegerSize = 8;
	uint8_t luaNumberSize = 8;
	uint64_t luaInt = 0x5678;
	double luacNum = 370.5;

	*/

	void phex(uint8_t *p, int len)
	{
		for(int i = 0;i < len;i++)
		{
			std::cout << std::setiosflags(std::ios::uppercase) << std::hex << std::setw(2) << std::setfill('0') << p[i] + 0;
		}
		std::cout << '\n';
	}
	void LuaHeader::print()
	{
		std::cout.fill(0);
		std::cout << "Sinature[4]: \t";
		phex(signature, sizeof(signature));
		std::cout << "version: \t";
		phex(&version, sizeof(version));
		std::cout << "format: \t";
		phex(&format, sizeof(format));
		std::cout << "luacData: \t";
		phex(luacData, sizeof(luacData));
		std::cout << "cintSize: \t";
		phex(&cintSize, sizeof(cintSize));
		std::cout << "sizetSize: \t";
		phex(&sizetSize, sizeof(sizetSize));
		std::cout << "instructionSize: \t";
		phex(&instructionSize, sizeof(instructionSize));
		std::cout << "luaIntegerSize: \t";
		phex(&luaIntegerSize, sizeof(luaIntegerSize));
		std::cout << "luaNumberSize: \t";
		phex(&luaNumberSize, sizeof(luaNumberSize));
		std::cout << "luaInt: \t" << luaInt << '\n';
		std::cout << "luacNum: \t" << luacNum << '\n';
		
	}


}
