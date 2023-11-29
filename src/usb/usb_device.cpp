#include <cstdarg>
#include <cstdint>
#include <pico/bootrom.h>
#include <pico/multicore.h>
#include <pico/unique_id.h>
#include <tusb.h>

#include "usb_device.h"
#include "usb_host.h"
#include "version.h"

#define USB_PID           0xfaaf
#define USB_VID           0x1209
#define USB_BCD           0x0200

#define EPNUM_HID         0x83
#define EPNUM_CDC_NOTIF   0x81
#define EPNUM_CDC_OUT     0x02
#define EPNUM_CDC_IN      0x82

#ifdef USE_CDC_DEVICE
#define CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN + TUD_HID_DESC_LEN)
#else
#define CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)
#endif

enum {
#ifdef USE_CDC_DEVICE
  ITF_NUM_CDC = 0,
  ITF_NUM_CDC_DATA,
#endif
  ITF_NUM_HID,
  ITF_NUM_TOTAL
};

bool is_dfu_mode = false;
char usb_serial_number[2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1];

tusb_desc_device_t const desc_device = {
  .bLength            = sizeof(tusb_desc_device_t),
  .bDescriptorType    = TUSB_DESC_DEVICE,
  .bcdUSB             = USB_BCD,
  .bDeviceClass       = 0x00,
  .bDeviceSubClass    = 0x00,
  .bDeviceProtocol    = 0x00,
  .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
  .idVendor           = USB_VID,
  .idProduct          = USB_PID,
  .bcdDevice          = 0x0100,
  .iManufacturer      = 0x01,
  .iProduct           = 0x02,
  .iSerialNumber      = 0x03,
  .bNumConfigurations = 0x01
};

char const* string_desc_arr [] = {
  (const char[]) { 0x09, 0x04 },
  "Generic",
  "MacroPad",
  usb_serial_number,
#ifdef USE_CDC_DEVICE
  "MacroPad CDC",
#endif
};

uint8_t const desc_hid_report[] = {
  TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(REPORT_ID_KEYBOARD)),
  TUD_HID_REPORT_DESC_CONSUMER(HID_REPORT_ID(REPORT_ID_CONSUMER_CONTROL))
};

uint8_t const desc_fs_configuration[] = {
  TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 500),
#ifdef USE_CDC_DEVICE
  TUD_CDC_DESCRIPTOR(ITF_NUM_CDC, 4, EPNUM_CDC_NOTIF, 8, EPNUM_CDC_OUT, EPNUM_CDC_IN, 64),
#endif
  TUD_HID_DESCRIPTOR(ITF_NUM_HID, 0, HID_ITF_PROTOCOL_NONE, sizeof(desc_hid_report), EPNUM_HID, CFG_TUD_HID_EP_BUFSIZE, 5)
};

uint8_t const * tud_descriptor_device_cb(void) {
  return (uint8_t const *) &desc_device;
}

uint8_t const * tud_descriptor_configuration_cb(uint8_t index) {
  (void) index;
  return desc_fs_configuration;
}

static uint16_t _desc_str[32];

uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
  (void) langid;
  uint8_t chr_count;
  if ( index == 0) {
    memcpy(&_desc_str[1], string_desc_arr[0], 2);
    chr_count = 1;
  } else {
    if (index == 3)
      pico_get_unique_board_id_string(usb_serial_number, sizeof(usb_serial_number));

    if ( !(index < sizeof(string_desc_arr)/sizeof(string_desc_arr[0])) ) return NULL;
    const char* str = string_desc_arr[index];
    chr_count = (uint8_t) strlen(str);
    if ( chr_count > 31 ) chr_count = 31;
    for(uint8_t i=0; i<chr_count; i++) {
      _desc_str[1+i] = str[i];
    }
  }
  _desc_str[0] = (TUSB_DESC_STRING << 8 ) | (2*chr_count + 2);
  return _desc_str;
}

uint8_t const * tud_hid_descriptor_report_cb(uint8_t itf) {
  (void) itf;
  return desc_hid_report;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) bufsize;
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;
  return 0;
}

void tud_cdc_rx_cb(uint8_t itf) {
#ifndef USE_CDC_DEVICE
  (void) itf;
#else
  if (tud_cdc_connected()) {
    if (tud_cdc_available()) {
      char buf[64] = {0};
      uint32_t count;
      count = tud_cdc_n_read(itf, buf, sizeof(buf));
      if (count >= 2) {
        usb_device_cdc_cb(buf);
      }
    }
  }
#endif
}

void cdc_tx(const char* format, ...) {
#ifndef USE_CDC_DEVICE
  (void) format;
#else
  char buf[1024];
  va_list args;
  va_start(args, format);
  int count = vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);
  tud_cdc_write(buf, count);
  tud_cdc_write_flush();
#endif
}

void hid_keyboard_report(uint8_t modifier, uint8_t code) {
  uint8_t keycode[6] = {0};
  keycode[0] = code;
  tud_hid_keyboard_report(REPORT_ID_KEYBOARD, modifier, keycode);
}

void hid_keyboard_write(const char * msg) {
  int i = 0;
  uint8_t const conv_table[128][2] = { HID_ASCII_TO_KEYCODE };
  while (msg[i] != '\0') {
    uint8_t modifier = 0;
    uint8_t key = msg[i];
    if (conv_table[key][0])
      modifier = KEYBOARD_MODIFIER_LEFTSHIFT;
    hid_keyboard_report(modifier, conv_table[key][1]);
    sleep_ms(5);
    hid_keyboard_report(0, 0);
    sleep_ms(5);
    i++;
  }
}

void enter_dfu() {
  is_dfu_mode = true;
}

int usb_device() {
  tud_init(0);
  sleep_ms(10);
  while (!is_dfu_mode) {
    tud_task();
  }
  multicore_reset_core1();
  reset_usb_boot(0, 0);
  return 0;
}
