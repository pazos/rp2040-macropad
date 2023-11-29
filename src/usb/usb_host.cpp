#include <pico/stdlib.h>
#include <pio_usb.h>
#include <tusb.h>

#include "usb_host.h"
#include "usb_device.h"
#include "keypad.h"


Keypad keypad;

void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance,
                      uint8_t const* desc_report, uint16_t desc_len) {
  (void)desc_report;
  (void)desc_len;

  uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);
  uint16_t vid, pid;
  tuh_vid_pid_get(dev_addr, &vid, &pid);
  if (itf_protocol == HID_ITF_PROTOCOL_KEYBOARD) {
    keypad.on_hid_mount(vid, pid);
    tuh_hid_receive_report(dev_addr, instance);
  }
}

void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
  uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);
  if (itf_protocol == HID_ITF_PROTOCOL_KEYBOARD)
    keypad.on_hid_umount();
}

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance,
                                uint8_t const* report, uint16_t len) {
  (void) len;

  uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);
  switch(itf_protocol) {
  case HID_ITF_PROTOCOL_KEYBOARD:
    {
      const hid_keyboard_report_t* r = (hid_keyboard_report_t const*) report;
      keypad.on_hid_report(r->keycode[0]);
    }
    break;
  default:
    break;
  }
  tuh_hid_receive_report(dev_addr, instance);
}

void usb_device_cdc_cb(const char* s) {
  keypad.on_cdc_rx(s);
}

void usb_host() {
  sleep_ms(10);
  pio_usb_configuration_t pio_cfg = PIO_USB_DEFAULT_CONFIG;
  tuh_configure(1, TUH_CFGID_RPI_PIO_USB_CONFIGURATION, &pio_cfg);
  tuh_init(1);
  while (true) {
    tuh_task();
  }
}
