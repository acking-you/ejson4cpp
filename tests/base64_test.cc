#include <ejson/base64.h>
#include <gtest/gtest.h>

#include <string>

TEST(UnitTest, Test_Base64)
{
   const char* input = "德萨发的大幅度萨芬阿迪斯发但是范德萨放到JKj";
   std::string encoded;
   std::string decoded;
   encoded = ejson::base64_encode(input);
   decoded = ejson::base64_decode(encoded);
   EXPECT_EQ(input, decoded);
}
