#pragma once

enum Instruction {
    OPCODE = 0x00FF,
    FLAGS  = 0xFF00
};

enum Opcode {
    NOP,
    MOV,
    INT,
    DRW,
    JMP,
    LD,
    STR,
    CMP,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    INC,
    DEC,
    CLR,
    DLY
};
