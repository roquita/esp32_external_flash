idf_component_register(SRCS "memory_external_flash.c"
                    INCLUDE_DIRS "include"
                    REQUIRES spi_flash fatfs)
