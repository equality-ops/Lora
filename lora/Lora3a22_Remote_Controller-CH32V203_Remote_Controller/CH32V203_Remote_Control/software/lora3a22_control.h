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

#ifndef _zf_lora3a22_control_h
#define _zf_lora3a22_control_h


#include "zf_common_typedef.h"


#define KEY_PAIR_GROUP          (GPIOB)
#define KEY_PAIR_PIN            (GPIO_Pin_9)


#define KEY_CALIBRATION_GROUP   (GPIOA)
#define KEY_CALIBRATION_PIN     (GPIO_Pin_3)


typedef struct
{
    uint8 head;                         // 帧头
    uint8 sum_check;                    // 和校验

    int16 joystick[4];
    // 0-左边 左右
    // 1-左边 上下
    // 2-右边 左右
    // 3-右边 上下

    uint8 key[4];
    // 0-摇杆左边
    // 1-摇杆右边
    // 2-侧向按键左边
    // 3-侧向按键右边

    uint8 switch_key[4];
    // 0-左边_1
    // 1-左边_2
    // 2-右边_1
    // 3-右边_2

}lora3a22_uart_transfer_dat_struct;

extern lora3a22_uart_transfer_dat_struct lora3a22_uart_transfer;


uint8 lora3a22_send_thread_create(void);

void key_pair_calibration_init(void);
uint8 pair_thread_create(void);
uint8 calibration_thread_create(void);
void set_pair_channel(int8 channel);

#endif
