#pragma once

#define EDEN_KEY_SPACE              32
#define EDEN_KEY_APOSTROPHE         39  /* ' */
#define EDEN_KEY_COMMA              44  /* , */
#define EDEN_KEY_MINUS              45  /* - */
#define EDEN_KEY_PERIOD             46  /* . */
#define EDEN_KEY_SLASH              47  /* / */
#define EDEN_KEY_0                  48
#define EDEN_KEY_1                  49
#define EDEN_KEY_2                  50
#define EDEN_KEY_3                  51
#define EDEN_KEY_4                  52
#define EDEN_KEY_5                  53
#define EDEN_KEY_6                  54
#define EDEN_KEY_7                  55
#define EDEN_KEY_8                  56
#define EDEN_KEY_9                  57
#define EDEN_KEY_SEMICOLON          59  /* ; */
#define EDEN_KEY_EQUAL              61  /* = */
#define EDEN_KEY_A                  65
#define EDEN_KEY_B                  66
#define EDEN_KEY_C                  67
#define EDEN_KEY_D                  68
#define EDEN_KEY_E                  69
#define EDEN_KEY_F                  70
#define EDEN_KEY_G                  71
#define EDEN_KEY_H                  72
#define EDEN_KEY_I                  73
#define EDEN_KEY_J                  74
#define EDEN_KEY_K                  75
#define EDEN_KEY_L                  76
#define EDEN_KEY_M                  77
#define EDEN_KEY_N                  78
#define EDEN_KEY_O                  79
#define EDEN_KEY_P                  80
#define EDEN_KEY_Q                  81
#define EDEN_KEY_R                  82
#define EDEN_KEY_S                  83
#define EDEN_KEY_T                  84
#define EDEN_KEY_U                  85
#define EDEN_KEY_V                  86
#define EDEN_KEY_W                  87
#define EDEN_KEY_X                  88
#define EDEN_KEY_Y                  89
#define EDEN_KEY_Z                  90
#define EDEN_KEY_LEFT_BRACKET       91  /* [ */
#define EDEN_KEY_BACKSLASH          92  /* \ */
#define EDEN_KEY_RIGHT_BRACKET      93  /* ] */
#define EDEN_KEY_GRAVE_ACCENT       96  /* ` */
#define EDEN_KEY_WORLD_1            161 /* non-US #1 */
#define EDEN_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define EDEN_KEY_ESCAPE             256
#define EDEN_KEY_ENTER              257
#define EDEN_KEY_TAB                258
#define EDEN_KEY_BACKSPACE          259
#define EDEN_KEY_INSERT             260
#define EDEN_KEY_DELETE             261
#define EDEN_KEY_RIGHT              262
#define EDEN_KEY_LEFT               263
#define EDEN_KEY_DOWN               264
#define EDEN_KEY_UP                 265
#define EDEN_KEY_PAGE_UP            266
#define EDEN_KEY_PAGE_DOWN          267
#define EDEN_KEY_HOME               268
#define EDEN_KEY_END                269
#define EDEN_KEY_CAPS_LOCK          280
#define EDEN_KEY_SCROLL_LOCK        281
#define EDEN_KEY_NUM_LOCK           282
#define EDEN_KEY_PRINT_SCREEN       283
#define EDEN_KEY_PAUSE              284
#define EDEN_KEY_F1                 290
#define EDEN_KEY_F2                 291
#define EDEN_KEY_F3                 292
#define EDEN_KEY_F4                 293
#define EDEN_KEY_F5                 294
#define EDEN_KEY_F6                 295
#define EDEN_KEY_F7                 296
#define EDEN_KEY_F8                 297
#define EDEN_KEY_F9                 298
#define EDEN_KEY_F10                299
#define EDEN_KEY_F11                300
#define EDEN_KEY_F12                301
#define EDEN_KEY_F13                302
#define EDEN_KEY_F14                303
#define EDEN_KEY_F15                304
#define EDEN_KEY_F16                305
#define EDEN_KEY_F17                306
#define EDEN_KEY_F18                307
#define EDEN_KEY_F19                308
#define EDEN_KEY_F20                309
#define EDEN_KEY_F21                310
#define EDEN_KEY_F22                311
#define EDEN_KEY_F23                312
#define EDEN_KEY_F24                313
#define EDEN_KEY_F25                314
#define EDEN_KEY_KP_0               320
#define EDEN_KEY_KP_1               321
#define EDEN_KEY_KP_2               322
#define EDEN_KEY_KP_3               323
#define EDEN_KEY_KP_4               324
#define EDEN_KEY_KP_5               325
#define EDEN_KEY_KP_6               326
#define EDEN_KEY_KP_7               327
#define EDEN_KEY_KP_8               328
#define EDEN_KEY_KP_9               329
#define EDEN_KEY_KP_DECIMAL         330
#define EDEN_KEY_KP_DIVIDE          331
#define EDEN_KEY_KP_MULTIPLY        332
#define EDEN_KEY_KP_SUBTRACT        333
#define EDEN_KEY_KP_ADD             334
#define EDEN_KEY_KP_ENTER           335
#define EDEN_KEY_KP_EQUAL           336
#define EDEN_KEY_LEFT_SHIFT         340
#define EDEN_KEY_LEFT_CONTROL       341
#define EDEN_KEY_LEFT_ALT           342
#define EDEN_KEY_LEFT_SUPER         343
#define EDEN_KEY_RIGHT_SHIFT        344
#define EDEN_KEY_RIGHT_CONTROL      345
#define EDEN_KEY_RIGHT_ALT          346
#define EDEN_KEY_RIGHT_SUPER        347
#define EDEN_KEY_MENU               348

