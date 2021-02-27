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


#define PIN_SDA 4
#define PIN_SCL 15
#define RESET_GPIO 16

#define SUNSHINE_WIDTH 128
#define SUNSHINE_HEIGHT 64

static unsigned char SUNSHINE_BITS[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0xe0, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f,
   0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x0c, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18,
   0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x40, 0x00, 0x40, 0x20,
   0x14, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
   0xb0, 0x8c, 0xb8, 0x58, 0x6e, 0xc4, 0xc6, 0x06, 0x00, 0x00, 0x00, 0x18,
   0x18, 0x00, 0x00, 0x00, 0x10, 0x88, 0x89, 0x09, 0x44, 0x44, 0x44, 0x08,
   0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x30, 0x84, 0x08, 0x19,
   0xc6, 0x64, 0x4c, 0x0d, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
   0xc0, 0x8c, 0x08, 0x61, 0x44, 0x44, 0x48, 0x05, 0x00, 0x00, 0x00, 0x18,
   0x18, 0x00, 0x00, 0x00, 0x80, 0x88, 0x08, 0x41, 0x42, 0x44, 0x64, 0x00,
   0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x80, 0xc9, 0x09, 0x41,
   0xc6, 0x44, 0x4c, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
   0xf0, 0xb8, 0x88, 0x79, 0x44, 0x64, 0x88, 0x07, 0x00, 0x00, 0x00, 0x18,
   0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18,
   0x98, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
   0xaa, 0xaa, 0xaa, 0x18, 0x18, 0x49, 0x4a, 0x4a, 0x4a, 0x4a, 0x4a, 0x4a,
   0x4a, 0x4a, 0x4a, 0x4a, 0x4a, 0x4a, 0x4a, 0x19, 0x18, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18,
   0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x01, 0x20, 0x80, 0x00,
   0x01, 0x01, 0x02, 0x80, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
   0x02, 0x40, 0x00, 0x81, 0x80, 0x00, 0x02, 0x40, 0x00, 0x00, 0x00, 0x18,
   0x18, 0x00, 0x00, 0x00, 0x04, 0x80, 0x00, 0x01, 0x81, 0x00, 0x01, 0x20,
   0x00, 0x00, 0x00, 0x18, 0x18, 0xc0, 0x00, 0x00, 0x08, 0x80, 0x00, 0x81,
   0x80, 0x00, 0x01, 0x10, 0x00, 0x00, 0x03, 0x18, 0x18, 0x00, 0x01, 0x00,
   0x10, 0x00, 0x01, 0x01, 0x81, 0x80, 0x00, 0x08, 0x00, 0x80, 0x00, 0x18,
   0x18, 0x00, 0x06, 0x00, 0x20, 0x00, 0x01, 0x82, 0x40, 0x80, 0x00, 0x04,
   0x00, 0x60, 0x00, 0x18, 0x18, 0x00, 0x18, 0x00, 0x40, 0x00, 0x02, 0x02,
   0x41, 0x40, 0x00, 0x02, 0x00, 0x18, 0x00, 0x18, 0x18, 0x00, 0x60, 0x00,
   0x80, 0x00, 0x04, 0x82, 0x40, 0x40, 0x00, 0x01, 0x00, 0x06, 0x00, 0x18,
   0x18, 0x00, 0x80, 0x00, 0x00, 0x01, 0x04, 0x02, 0x41, 0x20, 0x80, 0x00,
   0x00, 0x01, 0x00, 0x18, 0x18, 0x00, 0x00, 0x03, 0x00, 0x02, 0x08, 0x84,
   0x20, 0x10, 0x40, 0x00, 0xc0, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x0c,
   0x00, 0x04, 0x08, 0x04, 0x21, 0x10, 0x20, 0x00, 0x30, 0x00, 0x00, 0x18,
   0x18, 0x00, 0x00, 0x30, 0x00, 0x08, 0x10, 0x84, 0x20, 0x08, 0x10, 0x00,
   0x08, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x40, 0x00, 0x10, 0x10, 0x04,
   0x21, 0x08, 0x08, 0x00, 0x06, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x80,
   0x01, 0x20, 0x20, 0x00, 0x00, 0x04, 0x04, 0x80, 0x01, 0x00, 0x00, 0x18,
   0x18, 0x00, 0x00, 0x00, 0x06, 0x40, 0x20, 0x00, 0x00, 0x04, 0x02, 0x60,
   0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x08, 0x80, 0x00, 0x00,
   0x00, 0x00, 0x01, 0x10, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
   0x30, 0x00, 0x01, 0x00, 0x00, 0x80, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x18,
   0x18, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x02, 0xe0, 0x0f, 0x40, 0x00, 0x03,
   0x00, 0x00, 0x00, 0x18, 0x18, 0x70, 0x00, 0x00, 0x00, 0x03, 0x04, 0xfc,
   0x3f, 0x20, 0xc0, 0x00, 0x00, 0x00, 0x0f, 0x18, 0x18, 0x00, 0x07, 0x00,
   0x00, 0x04, 0x00, 0xff, 0xff, 0x00, 0x20, 0x00, 0x00, 0xe0, 0x00, 0x18,
   0x18, 0x00, 0x78, 0x00, 0x00, 0x18, 0x80, 0xff, 0xff, 0x03, 0x18, 0x00,
   0x00, 0x1e, 0x00, 0x18, 0x18, 0x00, 0x80, 0x07, 0x00, 0x60, 0xe0, 0xff,
   0xff, 0x07, 0x06, 0x00, 0xc0, 0x01, 0x00, 0x18, 0x18, 0x00, 0x00, 0x38,
   0x00, 0x80, 0xe0, 0xff, 0xff, 0x07, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x18,
   0x18, 0x00, 0x00, 0xc0, 0x03, 0x00, 0xf0, 0xff, 0xff, 0x0f, 0x00, 0xc0,
   0x03, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x3c, 0x00, 0xf8, 0xff,
   0xff, 0x1f, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
   0xc0, 0x03, 0xf8, 0xff, 0xff, 0x1f, 0x80, 0x03, 0x00, 0x00, 0x00, 0x18,
   0x18, 0x00, 0x00, 0x00, 0x00, 0x1c, 0xfc, 0xff, 0xff, 0x3f, 0x78, 0x00,
   0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff,
   0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xfe, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18,
   0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0x7f, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff,
   0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0xa8, 0xaa, 0xaa,
   0xaa, 0x0a, 0xfe, 0xff, 0xff, 0x7f, 0x50, 0x55, 0x55, 0x55, 0x15, 0x18,
   0x18, 0x48, 0x49, 0x49, 0x49, 0x15, 0xfe, 0xff, 0xff, 0x7f, 0x90, 0x92,
   0x92, 0x92, 0x2a, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff,
   0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xfe, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18,
   0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xff, 0x3f, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff,
   0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18,
   0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x0c, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0xc0, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00 };

