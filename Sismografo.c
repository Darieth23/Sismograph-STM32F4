#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "clock.h"
#include "console.h"
#include "sdram.h"
#include "lcd-spi.h"
#include "gfx.h"
//Global Variables
uint8_t com_en;
uint16_t battery;
uint8_t batt_alarm;
//Main to be executed
int main(void)
{
	clock_setup();
	console_setup(115200);
	sdram_init();
	lcd_spi_init();
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
		battery = 0;
		sprintf(int_to_str, "%d", battery);
		gfx_setCursor(15, 198);
		gfx_puts("Batt: ");
		gfx_setCursor(130, 232);
		gfx_puts(int_to_str);
		gfx_setCursor(160, 232);
		gfx_puts("V");
		lcd_show_frame();
		msleep(500);
	}
	return 0;
}
