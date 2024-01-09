#include "../include/components/cpu.hpp"
#include "../include/components/register.hpp"
#include "../include/definitions/flags.hpp"
#include "../include/definitions/interrupts.hpp"
#include "../include/definitions/graphics.hpp"
#include "../include/definitions/user/opcodes.hpp"
#include "../include/definitions/user/flags.hpp"

using enum Graphics::Colors;

int main(void) {
    CPU cpu;
    cpu.reset();

    cpu.load_program({
	IMM | MOV, A, 90/2 - 1,
	IMM | MOV, B, 90/2 - 1,
	IMM | MOV, C, WHITE,
	IMM | LD, D, 75,
	IMM | CMP, D, 1,
	FLG | ZER | INC, A,
	MOV, ST, RUNNING,
	IMM | LD, D, 76,
	IMM | CMP, D, 1,
	FLG | ZER | DEC, A,
	MOV, ST, RUNNING,
	IMM | LD, D, 77,
	IMM | CMP, D, 1,
	FLG | ZER | INC, B,
	MOV, ST, RUNNING,
	IMM | LD, D, 78,
	IMM | CMP, D, 1,
	FLG | ZER | DEC, B,
	MOV, ST, RUNNING,
	DRW, A, B, C,
	DLY, 16,
	CLR,
	IMM | JMP, 0
    });

    int code = cpu.run();
    cpu.debug();
    return code;
}
