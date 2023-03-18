#include <android/bitmap.h>
#include <jni.h>
#include <math.h>

#include "include/core/SkCanvas.h"
#include "include/core/SkGraphics.h"
#include "include/core/SkPictureRecorder.h"
#include "include/core/SkString.h"
#include "include/core/SkSurface.h"
#include "include/core/SkTime.h"
#include "include/core/SkTypeface.h"

void Canvas_drawPicture_2(SkCanvas* canvas) {
  SkPictureRecorder recorder;
  SkCanvas* recordingCanvas = recorder.beginRecording(50, 50);
  for (auto color : {SK_ColorRED, SK_ColorBLUE, 0xff007f00}) {
    SkPaint paint;
    paint.setColor(color);
    recordingCanvas->drawRect({10, 10, 30, 40}, paint);
    recordingCanvas->translate(10, 10);
    recordingCanvas->scale(1.2f, 1.4f);
  }
  sk_sp<SkPicture> playback = recorder.finishRecordingAsPicture();
  canvas->drawPicture(playback);
  canvas->scale(2, 2);
  canvas->translate(50, 0);
  canvas->drawPicture(playback);
}

void Canvas_drawPicture_3(SkCanvas* canvas) {
  SkPaint paint;
  SkPictureRecorder recorder;
  SkCanvas* recordingCanvas = recorder.beginRecording(50, 50);
  for (auto color : {SK_ColorRED, SK_ColorBLUE, 0xff007f00}) {
    paint.setColor(color);
    recordingCanvas->drawRect({10, 10, 30, 40}, paint);
    recordingCanvas->translate(10, 10);
    recordingCanvas->scale(1.2f, 1.4f);
  }
  sk_sp<SkPicture> playback = recorder.finishRecordingAsPicture();
  const SkPicture* playbackPtr = playback.get();
  SkMatrix matrix;
  matrix.reset();
  for (auto alpha : {70, 140, 210}) {
    paint.setAlpha(alpha);
    canvas->drawPicture(playbackPtr, &matrix, &paint);
    matrix.preTranslate(70, 70);
  }
}

void Canvas_drawPicture_4(SkCanvas* canvas) {
  SkPaint paint;
  SkPictureRecorder recorder;
  SkCanvas* recordingCanvas = recorder.beginRecording(50, 50);
  for (auto color : {SK_ColorRED, SK_ColorBLUE, 0xff007f00}) {
    paint.setColor(color);
    recordingCanvas->drawRect({10, 10, 30, 40}, paint);
    recordingCanvas->translate(10, 10);
    recordingCanvas->scale(1.2f, 1.4f);
  }
  sk_sp<SkPicture> playback = recorder.finishRecordingAsPicture();
  SkMatrix matrix;
  matrix.reset();
  for (auto alpha : {70, 140, 210}) {
    paint.setAlpha(alpha);
    canvas->drawPicture(playback, &matrix, &paint);
    matrix.preTranslate(70, 70);
  }
}

void Image_MakeFromPicture(SkCanvas* canvas) {
  SkPaint paint;
  SkPictureRecorder recorder;
  SkCanvas* recordingCanvas = recorder.beginRecording(50, 50);
  for (auto color : {SK_ColorRED, SK_ColorBLUE, 0xff007f00}) {
    paint.setColor(color);
    recordingCanvas->drawRect({10, 10, 30, 40}, paint);
    recordingCanvas->translate(10, 10);
    recordingCanvas->scale(1.2f, 1.4f);
  }
  sk_sp<SkPicture> playback = recorder.finishRecordingAsPicture();
  int x = 0, y = 0;
  for (auto alpha : {70, 140, 210}) {
    paint.setAlpha(alpha);
    auto srgbColorSpace = SkColorSpace::MakeSRGB();
    sk_sp<SkImage> image =
        SkImage::MakeFromPicture(playback, {50, 50}, nullptr, &paint,
                                 SkImage::BitDepth::kU8, srgbColorSpace);
    canvas->drawImage(image, x, y);
    x += 70;
    y += 70;
  }
}

void Picture_008(SkCanvas* canvas) {
  SkPictureRecorder recorder;
  SkCanvas* pictureCanvas = recorder.beginRecording({0, 0, 256, 256});
  SkPaint paint;
  pictureCanvas->drawRect(SkRect::MakeWH(200, 200), paint);
  paint.setColor(SK_ColorWHITE);
  pictureCanvas->drawRect(SkRect::MakeLTRB(20, 20, 180, 180), paint);
  sk_sp<SkPicture> picture = recorder.finishRecordingAsPicture();
  SkDynamicMemoryWStream writableStream;
  picture->serialize(&writableStream);
  sk_sp<SkData> readableData = writableStream.detachAsData();
  sk_sp<SkPicture> copy =
      SkPicture::MakeFromData(readableData->data(), readableData->size());
  copy->playback(canvas);
}

