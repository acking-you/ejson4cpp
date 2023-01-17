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
