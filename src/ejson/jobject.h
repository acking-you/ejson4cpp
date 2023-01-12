//
// Created by Alone on 2022-7-25.
//

#pragma once
#include "autogen.h"
#include "noncopyable.h"
#include "third_part.h"
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace ejson {
using std::map;
using std::string;
using std::stringstream;
using std::vector;

enum TYPE { T_NULL, T_BOOL, T_INT, T_DOUBLE, T_STR, T_LIST, T_DICT };

class JObject;

using null_t = bool;
using int_t = int64_t;
using bool_t = bool;
using double_t = double;
using str_t = string_view;
using list_t = vector<JObject>;
using dict_t = map<str_t, JObject>;

#define IS_TYPE(typea, typeb) std::is_same<typea, typeb>::value

template <class T> using decay = typename std::decay<T>::type;
template <class T> constexpr bool is_number() {
  return std::is_floating_point<T>::value || std::is_integral<T>::value;
}

template <class T> constexpr bool is_basic_type() {
  return IS_TYPE(T, str_t) || IS_TYPE(T, bool_t) || IS_TYPE(T, int_t) ||
         is_number<T>();
}

struct number {
  union {
    double_t d_;
    int_t i_;
  };
  template <class T, typename std::enable_if<std::is_integral<decay<T>>::value,
                                             bool>::type = true>
  explicit number(T i) : i_(i) {}

  template <class T,
            typename std::enable_if<std::is_floating_point<decay<T>>::value,
                                    bool>::type = true>
  explicit number(T d) : d_(d) {}
};

class JObject : noncopyable {
public:
  using value_t = ejson::variant<bool_t, number, str_t, list_t, dict_t>;

  struct ObjectRef {
    JObject &ref;
#if __cplusplus >= 201703L
    template <class T> ObjectRef &get_from(T &&src) {
      using RawT = decay<T>;
      try {
        if constexpr (is_basic_type<RawT>()) {
          ref = std::move(JObject(std::forward<T>(src)));
        } else if constexpr (IS_TYPE(RawT, string)) {
          ref = std::move(JObject(src));
        } else if constexpr (IS_TYPE(RawT, JObject)) {
          ref = std::forward<T>(src);
        } else {
          auto tmp = JObject(dict_t{});
          to_json(tmp, static_cast<const T &>(src));
          ref = std::move(tmp);
        }
      } catch (std::exception const &e) {
        EJSON_THROW_ERROR_WITH_TYPE(e.what(), "get_from()", src);
      }
      return *this;
    }
    template <class T> void get_to(T &dst) {
      using RawT = decay<T>;
      try {
        if constexpr (is_basic_type<RawT>()) {
          dst = std::move(ref.Value<RawT>());
        } else if constexpr (IS_TYPE(RawT, string)) {
          dst = ref.Value<str_t>();
        } else {
          from_json(ref, dst);
        }
      } catch (std::exception const &e) {
        EJSON_THROW_ERROR_WITH_TYPE(e.what(), "get_to()", dst);
      }
    }
#else
    template <typename T,
              typename std::enable_if<!is_basic_type<decay<T>>() &&
                                          !IS_TYPE(decay<T>, std::string) &&
                                          !IS_TYPE(decay<T>, JObject),
                                      bool>::type = true>
    ObjectRef &get_from(T &&src) {
      try {
        auto tmp = JObject(dict_t{});
        to_json(tmp, static_cast<const T &>(src));
        ref = std::move(tmp);
      } catch (std::exception const &e) {
        EJSON_THROW_ERROR_WITH_TYPE(e.what(), "get_from()", T);
      }
      return *this;
    }
    template <typename T, typename std::enable_if<is_basic_type<decay<T>>(),
                                                  bool>::type = true>
    ObjectRef &get_from(T &&src) {
      try {
        ref = std::move(JObject(std::forward<T>(src)));
      } catch (std::exception const &e) {
        EJSON_THROW_ERROR_WITH_TYPE(e.what(), "get_from()", T);
      }
      return *this;
    }
    template <typename T,
              typename std::enable_if<IS_TYPE(decay<T>, std::string),
                                      bool>::type = true>
    ObjectRef &get_from(T &&src) {
      try {
        ref = std::move(JObject(std::forward<T>(src)));
      } catch (std::exception const &e) {
        EJSON_THROW_ERROR_WITH_TYPE(e.what(), "get_from()", T);
      }
      return *this;
    }
    template <typename T, typename std::enable_if<IS_TYPE(decay<T>, JObject),
                                                  bool>::type = true>
    ObjectRef &get_from(T &&src) {
      try {
        ref = std::move(JObject(std::forward<T>(src)));
      } catch (std::exception const &e) {
        EJSON_THROW_ERROR_WITH_TYPE(e.what(), "get_from()", T);
      }
      return *this;
    }
    template <typename T,
              typename std::enable_if<!is_basic_type<decay<T>>() &&
                                          !IS_TYPE(decay<T>, std::string) &&
                                          !IS_TYPE(decay<T>, JObject),
                                      bool>::type = true>
    void get_to(T &dst) {
      try {
        from_json(ref, dst);
      } catch (std::exception const &e) {
        EJSON_THROW_ERROR_WITH_TYPE(e.what(), "get_to()", T);
      }
    }
    template <typename T, typename std::enable_if<is_basic_type<decay<T>>(),
                                                  bool>::type = true>
    void get_to(T &dst) {
      try {
        dst = std::move(ref.Value<decay<T>>());
      } catch (std::exception const &e) {
        EJSON_THROW_ERROR_WITH_TYPE(e.what(), "get_to()", T);
      }
    }
    template <typename T,
              typename std::enable_if<IS_TYPE(decay<T>, std::string),
                                      bool>::type = true>
    void get_to(T &dst) {
      try {
        auto str = ref.Value<str_t>();
        dst = std::string(str.data(), str.length());
      } catch (std::exception const &e) {
        EJSON_THROW_ERROR_WITH_TYPE(e.what(), "get_to()", T);
      }
    }
    template <typename T, typename std::enable_if<IS_TYPE(decay<T>, JObject),
                                                  bool>::type = true>
    void get_to(T &dst) {
      try {
        dst = std::move(ref);
      } catch (std::exception const &e) {
        EJSON_THROW_ERROR_WITH_TYPE(e.what(), "get_to()", T);
      }
    }
#endif
  };

  JObject(JObject &&) noexcept = default;
  JObject &operator=(JObject &&) = default;

  JObject() : m_value(false), m_type(T_NULL) {}

  template <class T, typename std::enable_if<std::is_integral<decay<T>>::value,
                                             bool>::type = true>
  explicit JObject(T value) : m_value(number{value}), m_type(T_INT) {}

  template <class T,
            typename std::enable_if<std::is_floating_point<decay<T>>::value,
                                    bool>::type = true>
  explicit JObject(T value) : m_value(number{value}), m_type(T_DOUBLE) {}

  explicit JObject(bool_t value) : m_value(value), m_type(T_BOOL) {}

  explicit JObject(str_t value) : m_value(value), m_type(T_STR) {}

  explicit JObject(list_t value) : m_value(std::move(value)), m_type(T_LIST) {}

  explicit JObject(dict_t value) : m_value(std::move(value)), m_type(T_DICT) {}

  template <typename T,
            typename std::enable_if<!is_basic_type<decay<T>>() &&
                                        !IS_TYPE(decay<T>, std::string) &&
                                        !IS_TYPE(std::decay<T>, char *),
                                    bool>::type = true>
  explicit JObject(T &&value) : m_value(dict_t{}), m_type(T_DICT) {
    to_json(*this, value);
  }

#if __cplusplus >= 201703L
  template <class V> [[nodiscard]] V &Value() const {
    // 添加安全检查
    if constexpr (IS_TYPE(V, str_t)) {
      if (m_type != T_STR)
        EJSON_THROW_GET_ERROR("string");
    } else if constexpr (IS_TYPE(V, bool_t)) {
      if (m_type != T_BOOL)
        EJSON_THROW_GET_ERROR("BOOL");
    } else if constexpr (is_number<V>()) {
      if (m_type != T_INT && m_type != T_DOUBLE)
        EJSON_THROW_GET_ERROR("NUMBER");
    } else if constexpr (IS_TYPE(V, list_t)) {
      if (m_type != T_LIST)
        EJSON_THROW_GET_ERROR("LIST");
    } else if constexpr (IS_TYPE(V, dict_t)) {
      if (m_type != T_DICT)
        EJSON_THROW_GET_ERROR("DICT");
    }

    void *v = value();
    if (v == nullptr)
      EJSON_THROW_ERROR_POS("unknown type in JObject::Value()");
    // 为了解决number类型之间的互转
    if constexpr (is_number<V>()) {
      thread_local V local_v;
      if (m_type == T_INT) {
        local_v = ((number *)(v))->i_;
      } else if (m_type == T_DOUBLE) {
        local_v = ((number *)(v))->d_;
      }
      return local_v;
    }

    return *((V *)v);
  }
#else
  template <typename V,
            typename std::enable_if<IS_TYPE(V, str_t), bool>::type = true>
  [[nodiscard]] V &Value() const {
    // 添加安全检查
    if (m_type != T_STR)
      EJSON_THROW_GET_ERROR("string");
    void *v = value();
    if (v == nullptr)
      EJSON_THROW_ERROR_POS("unknown type in JObject::Value()");
    return *((V *)v);
  }
  template <typename V,
            typename std::enable_if<IS_TYPE(V, bool_t), bool>::type = true>
  [[nodiscard]] V &Value() const {
    // 添加安全检查
    if (m_type != T_BOOL)
      EJSON_THROW_GET_ERROR("bool_t");
    void *v = value();
    if (v == nullptr)
      EJSON_THROW_ERROR_POS("unknown type in JObject::Value()");
    return *((V *)v);
  }
  template <typename V,
            typename std::enable_if<IS_TYPE(V, list_t), bool>::type = true>
  [[nodiscard]] V &Value() const {
    // 添加安全检查
    if (m_type != T_LIST)
      EJSON_THROW_GET_ERROR("list_t");
    void *v = value();
    if (v == nullptr)
      EJSON_THROW_ERROR_POS("unknown type in JObject::Value()");
    return *((V *)v);
  }
  template <typename V,
            typename std::enable_if<IS_TYPE(V, dict_t), bool>::type = true>
  [[nodiscard]] V &Value() const {
    // 添加安全检查
    if (m_type != T_DICT)
      EJSON_THROW_GET_ERROR("dict_t");
    void *v = value();
    if (v == nullptr)
      EJSON_THROW_ERROR_POS("unknown type in JObject::Value()");
    return *((V *)v);
  }
  template <typename V,
            typename std::enable_if<is_number<V>(), bool>::type = true>
  [[nodiscard]] V &Value() const {
    // 添加安全检查
    if (m_type != T_INT && m_type != T_DOUBLE)
      EJSON_THROW_GET_ERROR("number_t");
    void *v = value();
    if (v == nullptr)
      EJSON_THROW_ERROR_POS("unknown type in JObject::Value()");
    // 解决number结构体互转问题
    thread_local V local_v;
    if (m_type == T_INT) {
      local_v = ((number *)(v))->i_;
    } else if (m_type == T_DOUBLE) {
      local_v = ((number *)(v))->d_;
    }
    return local_v;
  }
#endif

  TYPE Type() { return m_type; }

  string to_string();

  void push_back(JObject item) {
    if (m_type == T_LIST) {
      auto &list = Value<list_t>();
      list.push_back(std::move(item));
      return;
    }
    EJSON_THROW_ERROR_POS("not a list type! JObjcct::push_back()");
  }

  void pop_back() {
    if (m_type == T_LIST) {
      auto &list = Value<list_t>();
      list.pop_back();
      return;
    }
    EJSON_THROW_ERROR_POS("not list type! JObjcct::pop_back()");
  }

  [[nodiscard]] bool HasKey(const str_t &key) const {
    if (m_type == T_DICT) {
      auto &dict = Value<dict_t>();
      return dict.find(key) != dict.end();
    }
    EJSON_THROW_ERROR_POS(
        std::string("not dict type! in JObject::HasKey() key=") +
        std::string(key.data(), key.length()));
  }

  [[nodiscard]] ObjectRef at(const str_t &key) const {
    if (m_type == T_DICT) {
      auto &dict = Value<dict_t>();
      return ObjectRef{dict[key]};
    }
    EJSON_THROW_ERROR_POS(std::string("not dict type! in JObject::at() key=") +
                          std::string(key.data(), key.length()));
  }

private:
  // 根据类型获取值的地址，直接硬转为void*类型，然后外界调用Value函数进行类型的强转
  [[nodiscard]] void *value() const;

private:
  value_t m_value;
  TYPE m_type;
};
} // namespace ejson
