/*********************************************************************************************************************
* CH32V203C8T6_REMOTE_CONTROL 即（CH32V203C8T6 遥控器）是一个基于官方 SDK 接口的第三方开源项目
* Copyright (c) 2024 SEEKFREE 逐飞科技
*
* CH32V203C8T6_REMOTE_CONTROL 开源项目的一部分
*
* CH32V203C8T6 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 version 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 version 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          led
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MounRiver Studio
* 适用平台          CH32V203C8T6
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期            作者         备注
* 2024-03-25        大W          first version
********************************************************************************************************************/

#include "led.h"


rt_event_t led_event;
rt_thread_t td_led;


//-------------------------------------------------------------------------------------------------------------------
// @brief       LED引脚初始化
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void led_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = REG_RED_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(RGB_RED_GROUP, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = REG_BLUE_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(RGB_BLUE_GROUP, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = REG_GREEN_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(RGB_GREEN_GROUP, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED_LOW_VOLTAGE_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED_LOW_VOLTAGE_GROUP, &GPIO_InitStructure);


    GPIO_WriteBit(RGB_RED_GROUP,   REG_RED_PIN,   1);
    GPIO_WriteBit(RGB_BLUE_GROUP,  REG_BLUE_PIN,  1);
    GPIO_WriteBit(RGB_GREEN_GROUP, REG_GREEN_PIN, 1);
    GPIO_WriteBit(LED_LOW_VOLTAGE_GROUP, LED_LOW_VOLTAGE_PIN, 1);


    GPIO_WriteBit(LED_LOW_VOLTAGE_GROUP, LED_LOW_VOLTAGE_PIN, 0);

    GPIO_WriteBit(RGB_RED_GROUP,   REG_RED_PIN,   0);
    rt_thread_mdelay(100);
    GPIO_WriteBit(RGB_RED_GROUP,   REG_RED_PIN,   1);
    rt_thread_mdelay(100);
    GPIO_WriteBit(RGB_BLUE_GROUP,  REG_BLUE_PIN,  0);
    rt_thread_mdelay(100);
    GPIO_WriteBit(RGB_BLUE_GROUP,  REG_BLUE_PIN,  1);
    rt_thread_mdelay(100);
    GPIO_WriteBit(RGB_GREEN_GROUP, REG_GREEN_PIN, 0);

    GPIO_WriteBit(LED_LOW_VOLTAGE_GROUP, LED_LOW_VOLTAGE_PIN, 1);
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       LED线程
// @param       parameter   参数
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void led_thread(void* parameter)
{
    rt_uint32_t recv_event;
    for(;;)
    {
        if (rt_event_recv(led_event, (0xFF),
        RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
        RT_WAITING_FOREVER, &recv_event) == RT_EOK)
        {
            if(RGB_LOW_POWER_EVENT == recv_event)
            {
                GPIO_WriteBit(RGB_RED_GROUP,   REG_RED_PIN,   1);
                GPIO_WriteBit(RGB_BLUE_GROUP,  REG_BLUE_PIN,  0);
                GPIO_WriteBit(RGB_GREEN_GROUP, REG_GREEN_PIN, 1);
            }

            if(RGB_PAIR_OR_CALIBRATION_EVENT == recv_event)
            {
                GPIO_WriteBit(RGB_RED_GROUP,   REG_RED_PIN,   0);
                GPIO_WriteBit(RGB_BLUE_GROUP,  REG_BLUE_PIN,  1);
                GPIO_WriteBit(RGB_GREEN_GROUP, REG_GREEN_PIN, 1);
            }

            if(RGB_NORMAL_EVENT == recv_event)
            {
                GPIO_WriteBit(RGB_RED_GROUP,   REG_RED_PIN,   1);
                GPIO_WriteBit(RGB_BLUE_GROUP,  REG_BLUE_PIN,  1);
                GPIO_WriteBit(RGB_GREEN_GROUP, REG_GREEN_PIN, 0);
            }

            if(LED_LOW_VOLTAGE_EVENT == recv_event)
            {
                GPIO_WriteBit(LED_LOW_VOLTAGE_GROUP, LED_LOW_VOLTAGE_PIN, 0);
            }

            if(LED_NORMAL_POWER_EVENT == recv_event)
            {
                GPIO_WriteBit(LED_LOW_VOLTAGE_GROUP, LED_LOW_VOLTAGE_PIN, 1);
            }

            if(RGB_CLOSE_EVENT == recv_event)
            {
                GPIO_WriteBit(RGB_RED_GROUP,   REG_RED_PIN,   1);
                GPIO_WriteBit(RGB_BLUE_GROUP,  REG_BLUE_PIN,  1);
                GPIO_WriteBit(RGB_GREEN_GROUP, REG_GREEN_PIN, 1);
            }

        }

        rt_thread_mdelay(20);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       LED线程创建
// @param       void
// @return      uint8   0-成功，1-失败
// @note
//-------------------------------------------------------------------------------------------------------------------
uint8 led_thread_create(void)
{
    led_event = rt_event_create("led_event", RT_IPC_FLAG_FIFO);
    if(led_event == RT_NULL)
    {
        rt_kprintf("led_event create ERROR.\n");
        return 1;
    }

    rt_event_send(led_event, RGB_NORMAL_EVENT);

    td_led = rt_thread_create("led_thread",                 //线程名称
                        led_thread,             // 线程入口函数
                        RT_NULL,                         // 线程参数
                        512,                             // 512个字节的栈空间
                        14,                              //数值越小，优先级越高，0为最高优先级。
                                                      //可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
                        100);                            //时间片为100


    if(td_led != RT_NULL)                   //线程创建成功
    {
        rt_kprintf("td_led create OK.\n");
        rt_thread_startup(td_led);          //运行该线程
    }
    else                                        //线程创建失败
    {
        rt_kprintf("td_led create ERROR.\n");
        return 1;
    }

    return 0;
}



