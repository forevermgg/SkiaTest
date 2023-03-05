// DisplaySettings contains the settings that are applicable when drawing all
// layers for a given display.
// https://cs.android.com/android/platform/superproject/+/master:frameworks/native/libs/renderengine/include/renderengine/DisplaySettings.h;l=26;drc=5ca657189aac546af0aafaba11bbc9c5d889eab3
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

struct DisplaySettings {
  size_t mWidth;
  size_t mHeight;
};