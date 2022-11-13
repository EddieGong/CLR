#pragma once

#include <array>

namespace CLR
{
	using uint8  = unsigned char;
	using int8   = signed	char;
	using uint16 = unsigned short;
	using int16  = signed	short;
	using uint32 = unsigned int;
	using int32  = signed	int;

	static_assert(sizeof(uint8)  == 1);
	static_assert(sizeof(int8)   == 1);
	static_assert(sizeof(uint16) == 2);
	static_assert(sizeof(int16)  == 2);
	static_assert(sizeof(uint32) == 4);
	static_assert(sizeof(int32)  == 4);
}