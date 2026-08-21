#include "stm32f4xx.h"
#include "delay.h"
#include "smg.h"
#include "led.h"
#include "exti.h"

int main(void)
{
    uint8_t num = 0;
    uint16_t tick = 0;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    Delay_Init();
    SMG_Init();
    LED_Hardware_Init();
    EXTI_Configure();

    while (1)
    {
        if (led_flow_flag == 1)
        {
            LED_TrafficFlash();
            led_flow_flag = 0;
            LED_SET(0x0000);
        }
        else if (led_flow_flag == 2)
        {
            LED_FlowForMs(5000);
            if (led_flow_flag == 2)
            {
                led_flow_flag = 0;
            }
            LED_SET(0x0000);
        }
        else
        {
            SMG_Sele(num);
            Delay_Ms(1);

            tick++;
            if (tick >= 500)
            {
                tick = 0;
                num++;
                if (num >= 10)
                {
                    num = 0;
                }
            }
        }
    }
}

// end file