static const char *TAG = "ssd1306";

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

	ESP_LOGI(TAG, "u8g2_SetFont");
    u8g2_SetFont(&u8g2, u8g2_font_logisoso16_tr);
	ESP_LOGI(TAG, "u8g2_DrawStr");
    u8g2_DrawStr(&u8g2, 2,17,"HenkJan");
	ESP_LOGI(TAG, "u8g2_SendBuffer");
	u8g2_SendBuffer(&u8g2);
	
	typedef struct point point;

	struct point {
	   int    x;
	   int    y;
	};

	const int NUM_POINTS = 250;	
	point points [NUM_POINTS];
 
	point currentPoint;
    currentPoint.x=64;
    currentPoint.y=32;
    
	for(int i=0; i<NUM_POINTS; i++)
		points[i]=currentPoint;

	int pointer = 0;
		
	while(1) {
		u8g2_ClearBuffer(&u8g2);

		u8g2_DrawXBM(&u8g2, 0, 0, SUNSHINE_WIDTH, SUNSHINE_HEIGHT, SUNSHINE_BITS);

		for(int i=0; i<10; i++) {
			if( (esp_random() & 0x1)== 0)
				{
				currentPoint.x++;
				if (currentPoint.x>127) currentPoint.x=127;
			}
			else
				{
				currentPoint.x--;
				if (currentPoint.x<0) currentPoint.x=0;
				}

			if( (esp_random() & 0x1)== 0)
				{
				currentPoint.y++;
				if(currentPoint.y>63) currentPoint.y=63;
			}
			else
				{
				currentPoint.y--;
				if(currentPoint.y<0) currentPoint.y=0;
			}
			
			points[pointer]=currentPoint;
			
			pointer++;
			if (pointer>NUM_POINTS) pointer=0;
		}
		
		for(int i=0; i<NUM_POINTS-1; i++) 
			if( (i+1) != pointer)
				u8g2_DrawLine(&u8g2, points[i].x, points[i].y, points[i+1].x, points[i+1].y);
				
		u8g2_SendBuffer(&u8g2);
	}

	ESP_LOGI(TAG, "All done!");

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