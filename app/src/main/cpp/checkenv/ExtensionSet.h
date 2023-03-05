//
// Created by centforever on 2023/3/5.
//
#include <string>
#include <unordered_set>

#ifndef SKIA_TEST_EXTENSION_SET_H
#define SKIA_TEST_EXTENSION_SET_H

class ExtensionSet {
 public:
  ExtensionSet(const char* extensions) {
    char const* curr = extensions;
    char const* head = curr;
    do {
      head = strchr(curr, ' ');
      size_t len = head ? head - curr : strlen(curr);
      if (len > 0) {
        mExtensions.emplace(curr, len);
      }
      curr = head + 1;
    } while (head);
  }

  bool hasExtension(const char* extension) const {
    return mExtensions.count(extension) > 0;
  }

 private:
  std::unordered_set<std::string> mExtensions;
};

#endif  // SKIA_TEST_EXTENSION_SET_H
