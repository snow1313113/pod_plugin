/*
 * * file name: parser.h
 * * description: ...
 * * author: lemonxu
 * * create time:2019 5月 21
 * */

#ifndef _PARSER_H_
#define _PARSER_H_

#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/compiler/plugin.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <map>
#include <string>
#include <vector>
#include "grammar.h"

using std::map;
using std::string;
using std::vector;
using namespace google::protobuf::compiler;

namespace Pepper
{
class PodMessage
{
public:
    PodMessage(const ::google::protobuf::FileDescriptor* file_) : m_file(file_) { Prepare(); }
    ~PodMessage();

    bool Parse(const string& params_str_);

    string GetHeaderPrologue() const;
    string GetHeaderIncludeFile() const;
    string GetHeaderDecl() const;
    string GetHeaderEpilogue() const;

    string GetSourcePrologue() const;
    string GetSourceIncludeFile() const;
    string GetSourceImpl() const;
    string GetSourceEpilogue() const;

    bool HasPodMessage() const;
    string base_file_name() const;
    const char* err_msg() const;

private:
    void Prepare();
    void InitBaseMessage();
    void CollectImportMsg(const ::google::protobuf::Descriptor* desc_, const BaseStruct* parent_ = nullptr);
    MessageStruct* ParseMessage(const ::google::protobuf::Descriptor* desc_, const MessageStruct* parent_ = nullptr);
    Field* ParseField(const ::google::protobuf::FieldDescriptor* desc_);
    EnumStruct* ParseEnum(const ::google::protobuf::EnumDescriptor* desc_, const MessageStruct* parent_ = nullptr);

private:
    CPP_STANDARD m_standard;
    const ::google::protobuf::FileDescriptor* m_file;
    string m_base_file_name;
    vector<const ::google::protobuf::Descriptor*> m_message_vec;
    SyntaxTree m_tree;
    // must be clear
    map<string, BaseStruct*> m_message_mgr;
    // must be clear
    vector<Field*> m_fields;
    string m_err_msg;
};

}  // namespace Pepper

#endif
