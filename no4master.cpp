#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void USART_Init(unsigned int ubrr) {
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void USART_Transmit(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

int main(void) {
    DDRD &= ~((1 << PD2) | (1 << PD3));
    PORTD |= (1 << PD2) | (1 << PD3);
    USART_Init(103);

    unsigned char lastButtonState1 = 0;
    unsigned char lastButtonState2 = 0;

    while (1) {
        unsigned char buttonState1 = PIND & (1 << PD2);
        unsigned char buttonState2 = PIND & (1 << PD3);

        if (buttonState1 && !lastButtonState1) {
            USART_Transmit('1');
        } else if (!buttonState1 && lastButtonState1) {
            USART_Transmit('0');
        }

        if (buttonState2 && !lastButtonState2) {
            USART_Transmit('3');
        } else if (!buttonState2 && lastButtonState2) {
            USART_Transmit('2');
        }

        lastButtonState1 = buttonState1;
        lastButtonState2 = buttonState2;
        _delay_ms(100);
    }
}
