#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

#define IR_LED PB0         
#define BUTTON1 PD2        
#define BUTTON2 PD3        

void setup() {
    DDRB |= (1 << IR_LED);       
    DDRD &= ~((1 << BUTTON1) | (1 << BUTTON2)); 
    PORTD |= (1 << BUTTON1) | (1 << BUTTON2);   
}

void sendIRSignal(uint8_t signal) {
    for (uint8_t i = 0; i < 10; i++) { 
        if (signal == 1) {
            PORTB |= (1 << IR_LED); 
            _delay_us(500);
            PORTB &= ~(1 << IR_LED);
            _delay_us(500);
        } else if (signal == 2) {
            PORTB |= (1 << IR_LED); 
            _delay_us(1000);
            PORTB &= ~(1 << IR_LED);
            _delay_us(1000);
        }
    }
}

void loop() {
    if (!(PIND & (1 << BUTTON1))) {   
        sendIRSignal(1);             
        _delay_ms(200);             
    } else if (!(PIND & (1 << BUTTON2))) { 
        sendIRSignal(2);            
        _delay_ms(200);            
    }
}
