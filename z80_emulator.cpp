#include "z80_emulator.hpp"

#include <cstdint>
#include <utility>

void z80::step(uint8_t* memory)
{
	switch (memory[_PC])
	{
	case 0x00:
		// NOP
		// No flags affected
		++_PC;
		break;
	case 0x01:
		// LD BC, nn
		_curr_reg->_BC.value(&memory[++_PC]);
		// No flags affected
		_PC += 2;
		break;
	case 0x02:
		// LD (BC), A
		memory[_curr_reg->_BC.value()] = _curr_reg->_AF.high();
		// No flags affected
		++_PC;
		break;
	case 0x03:
		// INC BC
		++_curr_reg->_BC.value();
		// No flags affected
		++_PC;
		break;
	case 0x04:
		// INC B
		_curr_reg->_BC.high(inc(_curr_reg->_BC.high()));
		++_PC;
		break;
	case 0x05:
		// DEC B
		_curr_reg->_BC.high(dec(_curr_reg->_BC.high()));
		++_PC;
		break;
	case 0x06:
		// LD B, n
		_curr_reg->_BC.high(memory[++_PC]);
		// No flags affected
		++_PC;
		break;
	case 0x07:
		// RLCA
		rlca();
		++_PC;
		break;
	case 0x08:
		// EX AF,AF'
		std::swap(_registers._AF, _registers_prime._AF);
		// No flags affected
		++_PC;
		break;
	case 0x09:
		// ADD HL,BC
		add(_curr_reg->_HL, _curr_reg->_BC);
		++_PC;
		break;
	case 0x0a:
		// LD A,(BC)
		_curr_reg->_AF.high(memory[_curr_reg->_BC.value()]);
		// No flags affected
		++_PC;
		break;
	case 0x0b:
		// DEC BC
		--_curr_reg->_BC.value();
		// No flags affected
		++_PC;
		break;
	case 0x0c:
		// INC C
		_curr_reg->_BC.low(inc(_curr_reg->_BC.low()));
		++_PC;
		break;
	case 0x0d:
		// DEC C
		_curr_reg->_BC.low(dec(_curr_reg->_BC.low()));
		++_PC;
		break;
	case 0x0e:
		// LD C, n
		_curr_reg->_BC.low(memory[++_PC]);
		// No flags affected
		++_PC;
		break;
	case 0x0f:
		// RRCA
		rrca();
		++_PC;
		break;
	case 0x10:
		// DJNZ n
		_curr_reg->_BC.high(dec(_curr_reg->_BC.high()));

		if (_curr_reg->_BC.high())
			jr(memory);
		else
			_PC += 2;

		break;
	case 0x11:
		// LD DE, nn
		_curr_reg->_DE.value(&memory[++_PC]);
		// No flags affected
		_PC += 2;
		break;
	case 0x12:
		// LD (DE), A
		memory[_curr_reg->_DE.value()] = _curr_reg->_AF.high();
		// No flags affected
		++_PC;
		break;
	case 0x13:
		// INC DE
		++_curr_reg->_DE.value();
		// No flags affected
		++_PC;
		break;
	case 0x14:
		// INC D
		_curr_reg->_DE.high(inc(_curr_reg->_DE.high()));
		++_PC;
		break;
	case 0x15:
		// DEC D
		_curr_reg->_DE.high(dec(_curr_reg->_DE.high()));
		++_PC;
		break;
	case 0x16:
		// LD D, n
		_curr_reg->_DE.high(memory[++_PC]);
		// No flags affected
		++_PC;
		break;
	case 0x17:
		// RLA
		_curr_reg->_AF.high(rla(_curr_reg->_AF.high()));
		++_PC;
		break;
	case 0x18:
		// JR d
		jr(memory);
		// No flags affected
		break;
	case 0x19:
		// ADD HL, DE
		add(_curr_reg->_HL, _curr_reg->_DE);
		++_PC;
		break;
	case 0x1a:
		// LD A, (DE)
		_curr_reg->_AF.high(memory[_curr_reg->_DE.value()]);
		// No flags affected
		++_PC;
		break;
	case 0x1b:
		// DEC DE
		--_curr_reg->_DE.value();
		// No flags affected
		++_PC;
		break;
	case 0x1c:
		// INC E
		_curr_reg->_DE.low(inc(_curr_reg->_DE.low()));
		++_PC;
		break;
	case 0x1d:
		// DEC E
		_curr_reg->_DE.low(dec(_curr_reg->_DE.low()));
		++_PC;
		break;
	case 0x1e:
		// LD E, n
		_curr_reg->_DE.low(memory[++_PC]);
		// No flags affected
		++_PC;
		break;
	case 0x1f:
		// RRA
		_curr_reg->_AF.high(rra(_curr_reg->_AF.high()));
		++_PC;
		break;
	case 0x20:
		// JR NZ, d
		if (_curr_reg->_AF.low() & FLAG::Z)
			_PC += 2;
		else
			jr(memory);

		// No flags affected
		break;
	case 0x21:
		// LD HL, nn
		_curr_reg->_HL.value(&memory[++_PC]);
		// No flags affected
		_PC += 2;
		break;
	case 0x22:
		// LD (nn), HL
		poke_register_indirect(memory, _curr_reg->_HL);
		// No flags affected
		_PC += 2;
		break;
	case 0x23:
		// INC HL
		++_curr_reg->_HL.value();
		// No flags affected
		++_PC;
		break;
	case 0x24:
		// INC H
		_curr_reg->_HL.high(inc(_curr_reg->_HL.high()));
		++_PC;
		break;
	case 0x25:
		// DEC H
		_curr_reg->_HL.high(dec(_curr_reg->_HL.high()));
		++_PC;
		break;
	case 0x26:
		// LD H, n
		_curr_reg->_HL.high(memory[++_PC]);
		// No flags affected
		++_PC;
		break;
	case 0x27:
		// DAA
		++_PC;
		break;
	case 0x28:
		// JR Z, d
		if (_curr_reg->_AF.low() & FLAG::Z)
			jr(memory);
		else
			_PC += 2;

		// No flags affected
		break;
	case 0x29:
		// ADD HL, HL
		add(_curr_reg->_HL, _curr_reg->_HL);
		++_PC;
		break;
	case 0x2a:
		// LD HL, (nn)
		_curr_reg->_HL.value(peek_indirect(memory));
		// No flags affected
		_PC += 2;
		break;
	case 0x2b:
		// DEC HL
		--_curr_reg->_HL.value();
		// No flags affected
		++_PC;
		break;
	case 0x2c:
		// INC L
		_curr_reg->_HL.low(inc(_curr_reg->_HL.low()));
		++_PC;
		break;
	case 0x2d:
		// DEC L
		_curr_reg->_HL.low(dec(_curr_reg->_HL.low()));
		++_PC;
		break;
	case 0x2e:
		// LD L, n
		_curr_reg->_HL.low(memory[++_PC]);
		// No flags affected
		++_PC;
		break;
	case 0x2f:
		// CPL
		_curr_reg->_AF.high(~_curr_reg->_AF.high());
		_curr_reg->_AF.low(_curr_reg->_AF.low() | FLAG::H | FLAG::N);
		++_PC;
		break;
	case 0x30:
		// JR NC, d
		if (_curr_reg->_AF.low() & FLAG::C)
			_PC += 2;
		else
			jr(memory);

		// No flags affected
		break;
	case 0x31:
		// LD SP, nn
		_SP.value(&memory[++_PC]);
		// No flags affected
		_PC += 2;
		break;
	case 0x32:
		// LD (nn), A
		memory[peek_word(&memory[++_PC])] = _curr_reg->_AF.high();
		// No flags affected
		_PC += 2;
		break;
	case 0x33:
		// INC SP
		++_SP.value();
		// No flags affected
		++_PC;
		break;
	case 0x34:
		// INC (HL)
		memory[_curr_reg->_HL.value()] = inc(memory[_curr_reg->_HL.value()]);
		++_PC;
		break;
	case 0x35:
		// DEC (HL)
		memory[_curr_reg->_HL.value()] = dec(memory[_curr_reg->_HL.value()]);
		++_PC;
		break;
	case 0x36:
		// LD (HL), n
		memory[_curr_reg->_HL.value()] = memory[++_PC];
		// No flags affected
		++_PC;
		break;
	case 0x37:
		// SCF
		_curr_reg->_AF.low(_curr_reg->_AF.low() & ~(FLAG::H | FLAG::N) | FLAG::C);
		++_PC;
		break;
	case 0x38:
		// JR C, d
		if (_curr_reg->_AF.low() & FLAG::C)
			jr(memory);
		else
			_PC += 2;

		// No flags affected
		break;
	case 0x39:
		// ADD HL, SP
		add(_curr_reg->_HL, _SP);
		++_PC;
		break;
	case 0x3a:
		// LD A, (nn)
		_curr_reg->_AF.high(memory[peek_word(&memory[++_PC])]);
		// No flags affected
		_PC += 2;
		break;
	case 0x3b:
		// DEC SP
		--_SP.value();
		// No flags affected
		++_PC;
		break;
	case 0x3c:
		// INC A
		_curr_reg->_AF.high(inc(_curr_reg->_AF.high()));
		++_PC;
		break;
	case 0x3d:
		// DEC A
		_curr_reg->_AF.high(dec(_curr_reg->_AF.high()));
		++_PC;
		break;
	case 0x3e:
		// LD A, n
		_curr_reg->_AF.high(memory[++_PC]);
		// No flags affected
		++_PC;
		break;
	case 0x3f:
	{
		// CCF
		uint8_t flags = _curr_reg->_AF.low();

		if (flags & FLAG::C)
			flags |= FLAG::H;
		else
			flags &= ~FLAG::H;

		flags &= ~FLAG::N;
		flags ^= FLAG::C;
		_curr_reg->_AF.low(flags);
		++_PC;
		break;
	}
	case 0x40:
		// LD B, B
		// No action required
		// No flags affected
		++_PC;
		break;
	case 0x41:
		// LD B, C
		_curr_reg->_BC.high(_curr_reg->_BC.low());
		// No flags affected
		++_PC;
		break;
	case 0x42:
		// LD B, D
		_curr_reg->_BC.high(_curr_reg->_DE.high());
		// No flags affected
		++_PC;
		break;
	case 0x43:
		// LD B, E
		_curr_reg->_BC.high(_curr_reg->_DE.low());
		// No flags affected
		++_PC;
		break;
	case 0x44:
		// LD B, H
		_curr_reg->_BC.high(_curr_reg->_HL.high());
		// No flags affected
		++_PC;
		break;
	case 0x45:
		// LD B, L
		_curr_reg->_BC.high(_curr_reg->_HL.low());
		// No flags affected
		++_PC;
		break;
	case 0x46:
		// LD B, (HL)
		_curr_reg->_BC.high(memory[_curr_reg->_HL.value()]);
		// No flags affected
		++_PC;
		break;
	case 0x47:
		// LD B, A
		_curr_reg->_BC.high(_curr_reg->_AF.high());
		// No flags affected
		++_PC;
		break;
	case 0x48:
		// LD C, B
		_curr_reg->_BC.low(_curr_reg->_BC.high());
		// No flags affected
		++_PC;
		break;
	case 0x49:
		// LD C, C
		// No action required
		// No flags affected
		++_PC;
		break;
	case 0x4a:
		// LD C, D
		_curr_reg->_BC.low(_curr_reg->_DE.high());
		// No flags affected
		++_PC;
		break;
	case 0x4b:
		// LD C, E
		_curr_reg->_BC.low(_curr_reg->_DE.low());
		// No flags affected
		++_PC;
		break;
	case 0x4c:
		// LD C, H
		_curr_reg->_BC.low(_curr_reg->_HL.high());
		// No flags affected
		++_PC;
		break;
	case 0x4d:
		// LD C, L
		_curr_reg->_BC.low(_curr_reg->_HL.low());
		// No flags affected
		++_PC;
		break;
	case 0x4e:
		// LD C, (HL)
		_curr_reg->_BC.low(memory[_curr_reg->_HL.value()]);
		// No flags affected
		++_PC;
		break;
	case 0x4f:
		// LD C, A
		_curr_reg->_BC.low(_curr_reg->_AF.high());
		// No flags affected
		++_PC;
		break;
	case 0x50:
		// LD D, B
		_curr_reg->_DE.high(_curr_reg->_BC.high());
		// No flags affected
		++_PC;
		break;
	case 0x51:
		// LD D, C
		_curr_reg->_DE.high(_curr_reg->_BC.low());
		// No flags affected
		++_PC;
		break;
	case 0x52:
		// LD D, D
		// No action required
		// No flags affected
		++_PC;
		break;
	case 0x53:
		// LD D, E
		_curr_reg->_DE.high(_curr_reg->_DE.low());
		// No flags affected
		++_PC;
		break;
	case 0x54:
		// LD D, H
		_curr_reg->_DE.high(_curr_reg->_HL.high());
		// No flags affected
		++_PC;
		break;
	case 0x55:
		// LD D, L
		_curr_reg->_DE.high(_curr_reg->_HL.low());
		// No flags affected
		++_PC;
		break;
	case 0x56:
		// LD D, (HL)
		_curr_reg->_DE.high(memory[_curr_reg->_HL.value()]);
		// No flags affected
		++_PC;
		break;
	case 0x57:
		// LD D, A
		_curr_reg->_DE.high(_curr_reg->_AF.high());
		// No flags affected
		++_PC;
		break;
	case 0x58:
		// LD E, B
		_curr_reg->_DE.low(_curr_reg->_BC.high());
		// No flags affected
		++_PC;
		break;
	case 0x59:
		// LD E, C
		_curr_reg->_DE.low(_curr_reg->_BC.low());
		// No flags affected
		++_PC;
		break;
	case 0x5a:
		// LD E, D
		_curr_reg->_DE.low(_curr_reg->_DE.high());
		// No flags affected
		++_PC;
		break;
	case 0x5b:
		// LD E, E
		// No action required
		// No flags affected
		++_PC;
		break;
	case 0x5c:
		// LD E, H
		_curr_reg->_DE.low(_curr_reg->_HL.high());
		// No flags affected
		++_PC;
		break;
	case 0x5d:
		// LD E, L
		_curr_reg->_DE.low(_curr_reg->_HL.low());
		// No flags affected
		++_PC;
		break;
	case 0x5e:
		// LD E, (HL)
		_curr_reg->_DE.low(memory[_curr_reg->_HL.value()]);
		// No flags affected
		++_PC;
		break;
	case 0x5f:
		// LD E, A
		_curr_reg->_DE.low(_curr_reg->_AF.high());
		// No flags affected
		++_PC;
		break;
	case 0x60:
		// LD H, B
		_curr_reg->_HL.high(_curr_reg->_BC.high());
		// No flags affected
		++_PC;
		break;
	case 0x61:
		// LD H, C
		_curr_reg->_HL.high(_curr_reg->_BC.low());
		// No flags affected
		++_PC;
		break;
	case 0x62:
		// LD H, D
		_curr_reg->_HL.high(_curr_reg->_DE.high());
		// No flags affected
		++_PC;
		break;
	case 0x63:
		// LD H, E
		_curr_reg->_HL.high(_curr_reg->_DE.low());
		// No flags affected
		++_PC;
		break;
	case 0x64:
		// LD H, H
		// No action required
		// No flags affected
		++_PC;
		break;
	case 0x65:
		// LD H, L
		_curr_reg->_HL.high(_curr_reg->_HL.low());
		// No flags affected
		++_PC;
		break;
	case 0x66:
		// LD H, (HL)
		_curr_reg->_HL.high(memory[_curr_reg->_HL.value()]);
		// No flags affected
		++_PC;
		break;
	case 0x67:
		// LD H, A
		_curr_reg->_HL.high(_curr_reg->_AF.high());
		// No flags affected
		++_PC;
		break;
	case 0x68:
		// LD L, B
		_curr_reg->_HL.low(_curr_reg->_BC.high());
		// No flags affected
		++_PC;
		break;
	case 0x69:
		// LD L, C
		_curr_reg->_HL.low(_curr_reg->_BC.low());
		// No flags affected
		++_PC;
		break;
	case 0x6a:
		// LD L, D
		_curr_reg->_HL.low(_curr_reg->_DE.high());
		// No flags affected
		++_PC;
		break;
	case 0x6b:
		// LD L, E
		_curr_reg->_HL.low(_curr_reg->_DE.low());
		// No flags affected
		++_PC;
		break;
	case 0x6c:
		// LD L, H
		_curr_reg->_HL.low(_curr_reg->_HL.high());
		// No flags affected
		++_PC;
		break;
	case 0x6d:
		// LD L, L
		// No action required
		// No flags affected
		++_PC;
		break;
	case 0x6e:
		// LD L, (HL)
		_curr_reg->_HL.low(memory[_curr_reg->_HL.value()]);
		// No flags affected
		++_PC;
		break;
	case 0x6f:
		// LD L, A
		_curr_reg->_HL.low(_curr_reg->_AF.high());
		// No flags affected
		++_PC;
		break;
	case 0x70:
		// LD (HL), B
		memory[_curr_reg->_HL.value()] = _curr_reg->_BC.high();
		// No flags affected
		++_PC;
		break;
	case 0x71:
		// LD (HL), C
		memory[_curr_reg->_HL.value()] = _curr_reg->_BC.low();
		// No flags affected
		++_PC;
		break;
	case 0x72:
		// LD (HL), D
		memory[_curr_reg->_HL.value()] = _curr_reg->_DE.high();
		// No flags affected
		++_PC;
		break;
	case 0x73:
		// LD (HL), E
		memory[_curr_reg->_HL.value()] = _curr_reg->_DE.low();
		// No flags affected
		++_PC;
		break;
	case 0x74:
		// LD (HL), H
		memory[_curr_reg->_HL.value()] = _curr_reg->_HL.high();
		// No flags affected
		++_PC;
		break;
	case 0x75:
		// LD (HL), L
		memory[_curr_reg->_HL.value()] = _curr_reg->_HL.low();
		// No flags affected
		++_PC;
		break;
	case 0x76:
		// HALT
		break;
	case 0x77:
		// LD (HL), A
		memory[_curr_reg->_HL.value()] = _curr_reg->_AF.high();
		// No flags affected
		++_PC;
		break;
	case 0x78:
		// LD A, B
		_curr_reg->_AF.high(_curr_reg->_BC.high());
		// No flags affected
		++_PC;
		break;
	case 0x79:
		// LD A, C
		_curr_reg->_AF.high(_curr_reg->_BC.low());
		// No flags affected
		++_PC;
		break;
	case 0x7a:
		// LD A, D
		_curr_reg->_AF.high(_curr_reg->_DE.high());
		// No flags affected
		++_PC;
		break;
	case 0x7b:
		// LD A, E
		_curr_reg->_AF.high(_curr_reg->_DE.low());
		// No flags affected
		++_PC;
		break;
	case 0x7c:
		// LD A, H
		_curr_reg->_AF.high(_curr_reg->_HL.high());
		// No flags affected
		++_PC;
		break;
	case 0x7d:
		// LD A, L
		_curr_reg->_AF.high(_curr_reg->_HL.low());
		// No flags affected
		++_PC;
		break;
	case 0x7e:
		// LD A, (HL)
		_curr_reg->_AF.high(memory[_curr_reg->_HL.value()]);
		// No flags affected
		++_PC;
		break;
	case 0x7f:
		// LD A, A
		// No action required
		// No flags affected
		++_PC;
		break;
	case 0x80:
		// ADD A, B
		add_a(_curr_reg->_BC.high());
		++_PC;
		break;
	case 0x81:
		// ADD A, C
		add_a(_curr_reg->_BC.low());
		++_PC;
		break;
	case 0x82:
		// ADD A, D
		add_a(_curr_reg->_DE.high());
		++_PC;
		break;
	case 0x83:
		// ADD A, E
		add_a(_curr_reg->_DE.low());
		++_PC;
		break;
	case 0x84:
		// ADD A, H
		add_a(_curr_reg->_HL.high());
		++_PC;
		break;
	case 0x85:
		// ADD A, L
		add_a(_curr_reg->_HL.low());
		++_PC;
		break;
	case 0x86:
		// ADD A, (HL)
		add_a(memory[_curr_reg->_HL.value()]);
		++_PC;
		break;
	case 0x87:
		// ADD A, A
		add_a(_curr_reg->_AF.high());
		++_PC;
		break;
	case 0x88:
		// ADC A, B
		adc_a(_curr_reg->_BC.high());
		++_PC;
		break;
	case 0x89:
		// ADC A, C
		adc_a(_curr_reg->_BC.low());
		++_PC;
		break;
	case 0x8a:
		// ADC A, D
		adc_a(_curr_reg->_DE.high());
		++_PC;
		break;
	case 0x8b:
		// ADC A, E
		adc_a(_curr_reg->_DE.low());
		++_PC;
		break;
	case 0x8c:
		// ADC A, H
		adc_a(_curr_reg->_HL.high());
		++_PC;
		break;
	case 0x8d:
		// ADC A, L
		adc_a(_curr_reg->_HL.low());
		++_PC;
		break;
	case 0x8e:
		// ADC A, (HL)
		adc_a(memory[_curr_reg->_HL.value()]);
		++_PC;
		break;
	case 0x8f:
		// ADC A, A
		adc_a(_curr_reg->_AF.high());
		++_PC;
		break;
	case 0x90:
		// SUB B
		sub_a(_curr_reg->_BC.high());
		++_PC;
		break;
	case 0x91:
		// SUB C
		sub_a(_curr_reg->_BC.low());
		++_PC;
		break;
	case 0x92:
		// SUB D
		sub_a(_curr_reg->_DE.high());
		++_PC;
		break;
	case 0x93:
		// SUB E
		sub_a(_curr_reg->_DE.low());
		++_PC;
		break;
	case 0x94:
		// SUB H
		sub_a(_curr_reg->_HL.high());
		++_PC;
		break;
	case 0x95:
		// SUB L
		sub_a(_curr_reg->_HL.low());
		++_PC;
		break;
	case 0x96:
		// SUB (HL)
		sub_a(memory[_curr_reg->_HL.value()]);
		++_PC;
		break;
	case 0x97:
		// SUB A
		sub_a(_curr_reg->_AF.high());
		++_PC;
		break;
	case 0x98:
		// SBC A, B
		sbc_a(_curr_reg->_BC.high());
		++_PC;
		break;
	case 0x99:
		// SBC A, C
		sbc_a(_curr_reg->_BC.low());
		++_PC;
		break;
	case 0x9a:
		// SBC A, D
		sbc_a(_curr_reg->_DE.high());
		++_PC;
		break;
	case 0x9b:
		// SBC A, E
		sbc_a(_curr_reg->_DE.low());
		++_PC;
		break;
	case 0x9c:
		// SBC A, H
		sbc_a(_curr_reg->_HL.high());
		++_PC;
		break;
	case 0x9d:
		// SBC A, L
		sbc_a(_curr_reg->_HL.low());
		++_PC;
		break;
	case 0x9e:
		// SBC A, (HL)
		sbc_a(memory[_curr_reg->_HL.value()]);
		++_PC;
		break;
	case 0x9f:
		// SBC A, A
		sbc_a(_curr_reg->_AF.high());
		++_PC;
		break;
	case 0xa0:
		// AND B
		and_a(_curr_reg->_BC.high());
		++_PC;
		break;
	case 0xa1:
		// AND C
		and_a(_curr_reg->_BC.low());
		++_PC;
		break;
	case 0xa2:
		// AND D
		and_a(_curr_reg->_DE.high());
		++_PC;
		break;
	case 0xa3:
		// AND E
		and_a(_curr_reg->_DE.low());
		++_PC;
		break;
	case 0xa4:
		// AND H
		and_a(_curr_reg->_HL.high());
		++_PC;
		break;
	case 0xa5:
		// AND L
		and_a(_curr_reg->_HL.low());
		++_PC;
		break;
	case 0xa6:
		// AND (HL)
		and_a(memory[_curr_reg->_HL.value()]);
		++_PC;
		break;
	case 0xa7:
		// AND A
		and_a(_curr_reg->_AF.high());
		++_PC;
		break;
	case 0xa8:
		// XOR B
		xor_a(_curr_reg->_BC.high());
		++_PC;
		break;
	case 0xa9:
		// XOR C
		xor_a(_curr_reg->_BC.low());
		++_PC;
		break;
	case 0xaa:
		// XOR D
		xor_a(_curr_reg->_DE.high());
		++_PC;
		break;
	case 0xab:
		// XOR E
		xor_a(_curr_reg->_DE.low());
		++_PC;
		break;
	case 0xac:
		// XOR H
		xor_a(_curr_reg->_HL.high());
		++_PC;
		break;
	case 0xad:
		// XOR L
		xor_a(_curr_reg->_HL.low());
		++_PC;
		break;
	case 0xae:
		// XOR (HL)
		xor_a(memory[_curr_reg->_HL.value()]);
		++_PC;
		break;
	case 0xaf:
		// XOR A
		xor_a(_curr_reg->_AF.high());
		++_PC;
		break;
	case 0xb0:
		// OR B
		or_a(_curr_reg->_BC.high());
		++_PC;
		break;
	case 0xb1:
		// OR C
		or_a(_curr_reg->_BC.low());
		++_PC;
		break;
	case 0xb2:
		// OR D
		or_a(_curr_reg->_DE.high());
		++_PC;
		break;
	case 0xb3:
		// OR E
		or_a(_curr_reg->_DE.low());
		++_PC;
		break;
	case 0xb4:
		// OR H
		or_a(_curr_reg->_HL.high());
		++_PC;
		break;
	case 0xb5:
		// OR L
		or_a(_curr_reg->_HL.low());
		++_PC;
		break;
	case 0xb6:
		// OR (HL)
		or_a(memory[_curr_reg->_HL.value()]);
		++_PC;
		break;
	case 0xb7:
		// OR A
		or_a(_curr_reg->_AF.high());
		++_PC;
		break;
	case 0xb8:
		// CP B
		cp_a(_curr_reg->_BC.high());
		++_PC;
		break;
	case 0xb9:
		// CP C
		cp_a(_curr_reg->_BC.low());
		++_PC;
		break;
	case 0xba:
		// CP D
		cp_a(_curr_reg->_DE.high());
		++_PC;
		break;
	case 0xbb:
		// CP E
		cp_a(_curr_reg->_DE.low());
		++_PC;
		break;
	case 0xbc:
		// CP H
		cp_a(_curr_reg->_HL.high());
		++_PC;
		break;
	case 0xbd:
		// CP L
		cp_a(_curr_reg->_HL.low());
		++_PC;
		break;
	case 0xbe:
		// CP (HL)
		cp_a(memory[_curr_reg->_HL.value()]);
		++_PC;
		break;
	case 0xbf:
		// CP A
		cp_a(_curr_reg->_AF.high());
		++_PC;
		break;
	case 0xc0:
		// RET NZ
		if (_curr_reg->_AF.low() & FLAG::Z)
			++_PC;
		else
			ret(memory);

		// No flags affected
		break;
	case 0xc1:
		// POP BC
		pop(memory, _curr_reg->_BC);
		// No flags affected
		++_PC;
		break;
	case 0xc2:
		// JP NZ, nn
		if (_curr_reg->_AF.low() & FLAG::Z)
			_PC += 3;
		else
			jp(memory);

		// No flags affected
		break;
	case 0xc3:
		// JP nn
		jp(memory);
		// No flags affected
		break;
	case 0xc4:
		// CALL NZ, nn
		if (_curr_reg->_AF.low() & FLAG::Z)
			_PC += 3;
		else
			call(memory);

		// No flags affected
		break;
	case 0xc5:
		// PUSH BC
		push(memory, _curr_reg->_BC);
		// No flags affected
		++_PC;
		break;
	case 0xc6:
		// ADD A, n
		add_a(memory[++_PC]);
		++_PC;
		break;
	case 0xc7:
		// RST 00H
		++_PC;
		break;
	case 0xc8:
		// RET Z
		if (_curr_reg->_AF.low() & FLAG::Z)
			ret(memory);
		else
			++_PC;

		// No flags affected
		break;
	case 0xc9:
		// RET
		ret(memory);
		// No flags affected
		break;
	case 0xca:
		// JP Z, nn
		if (_curr_reg->_AF.low() & FLAG::Z)
			jp(memory);
		else
			_PC += 3;

		// No flags affected
		break;
	case 0xcb:
		step_bits(memory);
		break;
	case 0xcc:
		// CALL Z, nn
		if (_curr_reg->_AF.low() & FLAG::Z)
			call(memory);
		else
			_PC += 3;

		// No flags affected
		break;
	case 0xcd:
		// CALL nn
		call(memory);
		// No flags affected
		break;
	case 0xce:
		// ADC A, n
		adc_a(memory[++_PC]);
		++_PC;
		break;
	case 0xcf:
		// RST 08H
		++_PC;
		break;
	case 0xd0:
		// RET NC
		if (_curr_reg->_AF.low() & FLAG::C)
			++_PC;
		else
			ret(memory);

		// No flags affected
		break;
	case 0xd1:
		// POP DE
		pop(memory, _curr_reg->_DE);
		// No flags affected
		++_PC;
		break;
	case 0xd2:
		// JP NC, nn
		if (_curr_reg->_AF.low() & FLAG::C)
			_PC += 3;
		else
			jp(memory);

		// No flags affected
		break;
	case 0xd3:
		// OUT (n), A
		++_PC;

		++_PC;
		break;
	case 0xd4:
		// CALL NC, nn
		if (_curr_reg->_AF.low() & FLAG::C)
			_PC += 3;
		else
			call(memory);

		// No flags affected
		break;
	case 0xd5:
		// PUSH DE
		push(memory, _curr_reg->_DE);
		// No flags affected
		++_PC;
		break;
	case 0xd6:
		// SUB n
		sub_a(memory[++_PC]);
		++_PC;
		break;
	case 0xd7:
		// RST 10H
		++_PC;
		break;
	case 0xd8:
		// RET C
		if (_curr_reg->_AF.low() & FLAG::C)
			ret(memory);
		else
			++_PC;

		// No flags affected
		break;
	case 0xd9:
		// EXX
		if (_curr_reg == &_registers)
			_curr_reg = &_registers_prime;
		else
			_curr_reg = &_registers;

		++_PC;
		break;
	case 0xda:
		// JP C, nn
		if (_curr_reg->_AF.low() & FLAG::C)
			jp(memory);
		else
			_PC += 3;

		// No flags affected
		break;
	case 0xdb:
		// IN A, (n)
		++_PC;

		++_PC;
		break;
	case 0xdc:
		// CALL C, nn
		if (_curr_reg->_AF.low() & FLAG::C)
			call(memory);
		else
			_PC += 3;

		// No flags affected
		break;
	case 0xdd:
		step_IX_IY(memory, 'X');
		break;
	case 0xde:
		// SBC A, n
		sbc_a(memory[++_PC]);
		++_PC;
		break;
	case 0xdf:
		// RST 18H
		++_PC;
		break;
	case 0xe0:
		// RET PO
		if (_curr_reg->_AF.low() & FLAG::PV)
			++_PC;
		else
			ret(memory);

		// No flags affected
		break;
	case 0xe1:
		// POP HL
		pop(memory, _curr_reg->_HL);
		// No flags affected
		++_PC;
		break;
	case 0xe2:
		// JP PO, nn
		if (_curr_reg->_AF.low() & FLAG::PV)
			_PC += 3;
		else
			jp(memory);

		// No flags affected
		break;
	case 0xe3:
	{
		// EX (SP), HL
		uint16_t val = peek_word(&memory[_SP.value()]);

		std::swap(val, _curr_reg->_HL.value());
		poke_word(&memory[_SP.value()], val);
		// No flags affected
		++_PC;
		break;
	}
	case 0xe4:
		// CALL PO, nn
		if (_curr_reg->_AF.low() & FLAG::PV)
			_PC += 3;
		else
			call(memory);

		// No flags affected
		break;
	case 0xe5:
		// PUSH HL
		push(memory, _curr_reg->_HL);
		// No flags affected
		++_PC;
		break;
	case 0xe6:
		// AND n
		and_a(memory[++_PC]);
		++_PC;
		break;
	case 0xe7:
		// RST 20H
		++_PC;
		break;
	case 0xe8:
		// RET PE
		if (_curr_reg->_AF.low() & FLAG::PV)
			ret(memory);
		else
			++_PC;

		// No flags affected
		break;
	case 0xe9:
		// JP (HL)
		jp_register(memory, _curr_reg->_HL);
		// No flags affected
		break;
	case 0xea:
		// JP PE, nn
		if (_curr_reg->_AF.low() & FLAG::PV)
			jp(memory);
		else
			_PC += 3;

		// No flags affected
		break;
	case 0xeb:
		// EX DE, HL
		std::swap(_curr_reg->_DE.value(), _curr_reg->_HL.value());
		// No flags affected
		++_PC;
		break;
	case 0xec:
		// CALL PE, nn
		if (_curr_reg->_AF.low() & FLAG::PV)
			call(memory);
		else
			_PC += 3;

		// No flags affected
		break;
	case 0xed:
		step_ext(memory);
		break;
	case 0xee:
		// XOR n
		xor_a(memory[++_PC]);
		++_PC;
		break;
	case 0xef:
		// RST 28H
		++_PC;
		break;
	case 0xf0:
		// RET P
		if (_curr_reg->_AF.low() & FLAG::S)
			++_PC;
		else
			ret(memory);

		// No flags affected
		break;
	case 0xf1:
		// POP AF
		pop(memory, _curr_reg->_AF);
		// No flags affected
		++_PC;
		break;
	case 0xf2:
		// JP P, nn
		if (_curr_reg->_AF.low() & FLAG::S)
			++_PC;
		else
			jp(memory);

		// No flags affected
		break;
	case 0xf3:
		// DI
		++_PC;
		break;
	case 0xf4:
		// CALL P, nn
		if (_curr_reg->_AF.low() & FLAG::S)
			++_PC;
		else
			call(memory);

		// No flags affected
		break;
	case 0xf5:
		// PUSH AF
		push(memory, _curr_reg->_AF);
		// No flags affected
		++_PC;
		break;
	case 0xf6:
		// OR n
		or_a(memory[++_PC]);
		++_PC;
		break;
	case 0xf7:
		// RST 30H
		++_PC;
		break;
	case 0xf8:
		// RET M
		++_PC;
		break;
	case 0xf9:
		// LD SP, HL
		_SP.value() = _curr_reg->_HL.value();
		// No flags affected
		++_PC;
		break;
	case 0xfa:
		// JP M, nn
		if (_curr_reg->_AF.low() & FLAG::S)
			jp(memory);
		else
			++_PC;

		// No flags affected
		break;
	case 0xfb:
		// EI
		++_PC;
		break;
	case 0xfc:
		// CALL M, nn
		if (_curr_reg->_AF.low() & FLAG::S)
			call(memory);
		else
			_PC += 3;

		// No flags affected
		break;
	case 0xfd:
		step_IX_IY(memory, 'Y');
		break;
	case 0xfe:
		// CP n
		cp_a(memory[++_PC]);
		++_PC;
		break;
	case 0xff:
		// RST 38H
		++_PC;
		break;
	}
}

