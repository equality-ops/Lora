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
* 文件名称          main
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
#include "ch32v20x.h"
#include "rtthread.h"
#include "rthw.h"
#include "adc.h"
#include "led.h"
#include "power.h"
#include "config.h"
#include "joystick.h"
#include "lora3a22_config.h"
#include "lora3a22_control.h"
#include "battery_manage.h"

// RGB绿灯常亮                  工作模式
// RGB蓝灯常亮                  低功耗模式
// RGB红灯常亮                  校准 或者 配对
// RGB红灯闪烁                  配对失败
// RGB红灯蓝交替闪烁       遥控器自检失败
// LED低电压闪烁               电池电压过低

int main(void)
{
    // 关机检测引脚初始化
    power_init();

    // 创建关机检测线程
    power_thread_create();
    
    // LED引脚初始化
    led_init();

    // 配对和校准按键初始化
    key_pair_calibration_init();

    // 摇杆和摇杆按键初始化
    joystick_and_key_init();

    // 电池电压引脚初始化
    battery_init();

    // 初始化ADC，通过DMA去采集
    adc_dma_init();

    rt_kprintf("hello\r\n");

    // LORA模块自检以及初始化
    while(lora3a22_init())
    {
        rt_kprintf("lora3a22_init error\r\n");
        GPIO_WriteBit(RGB_BLUE_GROUP, REG_BLUE_PIN, 1);
        GPIO_WriteBit(RGB_RED_GROUP,  REG_RED_PIN,  0);
        rt_thread_mdelay(200);

        GPIO_WriteBit(RGB_BLUE_GROUP, REG_BLUE_PIN, 0);
        GPIO_WriteBit(RGB_RED_GROUP,  REG_RED_PIN,  1);
        rt_thread_mdelay(200);
    }

#if REMOTE_POWER_ON_CALIBRATION
    // 摇杆上电校准
    joystick_calibration(REMOTE_CALIBRATION_TIME);
#endif

    // 创建LED线程
	led_thread_create();

	// 创建LORA模块发送数据线程
    lora3a22_send_thread_create();

    // 创建校准按键检测线程
    calibration_thread_create();

    // 创建配对检测线程
    pair_thread_create();

    // 创建电池电压检测线程
    battery_thread_create();

    // 设置默认配对信道
    // -1为随机信道
    // 大于0的值为固定信道
    // 信道范围0至115
    set_pair_channel(-1);

    while (1)
    {
        rt_kprintf("ABC123\r\n");
        rt_thread_mdelay(1000);
    }
}





