#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

// Define pins
#define IR_RECEIVER PD2 // IR receiver connected to PD2
#define LED1 PB3        // LED for signal 1 connected to PB3
#define LED2 PB4        // LED for signal 2 connected to PB4

void setup() {
    DDRD &= ~(1 << IR_RECEIVER); // Set IR receiver pin as input
    PORTD |= (1 << IR_RECEIVER); // Enable pull-up resistor for IR receiver
    DDRB |= (1 << LED1) | (1 << LED2); // Set LED pins as output
    PORTB &= ~((1 << LED1) | (1 << LED2)); // Turn off LEDs initially
}

uint8_t readPulseDuration() {
    uint16_t duration = 0;

    while (!(PIND & (1 << IR_RECEIVER))) { // Wait while the IR signal is LOW
        _delay_us(10);                     // Increment duration in 10µs steps
        duration++;
        if (duration > 200) break;        // Cap to avoid infinite loop
    }
    return duration; // Return pulse duration
}

void loop() {
    uint16_t pulse = readPulseDuration();

    if (pulse > 40 && pulse <= 60) { // Signal 1: Short pulse (500µs approx.)
        PORTB |= (1 << LED1);        // Turn on LED1
        _delay_ms(500);              // Keep LED on for visibility
        PORTB &= ~(1 << LED1);       // Turn off LED1
    } else if (pulse > 90 && pulse <= 120) { // Signal 2: Long pulse (1000µs approx.)
        PORTB |= (1 << LED2);              // Turn on LED2
        _delay_ms(500);                    // Keep LED on for visibility
        PORTB &= ~(1 << LED2);             // Turn off LED2
    }
}
