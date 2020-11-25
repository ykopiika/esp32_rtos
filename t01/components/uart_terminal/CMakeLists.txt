idf_component_register(
        SRCS "uart_terminal.c"
        SRCS "parse_uart_event.c"
        SRCS "parse_uart_buffer.c"
        SRCS "parse_command_line.c"
        SRCS "command_registration.c"
        INCLUDE_DIRS "."
        REQUIRES cmd_leds
        REQUIRES my_lib_func
        REQUIRES custom_error_functions
)
