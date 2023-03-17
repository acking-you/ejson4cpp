#pragma once
// Macros to simplify conversion from/to types

//[custom]
// EJSON_JSON{} = EJSON${raw}

#define EJSON_ERROR_POS_STRING(erron)                                          \
   (std::string("\nerror:") + std::string(erron) + std::string(" at \"") +     \
    std::string(__FILE__) + std::string(":") + std::to_string(__LINE__) +      \
    std::string("\",func:") + std::string(__FUNCTION__))

#define EJSON_THROW_ERROR_POS(erron)                                           \
   throw std::runtime_error(EJSON_ERROR_POS_STRING(erron))

#define EJSON_THROW_GET_ERROR(erron)                                           \
   EJSON_THROW_ERROR_POS("type error in get \"" erron "\" value")

#define EJSON_THROW_ERROR_WITH_TYPE(pre, erron, Type)                          \
   throw std::runtime_error(                                                   \
     std::string(pre) +                                                        \
     EJSON_ERROR_POS_STRING(std::string(erron " with type:\"") +               \
                            std::string(typeid(Type).name()) +                 \
                            std::string("\"")))
#define EJSON_EXPAND(x) x
#define EJSON_GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12,     \
                        _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, \
                        _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, \
                        _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, \
                        _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, \
                        _57, _58, _59, _60, _61, _62, _63, _64, NAME, ...)     \
   NAME

/**
 * @brief The first parameter is the macro that needs to be called, the other parameters that follow each call the macro once.
 * \n That is, the role of this macro is to call other macros according to the number of arguments.
 */
#define EJSON_PASTE(...)                                                       \
   EJSON_EXPAND(EJSON_GET_MACRO(                                               \
     __VA_ARGS__, EJSON_PASTE64, EJSON_PASTE63, EJSON_PASTE62, EJSON_PASTE61,  \
     EJSON_PASTE60, EJSON_PASTE59, EJSON_PASTE58, EJSON_PASTE57,               \
     EJSON_PASTE56, EJSON_PASTE55, EJSON_PASTE54, EJSON_PASTE53,               \
     EJSON_PASTE52, EJSON_PASTE51, EJSON_PASTE50, EJSON_PASTE49,               \
     EJSON_PASTE48, EJSON_PASTE47, EJSON_PASTE46, EJSON_PASTE45,               \
     EJSON_PASTE44, EJSON_PASTE43, EJSON_PASTE42, EJSON_PASTE41,               \
     EJSON_PASTE40, EJSON_PASTE39, EJSON_PASTE38, EJSON_PASTE37,               \
     EJSON_PASTE36, EJSON_PASTE35, EJSON_PASTE34, EJSON_PASTE33,               \
     EJSON_PASTE32, EJSON_PASTE31, EJSON_PASTE30, EJSON_PASTE29,               \
     EJSON_PASTE28, EJSON_PASTE27, EJSON_PASTE26, EJSON_PASTE25,               \
     EJSON_PASTE24, EJSON_PASTE23, EJSON_PASTE22, EJSON_PASTE21,               \
     EJSON_PASTE20, EJSON_PASTE19, EJSON_PASTE18, EJSON_PASTE17,               \
     EJSON_PASTE16, EJSON_PASTE15, EJSON_PASTE14, EJSON_PASTE13,               \
     EJSON_PASTE12, EJSON_PASTE11, EJSON_PASTE10, EJSON_PASTE9, EJSON_PASTE8,  \
     EJSON_PASTE7, EJSON_PASTE6, EJSON_PASTE5, EJSON_PASTE4, EJSON_PASTE3,     \
     EJSON_PASTE2, EJSON_PASTE1)(__VA_ARGS__))

#define EJSON_PASTE2(func, v1)     func(v1)
#define EJSON_PASTE3(func, v1, v2) EJSON_PASTE2(func, v1) EJSON_PASTE2(func, v2)
#define EJSON_PASTE4(func, v1, v2, v3)                                         \
   EJSON_PASTE2(func, v1) EJSON_PASTE3(func, v2, v3)
#define EJSON_PASTE5(func, v1, v2, v3, v4)                                     \
   EJSON_PASTE2(func, v1) EJSON_PASTE4(func, v2, v3, v4)
#define EJSON_PASTE6(func, v1, v2, v3, v4, v5)                                 \
   EJSON_PASTE2(func, v1) EJSON_PASTE5(func, v2, v3, v4, v5)
#define EJSON_PASTE7(func, v1, v2, v3, v4, v5, v6)                             \
   EJSON_PASTE2(func, v1) EJSON_PASTE6(func, v2, v3, v4, v5, v6)
#define EJSON_PASTE8(func, v1, v2, v3, v4, v5, v6, v7)                         \
   EJSON_PASTE2(func, v1) EJSON_PASTE7(func, v2, v3, v4, v5, v6, v7)
#define EJSON_PASTE9(func, v1, v2, v3, v4, v5, v6, v7, v8)                     \
   EJSON_PASTE2(func, v1) EJSON_PASTE8(func, v2, v3, v4, v5, v6, v7, v8)
#define EJSON_PASTE10(func, v1, v2, v3, v4, v5, v6, v7, v8, v9)                \
   EJSON_PASTE2(func, v1) EJSON_PASTE9(func, v2, v3, v4, v5, v6, v7, v8, v9)
#define EJSON_PASTE11(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)           \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE10(func, v2, v3, v4, v5, v6, v7, v8, v9, v10)
#define EJSON_PASTE12(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)      \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE11(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)
#define EJSON_PASTE13(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12) \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE12(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12)
#define EJSON_PASTE14(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13)                                                     \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE13(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13)
#define EJSON_PASTE15(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14)                                                \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE14(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14)
#define EJSON_PASTE16(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15)                                           \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE15(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15)
#define EJSON_PASTE17(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16)                                      \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE16(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16)
#define EJSON_PASTE18(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17)                                 \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE17(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17)
#define EJSON_PASTE19(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18)                            \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE18(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18)
#define EJSON_PASTE20(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19)                       \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE19(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19)
#define EJSON_PASTE21(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20)                  \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE20(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20)
#define EJSON_PASTE22(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21)             \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE21(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21)
#define EJSON_PASTE23(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22)        \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE22(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22)
#define EJSON_PASTE24(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23)   \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE23(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23)
#define EJSON_PASTE25(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24)                                                     \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE24(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24)
#define EJSON_PASTE26(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25)                                                \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE25(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25)
#define EJSON_PASTE27(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26)                                           \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE26(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26)
#define EJSON_PASTE28(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27)                                      \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE27(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27)
#define EJSON_PASTE29(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28)                                 \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE28(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28)
#define EJSON_PASTE30(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29)                            \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE29(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29)
#define EJSON_PASTE31(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30)                       \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE30(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30)
#define EJSON_PASTE32(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31)                  \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE31(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31)
#define EJSON_PASTE33(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32)             \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE32(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32)
#define EJSON_PASTE34(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33)        \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE33(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33)
#define EJSON_PASTE35(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34)   \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE34(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34)
#define EJSON_PASTE36(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34,   \
                      v35)                                                     \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE35(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35)
#define EJSON_PASTE37(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34,   \
                      v35, v36)                                                \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE36(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36)
#define EJSON_PASTE38(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34,   \
                      v35, v36, v37)                                           \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE37(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37)
#define EJSON_PASTE39(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34,   \
                      v35, v36, v37, v38)                                      \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE38(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38)
#define EJSON_PASTE40(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34,   \
                      v35, v36, v37, v38, v39)                                 \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE39(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38, v39)
#define EJSON_PASTE41(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34,   \
                      v35, v36, v37, v38, v39, v40)                            \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE40(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38, v39, v40)
#define EJSON_PASTE42(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34,   \
                      v35, v36, v37, v38, v39, v40, v41)                       \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE41(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38, v39, v40, v41)
