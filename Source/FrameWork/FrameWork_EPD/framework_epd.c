/***********************************************************************************************************************
* 文件名称: framework_epd.c
* 文件描述: 墨水屏相关封装函数
* 维护人员: bo.liangmin
* 版本信息: 20230819--代码创建
***********************************************************************************************************************/



/***********************************************************************************************************************
* include
***********************************************************************************************************************/
#include "hal_epd.h"
#include "framework_epd.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"



/***********************************************************************************************************************
* define
***********************************************************************************************************************/
epd_device_t framework_epd_device = EPD_DEVICE_MAX;




/***********************************************************************************************************************************************************************************
*                                                                                       黑白墨水屏
***********************************************************************************************************************************************************************************/
/***********************************************************************************
* 函数名称: static uint8_t FrameWork_EPD_E029A12_BusyPinReady(void)
* 函数说明: E029A12获取BusyPin状态
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static uint8_t FrameWork_EPD_E029A12_BusyPinReady(void)
{
    uint32_t busy_ticks =  xTaskGetTickCount();

    while(1)
    {
        if(0 == HAL_EPD_ReadBusyPinValue())
        {
            return 1;
        }
        else
        {
            if(xTaskGetTickCount() - busy_ticks >= 100)
            {
                return 0;
            }
        }
    }
}

/***********************************************************************************
* 函数名称: static uint8_t FrameWork_EPD_E029A12_Init(void)
* 函数说明: E029A12硬件初始化
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static uint8_t FrameWork_EPD_E029A12_Init(void)
{
    HAL_EPD_ResetOn();
    vTaskDelay(20);
    HAL_EPD_ResetOff();
    vTaskDelay(20);

    if(1 == FrameWork_EPD_E029A12_BusyPinReady())
    {
        HAL_EPD_WriteCommand(0x12);                                //SWRESET

        if(1 == FrameWork_EPD_E029A12_BusyPinReady())
        {
            HAL_EPD_WriteCommand(0x01);                            //Driver output control
            HAL_EPD_WriteData(0x27);
            HAL_EPD_WriteData(0x01);
            HAL_EPD_WriteData(0x00);

            HAL_EPD_WriteCommand(0x11);                            //data entry mode
            HAL_EPD_WriteData(0x01);

            HAL_EPD_WriteCommand(0x44);                            //set Ram-X address start/end position
            HAL_EPD_WriteData(0x00);
            HAL_EPD_WriteData(0x0F);                               //0x0F-->(15+1)*8=128

            HAL_EPD_WriteCommand(0x45);                            //set Ram-Y address start/end position
            HAL_EPD_WriteData(0x27);                               //0x0127-->(295+1)=296
            HAL_EPD_WriteData(0x01);
            HAL_EPD_WriteData(0x00);
            HAL_EPD_WriteData(0x00);

            HAL_EPD_WriteCommand(0x3C);                            //BorderWavefrom
            HAL_EPD_WriteData(0x05);

            HAL_EPD_WriteCommand(0x18);                            //Read built-in temperature sensor
            HAL_EPD_WriteData(0x80);

            HAL_EPD_WriteCommand(0x21);                            //Display update control
            HAL_EPD_WriteData(0x00);
            HAL_EPD_WriteData(0x80);

            HAL_EPD_WriteCommand(0x4E);                            //set RAM x address count to 0;
            HAL_EPD_WriteData(0x00);
            HAL_EPD_WriteCommand(0x4F);                            //set RAM y address count to 0X199;
            HAL_EPD_WriteData(0x27);
            HAL_EPD_WriteData(0x01);

            if(1 == FrameWork_EPD_E029A12_BusyPinReady())
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

/***********************************************************************************
* 函数名称: static uint8_t FrameWork_EPD_E029A12_Update(void)
* 函数说明: E029A12刷新
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static uint8_t FrameWork_EPD_E029A12_Update(void)
{
    HAL_EPD_WriteCommand(0x22);                                //Display Update Control
    HAL_EPD_WriteData(0xF7);
    HAL_EPD_WriteCommand(0x20);                                //Activate Display Update Sequence

    if(1 == FrameWork_EPD_E029A12_BusyPinReady())
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/***********************************************************************************
* 函数名称: static uint8_t FrameWork_EPD_E029A12_WriteScreenALL(...)
* 函数说明: E029A12全屏写
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static uint8_t FrameWork_EPD_E029A12_WriteScreenALL(const uint8_t* BW_datas)
{
    uint16_t i = 0;

    HAL_EPD_WriteCommand(0x24);                             //write RAM for black(0)/white (1)
    for(i=0; i<ALLSCREEN_GRAGHBYTES; i++)
    {
        HAL_EPD_WriteData(*BW_datas);
        BW_datas++;
    }

    return FrameWork_EPD_E029A12_Update();
}

/***********************************************************************************
* 函数名称: static uint8_t FrameWork_EPD_E029A12_WriteScreenALLClean(void)
* 函数说明: E029A12全屏擦
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static uint8_t FrameWork_EPD_E029A12_WriteScreenALLClean(void)
{
    uint16_t i;
    HAL_EPD_WriteCommand(0x24);                             //write RAM for black(0)/white (1)
    for(i=0; i<ALLSCREEN_GRAGHBYTES; i++)
    {
        HAL_EPD_WriteData(0xfe);
    }
    return FrameWork_EPD_E029A12_Update();
}

/***********************************************************************************
* 函数名称: static uint8_t FrameWork_EPD_E029A12_DeepSleep(void)
* 函数说明: E029A12 sleep
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static uint8_t FrameWork_EPD_E029A12_DeepSleep(void)
{
    HAL_EPD_WriteCommand(0x10);                           //enter deep sleep
    HAL_EPD_WriteData(0x01);
    vTaskDelay(100);

    if(1 == FrameWork_EPD_E029A12_BusyPinReady())
    {
        return 1;
    }
    else
    {
        return 0;                                           //blmcc???此处能否这样同还需验证
    }
}

/***********************************************************************************************************************************************************************************
*                                                                                       黑白红墨水屏
***********************************************************************************************************************************************************************************/
/***********************************************************************************
* 函数名称: static uint8_t FrameWork_EPD_E029A10_BusyPinReady(void)
* 函数说明: E029A10获取BusyPin状态
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static uint8_t FrameWork_EPD_E029A10_BusyPinReady(void)
{
    uint8_t res = 0;
    uint32_t busy_ticks =  xTaskGetTickCount();

    while(1)
    {
        //HAL_EPD_WriteCommand(0x71);

        if(1 == HAL_EPD_ReadBusyPinValue())
        {
            res = 1;
            break;
        }
        else
        {
            if(xTaskGetTickCount() - busy_ticks >= 20000)
            {
                res = 0;
                break;
            }
        }
    }

    vTaskDelay(200);
    return res;
}

/***********************************************************************************
* 函数名称: static uint8_t FrameWork_EPD_E029A10_Init(void)
* 函数说明: E029A10硬件初始化
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static uint8_t FrameWork_EPD_E029A10_Init(void)
{
    HAL_EPD_ResetOff();
    vTaskDelay(200);
    HAL_EPD_ResetOn();
    vTaskDelay(5);
    HAL_EPD_ResetOff();
    vTaskDelay(200);

    HAL_EPD_WriteCommand(0x04);
    if(1 == FrameWork_EPD_E029A10_BusyPinReady())
    {
        HAL_EPD_WriteCommand(0x00);         //panel setting
        HAL_EPD_WriteData(0x0f);            //LUT from OTP，128x296
        HAL_EPD_WriteData(0x0f);            //Temperature sensor, boost and other related timing settings

        HAL_EPD_WriteCommand(0x61);         //resolution setting
        HAL_EPD_WriteData (0x80);
        HAL_EPD_WriteData (0x01);
        HAL_EPD_WriteData (0x28);

        HAL_EPD_WriteCommand(0X50);         //VCOM AND DATA INTERVAL SETTING
        HAL_EPD_WriteData(0x77);           //WBmode:VBDF 17|D7 VBDW 97 VBDB 57   WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7

        return 1;
    }
    else
    {
        return 0;
    }
}

/***********************************************************************************
* 函数名称: static uint8_t FrameWork_EPD_E029A10_Update(void)
* 函数说明: E029A10刷新
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static uint8_t FrameWork_EPD_E029A10_Update(void)
{
    HAL_EPD_WriteCommand(0x12);                                //Display Update Control

    return FrameWork_EPD_E029A10_BusyPinReady();
}

/***********************************************************************************
* 函数名称: static uint8_t FrameWork_EPD_E029A10_WriteScreenALL(...)
* 函数说明: E029A10全屏写
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static uint8_t FrameWork_EPD_E029A10_WriteScreenALL(const uint8_t* BW_datas, const uint8_t* R_datas)
{
    uint16_t i;

    HAL_EPD_WriteCommand(0x10);                             //send black data
    for (i = 0; i < ALLSCREEN_GRAGHBYTES; i++)
    {
        HAL_EPD_WriteData(*BW_datas);
        BW_datas++;
    }

    HAL_EPD_WriteCommand(0x13);                            //send red data
    for (i = 0; i < ALLSCREEN_GRAGHBYTES; i++)
    {
        HAL_EPD_WriteData(*R_datas);
        R_datas++;
    }
    HAL_EPD_WriteCommand(0x92);

    return FrameWork_EPD_E029A10_Update();
}

/***********************************************************************************
* 函数名称: static uint8_t FrameWork_EPD_E029A10_WriteScreenALLClean(void)
* 函数说明: E029A10全屏擦
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static uint8_t FrameWork_EPD_E029A10_WriteScreenALLClean(void)
{
    uint16_t i;

    HAL_EPD_WriteCommand(0x10);
    for (i = 0; i < ALLSCREEN_GRAGHBYTES; i++)
    {
        HAL_EPD_WriteData(0xff);
    }

    HAL_EPD_WriteCommand(0x13);
    for (i = 0; i < ALLSCREEN_GRAGHBYTES; i++)
    {
        HAL_EPD_WriteData(0xff);
    }

    return FrameWork_EPD_E029A10_Update();
}

/***********************************************************************************
* 函数名称: static void FrameWork_EPD_E029A10_DeepSleep(void)
* 函数说明: E029A10 sleep
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static uint8_t FrameWork_EPD_E029A10_DeepSleep(void)
{
    HAL_EPD_WriteCommand(0x02);                          // POWER_OFF
    if(1 == FrameWork_EPD_E029A10_BusyPinReady())
    {
        HAL_EPD_WriteCommand(0x07);                     // DEEP_SLEEP
        HAL_EPD_WriteData(0xA5);                        // check code
        return 1;
    }
    else
    {
        return 0;
    }
}

/***********************************************************************************************************************************************************************************
*                                                                                       WFT029--S-GDEW029T5D
***********************************************************************************************************************************************************************************/
/***********************************************************************************
* 函数名称: static uint8_t FrameWork_EPD_WFT029_BusyPinReady(void)
* 函数说明: WFT029获取BusyPin状态
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static uint8_t FrameWork_EPD_WFT029_BusyPinReady(void)
{
    uint8_t res = 0;
    uint32_t busy_ticks =  xTaskGetTickCount();

    while(1)
    {
        if(1 == HAL_EPD_ReadBusyPinValue())
        {
            res = 1;
            break;
        }
        else
        {
            if(xTaskGetTickCount() - busy_ticks >= 20000)
            {
                res = 0;
                break;
            }
        }
    }

    return res;
}

/***********************************************************************************
* 函数名称: static uint8_t FrameWork_EPD_WFT029_Init(void)
* 函数说明: WFT029硬件初始化
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static uint8_t FrameWork_EPD_WFT029_Init(void)
{
    HAL_EPD_ResetOn();                                // Module reset
    vTaskDelay(10);                                   // At least 10ms delay
    HAL_EPD_ResetOff();
    vTaskDelay(10);                                   // At least 10ms delay

    HAL_EPD_ResetOn();                                // Module reset
    vTaskDelay(10);                                   // At least 10ms delay
    HAL_EPD_ResetOff();
    vTaskDelay(10);                                   //At least 10ms delay

    HAL_EPD_ResetOn();                                // Module reset
    vTaskDelay(10);                                   //At least 10ms delay
    HAL_EPD_ResetOff();
    vTaskDelay(10);                                   //At least 10ms delay

    HAL_EPD_WriteCommand(0x04);                       //Power on
    if(1 == FrameWork_EPD_WFT029_BusyPinReady())
    {
        HAL_EPD_WriteCommand(0x00);                   //panel setting
        HAL_EPD_WriteData(0x1f);                      //LUT from OTP

        HAL_EPD_WriteCommand(0x61);                   //resolution setting
        HAL_EPD_WriteData (0x80);                     //128
        HAL_EPD_WriteData (0x01);                     //296
        HAL_EPD_WriteData (0x28);

        HAL_EPD_WriteCommand(0X50);                   //VCOM AND DATA INTERVAL SETTING
        HAL_EPD_WriteData(0x97);                      //WBmode:VBDF 17|D7 VBDW 97 VBDB 57     WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7

        return 1;
    }
    else
    {
        return 0;
    }
}

/***********************************************************************************
* 函数名称: static uint8_t FrameWork_EPD_WFT029_Update(void)
* 函数说明: WFT029刷新
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static uint8_t FrameWork_EPD_WFT029_Update(void)
{
    HAL_EPD_WriteCommand(0x12);                          //DISPLAY REFRESH
    vTaskDelay(1);                                       //!!!The delay here is necessary, 200uS at least!!!
    return FrameWork_EPD_WFT029_BusyPinReady();          //waiting for the electronic paper IC to release the idle signal
}

/***********************************************************************************
* 函数名称: static uint8_t FrameWork_EPD_WFT029_WriteScreenALL(...)
* 函数说明: WFT029全屏写
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static uint8_t FrameWork_EPD_WFT029_WriteScreenALL(const uint8_t* BW_datas)
{
    uint16_t i;

    //Write Data
    HAL_EPD_WriteCommand(0x10);                          //Transfer old data
    for(i=0; i<4736; i++)
    {
        HAL_EPD_WriteData(0xff);
    }

    HAL_EPD_WriteCommand(0x13);                          //Transfer new data
    for(i=0; i<4736; i++)
    {
        HAL_EPD_WriteData(*BW_datas);                     //Transfer the actual displayed data
        BW_datas++;
    }

    return FrameWork_EPD_WFT029_Update();
}

/***********************************************************************************
* 函数名称: static uint8_t FrameWork_EPD_WFT029_WriteScreenALLClean(void)
* 函数说明: WFT029全屏擦
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static uint8_t FrameWork_EPD_WFT029_WriteScreenALLClean(void)
{
    uint16_t i;

    //Write Data
    HAL_EPD_WriteCommand(0x10);                          //Transfer old data
    for(i=0; i<4736; i++)
    {
        HAL_EPD_WriteData(0xff);
    }

    HAL_EPD_WriteCommand(0x13);                          //Transfer new data
    for(i=0; i<4736; i++)
    {
        HAL_EPD_WriteData(0xff);                         //Transfer the actual displayed data
    }

    return FrameWork_EPD_WFT029_Update();
}

/***********************************************************************************
* 函数名称: static uint8_t FrameWork_EPD_WFT029_DeepSleep(void)
* 函数说明: WFT029 sleep
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static uint8_t FrameWork_EPD_WFT029_DeepSleep(void)
{
    HAL_EPD_WriteCommand(0X50);                           //VCOM AND DATA INTERVAL SETTING
    HAL_EPD_WriteData(0xf7);                              //WBmode:VBDF 17|D7 VBDW 97 VBDB 57        WBRmode:VBDF  F7 VBDW 77 VBDB 37  VBDR B7
    HAL_EPD_WriteCommand(0X02);                           //power off

    if(1== FrameWork_EPD_WFT029_BusyPinReady())
    {
        vTaskDelay(1000);                                //!!!The delay here is necessary,1000mS at least!!!
        HAL_EPD_WriteCommand(0X07);                      //deep sleep
        HAL_EPD_WriteData(0xA5);

        return 1;
    }
    else
    {
        return 0;
    }
}

/***********************************************************************************************************************************************************************************
*                                                                                       A1360-微雪4in2bc
***********************************************************************************************************************************************************************************/
/***********************************************************************************
* 函数名称: static uint8_t FrameWork_EPD_A1360_BusyPinReady(void)
* 函数说明: A1360获取BusyPin状态
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static uint8_t FrameWork_EPD_A1360_BusyPinReady(void)
{
    uint32_t busy_ticks =  xTaskGetTickCount();

    while(1)
    {
        if(1 == HAL_EPD_ReadBusyPinValue())
        {
            return 1;
        }
        else
        {
            if(xTaskGetTickCount() - busy_ticks >= 20000)
            {
                return 0;
            }
        }
    }
}

/***********************************************************************************
* 函数名称: static uint8_t FrameWork_EPD_A1360_Init(void)
* 函数说明: A1360硬件初始化
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static uint8_t FrameWork_EPD_A1360_Init(void)
{
    HAL_EPD_ResetOn();
    vTaskDelay(10);
    HAL_EPD_ResetOff();
    vTaskDelay(10);

    HAL_EPD_WriteCommand(0x04);

    return FrameWork_EPD_A1360_BusyPinReady();
}


/***********************************************************************************
* 函数名称: static uint8_t FrameWork_EPD_A1360_Update(void)
* 函数说明: A1360刷新
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static uint8_t FrameWork_EPD_A1360_Update(void)
{
    HAL_EPD_WriteCommand(0x12);                                //Display Update Control
    vTaskDelay(10);

    return FrameWork_EPD_A1360_BusyPinReady();
}

/***********************************************************************************
* 函数名称: static uint8_t FrameWork_EPD_A1360_WriteScreenALL(...)
* 函数说明: A1360全屏写
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static uint8_t FrameWork_EPD_A1360_WriteScreenALL(const uint8_t* BW_datas, const uint8_t* Y_datas)
{
    uint16_t i,j;
    uint16_t Width, Height;
    Width = (A1360_WIDTH % 8 == 0)? (A1360_WIDTH / 8 ): (A1360_WIDTH / 8 + 1);
    Height = A1360_HEIGH;

    HAL_EPD_WriteCommand(0x10);                             //send black data
    for (i = 0; i < Height; i++)
    {
        for (j = 0; j < Width; j++)
        {
            HAL_EPD_WriteData(BW_datas[j + i * Width]);
        }
    }

    HAL_EPD_WriteCommand(0x13);                            //send red data
    for (i = 0; i < Height; i++)
    {
        for (j = 0; j < Width; j++)
        {
            HAL_EPD_WriteData(Y_datas[j + i * Width]);
        }
    }

    return FrameWork_EPD_A1360_Update();
}

/***********************************************************************************
* 函数名称: static uint8_t FrameWork_EPD_A1360_WriteScreenALLClean(void)
* 函数说明: A1360全屏擦
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static uint8_t FrameWork_EPD_A1360_WriteScreenALLClean(void)
{
    uint16_t i,j;
    uint16_t Width, Height;
    Width = (A1360_WIDTH % 8 == 0)? (A1360_WIDTH / 8 ): (A1360_WIDTH / 8 + 1);
    Height = A1360_HEIGH;

    HAL_EPD_WriteCommand(0x10);                             //send black data
    for (i = 0; i < Height; i++)
    {
        for (j = 0; j < Width; j++)
        {
            HAL_EPD_WriteData(0x00);
        }
    }

    HAL_EPD_WriteCommand(0x13);                            //send red data
    for (i = 0; i < Height; i++)
    {
        for (j = 0; j < Width; j++)
        {
            HAL_EPD_WriteData(0x00);
        }
    }

    return FrameWork_EPD_A1360_Update();
}

/***********************************************************************************
* 函数名称: static uint8_t FrameWork_EPD_A1360_DeepSleep(void)
* 函数说明: A1360 sleep
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
static uint8_t FrameWork_EPD_A1360_DeepSleep(void)
{
    HAL_EPD_WriteCommand(0x02);                            // POWER_OFF

    if(1 == FrameWork_EPD_A1360_BusyPinReady())
    {
        vTaskDelay(1000);
        HAL_EPD_WriteCommand(0x07);                        // DEEP_SLEEP
        HAL_EPD_WriteData(0XA5);
        return 1;
    }
    else
    {
        return 0;
    }
}

/***********************************************************************************
* 函数名称: void FrameWork_EPD_SetDeviceType(epd_device_t deviceType)
* 函数说明: 设置当前EPD设备类型
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
void FrameWork_EPD_SetDeviceType(epd_device_t deviceType)
{
    framework_epd_device = deviceType;
}

/***********************************************************************************
* 函数名称: epd_device_t FrameWork_EPD_GetDeviceType(void)
* 函数说明: 获取当前EPD设备类型
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
epd_device_t FrameWork_EPD_GetDeviceType(void)
{
    return framework_epd_device;
}

/***********************************************************************************
* 函数名称: uint8_t FrameWork_EPD_DeviceInit(void)
* 函数说明: EPD设备初始化
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
uint8_t FrameWork_EPD_DeviceInit(void)
{
    uint8_t initRes = 0;

    switch( FrameWork_EPD_GetDeviceType() )
    {
        case EPD_DEVICE_E029A12:
        {
            initRes = FrameWork_EPD_E029A12_Init();
            break;
        }

        case EPD_DEVICE_E029A10:
        {
            initRes = FrameWork_EPD_E029A10_Init();
            break;
        }

        case EPD_DEVICE_WFT029:
        {
            initRes = FrameWork_EPD_WFT029_Init();
            break;
        }

        case EPD_DEVICE_A1360:
        {
            initRes = FrameWork_EPD_A1360_Init();
            break;
        }

        default:
        {
            initRes = 0;
            break;
        }
    }

    return initRes;
}

/***********************************************************************************
* 函数名称: uint8_t FrameWork_EPD_WriteScreenALL(...)
* 函数说明: EPD设备全屏写
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
uint8_t FrameWork_EPD_WriteScreenALL(const uint8_t* bw_dat, const uint8_t* ry_dat)
{
    uint8_t writeRes = 0;

    switch( FrameWork_EPD_GetDeviceType() )
    {
        case EPD_DEVICE_E029A12:
        {
            writeRes = FrameWork_EPD_E029A12_WriteScreenALL(bw_dat);
            break;
        }

        case EPD_DEVICE_E029A10:
        {
            writeRes = FrameWork_EPD_E029A10_WriteScreenALL(bw_dat, ry_dat);
            break;
        }

        case EPD_DEVICE_WFT029:
        {
            writeRes = FrameWork_EPD_WFT029_WriteScreenALL(bw_dat);
            break;
        }

        case EPD_DEVICE_A1360:
        {
            writeRes = FrameWork_EPD_A1360_WriteScreenALL(bw_dat, ry_dat);
            break;
        }

        default:
        {
            writeRes = 0;
            break;
        }
    }

    return writeRes;
}

/***********************************************************************************
* 函数名称: uint8_t FrameWork_EPD_WriteScreenALLClean(void)
* 函数说明: EPD设备全屏擦除
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
uint8_t FrameWork_EPD_WriteScreenALLClean(void)
{
    uint8_t cleanRes = 0;

    switch( FrameWork_EPD_GetDeviceType() )
    {
        case EPD_DEVICE_E029A12:
        {
            cleanRes = FrameWork_EPD_E029A12_WriteScreenALLClean();
            break;
        }

        case EPD_DEVICE_E029A10:
        {
            cleanRes = FrameWork_EPD_E029A10_WriteScreenALLClean();
            break;
        }

        case EPD_DEVICE_WFT029:
        {
            cleanRes = FrameWork_EPD_WFT029_WriteScreenALLClean();
            break;
        }

        case EPD_DEVICE_A1360:
        {
            cleanRes = FrameWork_EPD_A1360_WriteScreenALLClean();
            break;
        }

        default:
        {
            cleanRes = 0;
            break;
        }
    }

    return cleanRes;
}

/***********************************************************************************
* 函数名称: uint8_t FrameWork_EPD_DeviceSleep(void)
* 函数说明: EPD设备休眠
* 输入参数: 无
* 输出参数: 无
* 返回数值: 无
***********************************************************************************/
uint8_t FrameWork_EPD_DeviceSleep(void)
{
    uint8_t sleepRes = 0;

    switch( FrameWork_EPD_GetDeviceType() )
    {
        case EPD_DEVICE_E029A12:
        {
            sleepRes = FrameWork_EPD_E029A12_DeepSleep();
            break;
        }

        case EPD_DEVICE_E029A10:
        {
            sleepRes = FrameWork_EPD_E029A10_DeepSleep();
            break;
        }

        case EPD_DEVICE_WFT029:
        {
            sleepRes = FrameWork_EPD_WFT029_DeepSleep();
            break;
        }

        case EPD_DEVICE_A1360:
        {
            sleepRes = FrameWork_EPD_A1360_DeepSleep();
            break;
        }

        default:
        {
            sleepRes = 0;
            break;
        }
    }

    return sleepRes;
}



/***********************************************************************************************************************
* end
***********************************************************************************************************************/


