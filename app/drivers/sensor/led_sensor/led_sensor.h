#ifndef LED_SENSOR_H
#define LED_SENSOR_H

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

/* Extended API — sensor_driver_api MUST be the first member so that the
 * standard sensor_*() calls can safely cast dev->api to sensor_driver_api*. */
struct led_sensor_driver_api {
	struct sensor_driver_api sensor;
	int (*set_state)(const struct device *dev, bool on);
};

static inline int led_sensor_set_state(const struct device *dev, bool on)
{
	const struct led_sensor_driver_api *api =
		(const struct led_sensor_driver_api *)dev->api;

	return api->set_state(dev, on);
}

#endif /* LED_SENSOR_H */
