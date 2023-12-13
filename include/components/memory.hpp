#pragma once

namespace Memory {
    const unsigned short SIZE = 0xFFFF;

    enum Segment {
	RESERVED = 0x0000,
	VIDEO    = 0x0200,
	TEXT     = 0x2200,
	DATA     = 0x4000,
	HEAP     = 0x8000,
	STACK    = 0xFFFF
    };
}
