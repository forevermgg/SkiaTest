//
// Created by centforever on 2023/3/11.
//

#ifndef SKIA_TEST_GL_WINDOW_CONTEXT_H
#define SKIA_TEST_GL_WINDOW_CONTEXT_H

#include "WindowContext.h"
#include "include/core/SkRefCnt.h"
#include "include/core/SkSurface.h"
#include "include/gpu/gl/GrGLInterface.h"

class GLWindowContext : public WindowContext {
 public:
  sk_sp<SkSurface> getBackbufferSurface() override;
  bool isValid() override { return SkToBool(fBackendContext.get()); }
  void resize(int w, int h) override;
  void swapBuffers() override;
  void setDisplayParams(const DisplayParams& params) override;

 protected:
  GLWindowContext(const DisplayParams&);
  // This should be called by subclass constructor. It is also called when
  // window/display parameters change. This will in turn call
  // onInitializeContext().
  void initializeContext();
  virtual sk_sp<const GrGLInterface> onInitializeContext() = 0;
  // This should be called by subclass destructor. It is also called when
  // window/display parameters change prior to initializing a new GL context.
  // This will in turn call onDestroyContext().
  void destroyContext();
  virtual void onDestroyContext() = 0;
  virtual void onSwapBuffers() = 0;
  sk_sp<const GrGLInterface> fBackendContext;
  sk_sp<SkSurface> fSurface;
};

#endif  // SKIA_TEST_GL_WINDOW_CONTEXT_H
