idf_component_register(
        SRCS "speaker.c"
        INCLUDE_DIRS "."
        REQUIRES structs
        REQUIRES custom_error_functions
)