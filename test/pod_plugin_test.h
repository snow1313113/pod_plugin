/*
 * * file name: pod_plugin_test.h
 * * description: ...
 * * author: snow
 * * create time:2019  6 05
 * */

#ifndef _POD_PLUGIN_TEST_H_
#define _POD_PLUGIN_TEST_H_

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include "gtest/gtest.h"
#include "test.pod.h"

using std::map;
using std::set;
using namespace Test;

TEST(PodPluginTest, to_pb)
{
    PODBaseMsg pod_base_msg;
    pod_base_msg.id = 10;

    BaseMsg base_msg;
    pod_base_msg.ToPb(&base_msg);

    EXPECT_EQ(base_msg.id(), pod_base_msg.id);
    EXPECT_EQ(pod_base_msg.id, 10);
}

#endif
