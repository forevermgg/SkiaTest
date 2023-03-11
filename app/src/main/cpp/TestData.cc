//
// Created by centforever on 2023/3/5.
//
#include "TestData.h"

#include <jni.h>

#include <memory>
#include <array>
#include <condition_variable>
#include <set>
#include <thread>

#include "common/types.h"
#include "renderengine/DisplaySettings.h"
#include "renderengine/RenderEngineCreationArgs.h"
#include "utils/Errors.h"
#include "utils/Timing.h"
#include "CommonPool.h"

TestData::TestData() {
  eglManager = std::make_unique<EglManager>();
  eglManager->initialize();

  // https://github.com/aosp-mirror/platform_frameworks_base/blob/60bcab93678eec400bbdaa53f1b7f3e3d9119165/libs/hwui/tests/unit/CommonPoolTests.cpp
  std::atomic_bool ran(false);
  CommonPool::post([&ran] { ran = true; });
  for (int i = 0; !ran && i < 1000; i++) {
    usleep(1);
  }
  LOGD("Failed to flip atomic after 1 second");

  std::set<pid_t> threads;
  std::array<std::future<pid_t>, 64> futures;
  for (int i = 0; i < futures.size(); i++) {
    futures[i] = CommonPool::async([] {
        usleep(10);
        return gettid();
    });
  }
  for (auto& f : futures) {
    threads.insert(f.get());
  }
  LOGD("threads.size(): %d", threads.size());
  LOGD("threads.size(): %d", CommonPool::THREAD_COUNT);
  LOGD("threads.count: %d", threads.count(gettid()));
}

TestData::~TestData() {
  eglManager->destroy();
  eglManager = nullptr;
}

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
