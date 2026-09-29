#pragma once
#include <cstdint>

class reg
{
public:
    uint8_t high(const uint8_t value);
    uint8_t low(const uint8_t value);
    [[nodiscard]] uint8_t high() const;
    [[nodiscard]] uint8_t low() const;
    void value(const uint16_t value);
    void value(const uint8_t* memory);
    [[nodiscard]] uint16_t& value();
    [[nodiscard]] uint16_t value() const;

private:
    uint16_t _value{ 0 };
};
