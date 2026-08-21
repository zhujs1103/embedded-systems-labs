#include "stm32f4xx.h"
#include "delay.h"
#include "smg.h"
#include "led.h"
#include "exti.h"

#define ROUND_TIMEOUT_MS       3000U
#define RANDOM_WAIT_BASE_MS    900U
#define RANDOM_WAIT_SPAN_MS    1800U
#define GAME_TICK_MS           10U

static uint16_t g_random_state = 0xACE1U;

static uint16_t Random_Next(void)
{
    uint16_t bit;

    bit = (uint16_t)(((g_random_state >> 0) ^
                      (g_random_state >> 2) ^
                      (g_random_state >> 3) ^
                      (g_random_state >> 5)) & 1U);
    g_random_state = (uint16_t)((g_random_state >> 1) | (bit << 15));

    return g_random_state;
}

static void Key_ClearEvents(void)
{
    key1_event = 0;
    key2_event = 0;
}

static void Key_WaitRelease(void)
{
    while (KEY1_IsDown() || KEY2_IsDown())
    {
        Random_Next();
        Delay_Ms(GAME_TICK_MS);
    }

    Key_ClearEvents();
}

static void Game_Countdown(void)
{
    SMG_ShowDigit(3);
    LED_Set(0x0018);
    Delay_Ms(250);

    SMG_ShowDigit(2);
    LED_Set(0x0024);
    Delay_Ms(250);

    SMG_ShowDigit(1);
    LED_Set(0x0042);
    Delay_Ms(250);

    SMG_ShowDigit(0);
    LED_Set(0x0000);
}

static uint8_t Game_WaitBeforeGo(uint16_t wait_ms)
{
    uint16_t elapsed = 0;
    uint8_t pos = 0;

    while (elapsed < wait_ms)
    {
        if (key1_event || KEY1_IsDown())
        {
            key1_event = 0;
            return 1;
        }

        if (key2_event)
        {
            key2_event = 0;
            return 2;
        }

        LED_Set((uint16_t)(1U << pos));
        pos++;
        pos &= 0x07;

        Random_Next();
        Delay_Ms(GAME_TICK_MS);
        elapsed = (uint16_t)(elapsed + GAME_TICK_MS);
    }

    return 0;
}

static uint16_t Game_MeasureReaction(void)
{
    uint16_t elapsed = 0;

    LED_Set(0x00FF);
    SMG_ShowDigit(0);
    Key_ClearEvents();

    while (elapsed < ROUND_TIMEOUT_MS)
    {
        if (key1_event || KEY1_IsDown())
        {
            key1_event = 0;
            return elapsed;
        }

        if (key2_event)
        {
            key2_event = 0;
            return ROUND_TIMEOUT_MS;
        }

        if ((elapsed % 100U) == 0U)
        {
            SMG_ShowDigit((uint8_t)((elapsed / 100U) % 10U));
        }

        Delay_Ms(GAME_TICK_MS);
        elapsed = (uint16_t)(elapsed + GAME_TICK_MS);
    }

    return ROUND_TIMEOUT_MS;
}

static uint8_t Game_ScoreFromTime(uint16_t reaction_ms)
{
    uint8_t score;

    if (reaction_ms >= ROUND_TIMEOUT_MS)
    {
        return 0;
    }

    if (reaction_ms <= 120U)
    {
        return 9;
    }

    if (reaction_ms >= 1800U)
    {
        return 1;
    }

    score = (uint8_t)(9U - ((reaction_ms - 120U) / 210U));
    if (score == 0U)
    {
        score = 1;
    }

    return score;
}

static void Game_ShowFoul(void)
{
    uint8_t i;

    SMG_ShowBlank();
    for (i = 0; i < 4; i++)
    {
        LED_Set(0x00FF);
        Delay_Ms(120);
        LED_Set(0x0000);
        Delay_Ms(120);
    }
    SMG_ShowDigit(0);
}

static void Game_ShowResult(uint8_t score)
{
    uint8_t i;

    for (i = 0; i < 18; i++)
    {
        SMG_ShowDigit(score);
        LED_ShowScore(score);
        Delay_Ms(100);
    }
}

static void Game_PlayRound(uint8_t *best_score)
{
    uint16_t wait_ms;
    uint16_t reaction_ms;
    uint8_t wait_result;
    uint8_t score;

    Key_WaitRelease();
    Game_Countdown();

    wait_ms = (uint16_t)(RANDOM_WAIT_BASE_MS +
                         (Random_Next() % RANDOM_WAIT_SPAN_MS));
    wait_result = Game_WaitBeforeGo(wait_ms);
    if (wait_result != 0U)
    {
        Game_ShowFoul();
        return;
    }

    reaction_ms = Game_MeasureReaction();
    score = Game_ScoreFromTime(reaction_ms);

    if (score > *best_score)
    {
        *best_score = score;
    }

    Game_ShowResult(score);
    Key_WaitRelease();
}

static void Game_ShowIdle(uint8_t best_score)
{
    static uint8_t pos = 0;

    SMG_ShowDigit(best_score);
    LED_Set((uint16_t)(1U << pos));
    pos++;
    pos &= 0x07;
}

int main(void)
{
    uint8_t best_score = 0;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    Delay_Init();
    LED_Hardware_Init();
    SMG_Init();
    EXTI_Configure();

    Key_ClearEvents();
    SMG_ShowDigit(0);
    LED_Set(0x0000);

    while (1)
    {
        if (key1_event)
        {
            key1_event = 0;
            Game_PlayRound(&best_score);
        }

        if (key2_event)
        {
            key2_event = 0;
            best_score = 0;
            LED_BlinkAll(2, 80);
            SMG_ShowDigit(0);
        }

        Game_ShowIdle(best_score);
        Random_Next();
        Delay_Ms(80);
    }
}
