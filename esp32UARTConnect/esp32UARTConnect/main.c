#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BAUD 9600
#define MY_UBRR F_CPU/16/BAUD-1

#define STR_SIZE 20  // ✅ 문자열 버퍼 크기

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
	DDRA = 0xFF;

	char str[STR_SIZE] = "";  // ✅ STR_SIZE 사용
	unsigned char idx = 0;

	usart0_init(MY_UBRR);

	while (1) {
		char received = rx0_ch();
		if (received == '\n' || received == '\r') {
			str[idx] = '\0';     // 끝 표시
			tx0_str(str);        // 문자열 출력
			tx0_str("\n");
			idx = 0;             // 버퍼 초기화
		}
		else if (idx < STR_SIZE - 1) {
			str[idx++] = received;
		}
	}
}
