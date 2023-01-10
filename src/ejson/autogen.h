#pragma once
// Macros to simplify conversion from/to types

//[custom]
// NLOHMANN{} = MY${raw}
#define EJSON_THROW_ERROR_POS(erron)                                           \
  throw std::runtime_error(std::string("error:") + std::string(erron) +        \
                           std::string(" at ") + std::string(__FILE__) +       \
                           std::string(":") + std::to_string(__LINE__))

#define EJSON_THROW_GET_ERROR(erron)                                           \
  EJSON_THROW_ERROR_POS("type error in get " erron " value!")

#define EJSON_EXPAND(x) x
#define EJSON_GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12,     \
                        _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, \
                        _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, \
                        _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, \
                        _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, \
                        _57, _58, _59, _60, _61, _62, _63, _64, NAME, ...)     \
  NAME
#define EJSON_PASTE(...)                                                       \
  EJSON_EXPAND(EJSON_GET_MACRO(                                                \
      __VA_ARGS__, MY_JSON_PASTE64, MY_JSON_PASTE63, MY_JSON_PASTE62,          \
      MY_JSON_PASTE61, MY_JSON_PASTE60, MY_JSON_PASTE59, MY_JSON_PASTE58,      \
      MY_JSON_PASTE57, MY_JSON_PASTE56, MY_JSON_PASTE55, MY_JSON_PASTE54,      \
      MY_JSON_PASTE53, MY_JSON_PASTE52, MY_JSON_PASTE51, MY_JSON_PASTE50,      \
      MY_JSON_PASTE49, MY_JSON_PASTE48, MY_JSON_PASTE47, MY_JSON_PASTE46,      \
      MY_JSON_PASTE45, MY_JSON_PASTE44, MY_JSON_PASTE43, MY_JSON_PASTE42,      \
      MY_JSON_PASTE41, MY_JSON_PASTE40, MY_JSON_PASTE39, MY_JSON_PASTE38,      \
      MY_JSON_PASTE37, MY_JSON_PASTE36, MY_JSON_PASTE35, MY_JSON_PASTE34,      \
      MY_JSON_PASTE33, MY_JSON_PASTE32, MY_JSON_PASTE31, MY_JSON_PASTE30,      \
      MY_JSON_PASTE29, MY_JSON_PASTE28, MY_JSON_PASTE27, MY_JSON_PASTE26,      \
      MY_JSON_PASTE25, MY_JSON_PASTE24, MY_JSON_PASTE23, MY_JSON_PASTE22,      \
      MY_JSON_PASTE21, MY_JSON_PASTE20, MY_JSON_PASTE19, MY_JSON_PASTE18,      \
      MY_JSON_PASTE17, MY_JSON_PASTE16, MY_JSON_PASTE15, MY_JSON_PASTE14,      \
      MY_JSON_PASTE13, MY_JSON_PASTE12, MY_JSON_PASTE11, MY_JSON_PASTE10,      \
      MY_JSON_PASTE9, MY_JSON_PASTE8, MY_JSON_PASTE7, MY_JSON_PASTE6,          \
      MY_JSON_PASTE5, MY_JSON_PASTE4, MY_JSON_PASTE3, MY_JSON_PASTE2,          \
      MY_JSON_PASTE1)(__VA_ARGS__))
#define MY_JSON_PASTE2(func, v1) func(v1)
#define MY_JSON_PASTE3(func, v1, v2)                                           \
  MY_JSON_PASTE2(func, v1) MY_JSON_PASTE2(func, v2)
#define MY_JSON_PASTE4(func, v1, v2, v3)                                       \
  MY_JSON_PASTE2(func, v1) MY_JSON_PASTE3(func, v2, v3)
#define MY_JSON_PASTE5(func, v1, v2, v3, v4)                                   \
  MY_JSON_PASTE2(func, v1) MY_JSON_PASTE4(func, v2, v3, v4)
#define MY_JSON_PASTE6(func, v1, v2, v3, v4, v5)                               \
  MY_JSON_PASTE2(func, v1) MY_JSON_PASTE5(func, v2, v3, v4, v5)
#define MY_JSON_PASTE7(func, v1, v2, v3, v4, v5, v6)                           \
  MY_JSON_PASTE2(func, v1) MY_JSON_PASTE6(func, v2, v3, v4, v5, v6)
#define MY_JSON_PASTE8(func, v1, v2, v3, v4, v5, v6, v7)                       \
  MY_JSON_PASTE2(func, v1) MY_JSON_PASTE7(func, v2, v3, v4, v5, v6, v7)
