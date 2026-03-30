#ifndef KSH_SYSCALL_CPP_WRAPPER_H
#define KSH_SYSCALL_CPP_WRAPPER_H

#include <string>
#include <vector>

int execv_cpp_wrapper(const std::string executable, const std::vector<std::string> &args, int *status = nullptr);

#endif // KSH_SYSCALL_CPP_WRAPPER_H