/************************************************************************************************************************************************
* 文件名称：main.c
* 文件描述：主函数
* 文件作者：bo.liangmin
* 版本信息：20230601--文件创建
************************************************************************************************************************************************/


#include "hal_rtc.h"
#include "hal_led.h"


void delay(uint16_t ms);





void delay(uint16_t ms)
{
    uint32_t blmcc = HAL_RTC_GetSysTicks();

    while(1)
    {
        if(HAL_RTC_GetSysTicks() - blmcc >= ms)
        {
            break;
        }
    }
}



int main(void)
{
    HAL_RTC_Init();

    HAL_LED_Init(LED_RED);
    HAL_LED_Init(LED_BLUE);


    while(1)
    {
        HAL_LED_TurnOff(LED_BLUE);
        HAL_LED_TurnOn(LED_RED);
        delay(5000);
        HAL_LED_TurnOn(LED_BLUE);
        HAL_LED_TurnOff(LED_RED);
        delay(5000);
    }
}




