#pragma once

#include "reg.hpp"

#include <cstdint>

namespace FLAG
{
    const uint8_t C = 0x01;
    const uint8_t N = 0x02;
    const uint8_t PV = 0x04;
    const uint8_t BIT3 = 0x08;
    const uint8_t H = 0x10;
    const uint8_t BIT5 = 0x20;
    const uint8_t Z = 0x40;
    const uint8_t S = 0x80;
};

struct z80
{
    struct registers
    {
        reg _AF;
        reg _BC;
        reg _DE;
        reg _HL;
        reg _IR;
        reg _IX;
        reg _IY;
    };

    registers _registers;
    registers _registers_prime;
    registers* _curr_reg{ &_registers };
    uint16_t _PC{ 0 };
    reg _SP;

    void step(uint8_t* memory);
    void step_ext(uint8_t* memory);
    void step_bits(uint8_t* memory);
    void step_IX_IY(uint8_t* memory, const char xy);
    void step_IX_IY_bits(uint8_t* memory, const char xy);

    void call(uint8_t* memory);
    void jp(const uint8_t* memory);
    void jp_register(const uint8_t* memory, const reg& source);
    void jr(const uint8_t* memory);
    void pop(const uint8_t* memory, reg& dest);
    void push(uint8_t* memory, reg& dest);
    void ret(const uint8_t* memory);

    reg& ixy(const char xy);
    [[nodiscard]] uint16_t& ixy_value(const char xy);
    void ixy_value(const char xy, const uint8_t* memory);
    [[nodiscard]] uint8_t ixy_high(const char xy) const;
    void ixy_high(const char xy, const uint8_t val);
    [[nodiscard]] uint8_t ixy_low(const char xy) const;
    void ixy_low(const char xy, const uint8_t val);

    [[nodiscard]] const uint8_t* peek_indirect(const uint8_t* memory);
    [[nodiscard]] uint16_t peek_word(const uint8_t* memory) const;
    void poke_register_indirect(uint8_t* memory, const reg& source);
    void poke_word(uint8_t* memory, const uint16_t value) const;

    void rlca();
    void rrca();
    void add_a(const uint8_t rhs);
    void add(reg& lhs, const reg& rhs);
    void adc_a(const uint8_t rhs);
    void adc_hl(const reg& rhs);
    void cp_a(const uint8_t rhs);
    void sub_a(const uint8_t rhs);
    void sbc_a(const uint8_t rhs);
    void sbc_hl(const reg& rhs);

    [[nodiscard]] uint8_t inc(const uint8_t old_val);
    [[nodiscard]] uint8_t dec(const uint8_t old_val);

    void and_a(const uint8_t val);
    void xor_a(const uint8_t val);
    void or_a(const uint8_t val);

    void bit(const bool set);

    [[nodiscard]] uint8_t rla(uint8_t val);
    [[nodiscard]] uint8_t rra(uint8_t val);
    [[nodiscard]] uint8_t rlc(uint8_t val);
    [[nodiscard]] uint8_t rrc(uint8_t val);
    [[nodiscard]] uint8_t rl(uint8_t val);
    [[nodiscard]] uint8_t rr(uint8_t val);
    [[nodiscard]] uint8_t sla(uint8_t val);
    [[nodiscard]] uint8_t sra(uint8_t val);
    [[nodiscard]] uint8_t sll(uint8_t val);
    [[nodiscard]] uint8_t srl(uint8_t val);

private:
    [[nodiscard]] uint8_t logic_flags(const uint8_t A) const;
    [[nodiscard]] bool parity(uint8_t value) const;
};
