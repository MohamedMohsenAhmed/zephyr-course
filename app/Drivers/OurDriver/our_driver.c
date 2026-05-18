#include <zephyr/kernel.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>





#define DT_DRV_COMPAT our_driver

#define STRIP_NODE DT_ALIAS(app_led)

#if DT_NODE_HAS_PROP(DT_ALIAS(app_led), chain_length)
#define STRIP_NUM_PIXELS DT_PROP(DT_ALIAS(app_led), chain_length)
#else
#error Unable to determine length of LED strip
#endif

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

typedef struct our_driver_data {
    int  DriverCounter;
}our_data_t;

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

static struct led_rgb pixels[STRIP_NUM_PIXELS];
static const struct device *const strip = DEVICE_DT_GET(STRIP_NODE);

static int OurDriver_init(const struct device *dev);
static int OurDriver_GetChannel(const struct device *dev, enum sensor_channel channel, struct sensor_value* val);
static int OurDriver_SampleFetch(const struct device *dev, enum sensor_channel channel);



static DEVICE_API(sensor, api_our_driver) = {
    .sample_fetch = OurDriver_SampleFetch,
    .channel_get = OurDriver_GetChannel,
};

#define DEV_INST(inst)                                              \
    static struct our_driver_data our_driver_data_##inst;           \
    DEVICE_DT_INST_DEFINE(inst,                                     \
        OurDriver_init,                                             \
        NULL,                                                       \
        &our_driver_data_##inst,                                    \
        NULL,                                                       \
        POST_KERNEL,                                                \
        80,                                                         \
        &api_our_driver);


DT_INST_FOREACH_STATUS_OKAY(DEV_INST);


static int OurDriver_init(const struct device *dev)
{
  int rc = 0;
  ARG_UNUSED(rc);
  LOG_INF("Hello from our driver init");
  return rc;
}

static int OurDriver_GetChannel(const struct device *dev, enum sensor_channel channel, struct sensor_value* val)
{
  int rc;
  LOG_INF("Hello from our driver Channel get %d", channel);
  memset(pixels, 0x00, sizeof(pixels));
  rc = led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);
  if (rc) {
           LOG_ERR("couldn't update strip: %d", rc);
          }
 return rc;
}

static int OurDriver_SampleFetch(const struct device *dev, enum sensor_channel channel)
{
  int rc;
  LOG_INF("Hello from our driver sample fetch %d", channel);
  memset(pixels, 0x00, sizeof(pixels));
  memcpy(pixels, &active_color, sizeof(struct led_rgb));
      
 rc = led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);
 if (rc) {
          LOG_ERR("couldn't update strip: %d", rc);
        }
 return rc;
}

int CustomDriver_ParamSet(const struct device *dev, int SetParam)
{
  int rc = 0;
  ARG_UNUSED(rc);
    our_data_t *OurDriver_data = (our_data_t *)dev->data;

    LOG_INF("Hello from set param");
    if (NULL != OurDriver_data)
    {
        OurDriver_data->DriverCounter = SetParam;
        LOG_INF("Setting my_param to %u", SetParam);
    }
    else
    {
        LOG_ERR("Device data is NULL");
    }
 return rc;
}
