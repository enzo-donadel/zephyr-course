#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#if CONFIG_LED_SUBSYSTEM
        #include "LED/led_subsystem.h"

        #include <zephyr/drivers/pwm.h>
#endif

#define SLEEP_TIME_MS 100

#define LED_NODE DT_ALIAS(led0)
#define PWM_LED_NODE DT_ALIAS(pwm_led0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
#if CONFIG_LED_SUBSYSTEM
static const struct pwm_dt_spec pwm_led = PWM_DT_SPEC_GET(PWM_LED_NODE);
#endif
LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void) {
        bool led_state = true;

        if (!gpio_is_ready_dt(&led))
                return 0;

        if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0)
                return 0;

#if CONFIG_LED_SUBSYSTEM
        #ifndef CONFIG_LED_SUBSYSTEM_CUSTOM_PATTERN
        led_subsystem_init(&pwm_led);
        #else
        led_subsystem_init(&pwm_led, 1000, 80, 200);
        #endif
#endif

        while (1) {
                int ret = 0;
                if ((ret = gpio_pin_toggle_dt(&led)) < 0)
                        return ret;

                led_state = !led_state;
                LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
                k_msleep(SLEEP_TIME_MS);
        }
        return 0;
}
