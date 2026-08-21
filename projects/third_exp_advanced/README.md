# Third Experiment - Advanced Part

Keil project:

```text
third_exp_advanced/USER/Test.uvprojx
```

Wiring:

```text
PF0-PF7 (P12) -> LED1-LED8 (P2)
PC0 (P11)     -> SI  (P4)
PC1 (P11)     -> RCK (P4)
PC2 (P11)     -> SCK (P4)
PC3 (P11)     -> A   (P4)
PC4 (P11)     -> B   (P4)
PC5 (P11)     -> C   (P4)
PB8 (P10)     -> KEY1_N (P1)
PB11 (P10)    -> KEY2_N (P1)
```

Behavior:

```text
1. The one-digit seven-segment display cycles from 0 to 9.
2. KEY1_N/PB8 is configured as EXTI8 with higher preemption priority.
3. KEY2_N/PB11 is configured as EXTI11 with lower preemption priority.
4. Press KEY1 to run the traffic-light flashing demo.
5. Press KEY2 to run the LED running-light demo for about 5 seconds.
6. If KEY1 is pressed while the KEY2 effect is running, KEY1 overrides it.
```

Main changed files:

```text
USER/main.c
HARDWARE/MY_EXIT/exti.c
HARDWARE/LED/led.c
HARDWARE/LED/led.h
HARDWARE/SMG/smg.c
```
