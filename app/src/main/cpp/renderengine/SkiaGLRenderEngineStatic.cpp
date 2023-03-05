#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <vector>

#include "../utils/Errors.h"
#include "../common/types.h"

using namespace android;
static status_t selectConfigForAttribute(EGLDisplay dpy, EGLint const* attrs,
                                         EGLint attribute, EGLint wanted,
                                         EGLConfig* outConfig) {
  EGLint numConfigs = -1, n = 0;
  eglGetConfigs(dpy, nullptr, 0, &numConfigs);
  std::vector<EGLConfig> configs(numConfigs, EGL_NO_CONFIG_KHR);
  eglChooseConfig(dpy, attrs, configs.data(), configs.size(), &n);
  configs.resize(n);

  if (!configs.empty()) {
    if (attribute != EGL_NONE) {
      for (EGLConfig config : configs) {
        EGLint value = 0;
        eglGetConfigAttrib(dpy, config, attribute, &value);
        if (wanted == value) {
          *outConfig = config;
          return NO_ERROR;
        }
      }
    } else {
      // just pick the first one
      *outConfig = configs[0];
      return NO_ERROR;
    }
  }
  return NAME_NOT_FOUND;
}

static status_t selectEGLConfig(EGLDisplay display, EGLint format,
                                EGLint renderableType, EGLConfig* config) {
  // select our EGLConfig. It must support EGL_RECORDABLE_ANDROID if
  // it is to be used with WIFI displays
  status_t err;
  EGLint wantedAttribute;
  EGLint wantedAttributeValue;

  std::vector<EGLint> attribs;
  if (renderableType) {
    const PixelFormat pixelFormat = static_cast<PixelFormat>(static_cast<const PixelFormat>(format));
    const bool is1010102 = pixelFormat == PixelFormat::RGBA_1010102;

    // Default to 8 bits per channel.
    const EGLint tmpAttribs[] = {
        EGL_RENDERABLE_TYPE,
        renderableType,
        EGL_RECORDABLE_ANDROID,
        EGL_TRUE,
        EGL_SURFACE_TYPE,
        EGL_WINDOW_BIT | EGL_PBUFFER_BIT,
        EGL_FRAMEBUFFER_TARGET_ANDROID,
        EGL_TRUE,
        EGL_RED_SIZE,
        is1010102 ? 10 : 8,
        EGL_GREEN_SIZE,
        is1010102 ? 10 : 8,
        EGL_BLUE_SIZE,
        is1010102 ? 10 : 8,
        EGL_ALPHA_SIZE,
        is1010102 ? 2 : 8,
        EGL_NONE,
    };
    std::copy(tmpAttribs, tmpAttribs + (sizeof(tmpAttribs) / sizeof(EGLint)),
              std::back_inserter(attribs));
    wantedAttribute = EGL_NONE;
    wantedAttributeValue = EGL_NONE;
  } else {
    // if no renderable type specified, fallback to a simplified query
    wantedAttribute = EGL_NATIVE_VISUAL_ID;
    wantedAttributeValue = format;
  }

  err = selectConfigForAttribute(display, attribs.data(), wantedAttribute,
                                 wantedAttributeValue, config);
  if (err == NO_ERROR) {
    EGLint caveat;
    if (eglGetConfigAttrib(display, *config, EGL_CONFIG_CAVEAT, &caveat)) {
      // ALOGW_IF(caveat == EGL_SLOW_CONFIG, "EGL_SLOW_CONFIG selected!");
    }
  }
  return err;
}

static bool checkSupportEgl() {
  EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  if (!eglInitialize(display, nullptr, nullptr)) {
    return false;
  }
  return true;
}

static status_t getEglVersion() {
  // initialize EGL for the default display
  EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  if (!eglInitialize(display, nullptr, nullptr)) {
    return UNKNOWN_ERROR;
  }

  const auto eglVersion = eglQueryString(display, EGL_VERSION);
  if (!eglVersion) {
    return BAD_VALUE;
  }
  return OK;
}

