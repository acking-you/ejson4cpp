#pragma once
#include <string>
#include <fstream>
#include <exception>

// 获取用于测试的json数据
inline auto getSourceString() -> std::string
{
   auto ifs = std::ifstream(BASE_DIR "test.json");
   if (ifs)
   {
      return std::string{std::istreambuf_iterator<char>(ifs),
                         std::istreambuf_iterator<char>()};
   }
   throw std::runtime_error("error in getSource");
}

inline void outPutValidJson(std::string const &src)
{
   auto ofs = std::ofstream(BASE_DIR "valid.json");
   ofs << src;
}
