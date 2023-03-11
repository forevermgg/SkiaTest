#ifndef ANDROID_HWUI_TIMING_H
#define ANDROID_HWUI_TIMING_H

#include <sys/time.h>

#include "../debug/android_debug.h"

#define TIME_METHOD() MethodTimer __method_timer(__func__)
class MethodTimer {
 public:
  explicit MethodTimer(const char* name) : mMethodName(name) {
    gettimeofday(&mStart, nullptr);
  }
  ~MethodTimer() {
    struct timeval stop;
    gettimeofday(&stop, nullptr);
    long long elapsed = (stop.tv_sec * 1000000) - (mStart.tv_sec * 1000000) +
                        (stop.tv_usec - mStart.tv_usec);
    LOGD("%s took %.2fms", mMethodName, elapsed / 1000.0);
  }

 private:
  const char* mMethodName;
  struct timeval mStart;
};

#endif