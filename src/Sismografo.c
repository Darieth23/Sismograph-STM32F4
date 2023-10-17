#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <libopencm3/stm32/adc.h>
#include "clock.h"
#include "console.h"
#include "sdram.h"
#include "lcd-spi.h"
#include "gfx.h"
#include "adc.c"
//Global Variables
uint8_t com_en;
uint16_t battery;
uint8_t batt_alarm;
//Main to be executed
int main(void)
{
	clock_setup();
	console_setup(115200);
	adc_setup();
	sdram_init();
	lcd_spi_init();
	com_en = 0;



	//Welcome 
	gfx_init(lcd_draw_pixel, 240, 320);
	gfx_fillScreen(LCD_BLACK);
	gfx_fillRoundRect(10, 10, 220, 220, 5, LCD_WHITE);
	gfx_drawRoundRect(10, 10, 220, 220, 5, LCD_BLUE);
	gfx_fillCircle(20, 250, 10, LCD_MAGENTA);
	gfx_fillCircle(120, 250, 10, LCD_GREEN);
	gfx_fillCircle(220, 250, 10, LCD_CYAN);
	gfx_setTextSize(2);
	gfx_setCursor(40, 25);
	gfx_puts("Sismografo");
	//gfx_setTextSize(1);
	gfx_setCursor(22, 78);
	gfx_puts("EIE-UCR-2023");
	lcd_show_frame();
	msleep(4000);
    int16_t gyr_x;
    int16_t gyr_y;
    int16_t gyr_z;
	char int_to_str[7];
	char lcd_out[3]; 


	while (1) {
        gyr_x = 0;
        gyr_y = 0;
        gyr_z = 0;

		if (gpio_get(GPIOA, GPIO0)) {
			if(com_en){
				com_en = 0;
			}
			else{
				com_en = 1;
			}
		}


		gfx_setTextColor(LCD_CYAN, LCD_BLACK);
		gfx_setTextSize(3);
		gfx_fillScreen(LCD_BLACK);	
		sprintf(lcd_out, "%s", "X:");
		sprintf(int_to_str, "%d", gyr_x);
		strcat(lcd_out, int_to_str);
		gfx_setCursor(15, 36);
		gfx_puts(lcd_out);
		sprintf(lcd_out, "%s", "Y:");
		sprintf(int_to_str, "%d", gyr_y);
		strcat(lcd_out, int_to_str);
		gfx_setCursor(15, 90);
		gfx_puts(lcd_out);
		sprintf(lcd_out, "%s", "Z:");
		sprintf(int_to_str, "%d", gyr_z);
		strcat(lcd_out, int_to_str);
		gfx_setCursor(15, 144);
		gfx_puts(lcd_out);
		gfx_setTextSize(2);
		battery = 0;
		sprintf(int_to_str, "%d", battery);
		gfx_setCursor(15, 250);
		gfx_puts("Bateria: ");
		gfx_setCursor(145, 250);
		gfx_puts(int_to_str);
		gfx_setCursor(165, 250);
		gfx_puts("%");
		lcd_show_frame();



		battery = adc_update();
		if(battery <= 7){
			batt_alarm = 1;
			//gpio_set(GPIOG, GPIO14);
		}
		else{
			batt_alarm = 0;
			//gpio_clear(GPIOG, GPIO14);
		}
		if(com_en){
	    	print_decimal_xyz(gyr_x); console_puts(",");
        	print_decimal_xyz(gyr_y); console_puts(",");
        	print_decimal_xyz(gyr_z); console_puts(",");
			print_decimal_xyz(batt_alarm); console_puts(",");
			print_decimal_xyz(battery); console_puts("\n");

		}
		else{
			//gpio_clear(GPIOG, GPIO13);
		}



		msleep(500);

	}
	return 0;
}
