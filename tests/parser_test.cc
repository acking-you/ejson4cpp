#define ANKERL_NANOBENCH_IMPLEMENT
#define EJSON_TAG_WITH_METHOD
#include "ejson/parser.h"

#include <gtest/gtest.h>

#include <iostream>
#include <sstream>
#include <string>

#include "common.h"
struct Score
{
   double p;
};

struct student
{
   int         id{};
   std::string name;
   Score       score{};
};

struct person
{
   std::string name;
   int         id{};
};

class Value
{
private:
   int         id_;
   std::string name_;

public:
   AUTO_GEN_INTRUSIVE(Value, id_, name_)
};

AUTO_GEN_NON_INTRUSIVE(person, name, id)

AUTO_GEN_NON_INTRUSIVE(Score, p)
AUTO_GEN_NON_INTRUSIVE(student, id, name, score)

struct before_hook
{
   before_hook() { ejson_literals::float_d(2); }
};
before_hook s_hook;

TEST(UnitTest, FromJson_ToJSON)
{
   const char *json1 =
     R"({"id":324,"name":"刘xx","score":{"p":2342343243242.12}})";
   const char *json2 = R"({"id":324,"name":"老王"})";
   student     stu;
   person      p;
   ejson::Parser::FromJSON(json2, p);
   ejson::Parser::FromJSON(json1, stu);
   auto j1 = ejson::Parser::ToJSON(stu);
   auto j2 = ejson::Parser::ToJSON(p);

   using namespace ejson_literals;

   // Testing Literals Operator Overload
   auto j = R"({"id":32,"name":"测试"})"_json;
   EXPECT_EQ(j.to_string(), R"({"id":32,"name":"测试"})");

   EXPECT_EQ(p.id, 324);
   EXPECT_EQ(p.name, "老王");
   EXPECT_EQ(stu.id, 324);
   EXPECT_EQ(stu.name, "刘xx");
   EXPECT_EQ(stu.score.p, 2342343243242.12);

   EXPECT_EQ(j1,
             json1);   // Here there may be inconsistencies in the results due
                       // to precision issues with floating point numbers
   EXPECT_EQ(j2, json2);
}

ENABLE_JSON_COUT(student, person, Score)

TEST(UnitTest, Valid_JsonCout)
{
   student stu;
   stu.id      = 3242;
   stu.score.p = 3243.242;
   stu.name    = "李明";
   person person;
   person.name = "小明";
   person.id   = 3234234;
   Score score{};
   score.p = 3234.234324;
   std::ostringstream os;
   os << stu << person << score;
   ASSERT_EQ(
     os.str(),
     R"(student{"id":3242,"name":"李明","score":{"p":3243.24}}person{"id":3234234,"name":"小明"}Score{"p":3234.23})");
}

struct container
{
   std::map<std::string, int> test;
   std::vector<student>       p;
};

AUTO_GEN_NON_INTRUSIVE(container, test, p)
ENABLE_JSON_COUT(container)

TEST(UnitTest, Valid_Container)
{
   container   p;
   const char *j =
     R"({"test":{"test":1,"test1":2},"p":[{"id":10,"name":"hhh","score":{"p":1032.2}}]})";
   ejson::Parser::FromJSON(j, p);
   EXPECT_EQ(p.test["test"], 1);
   EXPECT_EQ(p.test["test1"], 2);
   EXPECT_EQ(p.p.back().id, 10);
}

TEST(UnitTest, List)
{
   auto list = ejson::JObject::List();
   auto stu  = student{123, "2", Score{123.23}};
   list.push_back("1");
   list.push_back(std::move(stu));
   auto json = list.to_string();
   EXPECT_EQ(json, R"(["1",{"id":123,"name":"2","score":{"p":123.23}}])");
}

TEST(UnitTest, Valid_FromJSON)
{
   auto        src = getSourceString();
   auto        j   = ejson::Parser::FromJSON(src);
   std::string value;
   j.at("a").ref.at("b").ref.at("c").ref.at("d").get_to(value);
   EXPECT_EQ(value, "test_value");
}

TEST(UnitTest, Valid_FromFile)
{
   auto &j = ejson::Parser::FromFile(JSON_DIR "/test.json");
   outPutValidJson(j.to_string(2));
}

