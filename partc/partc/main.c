/*
 * partc.c
 *
 * Created: 2/13/2024 2:57:42 PM
 * Author : jessi
 */ 

#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "inc/uart.h"

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

char String[25];

volatile uint32_t overFlowCount = 0;

volatile uint32_t start = 0;

volatile uint32_t end = 0;

volatile uint32_t duration = 0;

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
	
	// Set clock source for Timer 1 with prescaler of 64
	TCCR1B |= (1 << CS11) | (1 << CS10);
	
	// Configure Timer 1 for input capture mode with rising and falling edge detection
	TCCR1B |= (1 << ICES1);    // Set to capture rising edge first
	TIFR1 |= (1 << ICF1);
	TIMSK1 |= (1 << ICIE1);    // Enable input capture interrupt
	
	// Enables Timer 1 overflow interrupt
	TIMSK1 |= (1 << TOIE1);
	
	// Enable all global interrupts
	sei();
}


ISR(TIMER1_OVF_vect)
{
	overFlowCount++;
}

// Interrupt Service Routine for Timer 1 input capture event
ISR(TIMER1_CAPT_vect)
{
	//// Clear the input capture flag by writing 1 to ICF1 bit
	TIFR1 |= (1 << ICF1);
	
	// Read the state of PB0 to determine button press status
	if ((PINB & (1 << PINB0))) // Button released
	{
		end  = TCNT1;
		
		// Turn off the LED connected to PB1
		PORTB &= ~(1 << PORTB3);
		
		// Set to capture falling edge first
		TCCR1B |= (1 << ICES1);
	}
	else // Button pressed
	{
		start = TCNT1; 
		
		// Turn on the LED connected to PB1
		PORTB |= (1 << PORTB3);
		
		// Set to capture rising edge first
		TCCR1B &= ~(1 << ICES1);   
	
	}
	
	helperCountTicks();
}

void helperCountTicks() 
{
	
	UART_init(BAUD_PRESCALER);
	
	// This sets the duration, depending on if there were overflows in between the push of
	// the button and the release of the button
	if (end >= start) {
		// (2^16)*overFlowCount + (endTime - startTime)
		duration = ((uint32_t)overFlowCount << 16) + end - start;
		} else {
		// Adjust calculation if timer overflow occurred during the press
		duration = ((uint32_t)(overFlowCount - 1) << 16) + 0xFFFF - start + end;
	}
	
	// Convert duration into time (ms) 
	// ASK ABOUT CONVERSION FROM TICK TO MS!!!!

	
	// Based on the duration, we check if dot, dash, or space
	if (duration >= 30 && duration < 200)
	{
		//Print dot
		sprintf(String,".");
		UART_putstring(String);
		
	} else if (duration >= 200 && duration < 400 )
		
		//Print dash
		sprintf(String,"-");
		UART_putstring(String);
	
	} else if (duration > 400) {
		
		//Print space
		sprintf(String," ");
		UART_putstring(String);
	}	
}

int main(void)
{
	// Initialize the microcontroller
	Initialize();
	while (1);
}

