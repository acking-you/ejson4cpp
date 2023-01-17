//
// Created by Alone on 2022-7-25.
//

#include "parser.h"

#include <cctype>
#include <iostream>

#include "hedley/hedley.hpp"
#include "jobject.h"

using namespace ejson;

#define THROW_LOGIC(msg)                                                       \
   do {                                                                        \
      throw std::logic_error(std::string(msg) + std::string(" [") +            \
                             std::string(__FILE__ ":") +                       \
                             std::to_string(__LINE__) + std::string("] "));    \
   } while (0);

const int kNearbyLen = 20;

// Gets the contents of a string parsed by a nearby error
#define DEBUG_CONTENT                                                          \
   do {                                                                        \
      if ((int64_t)idx_ - kNearbyLen > 0) idx_ = idx_ - kNearbyLen;            \
      else idx_ = 0;                                                           \
      std::cout << "error nearby this string:\r\n\r\n\""                       \
                << str_.substr(idx_, kNearbyLen * 5) << "\"\r\n\r\n"           \
                << std::endl;                                                  \
   } while (0);

//  thread-safe
JObject Parser::FromJSON(const string_view &content, bool skip_comment)
{
   thread_local Parser instance;
   instance.init(content, skip_comment);
   return instance.parse();
}

JObject &Parser::FromFile(const string_view &filename, bool skip_comment)
{
   std::ifstream ifs(filename.data());
   if (!ifs)
   {
      EJSON_THROW_ERROR_POS(std::string("path not exist:") +
                            std::string(filename.data(), filename.size()));
   }
   // To solve two memory security problems, use thread_local:
   // 1. The lifecycle problem of the string itself. Take the string
   // thread_local
   // 2. String is updated to cause JObject dangling pointer problem. Take the
   // JObject thread_local
   thread_local std::string t_str;
   thread_local JObject     t_object;
   t_str    = std::string(std::istreambuf_iterator<char>(ifs),
                          std::istreambuf_iterator<char>());
   t_object = FromJSON(t_str, skip_comment);

   return t_object;
}

void Parser::trim_right()
{
   // Remove the tail blank to facilitate the subsequent end judgment
   int idx = static_cast<int>(str_.size() - 1);
   while (idx > 0 && isspace(str_[idx])) { idx--; }
   str_ = string_view{str_.data(), static_cast<size_t>(idx + 1)};
}

void Parser::skip_comment()
{
   if (str_.compare(idx_, 2, R"(//)") == 0)
   {
      while (true)
      {
         auto next_pos = str_.find('\n', idx_);
         if (next_pos == string::npos)
         {
            DEBUG_CONTENT
            THROW_LOGIC("invalid comment area!")
         }
         // if the next line is still a comment
         idx_ = next_pos + 1;
         while (isspace(str_[idx_])) { idx_++; }
         if (str_.compare(idx_, 2, R"(//)") != 0) { return; }
      }
   }
}

char Parser::get_next_token()
{
   while (std::isspace(str_[idx_])) idx_++;
   if (JSON_HEDLEY_UNLIKELY(idx_ >= str_.size()))
      THROW_LOGIC("unexpected character in parse json")
   // Skip comments if you need to
   if (skip_comment_) skip_comment();
   return str_[idx_];
}

bool Parser::is_esc_consume(size_t pos)
{
   size_t end_pos = pos;
   while (str_[pos] == '\\') --pos;
   // If the number of '\' is even, it cancels successfully, if it is odd, it
   // does not cancel
   return (end_pos - pos) % 2 == 0;
}

JObject Parser::parse()
{
   char token = get_next_token();
   switch (token)
   {
      case 'n': return parse_null();
      case 't':
      case 'f': return JObject(parse_bool());
      case '-':
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9': return parse_number();
      case '\"': return JObject(parse_string());
      case '[': return JObject(parse_list());
      case '{': return JObject(parse_dict());
      default: DEBUG_CONTENT THROW_LOGIC("unexpected character in parse json")
   }
}

JObject Parser::parse_null()
{
   if (str_.compare(idx_, 4, "null") == 0)
   {
      idx_ += 4;
      return {};
   }
   DEBUG_CONTENT
   THROW_LOGIC("parse null error")
}

