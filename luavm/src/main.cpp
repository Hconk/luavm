#include <iostream>
#include <fstream>
#include  "chunk.hpp"


int main(int argc, char* argv[])
{
	using namespace luavm;
	LuaHeader head1;
	std::fstream is;
	is.open("luac.out", std::ios::in | std::ios::binary);
	is >> head1;
	Prototype t;
	is >> t;
	t.print();

	head1.print();
	
	std::fstream os1;
	os1.open("2", std::ios::out | std::ios::binary);
	os1 << head1;
	os1.close();

	return 0;
}
