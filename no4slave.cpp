#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void USART_Init(unsigned int ubrr) {
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << RXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

unsigned char USART_Receive(void) {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

int main(void) {
    DDRB |= (1 << PB0) | (1 << PB1);
    USART_Init(103);

    while (1) {
        unsigned char receivedData = USART_Receive();
        if (receivedData == '0') {
            PORTB |= ~(1 << PB0);
        } else if (receivedData == '1') {
            PORTB &= (1 << PB0);
        } else if (receivedData == '2') {
            PORTB |= ~(1 << PB1);
        } else if (receivedData == '3') {
            PORTB &= (1 << PB1);
        }
    }
}
