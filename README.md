# Embedded Systems Labs

Selected STM32F4 and FPGA experiments. Only student-written application/driver code, project configuration and concise documentation are included; vendor libraries, build products and J-Link logs are excluded.

## Projects

| Directory | Focus |
|---|---|
| `exp1_multi_pattern_led` | Multi-pattern GPIO LED control |
| `second_exp_plus_arm` | STM32F4-side memory/peripheral experiment |
| `second_exp_plus_fpga` | Verilog dual-port RAM and testbench |
| `third_exp_basic` | Running light plus EXTI-triggered traffic-light effect |
| `third_exp_advanced` | Nested interrupt priority, seven-segment display and LED effects |
| `exp4_reaction_game` | Reaction-speed game using GPIO, EXTI, timer, LEDs and seven-segment display |

## Toolchain and status

- STM32 projects use Keil MDK and the STM32F4 Standard Peripheral Library expected by the original experiment box.
- FPGA sources include Quartus project settings for the corresponding lab design.
- The curated copy is intentionally not standalone without the vendor libraries.
- Source structure was checked, but no fresh Keil/Quartus build or hardware test was run during portfolio preparation.
