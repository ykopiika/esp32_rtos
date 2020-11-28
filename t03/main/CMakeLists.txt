idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES custom_error_functions
    REQUIRES uart_terminal
    REQUIRES dht11
    REQUIRES structs
)
