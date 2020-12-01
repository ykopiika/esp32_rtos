#include "error_functions.h"

void err_print_exit(const char *err_str, const char *flname, const char *fxname, int line)
{
	printf(T_RED"%s:%d\n%s() => %s\n"R, flname, line, fxname, err_str);
	exit(1);
}

_Bool false_and_printf(const char *fxname, int lineno, char *str)
{
	printf("\t\t\t"T_TRK"Function %s: line %d:\n"R, fxname, lineno);
	printf("\t\t\t"T_YEL"%s\n"R, str);
	return 0;
}

void print_error(esp_err_t res, const char *fxname, int lineno, char *str)
{
	if (res != ESP_OK)
	{
		printf("\t\t\t"T_TRK"Function %s: line %d:\n"R, fxname, lineno);
		printf("\t\t\t"T_YEL"%s\n"R, str);
	}
}
