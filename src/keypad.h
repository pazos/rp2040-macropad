#include "empty.h"

enum Keys {
  KEY_NONE = 0,
  KEY_ESC = 41,
  KEY_TAB,
  KEY_BACKSPACE,
  KEY_BLOQ_NUM = 83,
  KEY_DIVIDE,
  KEY_MULTIPLY,
  KEY_SUBTRACT,
  KEY_ADD,
  KEY_INTRO,
  KEY_1,
  KEY_2,
  KEY_3,
  KEY_4,
  KEY_5,
  KEY_6,
  KEY_7,
  KEY_8,
  KEY_9,
  KEY_0,
  KEY_DOT,
};

class Keypad {
public:
  void on_cdc_rx(const char* s);
  void on_hid_mount(uint16_t vid, uint16_t pid);
  void on_hid_umount();
  void on_hid_report(uint8_t keycode);

private:
  void macro(uint8_t keycode);
  void next_layer();
  void previous_layer();
  void toggle_layer();
  void toggle_numeric();
  void set_color(const char* color);

private:
  int current_layer = 0;
  bool numeric_mode = true;

  Empty empty;

  Layer* const layers[1] = {
  };

  uint8_t layer_count = ((sizeof(layers) / sizeof(layers[0])) - 1);
};
