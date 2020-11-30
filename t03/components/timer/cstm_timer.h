#ifndef CSTM_TIMER_H
# define CSTM_TIMER_H

#include "structs.h"
#include "oled.h"

void	init_timer(t_data *data);
void	time_to_oled(void *ptr);
int		set_time(void *ptr);

#endif
