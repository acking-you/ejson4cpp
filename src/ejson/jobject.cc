//
// Created by Alone on 2022-7-25.
//

#include "jobject.h"

using namespace ejson;

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
   void              *value = this->value();
   std::ostringstream os;
   switch (m_type)
   {
      case kNull: os << "null"; break;
      case kBool:
         if (GET_VALUE(bool_t)) os << "true";
         else os << "false";
         break;
      case kInt: os << GET_VALUE(int_t); break;
      case kDouble: os << std::fixed << GET_VALUE(double_t); break;
      case kStr: os << '\"' << GET_VALUE(str_t) << '\"'; break;
      case kList: {
         list_t &list = GET_VALUE(list_t);
         os << '[';
         for (auto i = 0; i < list.size(); i++)
         {
            if (i != list.size() - 1)
            {
               os << ((list[i]).to_string());
               os << ',';
            }
            else os << ((list[i]).to_string());
         }
         os << ']';
         break;
      }
      case kDict: {
         dict_t &dict = GET_VALUE(dict_t);
         os << '{';
         for (auto it = dict.begin(); it != dict.end(); ++it)
         {
            if (it != dict.begin())   // 为了保证最后的json格式正确
               os << ',';
            os << '\"' << it->first << "\":" << it->second.to_string();
         }
         os << '}';
         break;
      }
      default: return "";
   }
   return os.str();
}
