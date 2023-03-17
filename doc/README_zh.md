简中 | [English](../README.md)
<!-- TOC -->
* [ejson4cpp](#ejson4cpp)
  * [新特性（C++17及以上）](#新特性c17及以上)
  * [快速开始](#快速开始)
    * [要求](#要求)
    * [安装与引入](#安装与引入)
    * [开始使用](#开始使用)
  * [常见用法](#常见用法)
  * [API介绍](#api介绍)
    * [通过命名风格识别API](#通过命名风格识别api)
    * [宏定义](#宏定义)
      * [FROM_JSON_FUNC&FROM_JSON_FRIEND_FUNC](#fromjsonfuncfromjsonfriendfunc)
      * [TO_JSON_FUNC&TO_JSON_FRIEND_FUNC](#tojsonfunctojsonfriendfunc)
      * [AUTO_GEN_NON_INTRUSIVE&AUTO_GEN_INTRUSIVE](#autogennonintrusiveautogenintrusive)
      * [ENABLE_JSON_COUT](#enablejsoncout)
    * [FromJSON系列函数](#fromjson系列函数)
      * [参数说明](#参数说明)
      * [使用示例](#使用示例)
    * [ToJSON系列函数](#tojson系列函数)
      * [参数说明](#参数说明-1)
      * [使用示例](#使用示例-1)
    * [JObject系列函数](#jobject系列函数)
      * [JObject的构造函数](#jobject的构造函数)
      * [JObject的成员函数](#jobject的成员函数)
        * [kDict](#kdict)
        * [kList](#klist)
        * [to_string](#tostring)
        * [static](#static)
  * [注意事项](#注意事项)
<!-- TOC -->
# ejson4cpp
[![License](https://img.shields.io/badge/License-MIT-green)](https://github.com/ACking-you/ejson4cpp/blob/master/LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Cross--platformable-blue)](https://img.shields.io/badge/Platform-Cross--platformable-blue)
[![Language](https://img.shields.io/badge/Language-C%2B%2B11%20or%20above-red)](https://en.cppreference.com/w/cpp/compiler_support/11)

`ejosn4cpp` ：意味着这是一个使用上非常 `easy`，同时性能上也非常 `efficiency` c++ json解析库。 支持c++11及以上，并且完全的跨平台。

* 使用 `easy` 体现在：
    1. api简单，你只需要关注两个函数，且支持一键json结构体互转。
    2. 引入简单，支持cmake命令一键引入项目并使用。
    3. 错误定位简单，无论是解析json还是序列化为json，任何错误的操作都会有详细的报错信息（模拟打印了堆栈信息），让错误定位更简单。

* 性能 `efficiency` 体现在：
  本机benchmark(3000行json)结果如图：
  ![benchmark](https://img-blog.csdnimg.cn/d79e5a8d930a43ffac5c2a445fb3e879.png)

    1. 反序列化（**Parse**)性能明显领先 `nlohmann-json` 和 `jsoncpp`，但只有 `rapidjson` 的一半性能。
    2. 序列化（**Stringify**）性能遥遥领先其他所有json库一个数量级。
    3. 查找（**FindMember**）：由于看过 `rapidjson`  源码，发现其内部每个元素的节点是以数组的形式组织的，并没有用到其他高深的数据结构，故专门对他进行成员查找测试，发现确实是 `O(n)` 级别的查找性能。

  benchmark的代码仓库：[https://github.com/ACking-you/bench_json4cpp](https://github.com/ACking-you/bench_json4cpp)
## 新特性（C++17及以上）
* 从 `v1.5.2` 开始支持从宏定义生成一个方法来控制 `json` 解析的行为。

例子：
```cpp
struct student
{
   int         id{};
   double      score{};
   std::string name;
   ALIAS_EJSON(id,studentNo) //取别名
   OPTION_EJSON(name,"null") //允许在解析时该值不存在，并在不存在时赋值为你指定的值

   AUTO_GEN_INTRUSIVE(student,id,score,name) //注册对应的字段用于json解析
};
```
这个特性同样支持非侵入式的调用。
```cpp
struct student
{
   int         id{};
   double      score{};
   std::string name;
   ALIAS_EJSON(id,studentNo) //取别名
   OPTION_EJSON(name,"null") //允许在解析时该值不存在，并在不存在时赋值为你指定的值
};
AUTO_GEN_NON_INTRUSIVE(student,id,score,name) //注册对应的字段用于json解析
```
> 请注意 `ALIAS_EJSON` 之类的宏只能在类的内部使用，且必须保证注册字段的宏代码在这些宏之后。

如果结构体中遇到无法直接支持的字段（比如 `enum` ），可以通过 `CUSTOM_EJSON` 这个宏来自定义对应字段的解析过程。

例子：
```cpp
enum class Type { kStudent, kTeacher };

// 自定义的解析过程实现，将枚举类型强转为支持的整型
void custom_solve(ejson::JObject* j, void* v, ejson::EJsonAction action)
{
   switch (action)
   {
      case ejson::EJsonAction::kJsonTo: j->at("type").get_from(*(int*)v); break;
      case ejson::EJsonAction::kJsonFrom: j->at("type").get_to(*(int*)v); break;
   }
}

struct people
{
   Type        type{Type::kStudent};
   int         id{};
   double      score{};
   std::string name;
   ALIAS_EJSON(id, studentNo)       // 取别名
   CUSTOM_EJSON(type, custom_solve) // 自定义解析过程
   OPTION_EJSON(name, "null")       // 允许在解析时该值不存在，并在不存在时赋值为你指定的值
};
AUTO_GEN_NON_INTRUSIVE(people, type, id, score,name)   // 注册对应的字段用于json解析
```
> 请注意 `CUSTOM_EJSON` 宏的第二个参数需要是一个函数指针，对应的函数签名为 `void(JObject*,void*,EJsonAction)` ，你可以直接写lambda表达式，也可以直接独立出一个函数。
## 快速开始

### 要求

* C++11及以上，是跨全平台的

### 安装与引入

推荐用以下两种方式进行引入：

* 方法一：通过cmake中的 `FetchContent` 模块引入

    1. 在项目的cmake中添加下列代码进行引入，国内如果因为网络问题无法使用可以换这个gitee的镜像源：https://gitee.com/acking-you/ejson4cpp.git

       ```cmake
       include(FetchContent)
       
       FetchContent_Declare(
               ejson4cpp
               GIT_REPOSITORY https://github.com/ACking-you/ejson4cpp.git
               GIT_TAG origin/master
               GIT_SHALLOW TRUE)
       FetchContent_MakeAvailable(ejson4cpp)
       ```

    2. 在需要使用该库的目标中链接 `ejson` 即可。

       ```cmake
       target_link_libraries(target  ejson)
       ```

* 方法二：手动下载包，然后通过cmake命令引入

    1. 通过git命令下载项目源码

       ```shell
       git clone https://github.com/ACking-you/ejson4cpp.git
       ````
    2. 将该项目添加到子项目中：

       ```cmake
       add_subdirectory(ejson4cpp)
       ```
    3. 在需要使用该库的目标中链接 `ejson` 即可。

       ```cmake
       target_link_libraries(target  ejson)
       ```

### 开始使用

这里以解析 json 的配置文件映射到 C++ 结构体为例子来进行讲解。

假设有redis、mysql、日志服务需要通过配置文件来进行配置，我们先写下结构体如下：

```cpp
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
   log    logger;
   server redis;
   server mysql;
};
```

一个模拟的json配置文件如下：

```json
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
}
```

现在要实现的功能是读取json配置文件的数据将 config 结构体进行初始化，我们可以按照下面的步骤进行：

完整代码请看 [example/example1.cc](https://github.com/ACking-you/ejson4cpp/blob/master/example/example1.cc)

1. 让server、log、config这几个自定义类型支持 json 序列化，添加下列宏定义即可：

   ```cpp
   // auto generate log/server/config to_json and from_json
   AUTO_GEN_NON_INTRUSIVE(log, level, filedir, formatter)
   AUTO_GEN_NON_INTRUSIVE(server, host, port)
   AUTO_GEN_NON_INTRUSIVE(config, logger, redis, mysql)
   ```

2. 定义config变量，调用 `FromFile` 函数，即可完成需求：

   ```cpp
   struct config s_config;
   // init config from config.json
   Parser::FromFile(CONFIG_PATH, s_config);
   ```

   如果需要重新写回文件，则可调用 `ToFile` 函数：

   ```cpp
   // write config to file
   Parser::ToFile(CONFIG_PATH, s_config);
   ```

   如果读取json字符串的数据并初始化对应的变量（反序列化）则可以调用 `FromJSON` 函数：

   ```cpp
   // init config struct from json string
   Parser::FromJSON(j, s_config);
   ```

   如果需要将变量转化为json字符串（序列化），则可调用 `ToJSON` 函数：

   ```cpp
   auto json_str = Parser::ToJSON(s_config);
   ```



好的，经过以上两步，你已经学会了整个库的核心用法，没错，这个库提倡使用直接的函数而不是类来实现对应的功能，这样能减少你的记忆和思考过程。当然如果需要更为细致的使用它，你可以去了解 `JObject` 类的相关用法，在API介绍里面写的很详细。

## 常见用法

在进行后端开发的过程中，前端传来的数据很多时候是 `json` 数据，那么我们现在就使用该库来模拟一个简单的后端业务。

比如一个视频平台的评论区，首先映入眼帘的是一条条评论，然后是发出该条评论的用户。

那么我们可以抽离出 `comment` 和 `user_info` 这两个结构体表示前端需要展示的消息，那么它在我们C++后端的请看可能是下面这样的结构体：

完整代码在 [example/example2.cc](https://github.com/ACking-you/ejson4cpp/blob/master/example/example2.cc)

```cpp
struct user_info
{
   bool        is_follow{};//是否关注
   int64_t     id{};//id信息
   int64_t     follow_count{};//关注的博主数量
   int64_t     follower_count{};//粉丝数量
   std::string name;//用户名
};

struct comment
{
   int64_t     id{};//id信息
   int64_t     user_id{};//用户id信息
   std::string created_date;//创建时间
   std::string content;//评论内容
};
```

那么我们的后端逻辑可能会经历下面的过程：

1. 从前端获取json数据（中间一般有鉴权的过程）。
2. 接收json数据并其初始化为对应的C++结构体。
3. 进行该次接口调用的业务逻辑处理。
4. 保存数据到数据库。

那么我们用模拟数据来模拟上述过程：

1. 前端的数据：

   ```cpp
   const char* comment_json   = "{\n"
       "  \"content\": \"这是一条\\\"测试\\\"评论\",\n"
       "  \"created_date\": \"2023-01-16\",\n"
       "  \"id\": 1,\n"
       "  \"user_id\": 10\n"
       "}";
   const char* user_info_json = "{\n"
       "  \"follow_count\": 12,\n"
       "  \"follower_count\": 23,\n"
       "  \"id\": 1,\n"
       "  \"is_follow\": false,\n"
       "  \"name\": \"某人名字\"\n"
       "}";
   ```

2. 将数据转为C++的结构体：
   需要先添加下列宏让对应的结构支持json互转

   ```cpp
   AUTO_GEN_NON_INTRUSIVE(user_info, is_follow, id, follow_count, follower_count,name)
   AUTO_GEN_NON_INTRUSIVE(comment, id, user_id, created_date, content)
   ```

   然后调用对应函数即可转化

   ```cpp
   comment   cmt;
   user_info uinfo;
   Parser::FromJSON(comment_json, cmt);
   Parser::FromJSON(user_info_json, uinfo);
   ```

3. 处理业务逻辑，这个跳过。

4. 保存数据到数据库，这个模拟为保存数据到文件：
   我们可以创建一个 `dict_t` 类型的 `JObject` ，然后把刚才的结构体以 `key-value` 对的形式放进去，最后再调用 `ToFile` 函数。

   ```cpp
   // 4.save data to database(we simulate it to local file)
   auto object = JObject::Dict();
   object.at("comment").get_from(cmt);
   object.at("user_info").get_from(uinfo);
   ejson::Parser::ToFile(DATA_PATH, object);
   ```

   最终得到下列json数据到文件中：

   ```json
   {
     "comment": {
       "content": "这是一条\"测试\"评论",
       "created_date": "2023-01-16",
       "id": 1,
       "user_id": 10
     },
     "user_info": {
       "follow_count": 12,
       "follower_count": 23,
       "id": 1,
       "is_follow": false,
       "name": "某人名字"
     }
   }
   ```

## API介绍

对所有类成员的描述的信息，可以点开 `doc/html/index.html` 进行查看。如果需要其他语言版本的文档，可以自己通过 `Doxygen` 进行生成。

### 通过命名风格识别API

1. 所有对外暴露的静态成员函数均以 `PascalCase` 风格命名。如下：

   ```cpp
   namespace ejson {
   class Parser
   {
      static JObject FromJSON(const str_t &content,bool skip_comment=false);
       
      template <class T>
      static void FromJSON(string_view const &src, T &dst,bool skip_comment=false);
       
      static JObject FromFile(string_view const &filename,bool skip_comment=false);
       
      template <class T>
      static void FromFile(string_view const &filename, T &dst);
      
      template <class T>
      static std::string ToJSON(T &&src,const int indent = -1,
                           const char indent_char = ' ', bool is_esc = false);
       
      template <class T>
      static void ToFile(string_view const &filename, T const &src, 
                         const int indent = -1, const char indent_char = ' ',
                         bool is_esc = false);
       
      static void ToFile(string_view const &filename, JObject const &src,
                        const int indent = -1, const char indent_char = ' ',
                         bool is_esc = false)
   };
   
   class JObject
   {
      static auto Dict() -> JObject;
      static auto List() -> JObject;
   };
   }   // namespace ejson
   ```

2. 所有想要暴露的普通成员函数均以 `snack_case` 风格命名，如下：

   ```cpp
   namespace ejson {
   class JObject
   {
      auto type() const -> Type;
       
      auto at(const str_t &key) const -> ObjectRef;
       
      auto to_string(int indent = -1, char indent_char = ' ',
                                     bool is_esc = false) const -> string;
       
      void push_back(JObject item);
       
      void pop_back();
       
      auto has_key(const str_t &key) const -> bool;
   
      template <class T>
      auto cast() const -> T;
   };
   
   struct ObjectRef
   {
      template <class T>
      auto get_from(T const &src) -> ObjectRef &;
      template <class T>
      void get_to(T &src);
   };
   }   // namespace ejson
   ```

3. 其余还有两个函数，如下：

   ```cpp
   namespace ejson_literals {
   
   auto operator""_json(const char *json, size_t len) -> JObject;
       
   auto float_d(int d) -> int;
   
   }   // namespace ejson_literals
   ```




### 宏定义

利用宏定义可以方便且迅速的让自定义类型支持 `FromJSON` 和 `ToJSON` 系列函数。

实际上自定义类型在使用 `FromJSON` 时，只需要定义对应的 `from_json` 函数，使用 `ToJSON` 时，只需定义对应的 `to_json` 函数。

下列是 from_json 和 to_json 的函数签名：

```cpp
void from_json(const ejson::JObject& ejson_j, T& ejson_t);

void to_json(ejson::JObject& ejson_j, const T& ejson_t);
```

你可以像下面这样自己实现上面这两个函数来让自定义类型支持 `FromJSON` 和 `ToJSON`  函数。

```cpp
struct student
{
   int         id;
   int         score;
   std::string name;
};

void to_json(ejson::JObject& ejson_j, const student& ejson_t)
{
   ejson_j.at("id").get_from(ejson_t.id);
   ejson_j.at("score").get_from(ejson_t.score);
   ejson_j.at("name").get_from(ejson_t.name);
}

void from_json(const ejson::JObject& ejson_j, student& ejson_t)
{
   ejson_j.at("id").get_to(ejson_t.id);
   ejson_j.at("score").get_to(ejson_t.score);
   ejson_j.at("name").get_to(ejson_t.name);
}
```

如果属性是 `private` 的，那么可以像下面这样侵入式的定义：

```cpp
struct student
{
   friend void to_json(ejson::JObject& ejson_j, const student& ejson_t)
   {
      ejson_j.at("id").get_from(ejson_t.id);
      ejson_j.at("score").get_from(ejson_t.score);
      ejson_j.at("name").get_from(ejson_t.name);
   }
private:
   int         id;
   int         score;
   std::string name;
};
```

#### FROM_JSON_FUNC&FROM_JSON_FRIEND_FUNC

用于简化 `from_json` 函数定义的书写，例如前面对于 `strudent` 类型的 `from_json` 函数可以这样写：

```cpp
struct student
{
   int         id;
   int         score;
   std::string name;
};
//非侵入式
FROM_JSON_FUNC(student, ejson_j, ejson_t) {
   ejson_j.at("id").get_to(ejson_t.id);
   ejson_j.at("score").get_to(ejson_t.score);
   ejson_j.at("name").get_to(ejson_t.name);
}

struct student
{
    //侵入式
   FROM_JSON_FRIEND_FUNC(student,ejson_j,ejson_t)
   {
      ejson_j.at("id").get_to(ejson_t.id);
      ejson_j.at("score").get_to(ejson_t.score);
      ejson_j.at("name").get_to(ejson_t.name);
   }
private:
   int         id;
   int         score;
   std::string name;
};
```

#### TO_JSON_FUNC&TO_JSON_FRIEND_FUNC

用于简化 `to_json` 函数定义的书写，例如前面对于 `strudent` 类型的 `to_json` 函数可以这样写：

```cpp
struct student
{
   int         id;
   int         score;
   std::string name;
};
//非侵入式
TO_JSON_FUNC(student, ejson_j, ejson_t) {
   ejson_j.at("id").get_from(ejson_t.id);
   ejson_j.at("score").get_from(ejson_t.score);
   ejson_j.at("name").get_from(ejson_t.name);
}

struct student
{
    //侵入式
   TO_JSON_FRIEND_FUNC(student,ejson_j,ejson_t)
   {
      ejson_j.at("id").get_from(ejson_t.id);
      ejson_j.at("score").get_from(ejson_t.score);
      ejson_j.at("name").get_from(ejson_t.name);
   }
private:
   int         id;
   int         score;
   std::string name;
};
```

#### AUTO_GEN_NON_INTRUSIVE&AUTO_GEN_INTRUSIVE

这两个宏可以帮助你一键生成之前例子中的 `to_json` 和 `from_json` 函数。

前面的代码可以替换为：

```cpp
struct student
{
   int         id;
   int         score;
   std::string name;
};
//非侵入式
AUTO_GEN_NON_INTRUSIVE(student,id,score,name)
    
struct student
{
//侵入式
AUTO_GEN_INTRUSIVE(student,id,score,name)
    
private:
   int         id;
   int         score;
   std::string name;
};
```

#### ENABLE_JSON_COUT

自动生成对应类型的 `operator<<(ostream&,T)` 运算符重载，用于将对应类型支持 `cout` 打印出json格式。该宏可以为多个类型生成。

```cpp
struct student
{
   int         id;
   int         score;
   std::string name;
};

struct info
{
   int         id;
   std::string msg;
};
//让对应类型支持json格式化
AUTO_GEN_NON_INTRUSIVE(student,id,score,name)
AUTO_GEN_NON_INTRUSIVE(info,id,msg)
    
//支持json格式cout打印
ENABLE_JSON_COUT(student,info)
```



### FromJSON系列函数

#### 参数说明

```cpp
static JObject Parser::FromJSON(const str_t &content, bool skip_comment = false);
```

根据json字符串内容反序列化为JObject结构。

参数说明：

* `content`：需要解析的json资源，这是一个string_view类型的参数，支持C风格字符串和 `std::string`。
* `skip_comment`：是否需要支持跳过注释，默认为false，未开启。

返回值：

* 返回解析完的 JObject 类型。

---



```cpp
template <class T>
static void Parser::FromJSON(string_view const &src, T &dst,bool skip_comment = false)
```

根据json字符串内容反序列化数据到 `dst`。

参数说明：

* `src`：需要解析的json资源，这是一个string_view类型的参数，支持C风格字符串和 `std::string`。
* `dst`：需要初始化的变量，可以是自定义类型。
* `skip_comment`：是否需要支持跳过注释，默认为false，未开启。

---



```cpp
static JObject& Parser::FromFile(string_view const &filename, bool skip_comment = false);
```

根据文件中的 json 数据获取 JObject&，这个JObject是thread_local变量，也就是每个线程共用一个JObject。所以请注意，当您调用此函数时，将更新这个共用的 JObject  的值。

参数说明：

* `filename` ：json文件路径。
* `skip_comment` ：是否需要支持跳过注释，默认为false，未开启。

返回值：

* 同一个线程共用的JObject&。

---



```cpp
static void Parser::FromFile(string_view const &filename, T &dst,bool skip_comment = false);
```

根据文件中的json数据设置 `dst` 的值。

参数说明：

* `filename` ：json文件路径。
* `dst` ：需要初始化的变量。
* `skip_comment` ：是否需要支持跳过注释，默认为false，未开启。

#### 使用示例

```cpp
#include <ejson/parser.h>
#include <iostream>
using namespace ejson;

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
//为Score类型自动生成to_json和from_json函数
AUTO_GEN_NON_INTRUSIVE(Score, p)
//为student类型自动生成to_json和from_json函数
AUTO_GEN_NON_INTRUSIVE(student, id, name, score)
//重载方便cout打印数据
ENABLE_JSON_COUT(Score,student)
int main(){
	const char *json1 =
     R"({"id":324,"name":"刘xx","score":{"p":2342343243242.124}})";
    student stu;
    //使用FromJSON初始化stu变量
   	Parser::FromJSON(json1,stu);
    //使用FromFile初始化stu变量
    Parser::FromFile("json文件路径",stu);
    std::cout<<stu;
}
```

### ToJSON系列函数

#### 参数说明

```cpp
template <class T>
static auto Parser::ToJSON(T &&src,const int indent = -1,
                        const char indent_char = ' ', bool is_esc = false) -> std::string;
```

将任意类型序列化为json字符串返回。

参数说明：

* `src` ：需要序列化为json字符串的数据。
* `indent` ：是否需要美化json输出，小于0表示美化，其余情况为美化时的缩进长度，默认不美化。
* `indent_char` ：美化时填入缩进的字符，默认为 `' '` 。
* `is_esc` ：是否需要识别转义字符，默认不识别。

返回值：

* json字符串。

---



```cpp
template <class T>
static void ToFile(string_view const &filename, T const &src,
                   const int indent = -1, const char indent_char = ' ',
                      bool is_esc = false);
```

根据 `src` 中的数据序列化为json数据到文件中。

参数说明：

* `filename` ：需要写入的文件路径。
* `src` ：需要序列化的变量。
* `indent` ：是否需要美化json输出，小于0表示美化，其余情况为美化时的缩进长度，默认不美化。
* `indent_char` ：美化时填入缩进的字符，默认为 `' '` 。
* `is_esc` ：是否需要识别转义字符，默认不识别。

---



```cpp
static void ToFile(string_view const &filename, JObject const &src
                   const int indent = -1, const char indent_char = ' ',
                      bool is_esc = false)
```

将JObject中的数据转为json写入到文件中。

参数说明：

* `filename` ：需要写入的文件路径。
* `src` ：JObject变量。
* `indent` ：是否需要美化json输出，小于0表示美化，其余情况为美化时的缩进长度，默认不美化。
* `indent_char` ：美化时填入缩进的字符，默认为 `' '` 。
* `is_esc` ：是否需要识别转义字符，默认不识别。

#### 使用示例

```cpp
#include <ejson/parser.h>
#include <iostream>
using namespace ejson;

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
//为Score类型自动生成to_json和from_json函数
AUTO_GEN_NON_INTRUSIVE(Score, p)
//为student类型自动生成to_json和from_json函数
AUTO_GEN_NON_INTRUSIVE(student, id, name, score)
//重载方便cout打印数据
ENABLE_JSON_COUT(Score,student)
int main(){
    student stu;
    stu.id = 324;
    stu.name = "刘xx";
    stu.score.p = 2342343243242.124;
    //使用ToJSON进行序列化
   	auto json_data = Parser::ToJSON(stu);
    //使用ToFile将数据序列化到文件
    Parser::ToFile("文件路径",stu);
    std::cout<<stu;
}
```

### JObject系列函数

#### JObject的构造函数

只需要清楚以下几点：

1. JObject有无参构造，但是无参构造产生的JObject为null类型，无法进行正常使用。
2. JObject的构造函数可以直接接受大部分类型，且包括自定义类型和部分stl容器。
3. JObject本身不支持拷贝构造，只支持移动构造。

前面的 ToJSON API完全可以用下列方式替代，因为所有的序列化过程其实都是构造JObject来进行：

```cpp
#include <ejson/parser.h>
#include <iostream>
using namespace ejson;

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
//为Score类型自动生成to_json和from_json函数
AUTO_GEN_NON_INTRUSIVE(Score, p)
//为student类型自动生成to_json和from_json函数
AUTO_GEN_NON_INTRUSIVE(student, id, name, score)
//重载方便cout打印数据
ENABLE_JSON_COUT(Score,student)
int main(){
    student stu;
    stu.id = 324;
    stu.name = "刘xx";
    stu.score.p = 2342343243242.124;
    //构造JObject并使用其成员函数
   	auto json_data = JObject(stu).to_string();
    std::cout<<stu;
}
```

#### JObject的成员函数

```cpp
Type JObject::type() const
```

返回当前JObject对象的类型，具体的类型有下列情况：

* `kNull` ：值为null类型
* `kBool`：值为bool类型
* `kInt`：值为整数类型
* `kDouble`：值为浮点类型
* `kStr`：值为字符串类型
* `kList`：值为列表类型
* `kDict`：值为字典类型（或叫做对象类型）

---

##### kDict

当你的JObject的类型为 `kDict` 时，下列成员函数可供使用：

```cpp
bool JObject::has_key(const str_t& key)	const
```

* 判断 `JObject` 中是否存在包含 `key` 的映射。

```cpp
ObjectRef JObject::at(const str_t& key)	const
```

* 根据key取出对应映射的value，value以 ObjectRef 类型的方式提供。

  而ObjectRef类型有这两个关键的成员函数：

    ```cpp
    ObjectRef& JObject::ObjectRef::get_from(T&& src)	
    ```
    从 `src` 中获取数据填充到 `JObject` 中，若 `src` 为自定义类型需要自定义 `to_json` 方法。
    ```cpp
    ObjectRef& JObject::ObjectRef::get_from(T& dst)
    ```
    从 `JObject` 中获取数据填充到 `dst` 中，若 `dst` 为自定义类型需要自定义 `from_json` 方法。

---

##### kList

当JObject类型为 `kList` 时，下列成员函数可用：

```cpp
void JObject::push_back(JObject item);
```

* 插入一个值到 `JObject` 列表的尾部，可以插入任意类型，但都需要显式的转化为 `JObject` 类型，如 `JObject(324234)`。

```cpp
void JObject::pop_back()
```

* 删除列表中末尾的值。

---

##### to_string

```cpp
string JObject::to_string(
int 	indent = -1,
char 	indent_char = ' ',
bool 	is_esc = false 
)const
```

序列化最终调用的API，将JObject序列化为json字符串返回。

参数说明：

* `indent` ：用于判断json解析是否需要美化，如果需要美化，则该值为缩进的长度。该值小于0时不进行美化，默认值为-1。
* `indent_char`：缩进填入的字符，默认为 `' '` 。
* `is_esc`：是否需要对转义字符进行处理，默认不开启。

返回值：

* 序列化后端json字符串。

---

##### static

为了方便快速创建 `dict_t` 类型和 `list_t` 类型的 `JObject` ，定义了下列静态函数：

---

```cpp
static JObject JObject::Dict()
```

使用如下：

```cpp
#include <ejson/parser.h>
#include <iostream>
using namespace ejson;

int main(){
    //构造JObject并使用其成员函数
   	auto json = JObject::Dict();
    json.at("a").get_from("bc");
    json.at("d").get_from("ef");
    std::cout<<json.to_string();
}
```

---

```cpp
static JObject JObject::List()
```

使用如下：

```cpp
#include <ejson/parser.h>
#include <iostream>
#include <vector>
using namespace ejson;
struct custom_type{
    int id;
    std::vector<int> data;
};

//自动生成to_json和from_json函数
AUTO_GEN_NON_INTRUSIVE(custom_type, id,data)

int main(){
    //构造JObject并使用其成员函数
   	auto json = JObject::List();
    json.push_back(JObject("abc"));
    custom_type v{1,{2,3,3}};
   	json.push_back(JObject(v));
    std::cout<<json.to_string();
}
```



## 注意事项

在使用本库时需要注意几点：

1. 本库不对你使用的字符编码进行验证，加入你使用 gbk 编码的json数据进行解析得到还是gbk编码的，所以这点需要注意，建议都使用utrf8编码。
2. 本库只支持对 `\` 和 `"` 的转义，其他的诸如 `\u` `\b` 等功能暂时不支持，我还是建议在json数据中不要存储二进制文件，如果需要存储二进制文件，后续的版本中可能会增base64编码的支持。
3. 本库对所有字符串解析到 `JObject` 中的过程都是浅拷贝（只拷贝了指针），故如果需要使用原生 `JObject` 进行数据的存储需要额外的注意内存的所有权和生命周期，建议使用 `JObject` 进行短期的解析而不是长期的存储，在大多数情况下直接使用函数是最好的选择，后续可能会出一个深拷贝的 `JObject` 那样的结构才适合存储。
4. 本库所有的错误均以异常抛出，好处在于可以模拟递归调用栈的栈信息进行打印，坏处当然是要写 `try catch`。