#define EJSON_PASTE43(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34,   \
                      v35, v36, v37, v38, v39, v40, v41, v42)                  \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE42(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38, v39, v40, v41, v42)
#define EJSON_PASTE44(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34,   \
                      v35, v36, v37, v38, v39, v40, v41, v42, v43)             \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE43(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38, v39, v40, v41, v42, v43)
#define EJSON_PASTE45(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34,   \
                      v35, v36, v37, v38, v39, v40, v41, v42, v43, v44)        \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE44(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38, v39, v40, v41, v42, v43, v44)
#define EJSON_PASTE46(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34,   \
                      v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45)   \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE45(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38, v39, v40, v41, v42, v43, v44, v45)
#define EJSON_PASTE47(                                                         \
  func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, \
  v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31,   \
  v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46)   \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE46(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38, v39, v40, v41, v42, v43, v44, v45, v46)
#define EJSON_PASTE48(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34,   \
                      v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45,   \
                      v46, v47)                                                \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE47(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38, v39, v40, v41, v42, v43, v44, v45, v46, v47)
#define EJSON_PASTE49(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34,   \
                      v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45,   \
                      v46, v47, v48)                                           \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE48(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48)
#define EJSON_PASTE50(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34,   \
                      v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45,   \
                      v46, v47, v48, v49)                                      \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE49(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49)
#define EJSON_PASTE51(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34,   \
                      v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45,   \
                      v46, v47, v48, v49, v50)                                 \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE50(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,   \
                 v50)
#define EJSON_PASTE52(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34,   \
                      v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45,   \
                      v46, v47, v48, v49, v50, v51)                            \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE51(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,   \
                 v50, v51)
#define EJSON_PASTE53(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34,   \
                      v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45,   \
                      v46, v47, v48, v49, v50, v51, v52)                       \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE52(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,   \
                 v50, v51, v52)
#define EJSON_PASTE54(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34,   \
                      v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45,   \
                      v46, v47, v48, v49, v50, v51, v52, v53)                  \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE53(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,   \
                 v50, v51, v52, v53)
#define EJSON_PASTE55(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34,   \
                      v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45,   \
                      v46, v47, v48, v49, v50, v51, v52, v53, v54)             \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE54(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,   \
                 v50, v51, v52, v53, v54)
#define EJSON_PASTE56(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34,   \
                      v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45,   \
                      v46, v47, v48, v49, v50, v51, v52, v53, v54, v55)        \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE55(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,   \
                 v50, v51, v52, v53, v54, v55)
#define EJSON_PASTE57(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34,   \
                      v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45,   \
                      v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56)   \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE56(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,   \
                 v50, v51, v52, v53, v54, v55, v56)
#define EJSON_PASTE58(                                                         \
  func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, \
  v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31,   \
  v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46,   \
  v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57)                       \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE57(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,   \
                 v50, v51, v52, v53, v54, v55, v56, v57)
#define EJSON_PASTE59(                                                         \
  func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, \
  v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31,   \
  v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46,   \
  v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58)                  \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE58(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,   \
                 v50, v51, v52, v53, v54, v55, v56, v57, v58)
#define EJSON_PASTE60(                                                         \
  func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, \
  v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31,   \
  v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46,   \
  v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59)             \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE59(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,   \
                 v50, v51, v52, v53, v54, v55, v56, v57, v58, v59)
#define EJSON_PASTE61(                                                         \
  func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, \
  v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31,   \
  v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46,   \
  v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60)        \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE60(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,   \
                 v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60)
#define EJSON_PASTE62(                                                         \
  func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, \
  v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31,   \
  v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46,   \
  v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61)   \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE61(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,   \
                 v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61)
#define EJSON_PASTE63(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34,   \
                      v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45,   \
                      v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56,   \
                      v57, v58, v59, v60, v61, v62)                            \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE62(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,   \
                 v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61,   \
                 v62)
#define EJSON_PASTE64(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, \
                      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,   \
                      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34,   \
                      v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45,   \
                      v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56,   \
                      v57, v58, v59, v60, v61, v62, v63)                       \
   EJSON_PASTE2(func, v1)                                                      \
   EJSON_PASTE63(func, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,     \
                 v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,   \
                 v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,   \
                 v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49,   \
                 v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61,   \
                 v62, v63)
#define EJSON_ARG2_PASTE(...)                                                  \
   EJSON_EXPAND(EJSON_GET_MACRO(                                               \
     __VA_ARGS__, EJSON_PLACEHOLDER, EJSON_ARGS62_PASTE31, EJSON_PLACEHOLDER,  \
     EJSON_ARGS60_PASTE30, EJSON_PLACEHOLDER, EJSON_ARGS58_PASTE29,            \
     EJSON_PLACEHOLDER, EJSON_ARGS56_PASTE28, EJSON_PLACEHOLDER,               \
     EJSON_ARGS54_PASTE27, EJSON_PLACEHOLDER, EJSON_ARGS52_PASTE26,            \
     EJSON_PLACEHOLDER, EJSON_ARGS50_PASTE25, EJSON_PLACEHOLDER,               \
     EJSON_ARGS48_PASTE24, EJSON_PLACEHOLDER, EJSON_ARGS46_PASTE23,            \
     EJSON_PLACEHOLDER, EJSON_ARGS44_PASTE22, EJSON_PLACEHOLDER,               \
     EJSON_ARGS42_PASTE21, EJSON_PLACEHOLDER, EJSON_ARGS40_PASTE20,            \
     EJSON_PLACEHOLDER, EJSON_ARGS38_PASTE19, EJSON_PLACEHOLDER,               \
     EJSON_ARGS36_PASTE18, EJSON_PLACEHOLDER, EJSON_ARGS34_PASTE17,            \
     EJSON_PLACEHOLDER, EJSON_ARGS32_PASTE16, EJSON_PLACEHOLDER,               \
     EJSON_ARGS30_PASTE15, EJSON_PLACEHOLDER, EJSON_ARGS28_PASTE14,            \
     EJSON_PLACEHOLDER, EJSON_ARGS26_PASTE13, EJSON_PLACEHOLDER,               \
     EJSON_ARGS24_PASTE12, EJSON_PLACEHOLDER, EJSON_ARGS22_PASTE11,            \
     EJSON_PLACEHOLDER, EJSON_ARGS20_PASTE10, EJSON_PLACEHOLDER,               \
     EJSON_ARGS18_PASTE9, EJSON_PLACEHOLDER, EJSON_ARGS16_PASTE8,              \
     EJSON_PLACEHOLDER, EJSON_ARGS14_PASTE7, EJSON_PLACEHOLDER,                \
     EJSON_ARGS12_PASTE6, EJSON_PLACEHOLDER, EJSON_ARGS10_PASTE5,              \
     EJSON_PLACEHOLDER, EJSON_ARGS8_PASTE4, EJSON_PLACEHOLDER,                 \
     EJSON_ARGS6_PASTE3, EJSON_PLACEHOLDER, EJSON_ARGS4_PASTE2,                \
     EJSON_PLACEHOLDER, EJSON_ARGS2_PASTE1, EJSON_PLACEHOLDER,                 \
     EJSON_PLACEHOLDER)(__VA_ARGS__))

#define EJSON_ARGS2_PASTE1(func, v1, v2) func(v1, v2)
#define EJSON_ARGS4_PASTE2(func, v1, v2, v3, v4)                               \
   EJSON_ARGS2_PASTE1(func, v1, v2) EJSON_ARGS2_PASTE1(func, v3, v4)
#define EJSON_ARGS6_PASTE3(func, v1, v2, v3, v4, v5, v6)                       \
   EJSON_ARGS4_PASTE2(func, v1, v2, v3, v4) EJSON_ARGS2_PASTE1(func, v5, v6)
#define EJSON_ARGS8_PASTE4(func, v1, v2, v3, v4, v5, v6, v7, v8)               \
   EJSON_ARGS6_PASTE3(func, v1, v2, v3, v4, v5, v6)                            \
   EJSON_ARGS2_PASTE1(func, v7, v8)
