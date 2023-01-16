//
// Created by Alone on 2022-7-25.
//

#include "jobject.h"

#include <array>
#include <cassert>
#include <cmath>

using namespace ejson;
// It's only to be initialized once
int ejson_literals::float_d(int d)
{
   static const int t_d = d;
   return t_d;
}

namespace ejson {
struct string_helper
{
   explicit string_helper(std::string &str) : str_(str) {}
   void push_back(char x) { str_.push_back(x); }

   void append(const char *x, size_t len) { str_.append(x, len); }

   void push_integer(int_t value)
   {
      static constexpr std::array<std::array<char, 2>, 100> digits_to_99{{
        {{'0', '0'}}, {{'0', '1'}}, {{'0', '2'}}, {{'0', '3'}}, {{'0', '4'}},
        {{'0', '5'}}, {{'0', '6'}}, {{'0', '7'}}, {{'0', '8'}}, {{'0', '9'}},
        {{'1', '0'}}, {{'1', '1'}}, {{'1', '2'}}, {{'1', '3'}}, {{'1', '4'}},
        {{'1', '5'}}, {{'1', '6'}}, {{'1', '7'}}, {{'1', '8'}}, {{'1', '9'}},
        {{'2', '0'}}, {{'2', '1'}}, {{'2', '2'}}, {{'2', '3'}}, {{'2', '4'}},
        {{'2', '5'}}, {{'2', '6'}}, {{'2', '7'}}, {{'2', '8'}}, {{'2', '9'}},
        {{'3', '0'}}, {{'3', '1'}}, {{'3', '2'}}, {{'3', '3'}}, {{'3', '4'}},
        {{'3', '5'}}, {{'3', '6'}}, {{'3', '7'}}, {{'3', '8'}}, {{'3', '9'}},
        {{'4', '0'}}, {{'4', '1'}}, {{'4', '2'}}, {{'4', '3'}}, {{'4', '4'}},
        {{'4', '5'}}, {{'4', '6'}}, {{'4', '7'}}, {{'4', '8'}}, {{'4', '9'}},
        {{'5', '0'}}, {{'5', '1'}}, {{'5', '2'}}, {{'5', '3'}}, {{'5', '4'}},
        {{'5', '5'}}, {{'5', '6'}}, {{'5', '7'}}, {{'5', '8'}}, {{'5', '9'}},
        {{'6', '0'}}, {{'6', '1'}}, {{'6', '2'}}, {{'6', '3'}}, {{'6', '4'}},
        {{'6', '5'}}, {{'6', '6'}}, {{'6', '7'}}, {{'6', '8'}}, {{'6', '9'}},
        {{'7', '0'}}, {{'7', '1'}}, {{'7', '2'}}, {{'7', '3'}}, {{'7', '4'}},
        {{'7', '5'}}, {{'7', '6'}}, {{'7', '7'}}, {{'7', '8'}}, {{'7', '9'}},
        {{'8', '0'}}, {{'8', '1'}}, {{'8', '2'}}, {{'8', '3'}}, {{'8', '4'}},
        {{'8', '5'}}, {{'8', '6'}}, {{'8', '7'}}, {{'8', '8'}}, {{'8', '9'}},
        {{'9', '0'}}, {{'9', '1'}}, {{'9', '2'}}, {{'9', '3'}}, {{'9', '4'}},
        {{'9', '5'}}, {{'9', '6'}}, {{'9', '7'}}, {{'9', '8'}}, {{'9', '9'}},
      }};

      // special case for "0"
      if (value == 0)
      {
         str_.push_back('0');
         return;
      }

      // use a pointer to fill the buffer
      auto buffer_ptr = number_buffer_.begin();

      uint64_t abs_value;

      unsigned int n_chars{};

      if (value < 0)
      {
         *buffer_ptr = '-';
         abs_value   = remove_sign(value);

         // account one more byte for the minus sign
         n_chars = 1 + count_digits(abs_value);
      }
      else
      {
         abs_value = static_cast<uint64_t>(value);
         n_chars   = count_digits(abs_value);
      }

      // spare 1 byte for '\0'
      assert(n_chars < number_buffer_.size() - 1);

      // jump to the end to generate the string from backward,
      // so we later avoid reversing the result
      buffer_ptr += n_chars;

      // Fast int2ascii implementation inspired by "Fastware" talk by Andrei
      // Alexandrescu See: https://www.youtube.com/watch?v=o4-CwDo2zpg
      while (abs_value >= 100)
      {
         const auto digits_index = static_cast<unsigned>((abs_value % 100));
         abs_value /= 100;
         *(--buffer_ptr) = digits_to_99[digits_index][1];
         *(--buffer_ptr) = digits_to_99[digits_index][0];
      }

      if (abs_value >= 10)
      {
         const auto digits_index = static_cast<unsigned>(abs_value);
         *(--buffer_ptr)         = digits_to_99[digits_index][1];
         *(--buffer_ptr)         = digits_to_99[digits_index][0];
      }
      else { *(--buffer_ptr) = static_cast<char>('0' + abs_value); }

      str_.append(number_buffer_.data(), n_chars);
   }