JObject Parser::parse_number()
{
   auto begin_pos = str_.data() + idx_;
   // integer part
   if (str_[idx_] == '-') { idx_++; }
   if (isdigit(str_[idx_]))
      while (isdigit(str_[idx_])) idx_++;
   else
   {
      DEBUG_CONTENT
      THROW_LOGIC("invalid character in number")
   }

   char *end_pos{};

   if (str_[idx_] != '.')
   {
      int_t v = std::strtol(begin_pos, &end_pos, 10);
      if (JSON_HEDLEY_UNLIKELY(end_pos && (end_pos != str_.data() + idx_)))
      {
         DEBUG_CONTENT
         THROW_LOGIC("invalid character in number")
      }
      return JObject(v);
   }

   // decimal part
   idx_++;
   if (JSON_HEDLEY_UNLIKELY(!std::isdigit(str_[idx_])))
   {
      DEBUG_CONTENT
      THROW_LOGIC("at least one digit required in parse float part!")
   }
   double_t v          = strtod(begin_pos, &end_pos);
   begin_pos           = str_.data() + idx_;
   std::ptrdiff_t diff = end_pos - begin_pos;
   if (JSON_HEDLEY_UNLIKELY(diff <= 0))
   {
      DEBUG_CONTENT
      THROW_LOGIC("invalid character in number")
   }
   idx_ += diff;
   return JObject(v);
}

bool_t Parser::parse_bool()
{
   if (str_.compare(idx_, 4, "true") == 0)
   {
      idx_ += 4;
      return true;
   }
   if (str_.compare(idx_, 5, "false") == 0)
   {
      idx_ += 5;
      return false;
   }
   DEBUG_CONTENT
   THROW_LOGIC("parse bool error")
}

str_t Parser::parse_string()
{
   auto pre_pos = ++idx_;
   auto pos     = str_.find('"', idx_);
   if (pos != string::npos)
   {
      // Determines whether it is an escaped end symbol
      while (true)
      {
         // If it is not escaped, the parsing ends
         if (JSON_HEDLEY_LIKELY(str_[pos - 1] != '\\')) { break; }
         // If escape character
         if (is_esc_consume(pos - 1)) { break; }
         pos = str_.find('"', pos + 1);
         if (pos == string::npos)
         {
            DEBUG_CONTENT
            THROW_LOGIC(R"(expected left '"' in parse string)")
         }
      }
      idx_ = pos + 1;
      return str_.substr(pre_pos, pos - pre_pos);
   }
   DEBUG_CONTENT
   THROW_LOGIC("parse string error")
}

list_t Parser::parse_list()
{
   auto arr = list_t();
   idx_++;
   char ch = get_next_token();
   if (ch == ']')
   {
      idx_++;
      return arr;
   }

   while (true)
   {
      arr.push_back(std::move(parse()));
      ch = get_next_token();
      if (ch == ']')
      {
         idx_++;
         break;
      }

      if (JSON_HEDLEY_UNLIKELY(ch != ','))
      {
         DEBUG_CONTENT
         THROW_LOGIC("expected ',' in parse list")
      }
      // Skip the comma
      idx_++;
   }
   return std::move(arr);
}

dict_t Parser::parse_dict()
{
   auto dict = dict_t();
   idx_++;
   char ch = get_next_token();
   if (ch == '}')
   {
      idx_++;
      return dict;
   }
   while (true)
   {
      // Parse key
      auto key = parse().Value<str_t>();
      ch       = get_next_token();
      if (JSON_HEDLEY_UNLIKELY(ch != ':'))
      {
         DEBUG_CONTENT
         THROW_LOGIC("expected ':' in parse dict")
      }
      idx_++;

      // Parse value
      dict[key] = std::move(parse());
      ch        = get_next_token();
      // Parse completed
      if (ch == '}')
      {
         idx_++;
         break;
      }
      // if is no end. this must be a comma
      if (JSON_HEDLEY_UNLIKELY(ch != ','))
      {
         DEBUG_CONTENT
         THROW_LOGIC("expected ',' in parse dict")
      }
      // Skip the comma
      idx_++;
   }
   return dict;
}

