idf_component_register(
        SRCS "dht_11.c"
        SRCS "dht_measuring_tsk.c"
        SRCS "dht_lists.c"
        INCLUDE_DIRS "."
        REQUIRES custom_error_functions
        REQUIRES structs
        REQUIRES oled
)