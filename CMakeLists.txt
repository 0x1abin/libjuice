file(GLOB_RECURSE SOURCES "src/*.c")

idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES esp_netif pthread mbedtls)
