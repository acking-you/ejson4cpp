#include <doctest/doctest.h>
#include <ejson/base64.h>

#include <string>

TEST_CASE("test base64")
{
   const char* input = "德萨发的大幅度萨芬阿迪斯发但是范德萨放到JKj";
   std::string encoded;
   std::string decoded;
   encoded = ejson::base64_encode(input);
   decoded = ejson::base64_decode(encoded);
   CHECK_EQ(input, decoded);
}
