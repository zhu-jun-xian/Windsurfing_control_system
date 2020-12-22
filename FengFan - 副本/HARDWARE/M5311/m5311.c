#include "M5311.h"
#include <string.h>
#include "stdio.h"
#include "usart.h"
#include "led.h"
#include "wdg.h"
unsigned char m5311_buf[128];
char *strx, *extstrx;
char  RxBuffer[100], RxCounter;
char PUB_BUF[256];//上传数据的buf
unsigned short m5311_cnt = 0, m5311_cntPre = 0;
//需要用户改的
#define ProductID  "iot"  //产品ID
#define DeviceID  "mercurio"  //设备ID
#define APIKEY  "123456789" //APIKEY
#define Submit "topic/iot"//Submit


//==========================================================
//	函数名称：	m5311_WaitRecive
//
//	函数功能：	等待接收完成
//
//	入口参数：	无
//
//	返回参数：	REV_OK-接收完成		REV_WAIT-接收超时未完成
//
//	说明：		循环调用检测是否接收完成
//==========================================================
_Bool m5311_WaitRecive(void)
{

    if(m5311_cnt == 0) 							//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
        return REV_WAIT;

    if(m5311_cnt == m5311_cntPre)				//如果上一次的值和这次相同，则说明接收完毕
    {
        m5311_cnt = 0;							//清0接收计数

        return REV_OK;								//返回接收完成标志
    }

    m5311_cntPre = m5311_cnt;					//置为相同

    return REV_WAIT;								//返回接收未完成标志

}
//==========================================================
//	函数名称：	m5311_Clear
//
//	函数功能：	清空缓存
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：
//==========================================================
void m5311_Clear(void)
{

    memset(m5311_buf, 0, sizeof(m5311_buf));
    m5311_cnt = 0;

}

//==========================================================
//	函数名称：	m5311_SendCmd
//
//	函数功能：	发送命令
//
//	入口参数：	cmd：命令
//				res：需要检查的返回指令
//
//	返回参数：	0-成功	1-失败
//
//	说明：
//==========================================================
_Bool m5311_SendCmd(char *cmd, char *res)
{

    unsigned char timeOut = 200;

    Usart_SendString(USART3, (unsigned char *)cmd, strlen((const char *)cmd));

    while(timeOut--)
    {
        if(m5311_WaitRecive() == REV_OK)							//如果收到数据
        {
            if(strstr((const char *)m5311_buf, res) != NULL)		//如果检索到关键词
            {
                m5311_Clear();								//清空缓存

                return 0;
            }
        }

        delay_ms(10);
    }

    return 1;

}
void Clear_Buffer(void)//清空缓存
{
    u8 i;
    Usart3_SendStr(RxBuffer);

    for(i = 0; i < 100; i++)
        RxBuffer[i] = 0; //缓存

    RxCounter = 0;
    IWDG_Feed();//喂狗
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
    strx = strstr((const char*)RxBuffer, (const char*)"OK"); //返回OK
    Clear_Buffer();
    UsartPrintf(USART_DEBUG, "%s K\r\n",strx);

    while(strx == NULL)
    {
        UsartPrintf(USART_DEBUG, " while\r\n");
        Clear_Buffer();
        m5311_SendCmd("AT\r\n", "");
        delay_ms(300);
        strx = strstr((const char*)RxBuffer, (const char*)"OK"); //返回OK
    }

    UsartPrintf(USART_DEBUG, "0.AT OK\r\n");
//注销登录
    m5311_SendCmd("AT+MQTTDISC\r\n", "");
    UsartPrintf(USART_DEBUG, "1.注销登录\r\n");
    delay_ms(300);
    //删除通讯
    m5311_SendCmd("AT+MQTTDEL\r\n", "");
    delay_ms(300);
    m5311_SendCmd("AT+CFUN=1\r\n", "");
    delay_ms(300);
//获取卡号，类似是否存在卡的意思，比较重要。
    m5311_SendCmd("AT+CIMI\r\n", "");
    delay_ms(300);
    strx = strstr((const char*)RxBuffer, (const char*)"460"); //返460，表明识别到卡了
    Clear_Buffer();

    while(strx == NULL)
    {
        Clear_Buffer();
        //获取卡号，类似是否存在卡的意思，比较重要。
        m5311_SendCmd("AT+CIMI\r\n", "");
        delay_ms(300);
        strx = strstr((const char*)RxBuffer, (const char*)"460"); //返回OK,说明卡是存在的
    }

    //激活网络，PDP
    m5311_SendCmd("AT+CGATT=1\r\n", "");
    delay_ms(300);
    strx = strstr((const char*)RxBuffer, (const char*)"OK"); //返OK
    Clear_Buffer();
    //查询激活状态
    m5311_SendCmd("AT+CGATT?\r\n", "");
    delay_ms(300);
    strx = strstr((const char*)RxBuffer, (const char*)"+CGATT: 1"); //返1
    Clear_Buffer();

    while(strx == NULL)
    {
        Clear_Buffer();
        //获取激活状态
        m5311_SendCmd("AT+CGATT?\r\n", "");
        delay_ms(300);
        strx = strstr((const char*)RxBuffer, (const char*)"+CGATT: 1"); //返回1,表明注网成功
    }

    //查询信号值
    m5311_SendCmd("AT+CSQ\r\n", "");
    delay_ms(300);
    Clear_Buffer();
    UsartPrintf(USART_DEBUG, " M5311_Init end\r\n");
}






