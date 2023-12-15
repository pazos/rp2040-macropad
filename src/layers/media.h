#include "layer.h"

class Media: public Layer {
  virtual char const* rgb() {
    return "#ff0000";
  }
  virtual void a0() {
    ConsumerKey(HID_USAGE_CONSUMER_AC_REFRESH);
  }
  virtual void a1() {
    ConsumerKey(HID_USAGE_CONSUMER_AC_BACK);
  }
  virtual void a2() {
    ConsumerKey(HID_USAGE_CONSUMER_AC_BOOKMARKS);
  }
  virtual void a3() {
    ConsumerKey(HID_USAGE_CONSUMER_AC_FORWARD);
  }
  virtual void a4() {
    ConsumerKey(HID_USAGE_CONSUMER_SCAN_PREVIOUS);
  }
  virtual void a5() {
    ConsumerKey(HID_USAGE_CONSUMER_PLAY_PAUSE);
  }
  virtual void a6() {
    ConsumerKey(HID_USAGE_CONSUMER_SCAN_NEXT);
  }
  virtual void a7() {
    Key(HID_KEY_VOLUME_DOWN);
  }
  virtual void a8() {
    ConsumerKey(HID_USAGE_CONSUMER_MUTE);
  }
  virtual void a9() {
    Key(HID_KEY_VOLUME_UP);
  }
  virtual void aPlus() {
    ConsumerKey(HID_USAGE_CONSUMER_BRIGHTNESS_INCREMENT);
  }
  virtual void aMinus() {
    ConsumerKey(HID_USAGE_CONSUMER_BRIGHTNESS_DECREMENT);
  }
  virtual void aMult() {
    ConsumerKey(HID_USAGE_CONSUMER_POWER);
  }
  virtual void aDiv() {
    WriteString("With great power comes great responsability.");
  }
  virtual void aDot() {
    ConsumerKey(HID_USAGE_CONSUMER_AC_HOME);
  }
  virtual void aIntro() {
    Key(HID_KEY_KEYPAD_ENTER);
  }
  virtual void aEsc() {
    Key(HID_KEY_ESCAPE);
  }
};
