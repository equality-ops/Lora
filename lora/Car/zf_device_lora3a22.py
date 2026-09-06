# -*- coding: utf-8 -*-
"""
zf_device_lora3a22
================================================================================
本文件由以下两个 C 语言源文件转换而来（SEEKFREE 逐飞科技 LORA3A22 遥控器接收端示例）：

    zf_device_lora3a22.h
    zf_device_lora3a22.c

转换规则：
    1. 原代码中通过 #include "zf_common_headfile.h" 引入的所有外部类型、
       宏与函数（uint8 / vuint8 / uint16 / int16 / UART_2 / UART2_TX_P10_5 /
       UART2_RX_P10_6 / LORA3A22_UART / uart_init / uart_read_byte /
       uart_rx_interrupt / set_wireless_type / memcpy 等）均已在本文件内部
       自行定义或实现，不依赖任何外部头文件、外部模块或第三方库。
    2. 本文件不进行任何 import，是一个完全封闭（自包含）的 Python 模块，
       同时兼容标准 Python 与 MicroPython 环境。
    3. 原 C 代码由硬件串口接收中断逐字节调用 lora3a22_uart_callback()；
       Python 中没有硬件中断，因此新增了公开接口 lora3a22_uart_rx_push()
       来模拟“串口收到一个字节”，每送入一个字节即触发一次回调处理，
       其余逻辑与原 C 代码保持一致。

原文件版权信息（SEEKFREE 逐飞科技，GPL3.0）：
    Copyright (c) 2022 SEEKFREE 逐飞科技
    此源码使用 GPL3.0 开源许可协议
    https://www.gnu.org/licenses/
    公司网站：https://seekfree.taobao.com/
    修改记录：2024-03-29  JKS  first version
================================================================================
"""

LORA3A22_DATA_LEN   = 18      # lora3a22 帧长
LORA3A22_FRAME_STAR = 0xA3    # 帧头信息

lora3a22_finsh_flag = 0             # 指示成功接收到一帧遥控器数据
lora3a22_state_flag = 1             # 遥控器状态(1表示连接正常，0表示失败)

class Lora3a22UartData:
    """遥控器数据接收结构体（与原 C 结构体字段一一对应）"""
    def __init__(self):
        self.head = 0                   # uint8 帧头
        self.sum_check = 0              # uint8 和校验
        self.joystick = [0, 0, 0, 0]    # int16[4] 摇杆值
        # joystick[0]: 左摇杆上下值
        # joystick[1]: 左摇杆左右值
        # joystick[2]: 右摇杆上下值
        # joystick[3]: 右摇杆左右值
        self.key = [0, 0, 0, 0]         # uint8[4] 按键值
        # 按下为1 松开为0
        # key[0]-摇杆左
        # key[1]-摇杆右
        # key[2]-左侧按键左边
        # key[3]-左侧按键右边
        self.switch_key = [0, 0, 0, 0]  # uint8[4] 拨码开关值
        # switch_key[0]-左边拨码开关_1
        # switch_key[1]-左边拨码开关_2
        # switch_key[2]-右边拨码开关_1
        # switch_key[3]-右边拨码开关_2

class LoraControl:
    def __init__(self, LoraUart):
        self.uart = LoraUart
        self.lora3a22_uart_transfer = Lora3a22UartData()
        self.lora3a22_uart_data = [0] * LORA3A22_DATA_LEN    # 遥控器数据的原始数据
        self.lora3a22_response_time = 0 

        self._uart_rx_buffer = []          # 模拟串口接收缓冲区（模拟硬件接收 FIFO）
        self._lora3a22_rx_callback = None  # set_wireless_type 注册的接收回调函数

        self._lora3a22_callback_length = 0


    def _uart_read_byte(self):
        """原外部函数 uart_read_byte 的封闭实现：从模拟接收缓冲区读取一个字节。"""
        if self._uart_rx_buffer:
            return self._uart_rx_buffer.pop(0)
        return 0    # 缓冲区为空时返回 0（原 C 中由中断保证读到有效数据）


    def lora3a22_uart_rx_push(self, byte):
        """
        模拟“串口收到一个字节”的公开接口（替代原 C 中的串口接收中断）：
        """
        self._uart_rx_buffer.append(byte & 0xFF)


    def lora3a22_uart_rx_push_bytes(self, data):
        """
        批量送入接口（新增的便利接口，原 C 中无此函数）：
        将一串字节依次送入接收处理流程，等价于逐字节调用 lora3a22_uart_rx_push()。
        """
        for byte in data:
            self.lora3a22_uart_rx_push(byte)


    def lora3a22_uart_callback(self):
        """
        函数名称     lora3a22串口回调函数
        函数说明     void
        返回参数     void
        使用示例     lora3a22_uart_callback()
        备注信息     此函数需要在串口接收中断内进行调用
                    （Python 版本中由 lora3a22_uart_rx_push() 内部自动调用）
        """
        global _lora3a22_callback_length
        global lora3a22_finsh_flag, lora3a22_state_flag, lora3a22_response_time

        parity_bit_sum = 0
        parity_bit = 0

        self.lora3a22_uart_data[self._lora3a22_callback_length] = self._uart_read_byte()
        self._lora3a22_callback_length += 1

        if (1 == self._lora3a22_callback_length) and (LORA3A22_FRAME_STAR != self.lora3a22_uart_data[0]):
            _lora3a22_callback_length = 0
        # 起始位判断

        if LORA3A22_DATA_LEN <= _lora3a22_callback_length:      # 数据长度判断
            parity_bit = self.lora3a22_uart_data[1]
            self.lora3a22_uart_data[1] = 0
            for i in range(LORA3A22_DATA_LEN):
                parity_bit_sum += self.lora3a22_uart_data[i]
                parity_bit_sum &= 0xFF    # 等价于原 C 中 uint8 类型的自动截断

            if parity_bit_sum == parity_bit:                    # 和校验判断
                lora3a22_finsh_flag = 1
                lora3a22_state_flag = 1
                lora3a22_response_time = 0
                self.lora3a22_uart_data[1] = parity_bit

                # 将接收到的数据拷贝到结构体中（原 C 的 memcpy 等价实现）
                self._lora3a22_memcpy_to_transfer()
            else:
                lora3a22_finsh_flag = 0

            parity_bit_sum = 0
            _lora3a22_callback_length = 0


    def _lora3a22_memcpy_to_transfer(self):
        """
        原 C 中 memcpy((uint8*)&lora3a22_uart_transfer, (uint8*)lora3a22_uart_data,
                    sizeof(lora3a22_uart_data)) 的封闭等价实现。
        按原结构体内存布局逐字段解析（小端序，与原硬件平台一致）：

            偏移 0      head         uint8
            偏移 1      sum_check    uint8
            偏移 2~9    joystick[4]  int16 x4（小端）
            偏移 10~13  key[4]       uint8 x4
            偏移 14~17  switch_key[4] uint8 x4
        """
        data = self.lora3a22_uart_data
        transfer = self.lora3a22_uart_transfer

        transfer.head = data[0]
        transfer.sum_check = data[1]

        for i in range(4):
            low = data[2 + i * 2]
            high = data[3 + i * 2]
            value = low | (high << 8)
            if value >= 0x8000:        # 还原 int16 符号位
                value -= 0x10000
            transfer.joystick[i] = value

        for i in range(4):
            transfer.key[i] = data[10 + i]

        for i in range(4):
            transfer.switch_key[i] = data[14 + i]

