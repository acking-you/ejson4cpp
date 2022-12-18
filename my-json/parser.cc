//
// Created by Alone on 2022-7-25.
//

#include "parser.h"
#include "jobject.h"
#include <cctype>
#include <iostream>

using namespace json;
#define THROW_LOGIC(msg)                                                       \
  do {                                                                         \
    char content[512];                                                         \
    sprintf(content, "%s [%s:%d]", msg, __FILE__, __LINE__);                   \
    throw std::logic_error(content);                                           \
  } while (0);

const int kNearbyLen = 20;

#ifndef NDEBUG
// 获得附近错误解析的字符串内容
#define DEBUG_CONTENT                                                          \
  do {                                                                         \
	if (m_idx - kNearbyLen > 0)                                                \
	  m_idx = m_idx - kNearbyLen;                                              \
	std::cout << "error nearby this string:\r\n\r\n"                           \
			  << m_str.substr(m_idx) << "\r\n\r\n"                             \
			  << std::endl;                                                    \
  } while (0);
#else
#define DEBUG_CONTENT
#endif // NDEBUG

//  thread-safe
JObject Parser::FromString(string_view content)
{
	thread_local Parser instance;
	instance.init(content);
	return std::move(instance.parse());
}

void Parser::init(std::string_view src)
{
	m_str = src;
	m_idx = 0;
	trim_right(); // 去除多余空格
}

void Parser::trim_right()
{
	// 去除尾部空字符，方便最后的结束判断
	int idx = static_cast<int>(m_str.size() - 1);
	while (idx > 0 && isspace(m_str[idx]))
	{
		idx--;
	}
	m_str = string_view{ m_str.data(), static_cast<size_t>(idx + 1) };
}

void Parser::skip_comment()
{
	if (m_str.compare(m_idx, 2, R"(//)") == 0)
	{
		while (true)
		{
			auto next_pos = m_str.find('\n', m_idx);
			if (next_pos == string::npos)
			{
				DEBUG_CONTENT
				THROW_LOGIC("invalid comment area!")
			}
			// 查看下一行是否还是注释
			m_idx = next_pos + 1;
			while (isspace(m_str[m_idx]))
			{
				m_idx++;
			}
			if (m_str.compare(m_idx, 2, R"(//)") != 0)
			{ // 结束注释
				return;
			}
		}
	}
}

char Parser::get_next_token()
{
	while (std::isspace(m_str[m_idx]))
		m_idx++;
	if (m_idx >= m_str.size())
		THROW_LOGIC("unexpected character in parse json")
	// 如果是注释，记得跳过
	skip_comment();
	return m_str[m_idx];
}

bool Parser::is_esc_consume(size_t pos)
{
	size_t end_pos = pos;
	while (m_str[pos] == '\\')
		pos--;
	auto cnt = end_pos - pos;
	// 如果 \ 的个数为偶数，则成功抵消，如果为奇数，则未抵消
	return cnt % 2 == 0;
}

JObject Parser::parse()
{
	char token = get_next_token();
	switch (token)
	{
	case 'n':
		return std::move(parse_null());
	case 't':
	case 'f':
		return std::move(JObject(parse_bool()));
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
	case '9':
		return std::move(JObject(parse_number()));
	case '\"':
		return std::move(JObject(parse_string()));
	case '[':
		return std::move(JObject(parse_list()));
	case '{':
		return std::move(JObject(parse_dict()));
	default:
		DEBUG_CONTENT
		THROW_LOGIC("unexpected character in parse json")
	}
}

JObject Parser::parse_null()
{
	if (m_str.compare(m_idx, 4, "null") == 0)
	{
		m_idx += 4;
		return {};
	}
	DEBUG_CONTENT
	THROW_LOGIC("parse null error")
}

JObject Parser::parse_number()
{
	auto pos = m_idx;
	// integer part
	if (m_str[m_idx] == '-')
	{
		m_idx++;
	}
	if (isdigit(m_str[m_idx]))
		while (isdigit(m_str[m_idx]))
			m_idx++;
	else
	{
		DEBUG_CONTENT
		THROW_LOGIC("invalid character in number")
	}

	if (m_str[m_idx] != '.')
	{
		return JObject((int_t)strtoll(m_str.begin() + pos, nullptr, 10));
	}

	// decimal part
	if (m_str[m_idx] == '.')
	{
		m_idx++;
		if (!std::isdigit(m_str[m_idx]))
		{
			DEBUG_CONTENT
			THROW_LOGIC("at least one digit required in parse float part!")
		}
		while (std::isdigit(m_str[m_idx]))
			m_idx++;
	}
	return JObject(strtod(m_str.begin() + pos, nullptr));
}

bool_t Parser::parse_bool()
{
	if (m_str.compare(m_idx, 4, "true") == 0)
	{
		m_idx += 4;
		return true;
	}
	if (m_str.compare(m_idx, 5, "false") == 0)
	{
		m_idx += 5;
		return false;
	}
	DEBUG_CONTENT
	THROW_LOGIC("parse bool error")
}

str_t Parser::parse_string()
{
	auto pre_pos = ++m_idx;
	auto pos = m_str.find('"', m_idx);
	if (pos != string::npos)
	{
		// 解析还没有结束，需要判断是否是转义的结束符号，如果是转义，则需要继续探查
		while (true)
		{
			if (m_str[pos - 1] != '\\') // 如果不是转义则解析结束
			{
				break;
			}
			// 如果是转义字符，则判断是否已经被抵消，已经被消耗完则跳出，否则继续寻找下个字符串结束符
			if (is_esc_consume(pos - 1))
			{
				break;
			}
			pos = m_str.find('"', pos + 1);
			if (pos == string::npos)
			{
				DEBUG_CONTENT
				THROW_LOGIC(R"(expected left '"' in parse string)")
			}
		}
		m_idx = pos + 1;
		return m_str.substr(pre_pos, pos - pre_pos);
	}
	DEBUG_CONTENT
	THROW_LOGIC("parse string error")
}

list_t Parser::parse_list()
{
	auto arr = list_t();
	m_idx++;
	char ch = get_next_token();
	if (ch == ']')
	{
		m_idx++;
		return arr;
	}

	while (true)
	{
		arr.push_back(std::move(parse()));
		ch = get_next_token();
		if (ch == ']')
		{
			m_idx++;
			break;
		}

		if (ch != ',') // 如果不是逗号
		{
			DEBUG_CONTENT
			THROW_LOGIC("expected ',' in parse list")
		}
		// 跳过逗号
		m_idx++;
	}
	return std::move(arr);
}

dict_t Parser::parse_dict()
{
	auto dict = dict_t();
	m_idx++;
	char ch = get_next_token();
	if (ch == '}')
	{
		m_idx++;
		return dict;
	}
	while (true)
	{
		// 解析key
		auto key = parse().Value<str_t>();
		ch = get_next_token();
		if (ch != ':')
		{
			DEBUG_CONTENT
			THROW_LOGIC("expected ':' in parse dict")
		}
		m_idx++;

		// 解析value
		dict[key] = std::move(parse());
		ch = get_next_token();
		if (ch == '}')
		{
			m_idx++;
			break; // 解析完毕
		}
		if (ch != ',') // 没有结束，此时必须为逗号
		{
			DEBUG_CONTENT
			THROW_LOGIC("expected ',' in parse dict")
		}
		// 跳过逗号
		m_idx++;
	}
	return dict;
}
