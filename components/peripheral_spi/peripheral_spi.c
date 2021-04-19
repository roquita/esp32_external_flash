#include "peripheral_spi.h"
#include "driver/spi_master.h"
#define TAG "SPI"

void init_native_vqspi()
{
    const spi_bus_config_t bus_config = {
        .mosi_io_num = VSPI_IOMUX_PIN_NUM_MOSI,
        .miso_io_num = VSPI_IOMUX_PIN_NUM_MISO,
        .sclk_io_num = VSPI_IOMUX_PIN_NUM_CLK,
        .quadwp_io_num = VSPI_IOMUX_PIN_NUM_WP,
        .quadhd_io_num = VSPI_IOMUX_PIN_NUM_HD,
    };
    // Initialize the SPI bus
    ESP_ERROR_CHECK(spi_bus_initialize(VSPI_HOST, &bus_config, 1));
}