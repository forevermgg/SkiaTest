#include <jni.h>
#include <math.h>
#include <jni.h>
#include <android/bitmap.h>

#include "include/core/SkCanvas.h"
#include "include/core/SkGraphics.h"
#include "include/core/SkSurface.h"
#include "include/core/SkString.h"
#include "include/core/SkTime.h"

extern "C" JNIEXPORT void JNICALL
Java_com_mgg_skiatest_SkiaDrawView_drawIntoBitmap(JNIEnv* env, jobject thiz,
                                                  jobject dstBitmap,
                                                  jlong elapsedTime) {
  // Grab the dst bitmap info and pixels
  AndroidBitmapInfo dstInfo;
  void* dstPixels;
  AndroidBitmap_getInfo(env, dstBitmap, &dstInfo);
  AndroidBitmap_lockPixels(env, dstBitmap, &dstPixels);

  SkImageInfo info = SkImageInfo::MakeN32Premul(dstInfo.width, dstInfo.height);

  // Create a surface from the given bitmap
  sk_sp<SkSurface> surface(
      SkSurface::MakeRasterDirect(info, dstPixels, dstInfo.stride));
  SkCanvas* canvas = surface->getCanvas();

  // Draw something "interesting"

  // Clear the canvas with a white color
  canvas->drawColor(SK_ColorWHITE);

  // Setup a SkPaint for drawing our text
  SkPaint paint;
  paint.setColor(SK_ColorBLACK);  // This is a solid black color for our text
  // paint.setTextSize(SkIntToScalar(30));  // Sets the text size to 30 pixels
  paint.setAntiAlias(
      true);  // We turn on anti-aliasing so that the text to looks good.

  // Adapt the SkPaint for drawing blue lines
  paint.setAntiAlias(
      false);  // Turning off anti-aliasing speeds up the line drawing
  paint.setColor(0xFF0000FF);  // This is a solid blue color for our lines
  paint.setStrokeWidth(
      SkIntToScalar(2));  // This makes the lines have a thickness of 2 pixels

  // Draw some interesting lines using trig functions
  for (int i = 0; i < 100; i++) {
    float x = (float)i / 99.0f;
    float offset = elapsedTime / 1000.0f;
    canvas->drawLine(sin(x * M_PI + offset) * 800.0f, 0,    // first endpoint
                     cos(x * M_PI + offset) * 800.0f, 800,  // second endpoint
                     paint);  // SkPapint to tell how to draw the line
  }

  // Unlock the dst's pixels
  AndroidBitmap_unlockPixels(env, dstBitmap);
}