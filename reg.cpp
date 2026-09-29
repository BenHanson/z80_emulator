#include "reg.hpp"

#include <cstdint>

uint8_t reg::high(const uint8_t value)
{
	_value &= 0x00ff;
	_value |= value << 8;
	return value;
}

uint8_t reg::low(const uint8_t value)
{
	_value &= 0xff00;
	_value |= value;
	return value;
}

uint8_t reg::high() const
{
	return _value >> 8;
}

uint8_t reg::low() const
{
	return _value & 0xff;
}

void reg::value(const uint16_t value)
{
	_value = value;
}

void reg::value(const uint8_t* memory)
{
	low(*memory);
	high(*++memory);
}

uint16_t& reg::value()
{
	return _value;
}

uint16_t reg::value() const
{
	return _value;
}