#define EJSON_ARGS10_PASTE5(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)     \
   EJSON_ARGS8_PASTE4(func, v1, v2, v3, v4, v5, v6, v7, v8)                    \
   EJSON_ARGS2_PASTE1(func, v9, v10)
#define EJSON_ARGS12_PASTE6(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                            v11, v12)                                          \
   EJSON_ARGS10_PASTE5(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)          \
   func(v11, v12)
#define EJSON_ARGS14_PASTE7(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                            v11, v12, v13, v14)                                \
   EJSON_ARGS12_PASTE6(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,     \
                       v12)                                                    \
   func(v13, v14)
#define EJSON_ARGS16_PASTE8(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                            v11, v12, v13, v14, v15, v16)                      \
   EJSON_ARGS14_PASTE7(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,     \
                       v12, v13, v14)                                          \
   func(v15, v16)
#define EJSON_ARGS18_PASTE9(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                            v11, v12, v13, v14, v15, v16, v17, v18)            \
   EJSON_ARGS16_PASTE8(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,     \
                       v12, v13, v14, v15, v16)                                \
   func(v17, v18)
#define EJSON_ARGS20_PASTE10(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,    \
                             v11, v12, v13, v14, v15, v16, v17, v18, v19, v20) \
   EJSON_ARGS18_PASTE9(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,     \
                       v12, v13, v14, v15, v16, v17, v18)                      \
   func(v19, v20)
#define EJSON_ARGS22_PASTE11(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,    \
                             v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, \
                             v21, v22)                                         \
   EJSON_ARGS20_PASTE10(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20)           \
   func(v21, v22)
#define EJSON_ARGS24_PASTE12(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,    \
                             v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, \
                             v21, v22, v23, v24)                               \
   EJSON_ARGS22_PASTE11(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22) \
   func(v23, v24)
#define EJSON_ARGS26_PASTE13(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,    \
                             v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, \
                             v21, v22, v23, v24, v25, v26)                     \
   EJSON_ARGS24_PASTE12(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24)                                              \
   func(v25, v26)
#define EJSON_ARGS28_PASTE14(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,    \
                             v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, \
                             v21, v22, v23, v24, v25, v26, v27, v28)           \
   EJSON_ARGS26_PASTE13(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26)                                    \
   func(v27, v28)
#define EJSON_ARGS30_PASTE15(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,    \
                             v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, \
                             v21, v22, v23, v24, v25, v26, v27, v28, v29, v30) \
   EJSON_ARGS28_PASTE14(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28)                          \
   func(v29, v30)
#define EJSON_ARGS32_PASTE16(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,    \
                             v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, \
                             v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, \
                             v31, v32)                                         \
   EJSON_ARGS30_PASTE15(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30)                \
   func(v31, v32)
#define EJSON_ARGS34_PASTE17(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,    \
                             v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, \
                             v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, \
                             v31, v32, v33, v34)                               \
   EJSON_ARGS32_PASTE16(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32)      \
   func(v33, v34)
#define EJSON_ARGS36_PASTE18(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,    \
                             v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, \
                             v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, \
                             v31, v32, v33, v34, v35, v36)                     \
   EJSON_ARGS34_PASTE17(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34)                                                   \
   func(v35, v36)
#define EJSON_ARGS38_PASTE19(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,    \
                             v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, \
                             v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, \
                             v31, v32, v33, v34, v35, v36, v37, v38)           \
   EJSON_ARGS36_PASTE18(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36)                                         \
   func(v37, v38)
#define EJSON_ARGS40_PASTE20(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,    \
                             v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, \
                             v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, \
                             v31, v32, v33, v34, v35, v36, v37, v38, v39, v40) \
   EJSON_ARGS38_PASTE19(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38)                               \
   func(v39, v40)
#define EJSON_ARGS42_PASTE21(                                                  \
  func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, \
  v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31,   \
  v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42)                       \
   EJSON_ARGS40_PASTE20(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40)                     \
   func(v41, v42)
#define EJSON_ARGS44_PASTE22(                                                  \
  func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, \
  v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31,   \
  v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44)             \
   EJSON_ARGS42_PASTE21(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41, v42)           \
   func(v43, v44)
#define EJSON_ARGS46_PASTE23(                                                  \
  func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, \
  v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31,   \
  v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46)   \
   EJSON_ARGS44_PASTE22(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44) \
   func(v45, v46)
#define EJSON_ARGS48_PASTE24(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,    \
                             v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, \
                             v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, \
                             v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, \
                             v41, v42, v43, v44, v45, v46, v47, v48)           \
   EJSON_ARGS46_PASTE23(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, \
                        v45, v46)                                              \
   func(v47, v48)
#define EJSON_ARGS50_PASTE25(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,    \
                             v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, \
                             v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, \
                             v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, \
                             v41, v42, v43, v44, v45, v46, v47, v48, v49, v50) \
   EJSON_ARGS48_PASTE24(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, \
                        v45, v46, v47, v48)                                    \
   func(v49, v50)
#define EJSON_ARGS52_PASTE26(                                                  \
  func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, \
  v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31,   \
  v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46,   \
  v47, v48, v49, v50, v51, v52)                                                \
   EJSON_ARGS50_PASTE25(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, \
                        v45, v46, v47, v48, v49, v50)                          \
   func(v51, v52)
#define EJSON_ARGS54_PASTE27(                                                  \
  func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, \
  v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31,   \
  v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46,   \
  v47, v48, v49, v50, v51, v52, v53, v54)                                      \
   EJSON_ARGS52_PASTE26(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, \
                        v45, v46, v47, v48, v49, v50, v51, v52)                \
   func(v53, v54)
#define EJSON_ARGS56_PASTE28(                                                  \
  func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, \
  v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31,   \
  v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46,   \
  v47, v48, v49, v50, v51, v52, v53, v54, v55, v56)                            \
   EJSON_ARGS54_PASTE27(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, \
                        v45, v46, v47, v48, v49, v50, v51, v52, v53, v54)      \
   func(v55, v56)
#define EJSON_ARGS58_PASTE29(                                                  \
  func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, \
  v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31,   \
  v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46,   \
  v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58)                  \
   EJSON_ARGS56_PASTE28(                                                       \
     func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15,   \
     v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,     \
     v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43,     \
     v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56)          \
   func(v57, v58)
#define EJSON_ARGS60_PASTE30(                                                  \
  func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, \
  v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31,   \
  v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46,   \
  v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60)        \
   EJSON_ARGS58_PASTE29(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, \
                        v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, \
                        v56, v57, v58)                                         \
   func(v59, v60)
#define EJSON_ARGS62_PASTE31(                                                  \
  func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, \
  v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31,   \
  v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46,   \
  v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61,   \
  v62)                                                                         \
   EJSON_ARGS60_PASTE30(func, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,    \
                        v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, \
                        v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, \
                        v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, \
                        v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, \
                        v56, v57, v58, v59, v60)                               \
   func(v61, v62)