void z80::step_IX_IY(uint8_t* memory, const char xy)
{
	switch (memory[++_PC])
	{
	case 0x09:
		// ADD I[XY], BC
		add(ixy(xy), _curr_reg->_BC);
		++_PC;
		break;
	case 0x19:
		// ADD I[XY], DE
		add(ixy(xy), _curr_reg->_DE);
		++_PC;
		break;
	case 0x21:
		// LD I[XY], nn
		ixy_value(xy, &memory[++_PC]);
		// No flags affected
		_PC += 2;
		break;
	case 0x22:
		// LD (nn), I[XY]
		++_PC;
		poke_register_indirect(memory, ixy(xy));
		// No flags affected
		_PC += 2;
		break;
	case 0x23:
		// INC I[XY]
		++ixy_value(xy);
		// No flags affected
		++_PC;
		break;
	case 0x24:
		// INC I[XY]H
		ixy_high(xy, inc(ixy_high(xy)));
		++_PC;
		break;
	case 0x25:
		// DEC I[XY]H;
		ixy_high(xy, dec(ixy_high(xy)));
		++_PC;
		break;
	case 0x26:
		// LD I[XY]H, d
		ixy_high(xy, memory[++_PC]);
		// No flags affected
		++_PC;
		break;
	case 0x29:
		// ADD I[XY], I[XY]
		add(ixy(xy), ixy(xy));
		++_PC;
		break;
	case 0x2a:
		// LD I[XY], (dd)
		ixy_value(xy, peek_indirect(memory));
		// No flags affected
		_PC += 2;
		break;
	case 0x2b:
		// DEC I[XY]
		--ixy_value(xy);
		// No flags affected
		++_PC;
		break;
	case 0x2c:
		// INC I[XY]L
		ixy_low(xy, inc(ixy_low(xy)));
		++_PC;
		break;
	case 0x2d:
		// DEC I[XY]L
		ixy_low(xy, dec(ixy_low(xy)));
		++_PC;
		break;
	case 0x2e:
		// LD I[XY]L, d
		ixy_low(xy, memory[++_PC]);
		// No flags affected
		++_PC;
		break;
	case 0x34:
	{
		// INC (I[XY] + d)
		uint16_t idx = ixy_value(xy);

		idx += static_cast<int8_t>(memory[++_PC]);
		memory[idx] = inc(memory[idx]);
		++_PC;
		break;
	}
	case 0x35:
	{
		// DEC (I[XY] + d)
		uint16_t idx = ixy_value(xy);

		idx += static_cast<int8_t>(memory[++_PC]);
		memory[idx] = dec(memory[idx]);
		++_PC;
		break;
	}
	case 0x36:
	{
		// LD (I[XY] + d), n
		uint16_t idx = ixy_value(xy);

		idx += static_cast<int8_t>(memory[++_PC]);
		memory[idx] = memory[++_PC];
		// No flags affected
		++_PC;
		break;
	}
	case 0x39:
		// ADD I[XY], SP
		add(ixy(xy), _SP);
		++_PC;
		break;
	case 0x44:
		// LD B, I[XY]H
		_curr_reg->_BC.high(ixy_high(xy));
		// No flags affected
		++_PC;
		break;
	case 0x45:
		// LD B, I[XY]L;
		_curr_reg->_BC.high(ixy_low(xy));
		// No flags affected
		++_PC;
		break;
	case 0x46:
	{
		// LD B, (I[XY] + d)
		uint16_t idx = ixy_value(xy);

		idx += static_cast<int8_t>(memory[++_PC]);
		_curr_reg->_BC.high(memory[idx]);
		// No flags affected
		++_PC;
		break;
	}
	case 0x4c:
		// LD C, I[XY]H
		_curr_reg->_BC.low(ixy_high(xy));
		// No flags affected
		++_PC;
		break;
	case 0x4d:
		// LD C, I[XY]L
		_curr_reg->_BC.low(ixy_low(xy));
		// No flags affected
		++_PC;
		break;
	case 0x4e:
	{
		// LD C, (I[XY] + d)
		uint16_t idx = ixy_value(xy);

		idx += static_cast<int8_t>(memory[++_PC]);
		_curr_reg->_BC.low(memory[idx]);
		// No flags affected
		++_PC;
		break;
	}
	case 0x54:
		// LD D, I[XY]H
		_curr_reg->_DE.high(ixy_high(xy));
		// No flags affected
		++_PC;
		break;
	case 0x55:
		// LD D, I[XY]L
		_curr_reg->_DE.high(ixy_low(xy));
		// No flags affected
		++_PC;
		break;
	case 0x56:
	{
		// LD D, (I[XY] + d)
		uint16_t idx = ixy_value(xy);

		idx += static_cast<int8_t>(memory[++_PC]);
		_curr_reg->_DE.high(memory[idx]);
		// No flags affected
		++_PC;
		break;
	}
	case 0x5c:
		// LD E, I[XY]H
		_curr_reg->_DE.low(ixy_high(xy));
		// No flags affected
		++_PC;
		break;
	case 0x5d:
		// LD E, I[XY]L
		_curr_reg->_DE.low(ixy_low(xy));
		// No flags affected
		++_PC;
		break;
	case 0x5e:
	{
		// LD E, (I[XY] + d)
		uint16_t idx = ixy_value(xy);

		idx += static_cast<int8_t>(memory[++_PC]);
		_curr_reg->_DE.low(memory[idx]);
		// No flags affected
		++_PC;
		break;
	}
	case 0x60:
		// LD I[XY]H, B
		ixy_high(xy, _curr_reg->_BC.high());
		// No flags affected
		++_PC;
		break;
	case 0x61:
		// LD I[XY]H, C
		ixy_high(xy, _curr_reg->_BC.low());
		// No flags affected
		++_PC;
		break;
	case 0x62:
		// LD I[XY]H, D
		ixy_high(xy, _curr_reg->_DE.high());
		// No flags affected
		++_PC;
		break;
	case 0x63:
		// LD I[XY]H, E
		ixy_high(xy, _curr_reg->_DE.low());
		// No flags affected
		++_PC;
		break;
	case 0x64:
		// LD I[XY]H, I[XY]H
		// Nothing to do
		// No flags affected
		++_PC;
		break;
	case 0x65:
		// LD I[XY]H, I[XY]L
		ixy_high(xy, ixy_low(xy));
		// No flags affected
		++_PC;
		break;
	case 0x66:
	{
		// LD H, (I[XY] + d)
		uint16_t idx = ixy_value(xy);

		idx += static_cast<int8_t>(memory[++_PC]);
		_curr_reg->_HL.high(memory[idx]);
		// No flags affected
		++_PC;
		break;
	}
	case 0x67:
		// LD I[XY]H, A
		ixy_high(xy, _curr_reg->_AF.high());
		// No flags affected
		++_PC;
		break;
	case 0x68:
		// LD I[XY]L, B
		ixy_low(xy, _curr_reg->_BC.high());
		// No flags affected
		++_PC;
		break;
	case 0x69:
		// LD I[XY]L, C
		ixy_low(xy, _curr_reg->_BC.low());
		// No flags affected
		++_PC;
		break;
	case 0x6a:
		// LD I[XY]L, D
		ixy_low(xy, _curr_reg->_DE.high());
		// No flags affected
		++_PC;
		break;
	case 0x6b:
		// LD I[XY]L, E
		ixy_low(xy, _curr_reg->_DE.low());
		// No flags affected
		++_PC;
		break;
	case 0x6c:
		// LD I[XY]L, I[XY]H
		ixy_low(xy, ixy_high(xy));
		// No flags affected
		++_PC;
		break;
	case 0x6d:
		// LD I[XY]L, I[XY]L
		// Nothing to do
		// No flags affected
		++_PC;
		break;
	case 0x6e:
	{
		// LD L, (I[XY] + d)
		uint16_t idx = ixy_value(xy);

		idx += static_cast<int8_t>(memory[++_PC]);
		_curr_reg->_HL.low(memory[idx]);
		// No flags affected
		++_PC;
		break;
	}
	case 0x6f:
		// LD I[XY]L, A
		ixy_low(xy, _curr_reg->_AF.high());
		// No flags affected
		++_PC;
		break;
	case 0x70:
	{
		// LD (I[XY] + d), B
		uint16_t idx = ixy_value(xy);

		idx += static_cast<int8_t>(memory[++_PC]);
		memory[idx] = _curr_reg->_BC.high();
		// No flags affected
		++_PC;
		break;
	}
	case 0x71:
	{
		// LD (I[XY] + d), C
		uint16_t idx = ixy_value(xy);

		idx += static_cast<int8_t>(memory[++_PC]);
		memory[idx] = _curr_reg->_BC.low();
		// No flags affected
		++_PC;
		break;
	}
	case 0x72:
	{
		// LD (I[XY] + d), D
		uint16_t idx = ixy_value(xy);

		idx += static_cast<int8_t>(memory[++_PC]);
		memory[idx] = _curr_reg->_DE.high();
		// No flags affected
		++_PC;
		break;
	}
	case 0x73:
	{
		// LD (I[XY] + d), E
		uint16_t idx = ixy_value(xy);

		idx += static_cast<int8_t>(memory[++_PC]);
		memory[idx] = _curr_reg->_DE.low();
		// No flags affected
		++_PC;
		break;
	}
	case 0x74:
	{
		// LD (I[XY] + d), H
		uint16_t idx = ixy_value(xy);

		idx += static_cast<int8_t>(memory[++_PC]);
		memory[idx] = _curr_reg->_HL.high();
		// No flags affected
		++_PC;
		break;
	}
	case 0x75:
	{
		// LD (I[XY] + d), L
		uint16_t idx = ixy_value(xy);

		idx += static_cast<int8_t>(memory[++_PC]);
		memory[idx] = _curr_reg->_HL.low();
		// No flags affected
		++_PC;
		break;
	}
	case 0x77:
	{
		// LD (I[XY] + d), A
		uint16_t idx = ixy_value(xy);

		idx += static_cast<int8_t>(memory[++_PC]);
		memory[idx] = _curr_reg->_AF.high();
		// No flags affected
		++_PC;
		break;
	}
	case 0x7c:
		// LD A, I[XY]H;
		_curr_reg->_AF.high(ixy_high(xy));
		// No flags affected
		++_PC;
		break;
	case 0x7d:
		// LD A, I[XY]L
		_curr_reg->_AF.high(ixy_low(xy));
		// No flags affected
		++_PC;
		break;
	case 0x7e:
	{
		// LD A, (I[XY] + d)
		uint16_t idx = ixy_value(xy);

		idx += static_cast<int8_t>(memory[++_PC]);
		_curr_reg->_AF.high(memory[idx]);
		// No flags affected
		++_PC;
		break;
	}
	case 0x84:
		// ADD A, I[XY]H
		add_a(ixy_high(xy));
		++_PC;
		break;
	case 0x85:
		// ADD A, I[XY]L
		add_a(ixy_low(xy));
		++_PC;
		break;
	case 0x86:
	{
		// ADD A, (I[XY] + d)
		uint16_t idx = ixy_value(xy);

		idx += static_cast<int8_t>(memory[++_PC]);
		add_a(memory[idx]);
		++_PC;
		break;
	}
	case 0x8c:
		// ADC A, I[XY]H
		adc_a(ixy_high(xy));
		++_PC;
		break;
	case 0x8d:
		// ADC A, I[XY]L
		adc_a(ixy_low(xy));
		++_PC;
		break;
	case 0x8e:
	{
		// ADC A, (I[XY] + d)
		uint16_t idx = ixy_value(xy);

		idx += static_cast<int8_t>(memory[++_PC]);
		adc_a(memory[idx]);
		++_PC;
		break;
	}
	case 0x94:
		// SUB I[XY]H
		sub_a(ixy_high(xy));
		++_PC;
		break;
	case 0x95:
		// SUB I[XY]L
		sub_a(ixy_low(xy));
		++_PC;
		break;
	case 0x96:
	{
		// SUB (I[XY] + d)
		uint16_t idx = ixy_value(xy);

		idx += static_cast<int8_t>(memory[++_PC]);
		sub_a(memory[idx]);
		++_PC;
		break;
	}
	case 0x9c:
		// SBC A, I[XY]H
		sbc_a(ixy_high(xy));
		++_PC;
		break;
	case 0x9d:
		// SBC A, I[XY]L
		sbc_a(ixy_low(xy));
		++_PC;
		break;
	case 0x9e:
	{
		// SBC A, (I[XY] + d)
		uint16_t idx = ixy_value(xy);

		idx += static_cast<int8_t>(memory[++_PC]);
		sbc_a(memory[idx]);
		++_PC;
		break;
	}
	case 0xa4:
		// AND I[XY]H
		and_a(ixy_high(xy));
		++_PC;
		break;
	case 0xa5:
		// AND I[XY]L
		and_a(ixy_low(xy));
		++_PC;
		break;
	case 0xa6:
	{
		// AND (I[XY] + d)
		uint16_t idx = ixy_value(xy);

		idx += static_cast<int8_t>(memory[++_PC]);
		and_a(memory[idx]);
		++_PC;
		break;
	}
	case 0xac:
		// XOR I[XY]H
		xor_a(ixy_high(xy));
		++_PC;
		break;
	case 0xad:
		// XOR I[XY]L
		xor_a(ixy_low(xy));
		++_PC;
		break;
	case 0xae:
	{
		// XOR (I[XY] + d)
		uint16_t idx = ixy_value(xy);

		idx += static_cast<int8_t>(memory[++_PC]);
		xor_a(memory[idx]);
		++_PC;
		break;
	}
	case 0xb4:
		// OR I[XY]H
		or_a(ixy_high(xy));
		++_PC;
		break;
	case 0xb5:
		// OR I[XY]L
		or_a(ixy_low(xy));
		++_PC;
		break;
	case 0xb6:
	{
		// OR (I[XY] + d)
		uint16_t idx = ixy_value(xy);

		idx += static_cast<int8_t>(memory[++_PC]);
		or_a(memory[idx]);
		++_PC;
		break;
	}
	case 0xbc:
		// CP I[XY]H
		cp_a(ixy_high(xy));
		++_PC;
		break;
	case 0xbd:
		// CP I[XY]L
		cp_a(ixy_low(xy));
		++_PC;
		break;
	case 0xbe:
	{
		// CP (I[XY] + d)
		uint16_t idx = ixy_value(xy);

		idx += static_cast<int8_t>(memory[++_PC]);
		cp_a(memory[idx]);
		++_PC;
		break;
	}
	case 0xcb:
		step_IX_IY_bits(memory, xy);
		break;
	case 0xe1:
		// POP I[XY]
		pop(memory, ixy(xy));
		// No flags affected
		++_PC;
		break;
	case 0xe3:
	{
		// EX (SP), I[XY]
		uint16_t val = peek_word(&memory[_SP.value()]);

		std::swap(val, ixy_value(xy));
		poke_word(&memory[_SP.value()], val);
		// No flags affected
		++_PC;
		break;
	}
	case 0xe5:
		// PUSH I[XY]
		push(memory, ixy(xy));
		// No flags affected
		++_PC;
		break;
	case 0xe9:
		// JP (I[XY])
		jp_register(memory, ixy(xy));
		// No flags affected
		break;
	case 0xf9:
		// LD SP, I[XY]
		_SP.value(ixy_value(xy));
		// No flags affected
		++_PC;
		break;
	default:
		break;
	}
}

