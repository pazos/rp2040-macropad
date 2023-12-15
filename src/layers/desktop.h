#include "layer.h"

class Desktop : public Layer {
  virtual char const* rgb() {
    return "#00ff00";
  }
  virtual void a0() {
    Key(HID_KEY_F13);
  }
  virtual void a1() {
    Key(HID_KEY_F14);
  }
  virtual void a2() {
    Key(HID_KEY_F15);
  }
  virtual void a3() {
    Key(HID_KEY_F16);
  }
  virtual void a4() {
    Key(HID_KEY_F17);
  }
  virtual void a5() {
    Key(HID_KEY_F18);
  }
  virtual void a6() {
    Key(HID_KEY_F19);
  }
  virtual void a7() {
    Key(HID_KEY_F20);
  }
  virtual void a8() {
    Key(HID_KEY_F21);
  }
  virtual void a9() {
    Key(HID_KEY_F22);
  }
  virtual void aPlus() {
    Key(HID_KEY_F23);
  }
  virtual void aMinus() {
    Key(HID_KEY_F24);
  }
  virtual void aMult() {
    CtrlKey(HID_KEY_F13);
  }
  virtual void aDiv() {
    CtrlKey(HID_KEY_F14);
  }
  virtual void aDot() {
    CtrlKey(HID_KEY_F15);
  }
  virtual void aIntro() {
    CtrlKey(HID_KEY_F16);
  }
  virtual void aEsc() {
    Key(HID_KEY_ESCAPE);
  }
};
