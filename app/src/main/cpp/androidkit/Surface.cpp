/*
 * Copyright 2021 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#include "Surface.h"

#include <android/bitmap.h>
#include <android/log.h>

#include "../DisplayParams.h"

namespace {

class BitmapSurface final : public Surface {
 public:
  BitmapSurface(JNIEnv* env, jobject bitmap) {
    AndroidBitmapInfo bm_info;
    if (AndroidBitmap_getInfo(env, bitmap, &bm_info) !=
        ANDROID_BITMAP_RESULT_SUCCESS) {
      return;
    }

    const auto info = SkImageInfo::Make(bm_info.width, bm_info.height,
                                        color_type(bm_info.format),
                                        alpha_type(bm_info.flags));

    void* pixels;
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) !=
        ANDROID_BITMAP_RESULT_SUCCESS) {
      return;
    }

    fSurface = SkSurface::MakeRasterDirect(info, pixels, bm_info.stride);
    if (!fSurface) {
      AndroidBitmap_unlockPixels(env, bitmap);
      return;
    }

    fBitmap = env->NewGlobalRef(bitmap);
  }

 private:
  void release(JNIEnv* env) override {
    if (fSurface) {
      AndroidBitmap_unlockPixels(env, fBitmap);
      fSurface.reset();
      env->DeleteGlobalRef(fBitmap);
    }
  }

  SkCanvas* getCanvas() override {
    if (fSurface) {
      return fSurface->getCanvas();
    }
    return nullptr;
  }

  void flushAndSubmit() override {
    // Nothing to do.
  }

  static SkColorType color_type(int32_t format) {
    switch (format) {
      case ANDROID_BITMAP_FORMAT_RGBA_8888:
        return kRGBA_8888_SkColorType;
      case ANDROID_BITMAP_FORMAT_RGB_565:
        return kRGB_565_SkColorType;
      case ANDROID_BITMAP_FORMAT_RGBA_4444:
        return kARGB_4444_SkColorType;
      case ANDROID_BITMAP_FORMAT_RGBA_F16:
        return kRGBA_F16_SkColorType;
      case ANDROID_BITMAP_FORMAT_A_8:
        return kAlpha_8_SkColorType;
      default:
        break;
    }

    return kUnknown_SkColorType;
  }

  static SkAlphaType alpha_type(int32_t flags) {
    switch ((flags >> ANDROID_BITMAP_FLAGS_ALPHA_SHIFT) &
            ANDROID_BITMAP_FLAGS_ALPHA_MASK) {
      case ANDROID_BITMAP_FLAGS_ALPHA_OPAQUE:
        return kOpaque_SkAlphaType;
      case ANDROID_BITMAP_FLAGS_ALPHA_PREMUL:
        return kPremul_SkAlphaType;
      case ANDROID_BITMAP_FLAGS_ALPHA_UNPREMUL:
        return kUnpremul_SkAlphaType;
      default:
        break;
    }

    return kUnknown_SkAlphaType;
  }

  jobject fBitmap;
};

// *** JNI methods ***

static jlong Surface_CreateBitmap(JNIEnv* env, jclass clazz, jobject bitmap) {
  return reinterpret_cast<jlong>(new BitmapSurface(env, bitmap));
}

static void Surface_Release(JNIEnv* env, jclass clazz, jlong native_surface) {
  if (auto* surface = reinterpret_cast<Surface*>(native_surface)) {
    surface->release(env);
    SkSafeUnref(surface);
  }
}

static jlong Surface_GetNativeCanvas(JNIEnv* env, jclass clazz,
                                     jlong native_surface) {
  auto* surface = reinterpret_cast<Surface*>(native_surface);
  return surface ? reinterpret_cast<jlong>(surface->getCanvas()) : 0;
}

static void Surface_FlushAndSubmit(JNIEnv* env, jclass clazz,
                                   jlong native_surface) {
  if (auto* surface = reinterpret_cast<Surface*>(native_surface)) {
    surface->flushAndSubmit();
  }
}

static jint Surface_GetWidth(JNIEnv* env, jclass clazz, jlong native_surface) {
  const auto* surface = reinterpret_cast<Surface*>(native_surface);
  return surface ? surface->width() : 0;
}

static jint Surface_GetHeight(JNIEnv* env, jclass clazz, jlong native_surface) {
  const auto* surface = reinterpret_cast<Surface*>(native_surface);
  return surface ? surface->height() : 0;
}

static jlong Surface_MakeSnapshot(JNIEnv* env, jclass clazz,
                                  jlong native_surface) {
  if (const auto* surface = reinterpret_cast<Surface*>(native_surface)) {
    auto snapshot = surface->makeImageSnapshot();
    return reinterpret_cast<jlong>(snapshot.release());
  }
  return 0;
}

// *** End of JNI methods ***

}  // namespace

int register_androidkit_Surface(JNIEnv* env) {
  static const JNINativeMethod methods[] = {
      {"nCreateBitmap", "(Landroid/graphics/Bitmap;)J",
       reinterpret_cast<void*>(Surface_CreateBitmap)},
      {"nRelease", "(J)V", reinterpret_cast<void*>(Surface_Release)},
      {"nGetNativeCanvas", "(J)J",
       reinterpret_cast<void*>(Surface_GetNativeCanvas)},
      {"nFlushAndSubmit", "(J)V",
       reinterpret_cast<void*>(Surface_FlushAndSubmit)},
      {"nGetWidth", "(J)I", reinterpret_cast<void*>(Surface_GetWidth)},
      {"nGetHeight", "(J)I", reinterpret_cast<void*>(Surface_GetHeight)},
      {"nMakeImageSnapshot", "(J)J",
       reinterpret_cast<void*>(Surface_MakeSnapshot)},
  };

  const auto clazz = env->FindClass("org/skia/androidkit/Surface");
  return clazz ? env->RegisterNatives(clazz, methods, SK_ARRAY_COUNT(methods))
               : JNI_ERR;
}
