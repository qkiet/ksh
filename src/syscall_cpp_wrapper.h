#ifndef KSH_SYSCALL_CPP_WRAPPER_H
#define KSH_SYSCALL_CPP_WRAPPER_H

#include <string>
#include <vector>

/**
 * Wrapper for the execv system call.
 * @param absolute_executable_path: The absolute path to the executable.
 * @param args: The arguments to pass to the executable.
 * @param status: The status of the executable. It is set to the status of the child process.
 * @return:
 * - 0 on success
 * - ENOEXEC on failure
 * - ECHILD on failure to fork
 */
int execv_cpp_wrapper(const std::string &absolute_executable_path, const std::vector<std::string> &args, int *status = nullptr);

#endif // KSH_SYSCALL_CPP_WRAPPER_H