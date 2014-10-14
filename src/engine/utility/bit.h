#pragma once

#include <stdint.h>

template <uint32_t b>
static constexpr uint32_t bit()
{
	return 1 << b;
}

inline bool CheckBit(uint32_t b, uint32_t b2)
{
	return (b&b2)==b2;
}

inline bool CheckBit(uint8_t b, uint8_t b2)
{
	return (b&b2)==b2;
}

inline void RemoveBit(uint32_t & b, uint32_t b2)
{
    b &= ~b2;
}

inline void RemoveBit(uint8_t & b, uint8_t b2)
{
    b &= ~b2;
}

inline void AddBit(uint32_t & b, uint32_t b2)
{
    b |= b2;
}

enum BITS
{
	BIT0 = bit<0>(),
	BIT1 = bit<1>(),
	BIT2 = bit<2>(),
	BIT3 = bit<3>(),
	BIT4 = bit<4>(),
	BIT5 = bit<5>(),
	BIT6 = bit<6>(),
	BIT7 = bit<7>(),
	BIT8 = bit<8>(),
	BIT9 = bit<9>(),
	BIT10 = bit<10>(),
	BIT11 = bit<11>(),
	BIT12 = bit<12>(),
	BIT13 = bit<13>(),
	BIT14 = bit<14>(),
	BIT15 = bit<15>(),
	BIT16 = bit<16>(),
	BIT17 = bit<17>(),
	BIT18 = bit<18>(),
	BIT19 = bit<19>(),
	BIT20 = bit<20>(),
	BIT21 = bit<21>(),
	BIT22 = bit<22>(),
	BIT23 = bit<23>(),
	BIT24 = bit<24>(),
	BIT25 = bit<25>(),
	BIT26 = bit<26>(),
	BIT27 = bit<27>(),
	BIT28 = bit<28>(),
	BIT29 = bit<29>(),
	BIT30 = bit<30>(),
	BIT31 = bit<31>()
};
