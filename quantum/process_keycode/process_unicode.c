#include "process_unicode.h"

static uint8_t input_mode;

uint16_t hex_to_keycode(uint8_t hex)
{
  if (hex == 0x0) {
    return KC_0;
  } else if (hex < 0xA) {
    return KC_1 + (hex - 0x1);
  } else {
    return KC_A + (hex - 0xA);
  }
}

void set_unicode_input_mode(uint8_t os_target)
{
  input_mode = os_target;
}

void unicode_input_start (void) {
  switch(input_mode) {
  case UC_OSX:
    register_code(KC_LALT);
    break;
  case UC_LNX:
    register_code(KC_LCTL);
    register_code(KC_LSFT);
    register_code(KC_U);
    unregister_code(KC_U);
    unregister_code(KC_LSFT);
    unregister_code(KC_LCTL);
    break;
  case UC_WIN:
    register_code(KC_LALT);
    register_code(KC_PPLS);
    unregister_code(KC_PPLS);
    break;
  }
}

void unicode_input_finish (void) {
  switch(input_mode) {
  case UC_OSX:
  case UC_WIN:
    unregister_code(KC_LALT);
    break;
  case UC_LNX:
    register_code(KC_SPC);
    unregister_code(KC_SPC);
    break;
  }
}

void register_hex(uint16_t hex) {
  for(int i = 3; i >= 0; i--) {
    uint8_t digit = ((hex >> (i*4)) & 0xF);
    register_code(hex_to_keycode(digit));
    unregister_code(hex_to_keycode(digit));
  }
}

bool process_unicode(uint16_t keycode, keyrecord_t *record) {
  if (keycode > QK_UNICODE && record->event.pressed) {
    uint16_t unicode = keycode & 0x7FFF;
    unicode_input_start();
    register_hex(unicode);
    unicode_input_finish();
  }
  return true;
}