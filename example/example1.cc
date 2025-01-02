#include <ejson/parser.h>

#include <iostream>

using namespace ejson;

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

int main()
{
   try
   {
      // init config from config.json
      Parser::FromFile(CONFIG_PATH, s_config);

      std::cout << "config init FromFile:" << s_config << "\n";

      // get JObject from json string
      auto object = Parser::FromJSON(j);
      std::cout << "JObject:" << object.to_string() << "\n";

      // init config struct from json string
      Parser::FromJSON(j, s_config);
      std::cout << "config init FromJSON:" << s_config << "\n";

      // get json string from s_config
      std::cout << "config ToJSON:" << Parser::ToJSON(s_config) << "\n";

      // write config to file
      Parser::ToFile(CONFIG_PATH, s_config);
   }
   catch (std::exception const& e)
   {
      std::cout << "have error:" << e.what();
   }
}