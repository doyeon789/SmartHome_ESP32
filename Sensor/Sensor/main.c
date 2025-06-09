/*
 * Sensor.c
 *
 * Created: 2025-06-09 오후 6:08:20
 * Author : user
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void usart0_init(unsigned int UBRR0){
	UBRR0L=(unsigned char)UBRR0;
	UCSR0B=(1<<TXEN0)|(1<<RXEN0);
}
void tx0_ch(unsigned char data)
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0=data;
}
void tx0_str(unsigned char *str){
	while(*str){tx0_ch(*str++);}
}
void adc_init(){
	DDRF &= ~(3<<0);
	ADMUX = 0;
	ADCSRA = (1<<ADEN) | (7<<ADPS0);
}
unsigned short read_adc() {
	ADCSRA |= (1<<ADSC);
	while((ADCSRA & (1<<ADIF)) != (1<<ADIF));
	unsigned char adc_low = ADCL;
	unsigned char adc_high = ADCH;
	
	if(ADMUX == 0)	ADMUX = 1;
	else if(ADMUX == 1)	ADMUX = 0;
	
	return((unsigned short)adc_high << 8) | (unsigned short)adc_low;;
}
int main(void) {
	char str[40];
	unsigned int cds = 0;
	unsigned int waterSensor = 0;
	
	usart0_init(103);	
	adc_init();
	
    while (1){
		cds = read_adc();
		waterSensor = read_adc();
		sprintf(str,"cds: %d, water: %d\r\n",cds,waterSensor);
		tx0_str(str);
    }
}

