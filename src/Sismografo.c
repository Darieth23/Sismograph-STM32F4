#include <stdint.h>
#include <math.h>
#include <libopencm3/stm32/adc.h>
#include "clock.h"
#include "console.h"
#include "sdram.h"
#include "lcd-spi.h"
#include "gfx.h"
#include "adc_spi.c"
#include <stdio.h>
#include <string.h>
//Global Variables
uint8_t com_en;
uint16_t battery;
uint8_t batt_alarm;
//Constants
#define GYR_RNW			(1 << 7) /* Write when zero  (ahorita en 1, read)*/  
#define GYR_CTRL_REG1		0x20
#define GYR_CTRL_REG1_PD	(1 << 3)
#define GYR_CTRL_REG1_XEN	(1 << 1)
#define GYR_CTRL_REG1_YEN	(1 << 0)
#define GYR_CTRL_REG1_ZEN	(1 << 2)
#define GYR_CTRL_REG1_BW_SHIFT	4
#define GYR_CTRL_REG4		0x23
#define GYR_CTRL_REG4_FS_SHIFT	4
#define GYR_OUT_X_L		0x28
#define GYR_OUT_X_H		0x29
#define GYR_OUT_Y_L		0x2A
#define GYR_OUT_Y_H		0x2B
#define GYR_OUT_Z_L		0x2C
#define GYR_OUT_Z_H		0x2D
#define L3GD20_SENSITIVITY_250DPS  (0.00875F)
//Main to be executed
int main(void)
{
	clock_setup();
	console_setup(115200);
	spi_setup();
	adc_setup();
	sdram_init();
	lcd_spi_init();
	com_en = 0;
    gpio_clear(GPIOC, GPIO1);
	spi_send(SPI5, GYR_CTRL_REG1); 
	spi_read(SPI5);
	spi_send(SPI5, GYR_CTRL_REG1_PD | GYR_CTRL_REG1_XEN |
			GYR_CTRL_REG1_YEN | GYR_CTRL_REG1_ZEN |
			(3 << GYR_CTRL_REG1_BW_SHIFT));
	spi_read(SPI5);
	gpio_set(GPIOC, GPIO1); 
    gpio_clear(GPIOC, GPIO1);
	spi_send(SPI5, GYR_CTRL_REG4);
	spi_read(SPI5);
	spi_send(SPI5, (1 << GYR_CTRL_REG4_FS_SHIFT));
	spi_read(SPI5);
	gpio_set(GPIOC, GPIO1);
	gfx_init(lcd_draw_pixel, 240, 320);
	gfx_fillScreen(LCD_BLACK);
	gfx_fillRoundRect(10, 10, 220, 220, 5, LCD_WHITE);
	gfx_drawRoundRect(10, 10, 220, 220, 5, LCD_BLUE);
	gfx_fillCircle(20, 250, 10, LCD_MAGENTA);
	gfx_fillTriangle(120, 240, 130, 265, 110, 265, LCD_GREEN);
	gfx_fillCircle(220, 250, 10, LCD_CYAN);
	gfx_setTextSize(2);
	gfx_setCursor(40, 25);
	gfx_puts("Sismografo");
	gfx_setCursor(22, 78);
	gfx_puts("EIE-UCR-2023");
	gfx_setCursor(60, 105);
	gfx_puts("IE-0624");
	gfx_setCursor(35, 165);
	gfx_puts("Bienvenido!");
	gfx_setTextColor(LCD_GREEN, LCD_WHITE);
	gfx_setTextSize(1);
	gfx_setCursor(80, 200);
	gfx_puts("Cargando...");
	lcd_show_frame();
	msleep(4000);

    int16_t gyr_x;
    int16_t gyr_y;
    int16_t gyr_z;
	char int_to_str[7];
	char lcd_out[3];  
	while (1) {

		if (gpio_get(GPIOA, GPIO0)) {
			if(com_en){
				com_en = 0;
			}
			else{
				com_en = 1;
			}
		}

		battery = adc_update();
		if(battery <= 7){
			batt_alarm = 0;
			gpio_toggle(GPIOG, GPIO14);
		}
		else{
			batt_alarm = 1;
			gpio_clear(GPIOG, GPIO14);
		}

		gpio_clear(GPIOC, GPIO1);
		spi_send(SPI5, GYR_OUT_X_L | GYR_RNW);
		spi_read(SPI5);
		spi_send(SPI5, 0);
		gyr_x=spi_read(SPI5);
		gpio_set(GPIOC, GPIO1);
		gpio_clear(GPIOC, GPIO1);
		spi_send(SPI5, GYR_OUT_X_H | GYR_RNW);
		spi_read(SPI5);
		spi_send(SPI5, 0);
		gyr_x|=spi_read(SPI5) << 8;
		gpio_set(GPIOC, GPIO1);
		gpio_clear(GPIOC, GPIO1);
		spi_send(SPI5, GYR_OUT_Y_L | GYR_RNW);
		spi_read(SPI5);
		spi_send(SPI5, 0);
		gyr_y=spi_read(SPI5);
		gpio_set(GPIOC, GPIO1);
		gpio_clear(GPIOC, GPIO1);
		spi_send(SPI5, GYR_OUT_Y_H | GYR_RNW);
		spi_read(SPI5);
		spi_send(SPI5, 0);
		gyr_y|=spi_read(SPI5) << 8;
		gpio_set(GPIOC, GPIO1);
		gpio_clear(GPIOC, GPIO1);
		spi_send(SPI5, GYR_OUT_Z_L | GYR_RNW);
		spi_read(SPI5);
		spi_send(SPI5, 0);
		gyr_z=spi_read(SPI5);
		gpio_set(GPIOC, GPIO1);
		gpio_clear(GPIOC, GPIO1);
		spi_send(SPI5, GYR_OUT_Z_H | GYR_RNW);
		spi_read(SPI5);
		spi_send(SPI5, 0);
		gyr_z|=spi_read(SPI5) << 8;
		gpio_set(GPIOC, GPIO1);
        
        gyr_x = gyr_x*L3GD20_SENSITIVITY_250DPS;
        gyr_y = gyr_y*L3GD20_SENSITIVITY_250DPS;
        gyr_z = gyr_z*L3GD20_SENSITIVITY_250DPS;
		
		gfx_setTextColor(LCD_CYAN, LCD_BLACK);
		gfx_setTextSize(3);
		gfx_fillScreen(LCD_BLACK);
		sprintf(lcd_out, "%s", "X:");
		sprintf(int_to_str, "%d", gyr_x);
		strcat(lcd_out, int_to_str);
		gfx_setCursor(75, 36);
		gfx_puts(lcd_out);
		sprintf(lcd_out, "%s", "Y:");
		sprintf(int_to_str, "%d", gyr_y);
		strcat(lcd_out, int_to_str);
		gfx_setCursor(75, 90);
		gfx_puts(lcd_out);
		sprintf(lcd_out, "%s", "Z:");
		sprintf(int_to_str, "%d", gyr_z);
		strcat(lcd_out, int_to_str);
		gfx_setCursor(75, 144);
		gfx_puts(lcd_out);
		gfx_setTextSize(2);
		gfx_setCursor(20, 260);
		gfx_puts("% Bateria: ");
		gfx_setCursor(175, 260);
		sprintf(int_to_str, "%d", battery);
		gfx_puts(int_to_str);
		
		if(com_en){
	    	print_decimal_xyz(gyr_x); console_puts(",");
        	print_decimal_xyz(gyr_y); console_puts(",");
        	print_decimal_xyz(gyr_z); console_puts(",");
			print_decimal_xyz(batt_alarm); console_puts(",");
			print_decimal_xyz(battery); console_puts("\n");
			gpio_toggle(GPIOG, GPIO13);
			gfx_setCursor(40, 220);
			gfx_puts("Com Ser: ");
			gfx_setCursor(165, 220);
			gfx_puts("ON");
		}
		else{
			gpio_clear(GPIOG, GPIO13);
			gfx_setCursor(40, 220);
			gfx_puts("Com Ser: ");
			gfx_setCursor(165, 220);
			gfx_puts("OFF");
		}

		lcd_show_frame();
		msleep(250);
	}
	return 0;
}