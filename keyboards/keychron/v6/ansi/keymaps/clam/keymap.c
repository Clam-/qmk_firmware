/* I guess?? I dunno how GPL works...
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "keycode.h"
#include QMK_KEYBOARD_H

// clang-format off

enum layers{
  MAC_BASE,
  MAC_FN,
  WIN_BASE,
  WIN_FN
};

// state for macro related events
// playback state is pointless at the moment, since there is no 'end' playback callback
// not sure if there ever will be either, since it looks like macro playback is done synchronously all at once.
typedef enum {
  STATE_DM_REC1 = 1,
  STATE_DM_REC2 = 2,
  STATE_DM_PLY1 = 4,
  STATE_DM_PLY2 = 8
} recstate;
recstate REC_STATE = 0;

#define KC_TASK LGUI(KC_TAB)
#define KC_FLXP LGUI(KC_E)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_ansi_108(
        KC_ESC,             KC_BRID,  KC_BRIU,  KC_NO,    KC_NO,    RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  KC_NO,    KC_NO,    RGB_MOD,  KC_F13,   KC_F14,   KC_F15,   KC_F16,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,     KC_BSPC,  KC_INS,   KC_HOME,  KC_PGUP,  KC_NUM,   KC_PSLS,  KC_PAST,  KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,    KC_BSLS,  KC_DEL,   KC_END,   KC_PGDN,  KC_P7,    KC_P8,    KC_P9,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,                                 KC_P4,    KC_P5,    KC_P6,    KC_PPLS,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,            KC_UP,              KC_P1,    KC_P2,    KC_P3,
        KC_LCTL,  KC_LOPT,  KC_LCMD,                                KC_SPC,                                 KC_RCMD,  KC_ROPT,  MO(MAC_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_P0,              KC_PDOT,  KC_PENT),
    [MAC_FN] = LAYOUT_ansi_108(
        _______,            KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,   _______,  _______,  RGB_TOG,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,              _______,                                _______,  _______,  _______,  _______,
        _______,            _______,  _______,  _______,  _______,  _______,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,            _______,            _______,  _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,    _______,  _______,  _______,  _______,  _______,            _______,  _______),
    [WIN_BASE] = LAYOUT_ansi_108(
        KC_ESC,             KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,   KC_PSCR,  KC_SCRL,  KC_PAUSE, DM_RSTP,  _______,  _______,  KC_CALC,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,     KC_BSPC,  KC_INS,   KC_HOME,  KC_PGUP,  KC_NUM,   KC_PSLS,  KC_PAST,  KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,    KC_BSLS,  KC_DEL,   KC_END,   KC_PGDN,  KC_P7,    KC_P8,    KC_P9,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,                                 KC_P4,    KC_P5,    KC_P6,    KC_PPLS,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,            KC_UP,              KC_P1,    KC_P2,    KC_P3,
        KC_LCTL,  KC_LWIN,  KC_LALT,                                KC_SPC,                                 KC_RALT,  MO(WIN_FN), KC_APP,   KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_P0,              KC_PDOT,  KC_PENT),
    [WIN_FN] = LAYOUT_ansi_108(
        _______,            KC_BRID,  KC_BRIU,  KC_TASK,  KC_FLXP,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  _______,  _______,  RGB_TOG,  DM_REC1,  DM_REC2,  DM_PLY1,  DM_PLY2,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,              _______,                                _______,  _______,  _______,  _______,
        _______,            _______,  _______,  _______,  _______,  _______,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,            _______,            _______,  _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,    _______,  _______,  _______,  _______,  _______,            _______,  _______),
};

// Handle scroll lock toggle RGB show and macro recording (like num lock and caps lock are in the base v6 implementation).
// https://github.com/qmk/qmk_firmware/blob/81a3aa025cda52732e847af8db256cb132605ce0/keyboards/keychron/v6/v6.c#L48
#if defined(RGB_MATRIX_ENABLE) && (\
  defined(SCROLL_LOCK_LED_INDEX) || (defined(DM_REC1_LED_INDEX) && defined(DM_REC2_LED_INDEX))\
  || (defined(DM_PLY1_LED_INDEX) && defined(DM_PLY2_LED_INDEX)))
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
# if defined(SCROLL_LOCK_LED_INDEX)
  if (host_keyboard_led_state().scroll_lock) {
        RGB_MATRIX_INDICATOR_SET_COLOR(SCROLL_LOCK_LED_INDEX, 255, 255, 255);
    } else {
        if (!rgb_matrix_get_flags()) {
            RGB_MATRIX_INDICATOR_SET_COLOR(SCROLL_LOCK_LED_INDEX, 0, 0, 0);
        }
    }
# endif // SCROLL_LOCK_LED_INDEX
// Handle displaying of macro recording indication
# if defined(DM_REC1_LED_INDEX)
  if (REC_STATE & STATE_DM_REC1) {
        RGB_MATRIX_INDICATOR_SET_COLOR(DM_REC1_LED_INDEX, 255, 0, 0);
  } else if (REC_STATE & STATE_DM_REC2) {
    RGB_MATRIX_INDICATOR_SET_COLOR(DM_REC2_LED_INDEX, 255, 0, 0);
  } else {
    if (!rgb_matrix_get_flags()) {
        RGB_MATRIX_INDICATOR_SET_COLOR(DM_REC1_LED_INDEX, 0, 0, 0);
        RGB_MATRIX_INDICATOR_SET_COLOR(DM_REC2_LED_INDEX, 0, 0, 0);
    }
  }
#  endif // DM_REC1_LED_INDEX
# if defined(DM_PLY1_LED_INDEX)
if (REC_STATE & STATE_DM_PLY1) {
  RGB_MATRIX_INDICATOR_SET_COLOR(DM_PLY1_LED_INDEX, 34, 168, 240);
} else if (REC_STATE & STATE_DM_PLY2) {
  RGB_MATRIX_INDICATOR_SET_COLOR(DM_PLY2_LED_INDEX, 34, 168, 240);
} else {
  if (!rgb_matrix_get_flags()) {
      RGB_MATRIX_INDICATOR_SET_COLOR(DM_PLY1_LED_INDEX, 0, 0, 0);
      RGB_MATRIX_INDICATOR_SET_COLOR(DM_PLY2_LED_INDEX, 0, 0, 0);
  }
}
#  endif // DM_PLY1_LED_INDEX
    return true;
}
#endif // RGB_MATRIX_ENABLE...

// Handle setting the state for macro related events
#if defined(RGB_MATRIX_ENABLE) && defined(DM_REC1_LED_INDEX) && defined(DM_REC2_LED_INDEX)
void dynamic_macro_record_start_user(int8_t direction) {
  if (direction == 1)
    REC_STATE |= STATE_DM_REC1;
  else if (direction == -1)
    REC_STATE |= STATE_DM_REC2;
  else
    REC_STATE &= ~(STATE_DM_REC1|STATE_DM_REC2);
}
void dynamic_macro_record_end_user(int8_t direction) {
  if (direction == 1)
    REC_STATE &= ~STATE_DM_REC1;
  else if (direction == -1)
    REC_STATE &= ~STATE_DM_REC2;
}
#endif // RGB_MATRIX_ENABLE...

// This is pointless as there is no "end" event for macro playback...
// #if defined(RGB_MATRIX_ENABLE) && defined(DM_PLY1_LED_INDEX) && defined(DM_PLY2_LED_INDEX)
// void dynamic_macro_play_user(int8_t direction) {
//   if (direction == 1)
//     REC_STATE |= STATE_DM_PLY1;
//   else if (direction == -1)
//     REC_STATE |= STATE_DM_PLY2;
//   else
//     REC_STATE &= ~(STATE_DM_PLY1|STATE_DM_PLY2);
// }
// #endif // RGB_MATRIX_ENABLE...
