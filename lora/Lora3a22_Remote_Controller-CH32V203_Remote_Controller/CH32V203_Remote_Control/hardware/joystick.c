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
* 文件名称          joystick
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
#include "stdio.h"
#include "ch32v20x.h"

uint16 g_adc_calibration_value[4];

//-------------------------------------------------------------------------------------------------------------------
// @brief       遥杆线程
// @param       void
// @return      ms      校准时长，单位ms
// @note
//-------------------------------------------------------------------------------------------------------------------
void joystick_calibration(uint16 ms)
{
    uint32 sum[4] = {0};

    for(uint16 i = 0; i < ms; i++)
    {
        sum[JOYSTICK_ADC_0_INDEX] += g_adc_reg_value[JOYSTICK_ADC_0_INDEX];
        sum[JOYSTICK_ADC_1_INDEX] += g_adc_reg_value[JOYSTICK_ADC_1_INDEX];
        sum[JOYSTICK_ADC_2_INDEX] += g_adc_reg_value[JOYSTICK_ADC_2_INDEX];
        sum[JOYSTICK_ADC_3_INDEX] += g_adc_reg_value[JOYSTICK_ADC_3_INDEX];

        rt_thread_mdelay(1);
    }

    g_adc_calibration_value[JOYSTICK_ADC_0_INDEX] = sum[JOYSTICK_ADC_0_INDEX] / ms;
    g_adc_calibration_value[JOYSTICK_ADC_1_INDEX] = sum[JOYSTICK_ADC_1_INDEX] / ms;
    g_adc_calibration_value[JOYSTICK_ADC_2_INDEX] = sum[JOYSTICK_ADC_2_INDEX] / ms;
    g_adc_calibration_value[JOYSTICK_ADC_3_INDEX] = sum[JOYSTICK_ADC_3_INDEX] / ms;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       摇杆和按键引脚初始化
// @param       void
// @return      void
// @note
//-------------------------------------------------------------------------------------------------------------------
void joystick_and_key_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = JOYSTICK_ADC_0_LEFT_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(JOYSTICK_ADC_0_LEFT_GROUP, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = JOYSTICK_ADC_1_LEFT_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(JOYSTICK_ADC_1_LEFT_GROUP, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = JOYSTICK_ADC_2_RIGHT_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(JOYSTICK_ADC_2_RIGHT_GROUP, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = JOYSTICK_ADC_3_RIGHT_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(JOYSTICK_ADC_3_RIGHT_GROUP, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = JOYSTICK_KEY_LEFT_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(JOYSTICK_KEY_LEFT_GROUP, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = JOYSTICK_KEY_RIGHT_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(JOYSTICK_KEY_RIGHT_GROUP, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = JOYSTICK_SWITCH_KEY_1_LEFT_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(JOYSTICK_SWITCH_KEY_1_LEFT_GROUP, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = JOYSTICK_SWITCH_KEY_2_LEFT_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(JOYSTICK_SWITCH_KEY_2_LEFT_GROUP, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = JOYSTICK_SWITCH_KEY_1_RIGHT_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(JOYSTICK_SWITCH_KEY_1_RIGHT_GROUP, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = JOYSTICK_SWITCH_KEY_2_RIGHT_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(JOYSTICK_SWITCH_KEY_2_RIGHT_GROUP, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = JOYSTICK_SIDE_KEY_LEFT_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(JOYSTICK_SIDE_KEY_LEFT_GROUP, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = JOYSTICK_SIDE_KEY_RIGHT_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(JOYSTICK_SIDE_KEY_RIGHT_GROUP, &GPIO_InitStructure);

}





