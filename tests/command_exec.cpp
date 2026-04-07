#include <gtest/gtest.h>
#include "command_exec.h"

TEST(CommandExecTest, ExecuteCommand) {
    bool properly_quoted;
    int status = execute_command("ls", properly_quoted);
    EXPECT_TRUE(properly_quoted);
    EXPECT_EQ(status, 0);
    status = execute_command("ls -l", properly_quoted);
    EXPECT_EQ(status, 0);
    status = execute_command("lslsdsds", properly_quoted);
    EXPECT_FALSE(properly_quoted);
    EXPECT_EQ(status, ENOENT);
    status = execute_command("", properly_quoted);
    EXPECT_EQ(status, EINVAL);
    status = execute_command(" ", properly_quoted);
    EXPECT_EQ(status, EINVAL);
    status = execute_command("  ", properly_quoted);
    EXPECT_FALSE(properly_quoted);
    EXPECT_EQ(status, EINVAL);
    status = execute_command("\"ls\"", properly_quoted);
    EXPECT_TRUE(properly_quoted);
    EXPECT_EQ(status, 0);
    status = execute_command("\"ls", properly_quoted);
    EXPECT_FALSE(properly_quoted);
    EXPECT_EQ(status, EINVAL);
}