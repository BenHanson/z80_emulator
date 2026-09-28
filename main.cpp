#include <iostream>
#include "../lexertl14/include/lexertl/memory_file.hpp"
#include <vector>
#include "z80_emulator.hpp"

int main()
{
	z80 emu;
	lexertl::memory_file mf(R"(D:\Ben\Dev\z80_assembler\sna\jsw.sna)");
	std::vector<unsigned char> memory(65536, 0);
	// .sna header is 27 bytes long
	constexpr std::size_t header_len = 27;
	const std::size_t offset = 16384 - header_len; // 16357

	memcpy(memory.data() + offset, mf.data(), 49179);
	mf.close();
	// Load Spectrum rom (overwrites .sna header, we don't care)
	mf.open(R"(D:\ZEsarUX_windows-12.0\48.rom)");
	memcpy(memory.data(), mf.data(), 16384);
	mf.close();
	emu._PC = 32765; // jsw entry point

	/*memory[0] = 22;
	memory[1] = 1;
	memory[2] = 21;
	memory[3] = 245;
	memory[4] = 209;
	memory[5] = 123;
	memory[6] = 50;
	memory[7] = 0;
	memory[8] = 91;
	memory[9] = 201;*/

	while (emu._PC != 0)
		emu.step(&memory.front());
}
