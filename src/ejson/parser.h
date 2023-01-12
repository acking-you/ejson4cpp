//
// Created by Alone on 2022-7-25.
//

#pragma once

#include "jobject.h"

#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

namespace ejson {

using std::string;
using std::stringstream;

class JObject;

class Parser : noncopyable {
public:
  Parser() = default;

  static JObject FromString(const str_t &content);

  template <typename T> using decay = std::decay<T>;

#if __cplusplus >= 201703L
  template <class T> static string ToJSON(T &&src) {
    // 如果是基本类型
    using RawT = decay<T>;
    if constexpr (is_basic_type<RawT>()) {
      JObject object(src);
      return object.to_string();
    }
    // 调用T类型对应的to_json方法，然后to_string即可
    auto tmp = JObject(dict_t{});
    to_json(tmp, src);
    return tmp.to_string();
  }

  template <class T> static void FromJSON(string_view src, T &dst) {
    JObject object = FromString(src);
    // 如果是基本类型
    using RawT = decay<T>;
    if constexpr (is_basic_type<RawT>()) {
      dst = std::move(object.template Value<RawT>());
      return;
    }
    // 调用T类型对应的from_json方法
    if (object.Type() != T_DICT)
      EJSON_THROW_ERROR_POS("not dict type in parse result");
    from_json(object, dst);
  }
#else
  template <class T, typename std::enable_if<is_basic_type<decay<T>>(),
                                             bool>::type = true>
  static string ToJSON(T &&src) {
    // 如果是基本类型
    JObject object(src);
    return object.to_string();
  }

  template <class T, typename std::enable_if<!is_basic_type<decay<T>>(),
                                             bool>::type = true>
  static string ToJSON(T &&src) {
    // 调用T类型对应的to_json方法，然后to_string即可
    auto tmp = JObject(dict_t{});
    to_json(tmp, src);
    return tmp.to_string();
  }

  template <class T, typename std::enable_if<is_basic_type<decay<T>>(),
                                             bool>::type = true>
  static void FromJSON(string_view const &src, T &dst) {
    JObject object = FromString(src);
    dst = std::move(object.template Value<decay<T>>());
  }

  template <class T, typename std::enable_if<!is_basic_type<decay<T>>(),
                                             bool>::type = true>
  static void FromJSON(string_view const &src, T &dst) {
    JObject object = FromString(src);
    // 调用T类型对应的from_json方法
    if (object.Type() != T_DICT)
      EJSON_THROW_ERROR_POS("not dict type in parse result");
    from_json(object, dst);
  }
#endif

  static JObject FromFile(string_view const &filename) {
    std::ifstream ifs(filename.data());
    if (!ifs) {
      EJSON_THROW_ERROR_POS(std::string("path not exist:") +
                            std::string(filename.data(), filename.size()));
    }
    thread_local std::string src;
    src = std::string(std::istreambuf_iterator<char>(ifs),
                      std::istreambuf_iterator<char>());
    return FromString(src);
  }

  template <class T> static void FromFile(string_view const &filename, T &dst) {
    JObject object = FromFile(filename);
    // 调用T类型对应的from_json方法
    if (object.Type() != T_DICT)
      EJSON_THROW_ERROR_POS("not dict type in parse result");
    from_json(object, dst);
  }

  template <class T> static void ToFile(string_view const &filename, T &&dst) {
    auto object = JObject(dict_t{});
    to_json(object, static_cast<T const &>(dst));
    std::ofstream ofs(filename.data());
    if (!ofs) {
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
  str_t m_str;
  size_t m_idx{};
};

} // namespace ejson

namespace ejson_literals {
inline ejson::JObject operator""_json(const char *json, size_t len) {
  return ejson::Parser::FromString({json, len});
}
} // namespace ejson_literals