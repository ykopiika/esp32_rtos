idf_component_register(
        SRCS "timer_functions.c"
        SRCS "cmd_time.c"
        INCLUDE_DIRS "."
        REQUIRES structs
        REQUIRES oled
)
