/*
 * hidcodes.h
 *
 *  Created on: Mar 7, 2025
 *      Author: bartosz
 */

#ifndef LIB_HIDCODES_H_
#define LIB_HIDCODES_H_

//special
#define HID_SPECIAL_FN				0xff
#define HID_SPECIAL_WHEELUP			1
#define HID_SPECIAL_WHEELDN			-1
#define HID_SPECIAL_DPI				0xfe
#define HID_SPECIAL_PRC				0xfd
#define HID_SPECIAL_HOME			0xfc


//mouse mask
#define HID_MOUSE_MASK_L			1<<0
#define HID_MOUSE_MASK_R			1<<1
#define HID_MOUSE_MASK_M			1<<2
#define HID_MOUSE_MASK_FWD			1<<3
#define HID_MOUSE_MASK_BCK			1<<4

//modifiers mask
#define HID_MOD_MASK_LCTRL			1<<0
#define HID_MOD_MASK_LSHIFT			1<<1
#define HID_MOD_MASK_LALT			1<<2
#define HID_MOD_MASK_LGUI			1<<3
#define HID_MOD_MASK_RCTRL			1<<4
#define HID_MOD_MASK_RSHIFT			1<<5
#define HID_MOD_MASK_RALT			1<<6
#define HID_MOD_MASK_RGUI			1<<7

//mouse bit shifts
#define HID_MOUSE_BITSHIFT_L			0
#define HID_MOUSE_BITSHIFT_R			1
#define HID_MOUSE_BITSHIFT_M			2
#define HID_MOUSE_BITSHIFT_FWD			3
#define HID_MOUSE_BITSHIFT_BCK			4

//modifiers bit shifts
#define HID_MOD_BITSHIFT_LCTRL			0
#define HID_MOD_BITSHIFT_LSHIFT			1
#define HID_MOD_BITSHIFT_LALT			2
#define HID_MOD_BITSHIFT_LGUI			3
#define HID_MOD_BITSHIFT_RCTRL			4
#define HID_MOD_BITSHIFT_RSHIFT			5
#define HID_MOD_BITSHIFT_RALT			6
#define HID_MOD_BITSHIFT_RGUI			7

//alphanumeric
#define HID_KEY_A               0x04
#define HID_KEY_B               0x05
#define HID_KEY_C               0x06
#define HID_KEY_D               0x07
#define HID_KEY_E               0x08
#define HID_KEY_F               0x09
#define HID_KEY_G               0x0a
#define HID_KEY_H               0x0b
#define HID_KEY_I               0x0c
#define HID_KEY_J               0x0d
#define HID_KEY_K               0x0e
#define HID_KEY_L               0x0f
#define HID_KEY_M               0x10
#define HID_KEY_N               0x11
#define HID_KEY_O               0x12
#define HID_KEY_P               0x13
#define HID_KEY_Q               0x14
#define HID_KEY_R               0x15
#define HID_KEY_S               0x16
#define HID_KEY_T               0x17
#define HID_KEY_U               0x18
#define HID_KEY_V               0x19
#define HID_KEY_W               0x1a
#define HID_KEY_X               0x1b
#define HID_KEY_Y               0x1c
#define HID_KEY_Z               0x1d
#define HID_KEY_1               0x1e
#define HID_KEY_2               0x1f
#define HID_KEY_3               0x20
#define HID_KEY_4               0x21
#define HID_KEY_5               0x22
#define HID_KEY_6               0x23
#define HID_KEY_7               0x24
#define HID_KEY_8               0x25
#define HID_KEY_9               0x26
#define HID_KEY_0               0x27
#define HID_KEY_ENTER           0x28
#define HID_KEY_ESC             0x29
#define HID_KEY_BACKSPACE       0x2a
#define HID_KEY_TAB             0x2b
#define HID_KEY_SPACE           0x2c
#define HID_KEY_DASH            0x2d
#define HID_KEY_EQUAL           0x2e
#define HID_KEY_BRACKETL        0x2f
#define HID_KEY_BRACKETR        0x30
#define HID_KEY_BACKSLASH       0x31
#define HID_KEY_NUS_HASH		0x32
#define HID_KEY_SEMICO          0x33
#define HID_KEY_APOSTROPHE      0x34
#define HID_KEY_TILDE           0x35
#define HID_KEY_COMMA           0x36
#define HID_KEY_DOT             0x37
#define HID_KEY_SLASH           0x38
#define HID_KEY_CAPS            0x39
#define HID_KEY_F1              0x3a
#define HID_KEY_F2              0x3b
#define HID_KEY_F3              0x3c
#define HID_KEY_F4              0x3d
#define HID_KEY_F5              0x3e
#define HID_KEY_F6              0x3f
#define HID_KEY_F7              0x40
#define HID_KEY_F8              0x41
#define HID_KEY_F9              0x42
#define HID_KEY_F10             0x43
#define HID_KEY_F11             0x44
#define HID_KEY_F12             0x45
#define HID_KEY_PRTSC           0x46
#define HID_KEY_SCROLL_LOCK		0x47
#define HID_KEY_PAUSE           0x48
#define HID_KEY_INSERT          0x49
#define HID_KEY_HOME            0x4a
#define HID_KEY_PGUP            0x4b
#define HID_KEY_DEL             0x4c
#define HID_KEY_END             0x4d
#define HID_KEY_PGDN            0x4e
#define HID_KEY_RIGHT           0x4f
#define HID_KEY_LEFT            0x50
#define HID_KEY_DOWN            0x51
#define HID_KEY_UP              0x52

//numpad
#define HID_NUMPAD_NUMLOCK      0x53
#define HID_NUMPAD_SLASH        0x54
#define HID_NUMPAD_MULTIPLY     0x55
#define HID_NUMPAD_MINUS        0x56
#define HID_NUMPAD_PLUS         0x57
#define HID_NUMPAD_ENTER        0x58
#define HID_NUMPAD_1            0x59
#define HID_NUMPAD_2            0x5a
#define HID_NUMPAD_3            0x5b
#define HID_NUMPAD_4            0x5c
#define HID_NUMPAD_5            0x5d
#define HID_NUMPAD_6            0x5e
#define HID_NUMPAD_7            0x5f
#define HID_NUMPAD_8            0x60
#define HID_NUMPAD_9            0x61
#define HID_NUMPAD_0            0x62
#define HID_NUMPAD_DOT          0x63
#define HID_NUMPAD_NUS_BACKSL	0x64

//modifiers
#define HID_MOD_LCTRL			0xe0
#define HID_MOD_LSHIFT			0xe1
#define HID_MOD_LALT			0xe2
#define HID_MOD_LGUI			0xe3
#define HID_MOD_RCTRL			0xe4
#define HID_MOD_RSHIFT			0xe5
#define HID_MOD_RALT			0xe6
#define HID_MOD_RGUI			0xe7



#endif /* LIB_HIDCODES_H_ */
