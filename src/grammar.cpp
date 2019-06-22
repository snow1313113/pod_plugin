/*
 * * file name: grammar.cpp
 * * description: ...
 * * author: lemonxu
 * * create time:2019 5月 22
 * */
#include "grammar.h"
#include <cassert>
#include "format_conf.h"

namespace Pepper
{
bool MessageStruct::DeclareStr(stringstream& ss_, const string& prefix_, CPP_STANDARD standard_) const
{
    ss_ << prefix_ << "struct " << name << "\n" << prefix_ << "{\n";

    for (auto child : nest_message)
    {
        if (child->DeclareStr(ss_, prefix_ + g_indent, standard_))
            ss_ << "\n";
    }

    for (auto field : fields)
        field->DeclareStr(ss_, prefix_ + g_indent, standard_);

    ss_ << "\n";
    if (standard_ == CPP_STANDARD::CPP_98)
        ConstructorDeclareStr(ss_, prefix_ + g_indent);

    ResetDeclareStr(ss_, prefix_ + g_indent);
    ClearDeclareStr(ss_, prefix_ + g_indent);
    FromPbDeclareStr(ss_, prefix_ + g_indent);
    ToPbDeclareStr(ss_, prefix_ + g_indent);

    ss_ << prefix_ << "};\n";
    return true;
}

void MessageStruct::ConstructorDeclareStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << name << "();\n";
}

void MessageStruct::ResetDeclareStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << "void Reset();\n";
}

void MessageStruct::ClearDeclareStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << "void Clear();\n";
}

void MessageStruct::FromPbDeclareStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << "bool FromPb(const " << pb_full_name << "& msg_);\n";
}

void MessageStruct::ToPbDeclareStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << "bool ToPb(" << pb_full_name << "* msg_) const;\n";
}

bool MessageStruct::ImplStr(stringstream& ss_, const string& prefix_, CPP_STANDARD standard_) const
{
    for (auto child : nest_message)
    {
        if (child->ImplStr(ss_, prefix_, standard_))
            ss_ << "\n";
    }

    if (standard_ == CPP_STANDARD::CPP_98)
        ConstructorImplStr(ss_, prefix_);

    ResetImplStr(ss_, prefix_);
    ss_ << "\n";
    ClearImplStr(ss_, prefix_);
    ss_ << "\n";
    FromPbImplStr(ss_, prefix_);
    ss_ << "\n";
    ToPbImplStr(ss_, prefix_);
    return true;
}

void MessageStruct::ConstructorImplStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << full_name << "::" << name << "()\n";
    ss_ << prefix_ << "{\n";
    // todo
    ss_ << prefix_ << g_indent << "Reset();\n";
    ss_ << prefix_ << "}\n";
}

void MessageStruct::ResetImplStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << "void " << full_name << "::Reset()\n";
    ss_ << prefix_ << "{\n";
    for (auto field : fields)
        field->InitStr(ss_, prefix_ + g_indent);
    ss_ << prefix_ << "}\n";
}

void MessageStruct::ClearImplStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << "void " << full_name << "::Clear()\n";
    ss_ << prefix_ << "{\n";
    ss_ << prefix_ << g_indent << "std::memset(this, 0, sizeof(" << full_name << "));\n";
    ss_ << prefix_ << "}\n";
}

void MessageStruct::FromPbImplStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << "bool " << full_name << "::FromPb(const " << pb_full_name << "& msg_)\n";
    ss_ << prefix_ << "{\n";
    for (auto field : fields)
        field->GetPbStr(ss_, prefix_ + g_indent);
    ss_ << prefix_ << g_indent << "return true;\n";
    ss_ << prefix_ << "}\n";
}

void MessageStruct::ToPbImplStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << "bool " << full_name << "::ToPb(" << pb_full_name << "* msg_) const\n";
    ss_ << prefix_ << "{\n";
    for (auto field : fields)
        field->SetPbStr(ss_, prefix_ + g_indent);
    ss_ << prefix_ << g_indent << "return true;\n";
    ss_ << prefix_ << "}\n";
}

