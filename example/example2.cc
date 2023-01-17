#include <ejson/parser.h>
#include <string>

struct user_info
{
   bool        is_follow{};
   int64_t     id{};
   int64_t     follow_count{};
   int64_t     follower_count{};
   std::string name;
};

struct comment
{
   int64_t     id{};
   int64_t     user_id{};
   std::string created_date;
   std::string content;
};

AUTO_GEN_NON_INTRUSIVE(user_info, is_follow, id, follow_count, follower_count,
                       name)
AUTO_GEN_NON_INTRUSIVE(comment, id, user_id, created_date, content)

int main()
{
   // 1.get some json from frontend
   //   ...
   auto*     comment_json   = "{\n"
                              "  \"content\": \"这是一条\\\"测试\\\"评论\",\n"
                              "  \"created_date\": \"2023-01-16\",\n"
                              "  \"id\": 1,\n"
                              "  \"user_id\": 10\n"
                              "}";
   auto*     user_info_json = "{\n"
                              "  \"follow_count\": 12,\n"
                              "  \"follower_count\": 23,\n"
                              "  \"id\": 1,\n"
                              "  \"is_follow\": false,\n"
                              "  \"name\": \"某人名字\"\n"
                              "}";
   // 2.receive data as cpp struct
   comment   cmt;
   user_info uinfo;
   ejson::Parser::FromJSON(comment_json, cmt);
   ejson::Parser::FromJSON(user_info_json, uinfo);

   // 3.handle some Business logic
   //...
   // 4.save data to database(we simulate it to local file)
   auto object = ejson::JObject::Dict();
   object.at("comment").get_from(cmt);
   object.at("user_info").get_from(uinfo);
   ejson::Parser::ToFile(DATA_PATH, object);
}