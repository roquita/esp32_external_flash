#include "memory_external_flash.h"

#define TAG "EXTERNAL_FLASH"

void init_external_flash(virtual_external_flash_t *v, esp_flash_spi_device_config_t *device_config)
{
    // Add device to the SPI bus
    ESP_ERROR_CHECK(spi_bus_add_flash_device(&v->spi_flash, device_config));

    // Probe the Flash chip and initialize it
    esp_err_t err = esp_flash_init(v->spi_flash);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize external Flash: %s (0x%x)", esp_err_to_name(err), err);
        return;
    }

    // Print out the ID and size
    uint32_t id;
    ESP_ERROR_CHECK(esp_flash_read_id(v->spi_flash, &id));
    ESP_LOGI(TAG, "Initialized external Flash, size=%d KB, ID=0x%x", (v->spi_flash)->size / 1024, id);
}

void create_partition_from_external_flash(virtual_external_flash_t *v, const char *partition_label)
{
    ESP_LOGI(TAG, "Adding external Flash as a partition, label=\"%s\", size=%d KB", partition_label, (v->spi_flash)->size / 1024);
    ESP_ERROR_CHECK(esp_partition_register_external(v->spi_flash, 0, (v->spi_flash)->size, partition_label, ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_FAT, &(v->partition)));
}

bool mount_vfs_fat_over_external_flash(virtual_external_flash_t *v, const char *vfs_label)
{
    ESP_LOGI(TAG, "Mounting FAT filesystem");
    const esp_vfs_fat_mount_config_t mount_config = {
        .max_files = 4,
        .format_if_mount_failed = true,
        .allocation_unit_size = CONFIG_WL_SECTOR_SIZE};
    esp_err_t err = esp_vfs_fat_spiflash_mount(vfs_label, (v->partition)->label, &mount_config, &(v->s_wl_handle));
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to mount FATFS (%s)", esp_err_to_name(err));
        return false;
    }
    v->vfs_label = vfs_label;
    return true;
}

void get_fatfs_usage_of_external_flash(virtual_external_flash_t *v)
{
    wl_handle_t s_wl_handle = v->s_wl_handle;
    uint8_t pdrv = ff_diskio_get_pdrv_wl(s_wl_handle);
    char drv[3] = {(char)('0' + pdrv), ':', 0};

    FATFS *fs;
    size_t free_clusters;
    int res = f_getfree(drv, &free_clusters, &fs);
    assert(res == FR_OK);
    size_t total_sectors = (fs->n_fatent - 2) * fs->csize;
    size_t free_sectors = free_clusters * fs->csize;

    size_t out_total_bytes;
    size_t out_free_bytes;

    out_total_bytes = total_sectors * fs->ssize;
    out_free_bytes = free_sectors * fs->ssize;

    ESP_LOGI(TAG, "FAT FS: %d kB total, %d kB free", out_total_bytes / 1024, out_free_bytes / 1024);
}

void test_external_flash(virtual_external_flash_t *v)
{
    // create file path for hello.txt
    char file_path[25];
    sprintf(file_path, "%s/hello.txt", v->vfs_label);
    ESP_LOGI(TAG, "test file path:%s", file_path);

    // Create a file in FAT FS
    ESP_LOGI(TAG, "Opening file");
    FILE *f = fopen(file_path, "wb");
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return;
    }
    fprintf(f, "Written using ESP-IDF %s\n", esp_get_idf_version());
    fclose(f);
    ESP_LOGI(TAG, "File written");

    // Open file for reading
    ESP_LOGI(TAG, "Reading file");
    f = fopen(file_path, "rb");
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return;
    }
    char line[128];
    fgets(line, sizeof(line), f);
    fclose(f);
    // strip newline
    char *pos = strchr(line, '\n');
    if (pos)
    {
        *pos = '\0';
    }
    ESP_LOGI(TAG, "Read from file: '%s'", line);
}
