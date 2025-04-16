/*
 * esp32UARTConnect.c
 *
 * Created: 2025-04-16 오후 12:22:13
 * Author : user
 */ 
#define F_CPU 16000000UL  // Ensure that the CPU frequency is correct
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BAUD 9600
#define MY_UBRR F_CPU/16/BAUD-1  // Baud rate calculation for 9600

void usart0_init(unsigned int UBRR0){
	UBRR0H = (unsigned char)(UBRR0>>8);
	UBRR0L = (unsigned char)UBRR0;
	UCSR0B = (1<<TXEN0)|(1<<RXEN0);
}

void tx0_ch(unsigned char data){
	while(!(UCSR0A & (1<<UDRE0)));  // Wait until buffer is empty
	UDR0 = data;  // Transmit data
}

void tx0_str(unsigned char *str){
	while(*str){  // Send each character of the string until '\0'
		tx0_ch(*str++);
	}
}

char rx0_ch(){
	while(!(UCSR0A & (1<<RXC0)));  // Wait for data to be received
	return UDR0;  // Return received character
}

int main(void) {
	DDRA = 0xff;
	
	char str[20] = "";  // Initialize empty string buffer
	unsigned char idx = 0;  // Index to track position in the string

	usart0_init(MY_UBRR);  // Initialize UART

	while (1) {
		char received = rx0_ch();  // Read received character
		if (received != '\0' && idx < 19) {  // Ensure space for '\0'
			str[idx++] = received;  // Store character
			str[idx] = '\0';  // Null-terminate string
			tx0_str(str);
			tx0_str("\n");
		}
	}
}
