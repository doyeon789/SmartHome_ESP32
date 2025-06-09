/*
 * GatherFunc.c
 *
 * Created: 2025-06-09 오후 8:16:17
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


void mortor_set_speed(uint8_t duty) {
	if (duty > 255) duty = 255;
	OCR1B = ((uint32_t)ICR1 * (255-duty)) / 255; 
}

void servo_init() {
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
	ICR1 = 40000;

	DDRB |= (1 << PB5) | (1 << PB6);

	OCR1A = 0;  // 서보 초기값 (필요하면 1500 정도로 초기화 가능)
	OCR1B = 0;  // 모터 초기 PWM 0 (멈춤)
}
void servo_set_angle(uint8_t angle) {
	if (angle > 180) angle = 180;
	uint16_t pulse = 2000 + ((uint32_t)angle * 4000) / 180;  // 1~2ms pulse
	OCR1A = pulse;
}


int main(void) {
	
	servo_init();
	
	
	DDRA = 0xff;
	DDRB = 0xff;
	DDRD = 0xff
	DDRC = 0xff;
	
	char received = "0";
	char str[20] = "";
	unsigned char idx = 0;
	usart0_init(103);
	while (1) {
		received = rx0_ch();
		
		if (received == '\n' || received == '\r') {
			str[idx] = '\0';     // 끝 표시
			tx0_str(str);
			tx0_str("\r\n");
			
			for (int i = 0; i < 8; i++) { // PORTA LED 제어
				if (str[i] == '0')
				PORTA &= ~(1 << i);
				else if (str[i] == '1')
				PORTA |= (1 << i);
			}		
			for (int i = 0; i < 3; i++) { // PORTB LED 제어
				if (str[i + 8] == '0')
				PORTB &= ~(1 << i);
				else if (str[i + 8] == '1')
				PORTB |= (1 << i);
			}
			switch(str[11] - '0'){ // RGB LED 제어
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
			switch(str[12] - '0'){
				case 0:
					servo_set_angle(0);
					break;
				case 1:
				servo_set_angle(90);
					break;
			}
			switch(str[13] - '0'){ // 모터 제어	
				case 0:
					mortor_set_speed(0);
					break;
				case 1:
					mortor_set_speed(60);
					break;
				case 2:
					mortor_set_speed(90);
					break;
				default:
					mortor_set_speed(120);
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
