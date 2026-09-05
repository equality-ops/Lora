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
* 文件名称          lora3a22_config
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

#ifndef _zf_common_clock_h_
#define _zf_common_clock_h_

#include "zf_common_typedef.h"
#include "uart.h"

#define LORA3A22_MODULE_VERSION 						0x00
#define LORA3A22_RSSI 									0x01
	
#define LORA3A22_BAUD 								0x04
#define LORA3A22_PARAMETER 						0x05
// BIT11-BIT5 	信道，默认为10111（23）信道
// BIT4-BIT3 	功率，默认为11，功率为21dBm
// BIT2-BIT0	空速，默认为010，空速为4.8K
#define LORA3A22_CHANNEL_POWER_AIRSPEED					0x06
// 默认为透传模式
#define LORA3A22_WORK_MODE								0x07
// Bit0=0,主机
// Bit0=1,从机
#define LORA3A22_HOST_SLAVE								0x0A
#define LORA3A22_AES_KEY								0x0B
// 唤醒时间，100ms为单位
#define LORA3A22_WAKE_TIME								0x14
#define LORA3A22_WORK_POTION							0x17
#define LORA3A22_LOCAL_GROUP_NUMBER						0x18
#define LORA3A22_LOCAL_ADDR								0x19
#define LORA3A22_TARGET_GROUP_NUMBER					0x1A
#define LORA3A22_TARGET_ADDR							0x1B
#define LORA3A22_RELAY_MODE_A_GROUP_NUMBER				0x1C
#define LORA3A22_RELAY_MODE_A_ADDR						0x1D
#define LORA3A22_RELAY_MODE_B_GROUP_NUMBER				0x1E
#define LORA3A22_RELAY_MODE_B_ADDR						0x1F
#define LORA3A22_RELAY_MODE_JUMP_TEMP_CHANNEL			0x20
#define LORA3A22_RELAY_MODE_TEMP_CHANNEL_RESIDENCE_TIME	0x21
#define LORA3A22_MODULE_RESTART							0x22
#define LORA3A22_MODULE_DEFAULT							0x23


#define LORA3A22_POWER_21DBM                         0x03
#define LORA3A22_POWER_17DBM                         0x02
#define LORA3A22_POWER_14DBM                         0x01
#define LORA3A22_POWER_11DBM                         0x00


#define LORA3A22_AIR_SPEED_62_5K                         0x07
// 65ms 发送512个字节
#define LORA3A22_AIR_SPEED_50K                           0x06
// 82ms 发送512个字节
#define LORA3A22_AIR_SPEED_38_4K                         0x05
// 107ms 发送512个字节
#define LORA3A22_AIR_SPEED_19_2K                         0x04
// 214ms 发送512个字节
#define LORA3A22_AIR_SPEED_9_6K                          0x03
// 427ms 发送512个字节
#define LORA3A22_AIR_SPEED_4_8K                          0x02
// 853ms 发送512个字节
#define LORA3A22_AIR_SPEED_2_4K                          0x01
// 1707ms 发送512个字节
#define LORA3A22_AIR_SPEED_1_2K                          0x00
// 3414ms 发送512个字节

#define LORA3A22_LOCAL_UART_INDEX                         (USART1)
#define LORA3A22_LOCAL_UART_INIT_FUNC                     (uart1_init)
#define LORA3A22_LOCAL_UART_TX_GPOUP                      (GPIOA)
#define LORA3A22_LOCAL_UART_TX_PIN                        (GPIO_Pin_10)
#define LORA3A22_LOCAL_UART_RX_GPOUP                      (GPIOA)
#define LORA3A22_LOCAL_UART_RX_PIN                        (GPIO_Pin_9)
#define LORA3A22_LOCAL_GPIO_RST_GPOUP                     (GPIOD)
#define LORA3A22_LOCAL_GPIO_RST_PIN                       (GPIO_Pin_1)
#define LORA3A22_LOCAL_GPIO_AUX_GPOUP                     (GPIOA)
#define LORA3A22_LOCAL_GPIO_AUX_PIN                       (GPIO_Pin_8)
#define LORA3A22_LOCAL_GPIO_MD0_GPOUP                     (GPIOA)
#define LORA3A22_LOCAL_GPIO_MD0_PIN                       (GPIO_Pin_12)
#define LORA3A22_LOCAL_GPIO_MD1_GPOUP                     (GPIOA)
#define LORA3A22_LOCAL_GPIO_MD1_PIN                       (GPIO_Pin_11)


#define LORA3A22_TARGET_UART_INDEX                         (UART4)
#define LORA3A22_TARGET_UART_INIT_FUNC                     (uart4_init)
#define LORA3A22_TARGET_UART_TX_GPOUP                      (GPIOB)
#define LORA3A22_TARGET_UART_TX_PIN                        (GPIO_Pin_0)
#define LORA3A22_TARGET_UART_RX_GPOUP                      (GPIOB)
#define LORA3A22_TARGET_UART_RX_PIN                        (GPIO_Pin_1)
#define LORA3A22_TARGET_GPIO_RST_GPOUP                     (GPIOD)
#define LORA3A22_TARGET_GPIO_RST_PIN                       (GPIO_Pin_0)
#define LORA3A22_TARGET_GPIO_MD0_GPOUP                     (GPIOC)
#define LORA3A22_TARGET_GPIO_MD0_PIN                       (GPIO_Pin_15)

#define LORA3A22_TARGET_GPIO_AUX_GPOUP                     (NULL)
#define LORA3A22_TARGET_GPIO_AUX_PIN                       (0)
#define LORA3A22_TARGET_GPIO_MD1_GPOUP                     (NULL)
#define LORA3A22_TARGET_GPIO_MD1_PIN                       (0)

#define LORA3A22_UART_BUFF_LEN                             (64)

#define LORA3A22_SEND_TIMEOUT_COUNT                        (200)




typedef struct
{
    USART_TypeDef   *uart_x;        // 串口号

    GPIO_TypeDef    *tx_group;
    uint16          tx_pin;
    GPIO_TypeDef    *rx_group;
    uint16          rx_pin;

    GPIO_TypeDef    *rst_group;
    uint16          rst_pin;

    GPIO_TypeDef    *aux_group;
    uint16          aux_pin;

    GPIO_TypeDef    *md0_group;
    uint16          md0_pin;

    GPIO_TypeDef    *md1_group;
    uint16          md1_pin;

    void            (*uart_init)(uint32 );

}lora3a22_struct;


extern lora3a22_struct lora3a22_local;
extern lora3a22_struct lora3a22_target;

typedef enum
{
    CONFIG_MODE = 0,
    TRANSPARENT_MODE = 1,
    LOW_POWER_MODE = 2,
}lora3a22_work_mode_enum;




uint8 lora3a22_pair(lora3a22_struct *lora3a22_uart, uint8 channel, uint8 group_num, uint8 addr);
uint8 lora3a22_send_dat(lora3a22_struct *lora3a22_uart, uint8 *buff, uint32 len);
uint8 lora3a22_set_work_mode(lora3a22_struct *lora3a22_uart, lora3a22_work_mode_enum work_mode);


uint8 lora3a22_init(void);

#endif
