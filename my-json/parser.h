//
// Created by Alone on 2022-7-25.
//

#ifndef MYUTIL_PARSER_H
#define MYUTIL_PARSER_H

#include "jobject.h"
#include "autogen.h"
#include "noncopyable.h"

#include <sstream>
#include <string>
#include <string_view>

namespace json
{

	using std::string;
	using std::string_view;
	using std::stringstream;

	class JObject;

	class Parser : noncopyable
	{
	 public:
		Parser() = default;

		static JObject FromString(str_t content);

		template<typename T> using decay = std::decay<T>;

		template<class T>
		static string ToJSON(T&& src)
		{
			// 如果是基本类型
			using RawT = decay<T>;
			if constexpr (is_basic_type<RawT>())
			{
				JObject object(src);
				return object.to_string();
			}
			// 调用T类型对应的to_json方法，然后to_string即可
			auto tmp = JObject(dict_t{});
			to_json(tmp, src);
			return tmp.to_string();
		}

		template<class T>
		static void FromJson(string_view src, T& dst)
		{
			JObject object = FromString(src);
			// 如果是基本类型
			using RawT = decay<T>;
			if constexpr (is_basic_type<RawT>())
			{
				dst = std::move(object.template Value<RawT>());
				return;
			}
			// 调用T类型对应的from_json方法
			if (object.Type() != T_DICT)
				throw std::logic_error("not dict type in parse result");
			from_json(object, dst);
		}

		void init(string_view src);

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
} // namespace json

#endif // MYUTIL_PARSER_H