void Field::DeclareStr(stringstream& ss_, const string& prefix_, CPP_STANDARD standard_) const
{
    assert(type_message);
    if (len == 0)
    {
        if (default_value.empty() || standard_ == CPP_STANDARD::CPP_98)
            ss_ << prefix_ << type_message->name << " " << name << ";\n";
        else
            ss_ << prefix_ << type_message->name << " " << name << " = " << default_value << ";\n";
    }
    else
    {
        string max_len_name = string("max_") + name + string("_count");
        if (len <= 0xFF)
        {
            ss_ << prefix_ << "static const uint8_t " << max_len_name << " = " << len << ";\n";
            if (!fixed_len)
                ss_ << prefix_ << "uint8_t " << name << "_count;\n";
        }
        else if (len <= 0xFFFF)
        {
            ss_ << prefix_ << "static const uint16_t " << max_len_name << " = " << len << ";\n";
            if (!fixed_len)
                ss_ << prefix_ << "uint16_t " << name << "_count;\n";
        }
        else if (len <= 0xFFFFFFFF)
        {
            ss_ << prefix_ << "static const uint32_t " << max_len_name << " = " << len << ";\n";
            if (!fixed_len)
                ss_ << prefix_ << "uint32_t " << name << "_count;\n";
        }
        else
        {
            ss_ << prefix_ << "static const uint64_t " << max_len_name << " = " << len << ";\n";
            if (!fixed_len)
                ss_ << prefix_ << "uint64_t " << name << "_count;\n";
        }

        if (standard_ == CPP_STANDARD::CPP_98)
            ss_ << prefix_ << type_message->name << " " << name << "[" << max_len_name << "];\n";
        else
        {
            if (type_message->msg_type == MSG_TYPE::STRING)
            {
                if (default_value.empty())
                    ss_ << prefix_ << type_message->name << " " << name << "[" << max_len_name << "] = {0};\n";
                else
                    ss_ << prefix_ << type_message->name << " " << name << "[" << max_len_name << "] = \""
                        << default_value << "\";\n";
            }
            else
            {
                if (default_value.empty())
                    ss_ << prefix_ << type_message->name << " " << name << "[" << max_len_name << "];\n";
                else
                    ss_ << prefix_ << type_message->name << " " << name << "[" << max_len_name << "] = {"
                        << default_value << "};\n";
            }
        }
    }
}

void Field::SetPbStr(stringstream& ss_, const string& prefix_) const
{
    assert(type_message);
    if (type_message->msg_type == MSG_TYPE::STRING)
        SetStringStr(ss_, prefix_);
    else if (type_message->msg_type == MSG_TYPE::STRUCT)
    {
        if (len == 0)
            SetSingleMessageStr(ss_, prefix_);
        else if (fixed_len)
            SetFixedArrayMessageStr(ss_, prefix_);
        else
            SetArrayMessageStr(ss_, prefix_);
    }
    else
    {
        if (len == 0)
            SetSingleVarStr(ss_, prefix_);
        else if (fixed_len)
            SetFixedArrayVarStr(ss_, prefix_);
        else
            SetArrayVarStr(ss_, prefix_);
    }
}

void Field::SetSingleVarStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << "msg_->set_" << name << "(" << name << ");\n";
}

void Field::SetSingleMessageStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << "if (!(" << name << ".ToPb(msg_->mutable_" << name << "())"
        << "))\n";
    ss_ << prefix_ << g_indent << "return false;\n";
}

void Field::SetArrayVarStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << "msg_->mutable_" << name << "()->Clear();\n";
    ss_ << prefix_ << "for (size_t i = 0; i < " << name << "_count && i < max_" << name << "_count; ++i)\n";
    ss_ << prefix_ << g_indent << "msg_->mutable_" << name << "()->Add(" << name << "[i]);\n";
}

