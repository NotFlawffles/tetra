#include "../../include/components/cpu.hpp"
#include "../../include/components/register.hpp"
#include "../../include/definitions/instruction.hpp"
#include "../../include/definitions/flags.hpp"
#include "../../include/definitions/interrupts.hpp"
#include "../../include/definitions/graphics.hpp"
#include <iostream>
#include <unistd.h>

#define MIN(a, b) a > b ? a : b

CPU::CPU(void) {
    memory.fill(0);
    update_video();
}

void CPU::reset(void) {
    memory.fill(0);

    for (auto& block: memory) {
	block = 0;
    }

    registers = {0, 0, 0, 0, 0, Memory::Segment::TEXT, Memory::Segment::STACK, 0};
    flags = 0;
    a = 0;
    b = 0;
    opcode = 0;
}

unsigned short CPU::fetch(void) {
    fetched = memory[registers[Register::IP]++];
    return fetched;
}

void CPU::decode(void) {
    opcode = fetched & OPCODE;
    flags = (fetched & FLAGS) >> 0x8;
}

void CPU::execute(void) {
    auto* instruction = &LOOKUP[opcode];
    registers[Register::ST] |= flags & IMMEDIATE;
    (instruction->addressing_mode)();

    if (flags & FLAGGED && !(((flags ^ FLAGGED) >> 0x2) & (registers[Register::ST] >> 0x2))) {
	return;
    }

    (instruction->operation)();
}

void CPU::clock(void) {
    if (registers[Register::ST] & IMMEDIATE) {
	registers[Register::ST] ^= IMMEDIATE;
    }

    fetch();
    decode();
    execute();

    SDL_PollEvent(&ppu.event);

    if (ppu.event.type == SDL_QUIT) {
	registers[Register::A] = EXIT;
	registers[Register::ST] |= SOFTWARE_INTERRUPT;
    }

    if (ppu.event.type == SDL_KEYDOWN) {
	update_keyboard();
    }

    if (ppu.event.type == SDL_KEYUP) {
	SDL_ResetKeyboard();
	update_keyboard();
    }

    if (registers[Register::ST] & (SOFTWARE_INTERRUPT | HARDWARE_INTERRUPT)) {
	handle_interrupt();
    }

    if (opcode == DRW) {
	update_video();
	registers[Register::ST] ^= SOFTWARE_INTERRUPT;
    }
}

void CPU::update_keyboard(void) {
    registers[Register::ST] |= HARDWARE_INTERRUPT;
}

void CPU::update_video(void) {
    ppu.update_texture(&memory[Memory::Segment::VIDEO]);
    ppu.draw_texture();
}

unsigned char CPU::run(void) {
    registers[Register::ST] |= RUNNING;

    while (registers[Register::ST] & RUNNING) {
	clock();
    }

    return registers[Register::B];
}

void CPU::load_program(std::vector<unsigned short> program) {
    for (size_t i = 0; i < program.size(); i++) {
	memory[Memory::Segment::TEXT + i] = program[i];
    }
}

void CPU::debug(void) {
    std::cout << "A: " << registers[A] << std::endl
	      << "B: " << registers[B] << std::endl
	      << "C: " << registers[C] << std::endl
	      << "D: " << registers[D] << std::endl
	      << "E: " << registers[E] << std::endl
	      << "IP: " << registers[IP] << std::endl
	      << "SP: " << registers[SP] << std::endl
	      << "ST: " << registers[ST] << std::endl;
}

void CPU::implied(void) {}

void CPU::ab(void) {
    a = fetch();
    b = fetch();
}

void CPU::abc(void) {
    a = fetch();
    b = fetch();
    c = fetch();
}

void CPU::ao(void) {
    a = fetch();
}

void CPU::nop(void) {}

void CPU::mov(void) {
    registers[a] = registers[Register::ST] & IMMEDIATE ? b : registers[b];
}

void CPU::_int(void) {
    registers[Register::ST] |= a;
}

void CPU::drw(void) {
    bool immediate = registers[Register::ST] & IMMEDIATE;
    std::vector<unsigned short> coordinates = {(immediate ? a : registers[a]), (immediate ? b : registers[b])};

    if (!coordinates[0] || coordinates[0] >= 90 || !coordinates[1] || coordinates[1] >= 90) {
	return;
    }

    auto position = (coordinates[1] * 90) + coordinates[0];
    memory[Memory::Segment::VIDEO + position] = immediate ? c : registers[c];
}

void CPU::jmp(void) {
    registers[IP] = Memory::Segment::TEXT + (registers[Register::ST] & IMMEDIATE ? a : registers[a]);
}

void CPU::ld(void) {
    registers[a] = memory[b];
}

void CPU::str(void) {
    memory[registers[a]] = registers[Register::ST] & IMMEDIATE ? b : registers[b];
}

void CPU::cmp(void) {
    long long result = registers[a] - (registers[Register::ST] & IMMEDIATE ? b : registers[b]);
    registers[Register::ST] |= (result == 0) << 0x2;
    registers[Register::ST] |= (result > 0) << 0x3;
    registers[Register::ST] |= (result < 0) << 0x4;
}

void CPU::add(void) {
    registers[a] += registers[Register::ST] & IMMEDIATE ? b : registers[b];
}

void CPU::sub(void) {
    registers[a] -= registers[Register::ST] & IMMEDIATE ? b : registers[b];
}

void CPU::mul(void) {
    registers[a] *= registers[Register::ST] & IMMEDIATE ? b : registers[b];
}

void CPU::div(void) {
    registers[a] /= registers[Register::ST] & IMMEDIATE ? b : registers[b];
}

void CPU::mod(void) {
    registers[a] %= registers[Register::ST] & IMMEDIATE ? b : registers[b];
}

void CPU::inc(void) {
    registers[a]++;
}

void CPU::dec(void) {
    registers[a]--;
}

void CPU::clr(void) {
    for (int i = Memory::Segment::VIDEO; i < Memory::Segment::TEXT - 92; i++) {
	memory[i] = 0;
    }
}

void CPU::dly(void) {
    usleep(registers[Register::ST] & IMMEDIATE ? a : registers[a]);
}

void CPU::handle_interrupt(void) {
    if (registers[Register::ST] & SOFTWARE_INTERRUPT) {
	switch (registers[Register::A]) {
	    case RESET:
		break;

	    case EXIT:
		registers[Register::ST] ^= RUNNING;
	}

	registers[Register::ST] ^= SOFTWARE_INTERRUPT;
    }

    if (registers[Register::ST] & HARDWARE_INTERRUPT) {
	const unsigned char* keyboard_state = SDL_GetKeyboardState(nullptr);

	for (int i = 4; i < 255; i++) {
	    memory[i - 4] = keyboard_state[i];
	}

	registers[Register::ST] ^= HARDWARE_INTERRUPT;
    }
}
