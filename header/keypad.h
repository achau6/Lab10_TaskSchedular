#ifndef KEYPAD_H
#define KEYPAD_H
#include <bit.h>

unsigned char GetKeypadKey(){
        PORTC = 0xEF;
        asm("nop");
        if(GetBit(PINC, 0) == 0) { return('1'); }
        if(GetBit(PINC, 1) == 0) { return('4'); }
        if(GetBit(PINC, 2) == 0) { return('7'); }
        if(GetBit(PINC, 3) == 0) { return('*'); }

        PORTC = 0xDF;
        asm("nop");
        if(GetBit(PINC, 0) == 0) { return('2'); }
        if(GetBit(PINC, 1) == 0) { return('5'); }
        if(GetBit(PINC, 2) == 0) { return('8'); }
        if(GetBit(PINC, 3) == 0) { return('0'); }

        PORTC = 0xBF;
        asm("nop");
        if(GetBit(PINC, 0) == 0) { return('3'); }
        if(GetBit(PINC, 1) == 0) { return('6'); }
        if(GetBit(PINC, 2) == 0) { return('9'); }
        if(GetBit(PINC, 3) == 0) { return('#'); }

        PORTC = 0x7F;
        asm("nop");
        if(GetBit(PINC, 0) == 0) { return('A'); }
        if(GetBit(PINC, 1) == 0) { return('B'); }
        if(GetBit(PINC, 2) == 0) { return('C'); }
        if(GetBit(PINC, 3) == 0) { return('D'); }

        return('\0');
}

#endif //KEYPAD_H
