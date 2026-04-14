#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <zephyr/drivers/gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the heartbeat subsystem and start the heartbeat thread.
 * The heartbeat subsystem will toggle the specified LED at a regular interval
 * defined by the APP_HEARTBEAT_PERIOD_MS Kconfig option. The LED will be
 * turned on for half of the period and off for the other half.
 *
 * @param hb_led  Pointer to a ready GPIO DT spec for the heartbeat LED.
 * @return 0 on success, negative errno on failure.
 */

int heartbeat_subsystem_init(const struct gpio_dt_spec *hb_led);

#ifdef __cplusplus
}
#endif

#endif /* HEARTBEAT_H */
