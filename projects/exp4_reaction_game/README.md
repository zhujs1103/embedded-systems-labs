# Experiment 4 - Reaction Game

Keil project:

```text
exp4_reaction_game/USER/Test.uvprojx
```

## Idea

This free-design experiment turns the experiment box into a small reaction-speed
game. It combines GPIO output, seven-segment display output, GPIO input and EXTI
interrupts.

## Wiring

```text
PF0-PF7 (P12) -> LED1-LED8 (P2)

PC0 (P11)     -> SI  (P4)
PC1 (P11)     -> RCK (P4)
PC2 (P11)     -> SCK (P4)
PC3 (P11)     -> A   (P4)
PC4 (P11)     -> B   (P4)
PC5 (P11)     -> C   (P4)

PB8  (P10)    -> KEY1_N (P1)
PB11 (P10)    -> KEY2_N (P1)
```

## Controls

```text
KEY1: start a round, then press again after the GO signal
KEY2: reset the best score while idle, abort the current round while playing
```

## Behavior

```text
1. Idle mode shows the best score on the one-digit seven-segment display.
2. Press KEY1 to start.
3. The display counts down 3, 2, 1.
4. The program waits for a pseudo-random time while the LEDs run.
5. Pressing KEY1 before the GO signal is a foul; all LEDs flash and score is 0.
6. When all LEDs turn on, press KEY1 as quickly as possible.
7. The reaction time is converted to a 0-9 score.
8. LED1-LED8 show the score as a bar graph, and the display shows the score.
9. The best score is kept until KEY2 resets it or the board is reset.
```

## Main Changed Files

```text
USER/main.c
HARDWARE/LED/led.c
HARDWARE/LED/led.h
HARDWARE/SMG/smg.c
HARDWARE/SMG/smg.h
HARDWARE/MY_EXIT/exti.c
HARDWARE/MY_EXIT/exti.h
```

## Scoring

```text
<= 120 ms: 9 points
120-1800 ms: decreasing score
timeout or abort: 0 points
```
