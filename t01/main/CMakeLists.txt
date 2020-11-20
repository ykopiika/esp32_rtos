idf_component_register(
    SRCS "main.c"
    SRCS "parse_uart_event.c"
    SRCS "parse_uart_buffer.c"
    SRCS "parse_command_line.c"
    SRCS "command_registration.c"
    SRCS "search_command.c"
    SRCS "led_functions.c"
    SRCS "tasks.c"
    INCLUDE_DIRS "."
    REQUIRES my_lib_func
    REQUIRES custom_error_functions
    REQUIRES flashing_led
)
