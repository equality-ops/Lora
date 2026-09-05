/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-12-18
 ********************************************************************************************************************/

#include "headfile.h"

#include "zf_device_lora3a22.h"
/*
 * 系统频率，可查看board.h中的 FOSC 宏定义修改。
 * board.h文件中FOSC的值设置为0,则程序自动设置系统频率为33.1776MHZ
 * 在board_init中,已经将P54引脚设置为复位
 * 如果需要使用P54引脚,可以在board.c文件中的board_init()函数中删除SET_P54_RESRT即可
 */
#define PIT_MS 5
void main()
{
	board_init();			// 初始化寄存器,勿删除此句代码。
	
	// 此处编写用户代码(例如：外设初始化代码等)
	lora3a22_init();
	
	
	pit_timer_ms(TIM_0, PIT_MS);                          		// 初始化 TIM_0 为周期中断 5ms 周期
	
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

		delay_ms(200);
    }
}

// 该函数在isr.c中进行回调
void pit_callback()
{

    lora3a22_response_time++;
    if (lora3a22_response_time > 500 / PIT_MS)   	//500ms 没有接受倒数据判断位发送端异常
    {
        lora3a22_state_flag = 0;                    //遥控器状态位清零
        lora3a22_response_time = 0;
    }

}	