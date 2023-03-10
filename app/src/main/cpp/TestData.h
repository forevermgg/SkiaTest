//
// Created by centforever on 2023/3/5.
//
#include <memory>
#include "EglManager.h"
#ifndef SKIA_TEST_TESTDATA_H
#define SKIA_TEST_TESTDATA_H

class TestData {
 public:
  TestData();
  ~TestData();

 private:
  std::unique_ptr<EglManager> eglManager;
};

#endif  // SKIA_TEST_TESTDATA_H
