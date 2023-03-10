#ifndef MACROS_H
#define MACROS_H

#include <type_traits>

#define PREVENT_COPY_AND_ASSIGN(Type) \
 private:                             \
  Type(const Type&) = delete;         \
  void operator=(const Type&) = delete

#define HASHABLE_TYPE(Type)                                              \
  bool operator==(const Type& other) const;                              \
  hash_t hash() const;                                                   \
  bool operator!=(const Type& other) const { return !(*this == other); } \
  friend inline hash_t hash_type(const Type& entry) { return entry.hash(); }

#define REQUIRE_COMPATIBLE_LAYOUT(Type)               \
  static_assert(std::is_standard_layout<Type>::value, \
                #Type " must have standard layout")

#define WARN_UNUSED_RESULT __attribute__((warn_unused_result))

#if ALIGN_DOUBLE
#define ALIGN_SZ (sizeof(double))
#else
#define ALIGN_SZ (sizeof(int))
#endif

#define ALIGN(x) (((x) + ALIGN_SZ - 1) & ~(ALIGN_SZ - 1))
#define ALIGN_PTR(p) ((void*)(ALIGN((size_t)(p))))

#endif /* MACROS_H */