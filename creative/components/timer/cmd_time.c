#include "cstm_timer.h"

static _Bool time_validation_parse(t_cmd_tmp *t, t_hms_time *time)
{
	if ((strlen(t->str[2]) != 2)
		|| !isdigit((char)t->str[2][0])
		|| !isdigit((char)t->str[2][1]))
		return false;
	if ((strlen(t->str[3]) != 2)
		|| !isdigit((char)t->str[3][0])
		|| !isdigit((char)t->str[3][1]))
		return false;
	if ((strlen(t->str[4]) != 2)
		|| !isdigit((char)t->str[4][0])
		|| !isdigit((char)t->str[4][1]))
		return false;
	time->hrs = atoi(t->str[2]);
	if (time->hrs > 23)
		return false;
	time->min = atoi(t->str[3]);
	if (time->min > 59)
		return false;
	time->sec = atoi(t->str[4]);
	if (time->sec > 59)
		return false;
	return true;
}

int	set_time(void *ptr)
{
	t_data		*d = (t_data*)ptr;
	t_cmd_tmp	*t = &d->tmp;
	t_hms_time	time = {0, 0, 0};
	uint64_t	set_time = 0;

	if (!ptr || !t || t->len != 5)
		return ST_NOT_VALID;
	if (!time_validation_parse(t, &time))
		return ST_NOT_VALID;
	set_time = ((uint64_t)(time.hrs * 3600) + ((uint64_t)time.min * 60) + time.sec) * 1000000;
	timer_set_counter_value(TIMER_GROUP_0, TIMER_0, set_time);
	timer_set_alarm_value(TIMER_GROUP_0, TIMER_0,  set_time + ( 1 * TIMER_SCALE));
	return ST_SUCCESS;
}

int	set_alarm(void *ptr)
{
	t_data		*d = (t_data*)ptr;
	t_cmd_tmp	*t = &d->tmp;
	t_hms_time	time = {0, 0, 0};

	if (!ptr || !t || t->len != 5)
		return ST_NOT_VALID;
	if (!time_validation_parse(t, &time))
		return ST_NOT_VALID;
	d->alarm_time = ((uint64_t)(time.hrs * 3600) + ((uint64_t)time.min * 60) + time.sec);
	d->is_alarm_on = true;
	return ST_SUCCESS;
}
