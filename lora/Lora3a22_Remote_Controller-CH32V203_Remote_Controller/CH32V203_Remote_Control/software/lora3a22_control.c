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
* 文件名称          lora3a22_control
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

#include "joystick.h"
#include "led.h"
#include "lora3a22_config.h"
#include "lora3a22_control.h"
#include "stdio.h"
#include "ch32v20x.h"
#include "config.h"
#include "power.h"

static uint8 lora3a22_sleep_flag = 0;
static uint32 lora3a22_idle_count = 0;
static volatile int8 default_pair_channel = 0;

static rt_thread_t td_lora3a22_send = RT_NULL;
static rt_thread_t td_pair = RT_NULL;
static rt_thread_t td_calibration = RT_NULL;
static rt_mutex_t  lora3a22_mutex = RT_NULL;

lora3a22_uart_transfer_dat_struct lora3a22_transfer;



//-------------------------------------------------------------------------------------------------------------------
// @brief       限幅
// @param       adc_reg_value               采集到的ADC数据
// @param       adc_calibration_value       校准数据
// @param       threshold                   阈值
// @return      int16                       计算后的ADC值
// @note
//-------------------------------------------------------------------------------------------------------------------
static int16 limit(int16 adc_reg_value, int16 adc_calibration_value, int16 threshold)
{
    int16 ret = adc_reg_value - adc_calibration_value;

    if(threshold <= ret)
    {
        ret = ret - threshold;
    }
    else if(-threshold >= ret)
    {
        ret = ret + threshold;
    }
    else
    {
        ret = 0;
    }

    return ret;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       LORA发送线程
// @param       parameter       参数
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void lora3a22_send_thread(void* parameter)
{
    uint8 temp_sum_check = 0;
    uint8 *buff = (uint8 *)&lora3a22_transfer;


    for(;;)
    {

        // 向结构体里面填充数据
        temp_sum_check = 0;

        lora3a22_transfer.head = 0xA3;
        lora3a22_transfer.sum_check = 0x00;
        lora3a22_transfer.key[0] = GPIO_ReadInputDataBit(JOYSTICK_KEY_LEFT_GROUP , JOYSTICK_KEY_LEFT_PIN) ? 0 : 1;
        lora3a22_transfer.key[1] = GPIO_ReadInputDataBit(JOYSTICK_KEY_RIGHT_GROUP, JOYSTICK_KEY_RIGHT_PIN)? 0 : 1;
        lora3a22_transfer.key[2] = GPIO_ReadInputDataBit(JOYSTICK_SIDE_KEY_LEFT_GROUP , JOYSTICK_SIDE_KEY_LEFT_PIN) ? 0 : 1;
        lora3a22_transfer.key[3] = GPIO_ReadInputDataBit(JOYSTICK_SIDE_KEY_RIGHT_GROUP, JOYSTICK_SIDE_KEY_RIGHT_PIN)? 0 : 1;

        lora3a22_transfer.joystick[JOYSTICK_ADC_0_INDEX] = limit(g_adc_reg_value[JOYSTICK_ADC_0_INDEX], g_adc_calibration_value[JOYSTICK_ADC_0_INDEX], REMOTE_JOYSTICK_THRESHOLD);
        lora3a22_transfer.joystick[JOYSTICK_ADC_1_INDEX] = limit(g_adc_reg_value[JOYSTICK_ADC_1_INDEX], g_adc_calibration_value[JOYSTICK_ADC_1_INDEX], REMOTE_JOYSTICK_THRESHOLD);
        lora3a22_transfer.joystick[JOYSTICK_ADC_2_INDEX] = limit(g_adc_reg_value[JOYSTICK_ADC_2_INDEX], g_adc_calibration_value[JOYSTICK_ADC_2_INDEX], REMOTE_JOYSTICK_THRESHOLD);
        lora3a22_transfer.joystick[JOYSTICK_ADC_3_INDEX] = limit(g_adc_reg_value[JOYSTICK_ADC_3_INDEX], g_adc_calibration_value[JOYSTICK_ADC_3_INDEX], REMOTE_JOYSTICK_THRESHOLD);

        lora3a22_transfer.switch_key[0] = GPIO_ReadInputDataBit(JOYSTICK_SWITCH_KEY_1_LEFT_GROUP , JOYSTICK_SWITCH_KEY_1_LEFT_PIN)  ;
        lora3a22_transfer.switch_key[1] = GPIO_ReadInputDataBit(JOYSTICK_SWITCH_KEY_2_LEFT_GROUP, JOYSTICK_SWITCH_KEY_2_LEFT_PIN)   ;
        lora3a22_transfer.switch_key[2] = GPIO_ReadInputDataBit(JOYSTICK_SWITCH_KEY_1_RIGHT_GROUP , JOYSTICK_SWITCH_KEY_1_RIGHT_PIN);
        lora3a22_transfer.switch_key[3] = GPIO_ReadInputDataBit(JOYSTICK_SWITCH_KEY_2_RIGHT_GROUP, JOYSTICK_SWITCH_KEY_2_RIGHT_PIN) ;


        lora3a22_idle_count++;

        // 获取互斥量防止冲突
        rt_mutex_take(lora3a22_mutex, RT_WAITING_FOREVER);

        // 如果摇杆和按键没有被操作，超过设定的时间，就进入低功耗模式
        if(lora3a22_sleep_flag == 0)
        {
            if( lora3a22_transfer.key[0] == 0 &&
                lora3a22_transfer.key[1] == 0 &&
                lora3a22_transfer.key[2] == 0 &&
                lora3a22_transfer.key[3] == 0 &&
                lora3a22_transfer.joystick[JOYSTICK_ADC_0_INDEX] == 0 &&
                lora3a22_transfer.joystick[JOYSTICK_ADC_1_INDEX] == 0 &&
                lora3a22_transfer.joystick[JOYSTICK_ADC_2_INDEX] == 0 &&
                lora3a22_transfer.joystick[JOYSTICK_ADC_3_INDEX] == 0
               )
            {

                if((REMOTE_IDLE_TIME * (1000 / REMOTE_SEND_INTERVAL)) < lora3a22_idle_count)
                {
                    lora3a22_sleep_flag = 1;

                    // 设置RGB LED状态
                    rt_event_send(led_event, RGB_LOW_POWER_EVENT);

                    // 设置为低功耗模式
                    lora3a22_set_work_mode(&lora3a22_local, LOW_POWER_MODE);

                    rt_kprintf("LOW_POWER_MODE \r\n");
                }
            }
            else
            {
                lora3a22_idle_count = 0;
            }

            // 计算和校验
            for(uint8 i = 0; i < sizeof(lora3a22_transfer); i++)
            {
                temp_sum_check += buff[i];
            }

            lora3a22_transfer.sum_check = temp_sum_check;

        }
        else if(lora3a22_sleep_flag == 1)
        {

            // 关机
            if((REMOTE_POWEROFF_TIME * (1000 / REMOTE_SEND_INTERVAL)) < lora3a22_idle_count)
            {
                poweroff();
            }

            // 遥感被操作，进入正常发送模式
            if( lora3a22_transfer.key[0] != 0 ||
                lora3a22_transfer.key[1] != 0 ||
                lora3a22_transfer.key[2] != 0 ||
                lora3a22_transfer.key[3] != 0 ||
                lora3a22_transfer.joystick[JOYSTICK_ADC_0_INDEX] != 0 ||
                lora3a22_transfer.joystick[JOYSTICK_ADC_1_INDEX] != 0 ||
                lora3a22_transfer.joystick[JOYSTICK_ADC_2_INDEX] != 0 ||
                lora3a22_transfer.joystick[JOYSTICK_ADC_3_INDEX] != 0
               )
            {
                lora3a22_sleep_flag = 0;
                lora3a22_idle_count = 0;

                // 设置RGB LED状态
                rt_event_send(led_event, RGB_NORMAL_EVENT);

                // 设置为透传模式
                lora3a22_set_work_mode(&lora3a22_local, TRANSPARENT_MODE);

                rt_kprintf("TRANSPARENT_MODE \r\n");
            }
        }

        // 正常模式就发送数据
        if(lora3a22_sleep_flag == 0)
        {
            lora3a22_send_dat(&lora3a22_local, buff, sizeof(lora3a22_transfer));
        }

        rt_thread_mdelay(REMOTE_SEND_INTERVAL);

        // 释放互斥量
        rt_mutex_release(lora3a22_mutex);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       LORA模组发送线程创建
// @param       void
// @return      uint8       0-成功，1-失败
// @note
//-------------------------------------------------------------------------------------------------------------------
uint8 lora3a22_send_thread_create(void)
{
    lora3a22_mutex = rt_mutex_create("lora3a22_mutex", RT_IPC_FLAG_FIFO);
    if (lora3a22_mutex == RT_NULL)
    {
        rt_kprintf("create lora3a22_mutex failed.\n");
    }


    td_lora3a22_send = rt_thread_create("lora3a22_send",                 //线程名称
                        lora3a22_send_thread,             // 线程入口函数
                        RT_NULL,                         // 线程参数
                        512,                             // 512个字节的栈空间
                        1,                              //数值越小，优先级越高，0为最高优先级。
                                                      //可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
                        100);                            //时间片为100


    if(td_lora3a22_send != RT_NULL)                   //线程创建成功
    {
        rt_kprintf("td_lora3a22_send create OK.\n");
        rt_thread_startup(td_lora3a22_send);          //运行该线程
    }
    else                                        //线程创建失败
    {
        rt_kprintf("td_lora3a22_send create ERROR.\n");
        return 1;
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       设置默认配对信道
// @param       parameter   参数
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void set_pair_channel(int8 channel)
{
    // 信道最大值0x73，不允许超过。
    if(channel > 0x73)
    {
        channel = 0x73;
    }
    default_pair_channel = channel;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       配对线程
// @param       parameter   参数
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void pair_thread(void* parameter)
{
    uint16 count = 0;
    uint32 rng = 0;
    uint8  channel = 0;
    uint8  group_num = 0;
    uint8  addr = 0;

    for(;;)
    {
        // 配对按键检测
        if(GPIO_ReadInputDataBit(KEY_PAIR_GROUP, KEY_PAIR_PIN) == 0)
        {
            count++;

            // 按下按下超过 REMOTE_KEY_CALIBRATION_PAIR ms被认为按下
            if(count >= REMOTE_KEY_CALIBRATION_PAIR)
            {
                // 置随机数种子
                srand((unsigned int)SysTick->CNT);

                count = 0;
                rt_kprintf("start pair...\r\n");
                // 获取互斥量防止冲突
                rt_mutex_take(lora3a22_mutex, RT_WAITING_FOREVER);
                // 设置RGB LED状态
                rt_event_send(led_event, RGB_PAIR_OR_CALIBRATION_EVENT);

                rng = rand() % 0xFFFF;

                rt_kprintf("rng = %X\r\n", rng);

                if (default_pair_channel == -1)
                {
                    channel = rng & 0xFF;
                    group_num = (rng >> 8) & 0xFF;
                    addr = (rng >> 16) & 0xFF;
                    // 避免信道使用23信道，所以排除18到28之间的信道，防止干扰
                    // 避免通道号大于0x73
                    while((channel >= 0x12 && channel <= 0x1C) || channel > 0x73)
                    {
                        rng = rand() % 0xFFFF;

                        channel = rng & 0xFF;

                        rt_thread_mdelay(1);
                    }
                }
                else
                {
                    channel = default_pair_channel;
                    group_num = (rng >> 8) & 0xFF;
                    addr = (rng >> 16) & 0xFF;
                }


                rt_kprintf("channel = 0x%X.\r\n", channel);
                rt_kprintf("group_num = 0x%X.\r\n", group_num);
                rt_kprintf("addr = 0x%X.\r\n", addr);


                uint8 pair_state = 0;
                do
                {
                    // 设置排针端的LORA模组
                    if(lora3a22_pair(&lora3a22_local, channel, group_num, addr))
                    {
                        rt_kprintf("set lora3a22 target error\r\n");
                        pair_state = 1;
                        break;
                    }
                    else
                    {
                        rt_kprintf("set lora3a22 target ok\r\n");
                    }

                    // 设置遥控器上面的LORA模组
                    if(lora3a22_pair(&lora3a22_target, channel, group_num, addr))
                    {
                        rt_kprintf("set lora3a22 local error\r\n");
                        pair_state = 1;
                        break;
                    }
                    else
                    {
                        rt_kprintf("set lora3a22 local ok\r\n");
                    }


                }while(0);


                if(pair_state == 1)
                {
                    for(uint8 i = 0; i < 25 ;i++)
                    {
                        rt_event_send(led_event, RGB_PAIR_OR_CALIBRATION_EVENT);
                        rt_thread_mdelay(100);
                        rt_event_send(led_event, RGB_LOW_POWER_EVENT);
                        rt_thread_mdelay(100);
                    }
                }

                // 设置RGB LED状态
                rt_event_send(led_event, RGB_NORMAL_EVENT);

                // 如果现在是低功耗模式，强制设置为正常模式发送数据
                if(lora3a22_sleep_flag)
                {
                    lora3a22_idle_count = 0;
                    lora3a22_sleep_flag = 0;
                    // 设置为透传模式
                    lora3a22_set_work_mode(&lora3a22_local, TRANSPARENT_MODE);
                }

                // 释放互斥量
                rt_mutex_release(lora3a22_mutex);

                rt_kprintf("stop pair...\r\n");
                // 等待松开按键
                while(GPIO_ReadInputDataBit(KEY_PAIR_GROUP, KEY_PAIR_PIN) == 0)
                {
                    rt_thread_mdelay(1);
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
// @brief       配对线程创建
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
uint8 pair_thread_create(void)
{

    td_pair = rt_thread_create("pair",                 //线程名称
                        pair_thread,             // 线程入口函数
                        RT_NULL,                         // 线程参数
                        512,                             // 512个字节的栈空间
                        2,                              //数值越小，优先级越高，0为最高优先级。
                                                      //可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
                        100);                            //时间片为100


    if(td_pair != RT_NULL)                   //线程创建成功
    {
        rt_kprintf("pair_thread create OK.\n");
        rt_thread_startup(td_pair);          //运行该线程
    }
    else                                        //线程创建失败
    {
        rt_kprintf("pair_thread create ERROR.\n");
        return 1;
    }


    return 0;

}

//-------------------------------------------------------------------------------------------------------------------
// @brief       校准线程
// @param       parameter   参数
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void calibration_thread(void* parameter)
{
    uint16 count = 0;

    for(;;)
    {
        if(GPIO_ReadInputDataBit(KEY_CALIBRATION_GROUP, KEY_CALIBRATION_PIN) == 0)
        {
            count++;
            // 按下按下超过 REMOTE_KEY_CALIBRATION_PAIR ms被认为按下
            if(count >= REMOTE_KEY_CALIBRATION_PAIR)
            {
                count = 0;
                rt_kprintf("start calibration...\r\n");

                // 获取互斥量防止冲突
                rt_mutex_take(lora3a22_mutex, RT_WAITING_FOREVER);

                // 设置RGB LED状态
                rt_event_send(led_event, RGB_PAIR_OR_CALIBRATION_EVENT);

                // 开始校准
                joystick_calibration(REMOTE_CALIBRATION_TIME);

                //
                rt_event_send(led_event, RGB_NORMAL_EVENT);

                // 如果现在是低功耗模式，强制设置为正常模式发送数据
                if(lora3a22_sleep_flag)
                {
                    lora3a22_idle_count = 0;
                    lora3a22_sleep_flag = 0;
                    // 设置为透传模式
                    lora3a22_set_work_mode(&lora3a22_local, TRANSPARENT_MODE);
                }

                // 释放互斥量
                rt_mutex_release(lora3a22_mutex);

                rt_kprintf("stop calibration...\r\n");

                // 等待松开按键
                while(GPIO_ReadInputDataBit(KEY_CALIBRATION_GROUP, KEY_CALIBRATION_PIN) == 0)
                {
                    rt_thread_mdelay(1);
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
// @brief       校准线程创建
// @param       void
// @return      uint8       0-成功，1-失败
// @note
//-------------------------------------------------------------------------------------------------------------------
uint8 calibration_thread_create(void)
{


    td_calibration = rt_thread_create("calibration",                 //线程名称
            calibration_thread,             // 线程入口函数
                        RT_NULL,                         // 线程参数
                        512,                             // 512个字节的栈空间
                        2,                              //数值越小，优先级越高，0为最高优先级。
                                                      //可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
                        100);                            //时间片为100


    if(td_calibration != RT_NULL)                   //线程创建成功
    {
        rt_kprintf("calibration_thread create OK.\n");
        rt_thread_startup(td_calibration);          //运行该线程
    }
    else                                        //线程创建失败
    {
        rt_kprintf("calibration_thread create ERROR.\n");
        return 1;
    }

    return 0;

}


//-------------------------------------------------------------------------------------------------------------------
// @brief       配对和校准引脚初始化
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void key_pair_calibration_init(void)
{

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = KEY_PAIR_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY_PAIR_GROUP, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = KEY_CALIBRATION_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY_CALIBRATION_GROUP, &GPIO_InitStructure);

}





