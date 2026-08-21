#include "stm32f4xx.h"
#include "delay.h"
#include "led.h"
#include "exti.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    Delay_Init();
    LED_Hardware_Init();
    EXTI_Configure();

    while (1)
    {
        if (led_flow_flag == 2)
        {
            led_flow_flag = 0;
            LED_TrafficFlash();
        }
        else
        {
            LED_MainFlowStep();
        }
    }
}

// end file
