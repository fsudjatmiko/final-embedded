#include <avr/io.h>
#include <util/delay.h>

void clear_dadu() {
    PORTD &= 0x00;
    PORTB &= 0xC0;
}

void set_dadu1(uint8_t value) {
    PORTD &= 0x00;
    switch (value) {
        case 1: PORTD |= (1 << PD3); break;
        case 2: PORTD |= (1 << PD0 | 1 << PD6); break;
        case 3: PORTD |= (1 << PD2 | 1 << PD3 | 1 << PD4); break;
        case 4: PORTD |= (1 << PD0 | 1 << PD2 | 1 << PD4 | 1 << PD6); break;
        case 5: PORTD |= (1 << PD0 | 1 << PD2 | 1 << PD3 | 1 << PD4 | 1 << PD6); break;
        case 6: PORTD |= (1 << PD0 | 1 << PD1 | 1 << PD2 | 1 << PD4 | 1 << PD5 | 1 << PD6); break;
    }
}

void set_dadu2(uint8_t value) {
    PORTB &= 0xC0;
    switch (value) {
        case 1: PORTB |= (1 << PB2); break;
        case 2: PORTD |= (1 << PD7); PORTB |= (1 << PB5); break;
        case 3: PORTB |= (1 << PB1 | 1 << PB2 | 1 << PB3); break;
        case 4: PORTD |= (1 << PD7); PORTB |= (1 << PB1 | 1 << PB3 | 1 << PB5); break;
        case 5: PORTD |= (1 << PD7); PORTB |= (1 << PB1 | 1 << PB2 | 1 << PB3 | 1 << PB5); break;
        case 6: PORTD |= (1 << PD7); PORTB |= (1 << PB0 | 1 << PB1 | 1 << PB3 | 1 << PB4 | 1 << PB5); break;
    }
}

uint8_t acak(uint8_t seed) {
    seed = (seed * 1103515245 + 12345) & 0xFF;
    return (seed % 6) + 1;
}

int main() {
    DDRD = 0xFF;
    DDRB = 0xFF;
    DDRC &= ~(1 << PC0);
    PORTC |= (1 << PC0);

    uint8_t dadu1 = 1, dadu2 = 1, seed = 0;

    ulang:
        if (!(PINC & (1 << PC0))) {
            _delay_ms(50);
            if (!(PINC & (1 << PC0))) {
                seed++;
                dadu1 = acak(seed);
                dadu2 = acak(seed + 1);

                clear_dadu();
                set_dadu1(dadu1);
                set_dadu2(dadu2);

                while (!(PINC & (1 << PC0)));
            }
        }
        goto ulang;
}
