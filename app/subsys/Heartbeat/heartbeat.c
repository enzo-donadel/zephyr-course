
#include "Heartbeat/heartbeat.h"

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#ifdef CONFIG_APP_HEARTBEAT_DEBUG
LOG_MODULE_REGISTER(heartbeat, LOG_LEVEL_DBG);
#else
LOG_MODULE_REGISTER(heartbeat, LOG_LEVEL_ERR);
#endif

#define HB_STACK_SIZE 512
#define HB_PRIORITY K_LOWEST_APPLICATION_THREAD_PRIO

static K_THREAD_STACK_DEFINE(hb_stack, HB_STACK_SIZE);
static struct k_thread hb_thread_data;

static void hb_thread_fn(void *p1, void *p2, void *p3) {
        ARG_UNUSED(p2);
        ARG_UNUSED(p3);

        const struct gpio_dt_spec *hb = p1;
        while (1) {
                const int val = gpio_pin_get_dt(hb);
                gpio_pin_toggle_dt(hb);
                LOG_DBG("Heartbeat LED toggled from %d to %d", val, val ? 0 : 1);
                k_sleep(K_MSEC(CONFIG_APP_HEARTBEAT_PERIOD_MS / 2));
        }
}

int heartbeat_subsystem_init(const struct gpio_dt_spec *hb_led){
        if (!gpio_is_ready_dt(hb_led)){
                LOG_ERR("Heartbeat LED GPIO device not ready");
                return -ENODEV;
        }

        if (gpio_pin_configure_dt(hb_led, GPIO_OUTPUT_ACTIVE) < 0){
                LOG_ERR("Failed to configure Heartbeat LED GPIO pin");
                return -EIO;
        }

        k_thread_create(&hb_thread_data, hb_stack,
                        K_THREAD_STACK_SIZEOF(hb_stack), hb_thread_fn, (struct gpio_dt_spec *)hb_led,
                        NULL, NULL, HB_PRIORITY, 0, K_NO_WAIT);
        k_thread_name_set(&hb_thread_data, "heartbeat_subsystem");
        LOG_INF("Heartbeat subsystem started (period=%ums)", CONFIG_APP_HEARTBEAT_PERIOD_MS);

        return 0;
}
