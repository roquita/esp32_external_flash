#pragma once
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "esp_flash.h"
#include "esp_flash_spi_init.h"
#include "esp_partition.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "diskio_wl.h"

typedef struct
{
    esp_flash_t* spi_flash;
    const esp_partition_t* partition;
    wl_handle_t s_wl_handle;
    const char *vfs_label;
} virtual_external_flash_t;

void init_external_flash(virtual_external_flash_t *v, esp_flash_spi_device_config_t *device_config);
void create_partition_from_external_flash(virtual_external_flash_t *v, const char *partition_label);
bool mount_vfs_fat_over_external_flash(virtual_external_flash_t *v, const char *vfs_label);
void get_fatfs_usage_of_external_flash(virtual_external_flash_t *v);
void test_external_flash(virtual_external_flash_t *v);
