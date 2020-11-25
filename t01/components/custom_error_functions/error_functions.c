#include "error_functions.h"

void err_print_exit(const char *err_str, const char *flname, const char *fxname, int line)
{
	printf(T_RED"%s:%d\n%s() => %s\n"R, flname, line, fxname, err_str);
	exit(1);
}
