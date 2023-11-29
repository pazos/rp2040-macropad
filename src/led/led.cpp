#include <cstdio>
#include <hardware/clocks.h>

#include "led.h"
#include "ws2812.pio.h"

RGBLed::RGBLed(PIO pio_block, uint state_machine_id) {
  pio = pio_block;
  sm = state_machine_id;
  offset = pio_add_program(pio, &ws2812_program);
  ws2812_program_init(pio, sm, offset,
                      WAVESHARE_RP2040_ZERO_RGB_LED,
                      WAVESHARE_RP2040_ZERO_RGB_FREQ, true);
  setColor("#000000");
}

void RGBLed::setColor(const char* hex) {
  hex++;
  uint r, g, b;
  sscanf(hex, "%02x%02x%02x", &r, &g, &b);
  uint32_t mask = (g << 16) | (r << 8) | (b << 0);
  pio_sm_put_blocking(pio, sm, mask << 8u);
}