void z80::step_IX_IY_bits(uint8_t* memory, const char xy)
{
	const uint8_t data = memory[++_PC];
	const uint16_t idx = memory[ixy_value(xy) + static_cast<int8_t>(data)];

	switch (memory[++_PC])
	{
	case 0x00:
		// RLC (I[XY] + data), B
		memory[idx] = rlc(memory[idx]);
		_curr_reg->_BC.high(memory[idx]);
		break;
	case 0x01:
		// RLC (I[XY] + data), C
		memory[idx] = rlc(memory[idx]);
		_curr_reg->_BC.low(memory[idx]);
		break;
	case 0x02:
		// RLC (I[XY] + data), D
		memory[idx] = rlc(memory[idx]);
		_curr_reg->_DE.high(memory[idx]);
		break;
	case 0x03:
		// RLC (I[XY] + data), E
		memory[idx] = rlc(memory[idx]);
		_curr_reg->_DE.low(memory[idx]);
		break;
	case 0x04:
		// RLC (I[XY] + data), H
		memory[idx] = rlc(memory[idx]);
		_curr_reg->_HL.high(memory[idx]);
		break;
	case 0x05:
		// RLC (I[XY] + data), L
		memory[idx] = rlc(memory[idx]);
		_curr_reg->_HL.low(memory[idx]);
		break;
	case 0x06:
		// RLC (I[XY] + data)
		memory[idx] = rlc(memory[idx]);
		break;
	case 0x07:
		// RLC (I[XY] + data), A
		memory[idx] = rlc(memory[idx]);
		_curr_reg->_AF.high(memory[idx]);
		break;
	case 0x08:
		// RRC (I[XY] + data), B
		memory[idx] = rrc(memory[idx]);
		_curr_reg->_BC.high(memory[idx]);
		break;
	case 0x09:
		// RRC (I[XY] + data), C
		memory[idx] = rrc(memory[idx]);
		_curr_reg->_BC.low(memory[idx]);
		break;
	case 0x0a:
		// RRC (I[XY] + data), D
		memory[idx] = rrc(memory[idx]);
		_curr_reg->_DE.high(memory[idx]);
		break;
	case 0x0b:
		// RRC (I[XY] + data), E
		memory[idx] = rrc(memory[idx]);
		_curr_reg->_DE.low(memory[idx]);
		break;
	case 0x0c:
		// RRC (I[XY] + data), H
		memory[idx] = rrc(memory[idx]);
		_curr_reg->_HL.high(memory[idx]);
		break;
	case 0x0d:
		// RRC (I[XY] + data), L
		memory[idx] = rrc(memory[idx]);
		_curr_reg->_HL.low(memory[idx]);
		break;
	case 0x0e:
		// RRC (I[XY] + data)
		memory[idx] = rrc(memory[idx]);
		break;
	case 0x0f:
		// RRC (I[XY] + data), A
		memory[idx] = rrc(memory[idx]);
		_curr_reg->_AF.high(memory[idx]);
		break;
	case 0x10:
		// RL (I[XY] + data), B
		memory[idx] = rl(memory[idx]);
		_curr_reg->_BC.high(memory[idx]);
		break;
	case 0x11:
		// RL (I[XY] + data), C
		memory[idx] = rl(memory[idx]);
		_curr_reg->_BC.low(memory[idx]);
		break;
	case 0x12:
		// RL (I[XY] + data), D
		memory[idx] = rl(memory[idx]);
		_curr_reg->_DE.high(memory[idx]);
		break;
	case 0x13:
		// RL (I[XY] + data), E
		memory[idx] = rl(memory[idx]);
		_curr_reg->_DE.low(memory[idx]);
		break;
	case 0x14:
		// RL (I[XY] + data), H
		memory[idx] = rl(memory[idx]);
		_curr_reg->_HL.high(memory[idx]);
		break;
	case 0x15:
		// RL (I[XY] + data), L
		memory[idx] = rl(memory[idx]);
		_curr_reg->_HL.low(memory[idx]);
		break;
	case 0x16:
		// RL (I[XY] + data)
		memory[idx] = rl(memory[idx]);
		break;
	case 0x17:
		// RL (I[XY] + data), A
		memory[idx] = rl(memory[idx]);
		_curr_reg->_AF.high(memory[idx]);
		break;
	case 0x18:
		// RR (I[XY] + data), B
		memory[idx] = rr(memory[idx]);
		_curr_reg->_BC.high(memory[idx]);
		break;
	case 0x19:
		// RR (I[XY] + data), C
		memory[idx] = rr(memory[idx]);
		_curr_reg->_BC.low(memory[idx]);
		break;
	case 0x1a:
		// RR (I[XY] + data), D
		memory[idx] = rr(memory[idx]);
		_curr_reg->_DE.high(memory[idx]);
		break;
	case 0x1b:
		// RR (I[XY] + data), E
		memory[idx] = rr(memory[idx]);
		_curr_reg->_DE.low(memory[idx]);
		break;
	case 0x1c:
		// RR (I[XY] + data), H
		memory[idx] = rr(memory[idx]);
		_curr_reg->_HL.high(memory[idx]);
		break;
	case 0x1d:
		// RR (I[XY] + data), L
		memory[idx] = rr(memory[idx]);
		_curr_reg->_HL.low(memory[idx]);
		break;
	case 0x1e:
		// RR (I[XY] + data)
		memory[idx] = rr(memory[idx]);
		break;
	case 0x1f:
		// RR (I[XY] + data), A
		memory[idx] = rr(memory[idx]);
		_curr_reg->_AF.high(memory[idx]);
		break;
	case 0x20:
		// SLA (I[XY] + data), B
		memory[idx] = sla(memory[idx]);
		_curr_reg->_BC.high(memory[idx]);
		break;
	case 0x21:
		// SLA (I[XY] + data), C
		memory[idx] = sla(memory[idx]);
		_curr_reg->_BC.low(memory[idx]);
		break;
	case 0x22:
		// SLA (I[XY] + data), D
		memory[idx] = sla(memory[idx]);
		_curr_reg->_DE.high(memory[idx]);
		break;
	case 0x23:
		// SLA (I[XY] + data), E
		memory[idx] = sla(memory[idx]);
		_curr_reg->_DE.low(memory[idx]);
		break;
	case 0x24:
		// SLA (I[XY] + data), H
		memory[idx] = sla(memory[idx]);
		_curr_reg->_HL.high(memory[idx]);
		break;
	case 0x25:
		// SLA (I[XY] + data), L
		memory[idx] = sla(memory[idx]);
		_curr_reg->_HL.low(memory[idx]);
		break;
	case 0x26:
		// SLA (I[XY] + data)
		memory[idx] = sla(memory[idx]);
		break;
	case 0x27:
		// SLA (I[XY] + data), A
		memory[idx] = sla(memory[idx]);
		_curr_reg->_AF.high(memory[idx]);
		break;
	case 0x28:
		// SRA (I[XY] + data), B
		memory[idx] = sra(memory[idx]);
		_curr_reg->_BC.high(memory[idx]);
		break;
	case 0x29:
		// SRA (I[XY] + data), C
		memory[idx] = sra(memory[idx]);
		_curr_reg->_BC.low(memory[idx]);
		break;
	case 0x2a:
		// SRA (I[XY] + data), D
		memory[idx] = sra(memory[idx]);
		_curr_reg->_DE.high(memory[idx]);
		break;
	case 0x2b:
		// SRA (I[XY] + data), E
		memory[idx] = sra(memory[idx]);
		_curr_reg->_DE.low(memory[idx]);
		break;
	case 0x2c:
		// SRA (I[XY] + data), H
		memory[idx] = sra(memory[idx]);
		_curr_reg->_HL.high(memory[idx]);
		break;
	case 0x2d:
		// SRA (I[XY] + data), L
		memory[idx] = sra(memory[idx]);
		_curr_reg->_HL.low(memory[idx]);
		break;
	case 0x2e:
		// SRA (I[XY] + data)
		memory[idx] = sra(memory[idx]);
		break;
	case 0x2f:
		// SRA (I[XY] + data), A
		memory[idx] = sra(memory[idx]);
		_curr_reg->_AF.high(memory[idx]);
		break;
	case 0x30:
		// SLL (I[XY] + data), B
		memory[idx] = sll(memory[idx]);
		_curr_reg->_BC.high(memory[idx]);
		break;
	case 0x31:
		// SLL (I[XY] + data), C
		memory[idx] = sll(memory[idx]);
		_curr_reg->_BC.low(memory[idx]);
		break;
	case 0x32:
		// SLL (I[XY] + data), D
		memory[idx] = sll(memory[idx]);
		_curr_reg->_DE.high(memory[idx]);
		break;
	case 0x33:
		// SLL (I[XY] + data), E
		memory[idx] = sll(memory[idx]);
		_curr_reg->_DE.low(memory[idx]);
		break;
	case 0x34:
		// SLL (I[XY] + data), H
		memory[idx] = sll(memory[idx]);
		_curr_reg->_HL.high(memory[idx]);
		break;
	case 0x35:
		// SLL (I[XY] + data), L
		memory[idx] = sll(memory[idx]);
		_curr_reg->_HL.low(memory[idx]);
		break;
	case 0x36:
		// SLL (I[XY] + data)
		memory[idx] = sll(memory[idx]);
		break;
	case 0x37:
		// SLL (I[XY] + data), A
		memory[idx] = sll(memory[idx]);
		_curr_reg->_AF.high(memory[idx]);
		break;
	case 0x38:
		// SRL (I[XY] + data), B
		memory[idx] = srl(memory[idx]);
		_curr_reg->_BC.high(memory[idx]);
		break;
	case 0x39:
		// SRL (I[XY] + data), C
		memory[idx] = srl(memory[idx]);
		_curr_reg->_BC.low(memory[idx]);
		break;
	case 0x3a:
		// SRL (I[XY] + data), D
		memory[idx] = srl(memory[idx]);
		_curr_reg->_DE.high(memory[idx]);
		break;
	case 0x3b:
		// SRL (I[XY] + data), E
		memory[idx] = srl(memory[idx]);
		_curr_reg->_DE.low(memory[idx]);
		break;
	case 0x3c:
		// SRL (I[XY] + data), H
		memory[idx] = srl(memory[idx]);
		_curr_reg->_HL.high(memory[idx]);
		break;
	case 0x3d:
		// SRL (I[XY] + data), L
		memory[idx] = srl(memory[idx]);
		_curr_reg->_HL.low(memory[idx]);
		break;
	case 0x3e:
		// SRL (I[XY] + data)
		memory[idx] = srl(memory[idx]);
		break;
	case 0x3f:
		// SRL (I[XY] + data), A
		memory[idx] = srl(memory[idx]);
		_curr_reg->_AF.high(memory[idx]);
		break;
	case 0x40:
	case 0x41:
	case 0x42:
	case 0x43:
	case 0x44:
	case 0x45:
	case 0x46:
	case 0x47:
		// BIT 0, (I[XY] + data)
		bit(memory[idx] & 1);
		break;
	case 0x48:
	case 0x49:
	case 0x4a:
	case 0x4b:
	case 0x4c:
	case 0x4d:
	case 0x4e:
	case 0x4f:
		// BIT 1, (I[XY] + data)
		bit(memory[idx] & 2);
		break;
	case 0x50:
	case 0x51:
	case 0x52:
	case 0x53:
	case 0x54:
	case 0x55:
	case 0x56:
	case 0x57:
		// BIT 2, (I[XY] + data)
		bit(memory[idx] & 4);
		break;
	case 0x58:
	case 0x59:
	case 0x5a:
	case 0x5b:
	case 0x5c:
	case 0x5d:
	case 0x5e:
	case 0x5f:
		// BIT 3, (I[XY] + data)
		bit(memory[idx] & 8);
		break;
	case 0x60:
	case 0x61:
	case 0x62:
	case 0x63:
	case 0x64:
	case 0x65:
	case 0x66:
	case 0x67:
		// BIT 4, (I[XY] + data)
		bit(memory[idx] & 0x10);
		break;
	case 0x68:
	case 0x69:
	case 0x6a:
	case 0x6b:
	case 0x6c:
	case 0x6d:
	case 0x6e:
	case 0x6f:
		// BIT 5, (I[XY] + data)
		bit(memory[idx] & 0x20);
		break;
	case 0x70:
	case 0x71:
	case 0x72:
	case 0x73:
	case 0x74:
	case 0x75:
	case 0x76:
	case 0x77:
		// BIT 6, (I[XY] + data)
		bit(memory[idx] & 0x40);
		break;
	case 0x78:
	case 0x79:
	case 0x7a:
	case 0x7b:
	case 0x7c:
	case 0x7d:
	case 0x7e:
	case 0x7f:
		// BIT 7, (I[XY] + data)
		bit(memory[idx] & 0x80);
		break;
	case 0x80:
		// RES 0, (I[XY] + data), B
		memory[idx] &= ~1;
		_curr_reg->_BC.high(memory[idx]);
		// No flags affected
		break;
	case 0x81:
		// RES 0, (I[XY] + data), C
		memory[idx] &= ~1;
		_curr_reg->_BC.low(memory[idx]);
		// No flags affected
		break;
	case 0x82:
		// RES 0, (I[XY] + data), D
		memory[idx] &= ~1;
		_curr_reg->_DE.high(memory[idx]);
		// No flags affected
		break;
	case 0x83:
		// RES 0, (I[XY] + data), E
		memory[idx] &= ~1;
		_curr_reg->_DE.low(memory[idx]);
		// No flags affected
		break;
	case 0x84:
		// RES 0, (I[XY] + data), H
		memory[idx] &= ~1;
		_curr_reg->_HL.high(memory[idx]);
		// No flags affected
		break;
	case 0x85:
		// RES 0, (I[XY] + data), L
		memory[idx] &= ~1;
		_curr_reg->_HL.low(memory[idx]);
		// No flags affected
		break;
	case 0x86:
		// RES 0, (I[XY] + data)
		memory[idx] &= ~1;
		// No flags affected
		break;
	case 0x87:
		// RES 0, (I[XY] + data), A
		memory[idx] &= ~1;
		_curr_reg->_AF.high(memory[idx]);
		// No flags affected
		break;
	case 0x88:
		// RES 1, (I[XY] + data), B
		memory[idx] &= ~2;
		_curr_reg->_BC.high(memory[idx]);
		// No flags affected
		break;
	case 0x89:
		// RES 1, (I[XY] + data), C
		memory[idx] &= ~2;
		_curr_reg->_BC.low(memory[idx]);
		// No flags affected
		break;
	case 0x8a:
		// RES 1, (I[XY] + data), D
		memory[idx] &= ~2;
		_curr_reg->_DE.high(memory[idx]);
		// No flags affected
		break;
	case 0x8b:
		// RES 1, (I[XY] + data), E
		memory[idx] &= ~2;
		_curr_reg->_DE.low(memory[idx]);
		// No flags affected
		break;
	case 0x8c:
		// RES 1, (I[XY] + data), H
		memory[idx] &= ~2;
		_curr_reg->_HL.high(memory[idx]);
		// No flags affected
		break;
	case 0x8d:
		// RES 1, (I[XY] + data), L
		memory[idx] &= ~2;
		_curr_reg->_HL.low(memory[idx]);
		// No flags affected
		break;
	case 0x8e:
		// RES 1, (I[XY] + data)
		memory[idx] &= ~2;
		// No flags affected
		break;
	case 0x8f:
		// RES 1, (I[XY] + data), A
		memory[idx] &= ~2;
		_curr_reg->_AF.high(memory[idx]);
		// No flags affected
		break;
	case 0x90:
		// RES 2, (I[XY] + data), B
		memory[idx] &= ~4;
		_curr_reg->_BC.high(memory[idx]);
		// No flags affected
		break;
	case 0x91:
		// RES 2, (I[XY] + data), C
		memory[idx] &= ~4;
		_curr_reg->_BC.low(memory[idx]);
		// No flags affected
		break;
	case 0x92:
		// RES 2, (I[XY] + data), D
		memory[idx] &= ~4;
		_curr_reg->_DE.high(memory[idx]);
		// No flags affected
		break;
	case 0x93:
		// RES 2, (I[XY] + data), E
		memory[idx] &= ~4;
		_curr_reg->_DE.low(memory[idx]);
		// No flags affected
		break;
	case 0x94:
		// RES 2, (I[XY] + data), H
		memory[idx] &= ~4;
		_curr_reg->_HL.high(memory[idx]);
		// No flags affected
		break;
	case 0x95:
		// RES 2, (I[XY] + data), L
		memory[idx] &= ~4;
		_curr_reg->_HL.low(memory[idx]);
		// No flags affected
		break;
	case 0x96:
		// RES 2, (I[XY] + data)
		memory[idx] &= ~4;
		// No flags affected
		break;
	case 0x97:
		// RES 2, (I[XY] + data), A
		memory[idx] &= ~4;
		_curr_reg->_AF.high(memory[idx]);
		// No flags affected
		break;
	case 0x98:
		// RES 3, (I[XY] + data), B
		memory[idx] &= ~8;
		_curr_reg->_BC.high(memory[idx]);
		// No flags affected
		break;
	case 0x99:
		// RES 3, (I[XY] + data), C
		memory[idx] &= ~8;
		_curr_reg->_BC.low(memory[idx]);
		// No flags affected
		break;
	case 0x9a:
		// RES 3, (I[XY] + data), D
		memory[idx] &= ~8;
		_curr_reg->_DE.high(memory[idx]);
		// No flags affected
		break;
	case 0x9b:
		// RES 3, (I[XY] + data), E
		memory[idx] &= ~8;
		_curr_reg->_DE.low(memory[idx]);
		// No flags affected
		break;
	case 0x9c:
		// RES 3, (I[XY] + data), H
		memory[idx] &= ~8;
		_curr_reg->_HL.high(memory[idx]);
		// No flags affected
		break;
	case 0x9d:
		// RES 3, (I[XY] + data), L
		memory[idx] &= ~8;
		_curr_reg->_HL.low(memory[idx]);
		// No flags affected
		break;
	case 0x9e:
		// RES 3, (I[XY] + data)
		memory[idx] &= ~8;
		// No flags affected
		break;
	case 0x9f:
		// RES 3, (I[XY] + data), A
		memory[idx] &= ~8;
		_curr_reg->_AF.high(memory[idx]);
		// No flags affected
		break;
	case 0xa0:
		// RES 4, (I[XY] + data), B
		memory[idx] &= ~0x10;
		_curr_reg->_BC.high(memory[idx]);
		// No flags affected
		break;
	case 0xa1:
		// RES 4, (I[XY] + data), C
		memory[idx] &= ~0x10;
		_curr_reg->_BC.low(memory[idx]);
		// No flags affected
		break;
	case 0xa2:
		// RES 4, (I[XY] + data), D
		memory[idx] &= ~0x10;
		_curr_reg->_DE.high(memory[idx]);
		// No flags affected
		break;
	case 0xa3:
		// RES 4, (I[XY] + data), E
		memory[idx] &= ~0x10;
		_curr_reg->_DE.low(memory[idx]);
		// No flags affected
		break;
	case 0xa4:
		// RES 4, (I[XY] + data), H
		memory[idx] &= ~0x10;
		_curr_reg->_HL.high(memory[idx]);
		// No flags affected
		break;
	case 0xa5:
		// RES 4, (I[XY] + data), L
		memory[idx] &= ~0x10;
		_curr_reg->_HL.low(memory[idx]);
		// No flags affected
		break;
	case 0xa6:
		// RES 4, (I[XY] + data)
		memory[idx] &= ~0x10;
		// No flags affected
		break;
	case 0xa7:
		// RES 4, (I[XY] + data), A
		memory[idx] &= ~0x10;
		_curr_reg->_AF.high(memory[idx]);
		// No flags affected
		break;
	case 0xa8:
		// RES 5, (I[XY] + data), B
		memory[idx] &= ~0x20;
		_curr_reg->_BC.high(memory[idx]);
		// No flags affected
		break;
	case 0xa9:
		// RES 5, (I[XY] + data), C
		memory[idx] &= ~0x20;
		_curr_reg->_BC.low(memory[idx]);
		// No flags affected
		break;
	case 0xaa:
		// RES 5, (I[XY] + data), D
		memory[idx] &= ~0x20;
		_curr_reg->_DE.high(memory[idx]);
		// No flags affected
		break;
	case 0xab:
		// RES 5, (I[XY] + data), E
		memory[idx] &= ~0x20;
		_curr_reg->_DE.low(memory[idx]);
		// No flags affected
		break;
	case 0xac:
		// RES 5, (I[XY] + data), H
		memory[idx] &= ~0x20;
		_curr_reg->_HL.high(memory[idx]);
		// No flags affected
		break;
	case 0xad:
		// RES 5, (I[XY] + data), L
		memory[idx] &= ~0x20;
		_curr_reg->_HL.low(memory[idx]);
		// No flags affected
		break;
	case 0xae:
		// RES 5, (I[XY] + data)
		memory[idx] &= ~0x20;
		// No flags affected
		break;
	case 0xaf:
		// RES 5, (I[XY] + data), A
		memory[idx] &= ~0x20;
		_curr_reg->_AF.high(memory[idx]);
		// No flags affected
		break;
	case 0xb0:
		// RES 6, (I[XY] + data), B
		memory[idx] &= ~0x40;
		_curr_reg->_BC.high(memory[idx]);
		// No flags affected
		break;
	case 0xb1:
		// RES 6, (I[XY] + data), C
		memory[idx] &= ~0x40;
		_curr_reg->_BC.low(memory[idx]);
		// No flags affected
		break;
	case 0xb2:
		// RES 6, (I[XY] + data), D
		memory[idx] &= ~0x40;
		_curr_reg->_DE.high(memory[idx]);
		// No flags affected
		break;
	case 0xb3:
		// RES 6, (I[XY] + data), E
		memory[idx] &= ~0x40;
		_curr_reg->_DE.low(memory[idx]);
		// No flags affected
		break;
	case 0xb4:
		// RES 6, (I[XY] + data), H
		memory[idx] &= ~0x40;
		_curr_reg->_HL.high(memory[idx]);
		// No flags affected
		break;
	case 0xb5:
		// RES 6, (I[XY] + data), L
		memory[idx] &= ~0x40;
		_curr_reg->_HL.low(memory[idx]);
		// No flags affected
		break;
	case 0xb6:
		// RES 6, (I[XY] + data)
		memory[idx] &= ~0x40;
		// No flags affected
		break;
	case 0xb7:
		// RES 6, (I[XY] + data), A
		memory[idx] &= ~0x40;
		_curr_reg->_AF.high(memory[idx]);
		// No flags affected
		break;
	case 0xb8:
		// RES 7, (I[XY] + data), B
		memory[idx] &= ~0x80;
		_curr_reg->_BC.high(memory[idx]);
		// No flags affected
		break;
	case 0xb9:
		// RES 7, (I[XY] + data), C
		memory[idx] &= ~0x80;
		_curr_reg->_BC.low(memory[idx]);
		// No flags affected
		break;
	case 0xba:
		// RES 7, (I[XY] + data), D
		memory[idx] &= ~0x80;
		_curr_reg->_DE.high(memory[idx]);
		// No flags affected
		break;
	case 0xbb:
		// RES 7, (I[XY] + data), E
		memory[idx] &= ~0x80;
		_curr_reg->_DE.low(memory[idx]);
		// No flags affected
		break;
	case 0xbc:
		// RES 7, (I[XY] + data), H
		memory[idx] &= ~0x80;
		_curr_reg->_HL.high(memory[idx]);
		// No flags affected
		break;
	case 0xbd:
		// RES 7, (I[XY] + data), L
		memory[idx] &= ~0x80;
		_curr_reg->_HL.low(memory[idx]);
		// No flags affected
		break;
	case 0xbe:
		// RES 7, (I[XY] + data)
		memory[idx] &= ~0x80;
		// No flags affected
		break;
	case 0xbf:
		// RES 7, (I[XY] + data), A
		memory[idx] &= ~0x80;
		_curr_reg->_AF.high(memory[idx]);
		// No flags affected
		break;
	case 0xc0:
		// SET 0, (I[XY] + data), B
		memory[idx] |= 1;
		_curr_reg->_BC.high(memory[idx]);
		// No flags affected
		break;
	case 0xc1:
		// SET 0, (I[XY] + data), C
		memory[idx] |= 1;
		_curr_reg->_BC.low(memory[idx]);
		// No flags affected
		break;
	case 0xc2:
		// SET 0, (I[XY] + data), D
		memory[idx] |= 1;
		_curr_reg->_DE.high(memory[idx]);
		// No flags affected
		break;
	case 0xc3:
		// SET 0, (I[XY] + data), E
		memory[idx] |= 1;
		_curr_reg->_DE.low(memory[idx]);
		// No flags affected
		break;
	case 0xc4:
		// SET 0, (I[XY] + data), H
		memory[idx] |= 1;
		_curr_reg->_HL.high(memory[idx]);
		// No flags affected
		break;
	case 0xc5:
		// SET 0, (I[XY] + data), L
		memory[idx] |= 1;
		_curr_reg->_HL.low(memory[idx]);
		// No flags affected
		break;
	case 0xc6:
		// SET 0, (I[XY] + data)
		memory[idx] |= 1;
		// No flags affected
		break;
	case 0xc7:
		// SET 0, (I[XY] + data), A
		memory[idx] |= 1;
		_curr_reg->_AF.high(memory[idx]);
		// No flags affected
		break;
	case 0xc8:
		// SET 1, (I[XY] + data), B
		memory[idx] |= 2;
		_curr_reg->_BC.high(memory[idx]);
		// No flags affected
		break;
	case 0xc9:
		// SET 1, (I[XY] + data), C
		memory[idx] |= 2;
		_curr_reg->_BC.low(memory[idx]);
		// No flags affected
		break;
	case 0xca:
		// SET 1, (I[XY] + data), D
		memory[idx] |= 2;
		_curr_reg->_DE.high(memory[idx]);
		// No flags affected
		break;
	case 0xcb:
		// SET 1, (I[XY] + data), E
		memory[idx] |= 2;
		_curr_reg->_DE.low(memory[idx]);
		// No flags affected
		break;
	case 0xcc:
		// SET 1, (I[XY] + data), H
		memory[idx] |= 2;
		_curr_reg->_HL.high(memory[idx]);
		// No flags affected
		break;
	case 0xcd:
		// SET 1, (I[XY] + data), L
		memory[idx] |= 2;
		_curr_reg->_HL.low(memory[idx]);
		// No flags affected
		break;
	case 0xce:
		// SET 1, (I[XY] + data)
		memory[idx] |= 2;
		// No flags affected
		break;
	case 0xcf:
		// SET 1, (I[XY] + data), A
		memory[idx] |= 2;
		_curr_reg->_AF.high(memory[idx]);
		// No flags affected
		break;
	case 0xd0:
		// SET 2, (I[XY] + data), B
		memory[idx] |= 4;
		_curr_reg->_BC.high(memory[idx]);
		// No flags affected
		break;
	case 0xd1:
		// SET 2, (I[XY] + data), C
		memory[idx] |= 4;
		_curr_reg->_BC.low(memory[idx]);
		// No flags affected
		break;
	case 0xd2:
		// SET 2, (I[XY] + data), D
		memory[idx] |= 4;
		_curr_reg->_DE.high(memory[idx]);
		// No flags affected
		break;
	case 0xd3:
		// SET 2, (I[XY] + data), E
		memory[idx] |= 4;
		_curr_reg->_DE.low(memory[idx]);
		// No flags affected
		break;
	case 0xd4:
		// SET 2, (I[XY] + data), H
		memory[idx] |= 4;
		_curr_reg->_HL.high(memory[idx]);
		// No flags affected
		break;
	case 0xd5:
		// SET 2, (I[XY] + data), L
		memory[idx] |= 4;
		_curr_reg->_HL.low(memory[idx]);
		// No flags affected
		break;
	case 0xd6:
		// SET 2, (I[XY] + data)
		memory[idx] |= 4;
		// No flags affected
		break;
	case 0xd7:
		// SET 2, (I[XY] + data), A
		memory[idx] |= 4;
		_curr_reg->_AF.high(memory[idx]);
		// No flags affected
		break;
	case 0xd8:
		// SET 3, (I[XY] + data), B
		memory[idx] |= 8;
		_curr_reg->_BC.high(memory[idx]);
		// No flags affected
		break;
	case 0xd9:
		// SET 3, (I[XY] + data), C
		memory[idx] |= 8;
		_curr_reg->_BC.low(memory[idx]);
		// No flags affected
		break;
	case 0xda:
		// SET 3, (I[XY] + data), D
		memory[idx] |= 8;
		_curr_reg->_DE.high(memory[idx]);
		// No flags affected
		break;
	case 0xdb:
		// SET 3, (I[XY] + data), E
		memory[idx] |= 8;
		_curr_reg->_DE.low(memory[idx]);
		// No flags affected
		break;
	case 0xdc:
		// SET 3, (I[XY] + data), H
		memory[idx] |= 8;
		_curr_reg->_HL.high(memory[idx]);
		// No flags affected
		break;
	case 0xdd:
		// SET 3, (I[XY] + data), L
		memory[idx] |= 8;
		_curr_reg->_HL.low(memory[idx]);
		// No flags affected
		break;
	case 0xde:
		// SET 3, (I[XY] + data)
		memory[idx] |= 8;
		// No flags affected
		break;
	case 0xdf:
		// SET 3, (I[XY] + data), A
		memory[idx] |= 8;
		_curr_reg->_AF.high(memory[idx]);
		// No flags affected
		break;
	case 0xe0:
		// SET 4, (I[XY] + data), B
		memory[idx] |= 0x10;
		_curr_reg->_BC.high(memory[idx]);
		// No flags affected
		break;
	case 0xe1:
		// SET 4, (I[XY] + data), C
		memory[idx] |= 0x10;
		_curr_reg->_BC.low(memory[idx]);
		// No flags affected
		break;
	case 0xe2:
		// SET 4, (I[XY] + data), D
		memory[idx] |= 0x10;
		_curr_reg->_DE.high(memory[idx]);
		// No flags affected
		break;
	case 0xe3:
		// SET 4, (I[XY] + data), E
		memory[idx] |= 0x10;
		_curr_reg->_DE.low(memory[idx]);
		// No flags affected
		break;
	case 0xe4:
		// SET 4, (I[XY] + data), H
		memory[idx] |= 0x10;
		_curr_reg->_HL.high(memory[idx]);
		// No flags affected
		break;
	case 0xe5:
		// SET 4, (I[XY] + data), L
		memory[idx] |= 0x10;
		_curr_reg->_HL.low(memory[idx]);
		// No flags affected
		break;
	case 0xe6:
		// SET 4, (I[XY] + data)
		memory[idx] |= 0x10;
		// No flags affected
		break;
	case 0xe7:
		// SET 4, (I[XY] + data), A
		memory[idx] |= 0x10;
		_curr_reg->_AF.high(memory[idx]);
		// No flags affected
		break;
	case 0xe8:
		// SET 5, (I[XY] + data), B
		memory[idx] |= 0x20;
		_curr_reg->_BC.high(memory[idx]);
		// No flags affected
		break;
	case 0xe9:
		// SET 5, (I[XY] + data), C
		memory[idx] |= 0x20;
		_curr_reg->_BC.low(memory[idx]);
		// No flags affected
		break;
	case 0xea:
		// SET 5, (I[XY] + data), D
		memory[idx] |= 0x20;
		_curr_reg->_DE.high(memory[idx]);
		// No flags affected
		break;
	case 0xeb:
		// SET 5, (I[XY] + data), E
		memory[idx] |= 0x20;
		_curr_reg->_DE.low(memory[idx]);
		// No flags affected
		break;
	case 0xec:
		// SET 5, (I[XY] + data), H
		memory[idx] |= 0x20;
		_curr_reg->_HL.high(memory[idx]);
		// No flags affected
		break;
	case 0xed:
		// SET 5, (I[XY] + data), L
		memory[idx] |= 0x20;
		_curr_reg->_HL.low(memory[idx]);
		// No flags affected
		break;
	case 0xee:
		// SET 5, (I[XY] + data)
		memory[idx] |= 0x20;
		// No flags affected
		break;
	case 0xef:
		// SET 5, (I[XY] + data), A
		memory[idx] |= 0x20;
		_curr_reg->_AF.high(memory[idx]);
		// No flags affected
		break;
	case 0xf0:
		// SET 6, (I[XY] + data), B
		memory[idx] |= 0x40;
		_curr_reg->_BC.high(memory[idx]);
		// No flags affected
		break;
	case 0xf1:
		// SET 6, (I[XY] + data), C
		memory[idx] |= 0x40;
		_curr_reg->_BC.low(memory[idx]);
		// No flags affected
		break;
	case 0xf2:
		// SET 6, (I[XY] + data), D
		memory[idx] |= 0x40;
		_curr_reg->_DE.high(memory[idx]);
		// No flags affected
		break;
	case 0xf3:
		// SET 6, (I[XY] + data), E
		memory[idx] |= 0x40;
		_curr_reg->_DE.low(memory[idx]);
		// No flags affected
		break;
	case 0xf4:
		// SET 6, (I[XY] + data), H
		memory[idx] |= 0x40;
		_curr_reg->_HL.high(memory[idx]);
		// No flags affected
		break;
	case 0xf5:
		// SET 6, (I[XY] + data), L
		memory[idx] |= 0x40;
		_curr_reg->_HL.low(memory[idx]);
		// No flags affected
		break;
	case 0xf6:
		// SET 6, (I[XY] + data)
		memory[idx] |= 0x40;
		// No flags affected
		break;
	case 0xf7:
		// SET 6, (I[XY] + data), A
		memory[idx] |= 0x40;
		_curr_reg->_AF.high(memory[idx]);
		// No flags affected
		break;
	case 0xf8:
		// SET 7, (I[XY] + data), B
		memory[idx] |= 0x80;
		_curr_reg->_BC.high(memory[idx]);
		// No flags affected
		break;
	case 0xf9:
		// SET 7, (I[XY] + data), C
		memory[idx] |= 0x80;
		_curr_reg->_BC.low(memory[idx]);
		// No flags affected
		break;
	case 0xfa:
		// SET 7, (I[XY] + data), D
		memory[idx] |= 0x80;
		_curr_reg->_DE.high(memory[idx]);
		// No flags affected
		break;
	case 0xfb:
		// SET 7, (I[XY] + data), E
		memory[idx] |= 0x80;
		_curr_reg->_DE.low(memory[idx]);
		// No flags affected
		break;
	case 0xfc:
		// SET 7, (I[XY] + data), H
		memory[idx] |= 0x80;
		_curr_reg->_HL.high(memory[idx]);
		// No flags affected
		break;
	case 0xfd:
		// SET 7, (I[XY] + data), L
		memory[idx] |= 0x80;
		_curr_reg->_HL.low(memory[idx]);
		// No flags affected
		break;
	case 0xfe:
		// SET 7, (I[XY] + data)
		memory[idx] |= 0x80;
		// No flags affected
		break;
	case 0xff:
		// SET 7, (I[XY] + data), A
		memory[idx] |= 0x80;
		_curr_reg->_AF.high(memory[idx]);
		// No flags affected
		break;
	}
}

