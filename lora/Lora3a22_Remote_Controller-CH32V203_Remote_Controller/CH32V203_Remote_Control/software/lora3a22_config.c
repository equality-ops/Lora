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

#include "lora3a22_config.h"

#include "stdio.h"
#include "ch32v20x.h"
#include "zf_common_fifo.h"


uint8 lora3a22_uart_buff[LORA3A22_UART_BUFF_LEN];
fifo_struct lora3a22_uart_fifo;


lora3a22_struct lora3a22_local;
lora3a22_struct lora3a22_target;


//-------------------------------------------------------------------------------------------------------------------
// @brief       遥控器LORA模块回调函数
// @param       dat     串口数据
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void lora3a22_local_callback(uint8 dat)
{
    fifo_write_buffer(&lora3a22_uart_fifo, &dat, 1);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       串口中断服务函数
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void USART1_IRQHandler(void) __attribute__((interrupt()));
void USART1_IRQHandler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();

    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        lora3a22_local_callback(USART1->DATAR);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }

    rt_interrupt_leave();
    FREE_INT_SP();
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       排针端LORA模块回调函数
// @param       dat     串口数据
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void lora3a22_target_callback(uint8 dat)
{
    fifo_write_buffer(&lora3a22_uart_fifo, &dat, 1);
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       串口中断服务函数
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void UART4_IRQHandler(void) __attribute__((interrupt()));
void UART4_IRQHandler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();

    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {
        lora3a22_target_callback(UART4->DATAR);
        USART_ClearITPendingBit(UART4, USART_IT_RXNE);
    }

    rt_interrupt_leave();
    FREE_INT_SP();
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       LORA模块设置工作模式
// @param       *lora3a22_uart      需要操作的LORA模块对象
// @param       work_mode           工作模式
// @return      uint8               0-成功，1-失败
// @note
//-------------------------------------------------------------------------------------------------------------------
uint8 lora3a22_set_work_mode(lora3a22_struct *lora3a22_uart, lora3a22_work_mode_enum work_mode)
{
    uint8 return_state = 0;
    int16 time_out = 0;

    do{

        if(lora3a22_uart->aux_group != NULL)
        {
            // 工作状态切换只在AUX为高电平时才能切换
            // 若AUX为低电平，则表示此时模块繁忙，发送（接收）不为空，数据还未发送（接收收）完成，需用户添加延时，
            // 等待数据收发完成后，即可开始工作状态切换。
            time_out = 200;
            while((0 < time_out--) && (!GPIO_ReadInputDataBit(lora3a22_uart->aux_group, lora3a22_uart->aux_pin)))
            {
                rt_thread_mdelay(1);
            }
            if(0 > time_out)
            {
                return_state = 1;
                break;
            }
        }
        else
        {
            rt_thread_mdelay(200);
        }


        if(work_mode == CONFIG_MODE)
        {
            // 设置配置模式
            GPIO_WriteBit(lora3a22_uart->md0_group, lora3a22_uart->md0_pin, 0);
            if(lora3a22_uart->md1_group != NULL)
            {
                GPIO_WriteBit(lora3a22_uart->md1_group, lora3a22_uart->md1_pin, 0);
            }

        }
        else if(work_mode == TRANSPARENT_MODE)
        {
            // 设置透传模式
            GPIO_WriteBit(lora3a22_uart->md0_group, lora3a22_uart->md0_pin, 1);
            if(lora3a22_uart->md1_group != NULL)
            {
                GPIO_WriteBit(lora3a22_uart->md1_group, lora3a22_uart->md1_pin, 0);
            }
        }
        else if(work_mode == LOW_POWER_MODE)
        {
            // 设置低功耗模式
            GPIO_WriteBit(lora3a22_uart->md0_group, lora3a22_uart->md0_pin, 1);
            if(lora3a22_uart->md1_group != NULL)
            {
                GPIO_WriteBit(lora3a22_uart->md1_group, lora3a22_uart->md1_pin, 1);
            }
        }


        // 工作模式开始切换至配置模式需要120ms时长
        rt_thread_mdelay(120);

        if(lora3a22_uart->aux_group != NULL)
        {
            // 等待AUX引脚为高电平，代表初始化完成
            time_out = 200;
            while((0 < time_out--) && (!GPIO_ReadInputDataBit(lora3a22_uart->aux_group, lora3a22_uart->aux_pin)))
            {
                rt_thread_mdelay(1);
            }
            if(0 > time_out)
            {
                return_state = 1;
                break;
            }
        }
        else
        {
            rt_thread_mdelay(200);
        }
    }while(0);

    return return_state;
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       LORA模块读取多个寄存器
// @param       *lora3a22_uart      需要操作的LORA模块对象
// @param       reg                 寄存器首地址
// @param       reg_len             需要读取的寄存器个数
// @param       *buff               返回的寄存器数据buff
// @return      uint32              0-失败，其他-数据长度
// @note
//-------------------------------------------------------------------------------------------------------------------
uint32 lora3a22_read_regs(lora3a22_struct *lora3a22_uart, uint8 reg, uint8 reg_len, uint8 *buff)
{
//    int16 time_out = 0;
    uint32 buff_len = LORA3A22_UART_BUFF_LEN;
    do{
        // 清空FIFO
        fifo_clear(&lora3a22_uart_fifo);

        // 命令字
        uart_write_byte(lora3a22_uart->uart_x, 0x00);
        // 寄存器地址
        uart_write_byte(lora3a22_uart->uart_x, reg);
        // 长度
        uart_write_byte(lora3a22_uart->uart_x, reg_len);

        // 这里的500ms，是保证足够长的时间。
        rt_thread_mdelay(500);

        if(0 < fifo_used(&lora3a22_uart_fifo))
        {
            // 读取到数据，将数据存入BUFF中
            fifo_read_buffer(&lora3a22_uart_fifo, buff, &buff_len, FIFO_READ_AND_CLEAN);
        }
        else
        {
            buff_len = 0;
            break;
        }

    }while(0);

    return buff_len;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       LORA模块读取一个寄存器
// @param       *lora3a22_uart      需要操作的LORA模块对象
// @param       reg                 寄存器首地址
// @param       *buff               返回的寄存器数据buff
// @return      uint32              0-失败，其他-数据长度
// @note
//-------------------------------------------------------------------------------------------------------------------
uint32 lora3a22_read_reg(lora3a22_struct *lora3a22_uart, uint8 reg, uint8 *buff)
{
    uint32 buff_len = LORA3A22_UART_BUFF_LEN;
    do{


        // 清空FIFO
        fifo_clear(&lora3a22_uart_fifo);

        // 命令字
        uart_write_byte(lora3a22_uart->uart_x, 0x00);
        // 寄存器地址
        uart_write_byte(lora3a22_uart->uart_x, reg);
        // 长度
        uart_write_byte(lora3a22_uart->uart_x, 0x01);
        // 这里的500ms，是保证足够长的时间。
        rt_thread_mdelay(500);
        if(0 < fifo_used(&lora3a22_uart_fifo))
        {
            // 读取到数据，将数据存入BUFF中
            fifo_read_buffer(&lora3a22_uart_fifo, buff, &buff_len, FIFO_READ_AND_CLEAN);
        }
        else
        {
            buff_len = 0;
            break;
        }

    }while(0);

    return buff_len;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       LORA模块写入多个寄存器
// @param       *lora3a22_uart      需要操作的LORA模块对象
// @param       reg                 寄存器首地址
// @param       reg_len             需要写入的寄存器个数
// @param       *buff               写入的数据buff
// @param       *buff               写入的数据buff长度
// @return      uint8               0-成功，1-失败
// @note
//-------------------------------------------------------------------------------------------------------------------
uint8 lora3a22_write_regs(lora3a22_struct *lora3a22_uart, uint8 reg, uint8 reg_len, const uint8 *buff, uint8 buff_len)
{
    uint8 return_state = 0;
    int16 time_out = 0;
    uint8 recv_buff[3] = {0};
    uint32 recv_len = 3;
    do{
        // 清空FIFO
        fifo_clear(&lora3a22_uart_fifo);

        // 命令字
        uart_write_byte(lora3a22_uart->uart_x, 0x80);
        // 寄存器地址
        uart_write_byte(lora3a22_uart->uart_x, reg);
        // 配置的寄存器个数
        uart_write_byte(lora3a22_uart->uart_x, reg_len);
        // 传输buff
        uart_write_buffer(lora3a22_uart->uart_x, buff, buff_len);

        // 配置命令响应最大延时为140ms + 数据时长
        time_out = 200;
        while((0 < time_out--) && (3 > fifo_used(&lora3a22_uart_fifo)))
        {
            rt_thread_mdelay(1);
        }

        if((0 < time_out))
        {
             fifo_read_buffer(&lora3a22_uart_fifo, recv_buff, &recv_len, FIFO_READ_AND_CLEAN);
        }
        else
        {
            return_state = 1;
            break;
        }

        if((0X80 != recv_buff[0]) || (reg != recv_buff[1]) || (reg_len != recv_buff[2]))
        {

            return_state = 1;
            break;
        }

    }while(0);

    return return_state;
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       LORA模块写入一个寄存器
// @param       *lora3a22_uart      需要操作的LORA模块对象
// @param       reg                 寄存器地址
// @param       *buff               写入的数据buff
// @param       *buff               写入的数据buff长度
// @return      uint8               0-成功，1-失败
// @note
//-------------------------------------------------------------------------------------------------------------------
uint8 lora3a22_write_reg(lora3a22_struct *lora3a22_uart, uint8 reg, const uint8 *buff, uint8 buff_len)
{
    uint8 return_state = 0;
    int16 time_out = 0;
    uint8 recv_buff[3] = {0};
    uint32 recv_len = 3;
    do{
        // 清空FIFO
        fifo_clear(&lora3a22_uart_fifo);

        // 命令字
        uart_write_byte(lora3a22_uart->uart_x, 0x80);
        // 寄存器地址
        uart_write_byte(lora3a22_uart->uart_x, reg);
        // 配置的寄存器个数
        uart_write_byte(lora3a22_uart->uart_x, 0x01);
        // 传输buff
        uart_write_buffer(lora3a22_uart->uart_x, buff, buff_len);


        // 配置命令响应最大延时为140ms + 数据时长
        time_out = 200;
        while((0 < time_out--) && (3 > fifo_used(&lora3a22_uart_fifo)))
        {
            rt_thread_mdelay(1);
        }

        if((0 < time_out))
        {
             fifo_read_buffer(&lora3a22_uart_fifo, recv_buff, &recv_len, FIFO_READ_AND_CLEAN);
        }
        else
        {
            return_state = 1;
            break;
        }

        if((0X80 != recv_buff[0]) || (reg != recv_buff[1]) || (0x01 != recv_buff[2]))
        {
            return_state = 1;
            break;
        }

    }while(0);

    return return_state;
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       LORA模组引脚初始化
// @param       *lora3a22_uart      需要操作的LORA模块对象
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
static void lora3a22_gpio_init(lora3a22_struct *lora3a22_uart)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    if(lora3a22_uart->rst_group != NULL)
    {
        GPIO_InitStructure.GPIO_Pin = lora3a22_uart->rst_pin;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(lora3a22_uart->rst_group, &GPIO_InitStructure);
    }
    if(lora3a22_uart->aux_group != NULL)
    {
        GPIO_InitStructure.GPIO_Pin = lora3a22_uart->aux_pin;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(lora3a22_uart->aux_group, &GPIO_InitStructure);
    }
    if(lora3a22_uart->md0_group != NULL)
    {
        GPIO_InitStructure.GPIO_Pin = lora3a22_uart->md0_pin;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(lora3a22_uart->md0_group, &GPIO_InitStructure);
    }
    if(lora3a22_uart->md1_group != NULL)
    {
        GPIO_InitStructure.GPIO_Pin = lora3a22_uart->md1_pin;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(lora3a22_uart->md1_group, &GPIO_InitStructure);
    }

    GPIO_WriteBit(lora3a22_uart->rst_group, lora3a22_uart->rst_pin, 1);
    GPIO_WriteBit(lora3a22_uart->md0_group, lora3a22_uart->md0_pin, 1);

}

//-------------------------------------------------------------------------------------------------------------------
// @brief       LORA模组自检
// @param       *lora3a22_uart      需要操作的LORA模块对象
// @return      uint8           0-成功，1-失败
// @note
//-------------------------------------------------------------------------------------------------------------------
static uint8 lora3a22_self_test(lora3a22_struct *lora3a22_uart)
{
    uint8 return_state = 0;
    uint8 recv_buff[23] = {0};
    uint32 recv_len = 23;
    uint8 buff[3] = {0x00, 0x00, 0x01};
    int16 time_out = 0;

    do{
        // 清空FIFO
        fifo_clear(&lora3a22_uart_fifo);

        // 发送获取模块版本号指令
        uart_write_buffer(lora3a22_uart->uart_x, buff, 3);

        // 配置命令响应最大延时为140ms + 数据时长
        time_out = 200;
        while((time_out-- > 0) && (fifo_used(&lora3a22_uart_fifo) < 23))
        {
            rt_thread_mdelay(1);
        }

        if(0 < time_out)
        {
            fifo_read_buffer(&lora3a22_uart_fifo, recv_buff, &recv_len, FIFO_READ_AND_CLEAN);
            if(0 == strncmp(&recv_buff[3], "A39C-T400A22S1a-", strlen("A39C-T400A22S1a-")))
            {
                rt_kprintf("module version = %s\r\n", &recv_buff[3]);
                return_state = 0;
            }
        }
        else
        {
            return_state = 1;
        }

    }while(0);


    return return_state;
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       LORA恢复出厂设置
// @param       *lora3a22_uart      需要操作的LORA模块对象
// @return      uint8               0-成功，1-失败
// @note
//-------------------------------------------------------------------------------------------------------------------
static uint8 lora3a22_restore_default(lora3a22_struct *lora3a22_uart)
{
    uint8   temp_buff[10];
    uint32  temp_len = 10;
    uint8   return_state = 0;
    int16   time_out = 0;

    do
    {

        // 清空FIFO
        fifo_clear(&lora3a22_uart_fifo);

        // 命令字
        uart_write_byte(lora3a22_uart->uart_x, 0x80);
        // 寄存器地址
        uart_write_byte(lora3a22_uart->uart_x, LORA3A22_MODULE_DEFAULT);
        // 配置的寄存器个数
        uart_write_byte(lora3a22_uart->uart_x, 0x01);

        // 配置命令响应最大延时为140ms + 数据时长
        time_out = 300;
        while((0 < time_out--) && (6 > fifo_used(&lora3a22_uart_fifo)))
        {
            rt_thread_mdelay(1);
        }

        if(0 > time_out)
        {
            return_state = 1;
            break;
        }

        fifo_read_buffer(&lora3a22_uart_fifo, temp_buff, &temp_len, FIFO_READ_AND_CLEAN);

        if(temp_len != 6 || 0 != strncmp(&temp_buff[2], "OK\r\n", strlen("OK\r\n")))
        {
            return_state = 1;
            break;
        }

        if(lora3a22_uart->aux_group != NULL)
        {
            rt_thread_mdelay(20);
            time_out = 200;
            while((0 < time_out--) && (!GPIO_ReadInputDataBit(lora3a22_uart->aux_group, lora3a22_uart->aux_pin)))
            {
                rt_thread_mdelay(1);
            }

            if(0 > time_out)
            {
                return_state = 1;
                break;
            }

        }
        else
        {
            rt_thread_mdelay(220);
        }

        // 恢复完成需要等待一段时间模块才能收到数据
        rt_thread_mdelay(120);

    } while (0);


    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       LORA模组设置组和地址
// @param       *lora3a22_uart      需要操作的LORA模块对象
// @param       group_num           组
// @param       addr                地址
// @return      uint8               0-成功，1-失败
// @note
//-------------------------------------------------------------------------------------------------------------------
uint8 lora3a22_set_group_addr(lora3a22_struct *lora3a22_uart, uint8 group_num, uint8 addr)
{
    uint8 return_state = 0;
    uint8 temp_buff[4];
    do
    {
        temp_buff[0] = group_num;
        temp_buff[1] = addr;
        temp_buff[2] = group_num;
        temp_buff[3] = addr;

        if(lora3a22_write_regs(lora3a22_uart, LORA3A22_LOCAL_GROUP_NUMBER, 4, temp_buff, 4))
        {
            return_state = 1;
            break;
        }
    }
    while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       LORA模组传输数据
// @param       *lora3a22_uart      需要操作的LORA模块对象
// @param       *buff               buff的地址
// @param       len                 长度
// @return      uint8               0-成功，1-失败
// @note
//-------------------------------------------------------------------------------------------------------------------
uint8 lora3a22_send_dat(lora3a22_struct *lora3a22_uart, uint8 *buff, uint32 len)
{
    uint16 time_count = 0;

    while(0 != len)
    {
        // 发送数据前需要等待AUX引脚为高电平
        if(GPIO_ReadInputDataBit(lora3a22_uart->aux_group, lora3a22_uart->aux_pin) == 1)
        {
            if(512 <= len)                                                      // 数据分 30byte 每包发送
            {
                uart_write_buffer(lora3a22_uart->uart_x, buff, 512);                           // 发送数据
                buff += 512;                                                     // 地址偏移
                len -= 512;                                                      // 数量
                time_count = 0;
            }
            else                                                                // 不足 512byte 的数据一次性发送完毕
            {
                uart_write_buffer(lora3a22_uart->uart_x, buff, len);                           // 发送数据
                len = 0;
                break;
            }
        }
        else
        {

            if(LORA3A22_SEND_TIMEOUT_COUNT <= (++ time_count))              // 超出了最大等待时间
            {
                break;                                                          // 退出发送
            }
            rt_thread_mdelay(1);
        }
    }


    return 0;
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       LORA模组配对
// @param       *lora3a22_uart      需要操作的LORA模块对象
// @param       channel             信道
// @param       group_num           组
// @param       addr                地址
// @return      uint8               0-成功，1-失败
// @note
//-------------------------------------------------------------------------------------------------------------------
uint8 lora3a22_pair(lora3a22_struct *lora3a22_uart, uint8 channel, uint8 group_num, uint8 addr)
{
    uint8 return_state = 0;


    // 低电平复位
    GPIO_WriteBit(lora3a22_uart->rst_group, lora3a22_uart->rst_pin, 0);
    rt_thread_mdelay(5);
    GPIO_WriteBit(lora3a22_uart->rst_group, lora3a22_uart->rst_pin, 1);

    // 初始化串口
    lora3a22_uart->uart_init(9600);

    // 设置为配置模式
    lora3a22_set_work_mode(lora3a22_uart, CONFIG_MODE);

    uint8 temp_buff[10];

    do
    {
        // 自检
        if(lora3a22_self_test(lora3a22_uart))
        {
            rt_kprintf("lora3a22_self_test error\r\n");
            return_state = 1;
            break;
        }

        // 配置

        // 恢复出厂设置
        if(lora3a22_restore_default(lora3a22_uart))
        {
            rt_kprintf("lora3a22_restore_default error\r\n");
            return_state = 1;
            break;
        }

        // 设置波特率115200
        temp_buff[0] = ((uint32)115200 >> 24) & 0xFF;
        temp_buff[1] = ((uint32)115200 >> 16) & 0xFF;
        temp_buff[2] = ((uint32)115200 >> 8) & 0xFF;
        temp_buff[3] = ((uint32)115200 >> 0) & 0xFF;
        if(lora3a22_write_reg(lora3a22_uart, LORA3A22_BAUD, temp_buff, 4))
        {
            rt_kprintf("set baud error\r\n");
            return_state = 1;
            break;
        }

        // 设置8位数据位，1位停止位，无校验
        temp_buff[0] = 0;
        if(lora3a22_write_reg(lora3a22_uart, LORA3A22_PARAMETER, temp_buff, 1))
        {
            rt_kprintf("set uart parameter error\r\n");
            return_state = 1;
            break;
        }

        if(channel > 0x73)
        {
            channel = 0x73;
        }
        // 设置信道和功率
        temp_buff[0] = (channel >> 3);
        temp_buff[1] = LORA3A22_AIR_SPEED_62_5K | (LORA3A22_POWER_21DBM << 3) | ((channel & 0x07) << 5);
        if(lora3a22_write_reg(lora3a22_uart, LORA3A22_CHANNEL_POWER_AIRSPEED, temp_buff, 2))
        {
            rt_kprintf("set channel and power error\r\n");
            return_state = 1;
            break;
        }

        // 设置组和地址
        if(lora3a22_set_group_addr(lora3a22_uart, group_num, addr))
        {
            rt_kprintf("set group and addr error\r\n");
            return_state = 1;
            break;
        }


        // 设置为透传模式
        lora3a22_set_work_mode(lora3a22_uart, TRANSPARENT_MODE);

    }
    while(0);

    lora3a22_uart->uart_init(115200);
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       LORA模组初始化
// @param       void
// @return      uint8       0-成功，1-失败
// @note
//-------------------------------------------------------------------------------------------------------------------
uint8 lora3a22_init(void)
{
    uint8 return_state = 0;

    lora3a22_local.uart_x      = LORA3A22_LOCAL_UART_INDEX;
    lora3a22_local.uart_init   = LORA3A22_LOCAL_UART_INIT_FUNC;
    lora3a22_local.tx_group    = LORA3A22_LOCAL_UART_TX_GPOUP;
    lora3a22_local.tx_pin      = LORA3A22_LOCAL_UART_TX_PIN;
    lora3a22_local.rx_group    = LORA3A22_LOCAL_UART_RX_GPOUP;
    lora3a22_local.rx_pin      = LORA3A22_LOCAL_UART_RX_PIN;
    lora3a22_local.rst_group   = LORA3A22_LOCAL_GPIO_RST_GPOUP;
    lora3a22_local.rst_pin     = LORA3A22_LOCAL_GPIO_RST_PIN;
    lora3a22_local.aux_group   = LORA3A22_LOCAL_GPIO_AUX_GPOUP;
    lora3a22_local.aux_pin     = LORA3A22_LOCAL_GPIO_AUX_PIN;
    lora3a22_local.md0_group   = LORA3A22_LOCAL_GPIO_MD0_GPOUP;
    lora3a22_local.md0_pin     = LORA3A22_LOCAL_GPIO_MD0_PIN;
    lora3a22_local.md1_group   = LORA3A22_LOCAL_GPIO_MD1_GPOUP;
    lora3a22_local.md1_pin     = LORA3A22_LOCAL_GPIO_MD1_PIN;
    lora3a22_gpio_init(&lora3a22_local);

    lora3a22_target.uart_x      = LORA3A22_TARGET_UART_INDEX;
    lora3a22_target.uart_init   = LORA3A22_TARGET_UART_INIT_FUNC;
    lora3a22_target.tx_group    = LORA3A22_TARGET_UART_TX_GPOUP;
    lora3a22_target.tx_pin      = LORA3A22_TARGET_UART_TX_PIN;
    lora3a22_target.rx_group    = LORA3A22_TARGET_UART_RX_GPOUP;
    lora3a22_target.rx_pin      = LORA3A22_TARGET_UART_RX_PIN;
    lora3a22_target.rst_group   = LORA3A22_TARGET_GPIO_RST_GPOUP;
    lora3a22_target.rst_pin     = LORA3A22_TARGET_GPIO_RST_PIN;
    lora3a22_target.md0_group   = LORA3A22_TARGET_GPIO_MD0_GPOUP;
    lora3a22_target.md0_pin     = LORA3A22_TARGET_GPIO_MD0_PIN;
    lora3a22_target.aux_group   = LORA3A22_TARGET_GPIO_AUX_GPOUP;
    lora3a22_target.aux_pin     = LORA3A22_TARGET_GPIO_AUX_PIN;
    lora3a22_target.md1_group   = LORA3A22_TARGET_GPIO_MD1_GPOUP;
    lora3a22_target.md1_pin     = LORA3A22_TARGET_GPIO_MD1_PIN;
    lora3a22_gpio_init(&lora3a22_target);

    fifo_init(&lora3a22_uart_fifo, FIFO_DATA_8BIT, lora3a22_uart_buff, LORA3A22_UART_BUFF_LEN);

    // 低电平复位
    GPIO_WriteBit(lora3a22_local.rst_group, lora3a22_local.rst_pin, 0);
    rt_thread_mdelay(5);
    GPIO_WriteBit(lora3a22_local.rst_group, lora3a22_local.rst_pin, 1);

    // 初始化串口
    lora3a22_local.uart_init(9600);

    do
    {
        // 设置为配置模式
        if(lora3a22_set_work_mode(&lora3a22_local, CONFIG_MODE))
        {
            rt_kprintf("lora3a22_set_work_mode error\r\n");
            return_state = 1;
            break;
        }

        // 自检
        if(lora3a22_self_test(&lora3a22_local))
        {
            rt_kprintf("lora3a22_self_test error\r\n");
            return_state = 1;
            break;
        }

        // 设置为透传模式
        if(lora3a22_set_work_mode(&lora3a22_local, TRANSPARENT_MODE))
        {
            rt_kprintf("lora3a22_set_work_mode error\r\n");
            return_state = 1;
            break;
        }
//        uint8 temp_buff[10];
//        uint8 temp_len = lora3a22_uart_read_regs(&lora3a22_local, LORA3A22_LOCAL_GROUP_NUMBER, 4, temp_buff);
//        if(temp_len)
//        {
//            for (uint8 i = 0; i < temp_len; i++)
//            {
//                rt_kprintf("LORA3A22_LOCAL_GROUP_NUMBER [%d] = 0x%02X\r\n", i, temp_buff[i]);
//            }
//        }
    }
    while(0);

    lora3a22_local.uart_init(115200);

    return return_state;
}