TEST(UnitTest, Valid_StructToJSON)
{
   std::vector<int> vec;
   student          stu;
   stu.id      = 3242;
   stu.score.p = 3243.24232;
   stu.name    = "李明";
   ejson_literals::float_d(5);
   auto json    = ejson::Parser::ToJSON(stu, 2);
   auto jobject = ejson::Parser::FromJSON(json);
   EXPECT_EQ(jobject.at("id").ref.cast<int>(), stu.id);
   EXPECT_EQ(int(jobject.at("score").ref.at("p").ref.cast<double>()),
             int(stu.score.p));
   ASSERT_EQ(jobject.at("name").ref.cast<ejson::str_t>(), stu.name);
}

/**
 * test takes an alias through the method, and only supports C++17 or above.
 */
#if __cplusplus >= 201703L || (_MSC_VER && _MSVC_LANG >= 201703L)
struct DataIntrusive
{
   int         a{};
   std::string name;
   double      s{};

   ALIAS_EJSON(a, testa)
   ALIAS_EJSON(name, namex)
   OPTION_EJSON(a, -1)
   OPTION_EJSON(name, "default value")
   OPTION_EJSON(s, 32.232)

   AUTO_GEN_INTRUSIVE(DataIntrusive, a, name, s)
};

enum class DataType { V, X };

struct DataNonIntrusive
{
   int         a{};
   std::string name;
   double      s{};
   DataType    x{DataType::V};

   ALIAS_EJSON(a, testa)
   ALIAS_EJSON(name, namex)
   OPTION_EJSON(a, -1)
   OPTION_EJSON(name, "default value")
   OPTION_EJSON(s, 32.232)
   OPTION_EJSON(x, DataType::X)

   CUSTOM_EJSON(x, [](ejson::JObject *j, void *p, ejson::EJsonAction action) {
      switch (action)
      {
         case ejson::EJsonAction::kToJson: {
            auto &&cnt = *(int *)((DataType *)p);
            j->at("type").get_from(cnt);
            break;
         }
         case ejson::EJsonAction::kFromJson: {
            auto &cnt = *(int *)((DataType *)p);
            j->at("type").get_to(cnt);
            break;
         }
      }
   })
};

AUTO_GEN_NON_INTRUSIVE(DataNonIntrusive, a, name, s, x)

auto testAliasStr = R"(
{
"testa":234,
"namex":"dsfaasdsf",
"s":324324.2
}
)";

auto testOptionStr = R"(
{
"testa":234,
"name":"dsfaasdsf",
"ss":324324.2,
"type":234
}
)";

ENABLE_JSON_COUT(DataIntrusive, DataNonIntrusive)

using namespace ejson;

TEST(UnitTest, Macro_Alias)
{
   DataIntrusive    t;
   DataNonIntrusive tt;
   Parser::FromJSON(testAliasStr, t);
   Parser::FromJSON(testAliasStr, tt);
   EXPECT_EQ(t.name, tt.name);
   EXPECT_EQ(t.a, tt.a);
}

TEST(UnitTest, Micro_Option)
{
   DataIntrusive    t;
   DataNonIntrusive tt;
   Parser::FromJSON(testOptionStr, t);
   Parser::FromJSON(testOptionStr, tt);
   EXPECT_EQ(t.name, "default value");
   EXPECT_EQ(tt.name, "default value");
}

enum CustomEnum { kA, kB, kC };

void processCustomEnum(JObject *j, void *v, EJsonAction action)
{
   switch (action)
   {
      case EJsonAction::kToJson: j->at("type").get_from(*(int *)v); break;
      case EJsonAction::kFromJson: j->at("type").get_to(*(int *)v); break;
   }
}

class CustomTypeData
{
public:
   CUSTOM_EJSON(type_, processCustomEnum)
   ALIAS_EJSON(type_, type)
   ALIAS_EJSON(text_, text)

   AUTO_GEN_INTRUSIVE(CustomTypeData, type_, text_)

   auto get_enum() -> CustomEnum { return type_; }

private:
   CustomEnum  type_ = kB;
   std::string text_;
};

ENABLE_JSON_COUT(CustomTypeData)

TEST(UnitTest, Macro_Custom)
{
   CustomTypeData data;
   auto          *testData = R"(
{
"type":0,
"text":"hello world"
}
)";
   Parser::FromJSON(testData, data);
   EXPECT_EQ(int(data.get_enum()), 0);
}
#endif
