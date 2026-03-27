#include <iostream>
#include <fstream>
#include <errno.h>
#include <unistd.h>
#include <cstdlib>


int main() {
    // Find host name first
    std::ifstream host_name_file("/etc/hostname");
    if (!host_name_file.is_open()) {
        std::cerr << "Didn't find hostname" << std::endl;
        return ENOENT;
    }
    std::string host_name;
    host_name_file >> host_name;
    // Find what is this user name. For now, we simply inherit the uid and gid from calling process, which is parent shell
    // @todo: if let user execute this shell directly (from /etc/passwd), find the way to know what exactly user
    // this shell should serve. 
    uid_t process_uid = getuid();
    gid_t process_gid = getgid();
    std::ifstream passwd_file("/etc/passwd");
    std::string line;
    // Typical line of /etc/passwd looks like this:
    // root:x:0:0:root:/root:/bin/bash
    std::string username;
    while (std::getline(passwd_file, line)) {
        std::string parsed_username = line.substr(0, line.find(":"));
        auto begin_of_uid = line.find(":") + 3; // drop the next ":x:" characters
        auto end_of_uid = line.find(":", begin_of_uid);
        auto begin_of_gid = end_of_uid + 1; // skip the current ":" character
        auto end_of_gid = line.find(":", begin_of_gid);
        auto uid_str = line.substr(begin_of_uid, end_of_uid - begin_of_uid);
        auto gid_str = line.substr(begin_of_gid, end_of_gid - begin_of_gid);
        auto parsed_uid = atoi(uid_str.c_str());
        auto parsed_gid = atoi(gid_str.c_str());
        if ((parsed_uid != process_uid) || (parsed_gid != process_gid)) {
            continue;
        }
        username = parsed_username;
    }
    std::string red_color_code = "\033[31m";
    std::string yellow_color_code = "\033[33m";
    std::string white_color_code = "\033[0m";
    std::cout << red_color_code << username << " " << yellow_color_code << host_name << white_color_code << " > " << std::endl;
    return 0;
}
