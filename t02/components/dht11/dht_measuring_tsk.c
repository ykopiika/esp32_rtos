#include "dht_11.h"

void dht_measuring_tsk(void *param)
{
	t_dht dht = {0,0};
	_Bool is_on = true;

	while (is_on)
	{
		get_value_dht11(&dht.tem, &dht.hum);
		printf("DHT t = %d | h = %d\n", dht.tem, dht.hum);
		vTaskDelay(5000 / portTICK_PERIOD_MS);
	}
}

