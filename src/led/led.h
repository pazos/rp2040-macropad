#include <stdint.h>
#include <hardware/pio.h>

#define WAVESHARE_RP2040_ZERO_RGB_LED 16
#define WAVESHARE_RP2040_ZERO_RGB_FREQ 800000

class RGBLed {

public:
  RGBLed(PIO pio_block, uint state_machine_id);
  void setColor(const char* hex);

private:
  PIO pio;
  uint sm;
  uint offset;
};