#define EJSON_PASTE_WITH_TYPE(...)                                             \
   EJSON_EXPAND(EJSON_GET_MACRO(                                               \
     __VA_ARGS__, EJSON_TYPE_PASTE62, EJSON_TYPE_PASTE61, EJSON_TYPE_PASTE60,  \
     EJSON_TYPE_PASTE59, EJSON_TYPE_PASTE58, EJSON_TYPE_PASTE57,               \
     EJSON_TYPE_PASTE56, EJSON_TYPE_PASTE55, EJSON_TYPE_PASTE54,               \
     EJSON_TYPE_PASTE53, EJSON_TYPE_PASTE52, EJSON_TYPE_PASTE51,               \
     EJSON_TYPE_PASTE50, EJSON_TYPE_PASTE49, EJSON_TYPE_PASTE48,               \
     EJSON_TYPE_PASTE47, EJSON_TYPE_PASTE46, EJSON_TYPE_PASTE45,               \
     EJSON_TYPE_PASTE44, EJSON_TYPE_PASTE43, EJSON_TYPE_PASTE42,               \
     EJSON_TYPE_PASTE41, EJSON_TYPE_PASTE40, EJSON_TYPE_PASTE39,               \
     EJSON_TYPE_PASTE38, EJSON_TYPE_PASTE37, EJSON_TYPE_PASTE36,               \
     EJSON_TYPE_PASTE35, EJSON_TYPE_PASTE34, EJSON_TYPE_PASTE33,               \
     EJSON_TYPE_PASTE32, EJSON_TYPE_PASTE31, EJSON_TYPE_PASTE30,               \
     EJSON_TYPE_PASTE29, EJSON_TYPE_PASTE28, EJSON_TYPE_PASTE27,               \
     EJSON_TYPE_PASTE26, EJSON_TYPE_PASTE25, EJSON_TYPE_PASTE24,               \
     EJSON_TYPE_PASTE23, EJSON_TYPE_PASTE22, EJSON_TYPE_PASTE21,               \
     EJSON_TYPE_PASTE20, EJSON_TYPE_PASTE19, EJSON_TYPE_PASTE18,               \
     EJSON_TYPE_PASTE17, EJSON_TYPE_PASTE16, EJSON_TYPE_PASTE15,               \
     EJSON_TYPE_PASTE14, EJSON_TYPE_PASTE13, EJSON_TYPE_PASTE12,               \
     EJSON_TYPE_PASTE11, EJSON_TYPE_PASTE10, EJSON_TYPE_PASTE9,                \
     EJSON_TYPE_PASTE8, EJSON_TYPE_PASTE7, EJSON_TYPE_PASTE6,                  \
     EJSON_TYPE_PASTE5, EJSON_TYPE_PASTE4, EJSON_TYPE_PASTE3,                  \
     EJSON_TYPE_PASTE2, EJSON_TYPE_PASTE1, EJSON_PLACEHOLDER,                  \
     EJSON_PLACEHOLDER)(__VA_ARGS__))

#if defined(EJSON_TAG_WITH_METHOD)
#define EJSON_TYPE_PASTE1(func, Type, v1) func(Type, v1)
#define EJSON_TYPE_PASTE2(func, Type, v1, v2)                                  \
   EJSON_TYPE_PASTE1(func, Type, v1) EJSON_TYPE_PASTE1(func, Type, v2)
#define EJSON_TYPE_PASTE3(func, Type, v1, v2, v3)                              \
   EJSON_TYPE_PASTE2(func, Type, v1, v2) EJSON_TYPE_PASTE1(func, Type, v3)
#define EJSON_TYPE_PASTE4(func, Type, v1, v2, v3, v4)                          \
   EJSON_TYPE_PASTE3(func, Type, v1, v2, v3) EJSON_TYPE_PASTE1(func, Type, v4)
#define EJSON_TYPE_PASTE5(func, Type, v1, v2, v3, v4, v5)                      \
   EJSON_TYPE_PASTE4(func, Type, v1, v2, v3, v4)                               \
   EJSON_TYPE_PASTE1(func, Type, v5)
#define EJSON_TYPE_PASTE6(func, Type, v1, v2, v3, v4, v5, v6)                  \
   EJSON_TYPE_PASTE5(func, Type, v1, v2, v3, v4, v5)                           \
   EJSON_TYPE_PASTE1(func, Type, v6)
#define EJSON_TYPE_PASTE7(func, Type, v1, v2, v3, v4, v5, v6, v7)              \
   EJSON_TYPE_PASTE6(func, Type, v1, v2, v3, v4, v5, v6)                       \
   EJSON_TYPE_PASTE1(func, Type, v7)
#define EJSON_TYPE_PASTE8(func, Type, v1, v2, v3, v4, v5, v6, v7, v8)          \
   EJSON_TYPE_PASTE7(func, Type, v1, v2, v3, v4, v5, v6, v7)                   \
   EJSON_TYPE_PASTE1(func, Type, v8)
#define EJSON_TYPE_PASTE9(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9)      \
   EJSON_TYPE_PASTE8(func, Type, v1, v2, v3, v4, v5, v6, v7, v8)               \
   EJSON_TYPE_PASTE1(func, Type, v9)
#define EJSON_TYPE_PASTE10(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10)                                                \
   EJSON_TYPE_PASTE9(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9)           \
   EJSON_TYPE_PASTE1(func, Type, v10)
#define EJSON_TYPE_PASTE11(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11)                                           \
   EJSON_TYPE_PASTE10(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)     \
   EJSON_TYPE_PASTE1(func, Type, v11)
#define EJSON_TYPE_PASTE12(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12)                                      \
   EJSON_TYPE_PASTE11(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11)                                                     \
   EJSON_TYPE_PASTE1(func, Type, v12)
#define EJSON_TYPE_PASTE13(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13)                                 \
   EJSON_TYPE_PASTE12(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12)                                                \
   EJSON_TYPE_PASTE1(func, Type, v13)
#define EJSON_TYPE_PASTE14(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14)                            \
   EJSON_TYPE_PASTE13(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13)                                           \
   EJSON_TYPE_PASTE1(func, Type, v14)
#define EJSON_TYPE_PASTE15(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15)                       \
   EJSON_TYPE_PASTE14(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14)                                      \
   EJSON_TYPE_PASTE1(func, Type, v15)
#define EJSON_TYPE_PASTE16(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16)                  \
   EJSON_TYPE_PASTE15(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15)                                 \
   EJSON_TYPE_PASTE1(func, Type, v16)
#define EJSON_TYPE_PASTE17(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17)             \
   EJSON_TYPE_PASTE16(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16)                            \
   EJSON_TYPE_PASTE1(func, Type, v17)
#define EJSON_TYPE_PASTE18(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18)        \
   EJSON_TYPE_PASTE17(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17)                       \
   EJSON_TYPE_PASTE1(func, Type, v18)
#define EJSON_TYPE_PASTE19(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19)   \
   EJSON_TYPE_PASTE18(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18)                  \
   EJSON_TYPE_PASTE1(func, Type, v19)
#define EJSON_TYPE_PASTE20(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,   \
                           v20)                                                \
   EJSON_TYPE_PASTE19(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19)             \
   EJSON_TYPE_PASTE1(func, Type, v20)
#define EJSON_TYPE_PASTE21(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,   \
                           v20, v21)                                           \
   EJSON_TYPE_PASTE20(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20)        \
   EJSON_TYPE_PASTE1(func, Type, v21)
#define EJSON_TYPE_PASTE22(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,   \
                           v20, v21, v22)                                      \
   EJSON_TYPE_PASTE21(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21)   \
   EJSON_TYPE_PASTE1(func, Type, v22)
#define EJSON_TYPE_PASTE23(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,   \
                           v20, v21, v22, v23)                                 \
   EJSON_TYPE_PASTE22(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22)                                                     \
   EJSON_TYPE_PASTE1(func, Type, v23)
#define EJSON_TYPE_PASTE24(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,   \
                           v20, v21, v22, v23, v24)                            \
   EJSON_TYPE_PASTE23(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23)                                                \
   EJSON_TYPE_PASTE1(func, Type, v24)
#define EJSON_TYPE_PASTE25(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,   \
                           v20, v21, v22, v23, v24, v25)                       \
   EJSON_TYPE_PASTE24(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24)                                           \
   EJSON_TYPE_PASTE1(func, Type, v25)
#define EJSON_TYPE_PASTE26(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,   \
                           v20, v21, v22, v23, v24, v25, v26)                  \
   EJSON_TYPE_PASTE25(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25)                                      \
   EJSON_TYPE_PASTE1(func, Type, v26)
#define EJSON_TYPE_PASTE27(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,   \
                           v20, v21, v22, v23, v24, v25, v26, v27)             \
   EJSON_TYPE_PASTE26(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26)                                 \
   EJSON_TYPE_PASTE1(func, Type, v27)
