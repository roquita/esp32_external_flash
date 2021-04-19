#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "esp_system.h"
#include "peripheral_spi.h"
#include "memory_external_flash.h"

#define TAG "MAIN"

virtual_external_flash_t external_flash_1;

void example_list_data_partitions(void)
{
    ESP_LOGI(TAG, "Listing data partitions:");
    esp_partition_iterator_t it = esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, NULL);

    for (; it != NULL; it = esp_partition_next(it))
    {
        const esp_partition_t *part = esp_partition_get(it);
        ESP_LOGI(TAG, "- partition '%s', subtype %d, offset 0x%x, size %d kB",
                 part->label, part->subtype, part->address, part->size / 1024);
    }

    esp_partition_iterator_release(it);
}

void app_main(void)
{
    init_native_vqspi();

    const esp_flash_spi_device_config_t device_config = {
        .host_id = VSPI_HOST,
        .cs_id = 0,
        .cs_io_num = VSPI_IOMUX_PIN_NUM_CS,
        .io_mode = SPI_FLASH_FASTRD,
        .speed = ESP_FLASH_40MHZ};

    init_external_flash(&external_flash_1, &device_config);

    create_partition_from_external_flash(&external_flash_1, "storage1");
    example_list_data_partitions();

    mount_vfs_fat_over_external_flash(&external_flash_1, "/extflash1");
    get_fatfs_usage_of_external_flash(&external_flash_1);
    test_external_flash(&external_flash_1);
}
