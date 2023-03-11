#include <android/bitmap.h>
#include <jni.h>
#include <math.h>

#include "include/core/SkCanvas.h"
#include "include/core/SkGraphics.h"
#include "include/core/SkString.h"
#include "include/core/SkSurface.h"
#include "include/core/SkTime.h"
#include "include/core/SkTypeface.h"

extern "C" JNIEXPORT void JNICALL
Java_com_mgg_skiatest_SkiaDrawView_drawIntoBitmap(JNIEnv* env, jobject thiz,
                                                  jobject dstBitmap,
                                                  jlong elapsedTime) {
  AndroidBitmapInfo dstInfo;
  void* dstPixels;
  AndroidBitmap_getInfo(env, dstBitmap, &dstInfo);
  AndroidBitmap_lockPixels(env, dstBitmap, &dstPixels);

  SkImageInfo info = SkImageInfo::MakeN32Premul(dstInfo.width, dstInfo.height);

  sk_sp<SkSurface> surface(
      SkSurface::MakeRasterDirect(info, dstPixels, dstInfo.stride));
  SkCanvas* canvas = surface->getCanvas();

  canvas->drawColor(SK_ColorWHITE);

  SkPaint paint;
  paint.setColor(SK_ColorBLACK);
  paint.setAntiAlias(true);
  paint.setAntiAlias(false);
  paint.setColor(0xFF0000FF);
  paint.setStrokeWidth(SkIntToScalar(2));

  for (int i = 0; i < 100; i++) {
    float x = (float)i / 99.0f;
    float offset = elapsedTime / 1000.0f;
    canvas->drawLine(sin(x * M_PI + offset) * 800.0f, 0,
                     cos(x * M_PI + offset) * 800.0f, 800, paint);
  }

  AndroidBitmap_unlockPixels(env, dstBitmap);
}