void z80::step_ext(uint8_t* memory)
{
	switch (memory[++_PC])
	{
	case 0x40:
		// IN B, (C)
		++_PC;
		break;
	case 0x41:
		// OUT (C), B
		++_PC;
		break;
	case 0x42:
		// SBC HL, BC
		sbc_hl(_curr_reg->_BC);
		++_PC;
		break;
	case 0x43:
		// LD (nn), BC
		poke_register_indirect(memory, _curr_reg->_BC);
		// No flags affected
		_PC += 2;
		break;
	case 0x44:
	case 0x4c:
	case 0x54:
	case 0x5c:
	case 0x64:
	case 0x6c:
	case 0x74:
	case 0x7c:
		// NEG
		_curr_reg->_AF.high(0 - _curr_reg->_AF.high());
		// TODO: set flags
		++_PC;
		break;
	case 0x45:
		// RETN
		++_PC;
		break;
	case 0x46:
		// IM 0
		++_PC;
		break;
	case 0x47:
		// LD I, A
		_curr_reg->_IR.high(_curr_reg->_AF.high());
		// No flags affected
		++_PC;
		break;
	case 0x48:
		// IN C, (C)
		++_PC;
		break;
	case 0x49:
		// OUT (C), C
		++_PC;
		break;
	case 0x4a:
		// ADC HL, BC
		adc_hl(_curr_reg->_BC);
		++_PC;
		break;
	case 0x4b:
		// LD BC, (dd)
		_curr_reg->_BC.value(peek_indirect(memory));
		// No flags affected
		_PC += 2;
		break;
	case 0x4d:
		// RETI
		++_PC;
		break;
	case 0x4e:
		// IM 0/1
		++_PC;
		break;
	case 0x4f:
		// LD R, A
		_curr_reg->_IR.low(_curr_reg->_AF.high());
		// No flags affected
		++_PC;
		break;
	case 0x50:
		// IN D, (C)
		++_PC;
		break;
	case 0x51:
		// OUT (C), D
		++_PC;
		break;
	case 0x52:
		// SBC HL, DE
		sbc_hl(_curr_reg->_DE);
		++_PC;
		break;
	case 0x53:
		// LD (dd), DE
		poke_register_indirect(memory, _curr_reg->_DE);
		// No flags affected
		_PC += 2;
		break;
	case 0x55:
		// RETN
		++_PC;
		break;
	case 0x56:
		// IM 1
		++_PC;
		break;
	case 0x57:
		// LD A, I
		_curr_reg->_AF.high(_curr_reg->_IR.high());
		// TODO: set flags
		++_PC;
		break;
	case 0x58:
		// IN E, (C)
		++_PC;
		break;
	case 0x59:
		// OUT (C), E
		++_PC;
		break;
	case 0x5a:
		// ADC HL, DE
		adc_hl(_curr_reg->_DE);
		++_PC;
		break;
	case 0x5b:
		// LD DE, (dd)
		_curr_reg->_DE.value(peek_indirect(memory));
		// No flags affected
		_PC += 2;
		break;
	case 0x5d:
		// RETN
		++_PC;
		break;
	case 0x5e:
		// IM 2
		++_PC;
		break;
	case 0x5f:
		// LD A, R
		_curr_reg->_AF.high(_curr_reg->_IR.low());
		// TODO: set flags
		++_PC;
		break;
	case 0x60:
		// IN H, (C)
		++_PC;
		break;
	case 0x61:
		// OUT (C), H
		++_PC;
		break;
	case 0x62:
		// SBC HL, HL
		sbc_hl(_curr_reg->_HL);
		++_PC;
		break;
	case 0x63:
		// LD (dd), HL
		poke_register_indirect(memory, _curr_reg->_HL);
		// No flags affected
		_PC += 2;
		break;
	case 0x65:
		// RETN
		++_PC;
		break;
	case 0x66:
		// IM 0
		++_PC;
		break;
	case 0x67:
		// RRD
		++_PC;
		break;
	case 0x68:
		// IN L, (C)
		++_PC;
		break;
	case 0x69:
		// OUT (C), L
		++_PC;
		break;
	case 0x6a:
		// ADC HL, HL
		adc_hl(_curr_reg->_HL);
		++_PC;
		break;
	case 0x6b:
		// LD HL, (dd)
		_curr_reg->_HL.value(peek_indirect(memory));
		// No flags affected
		_PC += 2;
		break;
	case 0x6d:
		// RETN
		++_PC;
		break;
	case 0x6e:
		// IM 0/1
		++_PC;
		break;
	case 0x6f:
		// RLD
		++_PC;
		break;
	case 0x70:
		// IN (C)
		++_PC;
		break;
	case 0x71:
		// OUT (C), 0
		++_PC;
		break;
	case 0x72:
		// SBC HL, SP
		sbc_hl(_SP);
		++_PC;
		break;
	case 0x73:
		// LD (dd), SP
		poke_register_indirect(memory, _SP);
		// No flags affected
		_PC += 2;
		break;
	case 0x75:
		// RETN
		++_PC;
		break;
	case 0x76:
		// IM 1
		++_PC;
		break;
	case 0x78:
		// IN A, (C)
		++_PC;
		break;
	case 0x79:
		// OUT (C), A
		++_PC;
		break;
	case 0x7a:
		// ADC HL, SP
		adc_hl(_SP);
		++_PC;
		break;
	case 0x7b:
		// LD SP, (dd)
		_SP.value(peek_indirect(memory));
		// No flags affected
		_PC += 2;
		break;
	case 0x7d:
		// RETN
		++_PC;
		break;
	case 0x7e:
		// IM 2
		++_PC;
		break;
	case 0xa0:
		// LDI
		++_PC;
		break;
	case 0xa1:
		// CPI
		++_PC;
		break;
	case 0xa2:
		// INI
		++_PC;
		break;
	case 0xa3:
		// OUTI
		++_PC;
		break;
	case 0xa8:
		// LDD
		++_PC;
		break;
	case 0xa9:
		// CPD
		++_PC;
		break;
	case 0xaa:
		// IND
		++_PC;
		break;
	case 0xab:
		// OUTD
		++_PC;
		break;
	case 0xb0:
	{
		// LDIR
		auto src = &memory[_curr_reg->_HL.value()];
		auto dest = &memory[_curr_reg->_DE.value()];
		uint8_t flags = _curr_reg->_AF.low() & (FLAG::S | FLAG::Z | FLAG::C);

		// Don't use memcpy, as src and dest could overlap
		for (auto count = _curr_reg->_BC.value(); count; --count)
		{
			*dest++ = *src++;
		}

		add(_curr_reg->_HL, _curr_reg->_BC);
		add(_curr_reg->_DE, _curr_reg->_BC);
		_curr_reg->_BC.value(uint16_t(0));
		_curr_reg->_AF.low(flags);
		++_PC;
		break;
	}
	case 0xb1:
		// CPIR
		++_PC;
		break;
	case 0xb2:
		// INIR
		++_PC;
		break;
	case 0xb3:
		// OTIR
		++_PC;
		break;
	case 0xb8:
		// LDDR
		++_PC;
		break;
	case 0xb9:
		// CPDR
		++_PC;
		break;
	case 0xba:
		// INDR
		++_PC;
		break;
	case 0xbb:
		// OTDR
		++_PC;
		break;
	default:
		break;
	}
}

