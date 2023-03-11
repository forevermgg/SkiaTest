#pragma once
#include <jni.h>

int jniThrowException(JNIEnv* env, const char* className, const char* msg);

static int jniThrowRuntimeException(JNIEnv* env, const char* msg) {
  return jniThrowException(env, "java/lang/RuntimeException", msg);
}