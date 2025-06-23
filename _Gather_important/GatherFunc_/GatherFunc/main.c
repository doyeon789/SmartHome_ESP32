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

// USART 초기화 (보레이트 설정 및 송수신 활성화)
void usart0_init(unsigned int UBRR0){
	UBRR0H = (unsigned char)(UBRR0>>8);
	UBRR0L = (unsigned char)UBRR0;
	UCSR0B = (1<<TXEN0)|(1<<RXEN0);  // 송신 및 수신 활성화
}

// USART로 문자 1개 전송
void tx0_ch(unsigned char data){
	while(!(UCSR0A & (1<<UDRE0)));  // 송신 가능 대기
	UDR0 = data;
}

// USART로 문자열 전송
void tx0_str(unsigned char *str){
	while(*str){
		tx0_ch(*str++);
	}
}

// ADC 초기화 (ADC0, ADC1 사용 가능)
void adc_init(){
	DDRF &= ~(3<<0);  // PF0, PF1 입력 설정
	ADMUX = 0;  // ADC0부터 시작
	ADCSRA = (1<<ADEN) | (7<<ADPS0);  // ADC 활성화, 분주비 128
}

// ADC 값 읽기 및 채널 전환 (ADC0 <-> ADC1)
unsigned short read_adc() {
	ADCSRA |= (1<<ADSC);  // 변환 시작
	while((ADCSRA & (1<<ADIF)) != (1<<ADIF));  // 변환 완료 대기

	unsigned char adc_low = ADCL;
	unsigned char adc_high = ADCH;

	// 다음 측정을 위해 채널 전환
	if(ADMUX == 0)	ADMUX = 1;
	else if(ADMUX == 1)	ADMUX = 0;
	
	return ((unsigned short)adc_high << 8) | (unsigned short)adc_low;
}

// USART로 수신한 문자 1개 읽기
char rx0_ch(){
	while(!(UCSR0A & (1<<RXC0)));  // 수신 대기
	return UDR0;
}

// 모터 속도 설정 (PWM 출력)
void mortor_set_speed(uint8_t duty) {
	if (duty > 255) duty = 255;
	OCR1B = ((uint32_t)ICR1 * (255-duty)) / 255;  // 속도 조절 (0~255)
}

// 서보 및 모터 타이머 초기화
void servo_init() {
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);  // 분주비 8, Fast PWM 모드
	ICR1 = 40000;  // 20ms 주기 (서보용)

	DDRB |= (1 << PB5) | (1 << PB6);  // PWM 출력 핀 설정

	OCR1A = 0;  // 서보 초기 위치
	OCR1B = 0;  // 모터 정지
}

// 서보 각도 설정 함수 (0~180도)
void servo_set_angle(uint8_t angle) {
	if (angle > 180) angle = 180;
	uint16_t pulse = 2000 + ((uint32_t)angle * 4000) / 180;  // 1~2ms 펄스
	OCR1A = pulse;
}

int main(void) {
	// 포트 초기 설정
	DDRA = 0xff;  // LED 출력용
	DDRB = 0xff;  // PWM 출력용
	DDRD |= (1 << PD0) | (1 << PD1) | (1 << PD2);  // RGB LED 등

	char received = "0";
	char r_str[20] = "";
	unsigned char idx = 0;

	char s_str[20] = "";
	unsigned int cds = 0;	
	unsigned int waterSensor = 0;

	servo_init();         // 서보 초기화
	adc_init();           // ADC 초기화
	usart0_init(103);     // USART 보레이트 9600bps 기준

	while (1) {
		/*
		// 센서 값 송신 (필요시 사용)
		cds = read_adc();
		waterSensor = read_adc();
		sprintf(s_str,"%d,%d\r\n",cds,waterSensor);
		tx0_str(s_str);
		*/

		// 시리얼 수신 처리
		received = rx0_ch();
		if (received == '\n' || received == '\r') {
			r_str[idx] = '\0';  // 문자열 종료

			// [0~7] LED 제어
			for (int i = 0; i < 8; i++) {
				if (r_str[i] == '0')
					PORTA &= ~(1 << i);  // 끄기
				else if (r_str[i] == '1')
					PORTA |= (1 << i);   // 켜기
			}

			// [8] RGB LED 제어
			if(r_str[8] == '0'){
				PORTD &= ~(1<<1);  // 끄기
			}
			if(r_str[8] == '1'){
				PORTD |= (1<<1);   // 켜기
			}

			// [9] 창문(서보) 제어
			if(r_str[9] == '0'){
				servo_set_angle(0);    // 닫기
			}
			if(r_str[9] == '1'){
				servo_set_angle(90);   // 열기
			}

			// [10] 모터 속도 제어
			if(r_str[10] == '0'){
				mortor_set_speed(0);   // 정지
			}
			if(r_str[10] == '1'){
				mortor_set_speed(30);  // 저속
			}
			if(r_str[10] == '2'){
				mortor_set_speed(50);  // 중속
			}
			if(r_str[10] == '3'){
				mortor_set_speed(70);  // 고속
			}

			idx = 0;  // 다음 수신을 위해 인덱스 초기화
			memset(r_str, 0, 20);  // 문자열 초기화
		}
		else if (idx < 19) {
			r_str[idx++] = received;  // 수신 문자 저장
		}
	}
}
