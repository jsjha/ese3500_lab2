PART F FINAL CODE.
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include "uart.h"
#include <string.h>
#include <util/delay.h>
#include <ctype.h>
char String[25];

// Define durations for a dot, dash, and space in milliseconds
#define DOT_DURATION 200
#define DASH_DURATION 600
#define SPACE_DURATION 800


void Initialize() {
    // Set PB3 as output pin for the dot LED
    DDRB |= (1 << DDB3);
    // Set PB4 as output pin for the dash LED
    DDRB |= (1 << DDB4);
    // Set PB5 as output pin for the space LED
    DDRB |= (1 << DDB5);
}

// Function to output Morse code for a given character
void outputMorseCode(char c) {
    // Define Morse code for characters A-Z (you would need to extend this for more characters)
    const char* morseCodes[26] = {
        ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
        "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
        "..-", "...-", ".--", "-..-", "-.--", "--.."
    };

    // Convert character to uppercase if it's not already
    c = toupper(c);
   
    // Check if character is between A-Z
    if (c >= 'A' && c <= 'Z') {
        const char* morseCode = morseCodes[c - 'A']; // Get Morse code for character
        // Loop through the Morse code string
        for (int i = 0; morseCode[i] != '\0'; i++) {
            if (morseCode[i] == '.') {
                PORTB |= (1 << PORTB3);      // Turn on the LED
                _delay_ms(1200);         // Wait for the duration
                PORTB &= ~(1 << PORTB3);     // Turn off the LED
                _delay_ms(1200);
            } else if (morseCode[i] == '-') {
                 PORTB |= (1 << PORTB4);      // Turn on the LED
                 _delay_ms(1600);         // Wait for the duration
                 PORTB &= ~(1 << PORTB4);     // Turn off the LED
                 _delay_ms(1600);
            }
        }
    }
     PORTB |= (1 << PORTB5);      // Turn on the LED
     _delay_ms(1800);         // Wait for the duration
     PORTB &= ~(1 << PORTB5);     // Turn off the LED
     _delay_ms(1800);
}


int main(void) {
    // Initialize the microcontroller
    Initialize();

    // Define the message to be outputted in Morse code
    const char* message = "A A A A A A";

    // Loop through each character of the message
    for (int i = 0; message[i] != '\0'; i++) {
        outputMorseCode(message[i]);
        // Add a longer delay between words
        if (message[i] == ' ') {
            _delay_ms(SPACE_DURATION * 3); // Wait for the duration of a space between words
        }
    }

    // End of program, loop indefinitely
    while (1) {
    }

    return 0;
}