void z80::step_bits(uint8_t* memory)
{
	switch (memory[++_PC])
	{
	case 0x00:
		// RLC B
		_curr_reg->_BC.high(rlc(_curr_reg->_BC.high()));
		break;
	case 0x01:
		// RLC C
		_curr_reg->_BC.low(rlc(_curr_reg->_BC.low()));
		break;
	case 0x02:
		// RLC D
		_curr_reg->_DE.high(rlc(_curr_reg->_DE.high()));
		break;
	case 0x03:
		// RLC E
		_curr_reg->_DE.low(rlc(_curr_reg->_DE.low()));
		break;
	case 0x04:
		// RLC H
		_curr_reg->_HL.high(rlc(_curr_reg->_HL.high()));
		break;
	case 0x05:
		// RLC L
		_curr_reg->_HL.low(rlc(_curr_reg->_HL.low()));
		break;
	case 0x06:
		// RLC (HL)
		memory[_curr_reg->_HL.value()] = rlc(memory[_curr_reg->_HL.value()]);
		break;
	case 0x07:
		// RLC A
		_curr_reg->_AF.high(rlc(_curr_reg->_AF.high()));
		break;
	case 0x08:
		// RRC B
		_curr_reg->_BC.high(rrc(_curr_reg->_BC.high()));
		break;
	case 0x09:
		// RRC C
		_curr_reg->_BC.low(rrc(_curr_reg->_BC.low()));
		break;
	case 0x0a:
		// RRC D
		_curr_reg->_DE.high(rrc(_curr_reg->_DE.high()));
		break;
	case 0x0b:
		// RRC E
		_curr_reg->_DE.low(rrc(_curr_reg->_DE.low()));
		break;
	case 0x0c:
		// RRC H
		_curr_reg->_HL.high(rrc(_curr_reg->_HL.high()));
		break;
	case 0x0d:
		// RRC L
		_curr_reg->_HL.low(rrc(_curr_reg->_HL.low()));
		break;
	case 0x0e:
		// RRC (HL)
		memory[_curr_reg->_HL.value()] = rrc(memory[_curr_reg->_HL.value()]);
		break;
	case 0x0f:
		// RRC A
		_curr_reg->_AF.high(rrc(_curr_reg->_AF.high()));
		break;
	case 0x10:
		// RL B
		_curr_reg->_BC.high(rl(_curr_reg->_BC.high()));
		break;
	case 0x11:
		// RL C
		_curr_reg->_BC.low(rl(_curr_reg->_BC.low()));
		break;
	case 0x12:
		// RL D
		_curr_reg->_DE.high(rl(_curr_reg->_DE.high()));
		break;
	case 0x13:
		// RL E
		_curr_reg->_DE.low(rl(_curr_reg->_DE.low()));
		break;
	case 0x14:
		// RL H
		_curr_reg->_HL.high(rl(_curr_reg->_HL.high()));
		break;
	case 0x15:
		// RL L
		_curr_reg->_HL.low(rl(_curr_reg->_HL.low()));
		break;
	case 0x16:
		// RL (HL)
		memory[_curr_reg->_HL.value()] = rl(memory[_curr_reg->_HL.value()]);
		break;
	case 0x17:
		// RL A
		_curr_reg->_AF.high(rl(_curr_reg->_AF.high()));
		break;
	case 0x18:
		// RR B
		_curr_reg->_BC.high(rr(_curr_reg->_BC.high()));
		break;
	case 0x19:
		// RR C
		_curr_reg->_BC.low(rr(_curr_reg->_BC.low()));
		break;
	case 0x1a:
		// RR D
		_curr_reg->_DE.high(rr(_curr_reg->_DE.high()));
		break;
	case 0x1b:
		// RR E
		_curr_reg->_DE.low(rr(_curr_reg->_DE.low()));
		break;
	case 0x1c:
		// RR H
		_curr_reg->_HL.high(rr(_curr_reg->_HL.high()));
		break;
	case 0x1d:
		// RR L
		_curr_reg->_HL.low(rr(_curr_reg->_HL.low()));
		break;
	case 0x1e:
		// RR (HL)
		memory[_curr_reg->_HL.value()] = rr(memory[_curr_reg->_HL.value()]);
		break;
	case 0x1f:
		// RR A
		_curr_reg->_AF.high(rr(_curr_reg->_AF.high()));
		break;
	case 0x20:
		// SLA B
		_curr_reg->_BC.high(sla(_curr_reg->_BC.high()));
		break;
	case 0x21:
		// SLA C
		_curr_reg->_BC.low(sla(_curr_reg->_BC.low()));
		break;
	case 0x22:
		// SLA D
		_curr_reg->_DE.high(sla(_curr_reg->_DE.high()));
		break;
	case 0x23:
		// SLA E
		_curr_reg->_DE.low(sla(_curr_reg->_DE.low()));
		break;
	case 0x24:
		// SLA H
		_curr_reg->_HL.high(sla(_curr_reg->_HL.high()));
		break;
	case 0x25:
		// SLA L
		_curr_reg->_HL.low(sla(_curr_reg->_HL.low()));
		break;
	case 0x26:
		// SLA (HL)
		memory[_curr_reg->_HL.value()] = sla(memory[_curr_reg->_HL.value()]);
		break;
	case 0x27:
		// SLA A
		_curr_reg->_AF.high(sla(_curr_reg->_AF.high()));
		break;
	case 0x28:
		// SRA B
		_curr_reg->_BC.high(sra(_curr_reg->_BC.high()));
		break;
	case 0x29:
		// SRA C
		_curr_reg->_BC.low(sra(_curr_reg->_BC.low()));
		break;
	case 0x2a:
		// SRA D
		_curr_reg->_DE.high(sra(_curr_reg->_DE.high()));
		break;
	case 0x2b:
		// SRA E
		_curr_reg->_DE.low(sra(_curr_reg->_DE.low()));
		break;
	case 0x2c:
		// SRA H
		_curr_reg->_HL.high(sra(_curr_reg->_HL.high()));
		break;
	case 0x2d:
		// SRA L
		_curr_reg->_HL.low(sra(_curr_reg->_HL.low()));
		break;
	case 0x2e:
		// SRA (HL)
		memory[_curr_reg->_HL.value()] = sra(memory[_curr_reg->_HL.value()]);
		break;
	case 0x2f:
		// SRA A
		_curr_reg->_AF.high(sra(_curr_reg->_AF.high()));
		break;
	case 0x30:
		// SLL B
		_curr_reg->_BC.high(sll(_curr_reg->_BC.high()));
		break;
	case 0x31:
		// SLL C
		_curr_reg->_BC.low(sll(_curr_reg->_BC.low()));
		break;
	case 0x32:
		// SLL D
		_curr_reg->_DE.high(sll(_curr_reg->_DE.high()));
		break;
	case 0x33:
		// SLL E
		_curr_reg->_DE.low(sll(_curr_reg->_DE.low()));
		break;
	case 0x34:
		// SLL H
		_curr_reg->_HL.high(sll(_curr_reg->_HL.high()));
		break;
	case 0x35:
		// SLL L
		_curr_reg->_HL.low(sll(_curr_reg->_HL.low()));
		break;
	case 0x36:
		// SLL (HL)
		memory[_curr_reg->_HL.value()] = sll(memory[_curr_reg->_HL.value()]);
		break;
	case 0x37:
		// SLL A
		_curr_reg->_AF.high(sll(_curr_reg->_AF.high()));
		break;
	case 0x38:
		// SRL B
		_curr_reg->_BC.high(srl(_curr_reg->_BC.high()));
		break;
	case 0x39:
		// SRL C
		_curr_reg->_BC.low(srl(_curr_reg->_BC.low()));
		break;
	case 0x3a:
		// SRL D
		_curr_reg->_DE.high(srl(_curr_reg->_DE.high()));
		break;
	case 0x3b:
		// SRL E
		_curr_reg->_DE.low(srl(_curr_reg->_DE.low()));
		break;
	case 0x3c:
		// SRL H
		_curr_reg->_HL.high(srl(_curr_reg->_HL.high()));
		break;
	case 0x3d:
		// SRL L
		_curr_reg->_HL.low(srl(_curr_reg->_HL.low()));
		break;
	case 0x3e:
		// SRL (HL)
		memory[_curr_reg->_HL.value()] = srl(memory[_curr_reg->_HL.value()]);
		break;
	case 0x3f:
		// SRL A
		_curr_reg->_AF.high(srl(_curr_reg->_AF.high()));
		break;
	case 0x40:
		// BIT 0, B
		bit(_curr_reg->_BC.high() & 1);
		break;
	case 0x41:
		// BIT 0, C
		bit(_curr_reg->_BC.low() & 1);
		break;
	case 0x42:
		// BIT 0, D
		bit(_curr_reg->_DE.high() & 1);
		break;
	case 0x43:
		// BIT 0, E
		bit(_curr_reg->_DE.low() & 1);
		break;
	case 0x44:
		// BIT 0, H
		bit(_curr_reg->_HL.high() & 1);
		break;
	case 0x45:
		// BIT 0, L
		bit(_curr_reg->_HL.low() & 1);
		break;
	case 0x46:
		// BIT 0, (HL)
		bit(memory[_curr_reg->_HL.value()] & 1);
		break;
	case 0x47:
		// BIT 0, A
		bit(_curr_reg->_AF.high() & 1);
		break;
	case 0x48:
		// BIT 1, B
		bit(_curr_reg->_BC.high() & 2);
		break;
	case 0x49:
		// BIT 1, C
		bit(_curr_reg->_BC.low() & 2);
		break;
	case 0x4a:
		// BIT 1, D
		bit(_curr_reg->_DE.high() & 2);
		break;
	case 0x4b:
		// BIT 1, E
		bit(_curr_reg->_DE.low() & 2);
		break;
	case 0x4c:
		// BIT 1, H
		bit(_curr_reg->_HL.high() & 2);
		break;
	case 0x4d:
		// BIT 1, L
		bit(_curr_reg->_HL.low() & 2);
		break;
	case 0x4e:
		// BIT 1, (HL)
		bit(memory[_curr_reg->_HL.value()] & 2);
		break;
	case 0x4f:
		// BIT 1, A
		bit(_curr_reg->_AF.high() & 2);
		break;
	case 0x50:
		// BIT 2, B
		bit(_curr_reg->_BC.high() & 4);
		break;
	case 0x51:
		// BIT 2, C
		bit(_curr_reg->_BC.low() & 4);
		break;
	case 0x52:
		// BIT 2, D
		bit(_curr_reg->_DE.high() & 4);
		break;
	case 0x53:
		// BIT 2, E
		bit(_curr_reg->_DE.low() & 4);
		break;
	case 0x54:
		// BIT 2, H
		bit(_curr_reg->_HL.high() & 4);
		break;
	case 0x55:
		// BIT 2, L
		bit(_curr_reg->_HL.low() & 4);
		break;
	case 0x56:
		// BIT 2, (HL)
		bit(memory[_curr_reg->_HL.value()] & 4);
		break;
	case 0x57:
		// BIT 2, A
		bit(_curr_reg->_AF.high() & 4);
		break;
	case 0x58:
		// BIT 3, B
		bit(_curr_reg->_BC.high() & 8);
		break;
	case 0x59:
		// BIT 3, C
		bit(_curr_reg->_BC.low() & 8);
		break;
	case 0x5a:
		// BIT 3, D
		bit(_curr_reg->_DE.high() & 8);
		break;
	case 0x5b:
		// BIT 3, E
		bit(_curr_reg->_DE.low() & 8);
		break;
	case 0x5c:
		// BIT 3, H
		bit(_curr_reg->_HL.high() & 8);
		break;
	case 0x5d:
		// BIT 3, L
		bit(_curr_reg->_HL.low() & 8);
		break;
	case 0x5e:
		// BIT 3, (HL)
		bit(memory[_curr_reg->_HL.value()] & 8);
		break;
	case 0x5f:
		// BIT 3, A
		bit(_curr_reg->_AF.high() & 8);
		break;
	case 0x60:
		// BIT 4, B
		bit(_curr_reg->_BC.high() & 0x10);
		break;
	case 0x61:
		// BIT 4, C
		bit(_curr_reg->_BC.low() & 0x10);
		break;
	case 0x62:
		// BIT 4, D
		bit(_curr_reg->_DE.high() & 0x10);
		break;
	case 0x63:
		// BIT 4, E
		bit(_curr_reg->_DE.low() & 0x10);
		break;
	case 0x64:
		// BIT 4, H
		bit(_curr_reg->_HL.high() & 0x10);
		break;
	case 0x65:
		// BIT 4, L
		bit(_curr_reg->_HL.low() & 0x10);
		break;
	case 0x66:
		// BIT 4, (HL)
		bit(memory[_curr_reg->_HL.value()] & 0x10);
		break;
	case 0x67:
		// BIT 4, A
		bit(_curr_reg->_AF.high() & 0x10);
		break;
	case 0x68:
		// BIT 5, B
		bit(_curr_reg->_BC.high() & 0x20);
		break;
	case 0x69:
		// BIT 5, C
		bit(_curr_reg->_BC.low() & 0x20);
		break;
	case 0x6a:
		// BIT 5, D
		bit(_curr_reg->_DE.high() & 0x20);
		break;
	case 0x6b:
		// BIT 5, E
		bit(_curr_reg->_DE.low() & 0x20);
		break;
	case 0x6c:
		// BIT 5, H
		bit(_curr_reg->_HL.high() & 0x20);
		break;
	case 0x6d:
		// BIT 5, L
		bit(_curr_reg->_HL.low() & 0x20);
		break;
	case 0x6e:
		// BIT 5, (HL)
		bit(memory[_curr_reg->_HL.value()] & 0x20);
		break;
	case 0x6f:
		// BIT 5, A
		bit(_curr_reg->_AF.high() & 0x20);
		break;
	case 0x70:
		// BIT 6, B
		bit(_curr_reg->_BC.high() & 0x40);
		break;
	case 0x71:
		// BIT 6, C
		bit(_curr_reg->_BC.low() & 0x40);
		break;
	case 0x72:
		// BIT 6, D
		bit(_curr_reg->_DE.high() & 0x40);
		break;
	case 0x73:
		// BIT 6, E
		bit(_curr_reg->_DE.low() & 0x40);
		break;
	case 0x74:
		// BIT 6, H
		bit(_curr_reg->_HL.high() & 0x40);
		break;
	case 0x75:
		// BIT 6, L
		bit(_curr_reg->_HL.low() & 0x40);
		break;
	case 0x76:
		// BIT 6, (HL)
		bit(memory[_curr_reg->_HL.value()] & 0x40);
		break;
	case 0x77:
		// BIT 6, A
		bit(_curr_reg->_AF.high() & 0x40);
		break;
	case 0x78:
		// BIT 7, B
		bit(_curr_reg->_BC.high() & 0x80);
		break;
	case 0x79:
		// BIT 7, C
		bit(_curr_reg->_BC.low() & 0x80);
		break;
	case 0x7a:
		// BIT 7, D
		bit(_curr_reg->_DE.high() & 0x80);
		break;
	case 0x7b:
		// BIT 7, E
		bit(_curr_reg->_DE.low() & 0x80);
		break;
	case 0x7c:
		// BIT 7, H
		bit(_curr_reg->_HL.high() & 0x80);
		break;
	case 0x7d:
		// BIT 7, L
		bit(_curr_reg->_HL.low() & 0x80);
		break;
	case 0x7e:
		// BIT 7, (HL)
		bit(memory[_curr_reg->_HL.value()] & 0x80);
		break;
	case 0x7f:
		// BIT 7, A
		bit(_curr_reg->_AF.high() & 0x80);
		break;
	case 0x80:
		// RES 0, B
		_curr_reg->_BC.high(_curr_reg->_BC.high() & ~1);
		// No flags affected
		break;
	case 0x81:
		// RES 0, C
		_curr_reg->_BC.low(_curr_reg->_BC.low() & ~1);
		// No flags affected
		break;
	case 0x82:
		// RES 0, D
		_curr_reg->_DE.high(_curr_reg->_DE.high() & ~1);
		// No flags affected
		break;
	case 0x83:
		// RES 0, E
		_curr_reg->_DE.low(_curr_reg->_DE.low() & ~1);
		// No flags affected
		break;
	case 0x84:
		// RES 0, H
		_curr_reg->_HL.high(_curr_reg->_HL.high() & ~1);
		// No flags affected
		break;
	case 0x85:
		// RES 0, L
		_curr_reg->_HL.low(_curr_reg->_HL.low() & ~1);
		// No flags affected
		break;
	case 0x86:
		// RES 0, (HL)
		memory[_curr_reg->_HL.value()] &= ~1;
		// No flags affected
		break;
	case 0x87:
		// RES 0, A
		_curr_reg->_AF.high(_curr_reg->_AF.high() & ~1);
		// No flags affected
		break;
	case 0x88:
		// RES 1, B
		_curr_reg->_BC.high(_curr_reg->_BC.high() & ~2);
		// No flags affected
		break;
	case 0x89:
		// RES 1, C
		_curr_reg->_BC.low(_curr_reg->_BC.low() & ~2);
		// No flags affected
		break;
	case 0x8a:
		// RES 1, D
		_curr_reg->_DE.high(_curr_reg->_DE.high() & ~2);
		// No flags affected
		break;
	case 0x8b:
		// RES 1, E
		_curr_reg->_DE.low(_curr_reg->_DE.low() & ~2);
		// No flags affected
		break;
	case 0x8c:
		// RES 1, H
		_curr_reg->_HL.high(_curr_reg->_HL.high() & ~2);
		// No flags affected
		break;
	case 0x8d:
		// RES 1, L
		_curr_reg->_HL.low(_curr_reg->_HL.low() & ~2);
		// No flags affected
		break;
	case 0x8e:
		// RES 1, (HL)
		memory[_curr_reg->_HL.value()] &= ~2;
		// No flags affected
		break;
	case 0x8f:
		// RES 1, A
		_curr_reg->_AF.high(_curr_reg->_AF.high() & ~2);
		// No flags affected
		break;
	case 0x90:
		// RES 2, B
		_curr_reg->_BC.high(_curr_reg->_BC.high() & ~4);
		// No flags affected
		break;
	case 0x91:
		// RES 2, C
		_curr_reg->_BC.low(_curr_reg->_BC.low() & ~4);
		// No flags affected
		break;
	case 0x92:
		// RES 2, D
		_curr_reg->_DE.high(_curr_reg->_DE.high() & ~4);
		// No flags affected
		break;
	case 0x93:
		// RES 2, E
		_curr_reg->_DE.low(_curr_reg->_DE.low() & ~4);
		// No flags affected
		break;
	case 0x94:
		// RES 2, H
		_curr_reg->_HL.high(_curr_reg->_HL.high() & ~4);
		// No flags affected
		break;
	case 0x95:
		// RES 2, L
		_curr_reg->_HL.low(_curr_reg->_HL.low() & ~4);
		// No flags affected
		break;
	case 0x96:
		// RES 2, (HL)
		memory[_curr_reg->_HL.value()] &= ~4;
		// No flags affected
		break;
	case 0x97:
		// RES 2, A
		_curr_reg->_AF.high(_curr_reg->_AF.high() & ~4);
		// No flags affected
		break;
	case 0x98:
		// RES 3, B
		_curr_reg->_BC.high(_curr_reg->_BC.high() & ~8);
		// No flags affected
		break;
	case 0x99:
		// RES 3, C
		_curr_reg->_BC.low(_curr_reg->_BC.low() & ~8);
		// No flags affected
		break;
	case 0x9a:
		// RES 3, D
		_curr_reg->_DE.high(_curr_reg->_DE.high() & ~8);
		// No flags affected
		break;
	case 0x9b:
		// RES 3, E
		_curr_reg->_DE.low(_curr_reg->_DE.low() & ~8);
		// No flags affected
		break;
	case 0x9c:
		// RES 3, H
		_curr_reg->_HL.high(_curr_reg->_HL.high() & ~8);
		// No flags affected
		break;
	case 0x9d:
		// RES 3, L
		_curr_reg->_HL.low(_curr_reg->_HL.low() & ~8);
		// No flags affected
		break;
	case 0x9e:
		// RES 3, (HL)
		memory[_curr_reg->_HL.value()] &= ~8;
		// No flags affected
		break;
	case 0x9f:
		// RES 3, A
		_curr_reg->_AF.high(_curr_reg->_AF.high() & ~8);
		// No flags affected
		break;
	case 0xa0:
		// RES 4, B
		_curr_reg->_BC.high(_curr_reg->_BC.high() & ~0x10);
		// No flags affected
		break;
	case 0xa1:
		// RES 4, C
		_curr_reg->_BC.low(_curr_reg->_BC.low() & ~0x10);
		// No flags affected
		break;
	case 0xa2:
		// RES 4, D
		_curr_reg->_DE.high(_curr_reg->_DE.high() & ~0x10);
		// No flags affected
		break;
	case 0xa3:
		// RES 4, E
		_curr_reg->_DE.low(_curr_reg->_DE.low() & ~0x10);
		// No flags affected
		break;
	case 0xa4:
		// RES 4, H
		_curr_reg->_HL.high(_curr_reg->_HL.high() & ~0x10);
		// No flags affected
		break;
	case 0xa5:
		// RES 4, L
		_curr_reg->_HL.low(_curr_reg->_HL.low() & ~0x10);
		// No flags affected
		break;
	case 0xa6:
		// RES 4, (HL)
		memory[_curr_reg->_HL.value()] &= ~0x10;
		// No flags affected
		break;
	case 0xa7:
		// RES 4, A
		_curr_reg->_AF.high(_curr_reg->_AF.high() & ~0x10);
		// No flags affected
		break;
	case 0xa8:
		// RES 5, B
		_curr_reg->_BC.high(_curr_reg->_BC.high() & ~0x20);
		// No flags affected
		break;
	case 0xa9:
		// RES 5, C
		_curr_reg->_BC.low(_curr_reg->_BC.low() & ~0x20);
		// No flags affected
		break;
	case 0xaa:
		// RES 5, D
		_curr_reg->_DE.high(_curr_reg->_DE.high() & ~0x20);
		// No flags affected
		break;
	case 0xab:
		// RES 5, E
		_curr_reg->_DE.low(_curr_reg->_DE.low() & ~0x20);
		// No flags affected
		break;
	case 0xac:
		// RES 5, H
		_curr_reg->_HL.high(_curr_reg->_HL.high() & ~0x20);
		// No flags affected
		break;
	case 0xad:
		// RES 5, L
		_curr_reg->_HL.low(_curr_reg->_HL.low() & ~0x20);
		// No flags affected
		break;
	case 0xae:
		// RES 5, (HL)
		memory[_curr_reg->_HL.value()] &= ~0x20;
		// No flags affected
		break;
	case 0xaf:
		// RES 5, A
		_curr_reg->_AF.high(_curr_reg->_AF.high() & ~0x20);
		// No flags affected
		break;
	case 0xb0:
		// RES 6, B
		_curr_reg->_BC.high(_curr_reg->_BC.high() & ~0x40);
		// No flags affected
		break;
	case 0xb1:
		// RES 6, C
		_curr_reg->_BC.low(_curr_reg->_BC.low() & ~0x40);
		// No flags affected
		break;
	case 0xb2:
		// RES 6, D
		_curr_reg->_DE.high(_curr_reg->_DE.high() & ~0x40);
		// No flags affected
		break;
	case 0xb3:
		// RES 6, E
		_curr_reg->_DE.low(_curr_reg->_DE.low() & ~0x40);
		// No flags affected
		break;
	case 0xb4:
		// RES 6, H
		_curr_reg->_HL.high(_curr_reg->_HL.high() & ~0x40);
		// No flags affected
		break;
	case 0xb5:
		// RES 6, L
		_curr_reg->_HL.low(_curr_reg->_HL.low() & ~0x40);
		// No flags affected
		break;
	case 0xb6:
		// RES 6, (HL)
		memory[_curr_reg->_HL.value()] &= ~0x40;
		// No flags affected
		break;
	case 0xb7:
		// RES 6, A
		_curr_reg->_AF.high(_curr_reg->_AF.high() & ~0x40);
		// No flags affected
		break;
	case 0xb8:
		// RES 7, B
		_curr_reg->_BC.high(_curr_reg->_BC.high() & ~0x80);
		// No flags affected
		break;
	case 0xb9:
		// RES 7, C
		_curr_reg->_BC.low(_curr_reg->_BC.low() & ~0x80);
		// No flags affected
		break;
	case 0xba:
		// RES 7, D
		_curr_reg->_DE.high(_curr_reg->_DE.high() & ~0x80);
		// No flags affected
		break;
	case 0xbb:
		// RES 7, E
		_curr_reg->_DE.low(_curr_reg->_DE.low() & ~0x80);
		// No flags affected
		break;
	case 0xbc:
		// RES 7, H
		_curr_reg->_HL.high(_curr_reg->_HL.high() & ~0x80);
		// No flags affected
		break;
	case 0xbd:
		// RES 7, L
		_curr_reg->_HL.low(_curr_reg->_HL.low() & ~0x80);
		// No flags affected
		break;
	case 0xbe:
		// RES 7, (HL)
		memory[_curr_reg->_HL.value()] &= ~0x80;
		// No flags affected
		break;
	case 0xbf:
		// RES 7, A
		_curr_reg->_AF.high(_curr_reg->_AF.high() & ~0x80);
		// No flags affected
		break;
	case 0xc0:
		// SET 0, B
		_curr_reg->_BC.high(_curr_reg->_BC.high() | 1);
		// No flags affected
		break;
	case 0xc1:
		// SET 0, C
		_curr_reg->_BC.low(_curr_reg->_BC.low() | 1);
		// No flags affected
		break;
	case 0xc2:
		// SET 0, D
		_curr_reg->_DE.high(_curr_reg->_DE.high() | 1);
		// No flags affected
		break;
	case 0xc3:
		// SET 0, E
		_curr_reg->_DE.low(_curr_reg->_DE.low() | 1);
		// No flags affected
		break;
	case 0xc4:
		// SET 0, H
		_curr_reg->_HL.high(_curr_reg->_HL.high() | 1);
		// No flags affected
		break;
	case 0xc5:
		// SET 0, L
		_curr_reg->_HL.low(_curr_reg->_HL.low() | 1);
		// No flags affected
		break;
	case 0xc6:
		// SET 0, (HL)
		memory[_curr_reg->_HL.value()] |= 1;
		// No flags affected
		break;
	case 0xc7:
		// SET 0, A
		_curr_reg->_AF.high(_curr_reg->_AF.high() | 1);
		// No flags affected
		break;
	case 0xc8:
		// SET 1, B
		_curr_reg->_BC.high(_curr_reg->_BC.high() | 2);
		// No flags affected
		break;
	case 0xc9:
		// SET 1, C
		_curr_reg->_BC.low(_curr_reg->_BC.low() | 2);
		// No flags affected
		break;
	case 0xca:
		// SET 1, D
		_curr_reg->_DE.high(_curr_reg->_DE.high() | 2);
		// No flags affected
		break;
	case 0xcb:
		// SET 1, E
		_curr_reg->_DE.low(_curr_reg->_DE.low() | 2);
		// No flags affected
		break;
	case 0xcc:
		// SET 1, H
		_curr_reg->_HL.high(_curr_reg->_HL.high() | 2);
		// No flags affected
		break;
	case 0xcd:
		// SET 1, L
		_curr_reg->_HL.low(_curr_reg->_HL.low() | 2);
		// No flags affected
		break;
	case 0xce:
		// SET 1, (HL)
		memory[_curr_reg->_HL.value()] |= 2;
		// No flags affected
		break;
	case 0xcf:
		// SET 1, A
		_curr_reg->_AF.high(_curr_reg->_AF.high() | 2);
		// No flags affected
		break;
	case 0xd0:
		// SET 2, B
		_curr_reg->_BC.high(_curr_reg->_BC.high() | 4);
		// No flags affected
		break;
	case 0xd1:
		// SET 2, C
		_curr_reg->_BC.low(_curr_reg->_BC.low() | 4);
		// No flags affected
		break;
	case 0xd2:
		// SET 2, D
		_curr_reg->_DE.high(_curr_reg->_DE.high() | 4);
		// No flags affected
		break;
	case 0xd3:
		// SET 2, E
		_curr_reg->_DE.low(_curr_reg->_DE.low() | 4);
		// No flags affected
		break;
	case 0xd4:
		// SET 2, H
		_curr_reg->_HL.high(_curr_reg->_HL.high() | 4);
		// No flags affected
		break;
	case 0xd5:
		// SET 2, L
		_curr_reg->_HL.low(_curr_reg->_HL.low() | 4);
		// No flags affected
		break;
	case 0xd6:
		// SET 2, (HL)
		memory[_curr_reg->_HL.value()] |= 4;
		// No flags affected
		break;
	case 0xd7:
		// SET 2, A
		_curr_reg->_AF.high(_curr_reg->_AF.high() | 4);
		// No flags affected
		break;
	case 0xd8:
		// SET 3, B
		_curr_reg->_BC.high(_curr_reg->_BC.high() | 8);
		// No flags affected
		break;
	case 0xd9:
		// SET 3, C
		_curr_reg->_BC.low(_curr_reg->_BC.low() | 8);
		// No flags affected
		break;
	case 0xda:
		// SET 3, D
		_curr_reg->_DE.high(_curr_reg->_DE.high() | 8);
		// No flags affected
		break;
	case 0xdb:
		// SET 3, E
		_curr_reg->_DE.low(_curr_reg->_DE.low() | 8);
		// No flags affected
		break;
	case 0xdc:
		// SET 3, H
		_curr_reg->_HL.high(_curr_reg->_HL.high() | 8);
		// No flags affected
		break;
	case 0xdd:
		// SET 3, L
		_curr_reg->_HL.low(_curr_reg->_HL.low() | 8);
		// No flags affected
		break;
	case 0xde:
		// SET 3, (HL)
		memory[_curr_reg->_HL.value()] |= 8;
		// No flags affected
		break;
	case 0xdf:
		// SET 3, A
		_curr_reg->_AF.high(_curr_reg->_AF.high() | 8);
		// No flags affected
		break;
	case 0xe0:
		// SET 4, B
		_curr_reg->_BC.high(_curr_reg->_BC.high() | 0x10);
		// No flags affected
		break;
	case 0xe1:
		// SET 4, C
		_curr_reg->_BC.low(_curr_reg->_BC.low() | 0x10);
		// No flags affected
		break;
	case 0xe2:
		// SET 4, D
		_curr_reg->_DE.high(_curr_reg->_DE.high() | 0x10);
		// No flags affected
		break;
	case 0xe3:
		// SET 4, E
		_curr_reg->_DE.low(_curr_reg->_DE.low() | 0x10);
		// No flags affected
		break;
	case 0xe4:
		// SET 4, H
		_curr_reg->_HL.high(_curr_reg->_HL.high() | 0x10);
		// No flags affected
		break;
	case 0xe5:
		// SET 4, L
		_curr_reg->_HL.low(_curr_reg->_HL.low() | 0x10);
		// No flags affected
		break;
	case 0xe6:
		// SET 4, (HL)
		memory[_curr_reg->_HL.value()] |= 0x10;
		// No flags affected
		break;
	case 0xe7:
		// SET 4, A
		_curr_reg->_AF.high(_curr_reg->_AF.high() | 0x10);
		// No flags affected
		break;
	case 0xe8:
		// SET 5, B
		_curr_reg->_BC.high(_curr_reg->_BC.high() | 0x20);
		// No flags affected
		break;
	case 0xe9:
		// SET 5, C
		_curr_reg->_BC.low(_curr_reg->_BC.low() | 0x20);
		// No flags affected
		break;
	case 0xea:
		// SET 5, D
		_curr_reg->_DE.high(_curr_reg->_DE.high() | 0x20);
		// No flags affected
		break;
	case 0xeb:
		// SET 5, E
		_curr_reg->_DE.low(_curr_reg->_DE.low() | 0x20);
		// No flags affected
		break;
	case 0xec:
		// SET 5, H
		_curr_reg->_HL.high(_curr_reg->_HL.high() | 0x20);
		// No flags affected
		break;
	case 0xed:
		// SET 5, L
		_curr_reg->_HL.low(_curr_reg->_HL.low() | 0x20);
		// No flags affected
		break;
	case 0xee:
		// SET 5, (HL)
		memory[_curr_reg->_HL.value()] |= 0x20;
		// No flags affected
		break;
	case 0xef:
		// SET 5, A
		_curr_reg->_AF.high(_curr_reg->_AF.high() | 0x20);
		// No flags affected
		break;
	case 0xf0:
		// SET 6, B
		_curr_reg->_BC.high(_curr_reg->_BC.high() | 0x40);
		// No flags affected
		break;
	case 0xf1:
		// SET 6, C
		_curr_reg->_BC.low(_curr_reg->_BC.low() | 0x40);
		// No flags affected
		break;
	case 0xf2:
		// SET 6, D
		_curr_reg->_DE.high(_curr_reg->_DE.high() | 0x40);
		// No flags affected
		break;
	case 0xf3:
		// SET 6, E
		_curr_reg->_DE.low(_curr_reg->_DE.low() | 0x40);
		// No flags affected
		break;
	case 0xf4:
		// SET 6, H
		_curr_reg->_HL.high(_curr_reg->_HL.high() | 0x40);
		// No flags affected
		break;
	case 0xf5:
		// SET 6, L
		_curr_reg->_HL.low(_curr_reg->_HL.low() | 0x40);
		// No flags affected
		break;
	case 0xf6:
		// SET 6, (HL)
		memory[_curr_reg->_HL.value()] |= 0x40;
		// No flags affected
		break;
	case 0xf7:
		// SET 6, A
		_curr_reg->_AF.high(_curr_reg->_AF.high() | 0x40);
		// No flags affected
		break;
	case 0xf8:
		// SET 7, B
		_curr_reg->_BC.high(_curr_reg->_BC.high() | 0x80);
		// No flags affected
		break;
	case 0xf9:
		// SET 7, C
		_curr_reg->_BC.low(_curr_reg->_BC.low() | 0x80);
		// No flags affected
		break;
	case 0xfa:
		// SET 7, D
		_curr_reg->_DE.high(_curr_reg->_DE.high() | 0x80);
		// No flags affected
		break;
	case 0xfb:
		// SET 7, E
		_curr_reg->_DE.low(_curr_reg->_DE.low() | 0x80);
		// No flags affected
		break;
	case 0xfc:
		// SET 7, H
		_curr_reg->_HL.high(_curr_reg->_HL.high() | 0x80);
		// No flags affected
		break;
	case 0xfd:
		// SET 7, L
		_curr_reg->_HL.low(_curr_reg->_HL.low() | 0x80);
		// No flags affected
		break;
	case 0xfe:
		// SET 7, (HL)
		memory[_curr_reg->_HL.value()] |= 0x80;
		// No flags affected
		break;
	case 0xff:
		// SET 7, A
		_curr_reg->_AF.high(_curr_reg->_AF.high() | 0x80);
		// No flags affected
		break;
	}

	++_PC;
}