void Field::SetFixedArrayVarStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << "msg_->mutable_" << name << "()->Clear();\n";
    ss_ << prefix_ << "for (size_t i = 0; i < max_" << name << "_count; ++i)\n";
    ss_ << prefix_ << g_indent << "msg_->mutable_" << name << "()->Add(" << name << "[i]);\n";
}

void Field::SetArrayMessageStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << "msg_->mutable_" << name << "()->Clear();\n";
    ss_ << prefix_ << "for (size_t i = 0; i < " << name << "_count && i < max_" << name << "_count; ++i)\n";
    ss_ << prefix_ << "{\n";
    ss_ << prefix_ << g_indent << "if (!(" << name << "[i].ToPb(msg_->mutable_" << name << "()->Add())"
        << "))\n";
    ss_ << prefix_ << g_indent << g_indent << "return false;\n";
    ss_ << prefix_ << "}\n";
}

void Field::SetFixedArrayMessageStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << "msg_->mutable_" << name << "()->Clear();\n";
    ss_ << prefix_ << "for (size_t i = 0; i < max_" << name << "_count; ++i)\n";
    ss_ << prefix_ << "{\n";
    ss_ << prefix_ << g_indent << "if (!(" << name << "[i].ToPb(msg_->mutable_" << name << "()->Add())"
        << "))\n";
    ss_ << prefix_ << g_indent << g_indent << "return false;\n";
    ss_ << prefix_ << "}\n";
}

void Field::SetStringStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << "msg_->set_" << name << "(" << name << ", std::strlen(" << name << "));\n";
}

void Field::GetPbStr(stringstream& ss_, const string& prefix_) const
{
    assert(type_message);
    if (type_message->msg_type == MSG_TYPE::STRING)
        GetStringStr(ss_, prefix_);
    else if (type_message->msg_type == MSG_TYPE::STRUCT)
    {
        if (len == 0)
            GetSingleMessageStr(ss_, prefix_);
        else if (fixed_len)
            GetFixedArrayMessageStr(ss_, prefix_);
        else
            GetArrayMessageStr(ss_, prefix_);
    }
    else
    {
        if (len == 0)
            GetSingleVarStr(ss_, prefix_);
        else if (fixed_len)
            GetFixedArrayVarStr(ss_, prefix_);
        else
            GetArrayVarStr(ss_, prefix_);
    }
}

void Field::GetSingleVarStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << name << " = msg_." << name << "();\n";
}

void Field::GetSingleMessageStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << "if (msg_.has_" << name << "())\n";
    ss_ << prefix_ << "{\n";
    ss_ << prefix_ << g_indent << "if (!(" << name << ".FromPb(msg_." << name << "())"
        << "))\n";
    ss_ << prefix_ << g_indent << g_indent << "return false;\n";
    ss_ << prefix_ << "}\n";
}

void Field::GetArrayVarStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << "if (msg_." << name << "_size() > max_" << name << "_count)\n";
    ss_ << prefix_ << g_indent << name << "_count = max_" << name << "_count;\n";
    ss_ << prefix_ << "else\n";
    ss_ << prefix_ << g_indent << name << "_count = msg_." << name << "_size();\n";
    ss_ << prefix_ << "for (size_t i = 0; i < " << name << "_count; ++i)\n";
    ss_ << prefix_ << g_indent << name << "[i] = msg_." << name << "(i);\n";
}

void Field::GetFixedArrayVarStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << "size_t _" << name << "_count_ = msg_." << name << "_size();\n";
    ss_ << prefix_ << "if (msg_." << name << "_size() > max_" << name << "_count)\n";
    ss_ << prefix_ << g_indent << "_" << name << "_count_ = max_" << name << "_count;\n";
    ss_ << prefix_ << "for (size_t i = 0; i < _" << name << "_count_; ++i)\n";
    ss_ << prefix_ << g_indent << name << "[i] = msg_." << name << "(i);\n";
}

