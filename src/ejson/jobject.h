//
// Created by Alone on 2022-7-25.
//

#pragma once
#include <cstdint>
#include <limits>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
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
using dict_t   = std::unordered_map<str_t, JObject>;

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
struct ostream_helper;

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

   /**
    * Create a JObject of type dict_t
    * @return
    */
   static JObject Dict() { return JObject(dict_t{}); }
   /**
    * Create a JObject of type list_t
    * @return
    */
   static JObject List() { return JObject(list_t{}); }

#define EJSON_DECAY_IS(a, b) std::is_same<decay<a>, b>::value

   /**
    * Try to convert JObject to type std::string_view and return, throw an
    * exception if it fails
    * @tparam T
    * @return
    */
   template <class T>
   [[nodiscard]] auto cast() const ->
     typename std::enable_if<EJSON_DECAY_IS(T, str_t), T>::type
   {
      return Value<str_t>();
   }

   /**
    * Try to convert JObject to type std::string and return, throw an exception
    * if it fails
    * @tparam T
    * @return
    */
   template <class T>
   [[nodiscard]] auto cast() const ->
     typename std::enable_if<EJSON_DECAY_IS(T, std::string), T>::type
   {
      auto &str = Value<str_t>();
      return std::string({str.data(), str.size()});
   }

   /**
    * Try to convert JObject to type int64_t and return, throw an exception if
    * it fails
    * @tparam T
    * @return
    */
   template <class T>
   [[nodiscard]] auto cast() const ->
     typename std::enable_if<EJSON_DECAY_IS(T, int_t), T>::type
   {
      return Value<int_t>();
   }

   /**
    * Try to convert JObject to type int32_t and return, throw an exception if
    * it fails
    * @tparam T
    * @return
    */
   template <class T>
   [[nodiscard]] auto cast() const ->
     typename std::enable_if<EJSON_DECAY_IS(T, int32_t), T>::type
   {
      return static_cast<int32_t>(cast<int_t>());
   }

   /**
    * Try to convert JObject to type uint32_t and return, throw an exception if
    * it fails
    * @tparam T
    * @return
    */
   template <class T>
   [[nodiscard]] auto cast() const ->
     typename std::enable_if<EJSON_DECAY_IS(T, uint32_t), T>::type
   {
      return static_cast<uint32_t>(cast<int_t>());
   }

   /**
    * Try to convert JObject to type uint64_t and return, throw an exception if
    * it fails
    * @tparam T
    * @return
    */
   template <class T>
   [[nodiscard]] auto cast() const ->
     typename std::enable_if<EJSON_DECAY_IS(T, uint64_t), T>::type
   {
      return static_cast<uint64_t>(cast<int_t>());
   }

   /**
    * Try to convert JObject to type double and return, throw an exception if it
    * fails
    * @tparam T
    * @return
    */
   template <class T>
   [[nodiscard]] auto cast() const ->
     typename std::enable_if<EJSON_DECAY_IS(T, double_t), T>::type
   {
      return Value<double_t>();
   }
   /**
    * Try to convert JObject to type float and return, throw an exception if it
    * fails
    * @tparam T
    * @return
    */
   template <class T>
   [[nodiscard]] auto cast() const ->
     typename std::enable_if<EJSON_DECAY_IS(T, float), T>::type
   {
      return static_cast<float>(cast<double_t>());
   }
   /**
    * Try to convert JObject to type bool and return, throw an exception if it
    * fails
    * @tparam T
    * @return
    */
   template <class T>
   [[nodiscard]] auto cast() const ->
     typename std::enable_if<EJSON_DECAY_IS(T, bool_t), T>::type
   {
      return Value<bool_t>();
   }

   /**
    * Try to convert JObject to type T and return, throw an exception if it
    * fails
    * @tparam T
    * @return
    */
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
      // safety check
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
      // safety check
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
      // safety check
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
      // safety check
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
      // safety check
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
      // safety check
      if (m_type != kDouble) EJSON_THROW_GET_ERROR("number don't a double_t");
      void *v = value();
      if (v == nullptr)
         EJSON_THROW_ERROR_POS("unknown type in JObject::Value<double_t>()");
      return (*(number *)(v)).d_;
   }

   /**
    * Returns the JObject type
    * @return Type
    */
   [[nodiscard]] Type type() const { return m_type; }

   /**
    * Serialize to the json string
    * @param indent The indent size used for beautification, if less than 0, it
    * is not beautified
    * @param indent_char The character used to beautify the output
    * @param is_esc Whether to enable recognition of escape characters
    * @return
    */
   [[nodiscard]] string to_string(int indent = -1, char indent_char = ' ',
                                  bool is_esc = false) const;

   /**
    * Push a value to the end of the list. This method can only be used if the
    * current JObject is list_t, otherwise an exception is thrown.
    * @param item
    */
   template <class T, typename std::enable_if<EJSON_DECAY_IS(T, const char *) ||
                                                EJSON_DECAY_IS(T, std::string),
                                              bool>::type = true>
   void push_back(T &&item)
   {
      if (m_type == kList)
      {
         auto &list = Value<list_t>();
         list.push_back(JObject(str_t{item}));
         return;
      }
      EJSON_THROW_ERROR_POS("not a list type! JObjcct::push_back()");
   }

   /**
    * Push a value to the end of the list. This method can only be used if the
    * current JObject is list_t, otherwise an exception is thrown.
    * @param item
    */
   template <class T,
             typename std::enable_if<(!EJSON_DECAY_IS(T, const char *) &&
                                      !EJSON_DECAY_IS(T, std::string)),
                                     bool>::type = true>
   void push_back(T &&item)
   {
      if (m_type == kList)
      {
         auto &list = Value<list_t>();
         list.push_back(JObject(std::forward<T>(item)));
         return;
      }
      EJSON_THROW_ERROR_POS("not a list type! JObjcct::push_back()");
   }

   /**
    * Pop a value to the end of the list. This method can only be used if the
    * current JObject is list_t, otherwise an exception is thrown.
    * @param item
    */
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

   /**
    * Determine if this key is available in this dict. This method can only be
    * used if the current JObject is dict_t, otherwise an exception is thrown.
    * @param key
    * @return status of key
    */
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

   /**
    * @brief a helper for JObject to get_from and get_to
    */
   struct ObjectRef
   {
      JObject &ref;
      /**
       * Get the value from std::map and deposit it in JObject
       * @tparam T
       * @param src
       * @return
       */
      template <class T>
      ObjectRef &get_from(std::map<std::string, T> const &src)
      {
         ref = std::move(JObject(src));
         return *this;
      }

      /**
       * Get the value from std::unordered_map and deposit it in JObject
       * @tparam T
       * @param src
       * @return
       */
      template <class T>
      ObjectRef &get_from(std::unordered_map<std::string, T> const &src)
      {
         ref = std::move(JObject(src));
         return *this;
      }

      /**
       * Get the value from std::set and deposit it in JObject
       * @tparam T
       * @param src
       * @return
       */
      template <class T>
      ObjectRef &get_from(std::set<T> const &src)
      {
         ref = std::move(JObject(src));
         return *this;
      }

      /**
       * Get the value from std::unordered_set and deposit it in JObject
       * @tparam T
       * @param src
       * @return
       */
      template <class T>
      ObjectRef &get_from(std::unordered_set<T> const &src)
      {
         ref = std::move(JObject(src));
         return *this;
      }

      /**
       * Get the value from std::vector and deposit it in JObject
       * @tparam T
       * @param src
       * @return
       */
      template <class T>
      ObjectRef &get_from(std::vector<T> const &src)
      {
         ref = std::move(JObject(src));
         return *this;
      }

      /**
       * Get the value from JObject and deposit it in std::map
       * @tparam T
       * @param src
       */
      template <class T>
      ObjectRef &get_to(std::map<std::string, T> &dst)
      {
         auto &dict = ref.Value<dict_t>();
         for (auto &&v : dict)
         {
            dst[std::string({v.first.data(), v.first.size()})] =
              v.second.cast<T>();
         }
         return *this;
      }

      /**
       * Get the value from JObject and deposit it in std::unordered_map
       * @tparam T
       * @param src
       */
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

      /**
       * Get the value from JObject and deposit it in std::set
       * @tparam T
       * @param src
       */
      template <class T>
      ObjectRef &get_to(std::set<T> &dst)
      {
         auto &list = ref.Value<list_t>();
         for (auto &&v : list) { dst.insert(v); }
         return *this;
      }

      /**
       * Get the value from JObject and deposit it in std::unordered_set
       * @tparam T
       * @param src
       */
      template <class T>
      ObjectRef &get_to(std::unordered_set<T> &dst)
      {
         auto &list = ref.Value<list_t>();
         for (auto &&v : list) { dst.insert(v); }
         return *this;
      }

      /**
       * Get the value from JObject and deposit it in std::vector
       * @tparam T
       * @param src
       */
      template <class T>
      ObjectRef &get_to(std::vector<T> &dst)
      {
         auto &list = ref.Value<list_t>();
         for (auto &&v : list) { dst.push_back(v.cast<T>()); }
         return *this;
      }

      /**
       * Get the value from custom type and deposit it in JObject
       * @tparam T
       * @param src
       */
      template <typename T, typename std::enable_if<
                              !is_basic_type<decay<T>>() &&
                                !EJSON_TYPE_IS(decay<T>, std::string) &&
                                !EJSON_TYPE_IS(decay<T>, JObject),
                              bool>::type = true>
      ObjectRef &get_from(T &&src)
      {
         try
         {
            ref.m_type  = kDict;
            ref.m_value = dict_t{};
            to_json(ref, static_cast<const T &>(src));
         }
         catch (std::exception const &e)
         {
            EJSON_THROW_ERROR_WITH_TYPE(e.what(), "get_from()", T);
         }
         return *this;
      }

      /**
       * Get the value from basic type and deposit it in JObject
       * @tparam T
       * @param src
       */
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

      /**
       * Get the value from std::string and deposit it in JObject
       * @tparam T
       * @param src
       */
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

      /**
       * Get the value from JObject and deposit it in custom type
       * @tparam T
       * @param src
       */
      template <typename T, typename std::enable_if<
                              !is_basic_type<decay<T>>() &&
                                !EJSON_TYPE_IS(decay<T>, std::string) &&
                                !EJSON_TYPE_IS(decay<T>, JObject),
                              bool>::type = true>
      ObjectRef &get_to(T &dst)
      {
         try
         {
            from_json(ref, dst);
         }
         catch (std::exception const &e)
         {
            EJSON_THROW_ERROR_WITH_TYPE(e.what(), "get_to()", T);
         }
         return *this;
      }

      /**
       * Get the value from JObject and deposit it in basic type
       * @tparam T
       * @param src
       */
      template <typename T, typename std::enable_if<is_basic_type<decay<T>>(),
                                                    bool>::type = true>
      ObjectRef &get_to(T &dst)
      {
         try
         {
            dst = std::move(ref.cast<decay<T>>());
         }
         catch (std::exception const &e)
         {
            EJSON_THROW_ERROR_WITH_TYPE(e.what(), "get_to()", T);
         }
         return *this;
      }

      /**
       * Get the value from JObject and deposit it in std::string
       * @tparam T
       * @param src
       */
      template <typename T,
                typename std::enable_if<EJSON_TYPE_IS(decay<T>, std::string),
                                        bool>::type = true>
      ObjectRef &get_to(T &dst)
      {
         try
         {
            dst = ref.cast<std::string>();
         }
         catch (std::exception const &e)
         {
            EJSON_THROW_ERROR_WITH_TYPE(e.what(), "get_to()", T);
         }
         return *this;
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

   /**
    * The requirement must be dic t or list t, otherwise an exception is thrown
    * @return
    */
   [[nodiscard]] size_t size() const
   {
      if (m_type == kDict)
      {
         auto &dict = Value<dict_t>();
         return dict.size();
      }
      if (m_type == kList)
      {
         auto &list = Value<list_t>();
         return list.size();
      }
      EJSON_THROW_ERROR_POS("not dict or list type! in JObject::size()");
   }

private:
   // return the address that gets the value based on the type
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

   void to_string_impl(string_helper &out, int indent_step = -1,
                       unsigned int current_indent = 0,
                       bool         is_esc         = false) const;

private:
   value_t m_value;
   Type    m_type;
};
}   // namespace ejson

namespace ejson_literals {
/**
 * The function only works on the first call
 * @param d  Used to set the number of decimal places reserved for
 * float/double，\n By default, the entire float number is max_digits10 in
 * length
 * @return The number of bits currently reserved
 */
int float_d(int d = std::numeric_limits<ejson::double_t>::max_digits10);
}   // namespace ejson_literals
