#include "LED/led_subsystem.h"

#include <zephyr/drivers/pwm.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#ifdef CONFIG_LED_SUBSYSTEM_DEBUG
LOG_MODULE_REGISTER(led_subsystem, LOG_LEVEL_DBG);
#else
LOG_MODULE_REGISTER(led_subsystem, LOG_LEVEL_INF);
#endif

/* Fallbacks when CONFIG_LED_SUBSYSTEM_ADVANCED_SETTINGS is disabled. */
#ifndef CONFIG_LED_SUBSYSTEM_ADV_BRIGHTNESS
        #define CONFIG_LED_SUBSYSTEM_ADV_BRIGHTNESS 100
#endif
#ifndef CONFIG_LED_SUBSYSTEM_ADV_FADE_DURATION
        #define CONFIG_LED_SUBSYSTEM_ADV_FADE_DURATION 0
#endif

#define LED_PWM_PERIOD_NS PWM_MSEC(1)
#define FADE_STEPS 20U

#define LED_STACK_SIZE 512
#define LED_PRIORITY K_LOWEST_APPLICATION_THREAD_PRIO

static K_THREAD_STACK_DEFINE(led_stack, LED_STACK_SIZE);
static struct k_thread led_thread_data;

struct led_cfg {
        const struct pwm_dt_spec *led;
        uint32_t blink_period_ms;
        uint32_t fade_duration_ms;
        uint8_t brightness;
};

static struct led_cfg s_cfg;

static void led_thread_fn(void *p1, void *p2, void *p3) {
        ARG_UNUSED(p2);
        ARG_UNUSED(p3);

        const struct led_cfg *cfg = p1;

        uint32_t on_off_ms =
                (cfg->blink_period_ms - cfg->fade_duration_ms) / 2U;

        uint32_t on_pulse_ns =
                (uint64_t)LED_PWM_PERIOD_NS * cfg->brightness / 100U;
        uint32_t step_ms = (cfg->fade_duration_ms > 0U)
                                   ? cfg->fade_duration_ms / FADE_STEPS
                                   : 0U;

        LOG_DBG("period=%ums brightness=%u%% fade=%ums on_off=%ums",
                cfg->blink_period_ms, cfg->brightness, cfg->fade_duration_ms,
                on_off_ms);

        while (1) {
                pwm_set_dt(cfg->led, LED_PWM_PERIOD_NS, on_pulse_ns);
                k_sleep(K_MSEC(on_off_ms));

                if (cfg->fade_duration_ms == 0U) {
                        pwm_set_dt(cfg->led, LED_PWM_PERIOD_NS, 0U);
                        k_sleep(K_MSEC(on_off_ms));
                        continue;
                }

                for (uint32_t i = FADE_STEPS; i > 0U; i--) {
                        uint32_t pulse = on_pulse_ns * (i - 1U) / FADE_STEPS;
                        pwm_set_dt(cfg->led, LED_PWM_PERIOD_NS, pulse);
                        k_sleep(K_MSEC(step_ms));
                }

                pwm_set_dt(cfg->led, LED_PWM_PERIOD_NS, 0U);
                k_sleep(K_MSEC(on_off_ms));
        }
}

#ifdef CONFIG_LED_SUBSYSTEM_CUSTOM_PATTERN
int led_subsystem_init(const struct pwm_dt_spec *led, uint32_t blink_period_ms,
                       uint8_t brightness, uint32_t fade_duration_ms)
#else
int led_subsystem_init(const struct pwm_dt_spec *led)
#endif
{
#ifndef CONFIG_LED_SUBSYSTEM_CUSTOM_PATTERN
        uint32_t blink_period_ms = CONFIG_LED_SUBSYSTEM_BLINK_PERIOD;
        uint32_t fade_duration_ms = CONFIG_LED_SUBSYSTEM_ADV_FADE_DURATION;
        uint8_t brightness = CONFIG_LED_SUBSYSTEM_ADV_BRIGHTNESS;
#endif
        if (!pwm_is_ready_dt(led)) {
                LOG_ERR("PWM device not ready");
                return -ENODEV;
        }
        if (fade_duration_ms >= blink_period_ms) {
                LOG_ERR("fade_duration (%u ms) must be < blink_period (%u ms)",
                        fade_duration_ms, blink_period_ms);
                return -EINVAL;
        }

        s_cfg.led = led;
        s_cfg.blink_period_ms = blink_period_ms;
        s_cfg.brightness = brightness;
        s_cfg.fade_duration_ms = fade_duration_ms;

        k_thread_create(&led_thread_data, led_stack,
                        K_THREAD_STACK_SIZEOF(led_stack), led_thread_fn, &s_cfg,
                        NULL, NULL, LED_PRIORITY, 0, K_NO_WAIT);
        k_thread_name_set(&led_thread_data, "led_subsystem");
        LOG_INF("LED subsystem started (period=%ums brightness=%u%% fade=%ums)",
                blink_period_ms, brightness, fade_duration_ms);

        return 0;
}
