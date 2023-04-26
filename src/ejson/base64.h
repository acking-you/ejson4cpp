#pragma once
#include "third_part.h"

namespace ejson {
std::string base64_encode(string_view const &input);
std::string base64_decode(string_view const &input);
}   // namespace ejson