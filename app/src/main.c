#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "Heartbeat/heartbeat.h"

#if CONFIG_LED_SUBSYSTEM
#include "LED/led_subsystem.h"
#include <zephyr/drivers/pwm.h>
#endif

#if CONFIG_LED_SENSOR
#include <zephyr/drivers/sensor.h>
#endif

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#define LED_NODE DT_ALIAS(app_led)
static const struct gpio_dt_spec hb_led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

#define PWM_LED_NODE DT_ALIAS(pwm_led0)
#if CONFIG_LED_SUBSYSTEM
static const struct pwm_dt_spec pwm_led = PWM_DT_SPEC_GET(PWM_LED_NODE);
#endif

#if CONFIG_LED_SENSOR
static const struct device *led_sensor_dev = DEVICE_DT_GET(DT_NODELABEL(led_sensor0));
#endif

int main(void) {
        int ret = heartbeat_subsystem_init(&hb_led);
        if (ret < 0) {
                LOG_ERR("Failed to initialize Heartbeat subsystem: %d", ret);
                return ret;
        }

#if CONFIG_LED_SUBSYSTEM
        #ifndef CONFIG_LED_SUBSYSTEM_CUSTOM_PATTERN
        led_subsystem_init(&pwm_led);
        #else
        led_subsystem_init(&pwm_led, 1000, 80, 200);
        #endif
#endif

#if CONFIG_LED_SENSOR
        struct sensor_value val;

        if (!device_is_ready(led_sensor_dev)) {
                LOG_ERR("LED sensor device not ready");
        } else {
                LOG_INF("LED sensor device ready, starting sample fetch");
                sensor_sample_fetch(led_sensor_dev);
                k_sleep(K_MSEC(2000));
                sensor_channel_get(led_sensor_dev, SENSOR_CHAN_ALL, &val);
        }
#endif

        while (1) {
                k_sleep(K_FOREVER);
        }
        return 0;
}
