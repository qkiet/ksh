#include <gtest/gtest.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstring>
#include <limits.h>
#include <iostream>


class IntegTest : public ::testing::Test {
protected:
    char m_shell_output_buffer[1024];
    pid_t m_shell_pid = -1;
    int m_input_fd = -1;
    int m_output_fd = -1;
    int m_to_shell_pipe[2];
    int m_from_shell_pipe[2];
    const ::testing::TestInfo* m_test_info = ::testing::UnitTest::GetInstance()->current_test_info();
    void SetUp() override {
        char current_directory[PATH_MAX];
        memset(current_directory, 0, PATH_MAX);
        if (getcwd(current_directory, PATH_MAX) == nullptr) {
            FAIL() << "Failed to get current directory";
        }
        char shell_path[PATH_MAX];
        memset(shell_path, 0, PATH_MAX);
        strncpy(shell_path, current_directory, PATH_MAX);
        // @todo: use better way to get the path of shell
        strncat(shell_path, "/build/ksh", PATH_MAX);

        
        if (pipe(m_to_shell_pipe) == -1) {
            FAIL() << "Failed to create stdin pipe";
        }
        if (pipe(m_from_shell_pipe) == -1) {
            close(m_to_shell_pipe[0]);
            close(m_to_shell_pipe[1]);
            FAIL() << "Failed to create stdout pipe";
        }

        char test_log_file[PATH_MAX];
        memset(test_log_file, 0, PATH_MAX);
        snprintf(test_log_file, PATH_MAX, "ksh_integ_test_%s_%s.log", m_test_info->test_suite_name(), m_test_info->name());

        m_shell_pid = fork();
        if (m_shell_pid == -1) {
            FAIL() << "Failed to fork child process";
        }
        if (m_shell_pid == 0) {
            close(m_to_shell_pipe[1]);
            dup2(m_to_shell_pipe[0], STDIN_FILENO);
            close(m_to_shell_pipe[0]);

            close(m_from_shell_pipe[0]);
            dup2(m_from_shell_pipe[1], STDOUT_FILENO);
            close(m_from_shell_pipe[1]);

            char *args[] = {shell_path, (char *)"-d", (char *)"-l", test_log_file, nullptr};
            execv(shell_path, args);
            _exit(1);
        }

        close(m_to_shell_pipe[0]);
        close(m_from_shell_pipe[1]);
        m_input_fd = m_to_shell_pipe[1];
        m_output_fd = m_from_shell_pipe[0];
        std::cout << "Done setting up for suite \"" << m_test_info->test_suite_name() << "\" and test case \"" << m_test_info->name() << "\"" << std::endl;
    }

    void TearDown() override {
        if (m_input_fd != -1) close(m_input_fd);
        if (m_output_fd != -1) close(m_output_fd);
        if (m_shell_pid > 0) {
            kill(m_shell_pid, SIGTERM);
            waitpid(m_shell_pid, nullptr, 0);
        }
        std::cout << "Done tearing down for suite \"" << m_test_info->test_suite_name() << "\" and test case \"" << m_test_info->name() << "\"" << std::endl;
    }

    ssize_t read_shell_output() {
        memset(m_shell_output_buffer, 0, sizeof(m_shell_output_buffer));
        return read(m_output_fd, m_shell_output_buffer, sizeof(m_shell_output_buffer) - 1);
    }   

    ssize_t write_command_to_shell_with_newline(const std::string &command) {
        return write(m_input_fd, command.c_str(), command.size()) + write(m_input_fd, "\n", 1);
    }
};

TEST_F(IntegTest, SimpleCommandExecution) {
    read_shell_output(); // Skip the prompt
    write_command_to_shell_with_newline("echo hello");
    read_shell_output();
    EXPECT_EQ(std::string(m_shell_output_buffer), "hello\n");
}

TEST_F(IntegTest, ExitCommandExecution) {
    write_command_to_shell_with_newline("exit");
    int status;
    waitpid(m_shell_pid, &status, 0);
    EXPECT_TRUE(WIFEXITED(status));
    EXPECT_EQ(WEXITSTATUS(status), 0);
}

TEST_F(IntegTest, SendEOFToShell) {
    // Closing write-end of the pipe "to shell" makes kernel see zero writers remaining.
    // Shell's std::getline(std::cin, ...) calls read() on the "to shell" pipe's read-end,
    // gets 0 bytes (EOF). std::cin.eof() becomes true, shell exits cleanly.
    close(m_input_fd);
    int status;
    waitpid(m_shell_pid, &status, 0);
    EXPECT_TRUE(WIFEXITED(status));
    EXPECT_EQ(WEXITSTATUS(status), 0);
}

TEST_F(IntegTest, QuotedCommandExecution) {
    read_shell_output(); // Skip the prompt
    write_command_to_shell_with_newline("echo \"hello\"");
    read_shell_output();
    EXPECT_EQ(std::string(m_shell_output_buffer), "hello\n");
    read_shell_output(); // Skip the prompt
    write_command_to_shell_with_newline("echo hello\"world\"");
    read_shell_output();
    EXPECT_EQ(std::string(m_shell_output_buffer), "helloworld\n");
    read_shell_output(); // Skip the prompt
    write_command_to_shell_with_newline("echo \"hello world\"");
    read_shell_output();
    EXPECT_EQ(std::string(m_shell_output_buffer), "hello world\n");
}
