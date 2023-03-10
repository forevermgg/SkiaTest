#include <iostream>
#include <jni.h>

typedef void (*FreeFunction)(void*);

extern "C" JNIEXPORT void JNICALL Java_com_mgg_skiatest_java_impl_Managed__1nInvokeFinalizer
(JNIEnv* env, jclass jclass, jlong finalizerPtr, jlong ptr) {
  void* instance = reinterpret_cast<void*>(static_cast<uintptr_t>(ptr));
  FreeFunction finalizer = reinterpret_cast<FreeFunction>(static_cast<uintptr_t>(finalizerPtr));
  finalizer(instance);
}