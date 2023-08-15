/***********************************************************************************************************************
* 文件名称: framework_led.c
* 文件描述: LED指示灯相关封装函数
* 维护人员: bo.liangmin
* 版本信息: 20230815--代码创建
***********************************************************************************************************************/



/***********************************************************************************************************************
* include
***********************************************************************************************************************/
#include "hal_led.h"
#include "framework_led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"



/***********************************************************************************************************************
* define
***********************************************************************************************************************/
static xTimerHandle xTimerLEDRedFlash = NULL;
static xTimerHandle xTimerLEDBlueFlash = NULL;



/***********************************************************************************
* 函数名称: static void FrameWork_LED_LEDRedFlashTCB(xTimerHandle xTimer)
* 函数说明: 红色LED管脚翻转，实现闪烁
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static void FrameWork_LED_LEDRedFlashTCB(xTimerHandle xTimer)
{
    HAL_LED_Toggle(LED_RED_PIN);
}

/***********************************************************************************
* 函数名称: static void FrameWork_LED_LEDBlueFlashTCB(xTimerHandle xTimer)
* 函数说明: 蓝色LED管脚翻转，实现闪烁
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static void FrameWork_LED_LEDBlueFlashTCB(xTimerHandle xTimer)
{
    HAL_LED_Toggle(LED_BLUE_PIN);
}

/***********************************************************************************
* 函数名称: static void FrameWork_LED_Flash(led_num_t led_num)
* 函数说明: 设置LED指示灯闪烁
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static void FrameWork_LED_Flash(led_num_t led_num)
{
    switch(led_num)
    {
        case LED_RED:
        {
            xTimerLEDRedFlash = xTimerCreate("led_red flash", pdMS_TO_TICKS(200), pdTRUE, (void *)0, FrameWork_LED_LEDRedFlashTCB);
            if(NULL != xTimerLEDRedFlash)
            {
                xTimerStart(xTimerLEDRedFlash, 0);
            }
            break;
        }

        case LED_BLUE:
        {
            xTimerLEDBlueFlash = xTimerCreate("led_blue flash", pdMS_TO_TICKS(2000), pdTRUE, (void *)0, FrameWork_LED_LEDBlueFlashTCB);
            if(NULL != xTimerLEDBlueFlash)
            {
                xTimerStart(xTimerLEDBlueFlash, 0);
            }
            break;
        }

        default:
        {
            break;
        }
    }
}

/***********************************************************************************
* 函数名称: static void FrameWork_LED_On(led_num_t led_num)
* 函数说明: 设置LED指示灯常亮
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static void FrameWork_LED_On(led_num_t led_num)
{
    switch(led_num)
    {
        case LED_RED:
        {
            if(NULL != xTimerLEDRedFlash)
            {
                xTimerDelete(xTimerLEDRedFlash, 0);
            }
            HAL_LED_TurnOn(LED_RED_PIN);
            break;
        }

        case LED_BLUE:
        {
            if(NULL != xTimerLEDBlueFlash)
            {
                xTimerDelete(xTimerLEDBlueFlash, 0);
            }
            HAL_LED_TurnOn(LED_BLUE_PIN);
            break;
        }

        default:
        {
            break;
        }
    }
}

/***********************************************************************************
* 函数名称: static void FrameWork_LED_Off(led_num_t led_num)
* 函数说明: 设置LED指示灯常灭，设备关闭或者处于特殊模式
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static void FrameWork_LED_Off(led_num_t led_num)
{
    switch(led_num)
    {
        case LED_RED:
        {
            if(NULL != xTimerLEDRedFlash)
            {
                xTimerDelete(xTimerLEDRedFlash, 0);
            }
            HAL_LED_TurnOff(LED_RED_PIN);
            break;
        }

        case LED_BLUE:
        {
            if(NULL != xTimerLEDBlueFlash)
            {
                xTimerDelete(xTimerLEDBlueFlash, 0);
            }
            HAL_LED_TurnOff(LED_BLUE_PIN);
            break;
        }

        default:
        {
            break;
        }
    }
}

/***********************************************************************************
* 函数名称: void FrameWork_LED_StateSet(led_num_t led_num, led_state_t led_state)
* 函数说明: 设置LED指示灯状态
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
void FrameWork_LED_StateSet(led_num_t led_num, led_state_t led_state)
{
    switch(led_state)
    {
        case LED_STATE_FLASH:
        {
            FrameWork_LED_Flash(led_num);
            break;
        }

        case LED_STATE_ON:
        {
            FrameWork_LED_On(led_num);
            break;
        }

        case LED_STATE_OFF:
        {
            FrameWork_LED_Off(led_num);
            break;
        }

        default:
        {
            break;
        }
    }
}



/***********************************************************************************************************************
* end
***********************************************************************************************************************/