namespace Eden {

  std::string KeyCodeToString(int keycode)
  {
    switch (keycode) {
      case EDEN_KEY_SPACE:
        return "EDEN_KEY_SPACE";
      case EDEN_KEY_APOSTROPHE:
        return "EDEN_KEY_APOSTROPHE";
      case EDEN_KEY_COMMA:
        return "EDEN_KEY_COMMA";
      case EDEN_KEY_MINUS:
        return "EDEN_KEY_MINUS";
      case EDEN_KEY_PERIOD:
        return "EDEN_KEY_PERIOD";
      case EDEN_KEY_SLASH:
        return "EDEN_KEY_SLASH";
      case EDEN_KEY_0:
        return "EDEN_KEY_0";
      case EDEN_KEY_1:
        return "EDEN_KEY_1";
      case EDEN_KEY_2:
        return "EDEN_KEY_2";
      case EDEN_KEY_3:
        return "EDEN_KEY_3";
      case EDEN_KEY_4:
        return "EDEN_KEY_4";
      case EDEN_KEY_5:
        return "EDEN_KEY_5";
      case EDEN_KEY_6:
        return "EDEN_KEY_6";
      case EDEN_KEY_7:
        return "EDEN_KEY_7";
      case EDEN_KEY_8:
        return "EDEN_KEY_8";
      case EDEN_KEY_9:
        return "EDEN_KEY_9";
      case EDEN_KEY_SEMICOLON:
        return "EDEN_KEY_SEMICOLON";
      case EDEN_KEY_EQUAL:
        return "EDEN_KEY_EQUAL";
      case EDEN_KEY_A:
        return "EDEN_KEY_A";
      case EDEN_KEY_B:
        return "EDEN_KEY_B";
      case EDEN_KEY_C:
        return "EDEN_KEY_C";
      case EDEN_KEY_D:
        return "EDEN_KEY_D";
      case EDEN_KEY_E:
        return "EDEN_KEY_E";
      case EDEN_KEY_F:
        return "EDEN_KEY_F";
      case EDEN_KEY_G:
        return "EDEN_KEY_G";
      case EDEN_KEY_H:
        return "EDEN_KEY_H";
      case EDEN_KEY_I:
        return "EDEN_KEY_I";
      case EDEN_KEY_J:
        return "EDEN_KEY_J";
      case EDEN_KEY_K:
        return "EDEN_KEY_K";
      case EDEN_KEY_L:
        return "EDEN_KEY_L";
      case EDEN_KEY_M:
        return "EDEN_KEY_M";
      case EDEN_KEY_N:
        return "EDEN_KEY_N";
      case EDEN_KEY_O:
        return "EDEN_KEY_O";
      case EDEN_KEY_P:
        return "EDEN_KEY_P";
      case EDEN_KEY_Q:
        return "EDEN_KEY_Q";
      case EDEN_KEY_R:
        return "EDEN_KEY_R";
      case EDEN_KEY_S:
        return "EDEN_KEY_S";
      case EDEN_KEY_T:
        return "EDEN_KEY_T";
      case EDEN_KEY_U:
        return "EDEN_KEY_U";
      case EDEN_KEY_V:
        return "EDEN_KEY_V";
      case EDEN_KEY_W:
        return "EDEN_KEY_W";
      case EDEN_KEY_X:
        return "EDEN_KEY_X";
      case EDEN_KEY_Y:
        return "EDEN_KEY_Y";
      case EDEN_KEY_Z:
        return "EDEN_KEY_Z";
      case EDEN_KEY_LEFT_BRACKET:
        return "EDEN_KEY_LEFT_BRACKET";
      case EDEN_KEY_BACKSLASH:
        return "EDEN_KEY_BACKSLASH";
      case EDEN_KEY_RIGHT_BRACKET:
        return "EDEN_KEY_RIGHT_BRACKET";
      case EDEN_KEY_GRAVE_ACCENT:
        return "EDEN_KEY_GRAVE_ACCENT";
      case EDEN_KEY_WORLD_1:
        return "EDEN_KEY_WORLD_1";
      case EDEN_KEY_WORLD_2:
        return "EDEN_KEY_WORLD_2";
      case EDEN_KEY_ESCAPE:
        return "EDEN_KEY_ESCAPE";
      case EDEN_KEY_ENTER:
        return "EDEN_KEY_ENTER";
      case EDEN_KEY_TAB:
        return "EDEN_KEY_TAB";
      case EDEN_KEY_BACKSPACE:
        return "EDEN_KEY_BACKSPACE";
      case EDEN_KEY_INSERT:
        return "EDEN_KEY_INSERT";
      case EDEN_KEY_DELETE:
        return "EDEN_KEY_DELETE";
      case EDEN_KEY_RIGHT:
        return "EDEN_KEY_RIGHT";
      case EDEN_KEY_LEFT:
        return "EDEN_KEY_LEFT";
      case EDEN_KEY_DOWN:
        return "EDEN_KEY_DOWN";
      case EDEN_KEY_UP:
        return "EDEN_KEY_UP";
      case EDEN_KEY_PAGE_UP:
        return "EDEN_KEY_PAGE_UP";
      case EDEN_KEY_PAGE_DOWN:
        return "EDEN_KEY_PAGE_DOWN";
      case EDEN_KEY_HOME:
        return "EDEN_KEY_HOME";
      case EDEN_KEY_END:
        return "EDEN_KEY_END";
      case EDEN_KEY_CAPS_LOCK:
        return "EDEN_KEY_CAPS_LOCK";
      case EDEN_KEY_SCROLL_LOCK:
        return "EDEN_KEY_SCROLL_LOCK";
      case EDEN_KEY_NUM_LOCK:
        return "EDEN_KEY_NUM_LOCK";
      case EDEN_KEY_PRINT_SCREEN:
        return "EDEN_KEY_PRINT_SCREEN";
      case EDEN_KEY_PAUSE:
        return "EDEN_KEY_PAUSE";
      case EDEN_KEY_F1:
        return "EDEN_KEY_F1";
      case EDEN_KEY_F2:
        return "EDEN_KEY_F2";
      case EDEN_KEY_F3:
        return "EDEN_KEY_F3";
      case EDEN_KEY_F4:
        return "EDEN_KEY_F4";
      case EDEN_KEY_F5:
        return "EDEN_KEY_F5";
      case EDEN_KEY_F6:
        return "EDEN_KEY_F6";
      case EDEN_KEY_F7:
        return "EDEN_KEY_F7";
      case EDEN_KEY_F8:
        return "EDEN_KEY_F8";
      case EDEN_KEY_F9:
        return "EDEN_KEY_F9";
      case EDEN_KEY_F10:
        return "EDEN_KEY_F10";
      case EDEN_KEY_F11:
        return "EDEN_KEY_F11";
      case EDEN_KEY_F12:
        return "EDEN_KEY_F12";
      case EDEN_KEY_F13:
        return "EDEN_KEY_F13";
      case EDEN_KEY_F14:
        return "EDEN_KEY_F14";
      case EDEN_KEY_F15:
        return "EDEN_KEY_F15";
      case EDEN_KEY_F16:
        return "EDEN_KEY_F16";
      case EDEN_KEY_F17:
        return "EDEN_KEY_F17";
      case EDEN_KEY_F18:
        return "EDEN_KEY_F18";
      case EDEN_KEY_F19:
        return "EDEN_KEY_F19";
      case EDEN_KEY_F20:
        return "EDEN_KEY_F20";
      case EDEN_KEY_F21:
        return "EDEN_KEY_F21";
      case EDEN_KEY_F22:
        return "EDEN_KEY_F22";
      case EDEN_KEY_F23:
        return "EDEN_KEY_F23";
      case EDEN_KEY_F24:
        return "EDEN_KEY_F24";
      case EDEN_KEY_F25:
        return "EDEN_KEY_F25";
      case EDEN_KEY_KP_0:
        return "EDEN_KEY_KP_0";
      case EDEN_KEY_KP_1:
        return "EDEN_KEY_KP_1";
      case EDEN_KEY_KP_2:
        return "EDEN_KEY_KP_2";
      case EDEN_KEY_KP_3:
        return "EDEN_KEY_KP_3";
      case EDEN_KEY_KP_4:
        return "EDEN_KEY_KP_4";
      case EDEN_KEY_KP_5:
        return "EDEN_KEY_KP_5";
      case EDEN_KEY_KP_6:
        return "EDEN_KEY_KP_6";
      case EDEN_KEY_KP_7:
        return "EDEN_KEY_KP_7";
      case EDEN_KEY_KP_8:
        return "EDEN_KEY_KP_8";
      case EDEN_KEY_KP_9:
        return "EDEN_KEY_KP_9";
      case EDEN_KEY_KP_DECIMAL:
        return "EDEN_KEY_KP_DECIMAL";
      case EDEN_KEY_KP_DIVIDE:
        return "EDEN_KEY_KP_DIVIDE";
      case EDEN_KEY_KP_MULTIPLY:
        return "EDEN_KEY_KP_MULTIPLY";
      case EDEN_KEY_KP_SUBTRACT:
        return "EDEN_KEY_KP_SUBTRACT";
      case EDEN_KEY_KP_ADD:
        return "EDEN_KEY_KP_ADD";
      case EDEN_KEY_KP_ENTER:
        return "EDEN_KEY_KP_ENTER";
      case EDEN_KEY_KP_EQUAL:
        return "EDEN_KEY_KP_EQUAL";
      case EDEN_KEY_LEFT_SHIFT:
        return "EDEN_KEY_LEFT_SHIFT";
      case EDEN_KEY_LEFT_CONTROL:
        return "EDEN_KEY_LEFT_CONTROL";
      case EDEN_KEY_LEFT_ALT:
        return "EDEN_KEY_LEFT_ALT";
      case EDEN_KEY_LEFT_SUPER:
        return "EDEN_KEY_LEFT_SUPER";
      case EDEN_KEY_RIGHT_SHIFT:
        return "EDEN_KEY_RIGHT_SHIFT";
      case EDEN_KEY_RIGHT_CONTROL:
        return "EDEN_KEY_RIGHT_CONTROL";
      case EDEN_KEY_RIGHT_ALT:
        return "EDEN_KEY_RIGHT_ALT";
      case EDEN_KEY_RIGHT_SUPER:
        return "EDEN_KEY_RIGHT_SUPER";
      case EDEN_KEY_MENU:
        return "EDEN_KEY_MENU";
    }
  }

}
