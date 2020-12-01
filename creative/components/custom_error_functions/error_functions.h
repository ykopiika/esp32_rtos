#ifndef CUSTOM_ERROR_FUNCTIONS_H
# define CUSTOM_ERROR_FUNCTIONS_H

#include "esp_err.h"

#define T_BLK "\x1b[30m" // 30 - text is 	black
#define T_RED "\x1b[31m" // 31 - text is 	red
#define T_GRN "\x1b[32m" // 32 - text is 	green
#define T_YEL "\x1b[33m" // 33 - text is 	yellow
#define T_BLU "\x1b[34m" // 34 - text is 	blue
#define T_PNK "\x1b[35m" // 35 - text is 	pink
#define T_TRK "\x1b[36m" // 36 - text is 	turquoise

#define M_BLK "\x1b[40m" // 40 - black 		marker for text
#define M_RED "\x1b[41m" // 41 - red 		marker for text
#define M_GRN "\x1b[42m" // 42 - green		marker for text
#define M_YEL "\x1b[43m" // 43 - yellow		marker for text
#define M_BLU "\x1b[44m" // 44 - blue		marker for text
#define M_PNK "\x1b[45m" // 45 - pink		marker for text
#define M_TRK "\x1b[46m" // 46 - turquoise	marker for text

#define T_BLD "\x1b[1m"  //  1  bold 		text
#define T_CRS "\x1b[3m"  //  3  cursive 	text
#define T_UNL "\x1b[4m"  //  4  underline 	text
#define T_SLV "\x1b[2m"  //  2  text is 	silver

#define R "\x1B[0m"		 // reset


#define ERR_MALLOC "Fatal: malloc failed!"
#define ERR_VAL_NULL "Fatal: value is NULL!"

enum			e_state
{
	ST_NOT_FOUND = 0,
	ST_NOT_VALID = 1,
	ST_SUCCESS = 2,
	ST_OK = 3,
	ST_CMD_FAILED = 4,
};

_Bool false_and_printf(const char *fxname, int lineno, char *str);
void print_error(esp_err_t res, const char *fxname, int lineno, char *str);

void err_print_exit(const char *err_str,
					const char *flname,
					const char *fxname,
					int line);

#endif
