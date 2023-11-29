#include <pico/stdlib.h>
#include <pico/multicore.h>

#include "usb_host.h"
#include "usb_device.h"

int main(void)
{
  set_sys_clock_khz(120000, true);
  sleep_ms(10);
  multicore_reset_core1();
  multicore_launch_core1(usb_host);
  return usb_device();
}
