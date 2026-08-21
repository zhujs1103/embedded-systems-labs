# Third Experiment - Basic Part

Keil project:

```text
third_exp_basic/USER/Test.uvprojx
```

Wiring:

```text
PF0-PF7 (P12) -> LED1-LED8 (P2)
PB8 (P10)     -> KEY1_N (P1)
```

Behavior:

```text
1. After power-on, the main loop runs LED1-LED8 as a running light.
2. KEY1_N/PB8 is configured as EXTI8, falling-edge triggered.
3. Press KEY1 to enter the external interrupt.
4. The interrupt handler only sets led_flow_flag.
5. The main loop detects the flag and runs a traffic-light flashing demo.
6. After the flashing demo, the program returns to the normal running light.
```

Main changed files:

```text
USER/main.c
HARDWARE/MY_EXIT/exti.c
HARDWARE/LED/led.c
HARDWARE/LED/led.h
```
