//
// Created by centforever on 2023/3/11.
//

#include "GLWindowContext.h"

#include <GLES/gl.h>

#include "include/core/SkCanvas.h"
#include "include/core/SkMatrix.h"
#include "include/core/SkSurface.h"
#include "include/gpu/GrBackendSurface.h"
#include "include/gpu/GrDirectContext.h"
#include "include/gpu/gl/GrGLDefines.h"
#include "include/gpu/gl/GrGLTypes.h"

// These are common defines present on all OpenGL headers. However, we don't
// want to perform GL header reasolution on each platform we support. So just
// define these upfront. It is unlikely we will need more. But, if we do, we can
// add the same here.
#define GPU_GL_RGBA8 0x8058
#define GPU_GL_RGBA4 0x8056
#define GPU_GL_RGB565 0x8D62

static SkColorType FirstSupportedColorType(GrDirectContext* context,
                                           GrGLenum* format) {
#define RETURN_IF_RENDERABLE(x, y)                 \
  if (context->colorTypeSupportedAsSurface((x))) { \
    *format = (y);                                 \
    return (x);                                    \
  }
  RETURN_IF_RENDERABLE(kRGBA_8888_SkColorType, GPU_GL_RGBA8);
  RETURN_IF_RENDERABLE(kARGB_4444_SkColorType, GPU_GL_RGBA4);
  RETURN_IF_RENDERABLE(kRGB_565_SkColorType, GPU_GL_RGB565);
  return kUnknown_SkColorType;
}

GLWindowContext::GLWindowContext(const DisplayParams& params)
    : WindowContext(params), fBackendContext(nullptr), fSurface(nullptr) {
  fDisplayParams.fMSAASampleCount = 4;
}
void GLWindowContext::initializeContext() {
  SkASSERT(!fContext);
  fBackendContext = this->onInitializeContext();
  fContext = GrDirectContext::MakeGL(fBackendContext,
                                     fDisplayParams.fGrContextOptions);
  if (!fContext && fDisplayParams.fMSAASampleCount > 1) {
    fDisplayParams.fMSAASampleCount /= 2;
    this->initializeContext();
    return;
  }
}
void GLWindowContext::destroyContext() {
  fSurface.reset(nullptr);
  if (fContext) {
    // in case we have outstanding refs to this (lua?)
    fContext->abandonContext();
    fContext.reset();
  }
  fBackendContext.reset(nullptr);
  this->onDestroyContext();
}
sk_sp<SkSurface> GLWindowContext::getBackbufferSurface() {
  if (nullptr == fSurface) {
    // TODO(forevermeng)
    if (fContext) {
      GrGLenum format = kUnknown_SkColorType;
      const SkColorType color_type =
          FirstSupportedColorType(fContext.get(), &format);

      GrGLFramebufferInfo framebuffer_info = {};
      framebuffer_info.fFBOID = static_cast<GrGLuint>(0);
      framebuffer_info.fFormat = format;

      GrBackendRenderTarget render_target(fWidth,           // width
                                          fHeight,          // height
                                          0,                // sample count
                                          0,                // stencil bits
                                          framebuffer_info  // framebuffer info
      );

      sk_sp<SkColorSpace> colorspace = SkColorSpace::MakeSRGB();
      SkSurfaceProps surface_props(0, kUnknown_SkPixelGeometry);

      return SkSurface::MakeFromBackendRenderTarget(
          fContext.get(),                                // Gr context
          render_target,                                 // render target
          GrSurfaceOrigin::kBottomLeft_GrSurfaceOrigin,  // origin
          color_type,                                    // color type
          colorspace,                                    // colorspace
          &surface_props                                 // surface properties
      );
    }

    // TODO(forevermeng)
    /*if (fContext) {
      GrGLint buffer;
      fBackendContext.get()->fFunctions.fGetIntegerv(GR_GL_FRAMEBUFFER_BINDING,
                                                     &buffer);
      GrGLFramebufferInfo fbInfo;
      fbInfo.fFBOID = buffer;
      fbInfo.fFormat = GR_GL_RGBA8;
      GrBackendRenderTarget backendRT(fWidth, fHeight, fSampleCount,
                                      fStencilBits, fbInfo);
      fSurface = SkSurface::MakeFromBackendRenderTarget(
              fContext.get(), backendRT, kBottomLeft_GrSurfaceOrigin,
              kRGBA_8888_SkColorType, fDisplayParams.fColorSpace,
              &fDisplayParams.fSurfaceProps);
    }*/
  }
  return fSurface;
}
void GLWindowContext::swapBuffers() { this->onSwapBuffers(); }
void GLWindowContext::resize(int w, int h) {
  this->destroyContext();
  this->initializeContext();
}
void GLWindowContext::setDisplayParams(const DisplayParams& params) {
  fDisplayParams = params;
  this->destroyContext();
  this->initializeContext();
}