void Picture_cullRect(SkCanvas* canvas) {
  SkPictureRecorder recorder;
  SkCanvas* pictureCanvas = recorder.beginRecording({64, 64, 192, 192});
  SkPaint paint;
  pictureCanvas->drawRect(SkRect::MakeWH(200, 200), paint);
  paint.setColor(SK_ColorWHITE);
  pictureCanvas->drawRect(SkRect::MakeLTRB(20, 20, 180, 180), paint);
  sk_sp<SkPicture> picture = recorder.finishRecordingAsPicture();
  picture->playback(canvas);
  paint.setBlendMode(SkBlendMode::kModulate);
  paint.setColor(0x40404040);
  canvas->drawRect(picture->cullRect(), paint);
}

void Picture_playback(SkCanvas* canvas) {
  SkPictureRecorder recorder;
  SkCanvas* pictureCanvas = recorder.beginRecording({0, 0, 256, 256});
  SkPaint paint;
  pictureCanvas->drawRect(SkRect::MakeWH(200, 200), paint);
  paint.setColor(SK_ColorWHITE);
  pictureCanvas->drawRect(SkRect::MakeLTRB(20, 20, 180, 180), paint);
  sk_sp<SkPicture> picture = recorder.finishRecordingAsPicture();
  picture->playback(canvas);
}

void f16to8888busted(SkCanvas* canvas) {
  // Create the linear-rgb color space and the image info
  sk_sp<SkColorSpace> colorSpace = SkColorSpace::MakeSRGBLinear();
  SkImageInfo info =
      SkImageInfo::Make(100, 100, SkColorType::kRGBA_F16_SkColorType,
                        SkAlphaType::kPremul_SkAlphaType, colorSpace);

  // If we rendered offscreen, snap an image and push the results to the
  // window's canvas
  sk_sp<SkSurface> offscreen = SkSurface::MakeRaster(info);
  SkPaint paint;
  offscreen->getCanvas()->drawRect(SkRect::MakeXYWH(0, 0, 150, 150), paint);

  // Take a snapshot from surface and draw it on the canvas
  // We made/have an off-screen surface. Get the contents as an SkImage:
  sk_sp<SkImage> img = offscreen->makeImageSnapshot();
  canvas->drawImageRect(img, SkRect::MakeWH(100, 100), SkSamplingOptions());
}

sk_sp<SkPicture> make_pic(const std::function<void(SkCanvas*)>& drawer) {
    SkPictureRecorder rec;
    drawer(rec.beginRecording(128, 128));
    return rec.finishRecordingAsPicture();
}

void serial_procs_picture(SkCanvas* canvas) {
    auto p1 = make_pic([](SkCanvas* c) {
        // need to be large enough that drawPictures doesn't "unroll" us
        for (int i = 0; i < 20; ++i) {
            c->drawColor(SK_ColorRED);
        }
    });
    canvas->drawPicture(p1);
}

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

  SkPaint greenPaint;
  greenPaint.setColor(0xff008000);
  SkPaint blackPaint;
  blackPaint.setColor(0xff000000);
  SkPaint whitePaint;
  whitePaint.setColor(0xffffffff);
  SkPaint layerPaint;
  layerPaint.setColor(0xff000000);

  auto surf = SkSurface::MakeRasterN32Premul(100, 100);
  surf->getCanvas()->clear(0xffffffff);
  SkPaint circlePaint;
  circlePaint.setColor(0xff000000);
  surf->getCanvas()->drawCircle(50, 50, 50, circlePaint);
  sk_sp<SkImage> fCircleImage;
  fCircleImage = surf->makeImageSnapshot();
  canvas->drawImageRect(fCircleImage, SkRect::MakeXYWH(20, 20, 60, 60),
                        SkSamplingOptions(), &greenPaint);

  SkRect canvasRect(SkRect::MakeWH(SkIntToScalar(100), SkIntToScalar(100)));
  canvas->saveLayer(nullptr, &blackPaint);
  canvas->drawRect(canvasRect, greenPaint);
  canvas->saveLayer(nullptr, &layerPaint);
  canvas->drawImageRect(fCircleImage, SkRect::MakeXYWH(20, 20, 60, 60),
                        SkSamplingOptions(), &blackPaint);
  canvas->restore();
  canvas->restore();

  /*for (int i = 0; i < 100; i++) {
    float x = (float)i / 99.0f;
    float offset = elapsedTime / 1000.0f;
    canvas->drawLine(sin(x * M_PI + offset) * 800.0f, 0,
                     cos(x * M_PI + offset) * 800.0f, 800, paint);
  }*/
  // Canvas_drawPicture_2(canvas);
  // Canvas_drawPicture_3(canvas);
  // Canvas_drawPicture_4(canvas);
  // Image_MakeFromPicture(canvas);
  // Picture_008(canvas);
  // Picture_cullRect(canvas);
  // Picture_playback(canvas);
  // f16to8888busted(canvas);
    serial_procs_picture(canvas);
  AndroidBitmap_unlockPixels(env, dstBitmap);
}