void z80::call(uint8_t* memory)
{
	_SP.value() -= 2;
	poke_word(&memory[_SP.value()], _PC + 3);
	_PC = peek_word(&memory[++_PC]);
}

void z80::jp(const uint8_t* memory)
{
	_PC = peek_word(&memory[++_PC]);
}

void z80::jp_register(const uint8_t* memory, const reg& source)
{
	_PC = peek_word(&memory[source.value()]);
}

void z80::jr(const uint8_t* memory)
{
	const int8_t offset = memory[++_PC];

	++_PC;
	_PC += offset;
}

void z80::pop(const uint8_t* memory, reg& dest)
{
	dest.value(&memory[_SP.value()]);
	_SP.value() += 2;
}

void z80::push(uint8_t* memory, reg& dest)
{
	_SP.value() -= 2;
	poke_word(&memory[_SP.value()], dest.value());
}

void z80::ret(const uint8_t* memory)
{
	_PC = peek_word(&memory[_SP.value()]);
	_SP.value() += 2;
}

reg& z80::ixy(const char xy)
{
	if (xy == 'X')
		return _curr_reg->_IX;
	else
		return _curr_reg->_IY;
}

uint16_t& z80::ixy_value(const char xy)
{
	if (xy == 'X')
		return _curr_reg->_IX.value();
	else
		return _curr_reg->_IY.value();
}

