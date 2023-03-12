//
// Created by centforever on 2023/3/11.
//

#include "WindowContext.h"

WindowContext::WindowContext(const DisplayParams& params)
    : fDisplayParams(params), fSampleCount(1), fStencilBits(0) {}
WindowContext::~WindowContext() {}