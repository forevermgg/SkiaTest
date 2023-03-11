// Must match the constants in ActivityInfo.java
enum class ColorMode {
  // SRGB means HWUI will produce buffer in SRGB color space.
  Default = 0,
  // WideColorGamut selects the most optimal colorspace & format for the
  // device's display
  // Most commonly DisplayP3 + RGBA_8888 currently.
  WideColorGamut = 1,
  // HDR Rec2020 + F16
  Hdr = 2,
  // HDR Rec2020 + 1010102
  Hdr10 = 3,
  // Alpha 8
  A8 = 4,
};