   void push_float(double_t value)
   {
      // NaN / inf
      if (!std::isfinite(value))
      {
         str_.append("null", 4);
         return;
      }
      // number of decimal places
      constexpr auto d = std::numeric_limits<ejson::double_t>::max_digits10;
      const auto     float_d = ejson_literals::float_d();
      std::ptrdiff_t len;
      if (d == float_d)
      {
         len = std::snprintf(number_buffer_.data(), number_buffer_.size(),
                             "%.*g", d, value);
      }
      else
      {
         len = std::snprintf(number_buffer_.data(), number_buffer_.size(),
                             "%.*f", float_d, value);
      }
      if (len < 0) { EJSON_THROW_ERROR_POS("float change to string error"); }

      str_.append(number_buffer_.data(), len);
   }

private:
   static inline uint64_t remove_sign(int_t x) noexcept
   {
      return static_cast<uint64_t>(-(x + 1)) + 1;
   }

   static inline unsigned int count_digits(uint64_t x) noexcept
   {
      unsigned int n_digits = 1;
      for (;;)
      {
         if (x < 10) { return n_digits; }
         if (x < 100) { return n_digits + 1; }
         if (x < 1000) { return n_digits + 2; }
         if (x < 10000) { return n_digits + 3; }
         x = x / 10000u;
         n_digits += 4;
      }
   }

   std::string         &str_;
   std::array<char, 64> number_buffer_{{}};
};
}   // namespace ejson

void *JObject::value() const
{
   switch (m_type)
   {
      case kNull: return (void *)get_if<str_t>(&m_value);
      case kBool: return (void *)get_if<bool_t>(&m_value);
      case kInt:
      case kDouble: return (void *)get_if<number>(&m_value);
      case kList: return (void *)get_if<list_t>(&m_value);
      case kDict: return (void *)get_if<dict_t>(&m_value);
      case kStr: return (void *)get_if<str_t>(&m_value);
      default: return nullptr;
   }
}
// 用于简化指针强转过程的宏
#define GET_VALUE(type) *((type *)value)

string JObject::to_string()
{
   auto buf        = std::string();
   auto buf_helper = string_helper{buf};
   to_string_impl(buf_helper);
   return std::move(buf);
}

void JObject::to_string_impl(string_helper &out)
{
   void *value = this->value();
   switch (m_type)
   {
      case kNull: out.append("null", 4); break;
      case kBool:
         if (GET_VALUE(bool_t)) out.append("true", 4);
         else out.append("false", 5);
         break;
      case kInt: out.push_integer(GET_VALUE(int_t)); break;
      case kDouble: out.push_float(GET_VALUE(double_t)); break;
      case kStr: {
         out.push_back('\"');
         auto &v = GET_VALUE(str_t);
         out.append(v.data(), v.size());
         out.push_back('\"');
         break;
      }
      case kList: {
         list_t &list = GET_VALUE(list_t);
         out.push_back('[');
         for (auto i = 0; i < list.size(); i++)
         {
            if (i != list.size() - 1)
            {
               list[i].to_string_impl(out);
               out.push_back(',');
            }
            else list[i].to_string_impl(out);
         }
         out.push_back(']');
         break;
      }
      case kDict: {
         dict_t &dict = GET_VALUE(dict_t);
         out.push_back('{');
         for (auto it = dict.begin(); it != dict.end(); ++it)
         {
            if (it != dict.begin())   // 为了保证最后的json格式正确
               out.push_back(',');
            out.push_back('\"');
            out.append(it->first.data(), it->first.size());
            out.append("\":", 2);
            it->second.to_string_impl(out);
         }
         out.push_back('}');
         break;
      }
      default: break;
   }
}
