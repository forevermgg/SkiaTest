// https://cs.android.com/android/platform/superproject/+/master:frameworks/native/libs/renderengine/include/renderengine/RenderEngine.h;drc=5ca657189aac546af0aafaba11bbc9c5d889eab3;l=246
#include <stdint.h>
#include <sys/types.h>

#include <future>
#include <memory>

struct RenderEngineCreationArgs {
  int pixelFormat;
  uint32_t imageCacheSize;

  struct Builder;

 private:
  // must be created by Builder via constructor with full argument list
  RenderEngineCreationArgs(int _pixelFormat, uint32_t _imageCacheSize)
      : pixelFormat(_pixelFormat), imageCacheSize(_imageCacheSize) {}
  RenderEngineCreationArgs() = delete;
};

struct RenderEngineCreationArgs::Builder {
    Builder() {}

    Builder& setPixelFormat(int pixelFormat) {
        this->pixelFormat = pixelFormat;
        return *this;
    }
    Builder& setImageCacheSize(uint32_t imageCacheSize) {
        this->imageCacheSize = imageCacheSize;
        return *this;
    }
    RenderEngineCreationArgs build() const {
        return RenderEngineCreationArgs(pixelFormat, imageCacheSize);
    }

private:
    // 1 means RGBA_8888
    int pixelFormat = 1;
    uint32_t imageCacheSize = 0;
};