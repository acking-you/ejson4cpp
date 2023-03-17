#define ANKERL_NANOBENCH_IMPLEMENT
#define EJSON_TAG_WITH_METHOD
#include "ejson/parser.h"

#include <doctest/doctest.h>
#include <nanobench.h>

#include <fstream>
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
TEST_SUITE_BEGIN("unittest");
TEST_CASE("test FromJson&ToJSON")
{
   const char* json1 =
     R"({"id":324,"name":"刘xx","score":{"p":2342343243242.12}})";
   const char* json2 = R"({"id":324,"name":"老王"})";
   student     stu;
   person      p;
   ejson::Parser::FromJSON(json2, p);
   ejson::Parser::FromJSON(json1, stu);
   auto j1 = ejson::Parser::ToJSON(stu);
   auto j2 = ejson::Parser::ToJSON(p);

   using namespace ejson_literals;

   // 重载字面量运算符
   auto j = R"({"id":32,"name":"测试"})"_json;
   CHECK_EQ(j.to_string(), R"({"id":32,"name":"测试"})");

   CHECK_EQ(p.id, 324);
   CHECK_EQ(p.name, "老王");
   CHECK_EQ(stu.id, 324);
   CHECK_EQ(stu.name, "刘xx");
   CHECK_EQ(stu.score.p, 2342343243242.12);

   CHECK_EQ(j1, json1);   // 会因为浮点数的精度问题导致结果不一致
   CHECK_EQ(j2, json2);
}

ENABLE_JSON_COUT(student, person, Score)

TEST_CASE("JObject, valid JsonCout")
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
   for (int i = 0; i < 10; i++)
   {
      stu.id = i;
      std::cout << stu << person << score << "\n";
   }
}

struct container
{
   std::map<std::string, int> test;
   std::vector<student>       p;
};

AUTO_GEN_NON_INTRUSIVE(container, test, p)
ENABLE_JSON_COUT(container)

TEST_CASE("JOBject, to_valid_std_container")
{
   container   p;
   const char* j =
     R"({"test":{"test":1,"test1":2},"p":[{"id":10,"name":"哈哈哈","score":{"p":1032.2}}]})";
   ejson::Parser::FromJSON(j, p);
   CHECK_EQ(p.test["test"], 1);
   CHECK_EQ(p.test["test1"], 2);
   CHECK_EQ(p.p.back().id, 10);
}

TEST_CASE("JObejct,list")
{
   std::ostringstream ss;
   auto               list = ejson::JObject::List();
   list.push_back("1");
   list.push_back(student{123, "2", Score{123.23}});
   list.to_file(ss);
   CHECK_EQ(ss.str(), R"(["1",{"id":123,"name":"2","score":{"p":123.23}}])");
}

TEST_CASE("Parser, valid_FromJSON")
{
   auto src = getSourceString();
   auto j   = ejson::Parser::FromJSON(src);
   outPutValidJson(j.to_string(2));
}

TEST_CASE("Parser, valid_FromFile")
{
   auto& j = ejson::Parser::FromFile(BASE_DIR "test.json");
   outPutValidJson(j.to_string(2));
}

TEST_CASE("Parser, valid_ToFile")
{
   std::vector<int> vec;
   student          stu;
   stu.id      = 3242;
   stu.score.p = 3243.24232;
   stu.name    = "李明";
   ejson_literals::float_d(5);
   ejson::Parser::ToFile(BASE_DIR "valid.json", stu);
}

TEST_CASE("JObject, valid_Pretty")
{
   auto src = getSourceString();
   outPutValidJson(ejson::Parser::FromJSON(src).to_string(4));
}

/**
 * test takes an alias through the method, and only supports C++17 or above.
 */
#if __cplusplus >= 2017L
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

enum class Type { V, X };

struct DataNonIntrusive
{
   int         a{};
   std::string name;
   double      s{};
   Type        x = Type::V;

   ALIAS_EJSON(a, testa)
   ALIAS_EJSON(name, namex)
   OPTION_EJSON(a, -1)
   OPTION_EJSON(name, "default value")
   OPTION_EJSON(s, 32.232)

   CUSTOM_EJSON(x, [](ejson::JObject* j, void* p, ejson::EJsonAction action) {
      switch (action)
      {
         case ejson::EJsonAction::kJsonTo: {
            auto&& cnt = *(int*)((Type*)p);
            j->at("type").get_from(cnt);
            break;
         }
         case ejson::EJsonAction::kJsonFrom: {
            auto& cnt = *(int*)((Type*)p);
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

TEST_CASE("Micro, alias")
{
   DataIntrusive    t;
   DataNonIntrusive tt;
   Parser::FromJSON(testAliasStr, t);
   Parser::FromJSON(testAliasStr, tt);
   std::cout << t << "\n" << tt;
}

TEST_CASE("Micro, option")
{
   DataIntrusive    t;
   DataNonIntrusive tt;
   Parser::FromJSON(testOptionStr, t);
   Parser::FromJSON(testOptionStr, tt);
   std::cout << t << "\n" << tt;
}

enum CustomEnum{
   kA,
   kB,
   kC
};

void processCustomEnum(JObject* j,void*v,EJsonAction action){
   switch (action)
   {
      case EJsonAction::kJsonTo:
        j->at("type").get_from(*(int*)v);
         break;
      case EJsonAction::kJsonFrom:
         j->at("type").get_to(*(int*)v);
         break;
   }
}

class CustomTypeData{
public:
   CUSTOM_EJSON(type_,processCustomEnum)
   ALIAS_EJSON(type_,type)
   ALIAS_EJSON(text_,text)

   AUTO_GEN_INTRUSIVE(CustomTypeData,type_,text_)
private:
   CustomEnum type_ = kB;
   std::string text_;
};

ENABLE_JSON_COUT(CustomTypeData)

TEST_CASE("Micro, custom") {
   CustomTypeData data;
   auto* testData = R"(
{
"type":0,
"text":"hello world"
}
)";
   Parser::FromJSON(testData,data);
   std::cout<<data;
}

#endif

TEST_SUITE_END;