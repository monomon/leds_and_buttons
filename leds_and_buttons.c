/*
 * cycle leds, controlling direction with buttons,
 * speed with a potentiometer
 */

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <time.h>
#include <adc0832.h>

#define leds_interval_min 5
#define leds_interval_max 200
#define sampling_interval_ms 500

int main(void)
{
	uchar adc_value;
	
	if (wiringPiSetup() == -1) {
		printf("wiringPi setup failed!\n");
		exit(1);
	}
	
	uint adc_data_pin = 0;
	uint adc_clk_pin = 14;
	uint adc_cs_pin = 15;
	
	uint led_pins[] = {3, 29, 28, 24, 1, 6};
	uint button_pins[] = {27, 4};
	
	uint num_leds = 6;
	uint num_buttons = 2;
	int direction = 1;
	uint led_period_ms = 20;

	int current_led = 0;
	
	adc0832_setup(
		adc_data_pin,
		adc_cs_pin,
		adc_clk_pin
	);
	
	for (int i=0; i<num_leds; i++) {
		pinMode(led_pins[i], OUTPUT);
	}
	
	for (int i=0; i<num_buttons; i++) {
		pinMode(button_pins[i], INPUT);
	}
	
	while (1) {
		adc_value = adc0832_read(
			adc_data_pin,
			adc_cs_pin,
			adc_clk_pin,
			adc0832_mode_single_ended,
			adc0832_channel0
		);
			
		//~ printf("adc: %d\n", adc_value);
		//~ printf("led period: %d\n", led_period_ms);
		//~ printf("current led: %d\n", current_led);

		digitalWrite(led_pins[current_led], HIGH);
		delay(led_period_ms);
		digitalWrite(led_pins[current_led], LOW);
		delay(led_period_ms);
		
		if (digitalRead(button_pins[0]) == HIGH) {
			direction = -1;
		}
		
		if (digitalRead(button_pins[1]) == HIGH) {
			direction = 1;
		}
		
		current_led = (current_led + direction);
		// wrap around (use modulo for this)
		if (current_led < 0) {
			current_led = num_leds-1;
		} else if (current_led >= num_leds) {
			current_led = 0;
		}

		led_period_ms = leds_interval_min + (leds_interval_max*adc_value/255.0);
	}
	
	return 0;
}
