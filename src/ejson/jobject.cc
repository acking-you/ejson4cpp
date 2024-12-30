//
// Created by Alone on 2022-7-25.
//

#include "jobject.h"

// Include platform-specific headers
#if defined(__AVX2__)   // x86 platform with AVX2
#include <immintrin.h>
#elif defined(__ARM_NEON) || defined(__ARM_NEON__)   // ARM platform with NEON
#include <arm_neon.h>
#endif

#include <array>
#include <cassert>
#include <cmath>
#include <iostream>

#include "hedley/hedley.hpp"

using namespace ejson;
// It's only to be initialized once
int ejson_literals::float_d(int d)
{
   static const int t_d = d;
   return t_d;
}

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4018)
#endif

namespace ejson {
namespace utils {
inline void append_esc(std::string &str, const char *x, size_t len)
{
   for (size_t i = 0; i < len; ++i)
   {
      uint8_t c = x[i];
      switch (c)
      {
         case '\"': str.append("\\\"", 2); break;
         case '\\': str.append("\\\\", 2); break;
         case '\b': str.append("\\b", 2); break;
         case '\f': str.append("\\f", 2); break;
         case '\n': str.append("\\n", 2); break;
         case '\r': str.append("\\r", 2); break;
         case '\t': str.append("\\t", 2); break;
         default:
            if (c <= 0x1F)
            {
               char buffer[7];
               snprintf(buffer, sizeof(buffer), "\\u%04X", c & 0xFF);
               str.append(buffer, 6);
            }
            else { str.push_back(static_cast<char>(c)); }
      }
   }
}
}   // namespace utils
namespace simd {
inline void append_esc(std::string &str, const char *x, size_t len)
{
#if defined(__AVX2__) // x86 platform with AVX2

    size_t dest_pos = 0;

    size_t i = 0;
    const size_t simd_width = 32; // 32 bytes for AVX2

    while (i + simd_width <= len)
    {
        // Load 32 bytes from the input
        __m256i input = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(&x[i]));

        // Compare input bytes with '\\', '\"', '\b', '\f', '\n', '\r', '\t'
        __m256i backslash_mask = _mm256_cmpeq_epi8(input, _mm256_set1_epi8('\\'));
        __m256i quote_mask     = _mm256_cmpeq_epi8(input, _mm256_set1_epi8('\"'));
        __m256i b_mask         = _mm256_cmpeq_epi8(input, _mm256_set1_epi8('\b'));
        __m256i f_mask         = _mm256_cmpeq_epi8(input, _mm256_set1_epi8('\f'));
        __m256i n_mask         = _mm256_cmpeq_epi8(input, _mm256_set1_epi8('\n'));
        __m256i r_mask         = _mm256_cmpeq_epi8(input, _mm256_set1_epi8('\r'));
        __m256i t_mask         = _mm256_cmpeq_epi8(input, _mm256_set1_epi8('\t'));

        __m256i escape_mask1 = _mm256_or_si256(backslash_mask, quote_mask);
        __m256i escape_mask2 = _mm256_or_si256(escape_mask1, b_mask);
        __m256i escape_mask3 = _mm256_or_si256(escape_mask2, f_mask);
        __m256i escape_mask4 = _mm256_or_si256(escape_mask3, n_mask);
        __m256i escape_mask5 = _mm256_or_si256(escape_mask4, r_mask);
        __m256i escape_mask  = _mm256_or_si256(escape_mask5, t_mask);

        // Create a bitmask from the comparison results
        int mask = _mm256_movemask_epi8(escape_mask);

        if (mask == 0)
        {
            // No bytes need escaping; copy them directly
            str.append(x + i, simd_width);
            dest_pos += simd_width;
        }
        else
        {
            // Some bytes need escaping; process them individually
            utils::append_esc(str, x + i, simd_width);
        }
        i += simd_width;
    }

    // Process any remaining bytes that don't fit into the SIMD width
    if (i < len)
    {
        utils::append_esc(str, x + i, len - i);
    }

#elif defined(__ARM_NEON) || defined(__ARM_NEON__) // ARM platform with NEON

    size_t dest_pos = 0;

    size_t i = 0;
    const size_t simd_width = 16; // 16 bytes for NEON

    while (i + simd_width <= len)
    {
        // Load 16 bytes from the input
        uint8x16_t input = vld1q_u8(reinterpret_cast<const uint8_t *>(&x[i]));

        // Compare input bytes with '\\' and '\"', '\b', '\f', '\n', '\r', '\t'
        uint8x16_t backslash_mask = vceqq_u8(input, vdupq_n_u8('\\'));
        uint8x16_t quote_mask     = vceqq_u8(input, vdupq_n_u8('\"'));
        uint8x16_t b_mask         = vceqq_u8(input, vdupq_n_u8('\b'));
        uint8x16_t f_mask         = vceqq_u8(input, vdupq_n_u8('\f'));
        uint8x16_t n_mask         = vceqq_u8(input, vdupq_n_u8('\n'));
        uint8x16_t r_mask         = vceqq_u8(input, vdupq_n_u8('\r'));
        uint8x16_t t_mask         = vceqq_u8(input, vdupq_n_u8('\t'));

        uint8x16_t escape_mask1 = vorrq_u8(backslash_mask, quote_mask);
        uint8x16_t escape_mask2 = vorrq_u8(escape_mask1, b_mask);
        uint8x16_t escape_mask3 = vorrq_u8(escape_mask2, f_mask);
        uint8x16_t escape_mask4 = vorrq_u8(escape_mask3, n_mask);
        uint8x16_t escape_mask5 = vorrq_u8(escape_mask4, r_mask);
        uint8x16_t escape_mask  = vorrq_u8(escape_mask5, t_mask);

        // Create a bitmask from the comparison results
        uint8_t mask = vmaxvq_u8(escape_mask);

        if (mask == 0)
        {
            // No bytes need escaping; copy them directly
            str.append(x + i, simd_width);
            dest_pos += simd_width;
        }
        else
        {
            // Some bytes need escaping; process them individually
            utils::append_esc(str, x + i, simd_width);
        }
        i += simd_width;
    }

