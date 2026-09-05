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
* 文件名称          battery_manage
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

#include "adc.h"
#include "led.h"
#include "battery_manage.h"
#include "config.h"

rt_thread_t td_battery;

//-------------------------------------------------------------------------------------------------------------------
// @brief       ADC使用DMA初始化
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
static uint16 get_battery_adc(void)
{

    return (g_adc_reg_value[BATTERY_ADC_INDEX]);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       电池电压检测线程
// @param       void
// @return      parameter   参数
// @note
//-------------------------------------------------------------------------------------------------------------------
static void battery_thread(void* parameter)
{
    uint16 battery_voltage = 0;
    uint16 low_power_count = 0;
    uint8  low_power_flag = 0;

    uint16 normal_power_count = 0;

    for(;;)
    {
        // 这里的4095是12位ADC
        // 3300是基准电压300mv
        // 11是分压电阻分压系数
        battery_voltage = (float)get_battery_adc() / 4095 * 3300 * 11;

        // 3秒钟电池电压都低于阈值,且电源开关打开,则说明没电了。
        if((battery_voltage < REMOTE_MIN_BATTERY) && (battery_voltage > 100))
        {
            low_power_count++;
            if(300 < low_power_count)
            {
                low_power_flag = 1;
                rt_event_send(led_event, LED_LOW_VOLTAGE_EVENT);
                low_power_count = 0;
            }
        }
        else
        {
            low_power_count = 0;
        }

        // 电池电压过低，LED闪烁。
        if(low_power_flag == 1)
        {
            rt_thread_mdelay(100);
            rt_event_send(led_event, LED_NORMAL_POWER_EVENT);
            rt_thread_mdelay(100);
            rt_event_send(led_event, LED_LOW_VOLTAGE_EVENT);
        }

        // 3秒钟电池电压都高于阈值,则说明此时电压正常。
        if(battery_voltage > 4100 && low_power_flag == 1)
        {
            normal_power_count++;
            if(15 <normal_power_count)
            {
                low_power_flag = 0;
                rt_event_send(led_event, LED_NORMAL_POWER_EVENT);
                normal_power_count = 0;
            }
        }
        else
        {
            normal_power_count = 0;
        }

        rt_thread_mdelay(10);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       电池电压检测线程初始化
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
uint8 battery_thread_create(void)
{


    td_battery = rt_thread_create("battery",                //线程名称
                        battery_thread,                     // 线程入口函数
                        RT_NULL,                            // 线程参数
                        512,                                // 512个字节的栈空间
                        2,                                  //数值越小，优先级越高，0为最高优先级。
                                                            //可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
                        100);                               //时间片为100


    if(td_battery != RT_NULL)                               //线程创建成功
    {
        rt_kprintf("battery_thread create OK.\n");
        rt_thread_startup(td_battery);                      //运行该线程
    }
    else                                                    //线程创建失败
    {
        rt_kprintf("battery_thread create ERROR.\n");
        return 1;
    }

    return 0;

}

//-------------------------------------------------------------------------------------------------------------------
// @brief       电池电压引脚初始化
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void battery_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = BATTERY_ADC_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(BATTERY_ADC_GROUP, &GPIO_InitStructure);
}


