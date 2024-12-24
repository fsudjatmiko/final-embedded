#include <avr/io.h>
#include <util/delay.h>

uint8_t pola_kiri[6] = {0b000100, 0b001100, 0b111000, 0b111000, 0b001100, 0b000100};
uint8_t pola_kanan[6] = {0b001000, 0b001100, 0b000111, 0b000111, 0b001100, 0b001000};

void geser_kiri(uint8_t* pola) {
    for (uint8_t i = 0; i < 6; i++) {
        pola[i] = (pola[i] << 1) | (pola[i] >> 5);
        pola[i] &= 0x3F;
    }
}

void geser_kanan(uint8_t* pola) {
    for (uint8_t i = 0; i < 6; i++) {
        pola[i] = (pola[i] >> 1) | (pola[i] << 5);
        pola[i] &= 0x3F;
    }
}

void animasi_hazard() {
    uint8_t pola[6] = {0b000000, 0b000000, 0b000000, 0b000000, 0b000000, 0b000000};
    uint8_t steps[3][6] = {
        {0b000000, 0b000000, 0b001100, 0b001100, 0b000000, 0b000000}, // 2x2
        {0b000000, 0b010010, 0b011110, 0b011110, 0b010010, 0b000000}, // 4x4
        {0b100001, 0b110011, 0b111111, 0b111111, 0b110011, 0b100001}  // 6x6
    };

    for (uint8_t langkah = 0; langkah < 3; langkah++) {
        for (uint8_t baris = 0; baris < 6; baris++) {
            pola[baris] = steps[langkah][baris];
        }

        for (uint8_t i = 0; i < 50; i++) { 
            for (uint8_t baris = 0; baris < 6; baris++) {
                PORTD &= ~(1 << baris);
                PORTB = (PORTB & ~0x3F) | pola[baris];
                _delay_us(500);
                PORTD |= (1 << baris);
            }
        }
    }
}

void animasi_stop() {
    for (uint8_t i = 0; i < 5; i++) {
        // Turn on the X sign LEDs
        PORTD &= ~((1 << 0) | (1 << 1) | (1 << 4) | (1 << 5)); 
        PORTB = (PORTB & ~0x3F) | 0b100001; 
        _delay_ms(100);
        
        
        PORTD |= (1 << 0) | (1 << 1) | (1 << 4) | (1 << 5);
        PORTB = (PORTB & ~0x3F);
        _delay_ms(100);
    }
}

int main(void) {
    DDRD |= 0x3F;
    DDRB |= 0x3F;
    DDRC &= ~((1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3));
    PORTC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3);

    loop:
        if (!(PINC & (1 << PC2))) {
            animasi_hazard();
        } else if (!(PINC & (1 << PC0))) {
            for (uint8_t baris = 0; baris < 6; baris++) {
                PORTD &= ~(1 << baris);
                PORTB = (PORTB & ~0x3F) | pola_kiri[baris];
                _delay_us(500);
                PORTD |= (1 << baris);
            }
            geser_kiri(pola_kiri);
        } else if (!(PINC & (1 << PC1))) {
            for (uint8_t baris = 0; baris < 6; baris++) {
                PORTD &= ~(1 << baris);
                PORTB = (PORTB & ~0x3F) | pola_kanan[baris];
                _delay_us(500);
                PORTD |= (1 << baris);
            }
            geser_kanan(pola_kanan);
        } else if (!(PINC & (1 << PC3))) {
            animasi_stop();
        } else {
            for (uint8_t baris = 0; baris < 6; baris++) {
                PORTD |= (1 << baris);
            }
        }
        _delay_ms(50);
    goto loop;
    return 0;
}
