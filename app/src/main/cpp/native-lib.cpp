#include <jni.h>

#include <string>

#include "DisplayParams.h"
#include "exception/exception_helper.h"

void might_throw() { throw std::runtime_error("A C++ runtime_error"); }

extern "C" JNIEXPORT jstring JNICALL
Java_com_mgg_skiatest_MainActivity_getAbiString(JNIEnv *env, jobject thiz) {
#if defined(__arm__)
#if defined(__ARM_ARCH_7A__)
#if defined(__ARM_NEON__)
#if defined(__ARM_PCS_VFP)
#define ABI "armeabi-v7a/NEON (hard-float)"
#else
#define ABI "armeabi-v7a/NEON"
#endif
#else
#if defined(__ARM_PCS_VFP)
#define ABI "armeabi-v7a (hard-float)"
#else
#define ABI "armeabi-v7a"
#endif
#endif
#else
#define ABI "armeabi"
#endif
#elif defined(__i386__)
#define ABI "x86"
#elif defined(__x86_64__)
#define ABI "x86_64"
#elif defined(__mips64) /* mips64el-* toolchain defines __mips__ too */
#define ABI "mips64"
#elif defined(__mips__)
#define ABI "mips"
#elif defined(__aarch64__)
#define ABI "arm64-v8a"
#else
#define ABI "unknown"
#endif

  return (*env).NewStringUTF(
      "Hello from JNI !  "
      "Compiled with ABI " ABI ".");
}

extern "C" JNIEXPORT void JNICALL
Java_com_mgg_skiatest_MainActivity_throwsException(JNIEnv *env, jobject thiz) {
  try {
    might_throw();
  } catch (std::exception &e) {
    // jniThrowRuntimeException(env, e.what());
  } catch (...) {
    // We don't want any C++ exceptions to cross the JNI boundary, so include a
    // catch-all.
    // jniThrowRuntimeException(env, "Catch-all");
  }
}