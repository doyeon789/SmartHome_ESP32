/*
 * LEDON.c
 *
 * Created: 2025-05-25 오후 7:24:03
 * Author : user
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>


	
	
void usart0_init(unsigned int UBRR0){
	UBRR0H = (unsigned char)(UBRR0>>8);
	UBRR0L = (unsigned char)UBRR0;
	UCSR0B = (1<<TXEN0)|(1<<RXEN0);
}

void tx0_ch(unsigned char data){
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

void tx0_str(unsigned char *str){
	while(*str){
		tx0_ch(*str++);
	}
}

char rx0_ch(){
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

int main(void) {
	DDRA = 0xff;
	DDRB = 0xff;
	DDRD = 0xff
	DDRC = 0xff;
	
	char received = "0";
	char str[20] = "";  // ✅ STR_SIZE 사용
	unsigned char idx = 0;
	usart0_init(103);
	while (1) {
		received = rx0_ch();
		
		if (received == '\n' || received == '\r') {
			str[idx] = '\0';     // 끝 표시
			tx0_str(str);
			tx0_str("\r\n");
			
			for (int i = 0; i < 8; i++) {
				if (str[i] == '0')
				PORTA &= ~(1 << i);
				else if (str[i] == '1')
				PORTA |= (1 << i);
			}		
			for (int i = 0; i < 3; i++) {
				if (str[i + 8] == '0')
				PORTB &= ~(1 << i);
				else if (str[i + 8] == '1')
				PORTB |= (1 << i);
			}
			switch(str[11] - '0'){
				case 1:
					PORTC &= ~(1<<PC0);
					PORTC |= (1<<PC1);
					PORTC &= ~(1<<PC2);
					break;
				case 2:
					PORTC |= (1<<PC0);
					PORTC &= ~(1<<PC1);
					PORTC &= ~(1<<PC2);
					break;
				default:
					PORTC &= ~(1<<PC0);
					PORTC &= ~(1<<PC1);
					PORTC &= ~(1<<PC2);
					break;
			}

			idx = 0;
			memset(str, 0, 20);
		}
		else if (idx < 20 - 1) {
			str[idx++] = received;
		}
	}
}
