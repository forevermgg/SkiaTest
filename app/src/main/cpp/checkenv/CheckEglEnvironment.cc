//
// Created by centforever on 2023/3/5.
//

#include "CheckEglEnvironment.h"

#include <EGL/egl.h>
#include <jni.h>

CheckEglEnvironment::CheckEglEnvironment() {}

CheckEglEnvironment::~CheckEglEnvironment() {}

bool CheckEglEnvironment::CheckSupportEgl() {
  EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  if (!eglInitialize(display, nullptr, nullptr)) {
    return false;
  }
  return true;
}

bool CheckEglEnvironment::CheckSupportGLExtensions() {
  // initialize EGL for the default display
  EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  if (!eglInitialize(display, nullptr, nullptr)) {
    return false;
  }

  const auto eglVersion = eglQueryString(display, EGL_VERSION);
  if (!eglVersion) {
    return false;
  }

  const auto eglExtensions = eglQueryString(display, EGL_EXTENSIONS);
  if (!eglExtensions) {
    return false;
  }
  return true;
}

static void finalize_mixed(jlong ptr) {
  delete reinterpret_cast<CheckEglEnvironment *>(ptr);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_mgg_skiatest_CheckEglEnvironment__1nGetFinalizer(JNIEnv *env,
                                                          jclass clazz) {
  return reinterpret_cast<jlong>(&finalize_mixed);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_mgg_skiatest_CheckEglEnvironment__1nMakeEmpty(JNIEnv *env,
                                                       jclass clazz) {
  return reinterpret_cast<jlong>(new CheckEglEnvironment());
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_mgg_skiatest_CheckEglEnvironment__1nCheckSupportEgl(JNIEnv *env,
                                                             jclass clazz,
                                                             jlong ptr) {
  CheckEglEnvironment *instance =
      reinterpret_cast<CheckEglEnvironment *>(static_cast<uintptr_t>(ptr));
  return instance->CheckSupportEgl();
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_mgg_skiatest_CheckEglEnvironment__1nCheckSupportGLExtensions(
    JNIEnv *env, jclass clazz, jlong ptr) {
  CheckEglEnvironment *instance =
      reinterpret_cast<CheckEglEnvironment *>(static_cast<uintptr_t>(ptr));
  return instance->CheckSupportGLExtensions();
}