#define EJSON_TYPE_PASTE28(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,   \
                           v20, v21, v22, v23, v24, v25, v26, v27, v28)        \
   EJSON_TYPE_PASTE27(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27)                            \
   EJSON_TYPE_PASTE1(func, Type, v28)
#define EJSON_TYPE_PASTE29(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,   \
                           v20, v21, v22, v23, v24, v25, v26, v27, v28, v29)   \
   EJSON_TYPE_PASTE28(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28)                       \
   EJSON_TYPE_PASTE1(func, Type, v29)
#define EJSON_TYPE_PASTE30(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,   \
                           v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
                           v30)                                                \
   EJSON_TYPE_PASTE29(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29)                  \
   EJSON_TYPE_PASTE1(func, Type, v30)
#define EJSON_TYPE_PASTE31(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,   \
                           v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
                           v30, v31)                                           \
   EJSON_TYPE_PASTE30(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30)             \
   EJSON_TYPE_PASTE1(func, Type, v31)
#define EJSON_TYPE_PASTE32(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,   \
                           v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
                           v30, v31, v32)                                      \
   EJSON_TYPE_PASTE31(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31)        \
   EJSON_TYPE_PASTE1(func, Type, v32)
#define EJSON_TYPE_PASTE33(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,   \
                           v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
                           v30, v31, v32, v33)                                 \
   EJSON_TYPE_PASTE32(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32)   \
   EJSON_TYPE_PASTE1(func, Type, v33)
#define EJSON_TYPE_PASTE34(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,   \
                           v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
                           v30, v31, v32, v33, v34)                            \
   EJSON_TYPE_PASTE33(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33)                                                     \
   EJSON_TYPE_PASTE1(func, Type, v34)
#define EJSON_TYPE_PASTE35(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,   \
                           v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
                           v30, v31, v32, v33, v34, v35)                       \
   EJSON_TYPE_PASTE34(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34)                                                \
   EJSON_TYPE_PASTE1(func, Type, v35)
#define EJSON_TYPE_PASTE36(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,   \
                           v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
                           v30, v31, v32, v33, v34, v35, v36)                  \
   EJSON_TYPE_PASTE35(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34, v35)                                           \
   EJSON_TYPE_PASTE1(func, Type, v36)
#define EJSON_TYPE_PASTE37(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,   \
                           v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
                           v30, v31, v32, v33, v34, v35, v36, v37)             \
   EJSON_TYPE_PASTE36(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34, v35, v36)                                      \
   EJSON_TYPE_PASTE1(func, Type, v37)
#define EJSON_TYPE_PASTE38(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,   \
                           v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
                           v30, v31, v32, v33, v34, v35, v36, v37, v38)        \
   EJSON_TYPE_PASTE37(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34, v35, v36, v37)                                 \
   EJSON_TYPE_PASTE1(func, Type, v38)
#define EJSON_TYPE_PASTE39(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,   \
                           v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
                           v30, v31, v32, v33, v34, v35, v36, v37, v38, v39)   \
   EJSON_TYPE_PASTE38(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34, v35, v36, v37, v38)                            \
   EJSON_TYPE_PASTE1(func, Type, v39)
#define EJSON_TYPE_PASTE40(                                                    \
  func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14,     \
  v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
  v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40)                       \
   EJSON_TYPE_PASTE39(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34, v35, v36, v37, v38, v39)                       \
   EJSON_TYPE_PASTE1(func, Type, v40)
#define EJSON_TYPE_PASTE41(                                                    \
  func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14,     \
  v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
  v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41)                  \
   EJSON_TYPE_PASTE40(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34, v35, v36, v37, v38, v39, v40)                  \
   EJSON_TYPE_PASTE1(func, Type, v41)
#define EJSON_TYPE_PASTE42(                                                    \
  func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14,     \
  v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
  v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42)             \
   EJSON_TYPE_PASTE41(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34, v35, v36, v37, v38, v39, v40, v41)             \
   EJSON_TYPE_PASTE1(func, Type, v42)
#define EJSON_TYPE_PASTE43(                                                    \
  func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14,     \
  v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
  v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43)        \
   EJSON_TYPE_PASTE42(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34, v35, v36, v37, v38, v39, v40, v41, v42)        \
   EJSON_TYPE_PASTE1(func, Type, v43)
#define EJSON_TYPE_PASTE44(                                                    \
  func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14,     \
  v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
  v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44)   \
   EJSON_TYPE_PASTE43(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43)   \
   EJSON_TYPE_PASTE1(func, Type, v44)
#define EJSON_TYPE_PASTE45(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,   \
                           v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
                           v30, v31, v32, v33, v34, v35, v36, v37, v38, v39,   \
                           v40, v41, v42, v43, v44, v45)                       \
   EJSON_TYPE_PASTE44(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43,   \
                      v44)                                                     \
   EJSON_TYPE_PASTE1(func, Type, v45)
#define EJSON_TYPE_PASTE46(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,   \
                           v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
                           v30, v31, v32, v33, v34, v35, v36, v37, v38, v39,   \
                           v40, v41, v42, v43, v44, v45, v46)                  \
   EJSON_TYPE_PASTE45(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43,   \
                      v44, v45)                                                \
   EJSON_TYPE_PASTE1(func, Type, v46)
#define EJSON_TYPE_PASTE47(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,   \
                           v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
                           v30, v31, v32, v33, v34, v35, v36, v37, v38, v39,   \
                           v40, v41, v42, v43, v44, v45, v46, v47)             \
   EJSON_TYPE_PASTE46(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43,   \
                      v44, v45, v46)                                           \
   EJSON_TYPE_PASTE1(func, Type, v47)
#define EJSON_TYPE_PASTE48(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,   \
                           v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
                           v30, v31, v32, v33, v34, v35, v36, v37, v38, v39,   \
                           v40, v41, v42, v43, v44, v45, v46, v47, v48)        \
   EJSON_TYPE_PASTE47(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43,   \
                      v44, v45, v46, v47)                                      \
   EJSON_TYPE_PASTE1(func, Type, v48)
#define EJSON_TYPE_PASTE49(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9,     \
                           v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,   \
                           v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
                           v30, v31, v32, v33, v34, v35, v36, v37, v38, v39,   \
                           v40, v41, v42, v43, v44, v45, v46, v47, v48, v49)   \
   EJSON_TYPE_PASTE48(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43,   \
                      v44, v45, v46, v47, v48)                                 \
   EJSON_TYPE_PASTE1(func, Type, v49)
#define EJSON_TYPE_PASTE50(                                                    \
  func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14,     \
  v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
  v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44,   \
  v45, v46, v47, v48, v49, v50)                                                \
   EJSON_TYPE_PASTE49(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43,   \
                      v44, v45, v46, v47, v48, v49)                            \
   EJSON_TYPE_PASTE1(func, Type, v50)
#define EJSON_TYPE_PASTE51(                                                    \
  func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14,     \
  v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
  v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44,   \
  v45, v46, v47, v48, v49, v50, v51)                                           \
   EJSON_TYPE_PASTE50(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43,   \
                      v44, v45, v46, v47, v48, v49, v50)                       \
   EJSON_TYPE_PASTE1(func, Type, v51)
#define EJSON_TYPE_PASTE52(                                                    \
  func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14,     \
  v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
  v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44,   \
  v45, v46, v47, v48, v49, v50, v51, v52)                                      \
   EJSON_TYPE_PASTE51(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43,   \
                      v44, v45, v46, v47, v48, v49, v50, v51)                  \
   EJSON_TYPE_PASTE1(func, Type, v52)
#define EJSON_TYPE_PASTE53(                                                    \
  func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14,     \
  v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
  v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44,   \
  v45, v46, v47, v48, v49, v50, v51, v52, v53)                                 \
   EJSON_TYPE_PASTE52(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43,   \
                      v44, v45, v46, v47, v48, v49, v50, v51, v52)             \
   EJSON_TYPE_PASTE1(func, Type, v53)
