#define ANKERL_NANOBENCH_IMPLEMENT
#include "ejson/parser.h"
#include "common.h"

#include <fstream>
#include <string>

#include "gtest/gtest.h"
#include "nanobench.h"

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
   //	FROM_JSON_FRIEND_FUNC(Value, a, b)
   //	{
   //		a.at("id").get_to(b.id_);
   //		a.at("name").get_to(b.name_);
   //	}
   //	TO_JSON_FRIEND_FUNC(Value, a, b)
   //	{
   //		a.at("id").get_from(b.id_);
   //		a.at("name").get_from(b.name_);
   //	}
   AUTO_GEN_INTRUSIVE(Value, id_, name_)
};

AUTO_GEN_NON_INTRUSIVE(person, name, id)

AUTO_GEN_NON_INTRUSIVE(Score, p)
AUTO_GEN_NON_INTRUSIVE(student, id, name, score)



TEST(Parser, FromJson)
{
   const char *json1 =
     R"({"id":324,"name":"刘xx","score":{"p":2342343243242.124}})";
   const char *json2 = R"({"id":324,"name":"老王"})";
   student     stu;
   person      p;
   ejson::Parser::FromJSON(json2, p);
   ejson::Parser::FromJSON(json1, stu);
   auto j1 = ejson::Parser::ToJSON(stu);
   auto j2 = ejson::Parser::ToJSON(p);

   using namespace ejson_literals;

   // 重载字面量运算符
   auto j = R"({"id":32,"name":"测试"})"_json;
   EXPECT_EQ(j.to_string(), R"({"id":32,"name":"测试"})");

   EXPECT_EQ(p.id, 324);
   EXPECT_EQ(p.name, "老王");
   EXPECT_EQ(stu.id, 324);
   EXPECT_EQ(stu.name, "刘xx");
   EXPECT_EQ(stu.score.p, 2342343243242.124);

   EXPECT_EQ(j1, json1);   // 会因为浮点数的精度问题导致结果不一致
   EXPECT_EQ(j2, json2);
}

ENABLE_JSON_COUT(student, person, Score)

TEST(JObject, JsonCout)
{
   student stu;
   stu.id      = 3242;
   stu.score.p = 3243.242;
   stu.name    = "李明";
   person person;
   person.name = "小明";
   person.id   = 3234234;
   Score score;
   score.p = 3234.234324;
   for (int i = 0; i < 100; i++)
   {
      stu.id = i;
      std::cout << stu << person << score << "\n";
   }
}

struct container
{
   std::unordered_map<std::string, int> test;
   std::vector<student>                 p;
};

AUTO_GEN_NON_INTRUSIVE(container, test, p)
ENABLE_JSON_COUT(container)

TEST(JOBject, to_valid_std_container)
{
   container   p;
   const char *j =
     R"({"test":{"test":1,"test1":2},"p":[{"id":10,"name":"哈哈哈","score":{"p":1032.2}}]})";
   ejson::Parser::FromJSON(j, p);
   std::cout << p;
}

TEST(JObejct,list){
   auto list =ejson::JObject::List();
   list.push_back("nihao");
   list.push_back(student{324,"fadsfas",Score{32.32}});
   std::cout<<list.to_string();
}

TEST(Parser, to_valid_text)
{
   auto src = getSourceString();
   auto j   = ejson::Parser::FromJSON(src);
   outPutValidJson(j.to_string());
}

TEST(Parser, FromFile)
{
   auto& j = ejson::Parser::FromFile(BASE_DIR"test.json");
   outPutValidJson(j.to_string(2));
}

TEST(Parser, ToFile)
{
   std::vector<int> vec;
   student          stu;
   stu.id      = 3242;
   stu.score.p = 3243.242;
   stu.name    = "李明";
   ejson::Parser::ToFile(BASE_DIR"valid.json", stu);
}

TEST(JObject, Pretty)
{
   auto src = getSourceString();
   std::cout << ejson::Parser::FromJSON(src).to_string(4);
}
