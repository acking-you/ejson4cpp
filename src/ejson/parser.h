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

class JObject;

enum class EJsonAction { kToJson, kFromJson };

using custom_func_t = void (*)(JObject *, void *, EJsonAction);

struct Parser : noncopyable
{
   template <typename T>
   using decay = std::decay<T>;

   Parser() = default;

   /**
    * Convert a JSON string represented by const char or std::string to JObject
    * @param content
    * @param skip_comment
    * @return JObject
    */
   static JObject FromJSON(const str_t &content, bool skip_comment = false);

   /**
    * Initialize the contents of dst based on the JSON string content
    * @tparam T
    * @param src
    * @param dst
    * @param skip_comment If the value is true, comments will be skipped when
    * parsing JSON
    */
   template <class T>
   static void FromJSON(string_view const &src, T &dst,
                        bool skip_comment = false)
   {
      JObject object = FromJSON(src, skip_comment);
      dst            = std::move(object.cast<T>());
   }

   /**
    * Get a JObject based on the json data in the file.\n Notice that when you
    * call this function the value of the JObject is updated.
    * @param filename
    * @param skip_comment
    * @return returned is the same JObject in the same thread.
    */
   static JObject &FromFile(string_view const &filename,
                            bool               skip_comment = false);

   /**
    * Initialize the contents of dst based on the file json contents
    * @tparam T
    * @param filename
    * @param dst
    * @param skip_comment
    */
   template <class T>
   static void FromFile(string_view const &filename, T &dst,
                        bool skip_comment = false)
   {
      JObject &object = FromFile(filename, skip_comment);
      if (object.type() != kDict)
         EJSON_THROW_ERROR_POS("not dict type in parse result");
      from_json(object, dst);
   }

   /**
    * Convert any types to JSON strings
    * @tparam T
    * @param src
    * @param indent   If the value is less than 0, the returned result is not
    * beautified, otherwise the value is the size of the indentation during
    * beautification
    * @param indent_char Characters used for indentation when beautifying
    * @param is_esc Whether escape characters need to be recognized
    * @return string
    */
   template <class T>
   static string ToJSON(T &&src, const int indent = -1,
                        const char indent_char = ' ', bool is_esc = false)
   {
      auto object = JObject(std::forward<T>(src));
      return object.to_string(indent, indent_char, is_esc);
   }

   /**
    * Store the contents of the src(JObject) in JSON format in a file
    * @param filename
    * @param src
    * @param indent
    * @param indent_char
    * @param is_esc
    */
   static void ToFile(string_view const &filename, JObject const &src,
                      const int indent = -1, const char indent_char = ' ',
                      bool is_esc = false)
   {
      std::ofstream ofs(filename.data());
      if (!ofs)
      {
         EJSON_THROW_ERROR_POS(std::string("path not exist:") +
                               std::string(filename.data(), filename.size()));
      }
      ofs << src.to_string();
   }

   /**
    * Store the contents of the src(T) in JSON format in a file
    * @tparam T
    * @param filename
    * @param src
    * @param indent
    * @param indent_char
    * @param is_esc
    */
   template <class T, typename std::enable_if<!EJSON_DECAY_IS(T, JObject),
                                              bool>::type = true>
   static void ToFile(string_view const &filename, T const &src,
                      const int indent = -1, const char indent_char = ' ',
                      bool is_esc = false)
   {
      auto object = JObject(dict_t{});
      to_json(object, static_cast<T const &>(src));
      ToFile(filename, object, indent, indent_char, is_esc);
   }

private:
   static bool valid_utf8(string_view const &src);

   inline void init(const string_view &src, bool skip_comment = false)
   {
      skip_comment_ = skip_comment;
      idx_          = 0;
      str_          = src;
   }

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
   bool   skip_comment_{};
   size_t idx_{};
   str_t  str_;
};

}   // namespace ejson

namespace ejson_literals {
/**
 * Converts string constants to JObject
 * @param json
 * @param len
 * @return  JObject
 */
inline ejson::JObject operator""_json(const char *json, size_t len)
{
   return ejson::Parser::FromJSON({json, len});
}
}   // namespace ejson_literals