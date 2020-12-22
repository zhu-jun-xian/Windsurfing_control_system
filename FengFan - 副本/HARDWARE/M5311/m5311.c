#include "M5311.h"
#include <string.h>
#include "stdio.h"
#include "usart.h"
#include "led.h"
#include "wdg.h"
unsigned char m5311_buf[128];
char *strx, *extstrx;
char  RxBuffer[100], RxCounter;
char PUB_BUF[256];//�ϴ����ݵ�buf
unsigned short m5311_cnt = 0, m5311_cntPre = 0;
//��Ҫ�û��ĵ�
#define ProductID  "iot"  //��ƷID
#define DeviceID  "mercurio"  //�豸ID
#define APIKEY  "123456789" //APIKEY
#define Submit "topic/iot"//Submit


//==========================================================
//	�������ƣ�	m5311_WaitRecive
//
//	�������ܣ�	�ȴ��������
//
//	��ڲ�����	��
//
//	���ز�����	REV_OK-�������		REV_WAIT-���ճ�ʱδ���
//
//	˵����		ѭ�����ü���Ƿ�������
//==========================================================
_Bool m5311_WaitRecive(void)
{

    if(m5311_cnt == 0) 							//������ռ���Ϊ0 ��˵��û�д��ڽ��������У�����ֱ����������������
        return REV_WAIT;

    if(m5311_cnt == m5311_cntPre)				//�����һ�ε�ֵ�������ͬ����˵���������
    {
        m5311_cnt = 0;							//��0���ռ���

        return REV_OK;								//���ؽ�����ɱ�־
    }

    m5311_cntPre = m5311_cnt;					//��Ϊ��ͬ

    return REV_WAIT;								//���ؽ���δ��ɱ�־

}
//==========================================================
//	�������ƣ�	m5311_Clear
//
//	�������ܣ�	��ջ���
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����
//==========================================================
void m5311_Clear(void)
{

    memset(m5311_buf, 0, sizeof(m5311_buf));
    m5311_cnt = 0;

}

//==========================================================
//	�������ƣ�	m5311_SendCmd
//
//	�������ܣ�	��������
//
//	��ڲ�����	cmd������
//				res����Ҫ���ķ���ָ��
//
//	���ز�����	0-�ɹ�	1-ʧ��
//
//	˵����
//==========================================================
_Bool m5311_SendCmd(char *cmd, char *res)
{

    unsigned char timeOut = 200;

    Usart_SendString(USART3, (unsigned char *)cmd, strlen((const char *)cmd));

    while(timeOut--)
    {
        if(m5311_WaitRecive() == REV_OK)							//����յ�����
        {
            if(strstr((const char *)m5311_buf, res) != NULL)		//����������ؼ���
            {
                m5311_Clear();								//��ջ���

                return 0;
            }
        }

        delay_ms(10);
    }

    return 1;

}
void Clear_Buffer(void)//��ջ���
{
    u8 i;
    Usart3_SendStr(RxBuffer);

    for(i = 0; i < 100; i++)
        RxBuffer[i] = 0; //����

    RxCounter = 0;
    IWDG_Feed();//ι��
}
void M5311_Init(void)
{
    UsartPrintf(USART_DEBUG, " M5311_Init start\r\n");
    m5311_SendCmd("AT\r\n", "");
//   while(m5311_SendCmd("AT\r\n", "OK")) {
//		UsartPrintf(USART_DEBUG, " while\r\n");
//	 }
    UsartPrintf(USART_DEBUG, "0.AT O\r\n");
    delay_ms(300);
    strx = strstr((const char*)RxBuffer, (const char*)"OK"); //����OK
    Clear_Buffer();
    UsartPrintf(USART_DEBUG, "%s K\r\n",strx);

    while(strx == NULL)
    {
        UsartPrintf(USART_DEBUG, " while\r\n");
        Clear_Buffer();
        m5311_SendCmd("AT\r\n", "");
        delay_ms(300);
        strx = strstr((const char*)RxBuffer, (const char*)"OK"); //����OK
    }

    UsartPrintf(USART_DEBUG, "0.AT OK\r\n");
//ע����¼
    m5311_SendCmd("AT+MQTTDISC\r\n", "");
    UsartPrintf(USART_DEBUG, "1.ע����¼\r\n");
    delay_ms(300);
    //ɾ��ͨѶ
    m5311_SendCmd("AT+MQTTDEL\r\n", "");
    delay_ms(300);
    m5311_SendCmd("AT+CFUN=1\r\n", "");
    delay_ms(300);
//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
    m5311_SendCmd("AT+CIMI\r\n", "");
    delay_ms(300);
    strx = strstr((const char*)RxBuffer, (const char*)"460"); //��460������ʶ�𵽿���
    Clear_Buffer();

    while(strx == NULL)
    {
        Clear_Buffer();
        //��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
        m5311_SendCmd("AT+CIMI\r\n", "");
        delay_ms(300);
        strx = strstr((const char*)RxBuffer, (const char*)"460"); //����OK,˵�����Ǵ��ڵ�
    }

    //�������磬PDP
    m5311_SendCmd("AT+CGATT=1\r\n", "");
    delay_ms(300);
    strx = strstr((const char*)RxBuffer, (const char*)"OK"); //��OK
    Clear_Buffer();
    //��ѯ����״̬
    m5311_SendCmd("AT+CGATT?\r\n", "");
    delay_ms(300);
    strx = strstr((const char*)RxBuffer, (const char*)"+CGATT: 1"); //��1
    Clear_Buffer();

    while(strx == NULL)
    {
        Clear_Buffer();
        //��ȡ����״̬
        m5311_SendCmd("AT+CGATT?\r\n", "");
        delay_ms(300);
        strx = strstr((const char*)RxBuffer, (const char*)"+CGATT: 1"); //����1,����ע���ɹ�
    }

    //��ѯ�ź�ֵ
    m5311_SendCmd("AT+CSQ\r\n", "");
    delay_ms(300);
    Clear_Buffer();
    UsartPrintf(USART_DEBUG, " M5311_Init end\r\n");
}






