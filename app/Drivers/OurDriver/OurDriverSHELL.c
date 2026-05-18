#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>



int cmd_read_value(const struct shell *shell, size_t argc, char **argv)
{
struct sensor_value val;
int return_0;
const struct device* driver = shell_device_get_binding("our_driver0");
    shell_info(shell, "From get value command");
    shell_warn(shell, "This will return the last value read by the driver, not a new reading");
    return_0 = sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
    return 0;
}

int cmd_fetch_value(const struct shell *shell, size_t argc, char **argv)
{
int return_1;
const struct device* driver = shell_device_get_binding("our_driver0");
    shell_info(shell, "From fetch value command");
    return_1 = sensor_sample_fetch(driver);
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_OurDriver,
    SHELL_CMD_ARG(read, NULL, "Read the latest value from the driver", cmd_read_value, 1, 0),
    SHELL_CMD_ARG(fetch, NULL, "Fetch the latest value from the driver", cmd_fetch_value, 1, 0),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(OurDriver, &sub_OurDriver, "Commands to interact with our driver", NULL);