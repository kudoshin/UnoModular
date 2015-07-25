#ifndef MY_TONE
#define MY_TONE

#include <stdint.h>

/* Play a tone on the given pin using the given timer, frequency, and pwm val */
void my_tone(uint8_t pin, uint8_t timer, uint16_t frequency, double pwm);

#endif
