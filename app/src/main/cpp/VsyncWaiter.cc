#include <jni.h>
#include <cmath>
#include <utility>
#include <memory>
#include <mutex>

static jclass g_vsync_waiter_class = nullptr;
static jmethodID g_async_wait_for_vsync_method_ = nullptr;
static std::atomic_uint g_refresh_rate_ = 60;
static JNIEnv* gobalJNIEnv;

class VsyncWaiter {
 public:
  VsyncWaiter();
  ~VsyncWaiter();
    // There are two distinct situations where VsyncWaiter wishes to awaken at
    // the next vsync. Although the functionality can be the same, the intent is
    // different, therefore it makes sense to have a method for each intent.

    // The intent of AwaitVSync() is that the Animator wishes to produce a frame.
    // The underlying implementation can choose to be aware of this intent when
    // it comes to implementing backpressure and other scheduling invariants.
    //
    // Implementations are meant to override this method and arm their vsync
    // latches when in response to this invocation. On vsync, they are meant to
    // invoke the |FireCallback| method once (and only once) with the appropriate
    // arguments. This method should not block the current thread.
  void AwaitVSync();
 private:
};

VsyncWaiter::VsyncWaiter() {}

VsyncWaiter::~VsyncWaiter() {}

void VsyncWaiter::AwaitVSync() {
  jlong java_baton = reinterpret_cast<jlong>(this);
  gobalJNIEnv->CallStaticVoidMethod(g_vsync_waiter_class,     //
                            g_async_wait_for_vsync_method_,  //
                            java_baton                       //
  );
  // RequestFrame(); ->> AwaitVSync ->>  BeginFrame();
}

static void finalize_mixed(jlong ptr) {
  delete reinterpret_cast<VsyncWaiter *>(ptr);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_mgg_skiatest_VsyncWaiter__1nGetFinalizer(JNIEnv *env, jclass clazz) {
  return reinterpret_cast<jlong>(&finalize_mixed);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_mgg_skiatest_VsyncWaiter__1nMakeEmpty(JNIEnv *env, jclass jclass) {
  // jclass clazz = env->FindClass("com/mgg/skiatest/VsyncWaiter");
  gobalJNIEnv = env;
  g_vsync_waiter_class = jclass;
  g_async_wait_for_vsync_method_ = env->GetStaticMethodID(jclass, "asyncWaitForVsync", "(J)V");
  return reinterpret_cast<jlong>(new VsyncWaiter());
}

extern "C" JNIEXPORT void JNICALL
Java_com_mgg_skiatest_VsyncWaiter_nativeUpdateRefreshRate(JNIEnv *env, jclass thiz,
                                                          jfloat refresh_rate) {
  g_refresh_rate_ = static_cast<uint>(refresh_rate);
}

extern "C" JNIEXPORT void JNICALL
Java_com_mgg_skiatest_VsyncWaiter_nativeOnVsync(JNIEnv *env, jobject thiz, jlong frameDelayNanos,
                                                jlong refreshPeriodNanos, jlong cookie,
                                                jlong native_ptr) {
  const auto elapsed = std::chrono::steady_clock::now().time_since_epoch();
  auto now = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
  auto frame_time = now - (int64_t)frameDelayNanos;
  auto target_time =
          frame_time + (int64_t)(refreshPeriodNanos);
  auto* weak_this = reinterpret_cast<VsyncWaiter*>(native_ptr);
  weak_this->AwaitVSync();
}