deps_config := \
	/home/henkjan/esp/esp-idf/components/app_trace/Kconfig \
	/home/henkjan/esp/esp-idf/components/aws_iot/Kconfig \
	/home/henkjan/esp/esp-idf/components/bt/Kconfig \
	/home/henkjan/esp/esp-idf/components/driver/Kconfig \
	/home/henkjan/esp/esp-idf/components/efuse/Kconfig \
	/home/henkjan/esp/esp-idf/components/esp32/Kconfig \
	/home/henkjan/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/home/henkjan/esp/esp-idf/components/esp_event/Kconfig \
	/home/henkjan/esp/esp-idf/components/esp_http_client/Kconfig \
	/home/henkjan/esp/esp-idf/components/esp_http_server/Kconfig \
	/home/henkjan/esp/esp-idf/components/esp_https_ota/Kconfig \
	/home/henkjan/esp/esp-idf/components/espcoredump/Kconfig \
	/home/henkjan/esp/esp-idf/components/ethernet/Kconfig \
	/home/henkjan/esp/esp-idf/components/fatfs/Kconfig \
	/home/henkjan/esp/esp-idf/components/freemodbus/Kconfig \
	/home/henkjan/esp/esp-idf/components/freertos/Kconfig \
	/home/henkjan/esp/esp-idf/components/heap/Kconfig \
	/home/henkjan/esp/esp-idf/components/libsodium/Kconfig \
	/home/henkjan/esp/esp-idf/components/log/Kconfig \
	/home/henkjan/esp/esp-idf/components/lwip/Kconfig \
	/home/henkjan/esp/esp-idf/components/mbedtls/Kconfig \
	/home/henkjan/esp/esp-idf/components/mdns/Kconfig \
	/home/henkjan/esp/esp-idf/components/mqtt/Kconfig \
	/home/henkjan/esp/esp-idf/components/nvs_flash/Kconfig \
	/home/henkjan/esp/esp-idf/components/openssl/Kconfig \
	/home/henkjan/esp/esp-idf/components/pthread/Kconfig \
	/home/henkjan/esp/esp-idf/components/spi_flash/Kconfig \
	/home/henkjan/esp/esp-idf/components/spiffs/Kconfig \
	/home/henkjan/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/home/henkjan/esp/esp-idf/components/unity/Kconfig \
	/home/henkjan/esp/esp-idf/components/vfs/Kconfig \
	/home/henkjan/esp/esp-idf/components/wear_levelling/Kconfig \
	/home/henkjan/esp/esp-idf/components/app_update/Kconfig.projbuild \
	/home/henkjan/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/henkjan/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/henkjan/esp/test_gps/main/Kconfig.projbuild \
	/home/henkjan/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/henkjan/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_TARGET)" "esp32"
include/config/auto.conf: FORCE
endif
ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
