/*
 * * file name: code_gen.h
 * * description: ...
 * * author: lemonxu
 * * create time:2019 5月 20
 * */

#ifndef _CODE_GEN_H_
#define _CODE_GEN_H_

#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/compiler/plugin.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <map>
#include <string>
#include <vector>
#include "parser.h"

using std::map;
using std::string;
using std::vector;
using namespace google::protobuf::compiler;
using namespace Pepper;

class PodCodeGenerator : public CodeGenerator
{
public:
    virtual ~PodCodeGenerator(){};
    virtual bool Generate(const ::google::protobuf::FileDescriptor* file_, const string& parameter,
                          GeneratorContext* context, string* error) const
    {
        PodMessage pod_msg{file_};

        if (!pod_msg.HasPodMessage())
            return true;

        if (!pod_msg.Parse(parameter))
        {
            *error = pod_msg.err_msg();
            return false;
        }

        std::unique_ptr<::google::protobuf::io::ZeroCopyOutputStream> header_output(
            context->Open(pod_msg.base_file_name() + ".pod.h"));
        std::unique_ptr<::google::protobuf::io::ZeroCopyOutputStream> source_output(
            context->Open(pod_msg.base_file_name() + ".pod.cc"));

        ::google::protobuf::io::CodedOutputStream header_out(header_output.get());
        ::google::protobuf::io::CodedOutputStream source_out(source_output.get());

        string header_str = pod_msg.GetHeaderPrologue() + pod_msg.GetHeaderIncludeFile() + pod_msg.GetHeaderDecl() +
                            pod_msg.GetHeaderEpilogue();
        header_out.WriteString(header_str);

        string source_str = pod_msg.GetSourcePrologue() + pod_msg.GetSourceIncludeFile() + pod_msg.GetSourceImpl() +
                            pod_msg.GetSourceEpilogue();
        source_out.WriteString(source_str);

        return true;
    }
};

#endif
