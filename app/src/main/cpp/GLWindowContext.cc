//
// Created by centforever on 2023/3/11.
//

#include <GLES/gl.h>
#include "GLWindowContext.h"

#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "include/gpu/GrBackendSurface.h"
#include "include/gpu/GrDirectContext.h"
#include "include/gpu/gl/GrGLTypes.h"
#include "include/core/SkMatrix.h"
#include "include/gpu/gl/GrGLDefines.h"


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
      GrGLint buffer;
      fBackendContext.get()->fFunctions.fGetIntegerv(GR_GL_FRAMEBUFFER_BINDING, &buffer);
      GrGLFramebufferInfo fbInfo;
      fbInfo.fFBOID = buffer;
      fbInfo.fFormat = GR_GL_RGBA8;
      GrBackendRenderTarget backendRT(fWidth, fHeight, fSampleCount,
                                      fStencilBits, fbInfo);
      fSurface = SkSurface::MakeFromBackendRenderTarget(
          fContext.get(), backendRT, kBottomLeft_GrSurfaceOrigin,
          kRGBA_8888_SkColorType, fDisplayParams.fColorSpace,
          &fDisplayParams.fSurfaceProps);
    }
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
