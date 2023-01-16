//
// Created by Alone on 2022-7-25.
//

#pragma once
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include<limits>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "autogen.h"
#include "noncopyable.h"
#include "third_part.h"

namespace ejson {
using std::map;
using std::string;
using std::stringstream;
using std::vector;

enum Type { kNull, kBool, kInt, kDouble, kStr, kList, kDict };

class JObject;

using null_t   = bool;
using int_t    = int64_t;
using bool_t   = bool;
using double_t = double;
using str_t    = string_view;
using list_t   = std::vector<JObject>;
using dict_t   = std::map<str_t, JObject>;

#define EJSON_TYPE_IS(typea, typeb) std::is_same<typea, typeb>::value

template <class T>
using decay = typename std::decay<T>::type;
template <class T>
constexpr bool is_number()
{
   return !std::is_same<bool_t, T>::value &&
          (std::is_floating_point<T>::value || std::is_integral<T>::value);
}

template <class T>
constexpr bool is_basic_type()
{
   return EJSON_TYPE_IS(T, str_t) || EJSON_TYPE_IS(T, bool_t) ||
          EJSON_TYPE_IS(T, int_t) || is_number<T>();
}

struct number
{
   union
   {
      double_t d_;
      int_t    i_;
   };
   template <class T, typename std::enable_if<std::is_integral<decay<T>>::value,
                                              bool>::type = true>
   explicit number(T i) : i_(i)
   {
   }

   template <class T,
             typename std::enable_if<std::is_floating_point<decay<T>>::value,
                                     bool>::type = true>
   explicit number(T d) : d_(d)
   {
   }
};

struct string_helper;

class JObject : noncopyable
{
public:
   using value_t = ejson::variant<bool_t, number, str_t, list_t, dict_t>;

   JObject(JObject &&) noexcept   = default;
   JObject &operator=(JObject &&) = default;

   JObject() : m_value(false), m_type(kNull) {}

   template <class T, typename std::enable_if<std::is_integral<decay<T>>::value,
                                              bool>::type = true>
   explicit JObject(T value) : m_value(number{value}), m_type(kInt)
   {
   }

   template <class T,
             typename std::enable_if<std::is_floating_point<decay<T>>::value,
                                     bool>::type = true>
   explicit JObject(T value) : m_value(number{value}), m_type(kDouble)
   {
   }

   explicit JObject(bool_t value) : m_value(value), m_type(kBool) {}

   explicit JObject(str_t value) : m_value(value), m_type(kStr) {}

   explicit JObject(list_t value) : m_value(std::move(value)), m_type(kList) {}

   explicit JObject(dict_t value) : m_value(std::move(value)), m_type(kDict) {}

   template <class T>
   explicit JObject(std::unordered_map<string, T> const &src)
     : JObject(dict_t{})
   {
      from_map_container(src);
   }

   template <class T>
   explicit JObject(std::map<string, T> const &src) : JObject(dict_t{})
   {
      from_map_container(src);
   }

   template <class T>
   explicit JObject(std::unordered_set<T> const &src) : JObject(list_t{})
   {
      from_list_container(src);
   }

   template <class T>
   explicit JObject(std::set<T> const &src) : JObject(list_t{})
   {
      from_list_container(src);
   }

   template <class T>
   explicit JObject(std::vector<T> const &src) : JObject(list_t{})
   {
      from_list_container(src);
   }

   template <typename T,
             typename std::enable_if<!is_basic_type<decay<T>>() &&
                                       !EJSON_TYPE_IS(decay<T>, std::string) &&
                                       !EJSON_TYPE_IS(std::decay<T>, char *),
                                     bool>::type = true>
   explicit JObject(T &&value) : m_value(dict_t{}), m_type(kDict)
   {
      to_json(*this, value);
   }

#define EJSON_DECAY_IS(a, b) std::is_same<decay<a>, b>::value

   template <class T>
   [[nodiscard]] auto cast() const ->
     typename std::enable_if<EJSON_DECAY_IS(T, str_t), T>::type
   {
      return Value<str_t>();
   }

