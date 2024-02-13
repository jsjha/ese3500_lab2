/*
 * partc.c
 *
 * Created: 2/13/2024 2:57:42 PM
 * Author : jessi
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

void Initialize()
{
	// Disable all global interrupts initially
	cli();
	
	// Set PB0 as input pin for the button
	DDRB &= ~(1 << DDB0);
	
	// Enable pull-up resistor for PB0 (button)
	PORTB |= (1 << PORTB0);
	
	// Set PB1 as output pin for the LED
	DDRB |= (1 << DDB3);
	
	// Set clock source for Timer 1 with prescaler of 8
	TCCR1B |= (1 << CS11);
	
	// Configure Timer 1 for input capture mode with rising and falling edge detection
	TCCR1B |= (1 << ICES1);    // Set to capture rising edge first
	TIFR1 |= (1 << ICF1);
	TIMSK1 |= (1 << ICIE1);    // Enable input capture interrupt
	
	// Enable all global interrupts
	sei();
}

// Interrupt Service Routine for Timer 1 input capture event
ISR(TIMER1_CAPT_vect)
{
	//// Clear the input capture flag by writing 1 to ICF1 bit
	TIFR1 |= (1 << ICF1);
	
	// Read the state of PB0 to determine button press status
	if ((PINB & (1 << PINB0))) // Button released
	{
		// Turn off the LED connected to PB1
		PORTB &= ~(1 << PORTB3);
		TCCR1B &= ~(1 << ICES1);    // Set to capture falling edge first
	}
	else // Button pressed
	{
		// Turn on the LED connected to PB1
		PORTB |= (1 << PORTB3);
		TCCR1B |= (1 << ICES1);    // Set to capture rising edge first
		
	}
}

int main(void)
{
	// Initialize the microcontroller
	Initialize();
	while (1);
}

