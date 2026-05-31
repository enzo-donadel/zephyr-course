#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>

#if CONFIG_LED_SENSOR

static const struct device *led_sensor_dev = DEVICE_DT_GET(DT_NODELABEL(led_sensor0));

static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv)
{
	int ret = sensor_sample_fetch(led_sensor_dev);

	shell_print(sh, "sensor_sample_fetch returned %d", ret);
	return ret;
}

static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv)
{
	struct sensor_value val;
	int ret = sensor_channel_get(led_sensor_dev, SENSOR_CHAN_ALL, &val);

	if (ret == 0) {
		shell_print(sh, "val1=%d val2=%d", val.val1, val.val2);
	} else {
		shell_print(sh, "sensor_channel_get failed: %d", ret);
	}
	return ret;
}

static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv)
{
	shell_print(sh, "Device: %s", led_sensor_dev->name);
	shell_print(sh, "Ready:  %s", device_is_ready(led_sensor_dev) ? "yes" : "no");
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sensor_cmds,
	SHELL_CMD(fetch, NULL, "Call sensor_sample_fetch()", cmd_sensor_fetch),
	SHELL_CMD(read,  NULL, "Call sensor_channel_get() and print result", cmd_sensor_read),
	SHELL_CMD(info,  NULL, "Print device name and ready state", cmd_sensor_info),
	SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sensor_cmds, "LED sensor commands", NULL);

#endif /* CONFIG_LED_SENSOR */
