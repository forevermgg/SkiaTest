/*
 * Copyright 2021 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef AndroidKit_Surface_DEFINED
#define AndroidKit_Surface_DEFINED

#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <jni.h>

#include "include/core/SkImage.h"
#include "include/core/SkPictureRecorder.h"
#include "include/core/SkRefCnt.h"
#include "include/core/SkSurface.h"
#include "include/core/SkTypes.h"

class Surface : public SkRefCnt {
 public:
  virtual void release(JNIEnv*) = 0;
  virtual void flushAndSubmit() = 0;
  virtual SkCanvas* getCanvas() = 0;

  int width() const { return fSurface ? fSurface->width() : 0; }
  int height() const { return fSurface ? fSurface->height() : 0; }

  sk_sp<SkImage> makeImageSnapshot() const {
    return fSurface ? fSurface->makeImageSnapshot() : nullptr;
  }

 protected:
  sk_sp<SkSurface> fSurface;
};

#endif
