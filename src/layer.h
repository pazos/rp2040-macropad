#pragma once

#include <stdint.h>
#include <tusb.h>

class Layer {
public:
  // color code
  virtual char const* rgb() = 0;

  // actions
  virtual void a0() = 0;
  virtual void a1() = 0;
  virtual void a2() = 0;
  virtual void a3() = 0;
  virtual void a4() = 0;
  virtual void a5() = 0;
  virtual void a6() = 0;
  virtual void a7() = 0;
  virtual void a8() = 0;
  virtual void a9() = 0;
  virtual void aPlus() = 0;
  virtual void aMinus() = 0;
  virtual void aMult() = 0;
  virtual void aDiv() = 0;
  virtual void aDot() = 0;
  virtual void aIntro() = 0;
  virtual void aEsc() = 0;

protected:
  void delay(uint32_t ms);

  // send single key event with modifiers
  void Key(uint8_t code);
  void AltKey(uint8_t code);
  void CtrlKey(uint8_t code);
  void ShiftKey(uint8_t code);
  void SuperKey(uint8_t code);
  void CtrlAltKey(uint8_t code);
  void CtrlShiftKey(uint8_t code);
  void CtrlSuperKey(uint8_t code);

  // send single consumer event
  void ConsumerKey(uint16_t code);

  // send emacs-like key chords (ie: C-x C-f or C-x b)
  void CtrlChord(uint8_t code1, uint8_t code2, bool releaseFirst = false);
  void MetaChord(uint8_t code1, uint8_t code2, bool releaseFirst = false);
  void ShiftChord(uint8_t code1, uint8_t code2, bool releaseFirst = false);
  void SuperChord(uint8_t code1, uint8_t code2, bool releaseFirst = false);

  /* translate a string to a sequence of hid reports, limited to 0-127 ascii characters.
     See https://github.com/hathach/tinyusb/blob/master/src/class/hid/hid.h */
  void WriteString(const char* str);

private:
  void chord(uint8_t modifier, uint8_t code1, uint8_t code2, bool releaseFirst);
};
