#include "error_functions.h"

void print_error(esp_err_t res, const char *fxname, int lineno, char *str)
{
    if (res != ESP_OK)
    {
        printf("\t\t\t"T_TRK"Function %s: line %d:\n"R, fxname, lineno);
        printf("\t\t\t"T_YEL"%s\n"R, str);
    }
}

_Bool false_and_printf(const char *fxname, int lineno, char *str)
{
    printf("\t\t\t"T_TRK"Function %s: line %d:\n"R, fxname, lineno);
    printf("\t\t\t"T_YEL"%s\n"R, str);
    return 0;
}
