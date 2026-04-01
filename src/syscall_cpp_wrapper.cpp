#include "syscall_cpp_wrapper.h"
#include "utils.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int execv_cpp_wrapper(const std::string &absolute_executable_path, const std::vector<std::string> &args, int *status) {
    std::cout << "Executing executable: \"" << absolute_executable_path << "\"" << std::endl;
    std::cout << "Arguments: ";
    for (int i = 0; i < args.size(); i++) {
        std::cout << "\"" << args[i] << "\" ";
    }
    std::cout << std::endl;

    size_t args_to_syscall_len;
    char *args_to_syscall[args.size() + 2]; // +2 for the executable path and the null terminator
    args_to_syscall[0] = (char *) absolute_executable_path.c_str();
    for (int i = 0; i < args.size(); i++) {
        args_to_syscall[i + 1] = (char *) args[i].c_str();
    }
    args_to_syscall[args.size() + 1] = nullptr;
    for (int i = 0; i < args.size() + 1; i++) {
        std::cout << "args_to_syscall[" << i << "] = " << args_to_syscall[i] << std::endl;
    }
    pid_t child_pid = fork();
    if (child_pid == -1) {
        std::cerr << "Failed to fork" << std::endl;
        return ECHILD;
    }
    if (child_pid == 0) {
        execv(absolute_executable_path.c_str(), args_to_syscall);
        std::cerr << "Failed to execute executable: \"" << absolute_executable_path << "\"" << std::endl;
        _exit(ENOEXEC);
    }

    int wait_status;
    if (waitpid(child_pid, &wait_status, 0) == -1) {
        std::cerr << "Failed to wait for child process" << std::endl;
        return ECHILD;
    }
    if (WIFEXITED(wait_status) && WEXITSTATUS(wait_status) == ENOEXEC) {
        std::cerr << "Executable: \"" << absolute_executable_path << "\" exited with status " << WEXITSTATUS(wait_status) << std::endl;
        return ENOEXEC;
    }
    if (status != nullptr) {
        *status = wait_status;
    }
    return 0;
}
