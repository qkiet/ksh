#include <gtest/gtest.h>
#include "syscall_cpp_wrapper.h"
#include <vector>

TEST(SyscallCppWrapperTest, ExecvCppWrapper) {
  int status;
  int ret;
  ret = execv_cpp_wrapper("gibberish", std::vector<std::string>({"gibberish"}), &status);
  EXPECT_EQ(ret, ENOENT);
}