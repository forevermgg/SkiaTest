#include <sys/time.h>

#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include <variant>

template <typename E>
struct Error {
  E error;
};

template <typename R, typename E>
class Result {
 public:
  Result(const R& r) : result(std::forward<R>(r)) {}
  Result(R&& r) : result(std::forward<R>(r)) {}
  Result(Error<E>&& error) : result(std::forward<Error<E>>(error)) {}

  operator bool() const { return result.index() == 0; }

  R unwrap() const { return std::get<R>(result); }

  E error() const { return std::get<Error<E>>(result).error; }

 private:
  std::variant<R, Error<E>> result;
};