void z80::ixy_value(const char xy, const uint8_t* memory)
{
	if (xy == 'X')
		_curr_reg->_IX.value(memory);
	else
		_curr_reg->_IY.value(memory);
}

uint8_t z80::ixy_high(const char xy) const
{
	if (xy == 'X')
		return _curr_reg->_IX.high();
	else
		return _curr_reg->_IY.high();
}

void z80::ixy_high(const char xy, const uint8_t val)
{
	if (xy == 'X')
		_curr_reg->_IX.high(val);
	else
		_curr_reg->_IY.high(val);
}

uint8_t z80::ixy_low(const char xy) const
{
	if (xy == 'X')
		return _curr_reg->_IX.low();
	else
		return _curr_reg->_IY.low();
}

void z80::ixy_low(const char xy, const uint8_t val)
{
	if (xy == 'X')
		_curr_reg->_IX.low(val);
	else
		_curr_reg->_IY.low(val);
}

const uint8_t* z80::peek_indirect(const uint8_t* memory)
{
	return &memory[peek_word(&memory[++_PC])];
}

uint16_t z80::peek_word(const uint8_t* memory) const
{
	uint16_t ret = *memory;

	ret |= *(memory + 1) << 8;
	return ret;
}

void z80::poke_register_indirect(uint8_t* memory, const reg& source)
{
	const uint16_t idx = peek_word(&memory[++_PC]);

	poke_word(&memory[idx], source.value());
}

