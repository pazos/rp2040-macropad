#include "keypad.h"
#include "usb_device.h"
#include "version.h"

#ifdef USE_RGB_LED
#include "led.h"
RGBLed led(pio0, 2);
#endif

#define RGB_DFU       "#ffa500"
#define RGB_NUMERIC   "#98b6e4"
#define RGB_NO_DEVICE "#000000"

struct Device {
  bool mounted = false;
  uint vid = 0;
  uint pid = 0;
} host;

const char* status = R"(
%s %s.%s.%s

USB host: %s

USB device:
  Numeric mode: %b
  Current layer: %d

)";

char* host_status() {
  static char buf[40];
  buf[0] = '\0';
  if (host.mounted)
    snprintf(buf, sizeof(buf), "OK!\n  VID: %04x\n  PID: %04x",
             host.vid, host.pid);
  else
    snprintf(buf, sizeof(buf), "Not found!");
  return buf;
}

void Keypad::on_cdc_rx(const char* s) {
#ifdef USE_CDC_DEVICE
  if (strcmp(s, "dfu") == 0) {
    set_color(RGB_DFU);
    enter_dfu();
  } else if (strcmp(s, "info") == 0) {
    cdc_tx(status, PROJECT_NAME, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH,
           host_status(), numeric_mode, current_layer + 1);
  }
#else
  (void) s;
#endif
}

void Keypad::on_hid_mount(uint16_t vid, uint16_t pid) {
  host = { true, vid, pid };
  numeric_mode = true;
  set_color(RGB_NUMERIC);
}

void Keypad::on_hid_umount() {
  host = { false, 0, 0 };
  set_color(RGB_NO_DEVICE);
}

void Keypad::on_hid_report(uint8_t keycode) {
  if (numeric_mode) {
    switch(keycode) {
    case KEY_BLOQ_NUM:
      toggle_layer();
      break;
    default:
      hid_keyboard_report(0, keycode);
      break;
    }
  } else {
    macro(keycode);
  }
}

void Keypad::macro(uint8_t keycode) {
  switch(keycode)
    {
    case KEY_NONE:
      hid_keyboard_report(0, 0);
      break;
    case KEY_BLOQ_NUM:
      toggle_layer();
      break;
    case KEY_DIVIDE:
      layers[current_layer]->aDiv();
      break;
    case KEY_MULTIPLY:
      layers[current_layer]->aMult();
      break;
    case KEY_SUBTRACT:
      layers[current_layer]->aMinus();
      break;
    case KEY_ADD:
      layers[current_layer]->aPlus();
      break;
    case KEY_INTRO:
      layers[current_layer]->aIntro();
      break;
    case KEY_1:
      layers[current_layer]->a1();
      break;
    case KEY_2:
      layers[current_layer]->a2();
      break;
    case KEY_3:
      layers[current_layer]->a3();
      break;
    case KEY_4:
      layers[current_layer]->a4();
      break;
    case KEY_5:
      layers[current_layer]->a5();
      break;
    case KEY_6:
      layers[current_layer]->a6();
      break;
    case KEY_7:
      layers[current_layer]->a7();
      break;
    case KEY_8:
      layers[current_layer]->a8();
      break;
    case KEY_9:
      layers[current_layer]->a9();
      break;
    case KEY_0:
      layers[current_layer]->a0();
      break;
    case KEY_DOT:
      layers[current_layer]->aDot();
      break;
#ifdef USE_EXTRA_KEYS_EPOMAKER_TH21
    case KEY_ESC:
      layers[current_layer]->aEsc();
      break;
    case KEY_TAB:
      next_layer();
      break;
    case KEY_BACKSPACE:
      previous_layer();
      break;
#endif
    default:
#ifdef USE_CDC_DEVICE
      cdc_tx("Unregistered key: %d\n", keycode);
#endif
      break;
    }
}

void Keypad::next_layer() {
  if (current_layer < layer_count)
    current_layer++;
  else
    current_layer = 0;

  set_color(layers[current_layer]->rgb());
}

void Keypad::previous_layer() {
  if (current_layer > 0)
    current_layer--;
  else
    current_layer = layer_count;

  set_color(layers[current_layer]->rgb());
}

void Keypad::toggle_layer() {
#ifdef USE_EXTRA_KEYS_EPOMAKER_TH21
  toggle_numeric();
#else
  if (numeric_mode) {
    toggle_numeric();
  } else if (current_layer == layer_count) {
    current_layer = 0;
    toggle_numeric();
  } else {
    next_layer();
  }
#endif
}

void Keypad::toggle_numeric() {
  numeric_mode = !numeric_mode;
  if (numeric_mode)
    set_color(RGB_NUMERIC);
  else
    set_color(layers[current_layer]->rgb());
}

void Keypad::set_color(const char* color) {
#ifndef USE_RGB_LED
  (void) color;
#else
  led.setColor(color);
#endif
}
