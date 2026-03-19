#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_gpio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_LED_GPIO      BOARD_LED_RED_GPIO
#define BOARD_LED_GPIO_PIN  BOARD_LED_RED_GPIO_PIN

#define LOGIC_LED_ON   0U
#define LOGIC_LED_OFF  1U

#define MORSE_UNIT_MS  200U

/*******************************************************************************
 * Global
 ******************************************************************************/
volatile uint32_t g_ms = 0;

/*******************************************************************************
 * SysTick
 ******************************************************************************/
void SysTick_Handler(void)
{
    g_ms++;
}

void systick_init(void)
{
    SysTick_Config(SystemCoreClock / 1000U); // 1 ms
}

void delay_ms(uint32_t ms)
{
    uint32_t start = g_ms;
    while ((g_ms - start) < ms);
}

/*******************************************************************************
 * LED
 ******************************************************************************/
void led_on(void)
{
    GPIO_PinWrite(BOARD_LED_GPIO, BOARD_LED_GPIO_PIN, LOGIC_LED_ON);
}

void led_off(void)
{
    GPIO_PinWrite(BOARD_LED_GPIO, BOARD_LED_GPIO_PIN, LOGIC_LED_OFF);
}

/*******************************************************************************
 * Morse basic
 ******************************************************************************/
void dot(void)
{
    led_on();
    delay_ms(MORSE_UNIT_MS);
    led_off();
    delay_ms(MORSE_UNIT_MS);
}

void dash(void)
{
    led_on();
    delay_ms(3 * MORSE_UNIT_MS);
    led_off();
    delay_ms(MORSE_UNIT_MS);
}

void letter_gap(void)
{
    delay_ms(2 * MORSE_UNIT_MS);
}

/*******************************************************************************
 * Trimite un model Morse (string ".-" )
 ******************************************************************************/
void send_pattern(const char *p)
{
    while (*p)
    {
        if (*p == '.')
            dot();
        else
            dash();

        p++;
    }
    letter_gap();
}

/*******************************************************************************
 * Main
 ******************************************************************************/
int main(void)
{
    gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput,
        LOGIC_LED_OFF
    };

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    SystemCoreClockUpdate();

    GPIO_PinInit(BOARD_LED_GPIO, BOARD_LED_GPIO_PIN, &led_config);
    systick_init();

    while (1)
    {
        /* salut */
        send_pattern("...");   // s
        send_pattern(".-");    // a
        send_pattern(".-..");  // l
        send_pattern("..-");   // u
        send_pattern("-");     // t

        delay_ms(1000);
    }
}
