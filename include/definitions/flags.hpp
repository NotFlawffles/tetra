#pragma once

enum Flag {
    RUNNING            = 0b00000001,
    IMMEDIATE          = 0b00000010,
    ZERO               = 0b00000100,
    CARRY              = 0b00001000,
    OVERFLOW           = 0b00010000,
    SOFTWARE_INTERRUPT = 0b00100000,
    HARDWARE_INTERRUPT = 0b01000000,
    FLAGGED            = 0b10000000
};