#define EJSON_TYPE_PASTE54(                                                    \
  func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14,     \
  v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
  v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44,   \
  v45, v46, v47, v48, v49, v50, v51, v52, v53, v54)                            \
   EJSON_TYPE_PASTE53(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43,   \
                      v44, v45, v46, v47, v48, v49, v50, v51, v52, v53)        \
   EJSON_TYPE_PASTE1(func, Type, v54)
#define EJSON_TYPE_PASTE55(                                                    \
  func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14,     \
  v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
  v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44,   \
  v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55)                       \
   EJSON_TYPE_PASTE54(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43,   \
                      v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54)   \
   EJSON_TYPE_PASTE1(func, Type, v55)
#define EJSON_TYPE_PASTE56(                                                    \
  func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14,     \
  v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
  v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44,   \
  v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56)                  \
   EJSON_TYPE_PASTE55(                                                         \
     func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14,  \
     v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28,     \
     v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42,     \
     v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55)          \
   EJSON_TYPE_PASTE1(func, Type, v56)
#define EJSON_TYPE_PASTE57(                                                    \
  func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14,     \
  v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
  v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44,   \
  v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57)             \
   EJSON_TYPE_PASTE56(                                                         \
     func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14,  \
     v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28,     \
     v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42,     \
     v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56)     \
   EJSON_TYPE_PASTE1(func, Type, v57)
#define EJSON_TYPE_PASTE58(                                                    \
  func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14,     \
  v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
  v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44,   \
  v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58)        \
   EJSON_TYPE_PASTE57(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43,   \
                      v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54,   \
                      v55, v56, v57)                                           \
   EJSON_TYPE_PASTE1(func, Type, v58)
#define EJSON_TYPE_PASTE59(                                                    \
  func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14,     \
  v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
  v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44,   \
  v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59)   \
   EJSON_TYPE_PASTE58(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43,   \
                      v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54,   \
                      v55, v56, v57, v58)                                      \
   EJSON_TYPE_PASTE1(func, Type, v59)
#define EJSON_TYPE_PASTE60(                                                    \
  func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14,     \
  v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
  v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44,   \
  v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59,   \
  v60)                                                                         \
   EJSON_TYPE_PASTE59(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43,   \
                      v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54,   \
                      v55, v56, v57, v58, v59)                                 \
   EJSON_TYPE_PASTE1(func, Type, v60)
#define EJSON_TYPE_PASTE61(                                                    \
  func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14,     \
  v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
  v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44,   \
  v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59,   \
  v60, v61)                                                                    \
   EJSON_TYPE_PASTE60(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43,   \
                      v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54,   \
                      v55, v56, v57, v58, v59, v60)                            \
   EJSON_TYPE_PASTE1(func, Type, v61)
#define EJSON_TYPE_PASTE62(                                                    \
  func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14,     \
  v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,   \
  v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44,   \
  v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59,   \
  v60, v61, v62)                                                               \
   EJSON_TYPE_PASTE61(func, Type, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,     \
                      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,   \
                      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,   \
                      v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43,   \
                      v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54,   \
                      v55, v56, v57, v58, v59, v60, v61)                       \
   EJSON_TYPE_PASTE1(func, Type, v62)

#endif

#define TO_JSON_FRIEND_FUNC(Type, ejson_j, ejson_t)                            \
   friend inline void to_json(ejson::JObject &ejson_j, const Type &ejson_t)
#define FROM_JSON_FRIEND_FUNC(Type, ejson_j, ejson_t)                          \
   friend inline void from_json(const ejson::JObject &ejson_j, Type &ejson_t)

#define TO_JSON_FUNC(Type, ejson_j, ejson_t)                                   \
   inline void to_json(ejson::JObject &ejson_j, const Type &ejson_t)
#define FROM_JSON_FUNC(Type, ejson_j, ejson_t)                                 \
   inline void from_json(const ejson::JObject &ejson_j, Type &ejson_t)

#if defined(EJSON_TAG_WITH_METHOD)
#define EJSON_HAS_MEMBER_ALIAS(Type, Member)                                   \
   template <typename T, typename... Args>                                     \
   struct has_member_alias_ejson_##Type##_##Member                             \
   {                                                                           \
   private:                                                                    \
      template <typename U>                                                    \
      static auto Check(int)                                                   \
        -> decltype(std::declval<U>().alias_ejson_##Member(                    \
                      std::declval<Args>()...),                                \
                    std::true_type());                                         \
      template <typename U>                                                    \
      static std::false_type Check(...);                                       \
                                                                               \
   public:                                                                     \
      enum {                                                                   \
         value = std::is_same<decltype(Check<T>(0)), std::true_type>::value    \
      };                                                                       \
   };

#define EJSON_HAS_MEMBER_OPTION(Type, Member)                                  \
   template <typename T, typename... Args>                                     \
   struct has_member_option_ejson_##Type##_##Member                            \
   {                                                                           \
   private:                                                                    \
      template <typename U>                                                    \
      static auto Check(int)                                                   \
        -> decltype(std::declval<U>().option_ejson_##Member(                   \
                      std::declval<Args>()...),                                \
                    std::true_type());                                         \
      template <typename U>                                                    \
      static std::false_type Check(...);                                       \
                                                                               \
   public:                                                                     \
      enum {                                                                   \
         value = std::is_same<decltype(Check<T>(0)), std::true_type>::value    \
      };                                                                       \
   };

#define EJSON_HAS_MEMBER_CUSTOM(Type, Member)                                  \
   template <typename T, typename... Args>                                     \
   struct has_member_custom_ejson_##Type##_##Member                            \
   {                                                                           \
   private:                                                                    \
      template <typename U>                                                    \
      static auto Check(int)                                                   \
        -> decltype(std::declval<U>().custom_ejson_##Member(                   \
                      std::declval<Args>()...),                                \
                    std::true_type());                                         \
      template <typename U>                                                    \
      static std::false_type Check(...);                                       \
                                                                               \
   public:                                                                     \
      enum {                                                                   \
         value = std::is_same<decltype(Check<T>(0)), std::true_type>::value    \
      };                                                                       \
   };
#endif

#if defined(EJSON_TAG_WITH_METHOD)
#define EJSON_TEMPLATE_HELPER_FRIEND(Type, v1)                                 \
   template <class T1, class T2>                                               \
   friend inline void to_json##Type##_##v1(T1 &ejson_j, const T2 &ejson_t)     \
   {                                                                           \
      ejson_j.at(ejson_t.alias_ejson_##v1()).get_from(ejson_t.v1);             \
   }                                                                           \
   template <class T1, class T2>                                               \
   friend inline void from_json##Type##_##v1(const T1 &ejson_j, T2 &ejson_t)   \
   {                                                                           \
      ejson_j.at(ejson_t.alias_ejson_##v1()).get_to(ejson_t.v1);               \
   }                                                                           \
   template <class T1, class T2>                                               \
   friend inline void to_json_option_##Type##_##v1(T1       &ejson_j,          \
                                                   const T2 &ejson_t)          \
   {                                                                           \
      const_cast<Type &>(ejson_t).v1 = ejson_t.option_ejson_##v1();            \
   }                                                                           \
   template <class T1, class T2>                                               \
   friend inline void from_json_option_##Type##_##v1(const T1 &ejson_j,        \
                                                     T2       &ejson_t)        \
   {                                                                           \
      const_cast<Type &>(ejson_t).v1 = ejson_t.option_ejson_##v1();            \
   }                                                                           \
   template <class T1, class T2>                                               \
   friend inline void to_json_custom_##Type##_##v1(T1       &ejson_j,          \
                                                   const T2 &ejson_t)          \
   {                                                                           \
      auto &&o    = const_cast<Type &>(ejson_t);                               \
      auto &&func = ejson_t.custom_ejson_##v1();                               \
      func(&ejson_j, &o.v1, ejson::EJsonAction::kToJson);                      \
   }                                                                           \
   template <class T1, class T2>                                               \
   friend inline void from_json_custom_##Type##_##v1(const T1 &ejson_j,        \
                                                     T2       &ejson_t)        \
   {                                                                           \
      auto &&o    = const_cast<ejson::JObject &>(ejson_j);                     \
      auto &&func = ejson_t.custom_ejson_##v1();                               \
      func(&o, &ejson_t.v1, ejson::EJsonAction::kFromJson);                    \
   }

