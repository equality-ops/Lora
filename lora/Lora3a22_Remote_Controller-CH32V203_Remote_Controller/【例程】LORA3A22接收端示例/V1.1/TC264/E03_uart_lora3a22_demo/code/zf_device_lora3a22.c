/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC264 ��Դ���һ����
*
* TC264 ��Դ�� ���������
* �����Ը���������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù�������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ����֤Э�� ������������Ϊ���İ汾
* ��������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ����֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          zf_device_lora3a22
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.9.4
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2024-03-29       JKS            first version
********************************************************************************************************************/

#include "zf_device_lora3a22.h"

uint8   lora3a22_uart_data[LORA3A22_DATA_LEN]  = {0};               // ң����������ԭʼ����

vuint8  lora3a22_finsh_flag = 0;                                    // ��ʾ�ɹ����յ�һ֡ң��������
vuint8  lora3a22_state_flag = 1;                                    // ң����״̬(1��ʾ�����������ʾʧ��)
uint16  lora3a22_response_time = 0;

lora3a22_uart_transfer_dat_struct lora3a22_uart_transfer;

//--------------------------------  -----------------------------------------------------------------------------------
// �������     lora3a22���ڻص�����
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     lora3a22_uart_callback();
// ��ע��Ϣ     �˺�����Ҫ�ڴ��ڽ����ж��ڽ��е���
//-------------------------------------------------------------------------------------------------------------------

void lora3a22_uart_callback(void)
{
    static uint8 length = 0 ;
    uint8  parity_bit_sum  = 0, parity_bit  = 0;

    lora3a22_uart_data[length++] = uart_read_byte(LORA3A22_UART_INDEX);

    if((1 == length) && (LORA3A22_FRAME_STAR != lora3a22_uart_data[0]))
    {
        length =  0;
    }                                                             // ��ʼλ�ж�

    if(LORA3A22_DATA_LEN <= length)                            	  // ���ݳ����ж�
    {
        parity_bit = lora3a22_uart_data[1];
        lora3a22_uart_data[1] = 0;
        for(int  i = 0; i < LORA3A22_DATA_LEN; i ++)
        {
            parity_bit_sum += lora3a22_uart_data[i];
        }

        if (parity_bit_sum == parity_bit)                          // ��У���ж�
        {
            lora3a22_finsh_flag = 1;
            lora3a22_state_flag = 1;
            lora3a22_response_time = 0;
            lora3a22_uart_data[1]= parity_bit;

            // �����յ������ݿ������ṹ����
            memcpy((uint8*)&lora3a22_uart_transfer, (uint8*)lora3a22_uart_data, \
            sizeof(lora3a22_uart_data));

        }
        else
        {
            lora3a22_finsh_flag = 0;
        }
        parity_bit_sum = 0;
        length = 0;
    }

}

//-------------------------------------------------------------------------------------------------------------------
// �������     lora3a22��ʼ������
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     lora3a22_init();
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------

void lora3a22_init(void)
{
    uart_init(LORA3A22_UART_INDEX, LORA3A22_UART_BAUDRATE, LORA3A22_UART_TX_PIN, LORA3A22_UART_RX_PIN);

    uart_rx_interrupt(LORA3A22_UART_INDEX, 1);
    // ���ô����жϻص�����
    set_wireless_type(LORA3A22_UART, lora3a22_uart_callback);
}


