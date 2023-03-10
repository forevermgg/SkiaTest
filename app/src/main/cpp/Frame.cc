//
// Created by centforever on 2023/3/10.
//

#include "Frame.h"

Frame::Frame() {}

Frame::Frame(int32_t width, int32_t height, int32_t bufferAge)
    : mWidth(width), mHeight(height), mBufferAge(bufferAge) {}

Frame::~Frame() {}

int32_t Frame::width() const { return mWidth; }
int32_t Frame::height() const { return mHeight; }
int32_t Frame::bufferAge() const { return mBufferAge; }

void Frame::map(const SkRect& in, int32_t* out) const {
  /* The rectangles are specified relative to the bottom-left of the surface
   * and the x and y components of each rectangle specify the bottom-left
   * position of that rectangle.
   *
   * HWUI does everything with 0,0 being top-left, so need to map
   * the rect
   */
  SkIRect idirty;
  in.roundOut(&idirty);
  int32_t y = mHeight - (idirty.y() + idirty.height());
  // layout: {x, y, width, height}
  out[0] = idirty.x();
  out[1] = y;
  out[2] = idirty.width();
  out[3] = idirty.height();
}
