idf_component_register(
        SRCS "oled_init.c"
        SRCS "oled_functions.c"
        INCLUDE_DIRS "includes"
        REQUIRES custom_error_functions
)
