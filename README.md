# clock-project
Simple usage of RTC and UART with buttons that connected via EXTI

## Peripherals
1. RTC
    - clock source: LSE 32.768 KHz
    - format: BCD
2. UART
    - clock source: HSI 16 MHz
    - mode: async
    - baud rate: 115200 bps
3. EXTI
    - mode: rising edge trigger detection
    - internal pull down
