#include "main.h"

void	led_on(void *ptr)
{
	(void)ptr;
	printf(T_TRK"led_on called\n"R);
}

void	led_off(void *ptr)
{
	(void)ptr;
	printf(T_TRK"led_off called\n"R);
}

static void print_result(int return_value){
	if (return_value == 0){
		printf("Pattern found.\n");
	}
	else if (return_value == REG_NOMATCH){
		printf("Pattern not found.\n");
	}
	else{
		printf("An error occured.\n");
	}
}

void parse_command_line(t_buffer *line, t_command *cmds)
{

    //"led (on/off) (1/1 2 3)"
//  todo: /^led (on|off)( [1-3])( [1-3])?( [1-3])?$/
// todo: led (on|off|pulse)( [1-3])( [1-3])?( [1-3])?( f:([0-1])(?:.([0-9]))?)?
	regex_t regex;

	int return_value = regcomp(&regex,cmds[0].reg_str,0);
	if (return_value != 0)
		printf(T_RED"ERROR REGEX\n"R);
	return_value = regexec(&regex, (char*)line->data, 0, NULL, 0);
	print_result(return_value);
    // "led on 1"
    // "led on 1 2 3"
    // "led off 1"
    // "led off 1 2 3"
    printf(T_RED"\nparse line\n"R);
}

