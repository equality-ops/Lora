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
* 文件名称          config
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

#ifndef _REMOTE_CONTROL_CONFIG_H_
#define _REMOTE_CONTROL_CONFIG_H_


#define REMOTE_MIN_BATTERY                  (3700)  // 最小电压，单位mv，当检测到电压低于电压，低电压LED常亮

#define REMOTE_SEND_INTERVAL                (50)    // 发送间隔，单位ms

#define REMOTE_IDLE_TIME                    (120)    // 多少秒没有操作，进入LORA模块进入低功耗休眠模式

#define REMOTE_POWEROFF_TIME                (300)   // 多少秒没有操作，进入关机状态

#define REMOTE_POWER_ON_CALIBRATION         (1)     // 上电校准是否，0-不开启，1开启
#define REMOTE_CALIBRATION_TIME             (800)   // 校准时长，单位ms

#define REMOTE_KEY_CALIBRATION_PAIR         (1000)  // 按键检测，单位ms

#define REMOTE_JOYSTICK_THRESHOLD           (50)    // 遥杆ADC阈值。低于这个值强制设置为0.


#endif
