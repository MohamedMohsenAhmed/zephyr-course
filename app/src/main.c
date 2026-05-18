#include <errno.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include "../Drivers/OurDriver/our_driver.h"

#define DELAY_TIME K_MSEC(CONFIG_SAMPLE_LED_UPDATE_DELAY)
#define DELAY_TIME_1 K_MSEC(CONFIG_APP_HEARTBEAT_PERIOD_MS)

#define LOG_LEVEL 4
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);
    
int main(void)
{
    int return_0 = 0;
    int return_1 = 0;
    
    static int Counterinst0 = 0;
    static int Counterinst1 = 0;
    
    const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
    const struct device* driver1 = DEVICE_DT_GET(DT_NODELABEL(our_driver1));

    struct sensor_value val;

    while (1)
    {
        LOG_INF("set param counter to instance 0"); 
        Counterinst0 ++;
        CustomDriver_ParamSet(driver,Counterinst0);
        LOG_INF("set param counter to instance 1");
        Counterinst1 ++;
        CustomDriver_ParamSet(driver1,Counterinst1);
        return_0 = sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
        k_sleep(DELAY_TIME_1); 
        return_1 = sensor_sample_fetch(driver);
        k_sleep(DELAY_TIME_1);
    }
    return 0;
}
