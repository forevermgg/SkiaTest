#ifndef WindowContextFactory_android_DEFINED
#define WindowContextFactory_android_DEFINED

#include <android/native_window_jni.h>

#include <memory>

class WindowContext;
struct DisplayParams;

namespace window_context_factory {

std::unique_ptr<WindowContext> MakeVulkanForAndroid(ANativeWindow*,
                                                    const DisplayParams&);

std::unique_ptr<WindowContext> MakeGLForAndroid(ANativeWindow*,
                                                const DisplayParams&);

std::unique_ptr<WindowContext> MakeRasterForAndroid(ANativeWindow*,
                                                    const DisplayParams&);

}  // namespace window_context_factory

#endif
