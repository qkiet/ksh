#ifndef KSH_DEBUG_LOGGER_H
#define KSH_DEBUG_LOGGER_H

#include <iostream>
#include <fstream>

class DebugLogger {
  public:
    static DebugLogger &get() {
        static DebugLogger inst;
        return inst;
    }

    static void configure(bool enabled) { get().mEnabled = enabled; }
    /**
     * Set the output file for the debug logger. Note that after calling this function,
     * the output will be written to the file instead of the console. If the file is not
     * opened successfully, the output will be written to the console instead.

     * @param path: The path to the output file.
     */
    static void set_output_file(const std::string &path) {
        get().mOutputFileStream.open(path);
        if (!get().mOutputFileStream.is_open()) {
            std::cerr << "Failed to open output file " << path << std::endl;
        }
    }
    template <typename... Args>
    static void print(Args &&...args) {
        if (!get().mEnabled) {
            return;
        }
#ifndef TAG
        if (get().mOutputFileStream.is_open()) {
            (get().mOutputFileStream << ... << std::forward<Args>(args)) << std::endl;
        } else {
            (std::cout << ... << std::forward<Args>(args)) << std::endl;
        }
#else
        if (get().mOutputFileStream.is_open()) {
            (get().mOutputFileStream << "[" << TAG << "] " << ... << std::forward<Args>(args)) << std::endl;
        } else {
            (std::cout << "[" << TAG << "] " << ... << std::forward<Args>(args)) << std::endl;
        }
#endif
    }

    template <typename... Args>
    static void error(Args &&...args) {
        if (!get().mEnabled) {
            return;
        }
#ifndef TAG
        if (get().mOutputFileStream.is_open()) {
            (get().mOutputFileStream << ... << std::forward<Args>(args)) << std::endl;
        } else {
            (std::cout << ... << std::forward<Args>(args)) << std::endl;
        }
#else
        if (get().mOutputFileStream.is_open()) {
            (get().mOutputFileStream << "[" << TAG << "] " << ... << std::forward<Args>(args)) << std::endl;
        } else {
            (std::cout << "[" << TAG << "] " << ... << std::forward<Args>(args)) << std::endl;
        }
#endif
    }

  private:
    DebugLogger() = default;
    DebugLogger(const DebugLogger &) = delete;
    DebugLogger &operator=(const DebugLogger &) = delete;
    bool mEnabled = true;
    std::ofstream mOutputFileStream;
};

#endif // KSH_DEBUG_LOGGER_H