    // Process any remaining bytes that don't fit into the SIMD width
    if (i < len)
    {
        utils::append_esc(str, x + i, len - i);
    }

#else // Fallback to non-SIMD implementation

    // Fallback: Process characters individually
    utils::append_esc(str, x, len);

#endif // Platform selection
}
}   // namespace simd

struct string_helper
{
   explicit string_helper(std::string &str, char indent_char = ' ')
     : indent_char_(indent_char), str_(str), indent_string_(512, indent_char)
   {
   }

   inline void resize_indent_string(size_t size)
   {
      indent_string_.resize(size, indent_char_);
   }

   inline size_t indent_string_size() { return indent_string_.size(); }

   inline const char *indent_string_data() { return indent_string_.data(); }

   inline void push_back(char x) { str_.push_back(x); }

   inline void append(const char *x, size_t len, bool is_esc = false)
   {
      // Recognition of escape characters is disabled by default
      if (!is_esc) { str_.append(x, len); }
      else { simd::append_esc(str_, x, len); }
   }

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

   char                 indent_char_;
   std::string         &str_;
   std::vector<char>    indent_string_;
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

// to simplify conversion
#define GET_VALUE(type) *((type *)value)

string JObject::to_string(const int indent, const char indent_char,
                          bool is_esc) const
{
   auto buf        = std::string();
   auto buf_helper = string_helper{buf, indent_char};
   to_string_impl(buf_helper, indent, 0, is_esc);
   return std::move(buf);
}

void JObject::to_string_impl(string_helper &out, const int indent_step,
                             const unsigned int current_indent,
                             bool               is_esc) const
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
         out.append(v.data(), v.size(), is_esc);
         out.push_back('\"');
         break;
      }
      case kList: {
         list_t &list = GET_VALUE(list_t);
         if (list.empty())
         {
            out.append("[]", 2);
            return;
         }
         // no pretty print
         if (indent_step < 0)
         {
            out.push_back('[');
            // first n-1 elements
            size_t i = 0;
            for (; i < list.size() - 1; i++)
            {
               list[i].to_string_impl(out, indent_step, current_indent, is_esc);
               out.push_back(',');
            }
            // last element

            list[i].to_string_impl(out, indent_step, current_indent, is_esc);
            out.push_back(']');
         }
         // pretty print
         else
         {
            out.append("[\n", 2);
            // variable to hold indentation for recursive calls
            const auto new_indent = current_indent + indent_step;
            // to help compiler optimize
            if (JSON_HEDLEY_UNLIKELY(new_indent > out.indent_string_size()))
            {
               out.resize_indent_string(out.indent_string_size() * 2);
            }
            // first n-1 elements
            size_t i = 0;
            for (; i < list.size() - 1; i++)
            {
               out.append(out.indent_string_data(), new_indent);
               list[i].to_string_impl(out, indent_step, new_indent, is_esc);
               out.append(",\n", 2);
            }
            // last element

            out.append(out.indent_string_data(), new_indent);
            list[i].to_string_impl(out, indent_step, new_indent, is_esc);

            out.push_back('\n');
            out.append(out.indent_string_data(), current_indent);
            out.push_back(']');
         }
         break;
      }
      case kDict: {
         dict_t &dict = GET_VALUE(dict_t);
         if (dict.empty())
         {
            out.append("{}", 2);
            return;
         }

         // no pretty print
         if (indent_step < 0)
         {
            out.push_back('{');
            auto it = dict.begin();
            // first n-1 elements
            for (auto i = 0; i < dict.size() - 1; ++i, ++it)
            {
               out.push_back('\"');
               out.append(it->first.data(), it->first.size(), is_esc);
               out.append("\":", 2);
               it->second.to_string_impl(out, indent_step, current_indent,
                                         is_esc);
               out.push_back(',');
            }
            // last elements
            out.push_back('\"');
            out.append(it->first.data(), it->first.size(), is_esc);
            out.append("\":", 2);
            it->second.to_string_impl(out, indent_step, current_indent, is_esc);
            out.push_back('}');
         }
         // pretty print
         else
         {
            out.append("{\n", 2);
            // variable to hold indentation for recursive calls
            const auto new_indent = current_indent + indent_step;
            // to help compiler optimize
            if (JSON_HEDLEY_UNLIKELY(new_indent > out.indent_string_size()))
            {
               out.resize_indent_string(out.indent_string_size() * 2);
            }
            auto it = dict.begin();
            // first n-1 elements
            for (auto i = 0; i < dict.size() - 1; ++i, ++it)
            {
               out.append(out.indent_string_data(), new_indent);
               out.push_back('\"');
               out.append(it->first.data(), it->first.size(), is_esc);
               out.append("\": ", 3);
               it->second.to_string_impl(out, indent_step, new_indent, is_esc);
               out.append(",\n", 2);
            }
            // last elements
            out.append(out.indent_string_data(), new_indent);
            out.push_back('\"');
            out.append(it->first.data(), it->first.size(), is_esc);
            out.append("\": ", 3);
            it->second.to_string_impl(out, indent_step, new_indent, is_esc);
            out.push_back('\n');

            out.append(out.indent_string_data(), current_indent);
            out.push_back('}');
         }
         break;
      }
      default: break;
   }
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif