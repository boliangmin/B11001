/************************************************************************************************************************************************
* 文件名称：hal_led.c
* 文件描述：LED相关驱动函数
* 文件作者：bo.liangmin
* 版本信息：20230601--文件创建
* 变更信息：20230814--修改函数接口
************************************************************************************************************************************************/



/************************************************************************************************************************************************
* include
************************************************************************************************************************************************/
#include "nrf_gpio.h"
#include "hal_led.h"



/************************************************************************************************************************************************
* define
************************************************************************************************************************************************/



/************************************************************************************************************************************************
* 函数名称：void HAL_LED_TurnOn(uint32_t led_pin)
* 函数说明：LED开
* 输入参数：无
* 输出参数：无
* 返回数值：无
************************************************************************************************************************************************/
void HAL_LED_TurnOn(uint32_t led_pin)
{
    nrf_gpio_pin_clear(led_pin);
}

/************************************************************************************************************************************************
* 函数名称：void HAL_LED_TurnOff(uint32_t led_pin)
* 函数说明：LED关
* 输入参数：无
* 输出参数：无
* 返回数值：无
************************************************************************************************************************************************/
void HAL_LED_TurnOff(uint32_t led_pin)
{
    nrf_gpio_pin_set(led_pin);
}

/************************************************************************************************************************************************
* 函数名称：void HAL_LED_Toggle(uint32_t led_pin)
* 函数说明：LED翻转
* 输入参数：无
* 输出参数：无
* 返回数值：无
************************************************************************************************************************************************/
void HAL_LED_Toggle(uint32_t led_pin)
{
    nrf_gpio_pin_toggle(led_pin);
}



/************************************************************************************************************************************************
* end
************************************************************************************************************************************************/