//初始化MQTT
void M5311_MQTTInit(void)
{
    UsartPrintf(USART_DEBUG, " M5311_MQTTInit start\r\n");

    sprintf(PUB_BUF, "AT+MQTTCFG=\"47.100.95.148\",1883,\"%s\",20,\"%s\",\"%s\",1\r\n", DeviceID, ProductID, APIKEY); //设置登录参数
    m5311_SendCmd(PUB_BUF, "");
    delay_ms(300);
    Clear_Buffer();
    sprintf(PUB_BUF, "AT+MQTTOPEN=1,1,0,0,0,\"\",\"\"\r\n"); //连接到ONENET平台
    m5311_SendCmd(PUB_BUF, "");
    delay_ms(300);
    strx = strstr((const char*)RxBuffer, (const char*)"+MQTTOPEN: OK"); //返登录成功

    while(strx == NULL)
    {
        strx = strstr((const char*)RxBuffer, (const char*)"+MQTTOPEN: OK"); //登录到ONENET成功 此时设备会显示在线

    }

    Clear_Buffer();
    UsartPrintf(USART_DEBUG, " M5311_MQTTInit end\r\n");
    UsartPrintf(USART_DEBUG, " 登录到ONENET成功 此时设备会显示在线\r\n");
}



//订阅主题
void M5311_SUB(void)
{
    sprintf(PUB_BUF, "AT+MQTTPUB=\"%s\",2\r\n", Submit); //订阅主题
    delay_ms(300);
    strx = strstr((const char*)RxBuffer, (const char*)"OK");

    while(strx == NULL)
    {
        strx = strstr((const char*)RxBuffer, (const char*)"OK"); //订阅成功
    }

    Clear_Buffer();
}


//发送数据到服务器
void M5311_MQTTPUBMessage(char*len, char*data)
{
    sprintf(PUB_BUF, "AT+MQTTPUB=$dp,2,1,0,%s,%s\r\n", len, data); //发布数据到平台
    //sprintf(PUB_BUF,"AT+MQTTPUB=$dp,2,1,0,16,03000d7b226d7a68223a22303031227d\r\n",len,data);//发布数据到平台
    delay_ms(300);
    strx = strstr((const char*)RxBuffer, (const char*)"+MQTTPUBCOMP:"); //返回发布成功

    while(strx == NULL)
    {
        strx = strstr((const char*)RxBuffer, (const char*)"+MQTTPUBCOMP:"); //发布数据成功
    }

    Clear_Buffer();

}



void StringToHex(char *str, unsigned char *strhex)
{
    uint8_t i, cnt = 0;
    char *p = str;             //直针p初始化为指向str
    uint8_t len = strlen(str); //获取字符串中的字符个数

    while(*p != '\0')          //结束符判断
    {
        for (i = 0; i < len; i ++)  //循环判断当前字符是数字还是小写字符还是大写字母
        {
            if ((*p >= '0') && (*p <= '9')) //当前字符为数字0~9时
                strhex[cnt] = *p - '0' + 0x30;//转为十六进制

            if ((*p >= 'A') && (*p <= 'Z')) //当前字符为大写字母A~Z时
                strhex[cnt] = *p - 'A' + 0x41;//转为十六进制

            if ((*p >= 'a') && (*p <= 'z')) //当前字符为小写字母a~z时
                strhex[cnt] = *p - 'a' + 0x61;  //转为十六进制

            if(*p == 0x22) //双引号
                strhex[cnt] = 0x22;

            if(*p == 0x7b) //{左括号
                strhex[cnt] = 0x7b; //{左括号

            if(*p == 0x7d) //{右括号
                strhex[cnt] = 0x7d; //}右括号

            if(*p == 0x3a) //冒号
                strhex[cnt] = 0x3a; //:冒号

            p ++;    //指向下一个字符
            cnt ++;
        }
    }
}


//十六进制转成字符串十六进制
void HexToHexString(u8 *p, u8 *buffer)
{
    u8 i, j = 0;

    for(i = 0; i < strlen((char*)p); i++)
    {
        buffer[j * 2] = p[i] / 16 + 0x30; //转成0-9

        if(p[i] % 16 >= 10)
            buffer[j * 2 + 1] = p[i] % 16 + 0x37; //转成A-F
        else
            buffer[j * 2 + 1] = p[i] % 16 + 0x30; //转成0-9

        j++;
    }

    buffer[j * 2] = 0;
}
