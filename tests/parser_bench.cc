#include <ejson/parser.h>
#include <gtest/gtest.h>
#include <nanobench.h>

#include "common.h"

TEST(BenchMark, BenchAll)
{
   auto           src = getSourceString();
   ejson::JObject j;

   ankerl::nanobench::Bench().minEpochIterations(100).run(
     "FromJson:default",
     [&]() { j = std::move(ejson::Parser::FromJSON(src)); });

   ankerl::nanobench::Bench().minEpochIterations(109).run(
     "FromJson:have comment",
     [&]() { j = std::move(ejson::Parser::FromJSON(src, true)); });

   std::string out;
   ankerl::nanobench::Bench().minEpochIterations(109).run(
     "ToJSON:default", [&]() { out = std::move(j.to_string()); });

   ankerl::nanobench::Bench().minEpochIterations(109).run(
     "ToJSON:have pretty", [&]() { out = std::move(j.to_string(2)); });

   ankerl::nanobench::Bench().minEpochIterations(109).run(
     "ToJSON:have pretty,escape",
     [&]() { out = std::move(j.to_string(2, ' ', true)); });

   ankerl::nanobench::Bench().minEpochIterations(109).run(
     "ToFile:default",
     [&]() { ejson::Parser::ToFile("../../to_file_out.json", j); });
   outPutValidJson(out);
}

TEST(BenchMark, BenchEscapeParse)
{
   auto           src = getSourceString();
   ejson::JObject j;
   std::string    out;

   ankerl::nanobench::Bench().minEpochIterations(109).run(
     "ToJSON:have pretty,escape",
     [&]() { out = std::move(j.to_string(2, ' ', true)); });
}
