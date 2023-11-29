#include <pico/stdlib.h>
#include "layer.h"
#include "usb_device.h"

#define DELAY_MS 5

void Layer::Key(uint8_t code) {
  hid_keyboard_report(0, code);
}

void Layer::AltKey(uint8_t code) {
  hid_keyboard_report(KEYBOARD_MODIFIER_LEFTALT, code);
}

void Layer::CtrlKey(uint8_t code) {
  hid_keyboard_report(KEYBOARD_MODIFIER_LEFTCTRL, code);
}

void Layer::ShiftKey(uint8_t code) {
  hid_keyboard_report(KEYBOARD_MODIFIER_LEFTSHIFT, code);
}

void Layer::SuperKey(uint8_t code) {
  hid_keyboard_report(KEYBOARD_MODIFIER_LEFTGUI, code);
}

void Layer::CtrlAltKey(uint8_t code) {
  hid_keyboard_report(KEYBOARD_MODIFIER_LEFTCTRL
                      | KEYBOARD_MODIFIER_LEFTALT, code);
}

void Layer::CtrlShiftKey(uint8_t code) {
  hid_keyboard_report(KEYBOARD_MODIFIER_LEFTCTRL
                      | KEYBOARD_MODIFIER_LEFTSHIFT, code);
}

void Layer::CtrlSuperKey(uint8_t code) {
  hid_keyboard_report(KEYBOARD_MODIFIER_LEFTCTRL
                      | KEYBOARD_MODIFIER_LEFTGUI, code);
}

void Layer::ConsumerKey(uint16_t code) {
  uint16_t key = code;
  tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &key, 2);
  sleep_ms(DELAY_MS);
  key = 0;
  tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &key, 2);
  sleep_ms(DELAY_MS);
}

void Layer::CtrlChord(uint8_t code1, uint8_t code2, bool releaseFirst) {
  chord(KEYBOARD_MODIFIER_LEFTCTRL, code1, code2, releaseFirst);
}

void Layer::MetaChord(uint8_t code1, uint8_t code2, bool releaseFirst) {
  chord(KEYBOARD_MODIFIER_LEFTALT, code1, code2, releaseFirst);
}

void Layer::ShiftChord(uint8_t code1, uint8_t code2, bool releaseFirst) {
  chord(KEYBOARD_MODIFIER_LEFTSHIFT, code1, code2, releaseFirst);
}

void Layer::SuperChord(uint8_t code1, uint8_t code2, bool releaseFirst) {
  chord(KEYBOARD_MODIFIER_LEFTGUI, code1, code2, releaseFirst);
}

void Layer::WriteString(const char* str) {
  hid_keyboard_write(str);
}

void Layer::chord(uint8_t modifier, uint8_t code1,
                  uint8_t code2, bool releaseFirst) {
  hid_keyboard_report(modifier, code1);
  sleep_ms(DELAY_MS);
  if (releaseFirst)
    hid_keyboard_report(0, code2);
  else
    hid_keyboard_report(modifier, code2);
  sleep_ms(DELAY_MS);
}

void Layer::delay(uint32_t ms) {
  sleep_ms(ms);
}
