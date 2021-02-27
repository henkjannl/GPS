/* NMEA Parser example, that decode data stream from GPS receiver

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nmea_parser.h"

static const char *TAG = "gps_demo";

#define TIME_ZONE (+2)   //Netherlands, summertime
#define YEAR_BASE (2000) //date in GPS starts from 2000

/**
 * @brief GPS Event Handler
 *
 * @param event_handler_arg handler specific arguments
 * @param event_base event base, here is fixed to ESP_NMEA_EVENT
 * @param event_id event id
 * @param event_data event specific arguments
 */
static void gps_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    gps_t *gps = NULL;
    switch (event_id) {
    case GPS_UPDATE:
        gps = (gps_t *)event_data;
        /* print information parsed from GPS statements */
        ESP_LOGI(TAG, "%d/%d/%d %d:%d:%d \tlatitude = %.05f°N\tlongtitude = %.05f°E\taltitude = %.02fm",
                 gps->date.year + YEAR_BASE, gps->date.month, gps->date.day,
                 gps->tim.hour + TIME_ZONE, gps->tim.minute, gps->tim.second,
                 gps->latitude, gps->longitude, gps->altitude);

        ESP_LOGI(TAG, "speed %f m/s  course over ground %f magnetic variation %f",
                 gps->speed, gps->cog, gps->variation);

        ESP_LOGI(TAG, "Satelites in use: %d Dilution of precision hor %f ver %f",
                 gps->sats_in_use, gps->dop_h, gps->dop_v);

		if (gps->valid==true) ESP_LOGI(TAG, "GPS valid"); else ESP_LOGI(TAG, "GPS not valid"); 
				
		switch (gps->fix) {
			case GPS_FIX_INVALID:
				ESP_LOGI(TAG, "No GPS fix");
				break;
				
			case GPS_FIX_GPS:
				ESP_LOGI(TAG, "GPS fix");
				break;

			case GPS_FIX_DGPS:
				ESP_LOGI(TAG, "Differential GPS fix");
				break;
		}

		switch (gps->fix_mode) {
			case GPS_MODE_INVALID:
				ESP_LOGI(TAG, "Not fixed");
				break;
				
			case GPS_MODE_2D:
				ESP_LOGI(TAG, "2D GPS fix");
				break;

			case GPS_MODE_3D:
				ESP_LOGI(TAG, "3D GPS fix");
				break;
		}
	
		uint8_t *currentSatID = gps->sats_id_in_use;
		gps_satellite_t *currentSat = gps->sats_desc_in_view;
		for(int i = 0; i<gps->sats_in_use; i++) {
			ESP_LOGI(TAG, "  ID %d sat[%d] elevation: %d azimuth: %d snr: %d",
					 *currentSatID, currentSat->num, currentSat->elevation, 
					 currentSat->azimuth, currentSat->snr);
			currentSatID++;
			currentSat++;
		}
		ESP_LOGI(TAG, "\n\n");

        break;
        
    case GPS_UNKNOWN:
        /* print unknown statements */
        ESP_LOGW(TAG, "Unknown statement:%s", (char *)event_data);
        break;
    default:
        break;
    }
}

void app_main()
{
    /* NMEA parser configuration */
    nmea_parser_config_t config = NMEA_PARSER_CONFIG_DEFAULT();

	// Override the default RX pin number
	config.uart.rx_pin = 17;
    
    /* init NMEA parser library */
    nmea_parser_handle_t nmea_hdl = nmea_parser_init(&config);

    /* register event handler for NMEA parser library */
    nmea_parser_add_handler(nmea_hdl, gps_event_handler, NULL);

    vTaskDelay(10000 / portTICK_PERIOD_MS);

    /* unregister event handler */
    nmea_parser_remove_handler(nmea_hdl, gps_event_handler);

    /* deinit NMEA parser library */
    nmea_parser_deinit(nmea_hdl);
}
