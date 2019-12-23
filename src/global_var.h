/*
 * * file name: global_var.h
 * * description: ...
 * * author: snow
 * * create time:2019 12 23
 * */

#ifndef _GLOBAL_VAR_H_
#define _GLOBAL_VAR_H_

#include <string>

namespace Pepper
{
enum class CPP_STANDARD : size_t
{
    CPP_98 = 1,
    CPP_11 = 2,
    CPP_14 = 3,
};

struct GlobalVar
{
    static std::string message_prefix;
    static std::string indent;
    static CPP_STANDARD standard;
};

}  // namespace Pepper

#endif
