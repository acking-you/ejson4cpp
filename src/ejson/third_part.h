#pragma once

#if __cplusplus >= 201703L
#include <string_view>
#include <variant>
namespace ejson {
using std::string_view;
using std::get_if;
using std::variant;
} // namespace ejson
#else
#include "third_part.impl.h"
namespace ejson {
using mpark::get_if;
using mpark::variant;
using nonstd::string_view;
} // namespace ejson
#endif