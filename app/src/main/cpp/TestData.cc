//
// Created by centforever on 2023/3/5.
//
#include "TestData.h"
#include "renderengine/RenderEngineCreationArgs.h"
#include "renderengine/DisplaySettings.h"
#include "utils/Errors.h"
#include "common/types.h"
#include <jni.h>

TestData::TestData() {}

TestData::~TestData() {}

static void finalize_mixed(jlong ptr) {
  delete reinterpret_cast<TestData *>(ptr);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_mgg_skiatest_TestData__1nGetFinalizer(JNIEnv *env, jclass clazz) {
  return reinterpret_cast<jlong>(&finalize_mixed);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_mgg_skiatest_TestData__1nMakeEmpty(JNIEnv *env, jclass jclass) {
  return reinterpret_cast<jlong>(new TestData());
}