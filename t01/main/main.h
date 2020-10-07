#ifndef MAIN_H
#define MAIN_H

#include "oled.h"
#include <driver/adc.h>
#define MAX_VALUE 4095

typedef struct  s_app
{
    t_oled          oled;
//    TaskHandle_t    oled_tsk;
//    TaskHandle_t    photoresistor_tsk;
}               t_app;

#endif