void z80::poke_word(uint8_t* memory, const uint16_t value) const
{
	*memory = value & 0xff;
	*(memory + 1) = value >> 8;
}

void z80::rlca()
{
	uint8_t A = _curr_reg->_AF.high();
	uint8_t flags = _curr_reg->_AF.low();
	const bool carry = (A & 0x80) != 0;

	A <<= 1;

	if (carry)
	{
		A |= 1;
		flags |= FLAG::C;
	}
	else
	{
		A &= 0xFE;
		flags &= ~FLAG::C;
	}

	_curr_reg->_AF.high(A);
	flags &= ~(FLAG::H | FLAG::N);
	_curr_reg->_AF.low(flags);
}

void z80::rrca()
{
	uint8_t A = _curr_reg->_AF.high();
	uint8_t flags = _curr_reg->_AF.low();
	const bool carry = (A & 0x01) != 0;

	A >>= 1;

	if (carry)
	{
		A |= 0x80;
		flags |= FLAG::C;
	}
	else
	{
		A &= 0x7F;
		flags &= ~FLAG::C;
	}

	_curr_reg->_AF.high(A);
	flags &= ~(FLAG::H | FLAG::N);
	_curr_reg->_AF.low(flags);
}

void z80::add_a(const uint8_t rhs)
{
	uint8_t flags = 0;
	const uint16_t result = static_cast<uint16_t>(_curr_reg->_AF.high()) + rhs;

	if (result & 0x80)
		flags |= FLAG::S;

	if ((result & 0xff) == 0)
		flags |= FLAG::Z;

	if ((_curr_reg->_AF.high() ^ rhs ^ result) & 0x10)
		flags |= FLAG::H;

	if (~(_curr_reg->_AF.high() ^ rhs) & (_curr_reg->_AF.high() ^ result) & 0x80)
		flags |= FLAG::PV;

	if (result & 0x100)
		flags |= FLAG::C;

	_curr_reg->_AF.high(result & 0xff);
	_curr_reg->_AF.low(flags);
}

void z80::add(reg& lhs, const reg& rhs)
{
	const uint32_t result = static_cast<uint32_t>(lhs.value()) + rhs.value();
	const auto new_rr = static_cast<uint16_t>(result);
	uint8_t flags = _curr_reg->_AF.low() & (FLAG::S | FLAG::Z | FLAG::PV);

	if (((lhs.value() & 0x0FFF) + (rhs.value() & 0x0FFF)) & 0x1000)
		flags |= FLAG::H;

	if (result & 0x10000)
		flags |= FLAG::C;

	// Undocumented flags
	flags |= new_rr >> 8 & (FLAG::BIT3 | FLAG::BIT5);
	lhs.value(new_rr);
	_curr_reg->_AF.low(flags);
}

void z80::adc_a(const uint8_t rhs)
{
	uint8_t flags = 0;
	const uint8_t A = _curr_reg->_AF.high();
	const uint8_t carry = _curr_reg->_AF.low() & FLAG::C;
	const uint16_t result = A + rhs + carry;

	if (result & 0x80)
		flags |= FLAG::S;

	if ((result & 0xff) == 0)
		flags |= FLAG::Z;

	if ((A & 0x0F) + (rhs & 0x0F) + carry > 0x0F)
		flags |= FLAG::H;

	if (~(A ^ rhs) & (A ^ result) & 0x80)
		flags |= FLAG::PV;

	if (result & 0x100)
		flags |= FLAG::C;

	_curr_reg->_AF.high(result & 0xff);
	_curr_reg->_AF.low(flags);
}

void z80::adc_hl(const reg& rhs)
{
	uint32_t result = static_cast<uint32_t>(_curr_reg->_HL.value()) + rhs.value();
	uint8_t flags = 0;

	if (flags & FLAG::C)
		++result;

	if (result & 0x8000)
		flags |= FLAG::S;

	if ((result & 0xffff) == 0)
		flags |= FLAG::Z;

	if (((_curr_reg->_HL.value() & 0x0FFF) + (rhs.value() & 0x0FFF)) & 0x1000)
		flags |= FLAG::H;

	if (~(_curr_reg->_HL.value() ^ rhs.value()) &
		(_curr_reg->_HL.value() ^ result) & 0x8000)
	{
		flags |= FLAG::PV;
	}

	if (result & 0x10000)
		flags |= FLAG::C;

	_curr_reg->_HL.value(result & 0xffff);
	_curr_reg->_AF.low(flags);
}

void z80::cp_a(const uint8_t rhs)
{
	const uint16_t result = static_cast<uint16_t>(_curr_reg->_AF.high()) - rhs;
	uint8_t flags = 0;

	if (result & 0x80)
		flags |= FLAG::S;

	if ((result & 0xFF) == 0)
		flags |= FLAG::Z;

	if ((_curr_reg->_AF.high() & 0x0F) < (rhs & 0x0F))
		flags |= FLAG::H;

	if ((_curr_reg->_AF.high() ^ rhs) & (_curr_reg->_AF.high() ^ result) & 0x80)
		flags |= FLAG::PV;

	flags |= FLAG::N;

	if (_curr_reg->_AF.high() < rhs)
		flags |= FLAG::C;

	_curr_reg->_AF.low(flags);
}

void z80::sub_a(const uint8_t rhs)
{
	uint8_t flags = 0;
	const uint8_t A = _curr_reg->_AF.high();
	const uint16_t result = A - rhs;

	if (result & 0x80)
		flags |= FLAG::S;

	if ((result & 0xff) == 0)
		flags |= FLAG::Z;

	if ((A & 0x0F) < (rhs & 0x0F))
		flags |= FLAG::H;

	// Signed overflow
	if ((A ^ rhs) & (A ^ result) & 0x80)
		flags |= FLAG::PV;

	flags |= FLAG::N;

	if (A < rhs)
		flags |= FLAG::C;

	// Undocumented flags
	flags |= (result & 0xff) & (FLAG::BIT3 | FLAG::BIT5);
	_curr_reg->_AF.high(result & 0xff);
	_curr_reg->_AF.low(flags);
}

void z80::sbc_a(const uint8_t rhs)
{
	uint8_t flags = 0;
	const uint8_t A = _curr_reg->_AF.high();
	const uint8_t carry = _curr_reg->_AF.low() & FLAG::C;
	const uint16_t result = static_cast<uint16_t>(A) - rhs - carry;

	if (result & 0x80)
		flags |= FLAG::S;

	if ((result & 0xFF) == 0)
		flags |= FLAG::Z;

	if ((A & 0x0F) < ((rhs & 0x0F) + carry))
		flags |= FLAG::H;

	if ((A ^ rhs) & (A ^ result) & 0x80)
		flags |= FLAG::PV;

	flags |= FLAG::N;

	if (static_cast<uint16_t>(A) < static_cast<uint16_t>(rhs) + carry)
		flags |= FLAG::C;

	_curr_reg->_AF.high(result & 0xff);
	_curr_reg->_AF.low(flags);
}

void z80::sbc_hl(const reg& rhs)
{
	const uint8_t carry = _curr_reg->_AF.low() & FLAG::C;
	uint8_t flags = 0;
	uint32_t result = static_cast<uint32_t>(_curr_reg->_HL.value()) -
		_curr_reg->_BC.value() - carry;

	if (result & 0x8000)
		flags |= FLAG::S;

	if ((result & 0xffff) == 0)
		flags |= FLAG::Z;

	if ((_curr_reg->_HL.value() & 0x0FFF) < ((rhs.value() & 0x0FFF) + carry))
		flags |= FLAG::H;

	if ((_curr_reg->_HL.value() ^ rhs.value()) &
		(_curr_reg->_HL.value() ^ result) & 0x8000)
	{
		flags |= FLAG::PV;
	}

	flags |= FLAG::N;

	if (static_cast<uint32_t>(_curr_reg->_HL.value()) <
		static_cast<uint32_t>(rhs.value()) + carry)
		flags |= FLAG::C;

	_curr_reg->_HL.value(result & 0xffff);
	_curr_reg->_AF.low(flags);
}

uint8_t z80::inc(const uint8_t old_val)
{
	uint8_t flags = _curr_reg->_AF.low();
	const uint8_t new_val = old_val + 1;

	flags = (flags & FLAG::C) |
		(new_val & 0x80 ? FLAG::S : 0) |
		(new_val == 0 ? FLAG::Z : 0) |
		((old_val & 0x0F) == 0x0F ? FLAG::H : 0) |
		(old_val == 0x7F ? FLAG::PV : 0) |
		(new_val & (FLAG::BIT3 | FLAG::BIT5));
	_curr_reg->_AF.low(flags);
	return new_val;
}

uint8_t z80::dec(const uint8_t old_val)
{
	uint8_t flags = _curr_reg->_AF.low() & FLAG::C;
	const uint8_t new_val = old_val - 1;

	if (new_val & 0x80)
		flags |= FLAG::S;

	if (new_val == 0)
		flags |= FLAG::Z;

	if ((old_val & 0x0F) == 0)
		flags |= FLAG::H;

	if (old_val == 0x80)
		flags |= FLAG::PV;

	flags |= FLAG::N;
	_curr_reg->_AF.low(flags);
	return new_val;
}

void z80::and_a(const uint8_t val)
{
	const uint8_t A = _curr_reg->_AF.high() & val;

	_curr_reg->_AF.high(A);
	_curr_reg->_AF.low(logic_flags(A));
}

void z80::xor_a(const uint8_t val)
{
	const uint8_t A = _curr_reg->_AF.high() ^ val;

	_curr_reg->_AF.high(A);
	_curr_reg->_AF.low(logic_flags(A));
}

void z80::or_a(const uint8_t val)
{
	const uint8_t A = _curr_reg->_AF.high() | val;

	_curr_reg->_AF.high(A);
	_curr_reg->_AF.low(logic_flags(A));
}

void z80::bit(const bool set)
{
	uint8_t flags = _curr_reg->_AF.low() & ~FLAG::N;

	if (set)
		flags &= ~FLAG::Z;
	else
		flags |= FLAG::Z;

	flags |= FLAG::H;
	flags &= ~FLAG::N;
	_curr_reg->_AF.low(flags);
}

uint8_t z80::rla(uint8_t val)
{
	uint8_t flags = _curr_reg->_AF.low();
	const bool carry = (flags & FLAG::C) == FLAG::C;

	if (val & 0x80)
		flags |= FLAG::C;
	else
		flags &= ~FLAG::C;

	val <<= 1;

	if (carry)
		val |= 1;

	flags &= ~(FLAG::H | FLAG::N);
	_curr_reg->_AF.low(flags);
	return val;
}

uint8_t z80::rra(uint8_t val)
{
	uint8_t flags = _curr_reg->_AF.low();
	const bool carry = (flags & FLAG::C) == FLAG::C;

	if (val & 1)
		flags |= FLAG::C;
	else
		flags &= ~FLAG::C;

	val >>= 1;

	if (carry)
		val |= 0x80;

	flags &= ~(FLAG::H | FLAG::N);
	_curr_reg->_AF.low(flags);
	return val;
}

uint8_t z80::rlc(uint8_t val)
{
	uint8_t flags = 0;
		
	if (val & 0x80)
		flags |= FLAG::C;

	val <<= 1;

	if (flags & FLAG::C)
		val |= 1;

	if (val & 0x80)
		flags |= FLAG::S;

	if (val == 0)
		flags |= FLAG::Z;

	if (parity(val))
		flags |= FLAG::PV;

	_curr_reg->_AF.low(flags);
	return val;
}

uint8_t z80::rrc(uint8_t val)
{
	uint8_t flags = 0;

	if (val & 1)
		flags |= FLAG::C;

	val >>= 1;

	if (flags & FLAG::C)
		val |= 0x80;

	if (val & 0x80)
		flags |= FLAG::S;

	if (val == 0)
		flags |= FLAG::Z;

	if (parity(val))
		flags |= FLAG::PV;

	_curr_reg->_AF.low(flags);
	return val;
}

uint8_t z80::rl(uint8_t val)
{
	uint8_t flags = 0;
	bool carry = (flags & FLAG::C) == FLAG::C;

	if (val & 0x80)
		flags |= FLAG::C;

	val <<= 1;

	if (carry)
		val |= 1;

	if (val & 0x80)
		flags |= FLAG::S;

	if (val == 0)
		flags |= FLAG::Z;

	if (parity(val))
		flags |= FLAG::PV;

	_curr_reg->_AF.low(flags);
	return val;
}

uint8_t z80::rr(uint8_t val)
{
	uint8_t flags = 0;
	bool carry = (flags & FLAG::C) == FLAG::C;

	if (val & 1)
		flags |= FLAG::C;

	val >>= 1;

	if (carry)
		val |= 0x80;

	if (val & 0x80)
		flags |= FLAG::S;

	if (val == 0)
		flags |= FLAG::Z;

	if (parity(val))
		flags |= FLAG::PV;

	_curr_reg->_AF.low(flags);
	return val;
}

uint8_t z80::sla(uint8_t val)
{
	uint8_t flags = 0;

	if (val & 0x80)
		flags |= FLAG::C;

	val <<= 1;

	if (val & 0x80)
		flags |= FLAG::S;

	if (val == 0)
		flags |= FLAG::Z;

	if (parity(val))
		flags |= FLAG::PV;

	_curr_reg->_AF.low(flags);
	return val;
}

uint8_t z80::sra(uint8_t val)
{
	uint8_t flags = 0;
	const bool msb = (val & 0x80) == 0x80;

	if (val & 1)
		flags |= FLAG::C;

	val >>= 1;

	if (msb)
		val |= 0x80;

	if (val & 0x80)
		flags |= FLAG::S;

	if (val == 0)
		flags |= FLAG::Z;

	if (parity(val))
		flags |= FLAG::PV;

	_curr_reg->_AF.low(flags);
	return val;
}

uint8_t z80::sll(uint8_t val)
{
	uint8_t flags = 0;

	if (val & 0x80)
		flags |= FLAG::C;

	val <<= 1;
	val |= 1;

	if (val & 0x80)
		flags |= FLAG::S;

	if (val == 0)
		flags |= FLAG::Z;

	if (parity(val))
		flags |= FLAG::PV;

	_curr_reg->_AF.low(flags);
	return val;
}

uint8_t z80::srl(uint8_t val)
{
	uint8_t flags = 0;

	if (val & 1)
		flags |= FLAG::C;

	val >>= 1;

	if (val == 0)
		flags |= FLAG::Z;

	if (parity(val))
		flags |= FLAG::PV;

	_curr_reg->_AF.low(flags);
	return val;
}

uint8_t z80::logic_flags(const uint8_t A) const
{
	uint8_t flags = (A & 0x80 ? FLAG::S : 0) |
		(A == 0 ? FLAG::Z : 0) |
		FLAG::H |
		(parity(A) ? FLAG::PV : 0);

	return flags;
}

bool z80::parity(uint8_t value) const
{
	int count = 0;

	for (int i = 0; i < 8; ++i)
	{
		if (value & 1)
			++count;

		value >>= 1;
	}

	return (count % 2) == 0;
}
