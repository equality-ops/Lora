/*********************************************************************************************************************
* AI8051U Opensourec Library 即（AI8051U 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是STC 开源库的一部分
*
* AI8051U 开源库 是免费软件
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
* 文件名称          
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MDK FOR C251
* 适用平台          AI8051U
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者           备注
* 2024-08-01        大W            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"
#include "zf_device_lora3a22.h"


#define PIT_CH                  (TIM0_PIT )                	// 使用的周期中断编号 如果修改 需要同步对应修改周期中断编号与 isr.c 中的调用
#define PIT_PRIORITY            (TIMER0_IRQn)              	// 对应周期中断的中断编号

#define PIT_MS					(5)							// 周期中断时间5ms

void pit_handler();

void main()
{
    clock_init(SYSTEM_CLOCK_40M);				// 务必保留
	debug_init();								// 务必保留

	lora3a22_init();										// 初始化LORA模块

	tim0_irq_handler = pit_handler;							// 设置定时器0周期中断回调函数	
	
	pit_ms_init(PIT_CH, 5);                          		// 初始化 PIT_CH0 为周期中断 1000ms 周期
	
    interrupt_set_priority(PIT_PRIORITY, 0);            	// 设置 PIT1 对周期中断的中断优先级为 0，0为最低优先级
  
	
	
    while(1)
	{
		
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
		
		P52 = !P52;

		system_delay_ms(200);
    }
}


// 该函数在isr.c中进行回调
void pit_handler()
{

    lora3a22_response_time++;
    if (lora3a22_response_time > 500 / PIT_MS)   	//500ms 没有接受倒数据判断位发送端异常
    {
        lora3a22_state_flag = 0;                    //遥控器状态位清零
        lora3a22_response_time = 0;
    }

}	