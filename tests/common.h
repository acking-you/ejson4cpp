#pragma once
#include <exception>
#include <fstream>
#include <string>

// 获取用于测试的json数据
inline auto getSourceString() -> std::string
{
   auto ifs = std::ifstream(JSON_DIR "/test.json");
   if (ifs)
   {
      return std::string{std::istreambuf_iterator<char>(ifs),
                         std::istreambuf_iterator<char>()};
   }
   throw std::runtime_error("error in getSource");
}

inline void outPutValidJson(std::string const &src)
{
   auto ofs = std::ofstream(JSON_DIR"/valid.json");
   ofs << src;
}
