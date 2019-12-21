/*
 * * file name: grammar.h
 * * description: ...
 * * author: lemonxu
 * * create time:2019 5月 21
 * */

#ifndef _GRAMMAR_H_
#define _GRAMMAR_H_

#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::stringstream;
using std::vector;

namespace Pepper
{
enum class CPP_STANDARD : size_t
{
    CPP_98 = 1,
    CPP_11 = 2,
    CPP_14 = 3,
};

enum class MSG_TYPE : uint8_t
{
    SIMPLE = 0,
    STRING = 1,
    STRUCT = 2,
    ENUM = 3,
};

struct BaseStruct
{
    string name;
    string full_name;
    MSG_TYPE msg_type;

    BaseStruct(const string& name_, const string full_name_, MSG_TYPE type_)
        : name(name_), full_name(full_name_), msg_type(type_)
    {
    }
    virtual ~BaseStruct() = default;
    // return false if is nothing output
    virtual bool DeclareStr(stringstream& ss_, const string& prefix_, CPP_STANDARD standard_) const = 0;
    // return false if is nothing output
    virtual bool ImplStr(stringstream& ss_, const string& prefix_, CPP_STANDARD standard_) const = 0;
};

struct BuiltInStruct : public BaseStruct
{
    BuiltInStruct(const string& name_, const string& full_name_, MSG_TYPE type_ = MSG_TYPE::SIMPLE)
        : BaseStruct{name_, full_name_, type_}
    {
    }
    virtual ~BuiltInStruct() = default;
    virtual bool DeclareStr(stringstream& ss_, const string& prefix_, CPP_STANDARD standard_) const final
    {
        return false;
    };
    virtual bool ImplStr(stringstream& ss_, const string& prefix_, CPP_STANDARD standard_) const final
    {
        return false;
    };
};

struct BaseMessageStruct;
struct EnumStruct : public BaseStruct
{
    const BaseMessageStruct* parent_message = nullptr;

    EnumStruct() : BaseStruct{"", "", MSG_TYPE::ENUM} {}
    virtual ~EnumStruct() = default;
    virtual bool DeclareStr(stringstream& ss_, const string& prefix_, CPP_STANDARD standard_) const final
    {
        return false;
    };
    virtual bool ImplStr(stringstream& ss_, const string& prefix_, CPP_STANDARD standard_) const final
    {
        return false;
    };
};

struct BaseMessageStruct : public BaseStruct
{
    string pb_full_name;

    BaseMessageStruct() : BaseStruct{"", "", MSG_TYPE::STRUCT}, pb_full_name("") {}
    virtual ~BaseMessageStruct() = default;
    virtual bool DeclareStr(stringstream& ss_, const string& prefix_, CPP_STANDARD standard_) const override
    {
        return false;
    }
    virtual bool ImplStr(stringstream& ss_, const string& prefix_, CPP_STANDARD standard_) const override
    {
        return false;
    }
};

struct Field;
struct MessageStruct : public BaseMessageStruct
{
    vector<BaseStruct*> nest_message;
    vector<Field*> fields;

    MessageStruct() = default;
    virtual ~MessageStruct() = default;
    virtual bool DeclareStr(stringstream& ss_, const string& prefix_, CPP_STANDARD standard_) const final;
    virtual bool ImplStr(stringstream& ss_, const string& prefix_, CPP_STANDARD standard_) const final;

private:
    void ConstructorDeclareStr(stringstream& ss_, const string& prefix_) const;
    void ResetDeclareStr(stringstream& ss_, const string& prefix_) const;
    void ClearDeclareStr(stringstream& ss_, const string& prefix_) const;
    void FromPbDeclareStr(stringstream& ss_, const string& prefix_) const;
    void ToPbDeclareStr(stringstream& ss_, const string& prefix_) const;

    void ConstructorImplStr(stringstream& ss_, const string& prefix_) const;
    void ResetImplStr(stringstream& ss_, const string& prefix_) const;
    void ClearImplStr(stringstream& ss_, const string& prefix_) const;
    void FromPbImplStr(stringstream& ss_, const string& prefix_) const;
    void ToPbImplStr(stringstream& ss_, const string& prefix_) const;
};

// an unknow struct is can not found declare in this proto file,
// it may be declare in dependendent files
struct UnknowStruct : public BaseStruct
{
    UnknowStruct() : UnknowStruct("", "") {}
    UnknowStruct(const string& name_, const string& full_name_) : BaseStruct{name_, full_name_, MSG_TYPE::STRUCT} {}
    virtual ~UnknowStruct() = default;
    virtual bool DeclareStr(stringstream& ss_, const string& prefix_, CPP_STANDARD standard_) const final
    {
        return false;
    };
    virtual bool ImplStr(stringstream& ss_, const string& prefix_, CPP_STANDARD standard_) const final
    {
        return false;
    };
};

struct Field
{
    string name;
    string default_value;
    size_t len = 0;
    bool fixed_len = false;
    const BaseStruct* type_message = nullptr;

    void DeclareStr(stringstream& ss_, const string& prefix_, CPP_STANDARD standard_) const;

    void SetPbStr(stringstream& ss_, const string& prefix_) const;
    void SetSingleVarStr(stringstream& ss_, const string& prefix_) const;
    void SetSingleMessageStr(stringstream& ss_, const string& prefix_) const;
    void SetArrayVarStr(stringstream& ss_, const string& prefix_) const;
    void SetFixedArrayVarStr(stringstream& ss_, const string& prefix_) const;
    void SetArrayMessageStr(stringstream& ss_, const string& prefix_) const;
    void SetFixedArrayMessageStr(stringstream& ss_, const string& prefix_) const;
    void SetStringStr(stringstream& ss_, const string& prefix_) const;

    void GetPbStr(stringstream& ss_, const string& prefix_) const;
    void GetSingleVarStr(stringstream& ss_, const string& prefix_) const;
    void GetSingleMessageStr(stringstream& ss_, const string& prefix_) const;
    void GetArrayVarStr(stringstream& ss_, const string& prefix_) const;
    void GetFixedArrayVarStr(stringstream& ss_, const string& prefix_) const;
    void GetArrayMessageStr(stringstream& ss_, const string& prefix_) const;
    void GetFixedArrayMessageStr(stringstream& ss_, const string& prefix_) const;
    void GetStringStr(stringstream& ss_, const string& prefix_) const;

    void InitStr(stringstream& ss_, const string& prefix_) const;
    void InitSingleVarStr(stringstream& ss_, const string& prefix_) const;
    void InitSingleMessageStr(stringstream& ss_, const string& prefix_) const;
    void InitArrayVarStr(stringstream& ss_, const string& prefix_) const;
    void InitFixedArrayVarStr(stringstream& ss_, const string& prefix_) const;
    void InitArrayMessageStr(stringstream& ss_, const string& prefix_) const;
    void InitFixedArrayMessageStr(stringstream& ss_, const string& prefix_) const;
    void InitStringStr(stringstream& ss_, const string& prefix_) const;
};

struct SyntaxTree
{
    vector<string> includes;
    vector<string> public_includes;
    string space;
    vector<BaseStruct*> root;
};

}  // namespace Pepper

#endif
