#include "syscall_cpp_wrapper.h"
#include "utils.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

void execv_cpp_wrapper(const std::string executable, const std::vector<std::string> &args) {
    std::cout << "Executing executable: \"" << executable << "\"" << std::endl;
    std::cout << "Arguments: ";
    for (int i = 0; i < args.size(); i++) {
        std::cout << "\"" << args[i] << "\" ";
    }
    std::cout << std::endl;
    char **args_to_syscall;

    if (args.size() == 0) {
        std::cerr << "args must have at least one argument - the absolute path to executable path itself" << std::endl;
        return;
    }
    vector_to_null_term_char_pointer_list(args, &args_to_syscall);
    pid_t child_pid = fork();
    if (child_pid == -1) {
        std::cerr << "Failed to fork" << std::endl;
        return;
    }
    if (child_pid == 0) {
        execv(executable.c_str(), args_to_syscall);
        std::cerr << "Failed to execute executable: \"" << executable << "\"" << std::endl;
        return;
    }
    wait(NULL);
    free_char_pointer_list(&args_to_syscall, args.size() + 1);
}
