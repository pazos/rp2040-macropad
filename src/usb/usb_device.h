#include <stdint.h>

enum {
  REPORT_ID_KEYBOARD = 1,
  REPORT_ID_MOUSE,
  REPORT_ID_CONSUMER_CONTROL,
  REPORT_ID_GAMEPAD,
  REPORT_ID_COUNT
};

extern void cdc_tx(const char* format, ...);
extern void hid_keyboard_report(uint8_t modifier, uint8_t code);
extern void hid_keyboard_write(const char * msg);
extern void enter_dfu();
extern int usb_device();
