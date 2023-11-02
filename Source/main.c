/************************************************************************************************************************************************
* 文件名称：main.c
* 文件描述：主函数
* 文件作者：bo.liangmin
* 版本信息：20230601--文件创建
************************************************************************************************************************************************/



/************************************************************************************************************************************************
* include
************************************************************************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "framework_hardware.h"
#include "framework_led.h"
#include "framework_epd.h"

#include "pic.h"

#include "hal_board.h"
#include "hal_led.h"

#include "hal_rtc.h"



/************************************************************************************************************************************************
* define
************************************************************************************************************************************************/
#define main_task_PRIORITY (configMAX_PRIORITIES - 2)




static void main_task(void *pvParameters)
{
    static uint32_t old_tick = 0;
    static uint32_t new_tick = 0;

    static uint32_t ms_ticks = 0;
    static uint32_t cntttttt = 0;

    uint32_t blmcc = 0;
    FrameWork_LED_StateSet(LED_RED, LED_STATE_FLASH);
    FrameWork_LED_StateSet(LED_BLUE, LED_STATE_ON);

    //FrameWork_EPD_SetDeviceType(EPD_DEVICE_E029A12);
    FrameWork_EPD_SetDeviceType(EPD_DEVICE_E029A10);
    //FrameWork_EPD_SetDeviceType(EPD_DEVICE_WFT029);
    //FrameWork_EPD_SetDeviceType(EPD_DEVICE_A1360);

    while(1)
    {
        FrameWork_EPD_DeviceInit();
        //FrameWork_EPD_WriteScreenALL(gImage_E029A12, NULL);
        FrameWork_EPD_WriteScreenALL(gImage_E029A10, gImage_E029A10);
        //FrameWork_EPD_WriteScreenALL(gImage_WFT029, NULL);
        //FrameWork_EPD_WriteScreenALL(gImage_A1360, gImage_A1360);
        FrameWork_EPD_DeviceSleep();
        vTaskDelay(10000);

        FrameWork_EPD_DeviceInit();
        FrameWork_EPD_WriteScreenALLClean();
        vTaskDelay(10000);
    }
}

void main(void)
 {
    FrameWork_Hardware_Init();

    xTaskCreate(main_task, "main_task", 200, NULL, main_task_PRIORITY, NULL);
    vTaskStartScheduler();

    while(1)
    {
        //
    }
}




