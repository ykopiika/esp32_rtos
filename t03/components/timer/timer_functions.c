#include "cstm_timer.h"

void time_to_oled(void *ptr)
{
	t_data		*d = (t_data*)ptr;
	char		str[12];
	t_hms_time	t = (t_hms_time){0,0,0};
	uint32_t	time = 0;
	_Bool		is_on = true;

	timer_start(TIMER_GROUP_0, TIMER_0);
	while (is_on)
	{
		xTaskNotifyWait(0xffffffff, 0, &time, portMAX_DELAY);
		if(time >= 86400)
		{
			t = (t_hms_time){0,0,0};
			timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0);
			timer_set_alarm_value(TIMER_GROUP_0, TIMER_0,  1 * TIMER_SCALE);
		}
		else
		{
			t.hrs = time / 3600;
			t.min = (time / 60) % 60;
			t.sec = time % 60;
//			t.hrs = (time) / 3600;
//			t.min = (time - (3600 * t.hrs)) / 60;
//			t.sec = time - (3600 * t.hrs) - (t.sec * 60);
		}
		sprintf(str, "%02d:%02d:%02d", t.hrs, t.min, t.sec);
		str_to_oled_8x16(&d->oled, str, 3);
	}
}

static void IRAM_ATTR timer_interrupt(void *ptr)
{
	t_data		*d = (t_data*)ptr;

	timer_spinlock_take(TIMER_GROUP_0);
	uint64_t timer_val = timer_group_get_counter_value_in_isr(TIMER_GROUP_0, TIMER_0);
	timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, 0);
	uint64_t next_alarm = timer_val + ( 1 * TIMER_SCALE);
	timer_group_set_alarm_value_in_isr(TIMER_GROUP_0, TIMER_0, next_alarm);
	timer_group_enable_alarm_in_isr(TIMER_GROUP_0, 0);
	xTaskNotifyFromISR(d->oled_time_task, timer_val / 1000000, eSetValueWithOverwrite, 0);
	timer_spinlock_give(TIMER_GROUP_0);
}

void init_timer(t_data *data)
{
	timer_config_t timer_config_1 = {
			.alarm_en = TIMER_ALARM_DIS,
			.counter_en = TIMER_START,
			.intr_type = TIMER_INTR_LEVEL,
			.counter_dir = TIMER_COUNT_UP,
			.auto_reload = TIMER_AUTORELOAD_DIS,
			.divider = TIMER_DIVIDER,
	};
	ESP_ERROR_CHECK(timer_init(TIMER_GROUP_1, TIMER_0, &timer_config_1));
	timer_config_t timer_config_2 = {
			.alarm_en = TIMER_ALARM_EN,
			.counter_en = TIMER_PAUSE,
			.intr_type = TIMER_INTR_LEVEL,
			.counter_dir = TIMER_COUNT_UP,
			.auto_reload = TIMER_AUTORELOAD_DIS,
			.divider = TIMER_DIVIDER,
	};
	ESP_ERROR_CHECK(timer_init(TIMER_GROUP_0, 0, &timer_config_2));
	ESP_ERROR_CHECK(timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, 1 * TIMER_SCALE));
	ESP_ERROR_CHECK(timer_enable_intr(TIMER_GROUP_0, TIMER_0));
	ESP_ERROR_CHECK(timer_isr_register(TIMER_GROUP_0, TIMER_0, timer_interrupt,
									   (void *) data, ESP_INTR_FLAG_IRAM, NULL));
}

