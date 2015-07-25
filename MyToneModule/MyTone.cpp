#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "Arduino.h"
#include "pins_arduino.h"
#include "MyTone.h"

#define NUM_TIMERS 3
#define NUM_SCALES 5

static int scales[NUM_SCALES] = {1, 8, 64, 256, 1024};
static uint8_t scale_bits[NUM_SCALES] = { 0b001, 0b010, 0b011, 0b100, 0b101 };
static uint8_t pins[NUM_TIMERS] = { 255, 255, 255 };

static int
set_scaling_factor(uint16_t frequency, uint8_t timer)
{
	int i = 0;

	if (timer == 1) {
		i = 3;
	} else if (timer == 0 || timer == 2) {
		int ocr = F_CPU / frequency / scales[i];
		while (ocr > 255) {
			i++;
			if (i == NUM_SCALES - 1) break;
			ocr = F_CPU / frequency / scales[i];
		}
	}

	if (i > NUM_SCALES) {
		i = NUM_SCALES - 1;
	}

	switch (timer) {
		case 0:
			TCCR0B = (TCCR0B & 0b11111000) | scale_bits[i];
			break;
		case 1:
			TCCR1B = (TCCR1B & 0b11111000) | scale_bits[i];
			break;
		case 2:
			TCCR2B = (TCCR2B & 0b11111000) | scale_bits[i];
			break;
		default:
			return 1;
	}
	return scales[i];
}


static void
tone_begin(uint8_t pin, uint8_t timer)
{
	switch (timer) {
		case 0:
			TCCR0A = 0;
			TCCR0B = 0;
			TIMSK0 = 0;

			/* Fast PWM mode, top determined by OCR0A */
			bitWrite(TCCR0A, WGM00, 1);
			bitWrite(TCCR0A, WGM01, 1);
			bitWrite(TCCR0B, WGM02, 1);

			/* Set interrupt on OCR0A & OCR0B */
			bitWrite(TIMSK0, OCIE0A, 1);
			bitWrite(TIMSK0, OCIE0B, 1);
			break;
		case 1:
			TCCR1A = 0;
			TCCR1B = 0;
			TIMSK1 = 0;

			/* Fast PWM mode, top determined by OCR1A */
			bitWrite(TCCR1A, WGM10, 1);
			bitWrite(TCCR1A, WGM11, 1);
			bitWrite(TCCR1B, WGM12, 1);
			bitWrite(TCCR1B, WGM13, 1);

			/* Set interrupt on OCR1A & OCR1B */
			bitWrite(TIMSK1, OCIE1A, 1);
			bitWrite(TIMSK1, OCIE1B, 1);
			break;
		case 2:
			TCCR2A = 0;
			TCCR2B = 0;
			TIMSK2 = 0;

			/* Fast PWM mode, top determined by OCR2A */
			bitWrite(TCCR2A, WGM20, 1);
			bitWrite(TCCR2A, WGM21, 1);
			bitWrite(TCCR2B, WGM22, 1);

			/* Set interrupt on OCR2A & OCR2B */
			bitWrite(TIMSK2, OCIE2A, 1);
			bitWrite(TIMSK2, OCIE2B, 1);
			break;
		default:
			return;
	}
}

static void
set_levels(uint8_t timer, int scale, uint16_t frequency, double pwm)
{
	uint32_t max_freq = F_CPU / scale;
	uint16_t max_count = (uint16_t) max_freq / frequency;
	uint16_t comp_val = (uint16_t)(pwm * max_count);

	switch (timer) {
		case 0:
			OCR0A = (uint8_t) max_count;
			OCR0B = (uint8_t) comp_val;
			break;
		case 1:
			OCR1A = max_count;
			OCR1B = comp_val;
			break;
		case 2:
			OCR2A = (uint8_t) max_count;
			OCR2B = (uint8_t) comp_val;
			break;
		default:
			return;
	}
}

static void
disable_timer(uint8_t timer)
{
	switch(timer) {
		case 0:
			TIMSK0 = 0;
		case 1:
			TIMSK1 = 0;
		case 2:
			TIMSK2 = 0;
		default:
			return;
	}
}

void
my_tone(uint8_t pin, uint8_t timer, uint16_t frequency, double pwm)
{

	if (timer < NUM_TIMERS) {
		if (pwm == 0.0) {
			disable_timer(timer);
			pins[timer] = 255;
			return;
		} else {
			pins[timer] = pin;
			tone_begin(pin, timer);
			int scale = set_scaling_factor(frequency, timer);
			set_levels(timer, scale, frequency, pwm);
			pinMode(pin, OUTPUT);
		}
	}
}

ISR(TIMER0_COMPA_vect)
{
	if (pins[0] < NUM_TIMERS) {
		digitalWrite(pins[0], LOW);
	}
}

ISR(TIMER0_COMPB_vect)
{
	if (pins[0] < NUM_TIMERS) {
		digitalWrite(pins[0], HIGH);
	}
}

ISR(TIMER1_COMPA_vect)
{
	if (pins[1] < NUM_TIMERS) {
		digitalWrite(pins[1], LOW);
	}
}

ISR(TIMER1_COMPB_vect)
{
	if (pins[1] < NUM_TIMERS) {
		digitalWrite(pins[1], HIGH);
	}
}

ISR(TIMER2_COMPA_vect)
{
	if (pins[2] < NUM_TIMERS) {
		digitalWrite(pins[2], LOW);
	}
}

ISR(TIMER2_COMPB_vect)
{
	if (pins[2] < NUM_TIMERS) {
		digitalWrite(pins[2], HIGH);
	}
}