#define EJSON_TEMPLATE_HELPER(Type, v1)                                        \
   template <class T1, class T2>                                               \
   inline void to_json##Type##_##v1(T1 &ejson_j, const T2 &ejson_t)            \
   {                                                                           \
      ejson_j.at(ejson_t.alias_ejson_##v1()).get_from(ejson_t.v1);             \
   }                                                                           \
   template <class T1, class T2>                                               \
   inline void from_json##Type##_##v1(const T1 &ejson_j, T2 &ejson_t)          \
   {                                                                           \
      ejson_j.at(ejson_t.alias_ejson_##v1()).get_to(ejson_t.v1);               \
   }                                                                           \
   template <class T1, class T2>                                               \
   inline void to_json_option_##Type##_##v1(T1 &ejson_j, const T2 &ejson_t)    \
   {                                                                           \
      const_cast<Type &>(ejson_t).v1 = ejson_t.option_ejson_##v1();            \
   }                                                                           \
   template <class T1, class T2>                                               \
   inline void from_json_option_##Type##_##v1(const T1 &ejson_j, T2 &ejson_t)  \
   {                                                                           \
      ejson_t.v1 = ejson_t.option_ejson_##v1();                                \
   }                                                                           \
   template <class T1, class T2>                                               \
   inline void to_json_custom_##Type##_##v1(T1 &ejson_j, const T2 &ejson_t)    \
   {                                                                           \
      auto &&o    = const_cast<Type &>(ejson_t);                               \
      auto &&func = ejson_t.custom_ejson_##v1();                               \
      func(&ejson_j, &o.v1, ejson::EJsonAction::kToJson);                      \
   }                                                                           \
   template <class T1, class T2>                                               \
   inline void from_json_custom_##Type##_##v1(const T1 &ejson_j, T2 &ejson_t)  \
   {                                                                           \
      auto &&o    = const_cast<ejson::JObject &>(ejson_j);                     \
      auto &&func = ejson_t.custom_ejson_##v1();                               \
      func(&o, &ejson_t.v1, ejson::EJsonAction::kFromJson);                    \
   }

#endif

#if defined(EJSON_TAG_WITH_METHOD)
#define EJSON_PRE_HANDLE_FRIEND(Type, ...)                                     \
   EJSON_PASTE_WITH_TYPE(EJSON_HAS_MEMBER_ALIAS, Type, __VA_ARGS__)            \
   EJSON_PASTE_WITH_TYPE(EJSON_HAS_MEMBER_OPTION, Type, __VA_ARGS__)           \
   EJSON_PASTE_WITH_TYPE(EJSON_HAS_MEMBER_CUSTOM, Type, __VA_ARGS__)           \
   EJSON_PASTE_WITH_TYPE(EJSON_TEMPLATE_HELPER_FRIEND, Type, __VA_ARGS__)

#define EJSON_PRE_HANDLE(Type, ...)                                            \
   EJSON_PASTE_WITH_TYPE(EJSON_HAS_MEMBER_ALIAS, Type, __VA_ARGS__)            \
   EJSON_PASTE_WITH_TYPE(EJSON_HAS_MEMBER_OPTION, Type, __VA_ARGS__)           \
   EJSON_PASTE_WITH_TYPE(EJSON_HAS_MEMBER_CUSTOM, Type, __VA_ARGS__)           \
   EJSON_PASTE_WITH_TYPE(EJSON_TEMPLATE_HELPER, Type, __VA_ARGS__)
#endif

#define EJSON_TO(v1)   ejson_j.at(#v1).get_from(ejson_t.v1);
#define EJSON_FROM(v1) ejson_j.at(#v1).get_to(ejson_t.v1);

#if __cplusplus >= 2017L && defined(EJSON_TAG_WITH_METHOD)
#define EJSON_TO_IF(Type, v1)                                                  \
   try                                                                         \
   {                                                                           \
      if constexpr (has_member_custom_ejson_##Type##_##v1<Type>::value)        \
      {                                                                        \
         to_json_custom_##Type##_##v1(ejson_j, ejson_t);                       \
      }                                                                        \
      else if constexpr (has_member_alias_ejson_##Type##_##v1<Type>::value)    \
      {                                                                        \
         to_json##Type##_##v1(ejson_j, ejson_t);                               \
      }                                                                        \
      else { ejson_j.at(#v1).get_from(ejson_t.v1); }                           \
   }                                                                           \
   catch (std::exception const &e)                                             \
   {                                                                           \
      if constexpr (has_member_option_ejson_##Type##_##v1<Type>::value)        \
      {                                                                        \
         to_json_option_##Type##_##v1(ejson_j, ejson_t);                       \
      }                                                                        \
      else                                                                     \
      {                                                                        \
         EJSON_THROW_ERROR_WITH_TYPE(                                          \
           e.what(),                                                           \
           "error in:type->" #Type " field->" #v1                              \
           ",maybe the json field not equal this field name.",                 \
           Type);                                                              \
      }                                                                        \
   }

#define EJSON_FROM_IF(Type, v1)                                                \
   try                                                                         \
   {                                                                           \
      if constexpr (has_member_custom_ejson_##Type##_##v1<Type>::value)        \
      {                                                                        \
         from_json_custom_##Type##_##v1(ejson_j, ejson_t);                     \
      }                                                                        \
      else if constexpr (has_member_alias_ejson_##Type##_##v1<Type>::value)    \
      {                                                                        \
         from_json##Type##_##v1(ejson_j, ejson_t);                             \
      }                                                                        \
      else { ejson_j.at(#v1).get_to(ejson_t.v1); }                             \
   }                                                                           \
   catch (std::exception const &e)                                             \
   {                                                                           \
      if constexpr (has_member_option_ejson_##Type##_##v1<Type>::value)        \
      {                                                                        \
         from_json_option_##Type##_##v1(ejson_j, ejson_t);                     \
      }                                                                        \
      else                                                                     \
      {                                                                        \
         EJSON_THROW_ERROR_WITH_TYPE(                                          \
           e.what(),                                                           \
           "error in:type->" #Type " field->" #v1                              \
           ",maybe the json field not equal this field name.",                 \
           Type);                                                              \
      }                                                                        \
   }
#endif

/**
 * @brief only supported c++17 or above
 */
#if __cplusplus >= 2017L && defined(EJSON_TAG_WITH_METHOD)

#define ALIAS_EJSON(v1, v2)                                                    \
   inline const char *alias_ejson_##v1() const                                 \
   {                                                                           \
      (void)v1;                                                                \
      return #v2;                                                              \
   }
#define OPTION_EJSON(v1, v2)                                                   \
   inline auto option_ejson_##v1() const                                       \
   {                                                                           \
      (void)v1;                                                                \
      return v2;                                                               \
   }

#define CUSTOM_EJSON(v1, v2)                                                   \
   inline ejson::custom_func_t custom_ejson_##v1() const                       \
   {                                                                           \
      (void)v1;                                                                \
      return v2;                                                               \
   }

#define AUTO_GEN_INTRUSIVE(Type, ...)                                          \
   EJSON_PRE_HANDLE_FRIEND(Type, __VA_ARGS__)                                  \
   TO_JSON_FRIEND_FUNC(Type, ejson_j, ejson_t)                                 \
   {                                                                           \
      try                                                                      \
      {                                                                        \
         EJSON_EXPAND(EJSON_PASTE_WITH_TYPE(EJSON_TO_IF, Type, __VA_ARGS__))   \
      }                                                                        \
      catch (std::exception const &e)                                          \
      {                                                                        \
         EJSON_THROW_ERROR_WITH_TYPE(e.what(), "to_json()", Type);             \
      }                                                                        \
   }                                                                           \
   FROM_JSON_FRIEND_FUNC(Type, ejson_j, ejson_t)                               \
   {                                                                           \
      try                                                                      \
      {                                                                        \
         EJSON_EXPAND(EJSON_PASTE_WITH_TYPE(EJSON_FROM_IF, Type, __VA_ARGS__)) \
      }                                                                        \
      catch (std::exception const &e)                                          \
      {                                                                        \
         EJSON_THROW_ERROR_WITH_TYPE(e.what(), "from_json()", Type);           \
      }                                                                        \
   }

#define AUTO_GEN_NON_INTRUSIVE(Type, ...)                                      \
   EJSON_PRE_HANDLE(Type, __VA_ARGS__)                                         \
   TO_JSON_FUNC(Type, ejson_j, ejson_t)                                        \
   {                                                                           \
      try                                                                      \
      {                                                                        \
         EJSON_EXPAND(EJSON_PASTE_WITH_TYPE(EJSON_TO_IF, Type, __VA_ARGS__))   \
      }                                                                        \
      catch (std::exception const &e)                                          \
      {                                                                        \
         EJSON_THROW_ERROR_WITH_TYPE(e.what(), "to_json()", Type);             \
      }                                                                        \
   }                                                                           \
   FROM_JSON_FUNC(Type, ejson_j, ejson_t)                                      \
   {                                                                           \
      try                                                                      \
      {                                                                        \
         EJSON_EXPAND(EJSON_PASTE_WITH_TYPE(EJSON_FROM_IF, Type, __VA_ARGS__)) \
      }                                                                        \
      catch (std::exception const &e)                                          \
      {                                                                        \
         EJSON_THROW_ERROR_WITH_TYPE(e.what(), "from_json()", Type);           \
      }                                                                        \
   }
#else
/**
 * @brief intrusive automatic generation of from_json and to_json functions of
 * custom types
 */
#define AUTO_GEN_INTRUSIVE(Type, ...)                                          \
   TO_JSON_FRIEND_FUNC(Type, ejson_j, ejson_t)                                 \
   {                                                                           \
      try                                                                      \
      {                                                                        \
         EJSON_EXPAND(EJSON_PASTE(EJSON_TO, __VA_ARGS__))                      \
      }                                                                        \
      catch (std::exception const &e)                                          \
      {                                                                        \
         EJSON_THROW_ERROR_WITH_TYPE(e.what(), "to_json()", Type);             \
      }                                                                        \
   }                                                                           \
   FROM_JSON_FRIEND_FUNC(Type, ejson_j, ejson_t)                               \
   {                                                                           \
      try                                                                      \
      {                                                                        \
         EJSON_EXPAND(EJSON_PASTE(EJSON_FROM, __VA_ARGS__))                    \
      }                                                                        \
      catch (std::exception const &e)                                          \
      {                                                                        \
         EJSON_THROW_ERROR_WITH_TYPE(e.what(), "from_json()", Type);           \
      }                                                                        \
   }

/**
 * @brief Non-intrusive automatic generation of from_json and to_json functions
 * of custom types
 */
#define AUTO_GEN_NON_INTRUSIVE(Type, ...)                                      \
   TO_JSON_FUNC(Type, ejson_j, ejson_t)                                        \
   {                                                                           \
      try                                                                      \
      {                                                                        \
         EJSON_EXPAND(EJSON_PASTE(EJSON_TO, __VA_ARGS__))                      \
      }                                                                        \
      catch (std::exception const &e)                                          \
      {                                                                        \
         EJSON_THROW_ERROR_WITH_TYPE(e.what(), "to_json()", Type);             \
      }                                                                        \
   }                                                                           \
   FROM_JSON_FUNC(Type, ejson_j, ejson_t)                                      \
   {                                                                           \
      try                                                                      \
      {                                                                        \
         EJSON_EXPAND(EJSON_PASTE(EJSON_FROM, __VA_ARGS__))                    \
      }                                                                        \
      catch (std::exception const &e)                                          \
      {                                                                        \
         EJSON_THROW_ERROR_WITH_TYPE(e.what(), "from_json()", Type);           \
      }                                                                        \
   }
#endif

#define EJSON_ALIAS_TO(v1, v2)   ejson_j.at(#v2).get_from(ejson_t.v1);
#define EJSON_ALIAS_FROM(v1, v2) ejson_j.at(#v2).get_to(ejson_t.v1);

/**
 * @brief Non-intrusive automatic generation of from_json and to_json functions
 * of custom types with alias
 */
#define AUTO_GEN_ALIAS_NON_INTRUSIVE(Type, ...)                                \
   TO_JSON_FUNC(Type, ejson_j, ejson_t)                                        \
   {                                                                           \
      try                                                                      \
      {                                                                        \
         EJSON_EXPAND(EJSON_ARG2_PASTE(EJSON_ALIAS_TO, __VA_ARGS__))           \
      }                                                                        \
      catch (std::exception const &e)                                          \
      {                                                                        \
         EJSON_THROW_ERROR_WITH_TYPE(e.what(), "to_json()", Type);             \
      }                                                                        \
   }                                                                           \
   FROM_JSON_FUNC(Type, ejson_j, ejson_t)                                      \
   {                                                                           \
      try                                                                      \
      {                                                                        \
         EJSON_EXPAND(EJSON_ARG2_PASTE(EJSON_ALIAS_FROM, __VA_ARGS__))         \
      }                                                                        \
      catch (std::exception const &e)                                          \
      {                                                                        \
         EJSON_THROW_ERROR_WITH_TYPE(e.what(), "from_json()", Type);           \
      }                                                                        \
   }

/**
 * @brief intrusive automatic generation of from_json and to_json functions of
 * custom types with alias
 */
#define AUTO_GEN_ALIAS_INTRUSIVE(Type, ...)                                    \
   TO_JSON_FRIEND_FUNC(Type, ejson_j, ejson_t)                                 \
   {                                                                           \
      try                                                                      \
      {                                                                        \
         EJSON_EXPAND(EJSON_ARG2_PASTE(EJSON_ALIAS_TO, __VA_ARGS__))           \
      }                                                                        \
      catch (std::exception const &e)                                          \
      {                                                                        \
         EJSON_THROW_ERROR_WITH_TYPE(e.what(), "to_json()", Type);             \
      }                                                                        \
   }                                                                           \
   FROM_JSON_FRIEND_FUNC(Type, ejson_j, ejson_t)                               \
   {                                                                           \
      try                                                                      \
      {                                                                        \
         EJSON_EXPAND(EJSON_ARG2_PASTE(EJSON_ALIAS_FROM, __VA_ARGS__))         \
      }                                                                        \
      catch (std::exception const &e)                                          \
      {                                                                        \
         EJSON_THROW_ERROR_WITH_TYPE(e.what(), "from_json()", Type);           \
      }                                                                        \
   }

#define EJSON_COUT_OPERATOR_PREFIX(Type)                                       \
   inline std::ostream &operator<<(std::ostream &os, Type const &src)

#define EJSON_COUT_GEN(Type)                                                   \
   EJSON_COUT_OPERATOR_PREFIX(Type)                                            \
   {                                                                           \
      os << #Type << ejson::JObject(src).to_string();                          \
      return os;                                                               \
   }

/**
 *@brief Automatically generate operator<< overload of type based on parameters,
 *and support multiple parameter generation
 */
#define ENABLE_JSON_COUT(...)                                                  \
   EJSON_EXPAND(EJSON_PASTE(EJSON_COUT_GEN, __VA_ARGS__))
