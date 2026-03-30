#include "syscall_cpp_wrapper.h"
#include "utils.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int execv_cpp_wrapper(const std::string executable, const std::vector<std::string> &args, int *status) {
    std::cout << "Executing executable: \"" << executable << "\"" << std::endl;
    std::cout << "Arguments: ";
    for (int i = 0; i < args.size(); i++) {
        std::cout << "\"" << args[i] << "\" ";
    }
    std::cout << std::endl;

    if (args.size() == 0) {
        std::cerr << "args must have at least one argument - the absolute path to executable path itself" << std::endl;
        return EINVAL;
    }
    size_t args_to_syscall_len;
    auto args_to_syscall = vector_to_null_term_char_pointer_list(args, &args_to_syscall_len);
    pid_t child_pid = fork();
    if (child_pid == -1) {
        std::cerr << "Failed to fork" << std::endl;
        free_char_pointer_list((char ***)&args_to_syscall, args_to_syscall_len);
        free(args_to_syscall);
        return ECHILD;
    }
    if (child_pid == 0) {
        execv(executable.c_str(), args_to_syscall);
        std::cerr << "Failed to execute executable: \"" << executable << "\"" << std::endl;
        free_char_pointer_list((char ***)&args_to_syscall, args_to_syscall_len);
        free(args_to_syscall);
        return ENOEXEC;
    }
    waitpid(child_pid, status, 0);
    free_char_pointer_list((char ***)&args_to_syscall, args_to_syscall_len);
    free(args_to_syscall);
    return 0;
}
