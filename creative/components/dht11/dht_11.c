#include "dht_11.h"

static int count_status(int time, _Bool status)
{
    int count = 0;
    while (gpio_get_level(DH11_DATA_PIN) == status)
    {
        if (count > time)
            return -1;
        ets_delay_us(1);
        count++;
    }
    if (count == 0)
        return -1;
    return count;
}

static _Bool is_correct_call_to_dht11(void)
{
    ESP_ERROR_CHECK(gpio_set_direction(DH11_DATA_PIN, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(DH11_DATA_PIN, 1));
    ESP_ERROR_CHECK(gpio_set_level(DH11_DATA_PIN, 0));
    ets_delay_us(18000);
    ESP_ERROR_CHECK(gpio_set_level(DH11_DATA_PIN, 1));
    ets_delay_us(30);
    ESP_ERROR_CHECK(gpio_set_direction(DH11_DATA_PIN, GPIO_MODE_INPUT));
    if (count_status(80, 0) == -1)
        return false_and_printf(__func__ , __LINE__,
                                "STAGE_1 failed\n");
    if (count_status(80, 1) == -1)
        return false_and_printf(__func__ , __LINE__,
                                "STAGE_2 failed\n");
    return true;
}

static void set_bit(int result, uint8_t *arr, int *j, int i)
{
    if (result > 28)
    {
        arr[*j] <<= 1U;
        arr[*j] += 1U;
    }
    else
        arr[*j] <<= 1U;
    arr[*j]+=0;
    if (i % 8 == 0)
        (*j)++;
}

static _Bool is_correct_value(uint8_t *arr)
{
    int result = 0;
    for (int i = 1, j = 0; i < 41; i++)
    {
        if (count_status(50, 0) == -1)
            return false_and_printf(__func__ , __LINE__,
                                    "STAGE_3 failed\n");
        if ((result = count_status(70, 1)) == -1)
            return false_and_printf(__func__ , __LINE__,
                                    "STAGE_1 failed\n");
        set_bit(result, arr, &j, i);
    }
    if (arr[0] + arr[1] + arr[2] + arr[3] != arr[4])
        return false_and_printf(__func__ , __LINE__,
                                "Invalid checksum\n");
    return true;
}

void dht11_init(void)
{
    ESP_ERROR_CHECK(gpio_set_direction(DH11_POWER_PIN, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_direction(DH11_DATA_PIN, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(DH11_POWER_PIN, 1));
    ESP_ERROR_CHECK(gpio_set_level(DH11_DATA_PIN, 1));
    ets_delay_us(2000000);
}

_Bool get_value_dht11(uint8_t *temperature, uint8_t *humidity)
{
    uint8_t arr[5];
    if (!temperature || !humidity)
    {
        printf("Temperature and/or Humidity pointers is NULL!\n");
        return false;
    }
    bzero(&arr, sizeof(arr));
    *temperature = 0;
    *humidity = 0;
    if (is_correct_call_to_dht11() == false)
        return false;
    if (is_correct_value(arr) == false)
        return false;
    *humidity = arr[0];
    *temperature = arr[2];
	return true;
}
