#ifndef LED_H
#define LED_H

// 头文件
#include "main.h"

// 函数声明
void led_configuration(void);

void led_green_on(void);
void led_green_off(void);
void led_green_toggle(void);

void led_red_on(void);
void led_red_off(void);
void led_red_toggle(void);

void flow_led_on(uint16_t num);
void flow_led_off(uint16_t num);
void flow_led_toggle(uint16_t num);

void all_flow_led_on(void);
void all_flow_led_off(void);
void all_flow_led_toggle(void);

#endif
