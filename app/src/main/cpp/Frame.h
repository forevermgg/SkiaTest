//
// Created by centforever on 2023/3/10.
//

#ifndef SKIA_TEST_FRAME_H
#define SKIA_TEST_FRAME_H

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "include/core/SkRect.h"

class Frame {
 public:
  Frame();
  Frame(int32_t width, int32_t height, int32_t bufferAge);
  ~Frame();
  void map(const SkRect& in, int32_t* out) const;
  int32_t width() const;
  int32_t height() const;
  int32_t bufferAge() const;

  EGLSurface mSurface;
  int32_t mBufferAge;

  int32_t mWidth;
  int32_t mHeight;
};

#endif  // SKIA_TEST_FRAME_H
