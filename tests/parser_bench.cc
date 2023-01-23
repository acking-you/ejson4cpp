#include "common.h"
#include <ejson/parser.h>
#include <nanobench.h>
#include <gtest/gtest.h>

TEST(Parser, BenchM)
{
   auto           src = getSourceString();
   ejson::JObject j;

   ankerl::nanobench::Bench().minEpochIterations(10).run(
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
     "ToFile-v2:default", [&]() {
        std::ofstream os("../../to_file_out.json");
        j.to_file(os);
     });

   ankerl::nanobench::Bench().minEpochIterations(109).run(
     "ToFile-v1:default", [&]() {
        std::ofstream os("../../to_file_out.json");
        os << j.to_string();
     });

   ankerl::nanobench::Bench().minEpochIterations(109).run(
     "ToFile:default",
     [&]() { ejson::Parser::ToFile("../../to_file_out.json", j); });
   outPutValidJson(out);
}