void Field::GetArrayMessageStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << "if (msg_." << name << "_size() > max_" << name << "_count)\n";
    ss_ << prefix_ << g_indent << name << "_count = max_" << name << "_count;\n";
    ss_ << prefix_ << "else\n";
    ss_ << prefix_ << g_indent << name << "_count = msg_." << name << "_size();\n";
    ss_ << prefix_ << "for (size_t i = 0; i < " << name << "_count; ++i)\n";
    ss_ << prefix_ << "{\n";
    ss_ << prefix_ << g_indent << "if (!(" << name << "[i].FromPb(msg_." << name << "(i))"
        << "))\n";
    ss_ << prefix_ << g_indent << g_indent << "return false;\n";
    ss_ << prefix_ << "}\n";
}

void Field::GetFixedArrayMessageStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << "size_t _" << name << "_count_ = msg_." << name << "_size();\n";
    ss_ << prefix_ << "if (msg_." << name << "_size() > max_" << name << "_count)\n";
    ss_ << prefix_ << g_indent << "_" << name << "_count_ = max_" << name << "_count;\n";
    ss_ << prefix_ << "for (size_t i = 0; i < _" << name << "_count_; ++i)\n";
    ss_ << prefix_ << "{\n";
    ss_ << prefix_ << g_indent << "if (!(" << name << "[i].FromPb(msg_." << name << "(i))"
        << "))\n";
    ss_ << prefix_ << g_indent << g_indent << "return false;\n";
    ss_ << prefix_ << "}\n";
}

void Field::GetStringStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << "if (msg_.has_" << name << "())\n";
    ss_ << prefix_ << "{\n";
    ss_ << prefix_ << g_indent << "if (msg_." << name << "().length() > max_" << name << "_count)\n";
    ss_ << prefix_ << g_indent << g_indent << "return false;\n";
    ss_ << prefix_ << g_indent << "msg_." << name << "().copy(" << name << ", msg_." << name << "().length());\n";
    ss_ << prefix_ << "}\n";
}

void Field::InitStr(stringstream& ss_, const string& prefix_) const
{
    assert(type_message);
    if (type_message->msg_type == MSG_TYPE::STRING)
        InitStringStr(ss_, prefix_);
    else if (type_message->msg_type == MSG_TYPE::STRUCT)
    {
        if (len == 0)
            InitSingleMessageStr(ss_, prefix_);
        else if (fixed_len)
            InitFixedArrayMessageStr(ss_, prefix_);
        else
            InitArrayMessageStr(ss_, prefix_);
    }
    else
    {
        if (len == 0)
            InitSingleVarStr(ss_, prefix_);
        else if (fixed_len)
            InitFixedArrayVarStr(ss_, prefix_);
        else
            InitArrayVarStr(ss_, prefix_);
    }
}

void Field::InitSingleVarStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << name << " = " << default_value << ";\n";
}

void Field::InitSingleMessageStr(stringstream& ss_, const string& prefix_) const
{
    ss_ << prefix_ << name << ".Clear();\n";
}

void Field::InitArrayVarStr(stringstream& ss_, const string& prefix_) const
{
    // todo
}

void Field::InitFixedArrayVarStr(stringstream& ss_, const string& prefix_) const
{
    // todo
}

void Field::InitArrayMessageStr(stringstream& ss_, const string& prefix_) const
{
    // todo
}

void Field::InitFixedArrayMessageStr(stringstream& ss_, const string& prefix_) const
{
    // todo
}

void Field::InitStringStr(stringstream& ss_, const string& prefix_) const
{
    if (default_value.empty())
    {
        ss_ << prefix_ << name << "[0] = '\\0';\n";
    }
    else
    {
        ss_ << prefix_ << "std::string __tmp_" << name << "__{\"" << default_value << "\"};\n";
        ss_ << prefix_ << "__tmp_" << name << "__.copy(" << name << ", __tmp_" << name << "__.size());\n";
        ss_ << prefix_ << name << "[__tmp_" << name << "__.size()] = '\\0';\n";
    }
}

}  // namespace Pepper
