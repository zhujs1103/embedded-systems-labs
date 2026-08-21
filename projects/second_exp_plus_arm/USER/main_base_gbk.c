/*******************************************************************
实验名称：拨档开关输入实验

硬件模块：计算机原理应用实验箱

硬件接线：ARM P12接口---------LED P2接口 
			PF0~PF7--------LED1~LED8
		 ARM P11接口---------拨挡开关 P1接口  
			PC0~PC7--------SW1~SW8
         注：可用20P排线直连P12、P2接口，20P排线直连P11、P1接口。 
		   
实验现象：拨码开关状态映射到LED灯上。

更新时间：2018-10-11
********************************************************************/
#include "stm32f4xx.h"
#include "delay.h"

/*******************************
功  能：LED端口初始化
参  数：无
返回值：无
*******************************/


#define KEY_READ()       GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8)              // 上按键检测
#define KEY_WRITE()       GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11)            // 下按键检测


#define CLK(val)     GPIO_WriteBit(GPIOF, GPIO_Pin_6, (BitAction)val)          // FPGA 时钟信号
#define WREN(val)    GPIO_WriteBit(GPIOF, GPIO_Pin_7, (BitAction)val)          // FPGA 读写信号


/*******************************
功  能：LED灯读写数据显示
参  数：无
返回值：无
*******************************/

void Led_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);   //开启GPIOB的时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 \
								| GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;          //输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //速度50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_Write(GPIOB, 0x0000);      
}

/*******************************
功  能：拨码开关端口初始化
参  数：无
返回值：无
*******************************/
void Sw_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);   //开启GPIOC的时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 
								| GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7  | GPIO_Pin_8 | GPIO_Pin_11;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           //输入模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //上拉 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/*******************************
功  能：GPIOF与FPGA连接初始化
参  数：无
返回值：无
*******************************/
void GPIO_FPGA_Init()
{
  GPIO_InitTypeDef   GPIO_InitStructure;
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);   //开启GPIOF的时钟
	
	GPIO_InitStructure.GPIO_Pin = 0x0fff;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        //输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       //推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;     //输出速率25MHz 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	
  GPIO_InitStructure.GPIO_Pin = 0xf000;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;          //输出模式
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}




/************读取数据***************/
void Read_Data(uint8_t read_addr, uint8_t *read_date)
{
	uint16_t buf = 0;                                           // GPIOF 数据变量
	WREN(0);                                                    // 读数据
  CLK(0);                                                     // 时钟信号
	buf =  GPIO_ReadOutputData(GPIOF) & 0x00C0;                 // 保留第6、7位,即时钟信号和读写信号不变
	buf |= read_addr;                                           // 将读取地址放入GPIOF, 读地址 0-2位
	GPIO_Write(GPIOF, buf);                                     // GPIOF输出数据
	CLK(1);                                                     // 时钟周期发送读数据地址
  CLK(0);
	CLK(1);                                                     // 时钟信号获取内存数据
	*read_date =  ((GPIO_ReadInputData(GPIOF) & 0xf000) >> 12 );// 获得FPGA返回数据q, 数据pin在12-15位        
	WREN(0);
}

/***********写入数据***************/
void Write_Data(uint8_t write_addr, uint8_t write_date)
{
  uint16_t buf = 0;                                            // GPIOF 数据变量
	WREN(1);                                                     // 写数据
  CLK(0);                                                      // 时钟信号
	buf =  GPIO_ReadOutputData(GPIOF) & 0x00C0;                  // 保留第6、7位,即时钟信号和读写信号不变
	buf |= ((write_addr & 0x7) << 3) | (write_date << 8) ;       // 将读取地址放入GPIOF, 写地址在3-5位，写数据在8-11位
	GPIO_Write(GPIOF, buf);                                      // GPIOF输出数据
	CLK(1);
	WREN(0);
}

void Key_Scan(void)
{
	   uint8_t gpioc_data;  // GPIOC数据读取变量
	   uint8_t read_addr;   // 拨码开关 0-2	 
	   uint8_t write_addr;  // 写入地址
	   uint8_t read_date;   // 拨码开关 4-7
		 uint8_t write_date;  // 写入数据
	   if(KEY_READ() == 0)  // 读按键检测
		 {
			  Delay_Ms(5);      // 防抖
			  if(KEY_READ() == 0)
				{
				    while(KEY_READ() == 0){};
						gpioc_data = ~(GPIO_ReadInputData(GPIOC) & 0x00FF);  // 获取拨码开关数据,上0，下1
						read_addr = gpioc_data & 0x07;	                     // 获得读地址
						read_date = (gpioc_data & 0xF0) >> 4;                // 读数据，这个没用，调试用
						Read_Data(read_addr, &read_date);                    // 传入指针读取FPGA内存数据
						GPIO_Write(GPIOB, read_date);                        // 获得的数据从GPIOB发送到LED
							//LED1(!LED1_DATA());
				}
		 }
		 if(KEY_WRITE() == 0)  // 写按键检测
		 {
			  Delay_Ms(5);       // 防抖
			  if(KEY_WRITE() == 0)
				{
						
						while(KEY_WRITE() == 0){};
						gpioc_data = ~(GPIO_ReadInputData(GPIOC) & 0x00FF);  // 获取拨码开关数据,上0，下1
						write_addr = gpioc_data & 0x07;	                     // 获得写地址
						write_date = (gpioc_data & 0xF0) >> 4;               // 写数据保存
						Write_Data(write_addr, write_date);                  // 向FPGA内存写入数据
						GPIO_Write(GPIOB, write_date);	                     // 把向FPGA写入的数据显示出来
							//LED2(!LED2_DATA());
				}
		 }
 }
 
 
int main(void)
{
	Delay_Init();
	Led_Init();      //LED灯初始化
	Sw_Init();       //拨码开关初始化 按键初始化
	GPIO_FPGA_Init(); // GPIOF初始化
	while(1)
  {
    Key_Scan();
	}
}