   template <class T>
   [[nodiscard]] auto cast() const ->
     typename std::enable_if<EJSON_DECAY_IS(T, std::string), T>::type
   {
      auto &str = Value<str_t>();
      return std::string({str.data(), str.size()});
   }

   template <class T>
   [[nodiscard]] auto cast() const ->
     typename std::enable_if<EJSON_DECAY_IS(T, int_t), T>::type
   {
      return Value<int_t>();
   }

   template <class T>
   [[nodiscard]] auto cast() const ->
     typename std::enable_if<EJSON_DECAY_IS(T, int32_t), T>::type
   {
      return static_cast<int32_t>(cast<int_t>());
   }

   template <class T>
   [[nodiscard]] auto cast() const ->
     typename std::enable_if<EJSON_DECAY_IS(T, uint32_t), T>::type
   {
      return static_cast<uint32_t>(cast<int_t>());
   }

   template <class T>
   [[nodiscard]] auto cast() const ->
     typename std::enable_if<EJSON_DECAY_IS(T, uint64_t), T>::type
   {
      return static_cast<uint64_t>(cast<int_t>());
   }

   template <class T>
   [[nodiscard]] auto cast() const ->
     typename std::enable_if<EJSON_DECAY_IS(T, double_t), T>::type
   {
      return Value<double_t>();
   }

   template <class T>
   [[nodiscard]] auto cast() const ->
     typename std::enable_if<EJSON_DECAY_IS(T, float), T>::type
   {
      return static_cast<float>(cast<double_t>());
   }

   template <class T>
   [[nodiscard]] auto cast() const ->
     typename std::enable_if<EJSON_DECAY_IS(T, bool_t), T>::type
   {
      return Value<bool_t>();
   }

   template <class T,
             typename std::enable_if<
               !is_basic_type<decay<T>>() && !EJSON_DECAY_IS(T, int32_t) &&
                 !EJSON_DECAY_IS(T, uint32_t) && !EJSON_DECAY_IS(T, uint64_t) &&
                 !EJSON_DECAY_IS(T, float) && !EJSON_DECAY_IS(T, std::string),
               bool>::type = true>
   [[nodiscard]] T cast() const
   {
      auto t = T();
      from_json(*this, t);
      return std::move(t);
   }

   template <typename V, typename std::enable_if<EJSON_TYPE_IS(V, str_t),
                                                 bool>::type = true>
   [[nodiscard]] V &Value() const
   {
      // 添加安全检查
      if (m_type != kStr) EJSON_THROW_GET_ERROR("string");
      void *v = value();
      if (v == nullptr)
         EJSON_THROW_ERROR_POS("unknown type in JObject::Value()");
      return *((V *)v);
   }
   template <typename V, typename std::enable_if<EJSON_TYPE_IS(V, bool_t),
                                                 bool>::type = true>
   [[nodiscard]] V &Value() const
   {
      // 添加安全检查
      if (m_type != kBool) EJSON_THROW_GET_ERROR("bool_t");
      void *v = value();
      if (v == nullptr)
         EJSON_THROW_ERROR_POS("unknown type in JObject::Value()");
      return *((V *)v);
   }
   template <typename V, typename std::enable_if<EJSON_TYPE_IS(V, list_t),
                                                 bool>::type = true>
   [[nodiscard]] V &Value() const
   {
      // 添加安全检查
      if (m_type != kList) EJSON_THROW_GET_ERROR("list_t");
      void *v = value();
      if (v == nullptr)
         EJSON_THROW_ERROR_POS("unknown type in JObject::Value()");
      return *((V *)v);
   }
   template <typename V, typename std::enable_if<EJSON_TYPE_IS(V, dict_t),
                                                 bool>::type = true>
   [[nodiscard]] V &Value() const
   {
      // 添加安全检查
      if (m_type != kDict) EJSON_THROW_GET_ERROR("dict_t");
      void *v = value();
      if (v == nullptr)
         EJSON_THROW_ERROR_POS("unknown type in JObject::Value()");
      return *((V *)v);
   }
   template <typename V, typename std::enable_if<EJSON_DECAY_IS(V, int_t),
                                                 bool>::type = true>
   [[nodiscard]] V &Value() const
   {
      // 添加安全检查
      if (m_type != kInt) EJSON_THROW_GET_ERROR("number don't a int_t");
      void *v = value();
      if (v == nullptr)
         EJSON_THROW_ERROR_POS("unknown type in JObject::Value<int_t>()");
      return (*(number *)(v)).i_;
   }

