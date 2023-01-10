#pragma once

namespace ejson {
class noncopyable {
public:
  noncopyable() = default;
  noncopyable(noncopyable const &) = delete;
  noncopyable &operator=(noncopyable const &) = delete;
  noncopyable(noncopyable &&) = default;
  noncopyable &operator=(noncopyable &&) = default;
};
} // namespace ejson
