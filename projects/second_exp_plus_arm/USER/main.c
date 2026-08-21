#include "stm32f4xx.h"
#include "delay.h"

#define KEY_READ()   GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8)
#define KEY_WRITE()  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11)

#define CLK(val)     GPIO_WriteBit(GPIOF, GPIO_Pin_6, (BitAction)(val))
#define WREN(val)    GPIO_WriteBit(GPIOF, GPIO_Pin_7, (BitAction)(val))

void Led_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
	                              GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_Write(GPIOB, 0x0000);
}

void Sw_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
	                              GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |
	                              GPIO_Pin_8 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void GPIO_FPGA_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

	GPIO_InitStructure.GPIO_Pin = 0x0fff;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = 0xf000;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}

static void Display_Result(uint8_t mem_select, uint8_t data)
{
	/* LED1-LED4 show data. LED8 is on when M2 is selected. */
	GPIO_Write(GPIOB, (data & 0x0f) | ((mem_select & 0x01) << 7));
}

#define RESULT_ADDR 7

static void Read_Data(uint8_t mem_select, uint8_t read_addr, uint8_t *read_data)
{
	uint16_t buf;

	WREN(0);
	CLK(0);

	buf = GPIO_ReadOutputData(GPIOF) & 0x00c0;
	buf |= (read_addr & 0x07) | ((mem_select & 0x01) << 3);
	GPIO_Write(GPIOF, buf);

	CLK(1);
	CLK(0);
	CLK(1);

	*read_data = (GPIO_ReadInputData(GPIOF) & 0xf000) >> 12;
	WREN(0);
}

static void Write_Data(uint8_t mem_select, uint8_t write_addr, uint8_t write_data)
{
	uint16_t buf;

	WREN(1);
	CLK(0);

	buf = GPIO_ReadOutputData(GPIOF) & 0x00c0;
	buf |= (write_addr & 0x07) |
	       ((mem_select & 0x01) << 3) |
	       ((write_data & 0x0f) << 8);
	GPIO_Write(GPIOF, buf);

	CLK(1);
	WREN(0);
}

static void Key_Scan(void)
{
	uint8_t gpioc_data;
	uint8_t addr;
	uint8_t mem_select;
	uint8_t data;
	uint8_t data_m1;
	uint8_t data_m2;
	uint8_t result;
	uint8_t result_readback;
	uint16_t hold_count;

	if (KEY_READ() == 0) {
		Delay_Ms(5);
		if (KEY_READ() == 0) {
			hold_count = 0;
			while (KEY_READ() == 0) {
				Delay_Ms(10);
				if (hold_count < 1000) {
					hold_count++;
				}
			}

			gpioc_data = ~(GPIO_ReadInputData(GPIOC) & 0x00ff);
			addr = gpioc_data & 0x07;

			if (hold_count >= 80) {
				Read_Data(0, addr, &data_m1);
				Read_Data(1, addr, &data_m2);

				/* M1 and M2 are 4-bit memories, so save the low 4 bits. */
				result = (data_m1 + data_m2) & 0x0f;
				Write_Data(0, RESULT_ADDR, result);
				Read_Data(0, RESULT_ADDR, &result_readback);
				Display_Result(0, result_readback);
			} else {
				mem_select = (gpioc_data >> 3) & 0x01;

				Read_Data(mem_select, addr, &data);
				Display_Result(mem_select, data);
			}
		}
		return;
	}

	if (KEY_WRITE() == 0) {
		Delay_Ms(5);
		if (KEY_WRITE() == 0) {
			while (KEY_WRITE() == 0) {
			}

			gpioc_data = ~(GPIO_ReadInputData(GPIOC) & 0x00ff);
			addr = gpioc_data & 0x07;
			mem_select = (gpioc_data >> 3) & 0x01;
			data = (gpioc_data & 0xf0) >> 4;

			Write_Data(mem_select, addr, data);
			Display_Result(mem_select, data);
		}
	}
}

int main(void)
{
	Delay_Init();
	Led_Init();
	Sw_Init();
	GPIO_FPGA_Init();

	while (1) {
		Key_Scan();
	}
}
