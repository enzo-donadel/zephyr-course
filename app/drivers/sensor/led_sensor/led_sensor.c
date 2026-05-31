#define DT_DRV_COMPAT led_sensor

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(led_sensor, LOG_LEVEL_INF);

struct led_sensor_data {
	int state;
};

struct led_sensor_config {
	struct gpio_dt_spec led;
};

static int led_sensor_init(const struct device *dev)
{
	const struct led_sensor_config *cfg = dev->config;

	if (!gpio_is_ready_dt(&cfg->led)) {
		LOG_ERR("LED GPIO not ready");
		return -ENODEV;
	}

	return gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_INACTIVE);
}

static int led_sensor_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
	const struct led_sensor_config *cfg = dev->config;
	struct led_sensor_data *data = dev->data;

	gpio_pin_set_dt(&cfg->led, 1);
	data->state = 1;
	LOG_INF("LED ON");
	return 0;
}

static int led_sensor_channel_get(const struct device *dev, enum sensor_channel chan,
				   struct sensor_value *val)
{
	const struct led_sensor_config *cfg = dev->config;
	struct led_sensor_data *data = dev->data;

	val->val1 = data->state;
	val->val2 = 0;

	gpio_pin_set_dt(&cfg->led, 0);
	data->state = 0;
	LOG_INF("LED OFF");
	return 0;
}

static const struct sensor_driver_api led_sensor_api = {
	.sample_fetch = led_sensor_sample_fetch,
	.channel_get  = led_sensor_channel_get,
};

#define LED_SENSOR_INIT(n)                                                \
	static struct led_sensor_data led_sensor_data_##n;                \
                                                                          \
	static const struct led_sensor_config led_sensor_config_##n = {   \
		.led = GPIO_DT_SPEC_INST_GET(n, gpios),                   \
	};                                                                \
                                                                          \
	DEVICE_DT_INST_DEFINE(n,                                          \
			      led_sensor_init,                            \
			      NULL,                                       \
			      &led_sensor_data_##n,                       \
			      &led_sensor_config_##n,                     \
			      POST_KERNEL,                                \
			      CONFIG_SENSOR_INIT_PRIORITY,                \
			      &led_sensor_api);

DT_INST_FOREACH_STATUS_OKAY(LED_SENSOR_INIT)
