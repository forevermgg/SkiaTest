#ifndef __SAMPLE_ANDROID_DEBUG_H__
#define __SAMPLE_ANDROID_DEBUG_H__
#include "../../../../../../../Library/Android/sdk/ndk/25.1.8937393/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/android/log.h"

#if 1
#ifndef MODULE_NAME
#define MODULE_NAME "OpenGL-Wide-Color"
#endif

#define LOGV(...) \
  __android_log_print(ANDROID_LOG_VERBOSE, MODULE_NAME, __VA_ARGS__)
#define LOGD(...) \
  __android_log_print(ANDROID_LOG_DEBUG, MODULE_NAME, __VA_ARGS__)
#define LOGI(...) \
  __android_log_print(ANDROID_LOG_INFO, MODULE_NAME, __VA_ARGS__)
#define LOGW(...) \
  __android_log_print(ANDROID_LOG_WARN, MODULE_NAME, __VA_ARGS__)
#define LOGE(...) \
  __android_log_print(ANDROID_LOG_ERROR, MODULE_NAME, __VA_ARGS__)
#define LOGF(...) \
  __android_log_print(ANDROID_LOG_FATAL, MODULE_NAME, __VA_ARGS__)

#define ASSERT(cond, ...)                                  \
  if (!(cond)) {                                           \
    __android_log_assert(#cond, MODULE_NAME, __VA_ARGS__); \
  }
#else

#define LOGV(...)
#define LOGD(...)
#define LOGI(...)
#define LOGW(...)
#define LOGE(...)
#define LOGF(...)
#define ASSERT(cond, ...)

#endif

#endif  // __SAMPLE_ANDROID_DEBUG_H__
