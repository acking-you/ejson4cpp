#include "common.h"
#include <ejson/parser.h>
#include <nanobench.h>
#include <gtest/gtest.h>

TEST(Parser, BenchM)
{
   auto           src = getSourceString();
   ejson::JObject j;

   ankerl::nanobench::Bench().minEpochIterations(1185).run(
     "FromJson:default",
     [&]() { j = std::move(ejson::Parser::FromJSON(src)); });

   ankerl::nanobench::Bench().minEpochIterations(1185).run(
     "FromJson:have comment",
     [&]() { j = std::move(ejson::Parser::FromJSON(src, true)); });

   std::string out;
   ankerl::nanobench::Bench().minEpochIterations(1185).run(
     "ToJSON:default", [&]() { out = std::move(j.to_string()); });

   ankerl::nanobench::Bench().minEpochIterations(1185).run(
     "ToJSON:have pretty", [&]() { out = std::move(j.to_string(2)); });

   ankerl::nanobench::Bench().minEpochIterations(1185).run(
     "ToJSON:have pretty,escape",
     [&]() { out = std::move(j.to_string(2, ' ', true)); });

   outPutValidJson(out);
}

struct server
{
   int         port{};
   std::string host;
};

struct log
{
   std::string level;
   std::string filedir;
   std::string formatter;
};

struct config
{
   struct log logger;
   server     redis;
   server     mysql;
};

// auto generate log/server/config to_json and from_json
AUTO_GEN_NON_INTRUSIVE(struct log, level, filedir, formatter)
AUTO_GEN_NON_INTRUSIVE(server, host, port)
AUTO_GEN_NON_INTRUSIVE(config, logger, redis, mysql)

// enable to cout
ENABLE_JSON_COUT(struct log, server, config)

config s_config;

const char* j = R"(
{
"logger": {
  "filedir": "home/project/1",
  "formatter": "default",
  "level": "debug"
},
"mysql": {
  "host": "192.31.1.1",
  "port": 1314
},
"redis": {
  "host": "127.0.0.1",
  "port": 1444
}
})";

TEST(bench, to_struct)
{
   config config;
   ankerl::nanobench::Bench()
     .minEpochIterations(1185)
     .run("FromJSON:to_struct", [&]() { ejson::Parser::FromJSON(j, config); })
     .doNotOptimizeAway(config);
   std::cout << config;
}

TEST(bench, from_struct)
{
   config config;
   ejson::Parser::FromJSON(j, config);
   ankerl::nanobench::Bench()
     .minEpochIterations(1185)
     .run("FromJSON:to_struct", [&]() { ejson::Parser::ToJSON(config); })
     .doNotOptimizeAway(config);
}