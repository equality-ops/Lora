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

#include "power.h"




//-------------------------------------------------------------------------------------------------------------------
// @brief       LED引脚初始化
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void power_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Pin = K_ON_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(K_ON_GROUP, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = ONOFF_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(ONOFF_GROUP, &GPIO_InitStructure);

    GPIO_WriteBit(ONOFF_GROUP,   ONOFF_PIN,   1);

}


//-------------------------------------------------------------------------------------------------------------------
// @brief       LED线程
// @param       parameter   参数
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void power_thread(void* parameter)
{

    static uint16_t count = 0;
    while(GPIO_ReadInputDataBit(K_ON_GROUP, K_ON_PIN) == 0)
    {
        count++;
        if(count > 200)
        {
            rt_kprintf("Release the power button\r\n"); 
        }
        rt_thread_mdelay(10);
    }

    count = 0;

    for(;;)
    {
        if(GPIO_ReadInputDataBit(K_ON_GROUP, K_ON_PIN) == 0)
        {
            count++;
            if(count >= 50)
            {
                rt_kprintf("poweroff\r\n"); 
                rt_hw_interrupt_disable();
       
                while(1)
                {
                     GPIO_WriteBit(ONOFF_GROUP, ONOFF_PIN, 0);
                }
     
            }
        }
        else 
        {
            count = 0;
        }

        rt_thread_mdelay(1);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       关闭电源
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void poweroff(void)
{
    GPIO_WriteBit(ONOFF_GROUP, ONOFF_PIN, 0);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       LED线程创建
// @param       void
// @return      uint8   0-成功，1-失败
// @note
//-------------------------------------------------------------------------------------------------------------------
uint8 power_thread_create(void)
{
    rt_thread_t td_power;
    td_power = rt_thread_create("power_thread",                 //线程名称
                        power_thread,                   // 线程入口函数
                        RT_NULL,                         // 线程参数
                        512,                             // 512个字节的栈空间
                        14,                              //数值越小，优先级越高，0为最高优先级。
                                                      //可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
                        100);                            //时间片为100


    if(td_power != RT_NULL)                   //线程创建成功
    {
        rt_kprintf("td_power create OK.\n");
        rt_thread_startup(td_power);          //运行该线程
    }
    else                                        //线程创建失败
    {
        rt_kprintf("td_power create ERROR.\n");
        return 1;
    }

    return 0;
}


