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

# ============================================================================
# 一、原 zf_device_lora3a22.h 中通过外部头文件引入的内容，此处自行定义
#    （封闭实现：不依赖 zf_common_headfile.h）
# ============================================================================

# 原 C 类型映射说明：
#   uint8 / vuint8  -> Python int（0 ~ 255）
#   uint16          -> Python int（0 ~ 65535）
#   int16           -> Python int（-32768 ~ 32767）
# 需要按字节范围取值时用 & 0xFF / & 0xFFFF 掩码，与原 C 类型行为一致。

UART_2 = 2                # 原外部枚举值：UART 2 的索引号，此处自行定义
UART2_TX_P10_5 = "P10_5"  # 原外部引脚宏：UART2 的 TX 引脚（P10.5），此处自行定义
UART2_RX_P10_6 = "P10_6"  # 原外部引脚宏：UART2 的 RX 引脚（P10.6），此处自行定义
LORA3A22_UART = 0         # 原外部枚举值：无线模块类型标识（set_wireless_type 用），此处自行定义

# ============================================================================
# 二、原 zf_device_lora3a22.h 中的宏定义
# ============================================================================

LORA3A22_UART_INDEX    = UART_2            # 定义串口遥控器使用的串口
LORA3A22_UART_TX_PIN   = UART2_TX_P10_5    # 遥控器接收机的RX引脚 连接到单片机的TX引脚
LORA3A22_UART_RX_PIN   = UART2_RX_P10_6    # 遥控器接收机的TX引脚 连接到单片机的RX引脚
LORA3A22_UART_BAUDRATE = 115200            # 指定 lora3a22 遥控器使用的串口波特率

LORA3A22_DATA_LEN   = 18      # lora3a22 帧长
LORA3A22_FRAME_STAR = 0xA3    # 帧头信息


# ============================================================================
# 三、原 zf_device_lora3a22.h 中的结构体定义
#     typedef struct { ... } lora3a22_uart_transfer_dat_struct;
# ============================================================================

class Lora3a22UartTransferDatStruct:
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


# ============================================================================
# 四、原 zf_device_lora3a22.c 中的全局变量
# ============================================================================

lora3a22_uart_data = [0] * LORA3A22_DATA_LEN    # 遥控器数据的原始数据
lora3a22_finsh_flag = 0                          # 指示成功接收到一帧遥控器数据
lora3a22_state_flag = 1                          # 遥控器状态(1表示连接正常，0表示失败)
lora3a22_response_time = 0
lora3a22_uart_transfer = Lora3a22UartTransferDatStruct()


# ============================================================================
# 五、原 C 代码调用的外部函数，此处自行实现（封闭实现，模拟硬件串口）
# ============================================================================

_uart_rx_buffer = []          # 模拟串口接收缓冲区（模拟硬件接收 FIFO）
_uart_rx_interrupt_enable = 0 # 模拟串口接收中断使能状态
_lora3a22_rx_callback = None  # set_wireless_type 注册的接收回调函数
_uart_config = {}             # 串口配置（模拟硬件寄存器）


def _uart_init(index, baudrate, tx_pin, rx_pin):
    """原外部函数 uart_init 的封闭实现：初始化串口（Python 中仅记录配置）。"""
    global _uart_rx_buffer, _uart_config
    _uart_rx_buffer = []
    # 保存串口配置，便于外部查询（原 C 中由硬件直接生效）
    _uart_config = {"index": index, "baudrate": baudrate,
                    "tx_pin": tx_pin, "rx_pin": rx_pin}


def _uart_rx_interrupt(index, enable):
    """原外部函数 uart_rx_interrupt 的封闭实现：使能/关闭串口接收中断（模拟）。"""
    global _uart_rx_interrupt_enable
    _uart_rx_interrupt_enable = enable


def _set_wireless_type(wireless_type, callback):
    """原外部函数 set_wireless_type 的封闭实现：注册无线模块接收回调函数。"""
    global _lora3a22_rx_callback
    _lora3a22_rx_callback = callback


def _uart_read_byte(index):
    """原外部函数 uart_read_byte 的封闭实现：从模拟接收缓冲区读取一个字节。"""
    if _uart_rx_buffer:
        return _uart_rx_buffer.pop(0)
    return 0    # 缓冲区为空时返回 0（原 C 中由中断保证读到有效数据）


def lora3a22_uart_rx_push(byte):
    """
    模拟“串口收到一个字节”的公开接口（替代原 C 中的串口接收中断）：
    将收到的字节放入接收缓冲区，并触发一次 lora3a22_uart_callback() 处理。
    外部每收到一个字节调用一次本函数即可。
    """
    _uart_rx_buffer.append(byte & 0xFF)
    if _lora3a22_rx_callback is not None:
        _lora3a22_rx_callback()
    else:
        lora3a22_uart_callback()


def lora3a22_uart_rx_push_bytes(data):
    """
    批量送入接口（新增的便利接口，原 C 中无此函数）：
    将一串字节依次送入接收处理流程，等价于逐字节调用 lora3a22_uart_rx_push()。
    """
    for byte in data:
        lora3a22_uart_rx_push(byte)


# ============================================================================
# 六、原 zf_device_lora3a22.c 中的函数实现
# ============================================================================

# 原 C 中 static uint8 length，用于跨调用保持接收计数（Python 中放模块级实现）
_lora3a22_callback_length = 0


def lora3a22_uart_callback():
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

    lora3a22_uart_data[_lora3a22_callback_length] = _uart_read_byte(LORA3A22_UART_INDEX)
    _lora3a22_callback_length += 1

    if (1 == _lora3a22_callback_length) and (LORA3A22_FRAME_STAR != lora3a22_uart_data[0]):
        _lora3a22_callback_length = 0
    # 起始位判断

    if LORA3A22_DATA_LEN <= _lora3a22_callback_length:      # 数据长度判断
        parity_bit = lora3a22_uart_data[1]
        lora3a22_uart_data[1] = 0
        for i in range(LORA3A22_DATA_LEN):
            parity_bit_sum += lora3a22_uart_data[i]
            parity_bit_sum &= 0xFF    # 等价于原 C 中 uint8 类型的自动截断

        if parity_bit_sum == parity_bit:                    # 和校验判断
            lora3a22_finsh_flag = 1
            lora3a22_state_flag = 1
            lora3a22_response_time = 0
            lora3a22_uart_data[1] = parity_bit

            # 将接收到的数据拷贝到结构体中（原 C 的 memcpy 等价实现）
            _lora3a22_memcpy_to_transfer()
        else:
            lora3a22_finsh_flag = 0

        parity_bit_sum = 0
        _lora3a22_callback_length = 0


def _lora3a22_memcpy_to_transfer():
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
    data = lora3a22_uart_data
    transfer = lora3a22_uart_transfer

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


def lora3a22_init():
    """
    函数名称     lora3a22初始化函数
    函数说明     void
    返回参数     void
    使用示例     lora3a22_init()
    备注信息
    """
    _uart_init(LORA3A22_UART_INDEX, LORA3A22_UART_BAUDRATE,
               LORA3A22_UART_TX_PIN, LORA3A22_UART_RX_PIN)

    _uart_rx_interrupt(LORA3A22_UART_INDEX, 1)
    # 设置串口中断回调函数
    _set_wireless_type(LORA3A22_UART, lora3a22_uart_callback)
