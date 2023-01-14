//
// Created by Alone on 2022-7-25.
//

#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

#include "jobject.h"

namespace ejson {

using std::string;
using std::stringstream;

class JObject;

class Parser : noncopyable
{
public:
   Parser() = default;

   static JObject FromJSON(const str_t &content);

   template <typename T>
   using decay = std::decay<T>;

   // 模板的处理已经由JObject完成，所以不需要做任何额外的限制

   template <class T>
   static string ToJSON(T &&src)
   {
      auto object = JObject(std::forward<T>(src));
      return object.to_string();
   }

   template <class T>
   static void FromJSON(string_view const &src, T &dst)
   {
      JObject object = FromJSON(src);
      dst            = std::move(object.cast<T>());
   }

   static JObject FromFile(string_view const &filename)
   {
      std::ifstream ifs(filename.data());
      if (!ifs)
      {
         EJSON_THROW_ERROR_POS(std::string("path not exist:") +
                               std::string(filename.data(), filename.size()));
      }
      thread_local std::string src;
      src = std::string(std::istreambuf_iterator<char>(ifs),
                        std::istreambuf_iterator<char>());
      return FromJSON(src);
   }

   template <class T>
   static void FromFile(string_view const &filename, T &dst)
   {
      JObject object = FromFile(filename);
      // 调用T类型对应的from_json方法
      if (object.type() != kDict)
         EJSON_THROW_ERROR_POS("not dict type in parse result");
      from_json(object, dst);
   }

   template <class T>
   static void ToFile(string_view const &filename, T &&dst)
   {
      auto object = JObject(dict_t{});
      to_json(object, static_cast<T const &>(dst));
      std::ofstream ofs(filename.data());
      if (!ofs)
      {
         EJSON_THROW_ERROR_POS(std::string("path not exist:") +
                               std::string(filename.data(), filename.size()));
      }
      ofs << (object.to_string());
   }

   void init(const string_view &src);

   void trim_right();

   void skip_comment();

   bool is_esc_consume(size_t pos);

   char get_next_token();

   JObject parse();

   JObject parse_null();

   JObject parse_number();

   bool_t parse_bool();

   str_t parse_string();

   list_t parse_list();

   dict_t parse_dict();

private:
   str_t  m_str;
   size_t m_idx{};
};

}   // namespace ejson

namespace ejson_literals {
inline ejson::JObject operator""_json(const char *json, size_t len)
{
   return ejson::Parser::FromJSON({json, len});
}
}   // namespace ejson_literals