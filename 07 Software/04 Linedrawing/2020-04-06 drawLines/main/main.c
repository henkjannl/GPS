#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <string.h>
#include <u8g2.h>

#include "sdkconfig.h"
#include "u8g2_esp32_hal.h"
#include "esp_system.h"

#include "esp_spi_flash.h"

#include <driver/i2c.h>
#include <esp_log.h>
#include <fastmath.h>

#define PIN_SDA 4
#define PIN_SCL 15
#define RESET_GPIO 16

static const float PI = 3.1415926535;

static const char *TAG = "ssd1306";

void drawDot(u8g2_t *u8g2, float x, float y, float r) {
	for(float i=x-r; i<x+r; i++) {
		float dx = i-x;
		float dy = (float) sqrtf(r*r-dx*dx);
		for(float j=y-dy; j<y+dy; j++) {
			u8g2_DrawPixel(u8g2, (uint8_t) i, (uint8_t) j); 
		} 
	}
}

void drawLine(u8g2_t *u8g2, float x1, float y1, float x2, float y2, float w) {
	float x0, dx, y0, dy;

	drawDot(u8g2, x1, y1, 0.5*w);
	drawDot(u8g2, x2, y2, 0.5*w);
		
	// First obtain the absolute lengths of dx and dy to check slope
	if (x2>x1) dx=x2-x1; else dx=x1-x2;
	if (y2>y1) dy=y2-y1; else dy=y1-y2;

	float l = sqrtf(dx*dx+dy*dy);
	
	if (dx>dy) {
		// We will move in horizontal direction and draw lines vertically
		float s = w * l/(2*dx);
		
		// Revert if needed to make dx positive
		if (x2>x1) {
			x0=x1;
			dx=x2-x1;
			y0=y1;
			dy=y2-y1;
		}
		else {
			x0=x2;
			dx=x1-x2;
			y0=y2;
			dy=y1-y2;
		}

		for(float i=0; i<dx; i++) {
			float y=y0+dy*i/dx;
			for(float j=y-s; j<y+s; j++)
				u8g2_DrawPixel(u8g2, (uint8_t) x0+i, (uint8_t) j); 
			}
	}
	else {
		// We will move in vertical  direction and draw lines horizontally
		float s = w * l/(2*dy);

		// Revert if needed to make dy positive
		if (y2>y1) {
			x0=x1;
			dx=x2-x1;
			y0=y1;
			dy=y2-y1;
		}
		else {
			x0=x2;
			dx=x1-x2;
			y0=y2;
			dy=y1-y2;
		}
		
		for(float i=0; i<dy; i++) {
			float x=x0+dx*i/dy;
			for(float j=x-s; j<x+s; j++)
				u8g2_DrawPixel(u8g2, (uint8_t) j, (uint8_t) y0+i); 
			}
	}
}	
	


void task_test_SSD1306i2c(void *ignore) {
	u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
	u8g2_esp32_hal.sda  = PIN_SDA;
	u8g2_esp32_hal.scl  = PIN_SCL;
	u8g2_esp32_hal_init(u8g2_esp32_hal);

	u8g2_t u8g2; // a structure which will contain all the data for one display
	
	//u8g2_Setup_ssd1306_i2c_128x32_univision_f(
	u8g2_Setup_ssd1306_i2c_128x64_noname_f(
		&u8g2,
		U8G2_R2, // 180 deg rotation
		//u8x8_byte_sw_i2c,
		u8g2_esp32_i2c_byte_cb,
		u8g2_esp32_gpio_and_delay_cb);  // init u8g2 structure
	u8x8_SetI2CAddress(&u8g2.u8x8,0x78);

	ESP_LOGI(TAG, "u8g2_InitDisplay");
	u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,

	ESP_LOGI(TAG, "u8g2_SetPowerSave");
	u8g2_SetPowerSave(&u8g2, 0); // wake up display

	float fi1=0;
	float fi2=0;
	while (1) {
		u8g2_ClearBuffer(&u8g2);
		for(int i=0; i<22; i++) {
			float x1 = 64+60*cos(2*PI*i/24+fi1);
			float y1 = 32+28*sin(2*PI*i/24+fi1);
			float x2 = 64+60*cos(2*PI*(i+1)/24+fi1);
			float y2 = 32+28*sin(2*PI*(i+1)/24+fi1);
			drawLine(&u8g2, x1, y1, x2, y2, (float) 8.0*i/24);

			x1 = 64+40*cos(-2*PI* i   /24-fi2);
			y1 = 32+18*sin(-2*PI* i   /24-fi2);
			x2 = 64+40*cos(-2*PI*(i+1)/24-fi2);
			y2 = 32+18*sin(-2*PI*(i+1)/24-fi2);
			drawLine(&u8g2, x1, y1, x2, y2, (float) 4.0*i/24);
			
		}
	u8g2_SendBuffer(&u8g2);
	fi1+=0.1;
	fi2+=0.215;
	}
	
	vTaskDelete(NULL);
}

void app_main()
{

	// Reset the screen 
    gpio_pad_select_gpio(RESET_GPIO);
    // Set the GPIO as a push/pull output 
    gpio_set_direction(RESET_GPIO, GPIO_MODE_OUTPUT);

	// Reset the screen
	gpio_set_level(RESET_GPIO, 0);
	vTaskDelay(50 / portTICK_PERIOD_MS);

    // Return to normal operation
	gpio_set_level(RESET_GPIO, 1);

	// Start the screen
	task_test_SSD1306i2c(NULL);
}
