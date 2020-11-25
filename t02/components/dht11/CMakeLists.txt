idf_component_register(
        SRCS "dht_11.c"
        SRCS "dht_measuring_tsk.c"
        INCLUDE_DIRS "."
        REQUIRES custom_error_functions
)