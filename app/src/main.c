#include <errno.h>
#include <string.h>

#define LOG_LEVEL 4
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main);

#include <zephyr/kernel.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/sys/util.h>

#define STRIP_NODE DT_ALIAS(led_strip)

#if DT_NODE_HAS_PROP(DT_ALIAS(led_strip), chain_length)
#define STRIP_NUM_PIXELS DT_PROP(DT_ALIAS(led_strip), chain_length)
#else
#error Unable to determine length of LED strip
#endif

#define DELAY_TIME K_MSEC(CONFIG_SAMPLE_LED_UPDATE_DELAY)

#if defined(CONFIG_LED_ADVANCED_SETTINGS)
#define APPLY_BRIGHTNESS(ch) \
    ((uint8_t)(((uint32_t)(ch) * CONFIG_SAMPLE_LED_BRIGHTNESS) / 100))
#else
#define APPLY_BRIGHTNESS(ch) ((uint8_t)(ch))
#endif

#define RGB(_r, _g, _b) \
    { .r = APPLY_BRIGHTNESS(_r), \
      .g = APPLY_BRIGHTNESS(_g), \
      .b = APPLY_BRIGHTNESS(_b) }

/* Single active color derived from Kconfig choice */
static const struct led_rgb active_color =
    RGB(CONFIG_SAMPLE_LED_COLOR_R,
        CONFIG_SAMPLE_LED_COLOR_G,
        CONFIG_SAMPLE_LED_COLOR_B);

/* Custom pattern — three steps cycling through R, G, B */
#if defined(CONFIG_SAMPLE_LED_CUSTOM_PATTERN)
static const struct led_rgb custom_pattern[] = {
    RGB(255, 0,   0),
    RGB(0,   255, 0),
    RGB(0,   0,   255),
    RGB(255, 255, 0),
    RGB(0,   255, 255),
};
#endif

static struct led_rgb pixels[STRIP_NUM_PIXELS];
static const struct device *const strip = DEVICE_DT_GET(STRIP_NODE);

int main(void)
{
    int rc;

    if (device_is_ready(strip)) {
        LOG_INF("Found LED strip device %s", strip->name);
    } else {
        LOG_ERR("LED strip device %s is not ready", strip->name);
        return 0;
    }

#if defined(CONFIG_SAMPLE_LED_DEBUG)
    LOG_INF("LED Debug enabled");
    LOG_INF("  Delay     : %d ms", CONFIG_SAMPLE_LED_UPDATE_DELAY);
    LOG_INF("  Brightness: %d%%",  CONFIG_SAMPLE_LED_BRIGHTNESS);
    LOG_INF("  Color R   : %d",    CONFIG_SAMPLE_LED_COLOR_R);
    LOG_INF("  Color G   : %d",    CONFIG_SAMPLE_LED_COLOR_G);
    LOG_INF("  Color B   : %d",    CONFIG_SAMPLE_LED_COLOR_B);
#if defined(CONFIG_SAMPLE_LED_CUSTOM_PATTERN)
    LOG_INF("  Mode      : Custom pattern (%d steps)",
            ARRAY_SIZE(custom_pattern));
#else
    LOG_INF("  Mode      : Standard sweep");
#endif
#endif /* CONFIG_SAMPLE_LED_DEBUG */

    LOG_INF("Displaying pattern on strip");

    while (1) {

#if defined(CONFIG_SAMPLE_LED_CUSTOM_PATTERN)
        /* ── Custom pattern mode ── */
        for (size_t step = 0; step < ARRAY_SIZE(custom_pattern); step++) {
            for (size_t cursor = 0; cursor < STRIP_NUM_PIXELS; cursor++) {
                memset(pixels, 0x00, sizeof(pixels));
                memcpy(&pixels[cursor], &custom_pattern[step],
                       sizeof(struct led_rgb));

                rc = led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);
                if (rc) {
                    LOG_ERR("couldn't update strip: %d", rc);
                }

#if defined(CONFIG_SAMPLE_LED_DEBUG)
                LOG_DBG("Custom step %d cursor %d: R=%d G=%d B=%d",
                        step, cursor,
                        custom_pattern[step].r,
                        custom_pattern[step].g,
                        custom_pattern[step].b);
#endif
                k_sleep(DELAY_TIME);
            }
        }

#else
        /* ── Standard sweep mode — single Kconfig-selected color ── */
        for (size_t cursor = 0; cursor < STRIP_NUM_PIXELS; cursor++) {
            memset(pixels, 0x00, sizeof(pixels));
            memcpy(&pixels[cursor], &active_color, sizeof(struct led_rgb));

            rc = led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);
            if (rc) {
                LOG_ERR("couldn't update strip: %d", rc);
            }

#if defined(CONFIG_SAMPLE_LED_DEBUG)
            LOG_DBG("Cursor %d: R=%d G=%d B=%d",
                    cursor,
                    active_color.r,
                    active_color.g,
                    active_color.b);
#endif
            k_sleep(DELAY_TIME);
			    /* OFF */
    memset(pixels, 0x00, sizeof(pixels));
    led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);
    k_sleep(DELAY_TIME);
        }
#endif /* CONFIG_SAMPLE_LED_CUSTOM_PATTERN */

    }

    return 0;
}