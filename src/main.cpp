#include <iostream>
#include "code_gen.h"

int main(int argc, char* argv[])
{
    PodCodeGenerator generator;
    return google::protobuf::compiler::PluginMain(argc, argv, &generator);
}
