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

void timer1_pwm_init(){
	DDRB|=(1<<PB5)|(1<<PB6)|(1<<PB7);
	TCCR1A|=(1<<WGM10);
	TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<COM1C1);
	TCCR1B|=(1<<WGM12);
	TCCR1B|=(1<<CS02)|(1<<CS00);
	TIMSK|=(1<<OCIE1A)|(1<<OCIE1B);
	ETIMSK|=(1<<OCIE1C);
	TCNT1=0;
}

ISR(TIMER1_COMPA_vect){}
ISR(TIMER1_COMPB_vect){}
ISR(TIMER1_COMPC_vect){}
	
int main(void) {
	DDRA = 0xff;
	DDRB = 0xff;
	
	timer1_pwm_init();

    /* Replace with your application code */
    while (1)  {
		PORTA= 0xff;
		PORTB = (1<<PB0)|(1<<PB1)|(1<<PB2);
		
		OCR1A = 0xff; //R
		OCR1B = 0xff; //G
		OCR1C = 0xff; //B
    }
}