#define MY_JSON_PASTE9(func, v1, v2, v3, v4, v5, v6, v7, v8)                   \
  MY_JSON_PASTE2(func, v1) MY_JSON_PASTE8(func, v2, v3, v4, v5, v6, v7, v8)
#define MY_JSON_PASTE10(func, v1, v2, v3, v4, v5, v6, v7, v8, v9)              \
  MY_JSON_PASTE2(func, v1) MY_JSON_PASTE9(func, v2, v3, v4, v5, v6, v7, v8, v9)
#define MY_JSON_PASTE11(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)         \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE10(func, v2, v3, v4, v5, v6, v7, v8, v9, v10)
#define MY_JSON_PASTE12(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)    \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE11(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)
#define MY_JSON_PASTE13(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12)                                                   \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE12(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12)
#define MY_JSON_PASTE14(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13)                                              \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE13(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13)
#define MY_JSON_PASTE15(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14)                                         \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE14(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14)
#define MY_JSON_PASTE16(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15)                                    \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE15(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15)
#define MY_JSON_PASTE17(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16)                               \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE16(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16)
#define MY_JSON_PASTE18(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17)                          \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE17(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17)
#define MY_JSON_PASTE19(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18)                     \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE18(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18)
#define MY_JSON_PASTE20(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19)                \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE19(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19)
#define MY_JSON_PASTE21(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20)           \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE20(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20)
#define MY_JSON_PASTE22(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21)      \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE21(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21)
#define MY_JSON_PASTE23(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22) \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE22(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22)
#define MY_JSON_PASTE24(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23)                                                   \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE23(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23)
#define MY_JSON_PASTE25(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24)                                              \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE24(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24)
#define MY_JSON_PASTE26(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25)                                         \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE25(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25)
#define MY_JSON_PASTE27(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26)                                    \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE26(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26)
#define MY_JSON_PASTE28(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27)                               \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE27(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27)
#define MY_JSON_PASTE29(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28)                          \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE28(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28)
#define MY_JSON_PASTE30(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29)                     \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE29(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29)
#define MY_JSON_PASTE31(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30)                \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE30(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30)
#define MY_JSON_PASTE32(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31)           \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE31(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31)
#define MY_JSON_PASTE33(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32)      \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE32(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32)
#define MY_JSON_PASTE34(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33) \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE33(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33)
#define MY_JSON_PASTE35(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34)                                                   \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE34(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34)
#define MY_JSON_PASTE36(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35)                                              \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE35(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35)
#define MY_JSON_PASTE37(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36)                                         \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE36(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36)
#define MY_JSON_PASTE38(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37)                                    \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE37(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37)
#define MY_JSON_PASTE39(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38)                               \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE38(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38)
#define MY_JSON_PASTE40(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39)                          \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE39(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38, v39)
#define MY_JSON_PASTE41(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40)                     \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE40(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38, v39, v40)
#define MY_JSON_PASTE42(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41)                \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE41(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38, v39, v40, v41)
#define MY_JSON_PASTE43(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41, v42)           \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE42(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38, v39, v40, v41, v42)
#define MY_JSON_PASTE44(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41, v42, v43)      \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE43(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38, v39, v40, v41, v42, v43)
#define MY_JSON_PASTE45(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44) \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE44(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38, v39, v40, v41, v42, v43, v44)
#define MY_JSON_PASTE46(                                                       \
    func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15,    \
    v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, \
    v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45) \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE45(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38, v39, v40, v41, v42, v43, v44, v45)
#define MY_JSON_PASTE47(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, \
                        v45, v46)                                              \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE46(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38, v39, v40, v41, v42, v43, v44, v45, v46)
#define MY_JSON_PASTE48(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, \
                        v45, v46, v47)                                         \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE47(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38, v39, v40, v41, v42, v43, v44, v45, v46, v47)
#define MY_JSON_PASTE49(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, \
                        v45, v46, v47, v48)                                    \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE48(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48)
#define MY_JSON_PASTE50(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, \
                        v45, v46, v47, v48, v49)                               \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE49(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49)
#define MY_JSON_PASTE51(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, \
                        v45, v46, v47, v48, v49, v50)                          \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE50(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,  \
                  v50)
#define MY_JSON_PASTE52(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, \
                        v45, v46, v47, v48, v49, v50, v51)                     \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE51(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,  \
                  v50, v51)
#define MY_JSON_PASTE53(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, \
                        v45, v46, v47, v48, v49, v50, v51, v52)                \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE52(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,  \
                  v50, v51, v52)
