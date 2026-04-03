#ifndef LED_SUBSYSTEM_H
#define LED_SUBSYSTEM_H

#include <zephyr/drivers/pwm.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the LED subsystem and start the blink thread.
 *
 * Timing layout (total = BLINK_PERIOD):
 *   ON(on_off_ms) → FADE(fade_ms, brightness→0) → OFF(on_off_ms)
 *
 * @param led  Pointer to a ready PWM DT spec for the LED.
 *
 * When CONFIG_LED_SUBSYSTEM_CUSTOM_PATTERN is enabled, also accepts:
 * @param blink_period_ms  Total blink period in milliseconds.
 * @param brightness       Brightness during ON phase (0–100 %).
 * @param fade_duration_ms Duration of the fade-out after ON phase (ms).
 *
 * @return 0 on success, negative errno on failure.
 */
#ifdef CONFIG_LED_SUBSYSTEM_CUSTOM_PATTERN
int led_subsystem_init(const struct pwm_dt_spec *led,
		       uint32_t blink_period_ms,
		       uint8_t brightness,
		       uint32_t fade_duration_ms);
#else
int led_subsystem_init(const struct pwm_dt_spec *led);
#endif

#ifdef __cplusplus
}
#endif

#endif /* LED_SUBSYSTEM_H */
