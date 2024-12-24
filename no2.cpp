#include <avr/io.h>
#include <util/delay.h>

#define LED_PORT PORTD
#define LED_DDR DDRD
#define BUTTON_PIN PINC
#define BUTTON PC0

void semua_mati() {
    LED_PORT = 0x00;
}

void bpwm() {
    for (uint8_t i = 0; i < 255; i++) {
        LED_PORT = 0xFF;
        for (uint16_t j = 0; j < i; j++) {
            _delay_us(1);
        }
        LED_PORT = 0x00;
        for (uint16_t j = 0; j < (255 - (uint16_t)i); j++) {
            _delay_us(1);
        }
    }
    for (uint8_t i = 255; i > 0; i--) {
        LED_PORT = 0xFF;
        for (uint16_t j = 0; j < i; j++) {
            _delay_us(1);
        }
        LED_PORT = 0x00;
        for (uint16_t j = 0; j < (255 - (uint16_t)i); j++) {
            _delay_us(1);
        }
    }
}

void lampu_berjalan1() {
    for (int i = 0; i < 6; i++) {
        LED_PORT = (1 << i);
        _delay_ms(50);
    }
}

void lampu_berjalan2(){
      for (int i = 4; i >= 0; i--) {
        LED_PORT = (1 << i);
        _delay_ms(50);
    }
}

void ganjil() {
    LED_PORT = 0x2A;
    _delay_ms(50);
    LED_PORT = 0x00;
    _delay_ms(50);
}

void genap() {
    LED_PORT = 0x15;
    _delay_ms(50);
    LED_PORT = 0x00;
    _delay_ms(50);
}

void kedip_semua() {
    LED_PORT = 0xFF;
    _delay_ms(100);
    LED_PORT = 0x00;
    _delay_ms(100);
}

void tengah_ke_tepi() {
    int8_t i;
    for (i = 0; i < 3; i++) {
        LED_PORT = (0x21 >> i) | (0x21 << i);
        _delay_ms(50);
    }
    for (i = 1; i >= 0; i--) {
        LED_PORT = (0x21 >> i) | (0x21 << i);
        _delay_ms(50);
    }
}

int main() {
    LED_DDR = 0xFF;
    DDRC &= ~(1 << BUTTON);
    PORTC |= (1 << BUTTON);

    uint8_t state = 0;
    uint8_t button_pressed = 0;

    ulang:
        if (!(BUTTON_PIN & (1 << BUTTON))) {
            _delay_ms(50);
            if (!(BUTTON_PIN & (1 << BUTTON))) {
                button_pressed = 1;
            }
        }

        if (button_pressed) {
            state = (state + 1) % 8;
            button_pressed = 0;
            while (!(BUTTON_PIN & (1 << BUTTON)));
        }

        switch (state) {
            case 0: semua_mati(); break;
            case 1: bpwm(); break;
            case 2: lampu_berjalan1(); break;
            case 3: lampu_berjalan2(); break;
            case 4: ganjil(); break;
            case 5: genap(); break;
            case 6: kedip_semua(); break;
            case 7: tengah_ke_tepi(); break;
        }
        goto ulang;
}