#define MY_JSON_PASTE54(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, \
                        v45, v46, v47, v48, v49, v50, v51, v52, v53)           \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE53(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,  \
                  v50, v51, v52, v53)
#define MY_JSON_PASTE55(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, \
                        v45, v46, v47, v48, v49, v50, v51, v52, v53, v54)      \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE54(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,  \
                  v50, v51, v52, v53, v54)
#define MY_JSON_PASTE56(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, \
                        v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55) \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE55(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,  \
                  v50, v51, v52, v53, v54, v55)
#define MY_JSON_PASTE57(                                                       \
    func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15,    \
    v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, \
    v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, \
    v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56)                     \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE56(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,  \
                  v50, v51, v52, v53, v54, v55, v56)
#define MY_JSON_PASTE58(                                                       \
    func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15,    \
    v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, \
    v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, \
    v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57)                \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE57(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,  \
                  v50, v51, v52, v53, v54, v55, v56, v57)
#define MY_JSON_PASTE59(                                                       \
    func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15,    \
    v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, \
    v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, \
    v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58)           \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE58(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,  \
                  v50, v51, v52, v53, v54, v55, v56, v57, v58)
#define MY_JSON_PASTE60(                                                       \
    func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15,    \
    v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, \
    v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, \
    v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59)      \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE59(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,  \
                  v50, v51, v52, v53, v54, v55, v56, v57, v58, v59)
#define MY_JSON_PASTE61(                                                       \
    func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15,    \
    v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, \
    v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, \
    v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60) \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE60(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,  \
                  v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60)
#define MY_JSON_PASTE62(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, \
                        v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, \
                        v56, v57, v58, v59, v60, v61)                          \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE61(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,  \
                  v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61)
#define MY_JSON_PASTE63(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, \
                        v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, \
                        v56, v57, v58, v59, v60, v61, v62)                     \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE62(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,  \
                  v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61,  \
                  v62)
#define MY_JSON_PASTE64(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, \
                        v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, \
                        v56, v57, v58, v59, v60, v61, v62, v63)                \
  MY_JSON_PASTE2(func, v1)                                                     \
  MY_JSON_PASTE63(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,    \
                  v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,  \
                  v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,  \
                  v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,  \
                  v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61,  \
                  v62, v63)

//[custom]
// nlohmann{} = my${raw}
#define TO_JSON_FRIEND_FUNC(Type, ejson_j, ejson_t)                            \
  friend inline void to_json(ejson::JObject &ejson_j, const Type &ejson_t)
#define FROM_JSON_FRIEND_FUNC(Type, ejson_j, ejson_t)                          \
  friend inline void from_json(const ejson::JObject &ejson_j, Type &ejson_t)

#define TO_JSON_FUNC(Type, ejson_j, ejson_t)                                   \
  inline void to_json(ejson::JObject &ejson_j, const Type &ejson_t)
#define FROM_JSON_FUNC(Type, ejson_j, ejson_t)                                 \
  inline void from_json(const ejson::JObject &ejson_j, Type &ejson_t)

#define EJSON_TO(v1) ejson_j.at(#v1).get_from(ejson_t.v1);
#define EJSON_FROM(v1) ejson_j.at(#v1).get_to(ejson_t.v1);

#define AUTO_GEN_NON_INTRUSIVE(Type, ...)                                      \
  TO_JSON_FUNC(Type, ejson_j, ejson_t){EJSON_EXPAND(EJSON_PASTE(               \
      EJSON_TO, __VA_ARGS__))} FROM_JSON_FUNC(Type, ejson_j, ejson_t) {        \
    EJSON_EXPAND(EJSON_PASTE(EJSON_FROM, __VA_ARGS__))                         \
  }

#define AUTO_GEN_INTRUSIVE(Type, ...)                                          \
  TO_JSON_FRIEND_FUNC(Type, ejson_j, ejson_t){EJSON_EXPAND(EJSON_PASTE(        \
      EJSON_TO, __VA_ARGS__))} FROM_JSON_FRIEND_FUNC(Type, ejson_j, ejson_t) { \
    EJSON_EXPAND(EJSON_PASTE(EJSON_FROM, __VA_ARGS__))                         \
  }

#define ENABLE_JSON_COUT(Type)                                                 \
  inline std::ostream &operator<<(std::ostream &os, Type const &src) {         \
    os << ejson::JObject(src).to_string();                                     \
    return os;                                                                 \
  }