//��ʼ��MQTT
void M5311_MQTTInit(void)
{
    UsartPrintf(USART_DEBUG, " M5311_MQTTInit start\r\n");

    sprintf(PUB_BUF, "AT+MQTTCFG=\"47.100.95.148\",1883,\"%s\",20,\"%s\",\"%s\",1\r\n", DeviceID, ProductID, APIKEY); //���õ�¼����
    m5311_SendCmd(PUB_BUF, "");
    delay_ms(300);
    Clear_Buffer();
    sprintf(PUB_BUF, "AT+MQTTOPEN=1,1,0,0,0,\"\",\"\"\r\n"); //���ӵ�ONENETƽ̨
    m5311_SendCmd(PUB_BUF, "");
    delay_ms(300);
    strx = strstr((const char*)RxBuffer, (const char*)"+MQTTOPEN: OK"); //����¼�ɹ�

    while(strx == NULL)
    {
        strx = strstr((const char*)RxBuffer, (const char*)"+MQTTOPEN: OK"); //��¼��ONENET�ɹ� ��ʱ�豸����ʾ����

    }

    Clear_Buffer();
    UsartPrintf(USART_DEBUG, " M5311_MQTTInit end\r\n");
    UsartPrintf(USART_DEBUG, " ��¼��ONENET�ɹ� ��ʱ�豸����ʾ����\r\n");
}



//��������
void M5311_SUB(void)
{
    sprintf(PUB_BUF, "AT+MQTTPUB=\"%s\",2\r\n", Submit); //��������
    delay_ms(300);
    strx = strstr((const char*)RxBuffer, (const char*)"OK");

    while(strx == NULL)
    {
        strx = strstr((const char*)RxBuffer, (const char*)"OK"); //���ĳɹ�
    }

    Clear_Buffer();
}


//�������ݵ�������
void M5311_MQTTPUBMessage(char*len, char*data)
{
    sprintf(PUB_BUF, "AT+MQTTPUB=$dp,2,1,0,%s,%s\r\n", len, data); //�������ݵ�ƽ̨
    //sprintf(PUB_BUF,"AT+MQTTPUB=$dp,2,1,0,16,03000d7b226d7a68223a22303031227d\r\n",len,data);//�������ݵ�ƽ̨
    delay_ms(300);
    strx = strstr((const char*)RxBuffer, (const char*)"+MQTTPUBCOMP:"); //���ط����ɹ�

    while(strx == NULL)
    {
        strx = strstr((const char*)RxBuffer, (const char*)"+MQTTPUBCOMP:"); //�������ݳɹ�
    }

    Clear_Buffer();

}



void StringToHex(char *str, unsigned char *strhex)
{
    uint8_t i, cnt = 0;
    char *p = str;             //ֱ��p��ʼ��Ϊָ��str
    uint8_t len = strlen(str); //��ȡ�ַ����е��ַ�����

    while(*p != '\0')          //�������ж�
    {
        for (i = 0; i < len; i ++)  //ѭ���жϵ�ǰ�ַ������ֻ���Сд�ַ����Ǵ�д��ĸ
        {
            if ((*p >= '0') && (*p <= '9')) //��ǰ�ַ�Ϊ����0~9ʱ
                strhex[cnt] = *p - '0' + 0x30;//תΪʮ������

            if ((*p >= 'A') && (*p <= 'Z')) //��ǰ�ַ�Ϊ��д��ĸA~Zʱ
                strhex[cnt] = *p - 'A' + 0x41;//תΪʮ������

            if ((*p >= 'a') && (*p <= 'z')) //��ǰ�ַ�ΪСд��ĸa~zʱ
                strhex[cnt] = *p - 'a' + 0x61;  //תΪʮ������

            if(*p == 0x22) //˫����
                strhex[cnt] = 0x22;

            if(*p == 0x7b) //{������
                strhex[cnt] = 0x7b; //{������

            if(*p == 0x7d) //{������
                strhex[cnt] = 0x7d; //}������

            if(*p == 0x3a) //ð��
                strhex[cnt] = 0x3a; //:ð��

            p ++;    //ָ����һ���ַ�
            cnt ++;
        }
    }
}


//ʮ������ת���ַ���ʮ������
void HexToHexString(u8 *p, u8 *buffer)
{
    u8 i, j = 0;

    for(i = 0; i < strlen((char*)p); i++)
    {
        buffer[j * 2] = p[i] / 16 + 0x30; //ת��0-9

        if(p[i] % 16 >= 10)
            buffer[j * 2 + 1] = p[i] % 16 + 0x37; //ת��A-F
        else
            buffer[j * 2 + 1] = p[i] % 16 + 0x30; //ת��0-9

        j++;
    }

    buffer[j * 2] = 0;
}