   template <typename V, typename std::enable_if<EJSON_DECAY_IS(V, double_t),
                                                 bool>::type = true>
   [[nodiscard]] V &Value() const
   {
      // 添加安全检查
      if (m_type != kDouble) EJSON_THROW_GET_ERROR("number don't a double_t");
      void *v = value();
      if (v == nullptr)
         EJSON_THROW_ERROR_POS("unknown type in JObject::Value<double_t>()");
      return (*(number *)(v)).d_;
   }

   Type type() { return m_type; }

   string to_string();

   void push_back(JObject item)
   {
      if (m_type == kList)
      {
         auto &list = Value<list_t>();
         list.push_back(std::move(item));
         return;
      }
      EJSON_THROW_ERROR_POS("not a list type! JObjcct::push_back()");
   }

   void pop_back()
   {
      if (m_type == kList)
      {
         auto &list = Value<list_t>();
         list.pop_back();
         return;
      }
      EJSON_THROW_ERROR_POS("not list type! JObjcct::pop_back()");
   }

   [[nodiscard]] bool has_key(const str_t &key) const
   {
      if (m_type == kDict)
      {
         auto &dict = Value<dict_t>();
         return dict.find(key) != dict.end();
      }
      EJSON_THROW_ERROR_POS(
        std::string("not dict type! in JObject::HasKey() key=") +
        std::string(key.data(), key.size()));
   }

   struct ObjectRef
   {
      JObject &ref;
      template <class T>
      ObjectRef &get_from(std::map<std::string, T> const &src)
      {
         ref = std::move(JObject(src));
         return *this;
      }

      template <class T>
      ObjectRef &get_from(std::unordered_map<std::string, T> const &src)
      {
         ref = std::move(JObject(src));
         return *this;
      }

      template <class T>
      ObjectRef &get_from(std::set<T> const &src)
      {
         ref = std::move(JObject(src));
         return *this;
      }

      template <class T>
      ObjectRef &get_from(std::unordered_set<T> const &src)
      {
         ref = std::move(JObject(src));
         return *this;
      }

      template <class T>
      ObjectRef &get_from(std::vector<T> const &src)
      {
         ref = std::move(JObject(src));
         return *this;
      }

      template <class T>
      ObjectRef &get_to(std::map<std::string, T> &dst)
      {
         auto &dict = ref.Value<dict_t>();
         for (auto &&v : dict)
         {
            dst[std::string({v.first.data(), v.first.size()})] = v.second;
         }
         return *this;
      }

      template <class T>
      ObjectRef &get_to(std::unordered_map<std::string, T> &dst)
      {
         auto &dict = ref.Value<dict_t>();
         for (auto &&v : dict)
         {
            dst[std::string({v.first.data(), v.first.size()})] =
              v.second.cast<T>();
         }
         return *this;
      }

      template <class T>
      ObjectRef &get_to(std::set<T> &dst)
      {
         auto &list = ref.Value<list_t>();
         for (auto &&v : list) { dst.insert(v); }
         return *this;
      }

      template <class T>
      ObjectRef &get_to(std::unordered_set<T> &dst)
      {
         auto &list = ref.Value<list_t>();
         for (auto &&v : list) { dst.insert(v); }
         return *this;
      }

