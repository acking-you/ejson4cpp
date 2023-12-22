#include <string>

#include "ejson/parser.h"

struct person
{
   std::string name;
   int         id{};
   double      val;
};

AUTO_GEN_NON_INTRUSIVE(person, name, id, val)

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
   std::string fuzz_input(reinterpret_cast<const char *>(data), size);
   person      p;
   try
   {
      ejson::Parser::FromJSON(fuzz_input.c_str(), p);
   }
   catch (...)
   {
   }
   return 0;
}