#include "../include/components/cpu.hpp"
#include "../include/components/register.hpp"
#include "../include/definitions/instruction.hpp"
#include "../include/definitions/graphics.hpp"
#include "../include/definitions/flags.hpp"
#include "../include/definitions/interrupts.hpp"

using enum Graphics::Colors;

int main(void) {
    CPU cpu;
    cpu.reset();

    cpu.load_program({
	0x0200 | MOV, A, 40,
	0x0200 | MOV, B, 45,
	0x0200 | MOV, C, 50,
	0x0200 | MOV, D, WHITE,
	0x0200 | MOV, E, BLUE,
	
	0x0200 | MOV, ST, RUNNING,
	CMP, A, C,
	0x8600 | MOV, A, EXIT,
	0x8600 | INT, SOFTWARE_INTERRUPT,

	CLR,
	DRW, A, B, D, // A = left.x,  B = left.y
	DRW, C, B, E, // C = right.x, B = right.y
	
	INC, A,
	DEC, D,
	0x0200 | CMP, D, BLACK,
	0x8600 | MOV, D, BLUE,
	0x0200 | DLY, 0xFFFF,

	0x0200 | JMP, 15
    });

    int code = cpu.run();
    cpu.debug();
    return code;
}
