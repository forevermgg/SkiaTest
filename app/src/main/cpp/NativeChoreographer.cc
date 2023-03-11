//
// Created by gang meng on 2023/3/11.
//

#include "NativeChoreographer.h"

#include <EGL/egl.h>
#include <dlfcn.h>
#include <jni.h>
#include <sys/system_properties.h>

#include <cassert>

#include "android_debug.h"

const int32_t kFPSThrottleInterval = 2;
const int64_t kFPSThrottlePresentationInterval =
    (kFPSThrottleInterval / 60.0f) * 1000000000;
#define COULD_RENDER(now, last) \
  (((now) - (last)) >= kFPSThrottlePresentationInterval)

static int GetAPILevel() {
  char sdk_version_string[PROP_VALUE_MAX];
  if (__system_property_get("ro.build.version.sdk", sdk_version_string)) {
    return atoi(sdk_version_string);
  } else {
    return -1;
  }
}

// Declaration for native chreographer API.
struct AChoreographer;
typedef void (*AChoreographer_frameCallback)(long frameTimeNanos, void *data);
typedef AChoreographer *(*func_AChoreographer_getInstance)();
typedef void (*func_AChoreographer_postFrameCallback)(
    AChoreographer *choreographer, AChoreographer_frameCallback callback,
    void *data);

// Function pointers for native Choreographer API.
func_AChoreographer_getInstance AChoreographer_getInstance_;
func_AChoreographer_postFrameCallback AChoreographer_postFrameCallback_;

// Stuff for EGL Android presentation time extension.
int64_t presentation_time_;
bool (*eglPresentationTimeANDROID_)(EGLDisplay dpy, EGLSurface sur,
                                    khronos_stime_nanoseconds_t time);

NativeChoreographer::NativeChoreographer() {
  CheckAPISupport();
  StartFPSThrottle();
}

NativeChoreographer::~NativeChoreographer() { StopFPSThrottle(); }

void NativeChoreographer::CheckAPISupport() {
  auto apiLevel = GetAPILevel();
  LOGI("Device API Level %d", apiLevel);
  if (apiLevel >= 24) {
    // Native Choreographer API is supported in API level 24~.
    void *lib = dlopen("libandroid.so", RTLD_NOW | RTLD_LOCAL);
    if (lib != nullptr) {
      LOGI("Run with Choreographer Native API.");
      api_mode_ = kAPINativeChoreographer;

      // Retrieve function pointers from shared object.
      AChoreographer_getInstance_ =
          reinterpret_cast<func_AChoreographer_getInstance>(
              dlsym(lib, "AChoreographer_getInstance"));
      AChoreographer_postFrameCallback_ =
          reinterpret_cast<func_AChoreographer_postFrameCallback>(
              dlsym(lib, "AChoreographer_postFrameCallback"));
      assert(AChoreographer_getInstance_);
      assert(AChoreographer_postFrameCallback_);
    }
  } else if (apiLevel >= 18) {
    // eglPresentationTimeANDROID would be supported in API level 18~.
    LOGI("Run with EGLExtension.");
    api_mode_ = kAPIEGLExtension;

    // Retrieve the EGL extension's function pointer.
    eglPresentationTimeANDROID_ = reinterpret_cast<bool (*)(
        EGLDisplay, EGLSurface, khronos_stime_nanoseconds_t)>(
        eglGetProcAddress("eglPresentationTimeANDROID"));
    assert(eglPresentationTimeANDROID_);
    presentation_time_ = GetCurrentTime();
  } else if (apiLevel >= 16) {
    // Choreographer Java API is supported API level 16~.
    LOGI("Run with Chreographer Java API.");
    api_mode_ = kAPIJavaChoreographer;
  } else {
    api_mode_ = kAPINone;
  }
}

void NativeChoreographer::StartFPSThrottle() {
  if (api_mode_ == kAPINativeChoreographer) {
    // Initiate choreographer callback.
    StartChoreographer();
  } else if (api_mode_ == kAPIJavaChoreographer) {
    // Initiate Java choreographer callback.
    StartJavaChoreographer();
  }
}

void NativeChoreographer::StopFPSThrottle() {
  if (api_mode_ == kAPINativeChoreographer) {
  } else if (api_mode_ == kAPIJavaChoreographer) {
    StopJavaChoreographer();
  }
  api_mode_ = kAPINone;
}

int64_t NativeChoreographer::GetCurrentTime() {
  timespec time;
  clock_gettime(CLOCK_MONOTONIC, &time);
  return static_cast<int64_t>(time.tv_sec) * 1e9 +
         static_cast<int64_t>(time.tv_nsec);
}

// Native Chreographer API support.
void NativeChoreographer::StartChoreographer() {
  // Initiate choreographer callbacks.
  if (api_mode_ == kAPINativeChoreographer) {
    auto choreographer = AChoreographer_getInstance_();
    AChoreographer_postFrameCallback_(choreographer, choreographer_callback,
                                      this);
  }
}

// Native Choreographer callback.
void NativeChoreographer::choreographer_callback(long frameTimeNanos,
                                                 void *data) {
  LOGI("Run with Chreographer Callback.");
  auto engine = reinterpret_cast<NativeChoreographer *>(data);
  // Post next callback for self.

  // Swap buffer if the timing meets the 30fps time interval condition.
  // The callback is in the same thread context, so that we can just invoke
  // eglSwapBuffers().
  if (COULD_RENDER(frameTimeNanos, engine->prevFrameTimeNanos_)) {
    engine->StartChoreographer();
    engine->prevFrameTimeNanos_ = frameTimeNanos;
  }
}

void NativeChoreographer::StartJavaChoreographer() {
  /*JNIEnv *jni;
  jclass clazz = jni->GetObjectClass(app_->activity->clazz);
  jmethodID methodID = jni->GetMethodID(clazz, "startChoreographer", "()V");
  jni->CallVoidMethod(app_->activity->clazz, methodID);
  app_->activity->vm->DetachCurrentThread();*/
}

void NativeChoreographer::StopJavaChoreographer() {
  /*JNIEnv *jni;
  jclass clazz = jni->GetObjectClass(app_->activity->clazz);
  jmethodID methodID = jni->GetMethodID(clazz, "stopChoreographer", "()V");
  jni->CallVoidMethod(app_->activity->clazz, methodID);*/
}

void NativeChoreographer::SyncChoreographerInCallback(jlong frameTimeInNanos) {
  LOGI("Run with SyncChoreographerInCallback Callback.");
}

static void finalize_mixed(jlong ptr) {
  delete reinterpret_cast<NativeChoreographer *>(ptr);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_mgg_skiatest_NativeChoreographer__1nGetFinalizer(
    JNIEnv *env, jclass jNativeChoreographer) {
  return reinterpret_cast<jlong>(&finalize_mixed);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_mgg_skiatest_NativeChoreographer__1nMakeEmpty(
    JNIEnv *env, jclass jNativeChoreographer) {
  return reinterpret_cast<jlong>(new NativeChoreographer());
}

extern "C" JNIEXPORT void JNICALL
Java_com_mgg_skiatest_NativeChoreographer_nativeChoreographerCallback(
    JNIEnv *env, jobject thiz, jlong frame_time_namos, jlong native_ptr) {
  NativeChoreographer *instance = reinterpret_cast<NativeChoreographer *>(
      static_cast<uintptr_t>(native_ptr));
  return instance->SyncChoreographerInCallback(frame_time_namos);
}
