/*********************************************************************************************************************
* TC364 Opensourec Library 即（TC364 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 TC364 开源库的一部分
*
* TC364 开源库 是免费软件
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
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          cpu0_main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          ADS v1.9.20
* 适用平台          TC364DP
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2022-11-02       pudding            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中

// *************************** 例程硬件连接说明 ***************************
// lora3a22无线串口模块
//      模块管脚            单片机管脚
//      RX                  查看 zf_device_lora3a22.h 中 LORA3A22_UART_RX_PIN  宏定义 默认 P10_6
//      TX                  查看 zf_device_lora3a22.h 中 LORA3A22_UART_TX_PIN  宏定义 默认 P10_5
//      GND                 核心板电源地 GND
//      3V3                 核心板 3V3 电源

// *************************** 例程测试说明 ***************************
// 1.核心板烧录完成本例程，单独使用核心板与调试下载器或者 USB-TTL 模块，并连接好编码器，在断电情况下完成连接
// 2.将调试下载器或者 USB-TTL 模块连接电脑 完成上电
// 3.电脑上使用串口助手打开对应的串口，串口波特率为 zf_common_debug.h 文件中 DEBUG_UART_BAUDRATE 宏定义 默认 115200，核心板按下复位按键
// 4.可以在串口助手上看到如下串口信息：
//      head = 163
//      sum_check = 165
//      key0 = 1
//      key1 = 1
//      joystick[0] = 0
//      joystick[1] = 0
//      joystick[2] = 0
//      joystick[3] = 0
// 5.操作摇杆数据发送变化
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查

// **************************** 代码区域 ****************************
#include "zf_device_lora3a22.h"

#define PIT_NUM                 (CCU60_CH0 )                            // 使用的周期中断编号
#define PIT_period              (10 )                                   // 中断周期


int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等

    lora3a22_init();
    pit_ms_init(PIT_NUM, PIT_period);                                    // 初始化 CCU6_0_CH0 为周期中断 10ms 周期

    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();         // 等待所有核心初始化完毕
    while (TRUE)
    {
        // 此处编写需要循环执行的代码


        if (lora3a22_state_flag == 1)
        {
            if (lora3a22_finsh_flag == 1)
            {

                printf ("head = %d\r\n",lora3a22_uart_transfer.head);
                //lora3a22 帧头

                printf ("sum_check = %d\r\n",lora3a22_uart_transfer.sum_check);
                //lora3a22 和校验

                printf ("key0 = %d\r\n",lora3a22_uart_transfer.key[0]);
                //左边摇杆按键
                printf ("key1 = %d\r\n",lora3a22_uart_transfer.key[1]);
                //右边摇杆按键

                printf ("joystick[0] = %d\r\n",lora3a22_uart_transfer.joystick[0]);
                //左边摇杆左右值
                printf ("joystick[1] = %d\r\n",lora3a22_uart_transfer.joystick[1]);
                //左边摇杆上下值
                printf ("joystick[2] = %d\r\n",lora3a22_uart_transfer.joystick[2]);
                //右边摇杆左右值
                printf ("joystick[3] = %d\r\n",lora3a22_uart_transfer.joystick[3]);
                //右边摇杆上下值

               lora3a22_finsh_flag = 0;
            }

        }
        else
        {
            printf("lora3a22 connection fail \r\n");
        }

        system_delay_ms(50);

        // 此处编写需要循环执行的代码
    }
}

IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                      // 开启中断嵌套
    pit_clear_flag(CCU60_CH0);

    lora3a22_response_time++;
    if (lora3a22_response_time > 500 / PIT_period)   //500ms 没有接受倒数据判断位发送端异常
    {
        lora3a22_state_flag = 0;                     //遥控器状态位清零
        lora3a22_response_time = 0;
    }

}

#pragma section all restore
// **************************** 代码区域 ****************************

// *************************** 例程常见问题说明 ***************************
// 遇到问题时请按照以下问题检查列表检查
// 问题1：串口没有数据
//      查看串口助手打开的是否是正确的串口，检查打开的 COM 口是否对应的是 lora3a22模块 的 COM 口
//      查看lora3a22模块TX灯状态 如果快速闪烁证明模块接收倒数据
// 问题2：串口数据乱码
//      查看是否模块波特率被修改过 恢复出厂设置
// 问题3：遥控器端有发送数据，但接受端没有数据
//      重新配对模块
