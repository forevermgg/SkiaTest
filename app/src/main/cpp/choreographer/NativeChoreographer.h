//
// Created by gang meng on 2023/3/11.
//

#ifndef SKIA_TEST_NATIVE_CHOREOGRAPHER_H
#define SKIA_TEST_NATIVE_CHOREOGRAPHER_H

#include <jni.h>

#include <memory>

// Indicate API mode to achieve 30 FPS.
enum APIMode {
  kAPINone,
  kAPINativeChoreographer,
  kAPIJavaChoreographer,
  kAPIEGLExtension,
};

class NativeChoreographer {
 public:
  NativeChoreographer();
  ~NativeChoreographer();
  void CheckAPISupport();
  void StartFPSThrottle();
  void StopFPSThrottle();
  int64_t GetCurrentTime();
  void StartChoreographer();
  void StartJavaChoreographer();
  void StopJavaChoreographer();
  void SyncChoreographerInCallback(jlong frameTimeInNanos);
  static void choreographer_callback(long frameTimeNanos, void* data);

 private:
  APIMode api_mode_;
  int64_t prevFrameTimeNanos_ = static_cast<int64_t>(0);
};

#endif  // SKIA_TEST_NATIVE_CHOREOGRAPHER_H
