#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <esp_rom_sys.h>

void board_early_init_hook (void)
{
    /*pre main init*/
    esp_rom_printf("Board Initialized\n");
}
