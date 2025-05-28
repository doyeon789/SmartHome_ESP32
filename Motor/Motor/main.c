#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

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

void mortor_set_speed(uint8_t duty) {
	if (duty > 255) duty = 255;
	OCR1B = ((uint32_t)ICR1 * (255-duty)) / 255;  // Non-inverting 모드 기준
}

int main(void) {
	servo_init();

	while (1) {
		
		mortor_set_speed(60);
		
		servo_set_angle(0);
		_delay_ms(1000);

		servo_set_angle(90);
		_delay_ms(1000);

	}
}
