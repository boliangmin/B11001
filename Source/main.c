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

#include "fonts.h"
#include "app_epd.h"




/************************************************************************************************************************************************
* define
************************************************************************************************************************************************/
#define main_task_PRIORITY (configMAX_PRIORITIES - 2)


#define EPD_2IN9_WIDTH  128
#define EPD_2IN9_HEIGHT 296


uint8_t BlackImage[4736] = { 0 };

// 这是一个编码格式测试
static void main_task(void *pvParameters)
{
    static uint8_t error_cnt = 0;

    static uint32_t old_tick = 0;
    static uint32_t new_tick = 0;

    static uint32_t ms_ticks = 0;
    static uint32_t cntttttt = 0;

    uint32_t blmcc = 0;
    FrameWork_LED_StateSet(LED_RED, LED_STATE_FLASH);
    FrameWork_LED_StateSet(LED_BLUE, LED_STATE_ON);

    FrameWork_EPD_SetDeviceType(EPD_DEVICE_E029A12);
    //FrameWork_EPD_SetDeviceType(EPD_DEVICE_E029A10);
    //FrameWork_EPD_SetDeviceType(EPD_DEVICE_WFT029);
    //FrameWork_EPD_SetDeviceType(EPD_DEVICE_A1360);

    while(1)
    {
        Paint_NewImage(BlackImage, EPD_2IN9_WIDTH, EPD_2IN9_HEIGHT, 90, WHITE);
        Paint_SelectImage(BlackImage);
        //Paint_DrawBitMap(gImage_E029A12);

        //FrameWork_EPD_DeviceInit();
        //FrameWork_EPD_WriteScreenALL(BlackImage, NULL);
        //FrameWork_EPD_DeviceSleep();
        //vTaskDelay(5000);

        Paint_SelectImage(BlackImage);
        //Paint_Clear(BLACK);
        Paint_Clear(WHITE);

        //Paint_SetPixel(0,0,BLACK);
        //Paint_SetPixel(0,1,BLACK);
        //Paint_SetPixel(0,2,BLACK);
        //Paint_SetPixel(0,3,BLACK);
        //Paint_SetPixel(0,4,BLACK);
        //Paint_SetPixel(0,5,BLACK);
        //Paint_SetPixel(0,6,BLACK);
        //Paint_SetPixel(0,7,BLACK);
        //Paint_SetPixel(0,8,BLACK);
        //Paint_SetPixel(0,9,BLACK);
        //Paint_SetPixel(0,10,BLACK);
        //Paint_SetPixel(0,11,BLACK);
        //Paint_SetPixel(0,12,BLACK);
        //Paint_SetPixel(0,13,BLACK);
        //Paint_SetPixel(0,14,BLACK);
        //Paint_SetPixel(0,15,BLACK);
        //Paint_SetPixel(0,16,BLACK);






        //Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
        //Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
        //Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);

        //Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
        //Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

        //Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
        //Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);

        //Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
        //Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);

        //Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
        //Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);

        //Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
        //Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);

        //Paint_DrawNum(10, 33, 123056789, &Font12, BLACK, WHITE);

        Paint_DrawChar(0,   0, '1', &Font72, BLACK, WHITE);
        Paint_DrawChar(50,  0, '1', &Font72, BLACK, WHITE);
        Paint_DrawChar(100, 0, '2', &Font72, BLACK, WHITE);
        Paint_DrawChar(150, 0, '3', &Font72, BLACK, WHITE);
        Paint_DrawChar(200, 0, '2', &Font72, BLACK, WHITE);
        Paint_DrawChar(250, 0, '3', &Font72, BLACK, WHITE);
        FrameWork_EPD_WriteScreenALL(BlackImage, NULL);
        vTaskDelay(2000);

        //FrameWork_EPD_WriteScreenALLClean();

        //Paint_DrawChar(24, 0, '!', &Font72, BLACK, WHITE);
        //FrameWork_EPD_WriteScreenALL(BlackImage, NULL);
        //vTaskDelay(2000);

        //FrameWork_EPD_WriteScreenALLClean();

        //Paint_DrawChar(24, 0, ' ', &Font72, BLACK, WHITE);
        //FrameWork_EPD_WriteScreenALL(BlackImage, NULL);
        //vTaskDelay(2000);
        //Paint_Clear(WHITE);


        //Paint_DrawNum(10, 50, 987654321, &Font16, BLACK, WHITE);
        //Paint_DrawNum(0, 0,123456789, &Font24, BLACK, WHITE);
        //Paint_DrawString_EN(0, 0,"blmcc", &Font24, WHITE, BLACK);


        //Paint_SelectImage(BlackImage);
        //PAINT_TIME sPaint_time;
        //sPaint_time.Hour = 22;
        //sPaint_time.Min = 57;
        //sPaint_time.Sec = 00;
        //UBYTE num = 20;
        //for (;;)
        //{
        //    sPaint_time.Sec = sPaint_time.Sec + 5;
        //    if (sPaint_time.Sec >= 60)
        //    {
        //        sPaint_time.Min = sPaint_time.Min + 1;
        //        sPaint_time.Sec = 0;
        //        if (sPaint_time.Min == 60)
        //        {
        //            sPaint_time.Hour =  sPaint_time.Hour + 1;
        //            sPaint_time.Min = 0;

        //            if (sPaint_time.Hour == 24)
        //            {
        //                sPaint_time.Hour = 0;
        //                sPaint_time.Min = 0;
        //                sPaint_time.Sec = 0;
        //            }
        //        }
        //    }

        //    Paint_ClearWindows(150, 80, 150 + Font20.Width * 7, 80 + Font20.Height, WHITE);
        //    Paint_DrawTime(150, 80, &sPaint_time, &Font20, WHITE, BLACK);

        //    num = num - 1;
        //    if(num == 0)
        //    {
        //        break;
        //    }

        //    FrameWork_EPD_DeviceInit();
        //    FrameWork_EPD_WriteScreenALL(BlackImage, NULL);
        //    FrameWork_EPD_DeviceSleep();
        //    vTaskDelay(5000);
        //}

        FrameWork_EPD_DeviceInit();
        FrameWork_EPD_WriteScreenALL(gImage_ikun, NULL);
        FrameWork_EPD_DeviceSleep();
        vTaskDelay(5000);

        //FrameWork_EPD_DeviceInit();
        //FrameWork_EPD_WriteScreenALLClean();
        //vTaskDelay(5000);
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