      template <class T>
      ObjectRef &get_to(std::vector<T> &dst)
      {
         auto &list = ref.Value<list_t>();
         for (auto &&v : list) { dst.push_back(v.cast<T>()); }
         return *this;
      }
      template <typename T, typename std::enable_if<
                              !is_basic_type<decay<T>>() &&
                                !EJSON_TYPE_IS(decay<T>, std::string) &&
                                !EJSON_TYPE_IS(decay<T>, JObject),
                              bool>::type = true>
      ObjectRef &get_from(T &&src)
      {
         try
         {
            auto tmp = JObject(dict_t{});
            to_json(tmp, static_cast<const T &>(src));
            ref = std::move(tmp);
         }
         catch (std::exception const &e)
         {
            EJSON_THROW_ERROR_WITH_TYPE(e.what(), "get_from()", T);
         }
         return *this;
      }
      template <typename T, typename std::enable_if<is_basic_type<decay<T>>(),
                                                    bool>::type = true>
      ObjectRef &get_from(T &&src)
      {
         try
         {
            ref = std::move(JObject(std::forward<T>(src)));
         }
         catch (std::exception const &e)
         {
            EJSON_THROW_ERROR_WITH_TYPE(e.what(), "get_from()", T);
         }
         return *this;
      }
      template <typename T,
                typename std::enable_if<EJSON_TYPE_IS(decay<T>, std::string),
                                        bool>::type = true>
      ObjectRef &get_from(T &&src)
      {
         try
         {
            ref = std::move(JObject(std::forward<T>(src)));
         }
         catch (std::exception const &e)
         {
            EJSON_THROW_ERROR_WITH_TYPE(e.what(), "get_from()", T);
         }
         return *this;
      }
      template <typename T,
                typename std::enable_if<EJSON_TYPE_IS(decay<T>, JObject),
                                        bool>::type = true>
      ObjectRef &get_from(T &&src)
      {
         try
         {
            ref = std::move(JObject(std::forward<T>(src)));
         }
         catch (std::exception const &e)
         {
            EJSON_THROW_ERROR_WITH_TYPE(e.what(), "get_from()", T);
         }
         return *this;
      }
      template <typename T, typename std::enable_if<
                              !is_basic_type<decay<T>>() &&
                                !EJSON_TYPE_IS(decay<T>, std::string) &&
                                !EJSON_TYPE_IS(decay<T>, JObject),
                              bool>::type = true>
      void get_to(T &dst)
      {
         try
         {
            from_json(ref, dst);
         }
         catch (std::exception const &e)
         {
            EJSON_THROW_ERROR_WITH_TYPE(e.what(), "get_to()", T);
         }
      }
      template <typename T, typename std::enable_if<is_basic_type<decay<T>>(),
                                                    bool>::type = true>
      void get_to(T &dst)
      {
         try
         {
            dst = std::move(ref.cast<decay<T>>());
         }
         catch (std::exception const &e)
         {
            EJSON_THROW_ERROR_WITH_TYPE(e.what(), "get_to()", T);
         }
      }
      template <typename T,
                typename std::enable_if<EJSON_TYPE_IS(decay<T>, std::string),
                                        bool>::type = true>
      void get_to(T &dst)
      {
         try
         {
            dst = ref.cast<std::string>();
         }
         catch (std::exception const &e)
         {
            EJSON_THROW_ERROR_WITH_TYPE(e.what(), "get_to()", T);
         }
      }
      template <typename T,
                typename std::enable_if<EJSON_TYPE_IS(decay<T>, JObject),
                                        bool>::type = true>
      void get_to(T &dst)
      {
         dst = std::move(ref);
      }
   };

   [[nodiscard]] ObjectRef at(const str_t &key) const
   {
      if (m_type == kDict)
      {
         auto &dict = Value<dict_t>();
         return ObjectRef{dict[key]};
      }
      EJSON_THROW_ERROR_POS(
        std::string("not dict type! in JObject::at() key=") +
        std::string(key.data(), key.length()));
   }

private:
   // 根据类型获取值的地址，直接硬转为void*类型，然后外界调用Value函数进行类型的强转
   [[nodiscard]] void *value() const;

   template <class T>
   void from_list_container(T const &src)
   {
      for (auto it = src.begin(); it != src.end(); ++it)
      {
         push_back(std::move(JObject(*it)));
      }
   }

   template <class T>
   void from_map_container(T const &src)
   {
      auto &dict = Value<dict_t>();
      for (auto it = src.begin(); it != src.end(); ++it)
      {
         dict[it->first] = std::move(JObject(it->second));
      }
   }

   void to_string_impl(string_helper &out);

private:
   value_t m_value;
   Type    m_type;
};

namespace ejson_literals {
int float_d(int d =  std::numeric_limits<ejson::double_t>::max_digits10);
}   // namespace ejson_literals

}   // namespace ejson
