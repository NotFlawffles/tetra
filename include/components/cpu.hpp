#pragma once

#include <array>
#include <functional>
#include "memory.hpp"
#include "ppu.hpp"
#include "register.hpp"

class CPU {
    public:
    std::array<unsigned short, Memory::SIZE> memory = {0};
    std::array<unsigned short, Memory::SIZE> previous = memory;

    std::array<unsigned short, Register::COUNT> registers = {
	0, 0, 0, 0, 0, Memory::Segment::TEXT, Memory::Segment::STACK, 0
    };

    unsigned short fetched, opcode, flags, a, b, c;
    PPU ppu;
    
    // base
    CPU(void);
    void reset(void);
    unsigned short fetch(void);
    void decode(void);
    void execute(void);
    void clock(void);
    void update_keyboard(void);
    void update_video(void);
    unsigned char run(void);
    void load_program(std::vector<unsigned short> program);
    void debug(void);

    // addressing modes
    void implied(void);
    void ab(void);
    void abc(void);
    void ao(void);

    // operations
    void nop(void);
    void mov(void);
    void _int(void);
    void drw(void);
    void jmp(void);
    void ld(void);
    void str(void);
    void cmp(void);
    void add(void);
    void sub(void);
    void mul(void);
    void div(void);
    void mod(void);
    void inc(void);
    void dec(void);
    void clr(void);
    void dly(void);

    struct Instruction {
	std::function<void(void)> addressing_mode;
	std::function<void(void)> operation;
    };

    const std::vector<Instruction> LOOKUP {
	{std::bind(&CPU::implied, this), std::bind(&CPU::nop, this)},
	{std::bind(&CPU::ab,  this), std::bind(&CPU::mov,  this)},
	{std::bind(&CPU::ao,  this), std::bind(&CPU::_int, this)},
	{std::bind(&CPU::abc, this), std::bind(&CPU::drw,  this)},
	{std::bind(&CPU::ao,  this), std::bind(&CPU::jmp,  this)},
	{std::bind(&CPU::ab,  this), std::bind(&CPU::ld,   this)},
	{std::bind(&CPU::ab,  this), std::bind(&CPU::str,  this)},
	{std::bind(&CPU::ab,  this), std::bind(&CPU::cmp,  this)},
	{std::bind(&CPU::ab,  this), std::bind(&CPU::add,  this)},
	{std::bind(&CPU::ab,  this), std::bind(&CPU::sub,  this)},
	{std::bind(&CPU::ab,  this), std::bind(&CPU::mul,  this)},
	{std::bind(&CPU::ab,  this), std::bind(&CPU::div,  this)},
	{std::bind(&CPU::ab,  this), std::bind(&CPU::mod,  this)},
	{std::bind(&CPU::ao,  this), std::bind(&CPU::inc,  this)},
	{std::bind(&CPU::ao,  this), std::bind(&CPU::dec,  this)},
	{std::bind(&CPU::implied, this), std::bind(&CPU::clr,  this)},
	{std::bind(&CPU::ao, this), std::bind(&CPU::dly,  this)},
    };

    void handle_interrupt(void); 
};
