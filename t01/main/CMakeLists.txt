idf_component_register(
    SRCS "main.c"
    SRCS "parse_uart_event.c"
    SRCS "parse_uart_buffer.c"
    SRCS "parse_command_line.c"
    INCLUDE_DIRS "."
    REQUIRES my_lib_func
    REQUIRES custom_error_functions
)
